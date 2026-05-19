# Surakarta Traditional 校赛答辩信息素材报告

> 生成时间：2026-05-11  
> 范围：只整理当前仓库真实代码、文档和本轮验证结果；不改生产代码、不改搜索算法、不改棋规、不改训练、不改 selection gate、不进入 Phase 4。  
> 当前版本定位：**以 bitboard + fixed-depth PVS + 多线程根搜索为核心的校赛可用 GUI 人机对战版本**。

## 0. 结论先行

- **已实现 / 可讲进 PPT**：6x6 bitboard 棋盘表示、预计算 quiet/capture masks、苏拉卡尔塔圆弧吃子路径 variants、traditional/bitboard parity 测试、fixed-depth PVS/Alpha-Beta、iterative deepening、aspiration window、TT、qsearch、move ordering、killer/history、LMR、null move、Lazy SMP 风格根并行、Dear ImGui 人机对战 GUI、Depth/Threads 可配置、Undo Human Turn、Last Move/Thinking/PV/Debug stats 显示。
- **已验证 / 可作为可靠性展示**：`surakarta-bitboard-selftest`、`surakarta-dev-session-selftest`、CTest、Pester workflow、GUI smoke、P0 capture-rule golden parity、capture variant capacity invariant。本轮实测：CTest `8/8 passed`，Pester `26 passed / 0 failed`，bitboard/dev-session/GUI smoke 均 `[PASS]`。
- **实验性 / 谨慎讲**：TD-Learning / `bitboard-train` / `benchmark-select.ps1` 已有闭环和诊断，但 Phase 3 仍是 `implementation complete, trend validation failed`，不能说训练出了稳定最强权重。
- **blocked / 不能吹**：Phase 4 blocked；threat/mobility/stage 等特征扩展和后续搜索调参只能写后续计划。
- **别装糊涂**：历史 pre-fix training/selection artifacts 被标为 suspect/provisional，不能拿来当强度证明。legacy `max_no_capture_round` / `no_capture_ply` 也不是大赛第 6 条“双方都不能再吃子”的严格实现。

## 1. 项目基本信息

### 1.1 项目名称与目标

- 项目名称可写：**Surakarta Traditional / 苏拉卡尔塔棋程序**。仓库 README 标题仍是 `Surakarta Core`，说明它原本是 `libsurakarta` 的规则管理和简单 AI 库，并带 benchmark CLI 与测试；答辩时建议把校赛版本称为“Surakarta Traditional 校赛 GUI 对战版本”，别把 README 的上游库名硬改成宣传名。来源：`README.md:1-5`。
- 项目目标：实现苏拉卡尔塔棋规则管理、合法走法生成、搜索 AI、GUI 人机对战与工程验证。源码入口包括动态库、benchmark CLI、自测和 GUI。来源：`README.md:16-21`、`src/CMakeLists.txt:30-143`。

### 1.2 支持的运行方式

- **GUI 人机对战/验证工具**：`surakarta-gui.exe`，Dear ImGui + GLFW/OpenGL。来源：`src/CMakeLists.txt:87-143`、`src/surakarta_gui_main.cpp:978-1102`。
- **Benchmark / CLI**：`surakarta-benchmark.exe`，支持 `bitboard-search`、`bitboard-benchmark`、`bitboard-trace`、`bitboard-train`、`bitboard-eval`、legacy `play/statistic`。来源：`src/main.cpp:1506-1526`、`src/main.cpp:1529-1585`。
- **规则与搜索自测**：`surakarta-bitboard-selftest.exe`、`surakarta-dev-session-selftest.exe`。来源：`src/CMakeLists.txt:36-51`。
- **CTest 集成**：bitboard selftest、dev-session selftest、mask capacity fail-fast、GUI smoke。来源：`src/CMakeLists.txt:41-84`、`src/CMakeLists.txt:129-143`。
- **PowerShell workflow**：`workflow.ps1` 配置、构建、运行 CTest 并检查遗留进程；`perf-runner.ps1`、`benchmark-train.ps1`、`benchmark-select.ps1` 分别服务 perf、训练 smoke、selection。来源：`README.md:23-45`、`scripts/workflow.ps1:13-21`。

### 1.3 当前校赛版本主要功能

- GUI 默认校赛 Match preset 为 **Depth 10 / Threads 10**，并明确不是训练设置、不是 selection gate、不是 Phase 4 checkpoint。来源：`README.md:47-57`、`src/surakarta_gui_main.cpp:36-58`。
- GUI 可显示棋盘、棋子、合法目标、吃子路径、最近一步、搜索中 best move/PV、调试统计；可加载局面、重置局面、选择人类执黑/执白、修改 Depth/Threads。来源：`src/surakarta_gui_main.cpp:582-691`、`src/surakarta_gui_main.cpp:701-875`。
- 人类落子后，`DevelopmentSession` 异步启动引擎搜索；搜索完成后自动应用引擎走法。来源：`src/surakarta_dev_session.cpp:100-119`、`src/surakarta_dev_session.cpp:226-260`、`src/surakarta_dev_session.cpp:347-372`。

### 1.4 GUI 人机对战流程

当前校赛对战方式是“两台电脑人工转达”：

1. 在我方 GUI 中把对手程序的走法当作“人类走法”输入。
2. GUI 立即更新局面，并在对手回合触发我方引擎搜索。
3. 我方引擎搜索完成后，GUI 自动落子并在右侧显示 `Last Move`；人再把这步转达到对方电脑。

