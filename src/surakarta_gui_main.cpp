#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <optional>
#include <string>
#include <thread>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "surakarta_dev_session.h"

namespace {

using surakarta::bitboard::Color;
using surakarta::bitboard::LegalTargetInfo;
using surakarta::bitboard::MakeSquare;
using surakarta::bitboard::Move;
using surakarta::bitboard::PositionAdapter;
using surakarta::bitboard::SearchLimits;
using surakarta::bitboard::Square;
using surakarta::devtools::DevelopmentSession;

constexpr float kPi = 3.14159265358979323846f;
constexpr float kBoardOuterTrackPaddingSteps = 2.15f;
constexpr float kBoardLabelPaddingSteps = 0.55f;
constexpr float kBoardGridSpanSteps = 5.0f;
constexpr float kBoardVisualSpanSteps = kBoardGridSpanSteps + kBoardOuterTrackPaddingSteps * 2.0f;
constexpr float kBoardPanelSpanSteps = kBoardVisualSpanSteps + kBoardLabelPaddingSteps * 2.0f;

int DefaultThreadCount() {
    const auto hardware_threads = std::thread::hardware_concurrency();
    const auto suggested = hardware_threads == 0 ? 1u : std::max(1u, hardware_threads / 2);
    return static_cast<int>(suggested);
}

struct StartupOptions {
    bool smoke_test{false};
    bool smoke_test_init{false};
    bool smoke_test_geometry{false};
    std::string file_name;
    int depth{4};
    int threads{DefaultThreadCount()};
    Color human_color{Color::Black};
};

struct Palette {
    ImU32 background{IM_COL32(241, 235, 220, 255)};
    ImU32 board_panel{IM_COL32(224, 215, 196, 255)};
    ImU32 grid{IM_COL32(80, 70, 55, 255)};
    ImU32 quiet_target{IM_COL32(54, 119, 170, 180)};
    ImU32 capture_target{IM_COL32(186, 74, 47, 220)};
    ImU32 selected{IM_COL32(218, 172, 52, 255)};
    ImU32 hover_path{IM_COL32(240, 133, 52, 255)};
    ImU32 black_piece{IM_COL32(36, 34, 31, 255)};
    ImU32 white_piece{IM_COL32(245, 242, 234, 255)};
    ImU32 white_piece_outline{IM_COL32(84, 78, 67, 255)};
    ImU32 point{IM_COL32(110, 101, 85, 255)};
};

struct BoardLayout {
    ImVec2 panel_min;
    ImVec2 panel_max;
    ImVec2 grid_origin;
    ImVec2 board_center;
    float grid_step{0.0f};
    float outer_track_padding{0.0f};
    float label_padding{0.0f};
    float board_span{0.0f};
    float interaction_radius{0.0f};
    float piece_radius{0.0f};
};

struct CornerLoopSpec {
    int start_angle{0};
    int end_angle{0};
    bool clockwise{false};
};

struct ArcSweep {
    float start_radians{0.0f};
    float end_radians{0.0f};
};

struct SidebarState {
    int depth{4};
    int threads{1};
    Color human_color{Color::Black};
    char file_name[512]{};
    std::string status_message;
};

void ApplySearchSettings(DevelopmentSession& session, const SidebarState& state) {
    auto limits = SearchLimits{};
    limits.max_depth = std::max(1, state.depth);
    limits.threads = std::max(1, state.threads);
    session.SetSearchLimits(limits);
}

const char* ColorLabel(Color color) {
    return color == Color::White ? "White" : "Black";
}

std::string FormatSquare(Square square) {
    const auto position = PositionAdapter::ToLegacyPosition(square);
    return "(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
}

std::string FormatMove(Move move) {
    if (!move.IsValid()) {
        return "n/a";
    }
    auto text = FormatSquare(move.from) + " -> " + FormatSquare(move.to);
    if (move.IsCapture()) {
        text += " x";
    }
    return text;
}

std::string FormatPrincipalVariation(
    const surakarta::bitboard::Position& root,
    const std::array<Move, surakarta::bitboard::kMaxPly>& pv,
    int pv_length) {
    if (pv_length <= 0) {
        return "n/a";
    }

    auto working = root;
    auto text = std::string{};
    const auto safe_length = std::clamp(pv_length, 0, surakarta::bitboard::kMaxPly);
    for (int ply = 0; ply < safe_length; ++ply) {
        const auto move = pv[ply];
        if (!move.IsValid()) {
            break;
        }
        if (!text.empty()) {
            text += " | ";
        }
        text += FormatMove(move);
        auto undo = surakarta::bitboard::Undo{};
        surakarta::bitboard::MakeMove(working, move, undo);
    }
    return text.empty() ? "n/a" : text;
}

void GlfwErrorCallback(int error, const char* description) {
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

ImVec2 BoardPoint(const BoardLayout& layout, int x, int y) {
    return ImVec2(layout.grid_origin.x + layout.grid_step * static_cast<float>(x),
                  layout.grid_origin.y + layout.grid_step * static_cast<float>(y));
}

float AngleToRadians(int angle) {
    return static_cast<float>(angle) * (kPi * 0.5f);
}

CornerLoopSpec CornerLoopForCorner(std::size_t corner_index) {
    constexpr auto kCornerLoops = std::array<CornerLoopSpec, 4>{
        CornerLoopSpec{0, 1, true},
        CornerLoopSpec{1, 2, true},
        CornerLoopSpec{2, 3, true},
        CornerLoopSpec{3, 0, true},
    };
    return kCornerLoops.at(corner_index);
}

ArcSweep ResolveArcSweep(int start_angle, int end_angle, bool clockwise) {
    auto start = AngleToRadians(start_angle);
    auto end = AngleToRadians(end_angle);
    if (clockwise) {
        if (end > start) {
            end -= kPi * 2.0f;
        }
    } else if (end < start) {
        end += kPi * 2.0f;
    }
    return ArcSweep{start, end};
}

void StrokeArc(ImDrawList* draw_list,
               ImVec2 center,
               float radius,
               int start_angle,
               int end_angle,
               bool clockwise,
               ImU32 color,
               float thickness) {
    const auto sweep = ResolveArcSweep(start_angle, end_angle, clockwise);
    const int segments =
        std::max(12, static_cast<int>(std::abs(sweep.end_radians - sweep.start_radians) / (kPi * 0.5f) * 18.0f));
    draw_list->PathClear();
    for (int i = 0; i <= segments; ++i) {
        const float t = sweep.start_radians +
                        (sweep.end_radians - sweep.start_radians) *
                            (static_cast<float>(i) / static_cast<float>(segments));
        draw_list->PathLineTo(ImVec2(center.x + std::cos(t) * radius, center.y + std::sin(t) * radius));
    }
    draw_list->PathStroke(color, ImDrawFlags_None, thickness);
}

BoardLayout MakeBoardLayout(ImVec2 top_left, ImVec2 available_size) {
    const float visual_size = std::min(available_size.x, available_size.y);
    const float grid_step = visual_size / kBoardPanelSpanSteps;
    const float outer_track_padding = grid_step * kBoardOuterTrackPaddingSteps;
    const float label_padding = grid_step * kBoardLabelPaddingSteps;
    const float board_span = grid_step * kBoardGridSpanSteps + outer_track_padding * 2.0f;
    const float panel_size = board_span + label_padding * 2.0f;
    const auto centered_offset = ImVec2(std::max(0.0f, (available_size.x - panel_size) * 0.5f),
                                        std::max(0.0f, (available_size.y - panel_size) * 0.5f));
    BoardLayout layout{};
    layout.panel_min = ImVec2(top_left.x + centered_offset.x, top_left.y + centered_offset.y);
    layout.panel_max = ImVec2(layout.panel_min.x + panel_size, layout.panel_min.y + panel_size);
    layout.grid_origin =
        ImVec2(layout.panel_min.x + label_padding + outer_track_padding, layout.panel_min.y + label_padding + outer_track_padding);
    layout.board_center = ImVec2(layout.panel_min.x + panel_size * 0.5f, layout.panel_min.y + panel_size * 0.5f);
    layout.grid_step = grid_step;
    layout.outer_track_padding = outer_track_padding;
    layout.label_padding = label_padding;
    layout.board_span = board_span;
    layout.interaction_radius = layout.grid_step * 0.36f;
    layout.piece_radius = layout.grid_step * 0.28f;
    return layout;
}

std::optional<Square> DetectSquare(const BoardLayout& layout, ImVec2 mouse) {
    const auto local_x = (mouse.x - layout.grid_origin.x) / layout.grid_step;
    const auto local_y = (mouse.y - layout.grid_origin.y) / layout.grid_step;
    const auto x = static_cast<int>(std::lround(local_x));
    const auto y = static_cast<int>(std::lround(local_y));
    if (x < 0 || x >= surakarta::bitboard::kBoardSize || y < 0 || y >= surakarta::bitboard::kBoardSize) {
        return std::nullopt;
    }
    const auto snapped = BoardPoint(layout, x, y);
    const float dx = mouse.x - snapped.x;
    const float dy = mouse.y - snapped.y;
    if (dx * dx + dy * dy > layout.interaction_radius * layout.interaction_radius) {
        return std::nullopt;
    }
    return MakeSquare(x, y);
}

void DrawCornerLoops(ImDrawList* draw_list, const BoardLayout& layout, ImU32 color, float thickness) {
    const auto corners = std::array<ImVec2, 4>{
        BoardPoint(layout, 0, 0),
        BoardPoint(layout, 5, 0),
        BoardPoint(layout, 5, 5),
        BoardPoint(layout, 0, 5),
    };

    for (std::size_t i = 0; i < corners.size(); ++i) {
        const auto spec = CornerLoopForCorner(i);
        for (int radius = 1; radius <= 2; ++radius) {
            StrokeArc(draw_list,
                      corners[i],
                      layout.grid_step * static_cast<float>(radius),
                      spec.start_angle,
                      spec.end_angle,
                      spec.clockwise,
                      color,
                      thickness);
        }
    }
}

bool HasOuterTrackPadding(const BoardLayout& layout) {
    const auto top_left = BoardPoint(layout, 0, 0);
    const auto bottom_right = BoardPoint(layout, 5, 5);
    const float required_padding = layout.grid_step * 2.0f;
    const float available_left = top_left.x - layout.panel_min.x - layout.label_padding;
    const float available_top = top_left.y - layout.panel_min.y - layout.label_padding;
    const float available_right = layout.panel_max.x - bottom_right.x - layout.label_padding;
    const float available_bottom = layout.panel_max.y - bottom_right.y - layout.label_padding;
    return available_left >= required_padding &&
           available_top >= required_padding &&
           available_right >= required_padding &&
           available_bottom >= required_padding;
}

bool UsesOuterCornerLoopSweeps() {
    constexpr float expected_sweep = kPi * 1.5f;
    constexpr float tolerance = 0.05f;
    for (std::size_t i = 0; i < 4; ++i) {
        const auto spec = CornerLoopForCorner(i);
        const auto sweep = ResolveArcSweep(spec.start_angle, spec.end_angle, spec.clockwise);
        if (std::abs(std::abs(sweep.end_radians - sweep.start_radians) - expected_sweep) > tolerance) {
            return false;
        }
    }
    return true;
}

int RunBoardGeometrySmoke() {
    const auto layout = MakeBoardLayout(ImVec2(0.0f, 0.0f), ImVec2(720.0f, 720.0f));

    auto okay = true;
    if (!HasOuterTrackPadding(layout)) {
        std::fputs("surakarta-gui geometry smoke failed: board layout lacks padding for the outer track\n", stderr);
        okay = false;
    }
    if (!UsesOuterCornerLoopSweeps()) {
        std::fputs("surakarta-gui geometry smoke failed: corner loops do not sweep the outer 3/4 circle\n", stderr);
        okay = false;
    }
    if (!okay) {
        return 1;
    }

    std::puts("[PASS] surakarta-gui geometry smoke");
    return 0;
}

void DrawPathFragments(ImDrawList* draw_list,
                       const BoardLayout& layout,
                       const std::vector<SurakartaMovePathFragment>& path,
                       ImU32 color,
                       float thickness) {
    for (const auto& fragment : path) {
        if (!fragment.is_curve) {
            const auto from = BoardPoint(layout, fragment.info.straight.start_x, fragment.info.straight.start_y);
            const auto to = BoardPoint(layout, fragment.info.straight.end_x, fragment.info.straight.end_y);
            draw_list->AddLine(from, to, color, thickness);
            continue;
        }

        const auto center = BoardPoint(layout, fragment.info.curve.center_x, fragment.info.curve.center_y);
        StrokeArc(draw_list,
                  center,
                  layout.grid_step * static_cast<float>(fragment.info.curve.radius),
                  fragment.info.curve.start_angle,
                  fragment.info.curve.end_angle,
                  fragment.info.curve.is_clockwise,
                  color,
                  thickness);
    }
}

void RenderBoard(DevelopmentSession& session,
                 const Palette& palette,
                 const ImVec2& available_size,
                 std::optional<LegalTargetInfo>* hovered_target_info) {
    const auto canvas_pos = ImGui::GetCursorScreenPos();
    const auto layout = MakeBoardLayout(canvas_pos, available_size);
    ImGui::InvisibleButton("board-canvas", available_size);

    auto* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(layout.panel_min, layout.panel_max, palette.board_panel, 22.0f);

    DrawCornerLoops(draw_list, layout, palette.grid, 2.6f);

    for (int axis = 0; axis < surakarta::bitboard::kBoardSize; ++axis) {
        draw_list->AddLine(BoardPoint(layout, 0, axis), BoardPoint(layout, 5, axis), palette.grid, 2.0f);
        draw_list->AddLine(BoardPoint(layout, axis, 0), BoardPoint(layout, axis, 5), palette.grid, 2.0f);
    }

    for (int y = 0; y < surakarta::bitboard::kBoardSize; ++y) {
        for (int x = 0; x < surakarta::bitboard::kBoardSize; ++x) {
            draw_list->AddCircleFilled(BoardPoint(layout, x, y), layout.grid_step * 0.06f, palette.point);
        }
    }

    hovered_target_info->reset();
    if (ImGui::IsItemHovered()) {
        const auto hovered_square = DetectSquare(layout, ImGui::GetIO().MousePos);
        if (hovered_square.has_value()) {
            for (const auto& target : session.LegalTargets()) {
                if (target.to == *hovered_square && target.IsCapture()) {
                    session.SetHoveredTarget(*hovered_square);
                    *hovered_target_info = target;
                    break;
                }
            }
            if (!hovered_target_info->has_value()) {
                session.ClearHoveredTarget();
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                if (session.SelectedSquare().has_value() && *session.SelectedSquare() == *hovered_square) {
                    session.ClearSelection();
                } else if (!session.ApplyHumanMove(*hovered_square) && !session.SelectSquare(*hovered_square)) {
                    session.ClearSelection();
                }
            }
        } else {
            session.ClearHoveredTarget();
        }
    } else {
        session.ClearHoveredTarget();
    }

    for (const auto& target : session.LegalTargets()) {
        const auto center = BoardPoint(layout,
                                       PositionAdapter::ToLegacyPosition(target.to).x,
                                       PositionAdapter::ToLegacyPosition(target.to).y);
        const auto color = target.IsCapture() ? palette.capture_target : palette.quiet_target;
        draw_list->AddCircleFilled(center, layout.piece_radius * 0.62f, color);
        if (target.IsCapture()) {
            draw_list->AddCircle(center, layout.piece_radius * 0.82f, color, 0, 2.0f);
        }
    }

    if (!session.HoverPath().empty()) {
        DrawPathFragments(draw_list, layout, session.HoverPath(), palette.hover_path, 4.0f);
    }

    if (session.SelectedSquare().has_value()) {
        const auto selected = PositionAdapter::ToLegacyPosition(*session.SelectedSquare());
        draw_list->AddCircle(BoardPoint(layout, selected.x, selected.y), layout.piece_radius * 1.25f, palette.selected, 0, 3.2f);
    }

    const auto& position = session.Position();
    for (int y = 0; y < surakarta::bitboard::kBoardSize; ++y) {
        for (int x = 0; x < surakarta::bitboard::kBoardSize; ++x) {
            const auto color = position.board.ColorAt(MakeSquare(x, y));
            if (color == Color::None) {
                continue;
            }

            const auto center = BoardPoint(layout, x, y);
            if (color == Color::Black) {
                draw_list->AddCircleFilled(center, layout.piece_radius, palette.black_piece);
            } else {
                draw_list->AddCircleFilled(center, layout.piece_radius, palette.white_piece);
                draw_list->AddCircle(center, layout.piece_radius, palette.white_piece_outline, 0, 2.4f);
            }
        }
    }

    for (int index = 0; index < surakarta::bitboard::kBoardSize; ++index) {
        const auto top = BoardPoint(layout, index, 0);
        const auto left = BoardPoint(layout, 0, index);
        const auto top_text = std::to_string(index);
        const auto left_text = std::to_string(index);
        const auto top_text_size = ImGui::CalcTextSize(top_text.c_str());
        const auto left_text_size = ImGui::CalcTextSize(left_text.c_str());
        const float top_label_y =
            layout.grid_origin.y - layout.outer_track_padding - top_text_size.y - layout.label_padding * 0.15f;
        const float left_label_x =
            layout.grid_origin.x - layout.outer_track_padding - left_text_size.x - layout.label_padding * 0.15f;
        draw_list->AddText(ImVec2(top.x - top_text_size.x * 0.5f, top_label_y), palette.grid, top_text.c_str());
        draw_list->AddText(ImVec2(left_label_x, left.y - left_text_size.y * 0.5f), palette.grid, left_text.c_str());
    }
}

void RenderSidebar(DevelopmentSession& session,
                   SidebarState& sidebar,
                   const std::optional<LegalTargetInfo>& hovered_target_info) {
    const auto snapshot = session.SearchSnapshot();
    const auto eval_score = snapshot.active ? snapshot.score : session.EvalScore();
    const auto status = session.Status();
    const auto& position = session.Position();

    ImGui::Text("Side To Move: %s", ColorLabel(position.SideToMove()));
    ImGui::Text("Human Side: %s", ColorLabel(session.HumanColor()));
    ImGui::Text("Engine State: %s", session.SearchActive() ? "thinking" : "idle");
    if (status.terminal) {
        ImGui::TextColored(ImVec4(0.82f, 0.30f, 0.18f, 1.0f),
                           "Game Ended: %s",
                           SurakartaToString(status.end_reason).c_str());
    }

    int depth = sidebar.depth;
    if (ImGui::InputInt("Depth", &depth)) {
        sidebar.depth = std::max(1, depth);
        ApplySearchSettings(session, sidebar);
    }

    int threads = sidebar.threads;
    if (ImGui::InputInt("Threads", &threads)) {
        sidebar.threads = std::max(1, threads);
        ApplySearchSettings(session, sidebar);
    }

    const int current_color = sidebar.human_color == Color::Black ? 0 : 1;
    int chosen_color = current_color;
    if (ImGui::RadioButton("Human Black", chosen_color == 0)) {
        chosen_color = 0;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Human White", chosen_color == 1)) {
        chosen_color = 1;
    }
    if (chosen_color != current_color) {
        sidebar.human_color = chosen_color == 0 ? Color::Black : Color::White;
        session.SetHumanColor(sidebar.human_color);
    }

    ImGui::Separator();
    ImGui::InputText("Board File", sidebar.file_name, IM_ARRAYSIZE(sidebar.file_name));
    if (ImGui::Button("Load Position")) {
        std::string error;
        if (!session.LoadFromFile(sidebar.file_name, &error)) {
            sidebar.status_message = error;
        } else {
            sidebar.status_message = "Loaded position from file.";
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Position")) {
        session.Reset();
        sidebar.status_message = "Reset to initial position.";
    }

    if (!sidebar.status_message.empty()) {
        ImGui::TextWrapped("%s", sidebar.status_message.c_str());
    }

    ImGui::Separator();
    ImGui::Text("Zobrist: 0x%016llX", static_cast<unsigned long long>(position.zobrist_key));
    ImGui::Text("Eval: %d", eval_score);
    ImGui::Text("Depth: %d", snapshot.depth);
    ImGui::Text("Nodes: %llu", static_cast<unsigned long long>(snapshot.nodes));
    ImGui::Text("QNodes: %llu", static_cast<unsigned long long>(snapshot.qnodes));
    ImGui::Text("NPS: %llu", static_cast<unsigned long long>(snapshot.nps));
    ImGui::Text("TT Hits: %llu", static_cast<unsigned long long>(snapshot.tt_hits));
    ImGui::Text("Null Prunes: %llu", static_cast<unsigned long long>(snapshot.null_prunes));
    ImGui::Text("LMR Reductions: %llu", static_cast<unsigned long long>(snapshot.lmr_reductions));
    ImGui::Text("Fail-High: %llu", static_cast<unsigned long long>(snapshot.fail_highs));
    ImGui::Text("Fail-Low: %llu", static_cast<unsigned long long>(snapshot.fail_lows));
    ImGui::Text("Aspiration Retries: %llu", static_cast<unsigned long long>(snapshot.aspiration_retries));
    ImGui::Text("Aspiration Attempts: %llu", static_cast<unsigned long long>(snapshot.aspiration_attempts));
    ImGui::Text("Full-Window Re-searches: %llu", static_cast<unsigned long long>(snapshot.full_window_researches));
    ImGui::Text("Root Best Updates: %llu", static_cast<unsigned long long>(snapshot.root_best_updates));
    ImGui::Text("TT Exact Cutoffs: %llu", static_cast<unsigned long long>(snapshot.tt_exact_cutoffs));
    ImGui::Text("TT Bound Cutoffs: %llu", static_cast<unsigned long long>(snapshot.tt_bound_cutoffs));
    ImGui::Text("Beta Cutoffs: %llu", static_cast<unsigned long long>(snapshot.beta_cutoffs));
    ImGui::Text("Stale Root Tasks: %llu", static_cast<unsigned long long>(snapshot.stale_root_tasks));
    ImGui::Text("Aborted Root Tasks: %llu", static_cast<unsigned long long>(snapshot.aborted_root_tasks));
    ImGui::Text("Worker Idle Spins: %llu", static_cast<unsigned long long>(snapshot.worker_idle_spins));
    ImGui::Text("Thread Spawns: %llu", static_cast<unsigned long long>(snapshot.thread_spawn_count));
    ImGui::Text("Best Move: %s", FormatMove(snapshot.best_move).c_str());
    ImGui::TextWrapped("PV: %s", FormatPrincipalVariation(position, snapshot.pv, snapshot.pv_length).c_str());

    if (session.SelectedSquare().has_value()) {
        ImGui::Separator();
        ImGui::Text("Selected: %s", FormatSquare(*session.SelectedSquare()).c_str());
        for (const auto& target : session.LegalTargets()) {
            ImGui::BulletText("%s %s%s",
                              FormatSquare(target.to).c_str(),
                              target.IsCapture() ? "capture" : "quiet",
                              target.IsCapture()
                                  ? (" (" + std::to_string(target.variant_count) + " variants)").c_str()
                                  : "");
        }
    }

    if (hovered_target_info.has_value()) {
        ImGui::Separator();
        ImGui::Text("Hover Target: %s", FormatSquare(hovered_target_info->to).c_str());
        ImGui::Text("Path Variants: %u", hovered_target_info->variant_count);
        ImGui::Text("Displayed Variant: %u", hovered_target_info->primary_variant);
        ImGui::Text("Path Segments: %d", static_cast<int>(session.HoverPath().size()));
    }
}

StartupOptions ParseArgs(int argc, char** argv) {
    auto options = StartupOptions{};
    for (int i = 1; i < argc; ++i) {
        const auto arg = std::string(argv[i]);
        if (arg == "--smoke-test") {
            options.smoke_test = true;
        } else if (arg == "--smoke-test-init") {
            options.smoke_test_init = true;
        } else if (arg == "--smoke-test-geometry") {
            options.smoke_test_geometry = true;
        } else if ((arg == "--file" || arg == "-f") && i + 1 < argc) {
            options.file_name = argv[++i];
        } else if ((arg == "--depth" || arg == "-d") && i + 1 < argc) {
            options.depth = std::max(1, std::atoi(argv[++i]));
        } else if ((arg == "--threads" || arg == "-j") && i + 1 < argc) {
            options.threads = std::max(1, std::atoi(argv[++i]));
        } else if ((arg == "--human" || arg == "-h") && i + 1 < argc) {
            const auto value = std::string(argv[++i]);
            if (value == "white" || value == "W") {
                options.human_color = Color::White;
            } else {
                options.human_color = Color::Black;
            }
        }
    }
    return options;
}

void ConfigureOpenGlHints(const char** glsl_version, bool hidden_window) {
#if defined(__APPLE__)
    *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    glfwWindowHint(GLFW_VISIBLE, hidden_window ? GLFW_FALSE : GLFW_TRUE);
}

int RunGuiInitializationSmoke() {
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        std::puts("[SKIP] surakarta-gui init smoke: glfwInit failed");
        return 0;
    }

    const char* glsl_version = nullptr;
    ConfigureOpenGlHints(&glsl_version, true);

    auto* window = glfwCreateWindow(160, 120, "surakarta-gui-smoke", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        std::puts("[SKIP] surakarta-gui init smoke: no OpenGL window available");
        return 0;
    }

    glfwMakeContextCurrent(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto okay = ImGui_ImplGlfw_InitForOpenGL(window, true);
    if (okay) {
        okay = ImGui_ImplOpenGL3_Init(glsl_version);
    }

    if (okay) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();
    }

    if (okay) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    if (!okay) {
        std::fputs("surakarta-gui init smoke failed during ImGui backend initialization\n", stderr);
        return 1;
    }

    std::puts("[PASS] surakarta-gui init smoke");
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    const auto options = ParseArgs(argc, argv);

    if (options.smoke_test_init) {
        return RunGuiInitializationSmoke();
    }
    if (options.smoke_test_geometry) {
        return RunBoardGeometrySmoke();
    }

    auto session = DevelopmentSession{};
    auto sidebar = SidebarState{};
    sidebar.depth = options.depth;
    sidebar.threads = options.threads;
    sidebar.human_color = options.human_color;
    ApplySearchSettings(session, sidebar);
    session.SetHumanColor(options.human_color);
    if (!options.file_name.empty()) {
        strncpy_s(sidebar.file_name, options.file_name.c_str(), _TRUNCATE);
        std::string error;
        if (!session.LoadFromFile(options.file_name, &error)) {
            if (options.smoke_test) {
                std::fprintf(stderr, "%s\n", error.c_str());
                return 1;
            }
            sidebar.status_message = error;
        }
    }

    if (options.smoke_test) {
        session.Update();
        std::puts("[PASS] surakarta-gui smoke");
        return 0;
    }

    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        return 1;
    }

    const char* glsl_version = nullptr;
    ConfigureOpenGlHints(&glsl_version, false);

    auto* window = glfwCreateWindow(1440, 900, "Tactical Grinder Validator", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsLight();
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f;
    style.FrameRounding = 8.0f;
    style.ChildRounding = 10.0f;
    style.GrabRounding = 8.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.92f, 0.87f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.88f, 0.84f, 0.76f, 0.72f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    const auto palette = Palette{};
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        session.Update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(16.0f, 16.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 32.0f, io.DisplaySize.y - 32.0f), ImGuiCond_Always);
        ImGui::Begin("Surakarta Validator", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        const auto content = ImGui::GetContentRegionAvail();
        constexpr float kPaneGap = 12.0f;
        constexpr float kMinSidebarWidth = 180.0f;
        const float max_sidebar_width = std::max(kMinSidebarWidth, content.x - 280.0f - kPaneGap);
        const float sidebar_width = std::clamp(content.x * 0.26f, kMinSidebarWidth, max_sidebar_width);
        const float board_width = std::max(0.0f, content.x - sidebar_width - kPaneGap);
        auto hovered_target_info = std::optional<LegalTargetInfo>{};

        ImGui::BeginChild("board-pane", ImVec2(board_width, content.y), true);
        RenderBoard(session, palette, ImGui::GetContentRegionAvail(), &hovered_target_info);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("sidebar-pane", ImVec2(sidebar_width, content.y), true);
        RenderSidebar(session, sidebar, hovered_target_info);
        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        int display_w = 0;
        int display_h = 0;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.90f, 0.87f, 0.80f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