对应代码语义：GUI 点击棋盘时优先尝试 `ApplyHumanMove`，失败再选中棋子；`ApplyHumanMove` 只在人类回合、AI 空闲、非终局时接受合法目标；引擎回合由 `StartEngineSearchIfNeeded()` 异步搜索并由 `ApplyCompletedSearchIfReady()` 落子。来源：`src/surakarta_gui_main.cpp:621-626`、`src/surakarta_dev_session.cpp:80-119`、`src/surakarta_dev_session.cpp:194-260`、`src/surakarta_dev_session.cpp:347-372`。

### 1.5 编译和运行入口

- CMake 顶层启用 C++20，默认 `SURAKARTA_ENABLE_GUI=ON`。来源：`CMakeLists.txt:1-6`。
- Windows workflow 标准入口：`pwsh ./scripts/workflow.ps1 -Configuration Debug|Release`。来源：`README.md:29-35`、`scripts/workflow.ps1:13-21`。
- 当前本机实际存在 Debug 构建目录：`build\bin`，README 也列出这些二进制路径。来源：`README.md:16-21`；本轮本地检查确认 `build\bin\surakarta-gui.exe` 等存在。

## 2. 核心算法与关键技术

### 2.1 棋盘表示：bitboard / uint64_t / masks / precomputed move generation

- **已实现**：棋盘使用 6x6，共 36 格；`Bitboard = std::uint64_t`，每种颜色一个 bitboard。来源：`include/surakarta_bitboard.h:17-25`、`include/surakarta_bitboard.h:88-120`。
- `Position` 是 64 字节对齐结构，含 `Board`、Zobrist key、side-to-move、ply、no-capture 状态等。来源：`include/surakarta_bitboard.h:174-198`；selftest 断言 `alignof(Position)==64`，来源：`src/surakarta_bitboard_selftest.cpp:2132-2135`。
- 走法表包含 `quiet_move_mask`、`capture_target_mask`、`capture_variant_count`、`capture_clear_mask`、`capture_start_dir`。来源：`include/surakarta_bitboard.h:200-206`。
- 默认使用静态预计算表 `include/surakarta_masks.h`，动态生成逻辑由 `BitboardGenerator::Build()` 维护；`surakarta-mask-export` 可重新生成 masks。来源：`src/surakarta_bitboard.cpp:275-282`、`src/surakarta_mask_export.cpp:90-118`。

### 2.2 苏拉卡尔塔圆弧吃子的建模

- 圆弧吃子不是临场随便 DFS，而是预计算成“从 from 到 to 的若干 path variants”。`NextCaptureStep()` 处理直线越界后沿角弧转向；`CapturePathFragment()` 生成可视化路径片段。来源：`src/surakarta_bitboard.cpp:55-92`、`src/surakarta_bitboard.cpp:129-166`。
- `BitboardGenerator::Build()` 从每个格子沿 4 个直线方向出发，记录经过至少一个 corner 后可到达的目标；每条 variant 保存需要为空的 `clear_mask` 和起始方向。来源：`src/surakarta_bitboard.cpp:216-273`。
- 吃子合法性由 `capture_target_mask` 先筛目标，再逐 variant 检查 `occupied & capture_clear_mask == 0`；这正是 P0 修复相关的关键点。来源：`src/surakarta_bitboard.cpp:168-181`、`src/surakarta_bitboard.cpp:345-360`。

### 2.3 合法走法与吃子路径生成

- `GenerateMoves()` 遍历当前方 bitboard：普通走法来自 `quiet_move_mask & ~occupied`，吃子候选来自 `enemy & capture_target_mask[from]`，再由 `IsCaptureLegal()` 过滤。支持 `captures_only=true` 供 qsearch 使用。来源：`src/surakarta_bitboard.cpp:481-508`。
- `DescribeLegalTargets()` 给 GUI 和 debug 使用，返回目标、是否 capture、variant 数量、primary variant。来源：`src/surakarta_bitboard.cpp:385-415`。
- `ReconstructCapturePath()` 根据 selected variant 还原直线/弧线片段，用于 GUI 显示路径和测试对比 legacy trace。来源：`src/surakarta_bitboard.cpp:417-479`、`src/surakarta_gui_main.cpp:247-252`。
- `MakeMove()` / `UnmakeMove()` 维护吃子移除、side-to-move、ply、no-capture、Zobrist、eval cache；吃子会重置 `no_capture_ply=0`。来源：`src/surakarta_bitboard.cpp:527-570`。

### 2.4 traditional movegen 与 bitboard movegen 一致性

- **已验证**：`TestRuleParity()` 对历史测试局面比较 traditional rule manager 与 bitboard rule manager 的 move reason、legal targets、end reason。来源：`src/surakarta_bitboard_selftest.cpp:653-741`。
- **已验证**：`TestRuleCorrectnessGoldenParity()` 对中心、角、外环、内环、多路径 P0、己方阻挡、敌方目标等 golden 局面比较 traditional 与 bitboard，并比较 full movegen / capture-only movegen。来源：`src/surakarta_bitboard_selftest.cpp:743-857`。
- **意义**：traditional 仍是规则 oracle 之一，bitboard 是高性能实现；parity 测试保证 GUI/搜索使用的 bitboard 不偏离 legacy 规则。

### 2.5 搜索算法实际实现

- **PVS / Alpha-Beta 已实现**：核心函数 `WorkerContext::Pvs()` 使用 negamax PVS；首个 move 全窗口，后续 move 先零窗口，必要时 re-search。来源：`src/surakarta_search.cpp:828-986`。
- **参考 Alpha-Beta 已实现**：`SearchReference()` 调 `ReferenceAlphaBeta()`，selftest 比较 PVS 与 reference 的分数。来源：`src/surakarta_search.cpp:1319-1348`、`src/surakarta_search.cpp:1900-1918`、`src/surakarta_bitboard_selftest.cpp:1191-1205`。
- **Iterative deepening 已实现**：`SearchController::Search()` 从 depth 1 循环到 `limits.max_depth`。来源：`src/surakarta_search.cpp:1734-1884`。
- **Aspiration window 已实现**：depth>1 时围绕上一层分数开窗口，fail-low/high 扩窗或全窗口重搜；统计 `aspiration_retries`、`fail_highs/lows`。来源：`src/surakarta_search.cpp:1760-1877`。
- **Transposition Table 已实现**：`TranspositionTable` 为 4-entry cluster，原子 key/meta/move，支持 `Probe()` / `Store()`；PVS 使用 TT move 与 bound cutoffs。来源：`include/surakarta_search.h:149-193`、`src/surakarta_search.cpp:1353-1444`、`src/surakarta_search.cpp:869-890`、`src/surakarta_search.cpp:984-985`。
- **Quiescence search 已实现**：depth<=0 进入 `Quiescence()`，只生成 capture-only movegen。来源：`src/surakarta_search.cpp:780-826`、`src/surakarta_search.cpp:842-866`。
- **Move ordering 已实现**：TT move、上一轮 preferred root move、capture、recapture、killer、history。来源：`src/surakarta_search.cpp:710-778`、`src/surakarta_search.cpp:917-918`、`src/surakarta_search.cpp:1282-1283`。
- **Killer / history 已实现**：`WorkerContext` 持有 `history` 和 `killer`，beta cutoff 后更新。来源：`src/surakarta_search.cpp:613-615`、`src/surakarta_search.cpp:768-778`、`src/surakarta_search.cpp:971-974`。
- **LMR 已实现**：非 PV、非吃子、depth>=3、late move、非 killer/TT move 时降深，必要时补搜。来源：`src/surakarta_search.cpp:930-956`。
- **Null Move Pruning 已实现**：非 PV、允许 null、depth>=4、棋子数>2、有 quiet move 时尝试 null move cutoff。来源：`src/surakarta_search.cpp:892-908`。
- **Lazy SMP / 多线程根并行已实现**：`RootWorkerPool` 按 `threads-1` 创建 worker，多个线程消费 root move queue，共享 alpha/beta、stop、统计和 TT。来源：`src/surakarta_search.cpp:1120-1252`、`src/surakarta_search.cpp:1255-1317`。

### 2.6 Depth / Threads 如何生效

- GUI 的 `Depth` 和 `Threads` 写入 `SearchLimits.max_depth` / `SearchLimits.threads`，并通过 `DevelopmentSession::SetSearchLimits()` 触发下一次搜索设置。来源：`src/surakarta_gui_main.cpp:157-162`、`src/surakarta_gui_main.cpp:744-758`、`src/surakarta_dev_session.cpp:70-78`。
- CLI 的 `--depth/-d`、`--threads/-j` 解析到 `CommonBitboardOptions`，再写入 `SearchLimits`。来源：`src/main.cpp:777-824`、`src/main.cpp:1008-1039`。
- `max_depth` 决定 iterative deepening 的最终层数；`threads` 决定是否创建 root worker pool。来源：`src/surakarta_search.cpp:1751-1757`、`src/surakarta_search.cpp:1760`。

### 2.7 当前是 fixed-depth，不是真正 time management

- `SearchLimits` 只有 `max_depth`、`threads`、`aspiration_window`、`node_limit` 和若干开关，没有 time limit / movetime 字段。来源：`include/surakarta_search.h:15-23`。
- `SharedSearchState.start_time` 只用于 snapshot/NPS 统计，不用于按时间停止。来源：`src/surakarta_search.cpp:85-92`、`src/surakarta_search.cpp:252-254`。
- 所以校赛实战要用固定 Depth preset 控制耗时，而不是期待引擎自动在某个秒数刹车。这个地方要讲实话，别整“智能时间管理”那套，评委追问两句就翻车。

## 3. 评估函数与训练

### 3.1 当前评估函数组成

- 基础项：`MaterialBalance()`，黑白棋子数差乘 100。来源：`src/surakarta_bitboard.cpp:597-599`。
- N-Tuple 项：45 个 tuple descriptors，包括局部 2x2、横行、竖列和若干形状；每个 tuple 用三态编码加入权重。来源：`src/surakarta_search.cpp:393-407`、`src/surakarta_search.cpp:411-466`。
- `NTupleEvaluator::Evaluate()` 返回从 side-to-move 视角的分数；白方走子时取负。来源：`src/surakarta_search.cpp:1667-1675`。
- bootstrap 权重由 tuple 中黑白棋数量差初始化为 `(black-white)*6`。来源：`src/surakarta_search.cpp:1691-1715`。

### 3.2 TD-Learning / bitboard-train 工具状态

- **已实现工具链**：`TrainingOptions` 支持 `alpha/lambda/epsilon/epsilon_plies/terminal_reward/td_error_clip/near_terminal_curriculum/seed/checkpoint`。来源：`include/surakarta_search.h:281-297`。
- **TD 更新已实现**：`ApplyTrainingStep()` 计算 current value、terminal 或 bootstrap target、eligibility trace、TD error clipping、权重更新和 clamp。来源：`src/surakarta_search.cpp:1556-1620`。
- **self-play 训练已实现**：`RunBitboardTraining()` 支持从 bootstrap 或输入权重开始、自对弈、epsilon 探索、checkpoint、summary。来源：`src/surakarta_search.cpp:2190-2395`。
- **CLI / 脚本已实现**：`bitboard-train`、`bitboard-trace`、`bitboard-eval`、`benchmark-train.ps1`、`benchmark-select.ps1`。来源：`src/main.cpp:1143-1464`、`src/main.cpp:1466-1504`、`scripts/benchmark-train.ps1:1-55`、`scripts/benchmark-select.ps1:1-76`。

### 3.3 训练当前结论：不能说 completed

- Phase 3 文档状态是 `implementation complete, trend validation failed`；32 局和 64 局 evidence 仍 `regress`，Phase 4 blocked。来源：`task_plan.md:31-46`、`task_plan.md:73-81`。
- P0 事故后，疑似规则回归之后的 training / selection artifacts 必须标记为 suspect/provisional，不能作为趋势通过依据。来源：`task_plan.md:47-70`、`progress.md:757-764`。
- 可以在 PPT 里说：“已搭建 TD/self-play 和 selection 观察闭环，当前用于研究和诊断；校赛版本暂不依赖训练权重作为强度证明。”不要说“训练已得到最强权重”。这话不难听，但真实，老实比硬吹强。
- 当前校赛版本主要依赖 **fixed-depth PVS + bitboard movegen + 多线程搜索** 的搜索强度，训练模块作为实验性后续方向。来源：搜索实现见 `src/surakarta_search.cpp:828-986`、多线程见 `src/surakarta_search.cpp:1165-1252`；训练未通过趋势见 `task_plan.md:31-46`。

## 4. GUI 与校赛使用方式

### 4.1 GUI 当前能做什么

- 显示 6x6 棋盘、内外圆弧轨道、棋子、坐标标签、合法落点、吃子目标、hover path、last move path。来源：`src/surakarta_gui_main.cpp:557-691`。
- 侧边栏显示 Side To Move、Human Side、Engine State、终局状态、Preset、Depth、Threads、人类执黑/执白、Board File、Load/Reset/Undo、Last Move 或 Thinking/PV、Debug/Search Stats。来源：`src/surakarta_gui_main.cpp:701-875`。

### 4.2 加载局面

- 侧边栏 `Board File` 输入路径，点 `Load Position` 调 `session.LoadFromFile()`；CLI 也支持 `--file/-f`。来源：`src/surakarta_gui_main.cpp:777-786`、`src/surakarta_gui_main.cpp:891-892`。
- `DevelopmentSession::LoadFromFile()` 会取消搜索、加载 position、清空 undo、清 selection、刷新终局状态，并在需要时启动引擎搜索。来源：`src/surakarta_dev_session.cpp:53-56`、`src/surakarta_dev_session.cpp:202-217`。

### 4.3 人类执黑/执白

- GUI 通过 `Human Black` / `Human White` radio button 切换，调用 `SetHumanColor()`。来源：`src/surakarta_gui_main.cpp:763-775`。
- 切换人类方会取消已有搜索、清 undo、重建 selection/path，必要时立即让引擎走。来源：`src/surakarta_dev_session.cpp:58-68`。

### 4.4 Depth / Threads 设置

- 侧边栏有 Preset combo；`Fast=8/8`，`Match=10/10`，`Analysis=10/10`，手动改 Depth/Threads 会变成 `Custom`。来源：`src/surakarta_gui_main.cpp:41-59`、`src/surakarta_gui_main.cpp:723-758`。
- 搜索中 Depth/Threads 禁用，tooltip 提示设置应用于下一次引擎搜索。来源：`src/surakarta_gui_main.cpp:84-90`、`src/surakarta_gui_main.cpp:693-699`、`src/surakarta_gui_main.cpp:719-761`。

### 4.5 Match preset = Depth 10 / Threads 10 的依据

- README 记录 post-fix calibration：D10/T10 全局平均约 `0.482s`，max 约 `1.975s`；hard-position max 约 `1.783s`；Threads=12 没有稳定收益；Depth=4 是 debug preset，不适合比赛；该记录不是 Phase 3 trend pass。来源：`README.md:47-57`。
- 本机硬件确认：`13th Gen Intel(R) Core(TM) i7-13650HX`，14 cores / 20 logical processors。本项来自本轮环境命令 `Get-CimInstance Win32_Processor`，不是源码文件。

### 4.6 悔棋功能语义

- **已实现**：`CanUndo()` 只有在 AI 空闲、当前轮到人类、undo stack 非空时为 true。来源：`src/surakarta_dev_session.cpp:128-130`。
- `UndoHumanTurn()` 恢复最近一次人类回合 snapshot，清 selection，重置 search artifacts，保持 idle。来源：`src/surakarta_dev_session.cpp:132-149`。
- Snapshot 在成功应用人类走法前压栈；引擎自动回复不压栈。因此一次 undo 通常回到“上一次人类可走局面”，相当于撤销人类一步和引擎回复一步。来源：`src/surakarta_dev_session.cpp:100-119`、`src/surakarta_dev_session.cpp:339-345`。
- 自测覆盖：引擎回复后可撤回、人类回合恢复、搜索中禁止 undo、reset/load/change side 清历史。来源：`src/surakarta_dev_session_selftest.cpp:326-403`。

### 4.7 Last Move / Best Move / PV / Debug stats 显示逻辑

- 搜索中或引擎回合显示 `Thinking`，展示当前 `snapshot.best_move` 与 PV；搜索结束后显示 `Last Move` 和上一步路径；无结果时显示 `Last Move: n/a`。来源：`src/surakarta_gui_main.cpp:254-280`、`src/surakarta_gui_main.cpp:811-822`。
- Debug/Search Stats 默认折叠，里面才显示 Zobrist、Eval、Depth、Nodes、QNodes、NPS、TT Hits、Null Prunes、LMR、Aspiration、Root updates、Best Move、PV、选中目标 variants、hover path 信息。来源：`src/surakarta_gui_main.cpp:88-94`、`src/surakarta_gui_main.cpp:824-874`。
- **对局必要信息**：Side To Move、Human Side、Engine State、Last Move/Thinking、PV（思考中）、棋盘合法点/路径、Depth/Threads。
- **debug 信息**：Zobrist、Eval、nodes/qnodes/NPS、TT/LMR/null/aspiration counters、stale/aborted root tasks、selected target variants。这些适合答辩讲技术，不是比赛转达走法必需。

## 5. 校赛版本强度与性能

### 5.1 为什么 Depth 10 / Threads 10 适合作为默认

- D10/T10 是 README 记录的校赛 GUI 默认配置，不是训练/selection/Phase 4 配置。来源：`README.md:47-57`。
- 校准摘要显示平均耗时约半秒、最慢约两秒以内，符合人工转达对局节奏；Threads=10 在 20 logical processors 的 i7-13650HX 上留有系统和 GUI 余量。
- 因为当前没有严格 time management，fixed-depth 的优势是输出稳定、配置可复现；风险是个别局面耗时可能上探，所以校赛现场应提前用目标机器跑 GUI/benchmark smoke。

### 5.2 i7-13650HX 性能摘要

- 本机 CPU：i7-13650HX，14 核 / 20 逻辑线程，MaxClockSpeed 2600 MHz。本轮环境命令确认。
- README 校准：D10/T10 global average `~0.482s`，max `~1.975s`，hard-position max `~1.783s`。来源：`README.md:51-57`。
- 这些数值只能作为“post-fix preset calibration summary”，不能作为 Phase 3 trend pass 或训练成功证据。来源：`README.md:57`。

### 5.3 为什么不默认 Threads=12 或 20

- README 明确记录 `Threads=12` 没有稳定收益。来源：`README.md:55`。
- 代码多线程是根节点任务并行，线程数过大时受 root move 数量、搜索树不均衡、stale/aborted root tasks、同步开销影响，不是线程越多越快。来源：`src/surakarta_search.cpp:1120-1252`、`src/surakarta_bitboard_selftest.cpp:1331-1378`。
- Threads=20 会把所有逻辑线程都占满，GUI、系统、对方输入转达都容易受影响；没有代码证据支持默认 20，所以不能硬上。

### 5.4 为什么 Depth=4 是调试档

- `SearchLimits` 默认 max_depth 是 4，`DevelopmentSession` 构造也初始化 depth=4，这更像开发默认。来源：`include/surakarta_search.h:15-23`、`src/surakarta_dev_session.cpp:36-40`。
- README 已明确 `Depth=4` 是 debug preset，不适合作为比赛默认。来源：`README.md:56`。
- CLI `bitboard-eval`/selection 也常用 depth 4 是为了评估和训练成本控制，不等于比赛强度配置。来源：`findings.md:62-67`、`scripts/benchmark-select.ps1:1-15`。

### 5.5 Fixed-depth 实战使用建议

- 校赛前用 `Depth 10 / Threads 10` 打开 GUI，确认首步和几个测试局面耗时能接受。
- 对局中不要临时改 Depth/Threads，除非比赛节奏明显超时；修改设置只对下一次引擎搜索生效。
- 如果机器散热或现场负载很差，可降到 `Fast 8/8`；但 PPT 默认推荐仍写 Match `10/10`。

## 6. 规则正确性与可靠性

### 6.1 P0 capture-rule regression 是什么

- P0 局面：Black to move；黑子 `(2,1)`, `(2,2)`, `(2,3)`，白子 `(3,3)`；合法吃子应为 `(2,2)->(3,3)`。修复前 traditional 判 `LEGAL_CAPTURE_MOVE`，bitboard 判 `ILLIGAL_CAPTURE_MOVE` / capture-only movegen missing。来源：`task_plan.md:59-63`、`findings.md:653-673`。
- 根因：`kMaxCaptureVariants=4` 静默截断了实际 8 条 variants；前 4 条被己方棋子挡住，后续合法路径没进表。来源：`task_plan.md:62`、`findings.md:664-679`。

### 6.2 修复与保护

- 当前 `kMaxCaptureVariants` 默认是 8。来源：`include/surakarta_bitboard.h:11-21`。
- `BitboardGenerator::Build()` 在 variant 数超过容量时直接 throw fail-fast，不再静默截断。来源：`src/surakarta_bitboard.cpp:251-263`。
- `surakarta-mask-capacity-failfast` CTest 用 `SURAKARTA_CAPTURE_VARIANT_CAPACITY=4` 构建预期失败目标，保证容量不足时会失败。来源：`src/CMakeLists.txt:67-84`。
- `TestCaptureVariantCapacityInvariant()` 动态生成表和静态表全量扫描，确认当前最大 variants 为 8 且不超过容量。来源：`src/surakarta_bitboard_selftest.cpp:2020-2069`。

### 6.3 Golden tests 覆盖场景

- `TestRuleCorrectnessGoldenParity()` 覆盖中心 quiet、角 quiet、外环吃子、内环多路径 P0、己方阻挡、敌方目标、full movegen、capture-only movegen、traditional/bitboard move reason parity。来源：`src/surakarta_bitboard_selftest.cpp:743-857`。
- `TestCaptureVariantOverflowGolden()` 要求 `(2,2)->(3,3)` 保留 8 条 variants，bitboard/传统均合法，capture-only movegen 包含该吃子。来源：`src/surakarta_bitboard_selftest.cpp:859-896`。
- `TestIllegalCaptureVariantBlocked()` 验证己方阻挡时 traditional/bitboard 都拒绝。来源：`src/surakarta_bitboard_selftest.cpp:898-918`。
- `TestSearchDepthOneSeesGoldenCapture()` 验证 qsearch capture-only source 包含 golden capture，depth-1 search 选择该立即终局吃子。来源：`src/surakarta_bitboard_selftest.cpp:978-999`。

### 6.4 traditional / bitboard parity 的意义

- traditional 规则实现直观但慢，bitboard 适合搜索；parity 测试就是防止快版本跑偏。`TestRuleParity()`、`TestRuleCorrectnessGoldenParity()` 都在干这个。来源：`src/surakarta_bitboard_selftest.cpp:653-741`、`src/surakarta_bitboard_selftest.cpp:743-857`。

### 6.5 仍存在的终局规则 debt

- 当前 traditional/bitboard/search 仍使用 legacy `max_no_capture_round` / `no_capture_ply` 表达 stalemate/no-capture limit。来源：`src/surakarta_rule_manager_impl.cpp:74-89`、`src/surakarta_rule_manager_bitboard.cpp:43-65`、`src/surakarta_search.cpp:528-542`。
- 文档明确：这不是大赛第 6 条“双方都不能再吃子”的严格实现，后续要单独改 `JudgeEnd` / search terminal semantics 并补 golden cases。来源：`task_plan.md:65-71`、`findings.md:648-650`、`findings.md:758-761`、`progress.md:766-769`。

## 7. 工程结构

| 模块 | 主要文件 | 作用 |
|---|---|---|
| bitboard 表示 | `include/surakarta_bitboard.h` | 6x6 bitboard、Position、Move、MoveList、BitboardTables、接口定义。 |
| bitboard movegen | `src/surakarta_bitboard.cpp` | quiet/capture mask 动态生成、吃子合法性、路径重建、movegen、make/unmake、Zobrist。 |
| 静态 masks | `include/surakarta_masks.h` | 预计算 `kStaticBitboardTables`，运行时默认使用。 |
| mask export/fail-fast | `src/surakarta_mask_export.cpp` | 生成 masks header；容量不足时返回错误。 |
| 搜索 | `include/surakarta_search.h`、`src/surakarta_search.cpp` | SearchLimits/Stats、TT、N-Tuple eval、PVS/qsearch/ID/aspiration/null/LMR、多线程、training/eval。 |
| benchmark CLI | `src/main.cpp` | `bitboard-search`、`bitboard-benchmark`、`bitboard-trace`、`bitboard-train`、`bitboard-eval`。 |
| GUI | `src/surakarta_gui_main.cpp` | Dear ImGui 界面、棋盘渲染、Depth/Threads、Preset、Last Move/PV/Debug stats。 |
| GUI session | `include/surakarta_dev_session.h`、`src/surakarta_dev_session.cpp` | GUI 状态机、加载局面、人类走法、异步引擎搜索、undo。 |
| 规则管理 | `src/surakarta_rule_manager_impl.cpp`、`src/surakarta_rule_manager_bitboard.cpp` | legacy traditional rule manager 与 bitboard rule manager。 |
| 训练/selection 脚本 | `scripts/benchmark-train.ps1`、`scripts/benchmark-select.ps1`、`scripts/common.ps1` | TD smoke、multi-seed selection、manifest、构建工具定位。 |
| workflow/perf | `scripts/workflow.ps1`、`scripts/perf-runner.ps1`、`scripts/workflow.tests.ps1` | 默认回归、性能基线门禁、Pester 脚本测试。 |
| C++ 自测 | `src/surakarta_bitboard_selftest.cpp` | 规则、搜索、训练、CLI、mask、P0 regression 自测。 |
| GUI/session 自测 | `src/surakarta_dev_session_selftest.cpp` | GUI session 选择、hover、engine reply、undo、telemetry、终局状态。 |
| GTest legacy | `test/surakarta_rule_manager_test.cpp` | legacy TA 对比测试；当前默认 CTest 不一定包含它，取决于第三方配置。 |
| 测试数据 | `test/test_data/*.txt`、`test/test_data/perf-baseline.json` | 固定局面和性能基线。 |
| 过程文档 | `README.md`、`task_plan.md`、`findings.md`、`progress.md` | 工作流、Phase 状态、P0 修复、训练/selection 风险。 |

## 8. 编译、运行与演示命令

> 当前本机已有构建目录是 `build`。脚本标准目录是 `build-vs` / `build-vs-release` / `build-vs-train-release`，由 `scripts/common.ps1` 决定。来源：`scripts/common.ps1:16-27`。

### 8.1 标准 clean build / release build

```powershell
pwsh ./scripts/workflow.ps1 -Configuration Debug
pwsh ./scripts/workflow.ps1 -Configuration Release
```

说明：`workflow.ps1` 会 configure、clean build 指定目标、运行 CTest、检查遗留 `surakarta-*.exe`。来源：`scripts/workflow.ps1:13-21`。

### 8.2 当前已有 build 目录的演示构建

```powershell
$cmake = (Select-String -Path build\CMakeCache.txt -Pattern '^CMAKE_COMMAND:INTERNAL=(.*)$').Matches[0].Groups[1].Value
& $cmake --build build --target surakarta-bitboard-selftest surakarta-dev-session-selftest surakarta-gui
```

本轮结果：`ninja: no work to do.`。

### 8.3 运行 GUI

```powershell
.\build\bin\surakarta-gui.exe
.\build\bin\surakarta-gui.exe --depth 10 --threads 10 --human black
.\build\bin\surakarta-gui.exe --file .\test\test_data\game1.txt --depth 10 --threads 10 --human black
```

GUI 参数来源：`src/surakarta_gui_main.cpp:877-908`。

### 8.4 运行 selftest

```powershell
.\build\bin\surakarta-bitboard-selftest.exe
.\build\bin\surakarta-dev-session-selftest.exe
```

本轮结果：两者均 `[PASS]`。

### 8.5 运行 CTest

```powershell
$ctest = (Select-String -Path build\CMakeCache.txt -Pattern '^CMAKE_CTEST_COMMAND:INTERNAL=(.*)$').Matches[0].Groups[1].Value
& $ctest --test-dir build --output-on-failure
```

本轮结果：`100% tests passed, 0 tests failed out of 8`，总耗时约 `18.34 sec`。

### 8.6 运行 Pester workflow tests

```powershell
Invoke-Pester -Script .\scripts\workflow.tests.ps1 -PassThru -Quiet
```

本轮结果：`TotalCount=26, PassedCount=26, FailedCount=0`。输出中有红色 Exception，是负例 harness 测试故意触发的脚本异常，按 Pester 统计不是失败。

### 8.7 GUI smoke tests

```powershell
.\build\bin\surakarta-gui.exe --smoke-test --file .\test\test_data\game1.txt
.\build\bin\surakarta-gui.exe --smoke-test-init
.\build\bin\surakarta-gui.exe --smoke-test-geometry
.\build\bin\surakarta-gui.exe --smoke-test-coordinate-convention
.\build\bin\surakarta-gui.exe --smoke-test-sidebar-labels
```

CTest 已覆盖这些 smoke。来源：`src/CMakeLists.txt:129-143`。

### 8.8 Benchmark / 搜索演示命令

```powershell
.\build\bin\surakarta-benchmark.exe bitboard-search --depth 10 --threads 10 --format json --case-id opening
.\build\bin\surakarta-benchmark.exe bitboard-benchmark --depth 10 --threads 10 --format json --case-id middlegame --file .\test\test_data\game1.txt
```

CLI 来源：`src/main.cpp:1008-1075`、`src/main.cpp:1506-1526`。

## 9. 测试与验证结果

| 测试 | 本轮结果 | 验证内容 |
|---|---:|---|
| `surakarta-bitboard-selftest` | `[PASS]` | quiet mask、traditional/bitboard parity、P0 golden capture、make/unmake、qsearch/search depth-1、terminal parity、PVS/reference parity、node budget、stop request、多线程等价、PV bounds、aspiration stats、benchmark JSON、TD smoke、training reproducibility、mask capacity invariant、static mask parity、debug target/path。来源：`src/surakarta_bitboard_selftest.cpp:2132-2212`。 |
| `surakarta-dev-session-selftest` | `[PASS]` | GUI session 选子、hover capture path、人类走法后引擎回复、切换人类方取消搜索、undo、live snapshot/PV/counters、search cancellation、终局状态。来源：`src/surakarta_dev_session_selftest.cpp:424-444`。 |
| CTest | `8/8 passed` | 集成 bitboard/dev-session selftest、mask capacity fail-fast、GUI session/init/geometry/coordinate/sidebar smoke。来源：`src/CMakeLists.txt:41-84`、`src/CMakeLists.txt:129-143`。 |
| Pester workflow | `26 passed / 0 failed` | PowerShell 构建、configure、workflow clean-first、manifest、perf/selection gate、fake fixture 等脚本层行为。来源：`scripts/workflow.tests.ps1`。 |
| GUI smoke | `[PASS]` | sidebar 默认 Match 10/10、Debug stats 默认折叠、搜索中禁用设置、Last Move/Thinking/PV label、坐标约定、棋盘几何。来源：`src/surakarta_gui_main.cpp:430-554`。 |
| rule correctness golden tests | 已包含于 bitboard selftest | traditional vs bitboard、full/capture-only movegen、中心/边/角/内外环/多路径/阻挡/敌方目标/普通移动。来源：`src/surakarta_bitboard_selftest.cpp:743-857`。 |
| capture variant capacity invariant | 已包含于 bitboard selftest + CTest fail-fast | 当前最大 capture variants 为 8；容量不足不再静默截断。来源：`src/surakarta_bitboard_selftest.cpp:2020-2069`、`src/CMakeLists.txt:67-84`。 |

## 10. PPT 建议结构（11 页）

1. **项目背景**：苏拉卡尔塔棋简介、校赛对战场景、当前版本定位。
2. **规则简介**：6x6 棋盘、普通移动、圆弧吃子、必须过弧线、路径阻挡、胜负判断；终局 debt 单独留到不足页。
3. **系统架构**：GUI / DevelopmentSession / bitboard movegen / search / tests / scripts。
4. **棋盘表示**：`uint64_t` bitboard、Position、Zobrist、precomputed masks。
5. **走法生成与圆弧吃子**：quiet mask、capture variants、clear mask、path reconstruction、P0 regression 修复。
6. **搜索算法**：fixed-depth PVS、iterative deepening、aspiration、TT、qsearch、move ordering。
7. **多线程与性能**：Lazy SMP 风格 root parallel、Threads 参数、D10/T10 calibration、为什么不用 20 线程。
8. **GUI 演示**：加载局面、Human Black/White、Depth/Threads、Last Move、Thinking/PV、Undo Human Turn。
9. **规则正确性与测试**：golden parity、capacity invariant、CTest/Pester/selftest 结果。
10. **校赛配置**：Match preset D10/T10、人工转达流程、fixed-depth 使用边界。
11. **不足与后续计划 / 总结**：TD-Learning 仍 trend validation failed、Phase 4 blocked、终局规则 debt、后续严格实现和训练稳定化。

## 11. 答辩视频演示流程（5-8 分钟）

### 0:00-0:40 开场

- 介绍项目：苏拉卡尔塔棋程序，当前校赛版本是 GUI 人机对战工具。
- 强调当前版本核心：bitboard movegen + fixed-depth PVS + 多线程搜索。

### 0:40-1:30 编译

```powershell
pwsh ./scripts/workflow.ps1 -Configuration Release
```

如果现场时间紧，展示已有构建：

```powershell
$cmake = (Select-String -Path build\CMakeCache.txt -Pattern '^CMAKE_COMMAND:INTERNAL=(.*)$').Matches[0].Groups[1].Value
& $cmake --build build --target surakarta-bitboard-selftest surakarta-dev-session-selftest surakarta-gui
```

### 1:30-2:30 运行测试

```powershell
.\build\bin\surakarta-bitboard-selftest.exe
.\build\bin\surakarta-dev-session-selftest.exe
$ctest = (Select-String -Path build\CMakeCache.txt -Pattern '^CMAKE_CTEST_COMMAND:INTERNAL=(.*)$').Matches[0].Groups[1].Value
& $ctest --test-dir build --output-on-failure
Invoke-Pester -Script .\scripts\workflow.tests.ps1 -PassThru -Quiet
```

讲重点：规则 golden、P0 capture regression、traditional/bitboard parity、GUI smoke、workflow tests 全过。

### 2:30-3:20 打开 GUI 并设置校赛参数

```powershell
.\build\bin\surakarta-gui.exe --depth 10 --threads 10 --human black
```

展示右侧 Match preset、Depth 10、Threads 10、Human Black/White、Board File。

### 3:20-4:40 演示人机对战流程

- 手动选择一枚己方棋子。
- 展示合法落点和吃子路径提示。
- 输入对手走法或做一次人类走法。
- 等 AI 思考并自动落子。
- 指出 `Last Move` 是要人工转达到对手电脑的走法。

### 4:40-5:40 演示一次引擎输出和调试信息

- 搜索中展示 `Thinking`、PV。
- 打开 `Debug / Search Stats`，简要说明 Depth、Nodes、TT Hits、Null Prunes、LMR、Aspiration 是工程调试指标，不是比赛必需信息。

### 5:40-6:50 算法说明

- bitboard：两个 `uint64_t` 表示黑白棋。
- movegen：预计算 quiet/capture masks，吃子用 variants + clear mask。
- search：fixed-depth PVS + qsearch + TT + aspiration + Lazy SMP。
- 性能：D10/T10 平均约 0.482s，最慢约 1.975s；没有严格 time management，所以靠 fixed-depth preset 控制耗时。

### 6:50-7:40 当前不足和后续计划

- TD-Learning 已有工具链，但 Phase 3 trend validation failed；不能说训练完成。
- Phase 4 blocked，后续才做 threat/mobility/stage 特征扩展。
- legacy no-capture 终局不是大赛第 6 条严格实现，后续要单独补。

### 7:40-8:00 总结

- 当前校赛可用版本重点是：规则正确性加固 + GUI 可用 + 搜索强度可控。
- 结尾一句可以说：“这个版本不靠玄学训练权重吹强度，主要靠已经验证的 bitboard movegen 和 fixed-depth 多线程 PVS 搜索打比赛。”

## 12. PPT 表述红线

### 适合写进 PPT

- `bitboard + masks + capture variants` 的走法生成。
- P0 capture-rule regression 的复现、修复、fail-fast 和 golden tests。
- PVS / iterative deepening / aspiration / TT / qsearch / move ordering / LMR / null move / Lazy SMP 已实现。
- GUI Match preset `Depth 10 / Threads 10`、人工转达对战流程、Undo Human Turn。
- CTest / Pester / selftest / GUI smoke 的通过结果。
- TD/self-play 训练闭环“已搭建，处于诊断和验证阶段”。

### 只能写后续计划，不能写成已完成

- Phase 3 completed / trend validation passed。
- Phase 4 已开始或已完成。
- threat-aware、mobility、stage-aware、扩展 N-Tuple 特征已用于校赛。
- TD-Learning 训练出了稳定最强权重。
- suspect/provisional 旧 artifacts 证明强度提升。
- 真正 time management / 按秒自动控时。
- 大赛第 6 条“双方都不能再吃子”的严格终局实现。

