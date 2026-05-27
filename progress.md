# 进度日志

## 会话：2026-05-26（阶段 3.62：commit staging plan and tracked evidence materialization）

### 状态
- **阶段 3.62：** completed。
- **执行边界：** staging plan / tracked evidence summary / pathspec and hunk boundary only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_62_commit_staging_plan`。
- **仍不允许：** stage、commit、stash、reset、checkout、clean、active probe、dry-run、training、selection、promotion、`GamesPerSeed=64`。

### 已执行操作
- 按 planning 文件恢复上下文，并补读适用技能说明。
- 运行锚点检查：当前分支为 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 确认 `git diff --cached --name-only` 为空。
- 复核 Phase 3.60/3.61 manifest 与 Phase 3.61 ownership review JSON。
- 运行 `git check-ignore -v`，确认 Phase 3.60/3.61 evidence markdown 命中 `.gitignore:4:/build-local/`。
- 读取四个重点 mixed code 文件 diff 前 320 行，确认仍需要 partial staging 边界，不能 whole-file stage。
- 创建 Phase 3.62 输出目录，并生成 `commit-staging-plan.md`、`commit-staging-plan.json`、`commit-staging-plan-manifest.json`、`verify_phase3_62.ps1`。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`，把 ignored evidence 的摘要、hash、路径索引和复跑命令固化到 tracked 文档。

### 遇到的错误
| 错误 | 尝试次数 | 解决方案 |
|------|---------|---------|
| 初次读取两个技能文件时误用了 `.system` 子路径 | 1 | 改用实际路径 `C:\Users\Lenovo\.codex\skills\...` 补读；不影响仓库文件 |
| 嵌套 `pwsh -Command` 中逗号分隔 `git check-ignore` pathspec 被 PowerShell 传成畸形单参数 | 1 | 使用空格分隔 pathspec 的等价命令复核，确认两条 evidence markdown 均命中 `.gitignore:4:/build-local/` |
| 嵌套 `pwsh -Command` 中 `$_.Name` 被外层提前展开为 `.Name` | 1 | 用反引号转义 `$_.Name` 后复跑检查点 6，cached diff、禁用产物、禁词扫描均为空输出 |

### 当前结论
- Phase 3.62 已形成后续 staging execution 的人工操作计划，但本轮没有执行 staging 或 commit。
- `build-local` 证据包按 ignore 策略保留本地，tracked docs 提供摘要/hash/rerun commands。
- 下一步只建议人工授权后的 staging execution campaign 或 Phase 4 precondition planning；不进入 training/selection/promotion。

## 会话：2026-05-26（阶段 3.61：evidence archive and commit preparation ownership review）

### 状态
- **阶段 3.61：** completed。
- **执行边界：** archive index / ownership review / commit preparation only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_61_evidence_archive_and_commit_preparation`。
- **仍不允许：** stage、commit、stash、reset、checkout、clean、active probe、dry-run、training、selection、promotion、`GamesPerSeed=64`。

### 已执行操作
- 读取 planning 文件恢复上下文。
- 运行锚点检查：当前分支为 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户指定锚点。
- 复核当前 `git status --short` 与 protected diff 集合，确认仍是历史脏状态加允许文档更新。
- 查看四个相关代码文件 diff hunk：`include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- 识别混合 hunk：objective diagnostic 相关 hunk 可作为 future partial staging 候选；national terminal semantics、earlier opening-safe objective plumbing、perf baseline、untracked tools 不能 whole-file 混入 Phase 3.50-3.60 commit group。
- 创建 Phase 3.61 输出目录，并生成 `archive-and-commit-preparation.md`、`archive-and-commit-preparation.json`、`archive-and-commit-preparation-manifest.json`、`verify_phase3_61.ps1`。
- 运行 Phase 3.61 verify，取得 `PHASE3_61_VERIFY_OK`；仅出现既有 LF/CRLF warning。

### 当前结论
- Phase 3.61 ownership review 已完成。
- 下一步只建议 commit staging plan 或 Phase 4 precondition planning；不建议直接 stage/commit，更不能进入 training/selection/promotion。

## 会话：2026-05-26（阶段 3.60：final evidence consolidation and Phase 4 precondition map）

### 状态
- **阶段 3.60：** completed。
- **执行边界：** final evidence consolidation / status matrix / gap list / Phase 4 precondition map only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_60_final_evidence_consolidation`。
- **仍不允许：** active probe、dry-run、training、selection、promotion、`GamesPerSeed=64`、权重/checkpoint/candidate/weights-manifest、Phase 4 执行。

### 已执行操作
- 读取 planning 文件恢复上下文。
- 运行锚点检查：当前分支为 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户指定锚点。
- 记录当前 `git status --short` 仍存在历史脏改动；本轮只允许新增 Phase 3.60 输出目录并更新四个允许文档。
- 枚举 Phase 3.47-3.59 输入目录文件，确认各阶段证据包存在。
- 只读扫描 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 中 objective/report-only/gate 相关 marker；未编辑 protected 文件。
- 解析 Phase 3.47-3.59 manifest 状态矩阵：3.47-3.55 objective marker 均为 false，3.56-3.57 为 true，3.58-3.59 标记 diagnostic blocker closed。
- 复核 Phase 3.53-3.54 与 Phase 3.56-3.57 dry-run stdout 差异：前者 objective marker false，后者 true；两者均保持 no-output 和 gate isolation。
- 创建 Phase 3.60 输出目录，并生成 `final-evidence-consolidation.md`、`final-evidence-consolidation.json`、`final-evidence-consolidation-manifest.json`、`verify_phase3_60.ps1`。
- 运行 Phase 3.60 verify，取得 `PHASE3_60_VERIFY_OK`；仅出现既有 LF/CRLF warning。

### 当前结论
- Phase 3.60 final evidence consolidation 已完成。
- Phase 4 precondition map 只允许规划，不授权 execution、training、selection 或 promotion。

## 会话：2026-05-26（阶段 3.58-3.59：objective evidence review and completion gate）

### 状态
- **阶段 3.58-3.59：** completed。
- **执行边界：** objective evidence review / Phase 3 completion gate decision / documentation and verification package only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_58_59_objective_evidence_review_and_completion_gate`。
- **仍不允许：** 新 dry-run active probe、training、selection、promotion、`GamesPerSeed=64`、生产候选、权重/checkpoint/candidate/weights-manifest、Phase 4。

### 已执行操作
- 读取 planning 文件恢复上下文。
- 运行锚点检查：当前分支为 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户指定锚点。
- 记录当前 `git status --short` 存在历史脏改动；本轮只允许新增 Phase 3.58-3.59 输出目录并更新四个允许文档。
- 开始复核 Phase 3.52、3.53-3.54、3.55、3.56-3.57 输入证据目录。
- 复核 Phase 3.56-3.57 manifest/report/dry-run stdout：objective marker 为 true，report-only/no-output 与 artifact suppression 为 true，`games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- 只读复核四个源码文件中 summary/status/selftest marker；未编辑 `src` 或 `include`。
- 创建 Phase 3.58-3.59 输出目录，并生成 `report.md`、`report.json`、`manifest.json`、`verify_phase3_58_59.ps1`。
- 运行 Phase 3.58-3.59 verify，取得 `PHASE3_58_59_VERIFY_OK`。
- 构建 `surakarta-benchmark`，exit code `0`，输出 `ninja: no work to do.`。
- 运行 `surakarta-benchmark.exe bitboard-selftest`，exit code `0`，输出 `[PASS] surakarta-bitboard-selftest`。
- 运行 Phase 3.56-3.57 verify，exit code `0`，取得 `PHASE3_56_57_VERIFY_OK`。

### 遇到的错误
| 错误 | 尝试次数 | 解决方案 |
|------|---------|---------|
| PowerShell JSON 汇总命令在 `foreach` 后接管道导致 `An empty pipe element is not allowed` | 1 | 改为先收集 `$items` 再 `ConvertTo-Json`，不重复同一错误写法 |
| 嵌套 `pwsh -Command` 中 `$exe` 被外层 PowerShell 提前展开，导致 `Missing expression after unary operator '-not'` | 1 | 在工具调用中用反引号转义 `$exe` 后重跑，selftest 通过 |

### 当前结论
- Phase 3 diagnostic objective blocker 可判定 closed。
- Phase 3 整体仍不是通过态；Phase 4、training、selection、promotion 继续 no-go。

## 会话：2026-05-26（阶段 3.56-3.57：objective-execution report-only blocker closure）

### 状态
- **阶段 3.56-3.57：** completed。
- **执行边界：** minimal implementation + guarded report-only/no-output verification。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure`。
- **仍不允许：** training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 在 `session-baseline-sha256.json` 记录 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 的修改前 SHA256。
- TDD RED：先修改 selftest，要求 guarded no-output path 设置 objective diagnostic evidence true，并要求 ordinary output path 仍 false；运行 selftest 失败于两条 Phase 3.56 断言。
- GREEN：修改 `src\surakarta_search.cpp`，只在 guarded report-only/no-output path 下设置 `active_objective_probe_executed=true` 并输出 evidence status。
- 构建过程中并行 target 一次撞到 Windows obj 文件锁，随后顺序重跑 target 成功。
- 运行 `surakarta-benchmark.exe bitboard-selftest`，取得 `[PASS] surakarta-bitboard-selftest`。
- 执行一次隔离 dry-run：`surakarta-benchmark.exe bitboard-train --config <guarded-report-only-no-output-objective-config.json> --format json`，exit code `0`，stderr 为空。
- dry-run stdout 显示 guarded evidence true，同时 `games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- 扫描 Phase 3.56-3.57 输出目录，未发现禁用产物。
- 生成 `report.md`、`report.json`、`manifest.json`、`verify_phase3_56_57.ps1`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\src\surakarta_search.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\session-baseline-sha256.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\guarded-report-only-no-output-objective-config.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\logs\dry-run-stdout.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\logs\dry-run-stderr.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\logs\dry-run-exit-code.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\logs\forbidden-artifact-scan.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\logs\protected-diff-scan.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure\verify_phase3_56_57.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Objective diagnostic execution evidence 在 guarded report-only/no-output dry-run 分层成立。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`，selection/promotion 仍 no-go。
- 下一阶段只建议 Phase 3.58 objective evidence review / Phase 3 completion gate review。

## 会话：2026-05-26（阶段 3.55：dry-run evidence interpretation and blocker closure）

### 状态
- **阶段 3.55：** completed。
- **执行边界：** evidence interpretation / source-readonly review / blocker closure package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure`。
- **仍不允许：** 再次执行 active probe、training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 复跑 Phase 3.53-3.54 verify，取得全部 Phase 3.53-3.54 marker。
- 复核 Phase 3.53-3.54 dry-run stdout/report/manifest，确认 command path runnable 与 no-output/artifact suppression。
- 只读审查 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- 定位 false blocker：当前 path 只设置 report-only path indicator；`active_objective_probe_executed` 在 parser 与 summary 中保持 false；no-output 分支提前返回，不进入 objective diagnostic execution。
- 生成 Phase 3.55 `report.md`、`report.json`、`manifest.json`、`verify_phase3_55.ps1` 和两份 scan log。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\verify_phase3_55.ps1`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\logs\forbidden-artifact-scan.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure\logs\protected-diff-scan.txt`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.55 只完成解释和 blocker closure。
- `active_objective_probe_executed=false` 继续保持，Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 下一阶段只建议 Phase 3.56 objective-execution blocker closure plan 或单独授权的最小实现任务。

## 会话：2026-05-26（阶段 3.53-3.54：guarded report-only probe readiness and dry-run）

### 状态
- **阶段 3.53-3.54：** completed。
- **执行边界：** readiness closure + guarded report-only/no-output dry-run evidence campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run`。
- **仍不允许：** training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 读取并复核 Phase 3.51 / Phase 3.52 输入证据、Phase 3.52 manifest/report/verify，以及相关 no-output path 源码边界。
- 运行 `cmake --build ... --target surakarta-benchmark`，exit code `0`。
- 运行 `surakarta-benchmark.exe bitboard-selftest`，exit code `0`，输出 `[PASS] surakarta-bitboard-selftest`。
- 运行 Phase 3.52 verify，exit code `0`，取得 `PHASE3_52_SESSION_BASELINE_OK`、`PHASE3_52_STATUS_FLAGS_OK`、`PHASE3_52_REPORT_ONLY_PATH_IMPLEMENTED_OK`、`PHASE3_52_ARTIFACT_AND_PROTECTED_BOUNDARY_OK`、`PHASE3_52_VERIFY_OK`。
- 创建隔离 config `guarded-report-only-no-output-config.json`。
- 执行一次 no-output dry-run：`surakarta-benchmark.exe bitboard-train --config <config> --format json`，exit code `0`，stderr 为空。
- dry-run stdout 显示 `active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`active_objective_probe_executed=false`。
- 扫描 Phase 3.53-3.54 输出目录，未发现禁用产物。
- 生成 report/json/manifest/verify 脚本。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\guarded-report-only-no-output-config.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\logs\dry-run-stdout.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\logs\dry-run-stderr.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\logs\dry-run-exit-code.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\logs\forbidden-artifact-scan.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\logs\protected-diff-scan.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\guarded-report-only-probe-readiness-and-dry-run-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\guarded-report-only-probe-readiness-and-dry-run-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\guarded-report-only-probe-readiness-and-dry-run-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run\verify_phase3_53_54.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- guarded report-only/no-output command path 可运行，且未生成禁用产物。
- `active_objective_probe_executed=false` 仍是硬事实；本阶段不是 training、selection 或 promotion 许可。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 下一阶段只建议 Phase 3.55 evidence review / diagnostics interpretation / continued blocker closure。

## 会话：2026-05-26（阶段 3.52：report-only active probe path minimal implementation）

### 状态
- **阶段 3.52：** completed。
- **执行边界：** minimal implementation / no execution permission。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation`。
- **仍不允许：** 真实 probe、training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 在输出目录记录 session-baseline hash：`include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- TDD RED：先在 selftest 中加入 `active_interface_report_only_probe_path` 断言，构建失败于缺少 `TrainingSummary` 成员。
- GREEN：新增 summary 字段、RunBitboardTraining no-output path 状态、CLI JSON/text 输出、自测断言与 benchmark selftest 转发入口。
- 构建 `surakarta-bitboard-selftest` 和 `surakarta-benchmark`，并通过 `surakarta-benchmark.exe bitboard-selftest` 运行自测。
- 生成 Phase 3.52 report/json/manifest/session-baseline/verify 脚本。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\include\surakarta_search.h`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_search.cpp`
- `D:\Dev\Code\surakarta_traditional\src\main.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation\session-baseline-sha256.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation\report-only-active-probe-path-minimal-implementation-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation\report-only-active-probe-path-minimal-implementation-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation\report-only-active-probe-path-minimal-implementation-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation\verify_phase3_52.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.52 完成最小 report-only path indicator 与 summary/selftest 接线。
- 这不是执行许可；状态仍为 Phase 3 `not_passed`、Phase 4 `blocked`。
- 建议下一阶段只进入 Phase 3.53 guarded execution readiness review，不进入 training/selection。

## 会话：2026-05-26（阶段 3.51：report-only active probe path implementation plan）

### 状态
- **阶段 3.51：** completed。
- **执行边界：** implementation-plan-only / planning-only / no execution。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan`。
- **仍不允许：** probe、training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 复跑 Phase 3.50 verify，取得 `PHASE3_49_BLOCKER_INPUT_OK_PHASE3_50`、`STATUS_FLAGS_OK_PHASE3_50`、`WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_50`、`PHASE3_50_VERIFY_OK`。
- 只读复核 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 的 config/no-output/summary/selftest 相关事实。
- 生成 Phase 3.51 三件套与 `verify_phase3_51.ps1`。
- `verify_phase3_51.ps1` 验证 Phase 3.50 输入、planning-only 状态、forbidden artifact、黑名单文案、历史 scoped diff 边界。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan\report-only-active-probe-path-implementation-plan.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan\report-only-active-probe-path-implementation-plan.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan\report-only-active-probe-path-implementation-plan-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan\verify_phase3_51.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.51 只完成 future Phase 3.52 最小实现任务计划。
- Phase 3.52 可作为单独授权的最小实现任务开启。
- Phase 3.51 未执行 probe/training/selection，未改变 readiness 或 Phase 状态。

## 会话：2026-05-26（阶段 3.50A：acceptance audit addendum）

### 状态
- **阶段 3.50A：** completed。
- **执行边界：** acceptance-audit-addendum / no conclusion change / no Phase 3.51 entry。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan`。
- **仍不允许：** active probe、training、selection、promotion、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、gate/rating/scorecard/perf threshold 修改。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 记录本轮前保护范围状态：`src/include/test_data` 存在历史 scoped diff；`tools` 为未跟踪目录但本轮不编辑。
- 更新 `verify_phase3_50.ps1` marker，使其输出验收要求的 `PHASE3_49_BLOCKER_INPUT_OK_PHASE3_50`、`STATUS_FLAGS_OK_PHASE3_50`、`WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_50`。
- 新增 `guarded-active-probe-acceptance-audit-addendum.md` 与 `guarded-active-probe-acceptance-audit-addendum-manifest.json`。
- Addendum 记录历史 scoped diff 清单，并给出 protected scripts 与 tools 的文件级 SHA256 表。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\guarded-active-probe-acceptance-audit-addendum.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\guarded-active-probe-acceptance-audit-addendum-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\verify_phase3_50.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.50A 只补验收证据，不改变 Phase 3.50 结论。
- `ready_for_guarded_probe=false`、Phase 3 `not_passed`、Phase 4 `blocked` 保持不变。
- Phase 3.51 未进入；后续最多可开 planning-only，仍需单独授权。

## 会话：2026-05-26（阶段 3.50：guarded active probe blocker closure plan）

### 状态
- **阶段 3.50：** completed。
- **执行边界：** blocker-closure-plan / readiness-gate / no execution / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan`。
- **仍不允许：** active probe execution、training、selection、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint/weights-manifest`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚点。
- 记录工作区已有历史脏改动；本轮不认领既有 `src/include/tools/test_data` 状态。
- 复核 Phase 3.47、Phase 3.48、Phase 3.49 输入目录与 manifest。
- 生成 Phase 3.50 三件套：`guarded-active-probe-blocker-closure-plan.md`、`guarded-active-probe-blocker-closure-plan.json`、`guarded-active-probe-blocker-closure-plan-manifest.json`。
- 创建 `verify_phase3_50.ps1`，验证锚点、输入链、Phase 3.50 状态位、forbidden artifact、禁用文案和 protected tracked diff。
- 未执行 probe、training、selection，也未调用 benchmark-select 或 benchmark-train。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\guarded-active-probe-blocker-closure-plan.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\guarded-active-probe-blocker-closure-plan.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\guarded-active-probe-blocker-closure-plan-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan\verify_phase3_50.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.50 将 blocker 收束为真实 report-only active probe path 未被证明。
- Phase 3.51 可开工，但范围只能是 report-only probe path implementation plan 或单独授权的最小实现任务。
- `ready_for_guarded_probe=false` 保持不变。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。

## 会话：2026-05-26（阶段 3.49：guarded active probe execution readiness review）

### 状态
- **阶段 3.49：** completed。
- **执行边界：** readiness-only / no execution / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review`。
- **仍不允许：** 真实 probe、training、selection、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚点。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/test/tools` diff。
- 复核 Phase 3.47 manifest 与 Phase 3.48 manifest，确认 skeleton/no-output 和 verification-only 证据链成立。
- 生成 Phase 3.49 三件套：`guarded-active-probe-execution-readiness-review.md`、`guarded-active-probe-execution-readiness-review.json`、`guarded-active-probe-execution-readiness-review-manifest.json`。
- 创建 `verify_phase3_49.ps1`，验证锚点、输入 manifests、Phase 3.49 状态、forbidden artifact、禁词和 protected tracked diff。
- 未执行 probe、training、selection，也未调用 benchmark-select 或 benchmark-train。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review\guarded-active-probe-execution-readiness-review.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review\guarded-active-probe-execution-readiness-review.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review\guarded-active-probe-execution-readiness-review-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review\verify_phase3_49.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.49 readiness 结论为 `ready_for_guarded_probe=false`。
- 当前只证明 skeleton/no-output guard，不证明真实 report-only probe path 已经可用。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 不建议进入 Phase 3.50；先补真实 report-only probe path 评审或实现验证，再谈执行。

## 会话：2026-05-25（Phase 3.38 guarded limited training dry-run plan）

### 状态
- **Phase 3.38：** completed as training-dry-run-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_38_guarded_limited_training_dry_run_plan`。
- **生成产物：** `guarded-limited-training-dry-run-plan.md`、`guarded-limited-training-dry-run-plan.json`、`guarded-limited-training-dry-run-plan-manifest.json`。

### 执行记录
- 复核分支与 HEAD：`codex/phase2-baseline-trace`，`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致。
- 读取并恢复 `task_plan.md`、`findings.md`、`progress.md`。
- 复核 Phase 3.37 / 3.36 / 3.35 / 3.33 输入目录与 JSON 证据链。
- 复核 `scripts\benchmark-train.ps1` 参数与默认输出行为；确认它不适合 Phase 3.39 主隔离入口。
- 复核 `bitboard-train` CLI 参数；确认当前没有 opening objective activation flags。
- 生成 future Phase 3.39 dry-run 的命令模板、输入 artifact、输出隔离目录、参数边界、监控指标、停止条件、rollback triggers 和证据包结构。
- 明确本轮未训练、未 selection、未生成 `.bin`，未修改代码、tools、protected scripts 或 gate。

### 验证结果
- 检查点 1：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致；工作区仍存在历史 `src/include/tools` 等脏状态。
- 检查点 2：输入 JSON 均可解析，输出包含 `input-json-ok`。
- 检查点 3：Phase 3.38 manifest 状态位通过，输出 `STATUS_FLAGS_OK_PHASE3_38`。
- 检查点 4：文案黑名单与 no-bin 通过，`phase3_38_bin_count=0`，输出 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_38`。
- 检查点 5：protected path diff 为空；`src/include` diff 仅按历史脏状态复核，输出 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_38`。

### 下一步
- 建议进入 Phase 3.39 guarded limited training dry-run execution 或 blocker review。
- Phase 3.39 只能按 default-off / isolated / quarantine-only 范围执行；不允许 selection、`GamesPerSeed=64`、promotion、gate 变更或 Phase 4。

## 会话：2026-05-25（Phase 3.37 guarded limited training plan review）

### 状态
- **Phase 3.37：** completed as training-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_37_guarded_limited_training_plan_review`。
- **生成产物：** `guarded-limited-training-plan-review.md`、`guarded-limited-training-plan-review.json`、`guarded-limited-training-plan-review-manifest.json`。

### 执行记录
- 复核分支与 HEAD：`codex/phase2-baseline-trace`，`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致。
- 读取并恢复 `task_plan.md`、`findings.md`、`progress.md`。
- 复核 Phase 3.36 / 3.35 / 3.34 / 3.33 / 3.32 输入目录与 JSON 证据链。
- 生成 future guarded limited training 的前置条件、参数边界、监控指标、rollback triggers、禁用项、停止条件和通过/阻塞判据。
- 明确本轮未训练、未 selection、未生成 `.bin`，未修改代码、tools、protected scripts 或 gate。

### 验证结果
- 检查点 1：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致；工作区存在历史 `src/include/tools` 等脏状态。
- 检查点 2：输入 JSON 均可解析，输出包含 `input-json-ok`。
- 检查点 3：Phase 3.37 manifest 状态位通过，输出 `STATUS_FLAGS_OK_PHASE3_37`。
- 检查点 4：文案黑名单与 no-bin 通过，`phase3_37_bin_count=0`，输出 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_37`。
- 检查点 5：protected path diff 为空；`src/include` diff 仅按历史脏状态复核，输出 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_37`。

### 下一步
- 建议进入 Phase 3.38 guarded limited training dry-run plan 或 blocker review。
- 不允许从 Phase 3.37 直接启动训练、selection、`GamesPerSeed=64`、promotion 或 Phase 4。

## 会话：2026-05-25（Phase 3.35 guarded diagnostic dry-run plan）

### 状态
- **Phase 3.35：** completed as dry-run plan only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_35_guarded_diagnostic_dry_run_plan`。
- **生成产物：** `guarded-diagnostic-dry-run-plan.md`、`guarded-diagnostic-dry-run-plan.json`、`guarded-diagnostic-dry-run-plan-manifest.json`。

### 执行记录
- 复核分支与 HEAD：`codex/phase2-baseline-trace`，`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致。
- 复核 Phase 3.34 和 Phase 3.33 输入 JSON 可解析。
- 生成 future diagnostic dry-run 的输入、输出、指标、激活方式、禁用项和 rollback triggers。
- 复核 Phase 3.35 manifest 状态位、文案黑名单、no-bin 和 code/script protected diff 检查。

### 下一步
- 建议进入 Phase 3.36 guarded diagnostic dry-run execution plan 或 blocker review。
- 仍不允许训练、selection、`GamesPerSeed=64`、`.bin` 生成或 gate 改动。

## 会话：2026-05-25（Phase 3.34 guarded implementation verification package）

### 状态
- **Phase 3.34：** completed as verification-only / default-off / not selection eligible package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_34_guarded_implementation_verification_package`。
- **生成产物：** `guarded-implementation-verification-package.md`、`guarded-implementation-verification-package.json`、`guarded-implementation-verification-package-manifest.json`。

### 执行记录
- 复核分支与 HEAD：`codex/phase2-baseline-trace`，`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，与锚定 commit 一致。
- 构建并运行 `surakarta-bitboard-selftest`，得到 `BUILD_AND_SELFTEST_OK_PHASE3_34`。
- 复核 Phase 3.33 manifest，得到 `PHASE3_33_MANIFEST_OK`。
- 复核 Phase 3.34 manifest 状态位，得到 `STATUS_FLAGS_OK_PHASE3_34`。
- 复核 Phase 3.34 no-bin 和 protected diff，得到 `NO_BIN_AND_PROTECTED_DIFF_OK`。

### 下一步
- 建议进入 Phase 3.35 guarded diagnostic dry-run plan。
- 仍不允许训练、selection、`GamesPerSeed=64`、`.bin` 生成或 gate 改动。

## 会话：2026-05-25（Phase 3.33 guarded minimum-objective implementation）

### 状态
- **Phase 3.33：** completed as guarded implementation / default-off / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_33_guarded_minimum_objective_implementation`。
- **生成产物：** `guarded-minimum-objective-implementation-report.md`、`guarded-minimum-objective-implementation-report.json`、`guarded-minimum-objective-implementation-manifest.json`。
- **objective：** 已实现最小 scoped formula path。
- **default-off：** 默认关闭；默认 options 保持 disabled/zero/empty/false。
- **training loss：** active scoped path 修改 TD target，记录为 `training_loss_modified=true`；默认关闭路径自测等价。
- **tools/scripts/gate：** 未修改。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.33 输出目录未生成。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 执行的操作
- 复核分支、HEAD 和工作区状态；HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 在允许代码面内增加 opening-safe objective option contract、guarded formula helper、ApplyTrainingStep active scoped hook 和 report-only diagnostic fields。
- 在 selftest 中覆盖 default-off、inactive equivalence、scoped activation、active scoped formula、hard reject report-only 边界。
- 处理 selftest 中 trace CLI 子进程崩溃：根因是 `surakarta-benchmark.exe` 未按当前 `ApplyTrainingStep` ABI 重新链接；重建后通过。
- 生成 Phase 3.33 三件套并更新四个项目记录文件。

### 核心结论
- 最小 objective 代码路径存在，但默认关闭；关闭状态经自测证明 checked 行为等价。
- hard reject 仍只作为 report-only companion，不进入 rating、scorecard、perf threshold 或 selection gate。
- 下一阶段只建议进入 verification/diagnostic review；任何训练、selection、`.bin` 或 gate 行为仍需重新授权。

## 会话：2026-05-24（Phase 3.32 minimum objective implementation plan review）

### 状态
- **Phase 3.32：** completed as implementation-plan-only / design-only / report-only review package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_32_minimum_objective_implementation_plan_review`。
- **生成产物：** `minimum-objective-implementation-plan-review.md`、`minimum-objective-implementation-plan-review.json`、`minimum-objective-implementation-plan-review-manifest.json`。
- **objective：** 未落地。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\*` / `include\*`。
- **tools：** 未修改。
- **scripts：** 未修改。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.32 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 复核当前分支、HEAD 和工作区状态；HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 读取并解析 Phase 3.31 report/manifest、Phase 3.30 skeleton package、Phase 3.29 hard reject fixture report。
- 生成 Phase 3.32 markdown report、JSON report 和 manifest。
- 更新四个项目记录文件。

### 核心结论
- 最小未来改动面、禁止改动面、测试矩阵、validation order、rollback triggers、hard reject report-only 消费点已经明确。
- hard reject 仍只进入 report-only acceptance companion，不进入 rating/scorecard/perf threshold/selection gate/training loss。
- 下一阶段可在单独授权后进入 guarded minimum-objective work；任何训练、selection、`.bin` 或 Phase 4 行为仍不能启动。

## 会话：2026-05-24（Phase 3.31 minimum objective formula design review）

### 状态
- **Phase 3.31：** completed as design-only / formula-design-only / report-only review package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_31_minimum_objective_formula_design_review`。
- **生成产物：** `minimum-objective-formula-design-review.md`、`minimum-objective-formula-design-review.json`、`minimum-objective-formula-design-review-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\*` / `include\*`。
- **tools：** 未修改。
- **scripts：** 未修改。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.31 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 复核当前分支、HEAD 和工作区状态；HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 读取并解析 Phase 3.22 design、Phase 3.24 dry-run plan、Phase 3.30 skeleton package 和 Phase 3.30 manifest。
- 生成 Phase 3.31 markdown report、JSON report 和 manifest。
- 更新四个项目记录文件。

### 核心结论
- 最小 objective 公式边界已明确：opening-only、default-off、relationship-based unsafe rank degradation、bounded root-cost multiplier、scope mask。
- hard reject 仍只进入 report-only acceptance companion，不进入 selection gate/rating/scorecard/perf threshold/training loss。
- 下一阶段可进入 implementation plan review，但只限 plan review；任何实现、训练或 `.bin` 都必须重新授权。

## 会话：2026-05-24（Phase 3.30 skeleton validation evidence package）

### 状态
- **Phase 3.30：** completed as skeleton validation evidence package / report-only summary。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_30_skeleton_validation_evidence_package`。
- **生成产物：** `skeleton-validation-evidence-package.json`、`skeleton-validation-evidence-package-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\*` / `include\*`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.30 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取并解析 Phase 3.25-3.29 的 10 个输入 JSON。
- 记录 `src\*`、`include\*`、`tools\*` 和三个 benchmark/perf 脚本保护范围 hash。
- 生成 Phase 3.30 JSON report 和 manifest。
- 更新四个项目记录文件。

### 核心结论
- Phase 3.25-3.29 的骨架验证链路已经足以进入 Phase 3.31 minimum objective formula design review。
- 该允许只针对设计评审；不授权 objective implementation、training loss 修改、训练、selection、`.bin`、Phase 3 pass 或 Phase 4 unblocked。

## 会话：2026-05-24（Phase 3.29 hard reject fixture proof）

### 状态
- **Phase 3.29：** completed as hard reject fixture proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_29_hard_reject_fixture_proof`。
- **生成产物：** `hard-reject-fixture-proof-report.json`、`hard-reject-fixture-proof-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 Phase 3.29 hard reject fixture proof marker 和 selftest-only fixture matrix。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.29 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取 Phase 3.22 design、Phase 3.24 dry-run plan、Phase 3.28 wiring proof。
- 记录保护范围 hash。
- TDD red check：确认 `Phase 3.29` marker 缺失并按预期失败。
- 在 selftest 中增加 Phase 3.29 hard reject report-only fixture proof。
- 生成 Phase 3.29 JSON report 和 manifest。
- 更新四个项目记录文件。

### 核心结论
- Phase 3.29 只证明 Phase 3.22 / 3.24 hard reject 条件已经有 selftest fixture 覆盖。
- hard reject 仍只作为 report-only evidence，不进入 selection gate/rating/scorecard/perf threshold。
- 本轮没有训练、selection、`.bin` 或 Phase 4 行为。

## 会话：2026-05-24（Phase 3.28 report-only diagnostics wiring proof）

### 状态
- **Phase 3.28：** completed as report-only diagnostics wiring proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_28_report_only_diagnostics_wiring_proof`。
- **生成产物：** `report-only-diagnostics-wiring-proof-report.json`、`report-only-diagnostics-wiring-proof-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 Phase 3.28 report-only wiring proof marker 和 selftest-only evidence matrix。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.28 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取 Phase 3.24 plan、Phase 3.26 report、Phase 3.27 report，以及三个 gate/benchmark 脚本。
- 记录保护范围 hash。
- TDD red check：确认 `report-only wiring` marker 缺失并按预期失败。
- 在 selftest 中增加 Phase 3.28 report-only diagnostics wiring proof。
- 生成 Phase 3.28 JSON report 和 manifest。
- 运行实盘验证：`VsDevCmd.bat -arch=x64 -host_arch=x64` + `build-vs-release` + target `surakarta-bitboard-selftest`，自测和 report-only wiring check 通过。
- 更新四个项目记录文件。

### 核心结论
- Phase 3.28 只证明 hard reject / opening guard / root-cost companion / acceptance companion 的 diagnostics 仍停在 report-only evidence chain。
- selection gate、rating、scorecard、perf threshold 和 training loss 均未接入这些 diagnostics。
- 用户命令块的 exe 搜索名仍是旧 underscore；实际产物为 `surakarta-bitboard-selftest.exe`，按 target 一致的 hyphen 名定位后验证通过。

## 会话：2026-05-24（Phase 3.27 scoped activation no-op fixture）

### 状态
- **Phase 3.27：** completed as scoped activation no-op fixture / explicit enabled skeleton / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_27_scoped_activation_noop_fixture`。
- **生成产物：** `scoped-activation-noop-fixture-report.md`、`scoped-activation-noop-fixture-report.json`、`scoped-activation-noop-fixture-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 Phase 3.27 scoped activation no-op case。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.27 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取 Phase 3.24 plan、Phase 3.25 JSON report/manifest、Phase 3.26 JSON report/manifest。
- 记录保护范围 hash。
- TDD red check：确认 `activation no-op` marker 缺失。
- 在 selftest 中增加显式 enabled skeleton no-op fixture。
- 运行本机等价验证：`VsDevCmd.bat -arch=x64 -host_arch=x64` + `build-vs-release` + target `surakarta-bitboard-selftest`，自测通过。
- 生成 Phase 3.27 markdown report、JSON report 和 manifest。
- 更新四个项目记录文件。
- 执行用户字面验证命令：失败于 `build-local` 缺少 `CMakeCache.txt`，即 `build-local` 不是 CMake build dir。

### 核心结论
- Phase 3.27 只证明显式打开 skeleton 后仍 no-op，不授权 objective、训练 loss、selection、`.bin` 或 gate 改动。
- 用户给定字面验证命令仍不适配当前仓库：`build-local` 非 CMake build dir、target 名 underscore 错、`VsDevCmd.bat -no_logo` 会选 x86 libs。

## 会话：2026-05-24（Phase 3.26 scoped inactive-equivalence verification）

### 状态
- **Phase 3.26：** completed as scoped inactive-equivalence verification / default-off / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_26_scoped_inactive_equivalence_verification`。
- **生成产物：** `scoped-inactive-equivalence-verification-report.md`、`scoped-inactive-equivalence-verification-report.json`、`scoped-inactive-equivalence-verification-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 Phase 3.26 inactive equivalence marker。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.26 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 恢复并读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取 Phase 3.24 dry-run plan。
- 读取 Phase 3.25 JSON report/manifest；确认用户列出的 Phase 3.25 `.md` 报告不存在。
- 记录保护范围 hash。
- 在 selftest 中增加 `Phase 3.26 inactive equivalence default-off disabled path no-op marker`。
- 生成 Phase 3.26 markdown report、JSON report 和 manifest。
- 更新四个项目记录文件。
- 执行用户字面验证命令：失败于缺失 `C:\Program Files\Microsoft Visual Studio\2022\Community\...\cmake.exe`。
- 定位实际 VS CMake 与 `VsDevCmd.bat`，导入 VS dev 环境后复跑等价验证：通过。

### 核心结论
- Phase 3.26 只证明关闭态等价范围，不授权 objective、训练 loss、selection、`.bin` 或 gate 改动。
- 构建和 selftest 使用 VS CMake 绝对路径，避免再次让 `cmake` 在 PATH 里盲猜，土办法能跑但不能当流程。
- 本机硬编码入口必须包含 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat` 初始化和 `D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`，否则裸编译环境会连 `<algorithm>` 都找不到。

## 会话：2026-05-24（Phase 3.25 guarded implementation skeleton）

### 状态
- **Phase 3.25：** completed as guarded implementation skeleton / default-off / no-op。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_25_guarded_implementation_skeleton`。
- **生成产物：** `guarded-implementation-skeleton-report.json`、`guarded-implementation-skeleton-manifest.json`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 仅限允许文件内新增 default-off skeleton 与 inactive metadata。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** Phase 3.25 输出目录未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 Phase 3.24 dry-run plan 三件套。
- 记录禁改范围 before hash。
- 增加 `TrainingOptions` opening-safe 默认关闭字段。
- 增加 inactive report-only summary 字段。
- 增加 `ApplyTrainingStep` default-off 参数骨架，disabled path 不改变 TD target 或权重更新。
- 增加 selftest `TestOpeningSafeObjectiveDefaultOffAndInactiveEquivalence`。
- 更新 `main.cpp` 仅输出 inactive metadata，不新增默认启用路径。
- 生成 Phase 3.25 JSON report/manifest。

### 核心结论
- Phase 3.25 只完成骨架，不构成 objective implementation。
- default-off / inactive equivalence 自测通过。
- Phase 3 仍未通过，Phase 4 继续 blocked。别把骨架当胜利，这玩意儿现在就是个带安全帽的空壳。

## 会话：2026-05-24（Phase 3.24 guarded implementation dry-run plan）

### 状态
- **Phase 3.24：** completed as design-only / dry-run-plan-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_24_guarded_implementation_dry_run_plan`。
- **生成产物：** `guarded-implementation-dry-run-plan.md`、`guarded-implementation-dry-run-plan.json`、`guarded-implementation-dry-run-plan-manifest.json`。
- **结论：** `allow_phase3_25_guarded_implementation_skeleton=true`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.23 三件套。
- 读取 Phase 3.21 readiness review 与 Phase 3.22 minimum prototype design。
- 记录禁改路径 before/after aggregate hash：`src\`、`include\`、`tools\`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 生成 Phase 3.24 guarded implementation dry-run plan 三件套。
- 更新四个文档记录 dry-run task split、allowed/forbidden touch list、hash strategy、verification matrices、hard reject fixtures、report-only companion consumption、rollback triggers 和 risk register。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `guarded-implementation-dry-run-plan.md` | 319 |
| `guarded-implementation-dry-run-plan.json` | 457 |
| `guarded-implementation-dry-run-plan-manifest.json` | 172 |

### 核心结论
- 允许进入 Phase 3.25 guarded implementation skeleton，但仍不允许训练、selection、64 局、`.bin`、gate 修改或 Phase 4。
- Phase 3.25 必须先做 default-off / inactive equivalence skeleton，再谈任何 active fixture。
- hard reject fixture 只能进入 report-only acceptance companion，不得写成 selection gate。
- 禁改路径用 hash 监控；任何新增越界 diff 都触发 rollback。

## 会话：2026-05-24（Phase 3.23 opening-safe objective implementation plan review）

### 状态
- **Phase 3.23：** completed as design-only / implementation-plan-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_23_implementation_plan_review`。
- **生成产物：** `opening-safe-objective-implementation-plan-review.md`、`opening-safe-objective-implementation-plan-review.json`、`opening-safe-objective-implementation-plan-review-manifest.json`。
- **结论：** `allow_phase3_24_guarded_implementation_dry_run_plan=true`。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.18 三件套。
- 读取 Phase 3.21 三件套。
- 读取 Phase 3.22 三件套。
- 记录禁改路径 before hash：`src\`、`include\`、`tools\`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 生成 Phase 3.23 implementation plan review 三件套。
- 更新四个文档记录结论、future 最小改动面、禁止面、default-off / inactive equivalence、hard reject companion、rollback、validation order 和 risk register。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `opening-safe-objective-implementation-plan-review.md` | 150 |
| `opening-safe-objective-implementation-plan-review.json` | 221 |
| `opening-safe-objective-implementation-plan-review-manifest.json` | 142 |

### 核心结论
- 允许进入 Phase 3.24 guarded implementation dry-run plan，但不允许在 Phase 3.23 实现。
- future implementation 最小改动面只限训练 options / scoped soft penalty / CLI default-off / selftest fixture。
- hard reject 条件只能进入 report-only acceptance companion，不得接入 selection gate。
- inactive equivalence 不通过就停止，不能靠“应该没影响”糊弄过去。

## 会话：2026-05-24（Phase 3.22 opening-safe objective minimum prototype design）

### 状态
- **Phase 3.22：** completed as design-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_22_objective_minimum_prototype_design`。
- **生成产物：** `opening-safe-objective-minimum-prototype-design.md`、`opening-safe-objective-minimum-prototype-design.json`、`opening-safe-objective-minimum-prototype-manifest.json`。
- **设计结论：** 允许进入 `Phase 3.23 implementation plan review`；不允许 implementation。
- **实现 objective：** 未实现。
- **修改 training loss：** 未修改。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.18 三件套。
- 读取 Phase 3.20 三件套。
- 读取 Phase 3.21 三件套。
- 生成 Phase 3.22 minimum prototype design 三件套。
- 更新四个文档记录设计结论、hard reject、default-off / inactive equivalence、后续最小改动面与禁止面。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `opening-safe-objective-minimum-prototype-design.md` | 154 |
| `opening-safe-objective-minimum-prototype-design.json` | 179 |
| `opening-safe-objective-minimum-prototype-manifest.json` | 169 |

### 核心设计
- default-off opening-only soft penalty。
- 只针对 unsafe baseline rank degradation / costly PV migration。
- 不硬锁 baseline best move。
- final `20260423` side-effect / root over-neutralization 是 hard reject。
- guard/root-cost companions 继续 report-only，不进 gate。
- distributed tuple shrinkage 仍后置。

## 会话：2026-05-24（Phase 3.21 opening-safe objective readiness review）

### 状态
- **Phase 3.21：** completed as readiness review / design gate only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_21_objective_readiness_review`。
- **修改范围：** 新增 Phase 3.21 输出目录，更新四个文档。
- **readiness 结论：** `conditionally_ready`。
- **允许下一步：** 只允许 `Phase 3.22 opening-safe objective minimum prototype design`，不允许 implementation。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.18 三件套：`opening-safe-objective-preimplementation-plan.md/json`、manifest。
- 读取 Phase 3.20 三件套：`root-cost-companion-report.md/json`、manifest。
- 生成 Phase 3.21 readiness review 三件套。
- 更新四个文档记录结论、边界、blocked 条件和下一步建议。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `objective-readiness-review.md` | 87 |
| `objective-readiness-review.json` | 184 |
| `objective-readiness-review-manifest.json` | 146 |

### 核心结论
- Phase 3.20 root-cost companion 足以支持 readiness review，但不足以支持直接进入 loss 实现。
- cp2 strong explanation 支持设计 soft opening drift penalty；final partial explanation 要求把 `20260423` side-effect 写成硬拒绝条件。
- 推荐顺序仍为 guard companion -> root-cost companion -> opening drift penalty -> distributed tuple shrinkage。
- candidate acceptance guard 必须先接成 report-only wrapper；root-cost companion 继续只是旁路诊断。
- distributed tuple shrinkage 仍排在 opening drift penalty 后面。

### 下一步建议
- Phase 3.22：opening-safe objective minimum prototype design。
- 继续禁止实现、训练、selection、64 局、`.bin`、gate 阈值修改和 Phase 4。

## 2026-05-24 Phase 3.20 root-cost diagnostic companion dry-run

### 状态
- **Phase 3.20：** completed as report-only root-cost diagnostic companion dry-run / zero-mutation verification。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_20_root_cost_companion`。
- **生成产物：** `root-cost-companion-report.md`、`root-cost-companion-report.json`、`root-cost-companion-manifest.json`。
- **root-cost 结论：** cp2 overall improved；final 为 partial / mixed，且带 20260423 regress component。
- **低风险 variant：** `conservative_core` 与 `c2_focused_stable_k10`；`c1_only_k10` 与 `union_k10_riskier` 仍只能作为压力对照。
- **证据延续：** 继续支持 Phase 3.10 / Phase 3.17 的 `cp2 strong explanation` 与 `final partial explanation`。
- **边界：** root-cost companion 是 report-only，不改变 selection result，不是 selection gate。
- **zero-mutation：** changed_artifact_count=0；changed_artifacts=[]。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **生产代码：** 本阶段未修改 `src\` / `include\`。
- **tools：** 本阶段未修改。
- **禁改脚本：** 本阶段未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Phase 3.21 建议
- 若 Phase 3.20 zero-mutation 通过，建议进入 **opening-safe objective implementation readiness review**。
- 训练 loss / opening drift penalty / tuple clipping 继续暂缓，不直接改 training loss。现在只是旁路体检又多了一张单子，别一激动就把训练目标当方向盘猛拧。


## 会话：2026-05-24（Phase 3.19 report-only guard zero-mutation dry-run）

### 状态
- **Phase 3.19：** completed as report-only companion integration dry-run / zero-mutation verification。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation`。
- **修改范围：** 新增 Phase 3.19 输出目录，更新四个文档。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 `tools\candidate-acceptance-guard-runner.ps1` 与 `tools\opening-guard-diagnostics.ps1`。
- 定位并记录 run root 下 selection summary / rating summary / manifest / bootstrap / checkpoint / candidate 等关键 artifact 的 SHA256 before。
- 记录 Phase 3.13 guard 原始产物与 Phase 3.16 runner 原始产物 SHA256 before。
- 执行 Phase 3.16 wrapper dry-run，`RunnerOutputDir` 指向 Phase 3.19 输出目录，`GuardOutputDir` 指向既有 Phase 3.13 guard 目录。
- 重新计算同一批原始 artifact SHA256 after，并生成 zero-mutation report / manifest。

### Wrapper 调用
```powershell
pwsh -NoProfile -ExecutionPolicy Bypass -File "D:\Dev\Code\surakarta_traditional\tools\candidate-acceptance-guard-runner.ps1" -RunRoot "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134" -GuardOutputDir "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard" -RunnerOutputDir "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation" -OpeningGuardScript "D:\Dev\Code\surakarta_traditional\tools\opening-guard-diagnostics.ps1" -Force
```

### 输出产物
| 文件 | 行数 |
|---|---:|
| `candidate-acceptance-guard-summary.md` | 55 |
| `candidate-acceptance-guard-summary.json` | 104 |
| `candidate-acceptance-guard-manifest.json` | 83 |
| `guard-zero-mutation-report.md` | 42 |
| `guard-zero-mutation-report.json` | 1204 |
| `guard-zero-mutation-manifest.json` | 1200 |

### Zero-mutation 结果
| 指标 | 值 |
|---|---:|
| checked_artifact_count | 126 |
| changed_artifact_count | 0 |
| unchanged_artifact_count | 126 |

- `changed_artifacts=[]`。
- `selection_result_unchanged=true`。
- `gate_not_modified=true`。
- wrapper 只写 Phase 3.19 输出目录内产物。
- 原始 selection/training/guard artifacts 未变。

### 下一步建议
- Phase 3.20：root-cost diagnostic companion dry-run，继续 report-only / zero-mutation。
- 仍暂缓训练 loss 改造、opening drift penalty 和 tuple clipping。

## 会话：2026-05-24（Phase 3.18 opening-safe objective pre-implementation plan）

### 状态
- **Phase 3.18：** completed as pre-implementation plan only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_18_objective_preimplementation_plan`。
- **生成产物：** `opening-safe-objective-preimplementation-plan.md`、`opening-safe-objective-preimplementation-plan.json`、`opening-safe-objective-preimplementation-manifest.json`。
- **实现训练 objective：** 未实现。
- **修改训练逻辑：** 未修改。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.17 evidence package：`phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。
- 读取 Phase 3.13 / 3.16 guard artifacts：`opening-guard-summary.json`、`candidate-acceptance-guard-summary.json`、`candidate-acceptance-guard-manifest.json`。
- 只读复核 `scripts\benchmark-train.ps1`、`scripts\benchmark-select.ps1`、`scripts\perf-runner.ps1` 的接口。
- 只读定位未来训练 objective 可能涉及的实现入口：`include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- 生成 Phase 3.18 pre-implementation plan 三件套，并更新四个文档。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `opening-safe-objective-preimplementation-plan.md` | 246 |
| `opening-safe-objective-preimplementation-plan.json` | 460 |
| `opening-safe-objective-preimplementation-manifest.json` | 160 |

### 推荐实施顺序
1. `candidate acceptance guard companion integration`。
2. `root-cost diagnostic acceptance companion`。
3. `opening drift penalty prototype`。
4. `distributed tuple delta clipping / shrinkage prototype`。

### 关键边界
- 前两项优先 report-only companion / dry-run，不接 selection gate。
- 后两项涉及训练 objective / 权重更新，暂缓实际代码改造。
- opening drift penalty 不得硬锁 baseline best move。
- tuple clipping 不得 all_negative neutralization，不得把 diagnostic-neutralized `.bin` 当 candidate。

### Phase 3.19 建议
- 推荐优先做 report-only companion integration 的最小 wrapper 接入计划或 dry-run。
- 暂缓 opening drift penalty / tuple clipping 的实际代码改造。

## 会话：2026-05-24（Phase 3.17 report-only evidence package consolidation）

### 状态
- **Phase 3.17：** completed as report-only evidence package consolidation。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_17_evidence_package`。
- **生成产物：** `phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。
- **修改范围：** 新增 evidence package 输出目录，更新四个文档。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **gate：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.1 `selection-summary.json`。
- 读取 Phase 3.3 `opening-diagnostics-summary.md`。
- 读取 Phase 3.5 attribution manifest / summary JSON / summary CSV。
- 读取 Phase 3.7 static ablation manifest / summary JSON / summary CSV。
- 读取 Phase 3.9 neutralization manifest / summary CSV / root diagnostics CSV。
- 读取 Phase 3.13 opening guard metrics / summary / manifest。
- 读取 Phase 3.16 candidate acceptance guard summary / manifest。
- 计算输入文件 SHA256、当前 git commit 和输出产物 SHA256。
- 生成 Phase 3.17 evidence package，并更新四个文档记录边界和下一步。

### 输出产物
| 文件 | 行数 |
|---|---:|
| `phase3-evidence-package.md` | 45 |
| `phase3-evidence-package.json` | 353 |
| `phase3-evidence-package-manifest.json` | 187 |

### 核心证据链
- Phase 3.1 selection result：`regress`，`overall_rating=strength_rating=perf_rating=regress`，scorecard `0/0/3`。
- Phase 3.2-3.3：opening 是唯一硬 blocker；cp2 first drift 三 seed 同型，且 cp2 无 aspiration retry。
- Phase 3.5：no single tuple dominance，dominance `yes/watch/no=0/0/36`。
- Phase 3.7：distributed tuple deltas 可静态削弱 drift preference；`C1`/`C2` 均 success，K5/K10 跨 seed 成立。
- Phase 3.9：cp2 强解释，final partial explanation；`20260423` final 存在 side-effect risk。
- Phase 3.13：cp2 `explain=6/watch=6/regress=0/unknown=0`，final `explain=0/watch=8/regress=4/unknown=0`。
- Phase 3.16：`selection_result_unchanged=true`，`gate_not_modified=true`。

### 下一步建议
- Phase 3.18：opening-safe objective pre-implementation plan。
- 仍先做计划，不直接改训练 loss；不训练、不 selection、不跑 64、不进 Phase 4。

## 会话：2026-05-24（Phase 3.16 report-only candidate acceptance guard runner/wrapper）

### 状态
- **Phase 3.16：** completed as report-only runner/wrapper implementation。
- **新增脚本：** `D:\Dev\Code\surakarta_traditional\tools\candidate-acceptance-guard-runner.ps1`。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_16_guard_runner`。
- **opening guard：** 复用既有 `phase3_13_opening_guard`，未覆盖原始产物。
- **selection result：** unchanged。
- **gate：** not modified。
- **生产代码：** 未修改 `src\` / `include\`。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取并参考 `tools\opening-guard-diagnostics.ps1`。
- 只读确认 `scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、`scripts\perf-runner.ps1` 不需要修改。
- RED：确认 `tools\candidate-acceptance-guard-runner.ps1` 缺失。
- 新增 `tools\candidate-acceptance-guard-runner.ps1`，实现 selection 后 report-only wrapper。
- 运行 wrapper 生成 Phase 3.16 runner 输出；首次生成后修正 manifest 输入追溯数组，再用 `-Force` 仅覆盖 runner 自己的三份输出。
- 更新四个文档记录 Phase 3.16 结果和边界。

### Wrapper 调用
```powershell
pwsh -NoProfile -ExecutionPolicy Bypass -File .\tools\candidate-acceptance-guard-runner.ps1 -RunRoot 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134' -GuardOutputDir 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard' -RunnerOutputDir 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_16_guard_runner' -OpeningGuardScript '.\tools\opening-guard-diagnostics.ps1' -Force
```

### 输出产物
| 文件 | 行数 |
|---|---:|
| `candidate-acceptance-guard-summary.md` | 41 |
| `candidate-acceptance-guard-summary.json` | 104 |
| `candidate-acceptance-guard-manifest.json` | 83 |

### Guard 计数
| stage | explain | watch | regress | unknown |
|---|---:|---:|---:|---:|
| cp2 | 6 | 6 | 0 | 0 |
| final | 0 | 8 | 4 | 0 |

### Variant 聚合
| variant | guard_status | explain | watch | regress | unknown |
|---|---|---:|---:|---:|---:|
| c1_only_k10 | regress | 0 | 5 | 1 | 0 |
| c2_focused_stable_k10 | regress | 3 | 2 | 1 | 0 |
| conservative_core | regress | 3 | 2 | 1 | 0 |
| union_k10_riskier | regress | 0 | 5 | 1 | 0 |

### 结论
- `selection_result_unchanged=true`。
- `gate_not_modified=true`。
- summary/manifest 明确 `report_only=true`、`selection_gate_eligible=false`、`production_candidate=false`。
- cp2 strong explanation 仍成立；final partial explanation 仍带 regress side-effect 风险。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 会话：2026-05-24（Phase 3.15-A candidate acceptance guard integration report-only hook/spec）

### 状态
- **Phase 3.15-A：** completed as design-only hook/spec documentation。
- **修改范围：** 仅四个文档。
- **实现 hook：** 未实现。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取四个文档。
- 读取 `tools\opening-guard-diagnostics.ps1`，确认现有 guard runner 支持 `-RunRoot` 与 `-OutputDir`，输出 metrics/summary/manifest。
- 只读复核 `scripts\benchmark-select.ps1`，确认主 selection 输出为 `selection-summary.json` / `selection-summary.txt`，其中 `overall_rating`、`strength_rating`、`perf_rating` 与 scorecard `pass/watch/regress` 是 gate 结果。
- 只读复核 `scripts\benchmark-train.ps1` 与 `scripts\perf-runner.ps1` 的 session/output 风格，未修改脚本。
- 更新四个文档，定义 candidate acceptance guard integration 的 report-only hook/spec。

### Hook/spec 摘要
- Hook 位置：selection 写出 `selection-summary.json/txt` 后由外层 wrapper 或 post-selection runner 旁路运行；也可在最终 summary 汇总阶段追加 companion report。
- 明确禁止：不得插入 scorecard/rating 计算路径，不得影响 `overall_rating`、`strength_rating`、`perf_rating` 或 scorecard `pass/watch/regress`。
- 输入 contract：`RunRoot`、`weights-manifest.json`、opening diagnostics root、`tools\opening-guard-diagnostics.ps1`、selection summary / scorecard artifact。
- 输出 contract：`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`，可选 `candidate-acceptance-guard-summary.md`。
- 最小命令示例：`pwsh -NoProfile -ExecutionPolicy Bypass -File .\tools\opening-guard-diagnostics.ps1 -RunRoot <run-root> -OutputDir <guard-output-dir>`。

### Failure behavior
- guard artifact 缺失：selection result 不被覆盖，guard 写 `unknown`。
- guard 脚本失败：主 selection 不被 retroactively 改写，只记录失败状态。
- manifest 不合规：guard report invalid，不能作为证据。
- final `regress`：只能写 risk note，不得自动 reject candidate。

### 下一步建议
- Phase 3.16 推荐实现 report-only hook runner/wrapper，只在 selection 后生成附加报告。
- 暂缓训练 loss/objective 改造。

## 会话：2026-05-24（Phase 3.14 design-only opening-safe training objective minimal design）

### 状态
- **Phase 3.14：** completed as design-only documentation update。
- **修改范围：** 仅四个文档。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取四个文档和指定 Phase 3.13-B / 3.9 / 3.7 artifact。
- 复核 Phase 3.13-B guard 输出：metrics `24` 行；cp2 `explain=6/watch=6/regress=0/unknown=0`；final `explain=0/watch=8/regress=4/unknown=0`。
- 复核 Phase 3.9/3.7 证据：cp2 first drift/root-order inflation 可强解释，final 只能 partial explanation；opening drift 是 distributed tuple delta，不是单 tuple 问题。
- 更新四个文档，写入 opening-safe training objective 最小改造候选方案、推荐排序、不推荐路线和 Phase 3.15 建议。

### Phase 3.14 候选方案推荐排序
| 排序 | 方案 | 生产代码变更 |
|---:|---|---|
| 1 | `candidate acceptance guard integration` | 短期不需要；优先 report-only hook/spec |
| 2 | `root-cost regularization as diagnostic` | 短期不需要；优先训练后验收诊断 |
| 3 | `opening drift penalty` | 未来若进 objective 需要训练代码变更 |
| 4 | `distributed tuple delta clipping / shrinkage` | 未来需要训练/权重更新代码变更，风险最高 |

### 结论
- opening guard 必须继续是 report-only companion，不是 selection gate，不替代现有 pass/watch/regress gate。
- 不推荐直接 all_negative neutralization、直接扩 N-Tuple / threat / mobility / stage、直接调 gate 阈值、直接跑 `GamesPerSeed=64`、直接把 neutralized 权重当 candidate。
- Phase 3.15 推荐 A：先实现 candidate acceptance guard integration 的 report-only hook/spec；暂缓 B：直接改训练 loss。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 会话：2026-05-24（Phase 3.13-B report-only opening guard diagnostics）

### 状态
- **Phase 3.13-B：** completed as report-only diagnostics implementation。
- **新增脚本：** `D:\Dev\Code\surakarta_traditional\tools\opening-guard-diagnostics.ps1`。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **生产代码：** 未修改 `src\` / `include\`。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取四个文档和指定 Phase 3.3 / 3.7 / 3.9 输入产物。
- RED：执行 `pwsh -NoProfile -File .\tools\opening-guard-diagnostics.ps1`，确认目标脚本不存在并失败。
- 新增 `tools\opening-guard-diagnostics.ps1`，只做 table/json 汇总，不调用 benchmark/training/selection，不写权重。
- 运行脚本生成 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。
- 更新四个文档记录 Phase 3.13-B 结果和边界。

### 输出产物
| 文件 | 行数/计数 |
|---|---:|
| `opening-guard-metrics.csv` | 24 data rows |
| `opening-guard-summary.json` | 1 summary object |
| `opening-guard-manifest.json` | 1 manifest object |

### metrics 状态计数
| stage | explain | watch | regress | unknown |
|---|---:|---:|---:|---:|
| cp2 | 6 | 6 | 0 | 0 |
| final | 0 | 8 | 4 | 0 |

### variant 聚合
| variant | guard_status | explain | watch | regress | unknown |
|---|---|---:|---:|---:|---:|
| conservative_core | regress | 3 | 2 | 1 | 0 |
| c2_focused_stable_k10 | regress | 3 | 2 | 1 | 0 |
| c1_only_k10 | regress | 0 | 5 | 1 | 0 |
| union_k10_riskier | regress | 0 | 5 | 1 | 0 |

### 结论
- cp2 strong explanation verified：`true`。
- final partial explanation verified：`true`。
- guard 不是 selection gate，不修改 `overall_rating` / `strength_rating` / `perf_rating`。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 会话：2026-05-23（Phase 3.12 design-to-diagnostic bridge）

### 状态
- **Phase 3.12：** completed as report-only diagnostic spec documentation。
- **修改范围：** 仅四个文档。
- **代码/脚本：** 未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.3 `opening-diagnostics-summary.md`，确认 opening best move drift、baseline rank 降级、qnodes 集中和 aspiration retry 边界。
- 读取 Phase 3.7 `opening-child-static-ablation-summary.csv` / `opening-child-static-ablation-sweep.csv`，确认 C1/C2 distributed tuple support、K5/K10 weakening、controls 与 over-neutralization 风险。
- 读取 Phase 3.9 `neutralization-manifest.json`、`neutralization-summary.csv`、`neutralization-root-diagnostics.csv`，确认 cp2 强解释和 final side-effect 边界。
- 更新四个文档，定义 Phase 3.12 opening guard report-only diagnostic spec。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`。
- 未新增诊断脚本。
- 未生成新的 `.bin`。
- 未修改 selection gate 阈值。
- 未进入 Phase 4。

### Phase 3.12 guard 指标
- `opening best-move guard`：baseline move 是否保持、candidate move 是否 drift、new best move 是否可解释。
- `root-order guard`：baseline move rank 是否降级、drift move rank 是否升到 rank 1、root order 是否 baseline-like。
- `root-cost guard`：`nodes_ratio`、`qnodes_ratio`、`qnodes/nodes ratio`、`aspiration_retry_delta`、search-tree inflation watch/regress 条件。
- `static-drift guard`：Phase 3.7 distributed tuple support、K5/K10 weakening ratio、over-neutralization signature。
- `final-candidate side-effect guard`：non-baseline new best move、root over-neutralization、cp2 可解释但 final 不可解释。

### 输出 spec
- 建议 spec 产物：`opening-guard-spec.md` 或路线图章节。
- 建议 machine-readable 产物：`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。
- 必需字段草案：`seed`、`stage`、`artifact_role`、`baseline_best_move`、`candidate_best_move`、`best_move_status`、`baseline_rank`、`candidate_rank`、`root_order_status`、`nodes_ratio`、`qnodes_ratio`、`aspiration_retry_delta`、`static_support_status`、`over_neutralization_status`、`guard_status`、`explanation_scope`。
- 状态语义：`explain`、`watch`、`regress`、`unknown`；全部 report-only，不参与现有 selection pass/watch/regress gate。

### 下一步建议
- 推荐 Phase 3.13 选择 B：实现 report-only opening guard diagnostics 脚本。
- 理由：字段、输入和状态语义已经定义，先生成可复跑报告能尽快验证 schema 缺口；但仍不进入 Phase 4，不训练，不跑 selection，不生成 `.bin`，不改 gate。

## 会话：2026-05-23（Phase 3.11 design-only opening-safe objective / root-order guard）

### 状态
- **Phase 3.11：** completed as design-only documentation update。
- **修改范围：** 仅四个文档。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **新 `.bin`：** 未生成。
- **生产代码：** 未修改。
- **tools：** 未修改。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 复核 Phase 3.3/3.5/3.7/3.9/3.10 证据链结论，保持 cp2 强解释、final partial explanation 的边界。
- 设计三类后续方向：`candidate acceptance guard`、`opening-safe training objective`、`root-order regularization`。
- 更新四个文档，明确 Phase 3.11 只是 design-only，不进入 Phase 4，不实现训练改造。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`。
- 未生成新的 `.bin`。
- 未修改 selection gate 阈值。
- 未把 Phase 3.9 neutralized 权重写成 candidate 或 production 方案。

### 推荐方案排序
1. 优先 `design/report-only guard`：把 cp2 first drift、baseline root order 降级、root cost inflation、final new best move side effect 写成候选接受前的诊断桥。
2. 再考虑最小 `opening-safe training objective`：只约束 Phase 3 已证明的 opening distributed tuple drift 机制。
3. 最后才考虑生产搜索或特征扩展：Phase 4 仍 blocked，不能直接扩 N-Tuple / threat / mobility / stage。

### 候选方向风险
- `candidate acceptance guard`：过窄会漏问题，过宽会变成偷改 gate；短期必须 report-only。
- `opening-safe training objective`：容易过拟合 opening fixed case，压制真实学习信号；未来实现后必须重新走 32/64 selection。
- `root-order regularization`：可能把 baseline move 固化成教条，阻止真实强着替换；final `20260423` 的新 best move 副作用必须单独分类。

### 下一步建议
- 建议进入 Phase 3.12 design-to-diagnostic bridge。
- Phase 3.12 只能把设计转成 report-only diagnostic spec / artifact schema / minimal fixed-case checks，不允许直接实现训练目标、不允许 selection、不允许 64、不允许 Phase 4。

## 会话：2026-05-23（Phase 3.10 report-only root-order/root-cost explanation summary）

### 状态
- **Phase 3.10：** completed as report-only documentation summary。
- **修改范围：** 仅四个文档。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **neutralized `.bin`：** 未新增。
- **Phase 3：** 仍未通过。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.3 `opening-diagnostics-summary.md`，确认 cp2 first drift/root order inflation、baseline rank 退位、qnodes 集中和 aspiration retry 边界。
- 读取 Phase 3.5 attribution summary/top-deltas/manifest，确认 static eval delta 方向、active tuple count、hash/header 和 single tuple dominance 被排除。
- 读取 Phase 3.7 static ablation summary/sweep/manifest，确认 `C1_cp2_onset` / `C2_final_gate_blocker` success、K1/K3 不足、K5/K10 跨 seed 成立、controls pass。
- 读取 Phase 3.9 neutralization manifest/summary/root diagnostics/weight deltas，确认 cp2 强解释力、final partial explanation、`20260423` root over-neutralization 和 variant 风险差异。
- 更新四个文档，形成 Phase 3.10 证据链总结和下一步 design-only 建议。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未修改 `src\` / `include\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`。
- 未新增 neutralized `.bin`。
- 未把 Phase 3.9 写成 production candidate。
- 未写 Phase 3 通过结论，未写 Phase 4 解除阻塞结论。

### 核心结论
- Phase 3.1 selection gate 仍为 `regress`，不能跑 64。
- Phase 3.2-3.3 已定位 opening 是唯一硬 blocker，cp2 first drift/root order inflation 不是 aspiration retry 导致。
- Phase 3.5 排除 single tuple dominance，drift 来源是分布式 tuple delta，不是单点玄学锅。
- Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference，K5/K10 才跨 seed 稳定，K1/K3 不够。
- Phase 3.9 对 cp2 first drift/root order inflation 有强解释力：`3/3` seed 回到 baseline best move，root order `2->1`，nodes/qnodes 下降。
- final candidate 只有 partial explanation：`20260425` root cost 下降，但 `20260423` 出现 1/3 root over-neutralization、新 best move `(4, 1) -> (3, 2) (B)` 和 qnodes 上升。
- conservative_core / c2_focused_stable_k10 比 c1_only_k10 / union_k10_riskier 风险更低；后两者只能做压力对照。
- Phase 3 仍未通过；Phase 4 仍 blocked；下一步建议 Phase 3.11 design-only。

### 下一步建议
- Phase 3.11 只做 design-only：`opening-safe training objective`、`root-order regularization`、`candidate acceptance guard`。
- 不建议直接扩 N-Tuple、直接 all_negative neutralization、直接 rerun 64 games、调 gate 阈值、或把 neutralized 权重当 candidate。真这么干就是拿诊断报告当锤子，砸得响，不代表修得好。

## 会话：2026-05-23（Phase 3.9 offline neutralization diagnostics execution）

### 状态
- **Phase 3.9：** completed as report-only offline diagnostics。
- **权重写入：** 仅写入 diagnostics 目录下临时 `diagnostic-neutralized.bin`，共 24 个。
- **diagnostics：** opening root diagnostics 共 30 次，全部 depth `4`、threads `1`、format `json`、启用 `--search-diagnostics`。
- **训练/selection：** 未运行。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取并核对 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取并核对 `build-local\training\20260522-172134\weights-manifest.json`、`selection-summary.json`、`opening-diagnostics\opening-diagnostics-summary.md`。
- 读取 Phase 3.5 attribution 产物：summary / tuples / top-deltas / manifest。
- 读取 Phase 3.7 static ablation 产物：summary csv / tuples / sweep / summary json / manifest。
- 新增非生产脚本 `D:\Dev\Code\surakarta_traditional\tools\offline-neutralization-diagnostics.ps1`。
- 脚本从 manifest 解析 cp2 checkpoint-2 和 final candidate artifact，校验 SHA256、seed、Phase 3.1 参数、header、tuple hash、weight count、文件长度和 payload int32 数量。
- 生成四个 variant × 三 seed × cp2/final 的临时 neutralized `.bin`，并与 original 权重逐项执行 opening root diagnostics。

### 命令
```powershell
pwsh -NoProfile -File .\tools\offline-neutralization-diagnostics.ps1
```

diagnostics 内部命令形式：
```powershell
build-vs-train-release\bin\surakarta-benchmark.exe bitboard-search --depth 4 --threads 1 --format json --case-id opening --weights <path> --search-diagnostics
```

### 输出目录
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization`

### 输出文件
- `neutralization-manifest.json`
- `neutralization-summary.csv`
- `neutralization-root-diagnostics.csv`
- `neutralization-weight-deltas.csv`
- `diagnostics-json\*.json`
- `weights-temp\**\*.diagnostic-neutralized.bin`

### 验收计数
| 检查 | 结果 |
|---|---:|
| 临时 `.bin` 数量 | `24` |
| diagnostics JSON 数量 | `30` |
| summary 行数 | `24` |
| root diagnostics 行数 | `24` |
| weight deltas 行数 | `348` |
| neutralized artifact 标记 | `production_candidate=false`、`selection_gate_eligible=false` |
| Phase 3 状态 | `failed` |
| Phase 4 状态 | `blocked` |

### 核心结果
- cp2 conservative core：3/3 seed best move `(5,1)->(4,2)` 回到 `(0,1)->(1,2)`；baseline rank `2->1`；nodes/qnodes `1797/1277 -> 1569/1063`；无 root over-neutralization。
- cp2 C1-only K10：3/3 seed 回到 baseline；rank `2->1`；nodes/qnodes `1797/1277 -> 1722/1219`；static over-neutralization 为 3/3，但 root 未引入新 best move。
- cp2 C2-focused stable K10：结果与 conservative core 相同，说明 shared core 已足够解释 cp2 onset，43 在 cp2 不是必要条件。
- cp2 union K10 riskier：3/3 seed 回到 baseline；nodes/qnodes `1797/1277 -> 1722/1219`；static over-neutralization 为 3/3。
- final conservative core：`20260425` nodes/qnodes `2495/1485 -> 1619/1080`，`20260424` 基本不变，`20260423` best move 被推到 `(4,1)->(3,2)` 且 nodes/qnodes `1978/1123 -> 2456/1461`。
- final C2-focused stable K10：`20260425` nodes/qnodes `2495/1485 -> 1654/1135`，`20260424` 基本不变，`20260423` 同样推到 `(4,1)->(3,2)` 并涨到 `2456/1461`。
- final C1-only K10 / union K10 riskier：`20260423` 也出现新 best move `(4,1)->(3,2)`，并涨到 `2533/1532`；`20260425` 有成本下降，`20260424` 小幅变化。

### 结论
- Phase 3.9 支持 report-only root-order/root-cost explanation：cp2 first drift/root order inflation 能被小 tuple set offline neutralization 解释并缓解。
- final gate blocker 只得到部分解释；`20260423` 的新 root drift 是副作用，不能当 production candidate 或 selection gate 通过依据。
- Phase 3 仍未通过；Phase 4 仍 blocked。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| RED：首次执行目标脚本时 `tools\offline-neutralization-diagnostics.ps1` 不存在 | 新增非生产诊断脚本 |
| `HashSet[int]` 上错误调用 `.ToArray()` | 改为显式枚举后排序 |
| PowerShell 字符串中 `$variant.id` 被当作变量名解析 | 改为 `$($variant.id)` |
| 初版只应用 Phase 3.7 negative rows，部分 requested tuple 被标 missing | 改为按 Phase 3.5 active tuple row 对所有 requested tuple id 写入诊断权重，缺少 static negative row 只影响解释字段，不跳过 neutralization |

## 会话：2026-05-23（Phase 3.8 offline neutralization design review）

### 状态
- **Phase 3.8：** completed as design review only；未执行 neutralization。
- **权重写入：** `weights_written=false`。
- **neutralization：** `neutralization_executed=false`。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.5 attribution 产物：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`
- 读取 Phase 3.7 static ablation 产物：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-sweep.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-summary.json`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-manifest.json`
- 复核 Phase 3.7 的 C1/C2 success、K1/K3 不足、K5/K10 `3/3` seed success、controls pass、K10/all_negative over-neutralization 风险。
- 设计 Phase 3.9 仅限 offline diagnostics 的 neutralization variant、临时权重目录、manifest 字段、最小 diagnostics、解释力口径和停止条件。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未修改 `src\` / `include\` 生产代码。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`。
- 未运行 `bitboard-search`、training、selection。
- 未修改 selection gate 阈值。
- 未写任何 `.bin` 权重文件。
- 未创建 `phase3_8` neutralized 权重目录。
- 未进入 Phase 4。

### 关键设计结论
- 满足进入 offline neutralization 设计评审条件：`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`，K5/K10 均 `3/3` seed success，controls pass。
- 不满足直接执行条件：当前仍只是设计评审；neutralization 结果未来也只能作为 offline diagnostics，不能替代 selection gate。
- 推荐 tuple variants：
  - conservative core：`5,10,32,41`
  - C1-only K10：`4,5,6,10,11,27,32,36,38,41`
  - C2-focused stable K10：`5,10,32,41,43`
  - optional union K10：`4,5,6,10,11,27,32,36,38,41,43`，标记为 riskier
- C2 2/3 support set `0,5,6,10,11,32,33,37,41,43,44` 只作为解释/审计候选，不作为首轮默认写权重集合。
- 推荐 K10 设计，不推荐 K1/K3，也不推荐 all_negative：K1/K3 在 C1/C2 都不足，all_negative/K10 已出现 over-neutralization 风险，直接全砍就是把显微镜当锤子，挺响但不精确。

### 后续建议
- 建议进入 Phase 3.9 execution，但只能执行 offline neutralization diagnostics。
- 不建议进入 Phase 4。
- Phase 3 仍未通过；Phase 4 仍 blocked。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| 首次抽取 Phase 3.7 sweep/tuple 时用了错误列名导致空输出 | 重新读取列名后按 `topk` / `topk_membership` 精确抽取 |

## 会话：2026-05-22（Phase 3.7 child-position static ablation execution）

### 状态
- **Phase 3.7：** completed as report-only/table-only static ablation。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.5 attribution 产物：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`
- 复核 Phase 3.6 设计结论对应的 comparison 集合、`induced_pair_delta` 公式、TopK 策略和 success/failure/inconclusive 判定。
- 新增非生产只读脚本 `D:\Dev\Code\surakarta_traditional\tools\child-position-static-ablation.ps1`。
- 执行 table-only static ablation，按 `K=1/3/5/10/20/all_negative` 输出 sweep。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。

### 命令
```powershell
pwsh -NoProfile -File .\tools\child-position-static-ablation.ps1
```

### 输出目录
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`

### 输出文件
- `opening-child-static-ablation-summary.csv`
- `opening-child-static-ablation-tuples.csv`
- `opening-child-static-ablation-sweep.csv`
- `opening-child-static-ablation-summary.json`
- `opening-child-static-ablation-manifest.json`

### 关键结果
- summary `7` 行；selected tuple detail `174` 行；TopK sweep `126` 行。
- attribution manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`；tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`；tuple set hash `0x1D4A6EBF2BAF6484`；weight count `16605`；git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 执行的 comparison：`C1_cp2_onset`、`C2_final_gate_blocker`、`C3_final_cp2_residue`、`C4_final_transition`、`N1_bootstrap_cp2_sanity`、`N1_bootstrap_final_sanity`、`N2_cp2_final_negative_control`。
- `C1` 和 `C2` 均为 `success`：K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring。
- `C1` K10 tuple 三 seed 完全一致：`4,5,6,10,11,27,32,36,38,41`。
- `C2` K10 的 `3/3` tuple 为 `5,10,32,41,43`；`2/3` tuple 为 `0,5,6,10,11,32,33,37,41,43,44`。
- controls 全部 pass：bootstrap 不天然支持 drift，cp2 阶段 final drift child 仍不是首漂移原因。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\tools\child-position-static-ablation.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未修改 `src\` / `include\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`。
- 未运行 search diagnostics。
- 未生成或改写权重，未执行 offline neutralization。
- 未修改 selection gate，未进入 Phase 4。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| PowerShell 字符串 `$seed:` 被解析成非法变量边界 | 改为 `${seed}:...` |
| `if` 表达式返回单个对象后 `.Count` 不存在 | 对 selected rows 强制 `@(...)` 数组化 |
| ordered hash 中内联条件/集合表达式触发类型绑定异常 | 将条件结果和数组提前落到变量后再写入 JSON 对象 |

### 当前结论
- TopK 曲线显示分布式效应：top1/top3 不足，K5/K10 才稳定削弱/翻转。
- dominance=no 时 ablation 结果仍有效，但只说明 static child preference 可解释，不说明 PVS/root search 已恢复。
- 结果足够进入 offline neutralization 设计评审；必须停止在执行前，不写权重、不跑 search/training/64、不进 Phase 4。
- Phase 3 仍未通过。
- Phase 4 仍 blocked。

## 会话：2026-05-22（Phase 3.6 child-position static ablation design）

### 状态
- **Phase 3.6：** completed as design only；未执行 ablation。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.5 attribution 产物：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`
- 复核 summary 行数 `27`、tuple detail 行数 `1215`、top-deltas 行数 `1440`、manifest tuple hash `0x1D4A6EBF2BAF6484`、weight count `16605`。
- 基于 tuple detail 推导 child-position static ablation 的 `stage_delta` / `induced_pair_delta` 口径。
- 设计 comparison 集合、TopK 策略、输出 CSV/JSON、成功/失败/无结论判定、offline neutralization 升级条件和停止条件。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 未执行事项
- 未写代码。
- 未新增脚本。
- 未修改 `src\` / `include\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`。
- 未生成权重，未执行 ablation，未执行 offline neutralization。
- 未进入 Phase 4。

### 设计结论摘要
- ablation 应优先比较 `baseline_child -> cp2_drift_child` at `cp2` 和 `baseline_child -> final_drift_child` at `final`；辅以 final residual、cp2-to-final transition 和 bootstrap/cp2 negative controls。
- neutralization 应针对 stage-vs-bootstrap 的 `induced_pair_delta < 0`，不是直接砍 same-stage child 差值，更不是直接抓 top1。
- TopK 应输出 `1/3/5/10/20/all_negative` 累积曲线；dominance 全 `no` 时，top1 只能做观察点。
- dominance=no 时 static ablation 仍有意义，可以验证 distributed tuple deltas 的累计解释力；但不支持直接 offline neutralization。
- 只有小而稳定的 tuple 集合跨 seed 抹平 cp2/final drift 偏好，才允许进入 offline neutralization 设计；否则停止，避免瞎折腾。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| 一次 PowerShell `Sort-Object Count -Descending,Name` 写法触发 parser error | 改用显式排序表达式后继续提取 tuple 频次；未影响输入数据和结论 |

### 当前结论
- Phase 3.6 child-position static ablation design 已完成。
- 本轮没有执行 ablation，没有生成任何权重或 CSV/JSON ablation 产物。
- Phase 3 仍未通过。
- Phase 4 仍 blocked。

## 会话：2026-05-22（Phase 3.5 opening attribution-only execution）

### 状态
- **Phase 3.5：** completed as attribution-only execution；不是修复、不是 ablation、不是训练。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`。
- 读取 Phase 3.1/3.3 输入：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\weights-manifest.json`、`selection-summary.json`、`opening-diagnostics\opening-diagnostics-summary.md`。
- 只读复核 `src\surakarta_search.cpp`、`src\surakarta_bitboard.cpp`、`include\surakarta_search.h` 中 tuple descriptor、offset、EncodeTuple、material 和 side-to-move eval 逻辑。
- 新增非生产离线脚本 `D:\Dev\Code\surakarta_traditional\tools\opening-attribution.ps1`。
- 运行脚本，对 seed `20260423/20260424/20260425` × stage `bootstrap/cp2/final` × root child `baseline/cp2/final` 输出 static attribution。
- 解析输出 CSV/JSON，复核行数、active tuple count、artifact hash/header status、static eval delta 和 tuple delta dominance。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 记录 Phase 3.5 结果。

### 命令
```powershell
pwsh -NoProfile -File .\tools\opening-attribution.ps1 `
  -RunRoot 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134' `
  -OutputDir 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution' `
  -Seeds 20260423,20260424,20260425 `
  -Stages bootstrap,cp2,final `
  -RootMoves 'baseline:(0,1)->(1,2)','cp2:(5,1)->(4,2)','final:(3,1)->(2,2)' `
  -TopK 20
```

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\tools\opening-attribution.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 输出产物
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`

### 验收结果
| 检查 | 结果 |
|---|---|
| summary 行数 | `27` |
| tuple detail 行数 | `1215` |
| top-deltas 行数 | `1440` |
| active tuple count | 每行 `45`；异常数 `0` |
| artifact hash | 9/9 `pass` |
| weight header | 9/9 `pass` |
| tuple hash | `0x1D4A6EBF2BAF6484` |
| weight count | `16605` |
| 禁改脚本 | `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1` 无本轮 diff |
| whitespace check | `git diff --check` exit `0`；仅既有 LF/CRLF warning 可接受 |

### Static Eval 摘要
| seed | stage | baseline child | cp2 drift child | final drift child |
|---|---:|---:|---:|---:|
| `20260423` | bootstrap | `-6` | `-6` | `12` |
| `20260423` | cp2 | `-10` | `-23` | `4` |
| `20260423` | final | `64` | `-10` | `-41` |
| `20260424` | bootstrap | `-6` | `-6` | `12` |
| `20260424` | cp2 | `-10` | `-23` | `4` |
| `20260424` | final | `-359` | `-403` | `-497` |
| `20260425` | bootstrap | `-6` | `-6` | `12` |
| `20260425` | cp2 | `-10` | `-23` | `4` |
| `20260425` | final | `-214` | `-312` | `-364` |

结论：cp2 阶段 cp2 drift child 相对 baseline child delta 为 `-13`，三个 seed 一致；final 阶段 final drift child 相对 baseline child delta 为 `-105/-138/-150`。White to move 下 total static score 更低意味着 black-perspective 更好，方向上解释 root drift 倾向。

### Tuple Delta Dominance
- comparison 数量：`36`。
- dominance `yes/watch/no = 0/0/36`。
- max top1 abs share：`0.165094`。
- max top3 abs share：`0.376866`。
- 未发现少数 tuple 主导 opening child static eval 变化。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| RED：首次运行目标命令时 `tools\opening-attribution.ps1` 不存在 | 新增非生产离线脚本 |
| `-Seeds 20260423,20260424,20260425` 在 `pwsh -File` 下可能被绑定为单字符串 | 参数改为 `string[]` 并在脚本内部 split |
| PowerShell 返回 `List[object]` 时出现 `Argument types do not match` | 使用 `.ToArray()` 返回稳定数组 |
| `RootMoves` 参数包含坐标逗号和标签引号，直接拆分易错 | 使用 trim/regex 解析 label 与 `(x,y)->(x,y)` |

### 当前结论
- Phase 3.5 opening attribution-only execution 已完成。
- static eval delta 能解释 cp2/final drift child 相对 baseline child 的偏好方向，但它不是 PVS 全宽 score 证明。
- 证据足够进入 child-position static ablation 设计。
- 不支持 offline neutralization，不生成改写权重，不进入 Phase 4。
- Phase 3 仍未通过。
- Phase 4 仍 blocked。

## 会话：2026-05-22（Phase 3.4 opening active-tuple attribution and minimal ablation design）

### 状态
- **Phase 3.4：** completed as design / feasibility analysis；不是修复阶段。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.3 诊断产物：`opening-diagnostics-summary.md`、`opening-diagnostics-summary.csv`、`opening-root-moves-depth4-final-attempt.csv`、`opening-aspiration-attempts.csv`。
- 只读审计 `include\surakarta_search.h`、`include\surakarta_bitboard.h`、`src\surakarta_search.cpp`、`src\surakarta_bitboard.cpp`、`src\main.cpp`。
- 复核权重 manifest 和 run layout，确认 3 seed 的 `bootstrap.bin`、`checkpoints\checkpoint-2.bin`、`candidate.bin` 路径均由 `weights-manifest.json` 追溯。
- 实际读取 `seed-20260423\checkpoints\checkpoint-2.bin` header，确认 `magic=0x5455504C`、`version=1`、`tuple_set_hash=0x1D4A6EBF2BAF6484`、`weight_count=16605`、文件长度 `66444`。
- 设计 opening root child attribution 数据集、输出字段、离线脚本方案、report-only CLI 备选方案和最小 ablation 顺序。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 记录 Phase 3.4 结论。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 关键审计结果
| 检查 | 结果 |
|---|---|
| 现有 CLI | `bitboard-search --search-diagnostics` 无 material / active tuple / tuple contribution 输出 |
| 权重格式 | `24` byte header + `16605` 个 little-endian `int32` |
| header | `magic=0x5455504C`，`version=1`，`tuple_set_hash=0x1D4A6EBF2BAF6484`，`weight_count=16605` |
| active tuple | `45` 个 tuple；active index = `TupleOffsets()[tuple_id] + EncodeTuple(position, tuple)` |
| tuple 编码 | empty `0`，black `1`，white `2`，低位优先 |
| material | `(black_count - white_count) * 100` |
| side-to-move | black perspective 的 `material + tuple`；White to move 时整体取负 |
| root child dataset | 9 个权重 × 3 个 child position = `27` 行 summary，`1215` 行 tuple detail |

### 设计结论
- 现有 CLI 单独不能完成 active tuple attribution。
- 可以不改 `src` / `include`，用离线小脚本解析权重并复刻 tuple descriptor 完成 report-only attribution；脚本必须校验 tuple hash 和 weight_count。
- 若要求完全复用引擎内部 tuple logic，下一阶段可做默认关闭的 report-only `bitboard-attrib` / `bitboard-eval --explain-tuples` 扩展。
- 推荐最小 ablation 顺序：`attribution-only` -> `child-position static ablation` -> 条件满足时才做复制权重的 `offline neutralization`。
- root-order controlled experiment 现有 CLI 不支持，本阶段只记录为后续设计项。

### 未执行事项
- 未创建 attribution 脚本。
- 未生成 attribution CSV/JSON。
- 未复制或修改任何权重。
- 未运行 ablation。
- 未运行 search 参数对照、selection 或训练。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| 第一次手写 cp2 权重路径时漏掉 `checkpoints` 目录 | 改用 manifest 中的 `weights_path` 复核，确认 9 个目标权重均存在；下一阶段脚本必须从 manifest 取路径 |
| 一次 `Select-String -LiteralPath` 搭配通配符读取失败 | 该查询非关键路径；后续改为直接读取明确文件范围 |

### 当前结论
- Phase 3.4 opening active-tuple attribution and minimal ablation design 已完成。
- Phase 3 仍未通过。
- Phase 4 仍 blocked。

## 会话：2026-05-22（Phase 3.3 opening root diagnostics and minimal attribution）

### 状态
- **Phase 3.3：** completed as read-only diagnostics；未修复、未调参、未重新训练。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍未通过，继续 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.1/3.2 产物：`selection-summary.json`、`selection-summary.txt`、`weights-manifest.json`。
- 确认 `build-vs-train-release\bin\surakarta-benchmark.exe` usage：`bitboard-search` 支持 `--weights`、`--depth`、`--threads`、`--format json`、`--case-id`、`--search-diagnostics`；未见 `--debug-search-tree`。
- 使用 depth `4`、threads `1`、format `json`、case-id `opening`、`--search-diagnostics`，对每个 seed 的 `bootstrap`、`cp2`、`cp4`、`cp6`、首次最终 move checkpoint、`final` 跑 opening diagnostics。
- 解析 `diagnostics.root_moves`、`diagnostics.aspiration_attempts`、`diagnostics.qsearch_sources`，生成 root ranking、nodes/qnodes、aspiration retry 归因表。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 记录 Phase 3.3 结论。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-bootstrap.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-cp2.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-cp4.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-cp6.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-cp32.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260423-final.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-bootstrap.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-cp2.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-cp4.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-cp6.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-cp22.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260424-final.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-bootstrap.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-cp2.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-cp4.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-cp6.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-cp14.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\seed-20260425-final.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\opening-diagnostics-summary.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\opening-root-moves-depth4-final-attempt.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\opening-aspiration-attempts.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\opening-diagnostics-summary.md`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 关键结果
| 检查 | 结果 |
|---|---|
| diagnostics JSON | `18` 个 |
| summary CSV/MD | `4` 个汇总文件 |
| cp2 first drift | 三 seed 均为 `(5, 1) -> (4, 2) (B)` |
| cp2 root rank | `(5,1)->(4,2)` order rank `1`；baseline order rank `2`；二者 observed score-rank 都为 `1` |
| cp2 qnodes | `(5,1)->(4,2)` root `355`；baseline root `38` |
| cp2 aspiration | `aspiration_retries=0`，不能解释首次 drift |
| final convergence | 三 seed final candidate 均为 `(3, 1) -> (2, 2) (B)` |
| final root rank | final candidate 中 `(3,1)->(2,2)` order rank `1`；baseline order rank `2/2/3` |
| final qnodes | `(3,1)->(2,2)` root qnodes `358/356/440`；baseline root qnodes `16/16/18` |
| active tuple attribution | 现有 CLI 无字段，证据不足 |

### 根因结论
- cp2 首次 drift 不是 aspiration retry；三 seed retry 均为 `0`。
- cp2 也不是明确的全宽 root score 排序压低 baseline；`(5,1)->(4,2)` 与 baseline observed score  tied / bound，真正先变的是 root order / PV。
- qnodes 膨胀主要跟随当前 PV root move：cp2 是 `(5,1)->(4,2)`，final candidate 是 `(3,1)->(2,2)`。
- 后期 aspiration retry 会显著放大成本，但它是后期放大器，不是首次 root instability 的触发点。
- 三 seed 呈现“cp2 同源初漂移，中期分叉，后期同源收敛到 final move”的模式。final common move 更像共同 opening root ranking 收敛，不像随机漂。
- 没有 active tuple / weight attribution，不能把锅精确甩到某个 tuple 或权重局部拉偏；下一步只能做 report-only diagnostic extension / 最小 ablation 设计。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| `bitboard-search --help` 返回 `Unknown option: --help` | 改用顶层 usage 和实际 `bitboard-search` 探针确认 CLI 能力 |
| 初次解析顶层 `$json.root_moves` 得到空/数量误判 | 直接查看 JSON schema 后确认 root list 位于 `diagnostics.root_moves` |
| PowerShell 一行 `foreach` 直接接管道触发 parser error | 改为先赋值到 `$rows` 再管道输出 |
| 一次读取 root move CSV 时漏掉 `opening-diagnostics` 路径段 | 改用正确路径重跑成功 |

### 当前结论
- Phase 3.3 opening diagnostics 已完成。
- Phase 3 仍未通过。
- Phase 4 仍 blocked。
- 不支持直接调参、放宽 gate 或进入 Phase 4；只支持先做 report-only diagnostic extension / 最小 ablation 设计。

## 会话：2026-05-22（Phase 3.2 opening blocker root-cause attribution）

### 状态
- **Phase 3.2：** completed as root-cause attribution；未修复、未调参、未重新训练。
- **GamesPerSeed=64：** 未运行。
- **Phase 3：** 仍是 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 读取 Phase 3.1 产物：`selection-summary.json`、`selection-summary.txt`、`weights-manifest.json`。
- 解析 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\selection\seed-*\checkpoint-evals\*.eval-summary.json`，共 `48` 个 eval summary。
- 生成 checkpoint 时间线，字段覆盖 seed、checkpoint games、rating、strength/perf rating、C/B/D、score regressions、best move changes、opening best moves、score_delta、search/node/performance ratio。
- 复核 `scripts\benchmark-select.ps1`、`src\main.cpp`、`src\surakarta_search.cpp` 中 best_move、score_delta、node_ratio、performance_ratio、match summary 的字段来源；只读，不改代码。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 记录 Phase 3.2 归因结论。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 关键验证
| 检查 | 结果 |
|---|---|
| eval summary 数量 | `48` |
| checkpoint scorecard vs eval files | `mismatch_count=0` |
| manifest | `manifest_version=1`，`purpose=selection`，artifact count `54` |
| manifest artifact hash | `artifact_missing=0`，`hash_mismatch=0` |
| artifact kinds | `bootstrap=3`、`candidate=3`、`checkpoint=48` |
| 参数追溯 | `games=32`、`depth=4`、`checkpoint_every=2`、`alpha=0.005`、`lambda=0.5`、`epsilon=0.02`、`epsilon_plies=6`、`terminal_reward=1200`、`td_error_clip=0`、`terminal_only_warmup=0`、`near_terminal_curriculum=0` |

### 首次事件摘要
| seed | first opening drift | first final move | first score_delta < 0 | first node > 1.25 | first search > 1.25 | first perf regress | watch/pass -> regress |
|---|---|---|---|---|---|---:|---:|
| `20260423` | cp2 `(5, 1) -> (4, 2) (B)` | cp32 | cp30 `-70` | cp6 `1.903533` | cp2 `1.442543` | 2 | 6 |
| `20260424` | cp2 `(5, 1) -> (4, 2) (B)` | cp22 | none | cp4 `1.334239` | cp2 `1.313239` | 2 | 10 |
| `20260425` | cp2 `(5, 1) -> (4, 2) (B)` | cp14 | cp6 `-8` | cp4 `1.334239` | cp2 `1.579812` | 2 | 16 |

### 根因结论
- opening 是唯一硬 fixed-position blocker：三 seed 最终均从 baseline `(0, 1) -> (1, 2) (B)` 漂到 `(3, 1) -> (2, 2) (B)`。
- search_seconds 超阈与 best move drift 在 cp2 即同步出现，node inflation 稍后出现；最终 move 出现更晚，说明问题是 opening root move choice 被训练早期扰动，而不是单个最终 move 独立造成所有膨胀。
- `20260423` 的 score regression 是晚期才出现；`20260425` 的早期负分后来恢复；`20260424` 无负向 score_delta。
- 非 opening 局面 48 个 checkpoint 全部无 best move drift、无 score regression，node ratio 基本稳定，因此不是当前主 blocker。
- strength/perf blocker 耦合但不是同一个指标：存在 `strength=watch, perf=regress` 和 `strength=regress, perf=watch` 的 checkpoint，最终三 seed 又同时 C/B/D 落后和 opening performance 超阈。
- 当前没有证据支持直接调 `alpha/lambda/epsilon` 再跑；下一步应继续 opening-specific 诊断和最小 ablation。

### 遇到的错误
| 错误 | 处理 |
|---|---|
| manifest 校验脚本第一次误取 `artifacts[].path`，把 54 个 artifact 误报 missing | 读取 manifest schema 后改用 `artifacts[].weights_path` / `binary_sha256` 重验，结果 `artifact_missing=0`、`hash_mismatch=0` |
| 事件摘要脚本一次 PowerShell 格式化函数写法触发 parser error | 未产生文件改动；改为简单字符串格式化后重跑成功 |

### 当前结论
- Phase 3.2 根因归因已完成。
- 32 局结果不允许进入 64 局评审。
- Phase 3 仍未通过，Phase 4 仍 blocked。

## 会话：2026-05-22（Phase 3.1 fresh post-B1 GamesPerSeed=32 selection validation）

### 状态
- **Phase 3.1 GamesPerSeed=32：** completed, validation failed。
- **32 局是否允许进入 64 局评审：** 不允许。`overall_rating=regress`，按停止条件不跑 64。
- **Phase 3：** 仍是 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 运行 `git status --short`，记录既有 B1/B2.2 modified/untracked 状态；本轮未提交、未回滚、未清理。
- 校验 official perf baseline：`test\test_data\perf-baseline.json` SHA256 为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- 确认 `build-vs-train-release\bin\surakarta-benchmark.exe` 存在。
- 初次按外层 `pwsh` 逗号数组传参执行时，`Seeds` 被当成单个字符串，参数绑定失败，未进入正式 run。
- 使用同一参数值显式传 PowerShell 数组后正式运行 `GamesPerSeed=32` selection，输出 session `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- 读取 `selection-summary.json`、`selection-summary.txt`、`weights-manifest.json`，并校验 artifact 文件存在性与 SHA256。
- 复核本轮未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 关键结果
| 检查 | 结果 |
|---|---|
| session root | `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134` |
| summary JSON | `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\selection-summary.json` |
| summary TXT | `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\selection-summary.txt` |
| overall / strength / perf | `regress / regress / regress` |
| scorecard pass/watch/regress | `0 / 0 / 3` |
| candidate/baseline/draws | `12 / 16 / 2` |
| score_regressions | `1` |
| best_move_changes | `3` |
| average/worst performance ratio | `1.622708 / 1.864728` |
| manifest | 54 artifacts；bootstrap `3`、candidate `3`、checkpoint `48`；无缺失 artifact；SHA256 全匹配 |

### Per-seed 结果
| seed | rating | strength_rating | perf_rating | C/B/D | blocker |
|---|---|---|---|---|---|
| `20260423` | `regress` | `regress` | `regress` | `4/5/1` | 1 个 fixed score regression；opening performance ratio `1.343750` |
| `20260424` | `regress` | `regress` | `regress` | `4/6/0` | opening node ratio `1.659647` |
| `20260425` | `regress` | `regress` | `regress` | `4/5/1` | opening performance ratio `1.864728`，node ratio `1.694973` |

### 固定局面归因
- `opening`：三个 seed 全部 best move drift，baseline `(0, 1) -> (1, 2) (B)`，candidate `(3, 1) -> (2, 2) (B)`；node ratio `1.343750 / 1.659647 / 1.694973`，其中 `20260423` score delta `-78`。
- `capture-heavy`：无 best move drift、无 score regression；最大 node ratio `1.004457`。
- `threat-heavy`：无 drift、无 score regression、node ratio `1.0`。
- `endgame`：无 drift、无 score regression、node ratio `1.0`。
- `no-capture-critical`：无 drift、无 score regression、node ratio `1.0`。

### 结论
- Phase 3.1 `GamesPerSeed=32` 结果无效通过，属于明确 regress，不允许进入 `GamesPerSeed=64` 评审。
- Phase 3 继续 failed，Phase 4 继续 blocked。
- 这次失败不是 artifact 缺字段或 baseline hash 不匹配造成的；证据链完整，问题集中在 strength regression 与 opening search-tree inflation。想靠 64 局翻盘这事儿本轮不能干，gate 已经把门焊上了。

## 会话：2026-05-22（Phase 3 post-B1 fresh trend validation 准备）

### 状态
- **准备阶段：** completed。
- **正式验证：** 尚未运行 `GamesPerSeed=32/64`。
- **Phase 3：** 仍是 `implementation complete, trend validation failed`。
- **Phase 4：** 仍 blocked。

### 执行的操作
- 读取并复核 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 运行 `git status --short`，确认当前工作区仍有 B1/B2.2 相关未提交改动和 untracked 路线图；本轮未提交、未回滚、未清理。
- 复核 `scripts\benchmark-select.ps1` 与 `scripts\benchmark-train.ps1` 默认参数和 selection 判级逻辑；确认 perf regress 阈值仍为 `1.25`，未修改脚本。
- 复核 `build-local\training\20260522-145835` 与 `build-local\training\20260522-150225`，明确 training smoke 与 `GamesPerSeed=2` selection smoke 只能作为入口/产物链路证据。
- 检查 `build-vs-train-release\bin\surakarta-benchmark.exe`：文件存在；轻量启动输出 CLI usage，证明可启动；未重建。
- 复核 official perf baseline SHA256 与两次正式默认 perf gate 结果，确认 B2.2 baseline 可作为 fresh trend validation 的 perf 基线。
- 更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`，记录 Phase 3 正式验证路线和停止条件。

### 关键检查结果
| 检查 | 结果 |
|---|---|
| git status | 既有 B1/B2.2 源码、baseline、记忆文件仍 modified；`docs\national_competition_technical_roadmap.md` untracked |
| official baseline SHA256 | source/target 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB` |
| official perf gate | `20260522-165717` 与 `20260522-165815` 均 pass，`failure_count=0` |
| benchmark exe | `build-vs-train-release\bin\surakarta-benchmark.exe` 存在并可输出 usage |
| training smoke | `20260522-145835`：`reproducible=True`，artifact count `8`，不是 strength evidence |
| selection smoke | `20260522-150225`：`GamesPerSeed=2`，`overall_rating=regress`、`strength_rating=watch`、`perf_rating=regress`，只作为入口验证 |

### 封口结论
- 第一关应是 fresh post-B1 `GamesPerSeed=32` selection；当前不再做更小规模预检。
- 32 局若 `overall_rating=regress`，停止，不跑 64，进入 blocker 分类。
- 32 局至少 `watch`，才允许跑 64。
- 64 局不得 `regress`，才允许讨论 Phase 3 从 failed 升级为 watch/pass；即便 32/64 通过，也不能直接宣称“国赛级”。

## 会话：2026-05-22（阶段 A 接手复核）

### 阶段：manual acceptance 归档复核
- **状态：** completed
- **任务性质：** 接手后验证阶段 A 证据链；未修改源代码、脚本、训练、搜索、特征、selection gate，未进入阶段 B。
- 执行的操作：
  - 重新读取 `task_plan.md`、`findings.md`、`progress.md`，确认前一轮已将 manual acceptance 结果落盘。
  - 检查正式归档目录 `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321` 的文件和截图。
  - 运行归档断言：run root 存在，缺失文件数为 0，`result.txt` 为 `PASS` 且无 `PENDING`，`checklist.md` 无空 `Actual` / `Conclusion`，截图均存在且非空。
  - 检查 git diff，确认 `src/`、`include/`、`scripts/`、`test/`、`README.md`、`docs/school_competition_defense.md` 无本轮 diff，`src/surakarta_gui_main.cpp` 无 diff。
- 创建/修改的文件：
  - `D:\Dev\Code\surakarta_traditional\task_plan.md`
  - `D:\Dev\Code\surakarta_traditional\findings.md`
  - `D:\Dev\Code\surakarta_traditional\progress.md`

### 验证结果
| 检查 | 结果 | 状态 |
|---|---|---|
| run root | `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321` 存在 | 通过 |
| 必需归档文件 | `checklist.md`、`commands.txt`、`git-commit.txt`、`result.txt`、`screenshots\` 均存在 | 通过 |
| result | `Manual acceptance result: PASS`，不含 `PENDING` | 通过 |
| checklist | 空 `Actual:` 数 0，空 `Conclusion:` 数 0 | 通过 |
| 截图 | 必需截图和场景 3/4 旁证截图均存在且非空 | 通过 |
| 源码/脚本 diff | `src/`、`include/`、`scripts/`、`test/`、GUI 入口无 diff | 通过 |

### 当前结论
- 阶段 A 校赛冻结证据链已封口，可以切入阶段 B：大赛终局语义与规则完整性。
- 阶段 B 尚未开始。
- Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。

## 会话：2026-05-21（阶段 A manual acceptance 封口）

### 阶段：manual acceptance 归档闭环
- **状态：** completed
- **任务性质：** 校赛冻结证据链归档；未修改训练、搜索、特征、selection gate，未进入阶段 B。
- 执行的操作：
  - 读取 `task_plan.md`、`findings.md`、`progress.md`、`README.md`、`docs/school_competition_defense.md`、`scripts/manual-acceptance.ps1`、`test/manual_acceptance_checklist.md`。
  - 确认 `scripts/manual-acceptance.ps1` 的实际行为：configure/build `surakarta-gui`，创建 `build-local/manual-acceptance/<timestamp>/`，复制 checklist，写 `commands.txt` / `git-commit.txt` / 初始 `result.txt=PENDING`，创建 `screenshots\` 并启动 GUI。
  - 按用户纠偏停止鼠标注入 / `SendInput` / `PostMessage` / GUI acceptance-only 自动动作路线；撤回已准备的 GUI 自动场景改动，确认 `src/surakarta_gui_main.cpp` 无内容 diff。
  - 执行 `pwsh ./scripts/manual-acceptance.ps1 -Depth 10 -Threads 10`。第一次因旧 GUI 进程占用 `bin\surakarta-gui.exe` 导致链接 `LNK1104`，关闭旧进程后原命令重跑成功。
  - 正式归档目录：`D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321`。
  - 用户人工完成 GUI checklist 场景和截图；本轮检查截图内容，并将默认中文截图名复制为正式审计文件名。
  - 填写归档内 `checklist.md` 的 `Actual` / `Conclusion`，将 `result.txt` 从 `PENDING` 更新为 `Manual acceptance result: PASS`。
- 创建/修改的文件：
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\checklist.md`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\result.txt`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game1-targets.png`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game1-capture-gate.png`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game2-white-capture-a.png`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game2-white-capture-b.png`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game6-checkmate.png`
  - `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321\screenshots\game6-stalemate.png`
  - `D:\Dev\Code\surakarta_traditional\task_plan.md`
  - `D:\Dev\Code\surakarta_traditional\findings.md`
  - `D:\Dev\Code\surakarta_traditional\progress.md`

### 验收结果
| 检查 | 结果 | 状态 |
|---|---|---|
| run root | `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321` 存在 | 通过 |
| 必需文件 | `checklist.md`、`commands.txt`、`git-commit.txt`、`result.txt`、`screenshots\` 存在 | 通过 |
| checklist | 6 个正式场景均填写 Actual / Conclusion | 通过 |
| result | `Manual acceptance result: PASS`，不再是 `PENDING` | 通过 |
| 必需截图 | `opening-move.png`、`game1-targets.png`、`game6-checkmate.png`、`game6-stalemate.png` 存在 | 通过 |
| 额外截图 | `game1-capture-gate.png`、`game2-white-capture-a.png`、`game2-white-capture-b.png` 存在 | 通过 |

### 当前结论
- 阶段 A 校赛冻结证据链已封口，可以切入阶段 B：大赛终局语义与规则完整性。
- 阶段 B 尚未开始；本轮没有改终局语义。
- Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。

## 会话：2026-05-21（校赛冻结证据链与终局语义复核）

### 阶段：校赛证据链复核 / 终局语义审计
- **状态：** completed
- **任务性质：** 仅文档与证据审计；未修改生产代码、搜索算法、棋规、训练逻辑或 GUI 行为。
- 执行的操作：
  - 重新读取 `task_plan.md`、`findings.md`、`progress.md`、`README.md`、`docs/school_competition_defense.md`、`docs/national_competition_technical_roadmap.md`。
  - 读取 `scripts/manual-acceptance.ps1`、`test/manual_acceptance_checklist.md`，确认 manual acceptance 的实际归档要求。
  - 复核 `src/surakarta_rule_manager_impl.cpp`、`src/surakarta_rule_manager_bitboard.cpp`、`src/surakarta_search.cpp`、`src/surakarta_utils.cpp`、`src/surakarta_gui_main.cpp`，确认终局语义仍是 legacy no-capture 计数，且 strict “双方都不能再吃子” 语义尚未接入。
  - 用 `Test-Path` / `Get-ChildItem` 检查当前工作区，确认没有 `build-local/manual-acceptance` 归档目录。
- 创建/修改的文件：
  - `D:\Dev\Code\surakarta_traditional\task_plan.md`
  - `D:\Dev\Code\surakarta_traditional\findings.md`
  - `D:\Dev\Code\surakarta_traditional\progress.md`

### 当前结论
- 校赛功能面已经够稳，但 stage A 还不能算真正封口，因为 manual acceptance 归档没落盘。
- P0 capture-rule golden 够强，但只证明 capture regression 修好了，不证明大赛终局语义已经对齐。
- 下一步如果要进入阶段 B，必须先做最小终局语义修复设计，再补 golden / parity / GUI 终局展示；不碰训练 sweep、不扩特征、不调搜索参数。

### 验证结果
| 检查 | 输入 | 结果 | 状态 |
|---|---|---|---|
| manual acceptance 归档目录 | `Test-Path build-local/manual-acceptance` | `False` | 通过 |
| workspace build-local | `Get-ChildItem build-local` | path not found | 通过 |
| manual acceptance scaffold | `scripts/manual-acceptance.ps1` | 只生成 scaffold，`result.txt` 初始为 `PENDING` | 通过 |
| 终局语义路径 | `JudgeEnd` / `EvaluateBitboardTerminal` / `EvaluateTerminal` | 仍是 legacy no_capture 计数 | 通过 |

## 会话：2026-05-21（全项目复核与国赛技术路线图）

### 阶段：项目总纲与路线规划更新
- **状态：** completed
- **任务性质：** 文档/规划/记忆更新；未修改生产代码、搜索算法、棋规、训练逻辑或 GUI 行为。
- 执行的操作：
  - 读取并复核 `task_plan.md`、`findings.md`、`progress.md`、`README.md`、`docs/school_competition_defense.md`。
  - 检查源码结构：`include/`、`src/`、`scripts/`、`test/`、`docs/`。
  - 复核关键能力：bitboard movegen、capture variants、PVS/qsearch/TT/LMR/Null Move/Lazy SMP、N-Tuple、TD/self-play、selection、GUI Match preset、Undo Human Turn、CTest/Pester workflow。
  - 明确当前状态：校赛版本可用；Phase 3 训练趋势仍 failed；Phase 4 继续 blocked；P0 capture-rule 已修复并加固；终局语义仍需后续单独处理。
  - 新增国赛长期路线图文档，并同步更新三个根目录记忆文件。
- 创建/修改的文件：
  - `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`
  - `D:\Dev\Code\surakarta_traditional\task_plan.md`
  - `D:\Dev\Code\surakarta_traditional\findings.md`
  - `D:\Dev\Code\surakarta_traditional\progress.md`

### 测试结果（2026-05-21）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| bitboard selftest | `build\bin\surakarta-bitboard-selftest.exe` | 规则、搜索、训练、mask 自测通过 | `[PASS] surakarta-bitboard-selftest` | 通过 |
| dev-session selftest | `build\bin\surakarta-dev-session-selftest.exe` | GUI session / undo / engine reply 自测通过 | `[PASS] surakarta-dev-session-selftest` | 通过 |
| Pester workflow | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet` | PowerShell workflow 测试通过 | `26 passed / 0 failed` | 通过 |
| CTest | `Invoke-SurakartaCTest -BuildDir build` | 已注册 C++/GUI smoke 测试通过 | `8/8 passed` | 通过 |

### 当前结论
- 校赛短期路线：锁定稳定版本，优先规则正确性、GUI 可操作性、D10/T10 preset 实战验证和答辩证据链。
- 国赛长期路线：先补终局语义和 post-fix baseline，再重启训练稳定性；训练通过前不扩 Phase 4 特征；稳定评估函数之后再进入搜索调参和 time management。
- 不能写成已完成的内容：Phase 3 trend passed、Phase 4 started/completed、TD 已训练出稳定最强权重、大赛第 6 条终局已严格实现。

---

## 会话：2026-04-23

### 阶段 1：现状摸底与差距分析
- **状态：** in_progress
- **开始时间：** 2026-04-23
- 执行的操作：
  - 读取 `brainstorming`、`writing-plans`、`planning-with-files-zh` 技能内容
  - 确认真正项目根目录
  - 探索 `src/`、`include/`、`scripts/` 目录结构
  - 阅读 `surakarta_search.h`、`surakarta_search.cpp`、`surakarta_masks.h`、`surakarta_bitboard.cpp`、`main.cpp`、`perf-runner.ps1`
  - 阅读 `surakarta_bitboard_selftest.cpp` 的搜索/benchmark 相关段落
  - 读取现有 `build-local/perf/20260423-114445/perf-summary.txt`，确认当前 NPS 与多线程缩放
  - 搜索仓库中是否存在 TD/self-play/权重训练闭环
  - 识别方案 B 已有能力与潜在缺口
- 创建/修改的文件：
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 阶段 2：阶段目标拆解
- **状态：** pending
- 执行的操作：
  - 待开始
- 创建/修改的文件：
  - 无

## 测试结果
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| 仓库现状探索 | 搜索/位棋盘/脚本源码 | 识别已具备能力与缺口 | 已确认搜索骨架基本齐全 | 通过 |
| 历史 perf 基线读取 | `build-local/perf/20260423-114445/perf-summary.txt` | 得到当前 NPS 和缩放情况 | 已确认中局 4T 约 7.29M NPS，开局/残局缩放不均 | 通过 |
| 训练闭环排查 | 源码/脚本全文检索 | 确认是否已有 TD/self-play 训练器 | 未发现训练器或训练脚本，仅有权重加载接口 | 通过 |

## 错误日志
| 时间戳 | 错误 | 尝试次数 | 解决方案 |
|--------|------|---------|---------|
| 2026-04-23 | 误用旧仓库路径 `surakarta-core-main` | 1 | 改为真实根目录 `D:\\Dev\\Code\\surakarta_traditional` |

## 五问重启检查
| 问题 | 答案 |
|------|------|
| 我在哪里？ | 阶段 1：现状摸底与差距分析 |
| 我要去哪里？ | 阶段 2：阶段目标拆解与主线选择 |
| 目标是什么？ | 给出方案 B 下一阶段任务清单与优先级 |
| 我学到了什么？ | 当前仓库已经有搜索骨架、N-Tuple 评估、预计算 masks、基准脚本，但没有 TD/self-play 训练闭环 |
| 我做了什么？ | 已完成核心源码、自测与 perf 基线摸底，并创建规划文件 |

---
*每个阶段完成后或遇到错误时更新此文件*

## 会话：2026-05-02

### Phase 1B：验收门禁 + 多 Seed 观察选择
- **状态：** completed
- **开始时间：** 2026-05-02
- 执行的操作：
  - 复核 `benchmark-select.ps1` / `workflow.tests.ps1` / `README.md` 改动
  - 运行 `Invoke-Pester -Script 'scripts/workflow.tests.ps1' -PassThru -Quiet`
  - 运行真实 `pwsh ./scripts/benchmark-select.ps1`
  - 发现并修复 `benchmark-select.ps1` 的 `BuildDir` 绝对路径归一化问题
  - 扩展 `benchmark-select.ps1` 支持 `alpha` / `lambda` / `epsilon` / `epsilon_plies` 透传
  - 跑 8 组小矩阵：`alpha 0.005/0.01` × `lambda 0.5/0.7` × `epsilon 0.05/0.10`
  - 复核矩阵 JSON/TXT 汇总，确认没有 `pass` 配置
- 创建/修改的文件：
  - `D:\\Dev\\Code\\surakarta_traditional\\README.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`

### 结论
- 默认 selection 跑通，输出目录：`build-local/training/20260502-075547`
- 三个默认 seed 全部 `regress`
- 矩阵里相对最好的组合是 `alpha=0.005, lambda=0.5, epsilon=0.05`，但仍然是 `regress`
- 下一步应优先继续调训练参数，而不是动搜索排序策略

## 会话：2026-05-02（续）

### Selection 评级口径与长训练观察
- **状态：** completed
- 执行的操作：
  - 为 `benchmark-select.ps1` 增加小节点固定局面的性能判定修正：当 candidate/baseline 节点数均低于 500 时，`performance_ratio` 使用 `node_ratio`，避免微秒级 `search_seconds` 噪声一票否决。
  - 在 `workflow.tests.ps1` 增加回归测试，覆盖 36 节点残局中 `search_seconds` 比值很大但节点数相同的场景。
  - 运行 `Invoke-Pester -Script 'scripts/workflow.tests.ps1' -PassThru -Quiet`，结果 `11 passed / 0 failed`。
  - 使用 `alpha=0.0035, lambda=0.5, epsilon=0.05` 跑 `GamesPerSeed=32` 与 `GamesPerSeed=64` 稳定性观察。
- 输出目录：
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260502-084957`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260502-085044`

### 结论
- 新性能口径有效：残局 36 节点局面改用 `node_ratio_tiny_position`，不再因为微秒级耗时波动触发 `regress`。
- `alpha=0.0035, lambda=0.5, epsilon=0.05` 长训练不稳定：
  - `GamesPerSeed=32`：candidate/baseline/draws = `8/10/0`
  - `GamesPerSeed=64`：candidate/baseline/draws = `6/12/0`
- 主要问题已经不是小节点计时噪声，而是 opening 搜索节点膨胀和 candidate 胜负随训练局数增加恶化。

### 长期路线规划请求
- **状态：** in_progress
- 用户要求：给出从当前仓库到“PVS + Aspiration Windows + TD-trained Evaluation + Null Move + LMR”的完整长期计划。
- 规划前提：
  - 搜索骨架已有，不应重复把 PVS/TT/LMR/Null Move 当作未实现主线。
  - 当前最大短板是 TD 训练质量、评估特征表达、训练数据规模、selection 可信度和权重发布流程。
  - 搜索增强必须排在稳定权重收益之后，否则容易把评估噪声放大。

## 会话：2026-05-02（阶段 2 实施）

### 基线与可追溯性门禁
- **状态：** completed
- 执行的操作：
  - 在 `common.ps1` 增加权重 header 读取、git commit、SHA256、manifest 生成与 traceability 校验 helper。
  - 在 `benchmark-train.ps1` / `benchmark-select.ps1` 为 `bootstrap.bin`、`candidate.bin`、checkpoint 写出 `*.manifest.json`，并在会话根写 `weights-manifest.json`。
  - 将 `benchmark-train.ps1` 扩展为透传 `alpha` / `lambda` / `epsilon` / `epsilon_plies`，让训练 smoke 和 selection 的 metadata 结构一致。
  - 将 `perf-runner.ps1` 升级为读取 `test/test_data/perf-baseline.json` 的独立 perf baseline gate，输出 `perf-gate.json` / `perf-gate.txt`，并按 best move、score、performance ratio 判失败。
  - 增加 Pester 覆盖：manifest sidecar、缺失追溯字段、27 项 perf baseline 矩阵、tiny-position 计时噪声。
- 验证：
  - `Invoke-Pester -Script 'scripts/workflow.tests.ps1' -PassThru -Quiet`
  - 结果：`16 passed / 0 failed`

## 会话：2026-05-02（阶段 3 趋势验收 regress triage）

### 阶段 3：implementation complete, trend validation failed
- **状态：** failed validation / not completed
- **执行的操作：**
  - 读取并核对 `build-local/training/20260502-113208/selection-summary.json` 与 `selection-summary.txt`。
  - 检查 `benchmark-select.ps1` 的 scorecard 规则，确认 `candidate_wins < baseline_wins`、`score_delta < 0`、`performance_ratio > 1.25` 的判定方向。
  - 检查 `RunBitboardEvaluation` / `PlayEvaluationMatchGame` / `BuildEvalSearchReport`，确认 baseline 与 candidate 使用相同 depth/threads，match 按 candidate 黑白各一局对称执行。
  - 检查 manifest 与训练 stdout，确认 candidate 从 bootstrap 初始化，checkpoint-2/4/8/16/32 的评级与 TD error / weight delta。
  - 对 seed `20260424` final candidate/baseline 复跑 5 次 `bitboard-eval --depth 4 --format json`，重点核对 opening 与 threat-heavy 性能比。
  - 更新 `task_plan.md`、`findings.md`、`progress.md`，明确 Phase 4 blocked by GamesPerSeed=32 regress。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 结论
- `GamesPerSeed=32` 验收不通过：overall_rating=`regress`，三个 seed 全部 regress，candidate/baseline/draws = `12/18/0`。
- selection 没发现统计方向写反；fixed score regression 符号没发现全局反号；performance ratio 是 candidate/baseline。
- checkpoint-2 已开始 regress，checkpoint-4 起胜负/score/best-move 退步更明显。
- `20260424` opening 复跑仍显示节点比 `1841/1472 = 1.250679`，属于真实性能/搜索树退步；`threat-heavy` 复跑节点相同、耗时比回到 parity 附近，属于计时噪声。
- opening best move 三个 seed 全部漂移，且 score delta 有负有正，更像 TD 权重真实漂移，不像 side-to-move 或 score perspective 整体 bug。

## 测试结果（2026-05-02 regress triage）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| selection summary 复核 | `20260502-113208` | 确认用户给定事实来源 | overall `regress`，3 seed regress，12/18/0 | 通过 |
| checkpoint 复核 | checkpoint-2/4/8/16/32 | 找出最早 regress 点 | checkpoint-2 已 regress | 通过 |
| 20260424 opening/threat-heavy 复跑 | 5 次 `bitboard-eval` | 判断性能退步真假 | opening 真实退步，threat-heavy 计时噪声 | 通过 |

## 会话：2026-05-02（Phase 3 training stabilization）

### 阶段 3：保持 implementation complete, trend validation failed
- **状态：** failed validation / not completed
- **执行的操作：**
  - 用 TDD 给单个 artifact manifest 增加 `checkpoint_every` 断言，先看到 Pester 失败，再补实现。
  - 修改 `common.ps1`、`benchmark-train.ps1`、`benchmark-select.ps1`，让 `bootstrap`、`candidate`、`checkpoint` manifest 都写入 `checkpoint_every`。
  - 保留 selection 判级逻辑，没有把任何 regress 改成 watch。
  - 跑 12 组稳定性 sweep：`alpha=0.001/0.0025/0.005`，`lambda=0.3/0.5`，`epsilon=0.02/0.05`，seeds=`20260423,20260424,20260425`，`GamesPerSeed=32`，`CheckpointEvery=2`。
  - 将 sweep 结果落盘到 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-stability-sweep-20260502-115207`。
  - 将默认训练参数从 `0.01/0.7/0.10` 改为最稳候选 `alpha=0.005, lambda=0.5, epsilon=0.02`。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\common.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-train.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_search.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### Sweep 结论
- 全部 12 组仍为 `regress`，没有达到 `watch`。
- 相比原始 `alpha=0.01, lambda=0.7, epsilon=0.1` 的 `12/18/0` 与 max weight delta `739.324816`，最稳候选 `0.005/0.5/0.02` 达到 `15/15/0`、fixed score regressions `0`、earliest regress `checkpoint-4`、max weight delta `311.331578`。
- `0.001/0.3/0.02` 的 opening node ratio 最低（avg/max `1.066802/1.169837`）且胜负 `17/13/0`，但 3 个 seed 都有 fixed score regression，所以不能作为更稳选择。
- 下一轮应查 TD target / reward scaling / side-to-move perspective。weight delta 随 alpha 缩放明显，weight write/read 不是第一嫌疑。

## 测试结果（2026-05-02 stabilization）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester workflow | `Invoke-Pester -Script scripts/workflow.tests.ps1 -PassThru -Quiet` | manifest/default 参数测试通过 | `17 passed / 0 failed` | 通过 |
| C++ build | `Get-CMakePath ...; cmake --build build-vs-train-release --target surakarta-bitboard-selftest surakarta-benchmark` | 构建目标可用 | `ninja: no work to do.` | 通过 |
| bitboard selftest | `build-vs-train-release/bin/surakarta-bitboard-selftest.exe` | 自测通过 | `[PASS] surakarta-bitboard-selftest` | 通过 |
| stability sweep | 12 组参数矩阵 | 找出是否有 watch/pass | 全部 regress，最稳为 `0.005/0.5/0.02` | 通过 |

## 会话：2026-05-03（Phase 3 learning signal isolation）

### 阶段 3：保持 implementation complete, trend validation failed
- **状态：** failed validation / not completed
- **执行的操作：**
  - 新增 `bitboard-trace` CLI，按步输出 TD trace JSON/CSV 摘要，字段覆盖 `ply` / `side_to_move` / `reward` / `value_before` / `value_after` / `bootstrap_value` / `td_error` / `trace_norm` / `weight_delta_norm` / `terminal_reason`
  - 扩展 `surakarta-bitboard-selftest`，覆盖 terminal reward 符号、`lambda=0` trace locality、黑白终局小 alpha 方向一致性，以及 `bitboard-trace` CLI smoke
  - 重新构建 `surakarta-benchmark` 和 `surakarta-bitboard-selftest`
  - 跑 `bitboard-trace --games 2 --seed 20260423 --depth 4 --alpha 0.005 --lambda 0.5 --epsilon 0.02 --epsilon-plies 6 --format json`
- **输出文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-learning-signal-isolation-20260503\\td-trace-seed-20260423-games2.json`

### 结论
- `Alpha=0` 链路仍稳定，candidate 与 baseline 权重完全一致，不是 artifact / selection 链路 bug。
- `Lambda=0` 仍然 regress，eligibility trace 不是唯一主因。
- `Epsilon=0` 能减少一部分 opening drift，但不能单独救回趋势。
- `bitboard-trace` 显示早期步的 bootstrap / td_error 还算温和，但终局步会跳到 `1042+` / `1259+` 级别，`weight_delta_norm` 也被放大到 `60+` / `76+`，更像稀疏终局信号在拉爆前面所有步。
- 这轮诊断后，下一步仍应停在 Phase 3，优先看 near-terminal curriculum / reward scaling / target shaping，而不是开 Phase 4。

## 会话：2026-05-03（Phase 3 reward stabilization）

### 阶段 3：reward stabilization / clipping / curriculum diagnostic
- **状态：** failed validation / not completed
- **执行的操作：**
  - 修复 `benchmark-train.ps1` / `benchmark-select.ps1` 中 `Wait-Process -Id` 的短命进程竞态，改为 `$process.WaitForExit()`。
  - 补 Pester harness 红灯：fake process 增加 `WaitForExit()`，fake `Wait-Process` 抛错，确认旧实现失败后再修复。
  - 跑 reward scale sweep：`TerminalReward=1200/1000/500/250/100`，固定 `alpha=0.005, lambda=0.5, epsilon=0.02, GamesPerSeed=32, CheckpointEvery=2`。
  - 跑 TD error clipping sweep：`TerminalReward=500`，`TdErrorClip=500/250/100`。
  - 实现并诊断最小 near-terminal curriculum：前 N 局从一手终局短样本开始，交替黑/白视角；不改搜索，不扩特征。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_search.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_search.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\main.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard_selftest.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-train.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 结果
- Reward scale sweep 全部 `regress`；`TerminalReward=500` 最稳但 opening node ratio max `1.432065`。
- Clip sweep 全部 `regress`；`TerminalReward=500, TdErrorClip=250` 最接近 watch：C/B/D=`16/13/1`、score regressions=`0`、terminal td_error max=`250`、terminal weight_delta_norm max=`15.515945`，但 opening node ratio max=`1.295516`。
- Near-terminal `games=8` 诊断能学局部 reward：8/8 一步 checkmate，黑方 value `-18 -> 83.25`，白方 value `-59.25 -> 42.0`。
- `GamesPerSeed=32` 仍未达到 watch/pass；按门禁不跑 64，Phase 4 继续 blocked。

## 测试结果（2026-05-03 reward stabilization）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester 红灯 | fake `Wait-Process` 抛错 | 旧脚本失败 | `benchmark-harness.ps1` 抛出 WaitForExit 要求 | 通过 |
| Pester workflow | `Invoke-Pester -Script scripts/workflow.tests.ps1 -PassThru -Quiet` | 17/17 通过 | `17 passed / 0 failed` | 通过 |
| bitboard selftest | `surakarta-bitboard-selftest.exe` | 自测通过 | `[PASS] surakarta-bitboard-selftest` | 通过 |
| trace smoke | `bitboard-trace --format json/csv --near-terminal-curriculum 2` | 输出稳定化参数与 terminal step | JSON/CSV 均包含 `terminal_reward`、`td_error_clip`、`near_terminal_curriculum` | 通过 |
| reward scale sweep | 5 组 TerminalReward | 找到是否 watch/pass | 全部 `regress` | 通过 |
| clip sweep | 3 组 TdErrorClip | 找到是否 watch/pass | 全部 `regress` | 通过 |
| curriculum diagnostic | `near_terminal_curriculum=8` | 证明局部 terminal reward 可学 | 8/8 一步 checkmate，value 朝 reward 方向移动 | 通过 |

## 会话：2026-05-04（Phase 3 opening node-ratio attribution）

### 阶段 3：继续 blocked，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 读取 `task_plan.md`、`findings.md`、`progress.md`，确认当前仍停在 Phase 3。
  - 定位最好组合产物：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333`。
  - 从 `selection-summary.json` 列出 3 个 seed 的 opening baseline/candidate best move、score、nodes、time、node ratio、score delta 和 best move drift。
  - 对 3 个 seed 各重跑 5 次 `bitboard-eval --candidate ... --baseline ... --depth 4 --format json`；确认该子命令内部固定 `threads=1`，CLI 不接受 `--threads`。
  - 生成临时 opening child board 文件到 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-opening-node-attribution-20260504`，用 `bitboard-search --depth 3 --threads 1 --format json` 计算 root move score 列表。
  - 跑 baseline 与 3 个 candidate 的 opening root `bitboard-search --depth 4 --threads 1 --format json`，对比 root stats。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-opening-node-attribution-20260504\\*.txt`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 结果
- opening 原始归因：
  - seed `20260423`: candidate/baseline nodes `1653/1472`，node ratio `1.122962`，score delta `+59`，best move 未漂。
  - seed `20260424`: candidate/baseline nodes `1490/1472`，node ratio `1.012228`，score delta `+189`，best move 漂移到 `(1, 1) -> (2, 2) (B)`。
  - seed `20260425`: candidate/baseline nodes `1907/1472`，node ratio `1.295516`，score delta `+74`，best move 漂移到 `(1, 1) -> (2, 2) (B)`。
- 5 次重跑 nodes 完全稳定：三个 seed 的 baseline/candidate nodes 集合分别固定为 `1472/1653`、`1472/1490`、`1472/1907`。
- `1.295516` 是稳定 opening search-tree inflation，不是单次路径偶发或 time 抖动。
- root move score 对比显示 candidate top 2 并列，但 baseline 也 top 2 并列，且 baseline top 5 更接近；因此“多个 root move 评得太接近”不是单独根因。
- candidate `20260425` root stats：nodes `1907`、qnodes `1273`、root_best_updates `8`、fail_highs `1`、aspiration_retries `1`；baseline 为 nodes `1472`、qnodes `1010`、root_best_updates `5`、retries `0`。
- reward spike 已压住，当前 blocker 从 terminal TD spike 转移到 opening search-tree inflation。Phase 4 继续 blocked。

## 测试结果（2026-05-04 opening attribution）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| opening eval 5 次重跑 | 3 seed final candidate vs bootstrap, depth 4 | 判断 nodes 是否稳定 | nodes 完全稳定，max `1907/1472=1.295516` | 通过 |
| root move score attribution | opening 16 个合法 quiet move child search | 比较 candidate/baseline top move score | candidate 抬高多个 child，但 root score closeness 非单独根因 | 通过 |
| root stats 对比 | baseline 与 3 个 candidate depth 4 | 找到 node inflation 伴随信号 | `20260425` 出现 `fail_high=1`、`aspiration_retries=1`、qnodes 增加 | 通过 |

## 错误日志补充（2026-05-04）
| 时间戳 | 错误 | 尝试次数 | 解决方案 |
|--------|------|---------|---------|
| 2026-05-04 | 误读 `clip-250\\selection-summary.*`，实际只有 `clip-result.json` 指向 selection root | 1 | 读取 `clip-result.json` 的 `selection_root` 后转到 `build-local\\training\\20260503-234333` |

## 会话：2026-05-04（Phase 3 opening search-tree inflation targeted diagnosis）

### 阶段 3：继续 blocked，不进入 Phase 4
- **状态：** failed validation / not completed
- **执行的操作：**
  - 为 `SearchLimits` 增加默认关闭的 `enable_diagnostics`，并通过 `bitboard-search --search-diagnostics` / `--debug-search-tree` 输出 root move、aspiration attempt、qsearch source 诊断。
  - 诊断只记录数据，不修改 PVS、aspiration、qsearch、move ordering 或 selection 判级。
  - 构建 `surakarta-benchmark` / `surakarta-bitboard-selftest`，再对 opening / seed `20260425` 的 bootstrap 与 candidate 各跑 depth 4 单线程诊断。
  - 对比 baseline/candidate root PV、accepted depth-4 root top 5、每轮 aspiration window、qsearch source 聚合。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_search.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_search.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\main.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 结果
- baseline root PV：`(0, 1) -> (1, 2) (B) | (3, 4) -> (4, 3) (W) | (0, 0) -> (0, 1) (B) | (2, 5) -> (3, 4) (W)`。
- candidate root PV：`(1, 1) -> (2, 2) (B) | (0, 4) -> (1, 3) (W) | (0, 1) -> (1, 1) (B) | (1, 3) -> (2, 3) (W)`。
- baseline/candidate stats：nodes `1472/1907`，qnodes `1010/1273`，root_best_updates `5/8`，candidate `fail_highs=1` / `aspiration_retries=1`。
- aspiration retry 只解释一部分：candidate depth-3 first attempt fail-high 额外 `111` nodes / `77` qnodes，占 qnode inflation 约 `29.3%`。
- 主因是 PV 变化进入更贵子树：`(1, 1) -> (2, 2) (B)` 的 qnodes 从 baseline 同招 `18` 增到 candidate `469`，同时 baseline 原 PV root `(0, 1) -> (1, 2) (B)` 从 `357` 降到 candidate `173`。
- qsearch 是分布式变多，不是一条 line 爆炸：单条 qsearch source 最大只有 `2` qnodes，但 candidate PV root 下 qsearch entry 与 capture continuation 明显增加。
- selection 建议仅记录：未来可考虑“单点 max 超阈但 aggregate 胜负提升、fixed score 无回退”归 `watch` 的候选规则；本轮不放宽。

## 测试结果（2026-05-04 targeted diagnosis）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| C++ clean build | `Invoke-SurakartaBuild -BuildDir build-vs-train-release -CleanFirst -Targets surakarta-benchmark,surakarta-bitboard-selftest` | 诊断代码可编译且 DLL/EXE ABI 一致 | 构建成功；保留既有 MSVC warnings | 通过 |
| bitboard selftest | `surakarta-bitboard-selftest.exe` | 自测通过 | `[PASS] surakarta-bitboard-selftest` | 通过 |
| diagnostics default-off | candidate opening, depth 4, no debug flag | JSON 默认不包含 diagnostics | `HAS_DIAGNOSTICS=False` | 通过 |
| opening search diagnostics | seed `20260425` bootstrap/candidate, depth 4, threads 1 | 输出 root/aspiration/qsearch 诊断 | 成功输出并复现 `1907/1472` | 通过 |
| selection guard review | findings only | 不修改判级 | 只记录未来 watch 规则建议，selection 逻辑未改 | 通过 |
| 2026-05-04 | `bitboard-eval --threads 1` 报 `Unknown option: --threads` | 1 | 确认 `RunBitboardEvaluation` 内部 `NormalizeSingleThreadLimits` 固定单线程，重跑时移除 `--threads` |
| 2026-05-04 | PowerShell `foreach { ... } | Format-Table` 脚本写法触发 `An empty pipe element is not allowed` | 1 | 将 foreach 输出先收集到数组，再格式化输出 |

## 会话：2026-05-04（Phase 3 gate policy review）

### 阶段 3：只做 policy review，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 复核 `benchmark-select.ps1` 当前 seed 级与 overall 级判级规则。
  - 将当前 `TerminalReward=500, TdErrorClip=250` 组合的 gate 结论写入 `findings.md`。
  - 只做文档分析，没有修改 selection 判级、PVS、aspiration、qsearch、move ordering 或特征。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 结论
- 当前 selection 判 `regress` 符合现有规则：seed 级任一 `candidate_wins < baseline_wins`、fixed score regressions `>0`、或 max performance ratio `>1.25` 都是 `regress`；overall 级只要有任一 seed `regress` 就是 `regress`。
- 在当前诊断事实下，seed `20260425` opening node ratio `1907/1472 = 1.295516` 已经超过 `1.25`，且 5 次重跑稳定，不是 time noise。
- 现有规则保护的是训练权重把固定局面 search tree 真实放大的风险；aggregate `16/13/1` 与 fixed score regressions `0` 说明有强度信号，但不能抵消稳定性能退步。
- 未来候选规则可以讨论拆分 `strength_rating` / `perf_rating`，以及把孤立单点 max node ratio 超阈降为 `perf_watch`；前提应至少包括 aggregate 胜负不退步、fixed score regressions `0`、只有单 seed/单 fixed position performance max 超阈、repeat-run nodes 稳定且 attribution 清楚。
- 若真要降级成 `watch`，建议要求 `GamesPerSeed=64` 不退步并扩大 seed 覆盖；`64` 局能看训练趋势，更多 seed 才能判断是否只是孤立 performance outlier。
- 当前代码状态仍按现有规则判 `regress`；`task_plan.md` 保持 Phase 3 `implementation complete, trend validation failed`，Phase 4 继续 blocked。

## 测试结果（2026-05-04 gate policy review）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| selection rule review | `scripts\\benchmark-select.ps1` | 确认现有规则是否会判 regress | 规则仍为 seed regress 一票触发 overall regress | 通过 |
| docs-only guard | `findings.md` / `progress.md` | 不改 selection 判级代码 | 只追加 policy review 文档 | 通过 |

## 会话：2026-05-04（Phase 3 decision run, GamesPerSeed=64）

### 阶段 3：policy evidence run，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 使用现有 `benchmark-select.ps1` 运行 `GamesPerSeed=64`，参数为 `Alpha=0.005`、`Lambda=0.5`、`Epsilon=0.02`、`TerminalReward=500`、`TdErrorClip=250`、`CheckpointEvery=2`。
  - 解析 `selection-summary.json`，汇总 overall rating、per-seed rating、fixed score regressions、opening node ratio、checkpoint trend。
  - 对 final 3 个 seed 的 opening eval 各重跑 5 次，区分 node stability 与 `search_seconds` ratio 波动。
  - 没有修改搜索、特征或 selection 判级。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006\\selection-summary.json`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006\\selection-summary.txt`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 结果
- existing rules 下 `overall_rating=regress`。
- final C/B/D = `16/12/2`；fixed score regressions = `0`；opening node ratio avg/max = `1.122056 / 1.245924`。
- per seed：
  - `20260423`: `regress`，C/B/D=`5/4/1`，原因是 opening performance ratio `1.368354`。
  - `20260424`: `regress`，C/B/D=`5/4/1`，原因是 opening performance ratio `1.386007`。
  - `20260425`: `watch`，C/B/D=`6/4/0`，fixed best move 不漂，max performance ratio `1.050360`。
- 不再由 `20260425` opening 单点 node ratio 触发 performance regress；本轮 regress 来自 `20260423` / `20260424` opening 的 `search_seconds` ratio，opening node ratio 本身没有超过 `1.25`。
- 5 次 opening 重跑：nodes 完全稳定；`20260423` search ratio 平均 `1.443641`、4/5 次超过 `1.25`；`20260424` 平均 `1.246232`、2/5 次超过 `1.25`；`20260425` 平均 `1.011826`、0/5 次超过 `1.25`。
- checkpoint 趋势不算更稳定：final strength 信号保持，但中后段仍有 checkpoint-50 `14/15/1`、checkpoint-62 `13/15/2` 这种 aggregate strength 退步，overall 不能放行。
- 决策：不进入 Phase 4；不回滚到“纯训练稳定性先不谈 policy”，因为 strength 没退；下一步建议做 report-only rating split，保持现有 `overall_rating` 保守判定。

## 测试结果（2026-05-04 decision run）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| selection 64 run | `benchmark-select.ps1 -GamesPerSeed 64 -Alpha 0.005 -Lambda 0.5 -Epsilon 0.02 -TerminalReward 500 -TdErrorClip 250` | 产出 policy evidence | 成功生成 `20260504-123006`，overall `regress` | 通过 |
| final summary parse | `selection-summary.json` | 汇总 requested metrics | C/B/D=`16/12/2`，score regressions=`0`，opening node avg/max=`1.122056/1.245924` | 通过 |
| opening repeat eval | final candidate/bootstrap, 3 seeds x 5 | 区分 node 与 time | nodes 稳定，time ratio 波动；`20260425` 不再触发 regress | 通过 |

## 会话：2026-05-04（Phase 3 report-only rating split）

### 阶段 3：只拆诊断，不放宽 gate
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 在 `benchmark-select.ps1` 的 selection summary 中新增 `strength_rating`、`perf_rating`、`blocker_reason`。
  - 将 split 作为 report-only 诊断输出，`overall_rating` 仍按原有 `pass` / `watch` / `regress` 汇总规则生成。
  - strength 侧只记录胜负、fixed score regression、best move drift watch；perf 侧只记录现有 search/nodes performance ratio，并透出可选 `qnodes_ratio` / aspiration retry 指标。
  - 没有修改搜索、训练、特征或 selection 阈值。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 结论
- `strength_rating=pass` 不能覆盖 `perf_rating=regress`；性能 regress 仍使 existing `overall_rating=regress`。
- `strength_rating=watch` 且 `perf_rating=pass` 时，existing `overall_rating` 仍是 `watch`。
- fixed score regression 会产生 `strength_rating=regress`；performance ratio 超过既有阈值会产生 `perf_rating=regress`。
- Phase 4 继续 blocked；别一看强度没崩就去调 PVS/qsearch/特征，锅还热着呢。

## 测试结果（2026-05-04 report-only rating split）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester RED | 新增 split 断言后、实现前 | 新字段缺失导致 benchmark-select 测试失败 | `21 total / 16 passed / 5 failed`，失败集中在新增字段 | 通过 |
| Pester GREEN | `Invoke-Pester -Script 'scripts\\workflow.tests.ps1' -PassThru -Quiet` | 新旧 workflow 测试全通过 | `21 passed / 0 failed` | 通过 |

## 会话：2026-05-04（Phase 3 report-only performance instability attribution）

### 阶段 3：继续停在 trend validation failed，Phase 4 blocked
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 在 `benchmark-select.ps1` 中为 perf regress 增加 machine-readable `blocker_details`。
  - blocker detail 按 seed / category / metric 记录 baseline/candidate value、ratio、threshold、trigger 和 repeatability。
  - `selection-summary.txt` 增加 `Perf blocker details` 简短表格。
  - 保留 `blocker_reason`，但不再只靠字符串解释 perf blocker。
  - 保持 `overall_rating` 旧规则不变；没有新增 `perf_watch`，没有让 `strength_rating` 覆盖 `perf_rating`。
  - 没有修改搜索生产逻辑、训练算法、特征或搜索参数。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 结果
- `selection-summary.json` 顶层、per-seed 层和 seed scorecard 层都能输出 `blocker_details`。
- 单 seed 多 metric 超阈时，`search_seconds_ratio`、`node_ratio` 和同 position 可用的 `qnodes_ratio` 都能记录。
- `search_seconds_ratio` 单独超阈、`node_ratio` 单独超阈都能进入 perf blocker。
- Phase 3 仍未通过趋势验证；Phase 4 继续 blocked，别一看解释更清楚就想偷摸放行，这锅盖还烫手。

## 测试结果（2026-05-04 blocker_details attribution）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester RED | 新增 blocker_details 断言后、实现前 | 缺失结构化明细导致失败 | `23 total / 20 passed / 3 failed`，失败集中在 `blocker_details` 缺失 | 通过 |
| Pester GREEN | `Invoke-Pester -Script 'scripts\\workflow.tests.ps1' -PassThru -Quiet` | workflow 测试全通过 | `23 passed / 0 failed` | 通过 |

## 会话：2026-05-04（Phase 3 blocker evidence pack）

### 阶段 3：文档与证据整理，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 接受 quick smoke 结论：`GamesPerSeed=2` 产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-162204` 未触发 final perf regress，顶层/per-seed final `blocker_details` 为空，smoke validation failed / inconclusive。
  - 接受 targeted perf-regress artifact validation 结论：`GamesPerSeed=32` 产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241` 触发 `perf_rating=regress`，并验证顶层/per-seed/final scorecard `blocker_details` 均非空。
  - 将 32 局、64 局、quick smoke、targeted perf-regress validation 的关键结论整理为 Phase 3 blocker evidence pack。
  - 明确 report-only rating split completed；report-only `blocker_details` completed and targeted artifact-validated；但当前 blocker 仍是 opening performance instability / `perf_rating=regress`。
  - 明确禁止事项：不进入 Phase 4，不扩 N-Tuple / threat / mobility / stage 特征，不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move，不放宽 selection，不让 `strength_rating` 覆盖 `perf_rating`。
  - 本轮没有修改生产代码，没有新增逻辑字段。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### Targeted artifact validation 证据
- 产物目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`。
- `overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`。
- 顶层 `blocker_details` 数量：`3`；per-seed `blocker_details` 总数：`3`；final scorecard `blocker_details` 总数：`3`。
- `selection-summary.txt` 的 `Perf blocker details` 表格非空。
- Pester 复跑结果：`23 passed / 0 failed`。
- 示例：
  - `20260423 opening search_seconds_ratio = 1.469059 > 1.25`
  - `20260425 opening search_seconds_ratio = 2.159236 > 1.25`
  - `20260425 opening node_ratio = 1.295516 > 1.25`

## 测试结果（2026-05-04 evidence pack）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| docs-only guard | `findings.md` / `progress.md` / `task_plan.md` | 只整理文档，不改生产代码 | 仅修改 3 个规划文档 | 通过 |
| targeted artifact evidence | `20260504-163241/selection-summary.json` 和 `.txt` | 记录 top/per-seed/final blocker_details 落盘证据 | 已写入 evidence pack | 通过 |

## 会话：2026-05-04（Phase 3 repeatability evidence pass）

### 阶段 3：report-only repeatability，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 基于 targeted artifact validation 产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`，对触发 blocker 的 seed / opening position 做 repeatability 复核。
  - 使用 `bitboard-eval --candidate ... --baseline ... --depth 4 --format json` 对 seed `20260423` / `20260425` 各复跑 opening 10 次，记录 baseline/candidate nodes 与 search_seconds；该 eval 输出不包含 qnodes。
  - 使用 `bitboard-search --weights ... --depth 4 --threads 1 --format json --case-id opening` 对相同 bootstrap/candidate 权重各复跑 10 次，补充 qnodes 与 root 搜索指标。
  - 输出 evidence 文件到 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504`。
  - 只更新 `findings.md` / `progress.md`，没有把 repeatability 自动接入 `selection-summary.json`，没有修改 selection gate。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
- **证据产物：**
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504\\opening-repeatability-runs.csv`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504\\opening-repeatability-runs.json`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504\\opening-bitboard-search-repeatability-runs.csv`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504\\opening-bitboard-search-repeatability-runs.json`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504\\opening-bitboard-search-repeatability-summary.json`

### Repeatability 结论
- `20260423 opening search_seconds_ratio=1.469059 > 1.25`：10 次 `bitboard-search` 复跑 nodes 固定为 `1653/1472=1.122962`，qnodes 固定为 `1138/1010=1.126733`，均未越过 `1.25`；time ratio 范围 `0.845649-1.439340`，10 次中 `2` 次越过 `1.25`。结论是 timing-sensitive perf instability，不是稳定越阈 tree inflation。
- `20260425 opening search_seconds_ratio=2.159236 > 1.25`：10 次复跑 nodes 固定为 `1907/1472=1.295516`，qnodes 固定为 `1273/1010=1.260396`；time ratio 范围 `0.900220-1.715686`，10 次中 `7` 次越过 `1.25`。结论是 search_seconds 有 timing noise，但有 stable tree/qtree inflation 支撑。
- `20260425 opening node_ratio=1.295516 > 1.25`：10 次复跑 nodes 与 qnodes 都稳定越过 `1.25`。结论是 stable search-tree inflation。
- Phase 3 仍保持 `implementation complete, trend validation failed`；Phase 4 继续 blocked。
- 本轮没有改搜索生产逻辑、训练算法、特征、selection gate，没有新增 `perf_watch`，没有让 `strength_rating` 覆盖 `perf_rating`。

## 测试结果（2026-05-04 repeatability evidence）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| `bitboard-eval` repeatability | seed `20260423/20260425` opening, 10 次 | 复核 selection-level nodes/search_seconds | nodes 固定，search_seconds 波动；eval 不输出 qnodes | 通过 |
| `bitboard-search` repeatability | 同一 bootstrap/candidate 权重, depth 4, threads 1, 10 次 | 补充 qnodes 与 root 搜索指标 | `20260423` qnodes ratio `1.126733` 未越阈；`20260425` qnodes ratio `1.260396` 稳定越阈 | 通过 |
| gate guard | docs-only repeatability pass | selection gate 不变 | 未修改生产脚本或 C++ 逻辑 | 通过 |

## 会话：2026-05-04（Phase 3 repeatability artifact integration）

### 阶段 3：report-only artifact integration，Phase 4 继续 blocked
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 将文档层 repeatability 证据接入 `selection-summary.json` 的 perf `blocker_details`。
  - 对匹配到的 blocker 输出 `repeatability` / `repeatability_status` / `repeatability_runs` / `repeatability_exceeded_count` / `repeatability_notes`。
  - 只匹配已有证据：`20260423 opening search_seconds_ratio`、`20260425 opening search_seconds_ratio`、`20260425 opening node_ratio`。
  - 未匹配 blocker 保持 `repeatability=unknown`，不编造 repeatability。
  - `selection-summary.txt` 的 `Perf blocker details` 表格新增 `repeatability` 列。
  - 保持 `overall_rating` / `perf_rating` / `strength_rating` 判级规则不变；没有新增 `perf_watch`，没有放宽 selection。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\benchmark-select.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 结果
- `20260423 opening search_seconds_ratio` 标记为 `timing_sensitive`，但仍按旧 perf gate 保持 regress，不自动降为 watch。
- `20260425 opening search_seconds_ratio` 标记为 `stable_tree_inflation_with_timing_noise`。
- `20260425 opening node_ratio` 标记为 `stable_tree_inflation`，只作为诊断输出，不新增额外 gate。
- unknown repeatability 仍不影响旧 overall 判级。
- Phase 3 继续停在 `trend validation failed`；Phase 4 继续 blocked。

## 测试结果（2026-05-04 repeatability artifact integration）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester RED | 新增 repeatability artifact integration 断言后、实现前 | 缺失字段导致失败 | 失败集中在 `repeatability_status` / repeatability evidence 断言 | 通过 |
| Pester GREEN | `Invoke-Pester -Script 'scripts\\workflow.tests.ps1' -PassThru -Quiet` | workflow 测试全通过 | `24 passed / 0 failed` | 通过 |
| gate guard | repeatability diagnostics | 不改变 `overall_rating` / `perf_rating` / `strength_rating` | Pester 覆盖 timing-sensitive、stable tree、unknown 和旧 overall 兼容 | 通过 |

## 会话：2026-05-04（targeted repeatability artifact validation follow-up）

### 阶段 3：真实产物验证部分通过，不进入 Phase 4
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 接受本轮 targeted repeatability artifact validation 结论：不能标 artifact-validated。
  - 仅更新 `findings.md` / `progress.md` / `task_plan.md` 状态，不再反复刷真实训练 run。
  - 没有修改搜索、训练、特征、搜索参数或 selection gate。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 真实产物证据
- 产物目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525`。
- 评级：`overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`。
- 顶层 `blocker_details` 实际只有 `2` 条：
  - `20260425 opening search_seconds_ratio=1.413497`，`repeatability=unknown`。
  - `20260425 opening node_ratio=1.295516`，`repeatability=stable_tree_inflation`。

### 验收结论
- Repeatability artifact integration remains implemented and unit-tested。
- Real artifact validation is partial / not accepted。
- `20260425 opening node_ratio=1.295516` 的 `stable_tree_inflation` artifact path validated。
- `20260425 opening search_seconds_ratio` 已从 known evidence ratio `2.159236` 变为 `1.413497`，因此正确保持 `repeatability=unknown`。
- `20260423` timing blocker 未在本轮真实产物中出现。
- Full three-blocker real artifact validation not achieved。
- 这不是放行信号：Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 继续 blocked；不新增 `perf_watch`；不让 `strength_rating` 覆盖 `perf_rating`；`repeatability_status` 不参与 gate。
- Pester 结果：`24 passed / 0 failed`；预期异常文本来自负向测试，不影响结果。
- `search_seconds_ratio` 有 timing noise，真实 run 不适合作为精确 ratio 命中的唯一验收方式；不要继续通过反复跑真实训练来凑三条 blocker。

## 会话：2026-05-04（deterministic repeatability artifact fixture validation）

### 阶段 3：确定性 fixture 验证，Phase 4 继续 blocked
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 在 `workflow.tests.ps1` 中新增 deterministic fixture validation 用例。
  - fixture 使用 fake selection harness 生成临时 `selection-summary.json` / `selection-summary.txt`，不跑真实训练 run。
  - 没有修改搜索生产逻辑、训练算法、特征、搜索参数或 selection gate。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\scripts\\workflow.tests.ps1`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### Fixture 覆盖
- `20260423 opening search_seconds_ratio=1.469059` => `timing_sensitive`。
- `20260425 opening search_seconds_ratio=2.159236` => `stable_tree_inflation_with_timing_noise`。
- `20260425 opening node_ratio=1.295516` => `stable_tree_inflation`。
- `20260430 opening search_seconds_ratio=1.400000` => `unknown`。
- JSON 字段覆盖：`repeatability`、`repeatability_status`、`repeatability_runs`、`repeatability_exceeded_count`、`repeatability_notes`。
- TXT 覆盖：`Perf blocker details` 表格包含 `repeatability` 列，三条 known fixture 状态正确，未匹配 blocker 输出 `unknown`。

### Gate guard
- `overall_rating=regress` 仍按旧规则。
- `strength_rating=pass` 不覆盖 `perf_rating=regress`。
- `timing_sensitive` 不自动降为 watch。
- `stable_tree_inflation` 不新增额外 gate。
- `unknown` 不影响判级。
- 不新增 `perf_watch`，`repeatability_status` 不参与 gate。

## 测试结果（2026-05-04 deterministic fixture validation）
| 检查 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| Pester fixture wiring | 新增用例初跑 | fixture 必须覆盖 4 seed | `-Seeds` 数组经 `pwsh -File` 被当单字符串/单值，修为 `pwsh -Command` 后覆盖 4 seed | 通过 |
| Pester fixture GREEN | fake selection harness deterministic fixture | JSON/TXT repeatability mapping 和 gate guard 均稳定 | `25 passed / 0 failed` | 通过 |
| scope guard | fixture-only validation | 不跑真实训练、不改 gate、不放宽 selection | 仅修改测试与文档；预期异常文本来自负向测试，不影响结果 | 通过 |

### 结论
- Deterministic artifact fixture validation 已完成并接受：它验证 artifact formatting / repeatability mapping。
- 这不是 Phase 3 趋势放行信号；Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 继续 blocked。

## 会话：2026-05-04（Phase 3 freeze / handoff summary）

### 阶段 3：冻结交接，Phase 4 继续 blocked
- **状态：** implementation complete, trend validation failed
- **执行的操作：**
  - 接受 deterministic artifact fixture validation 结果：Pester `25 passed / 0 failed`。
  - 只更新 `findings.md` / `progress.md` / `task_plan.md` 的最终交接状态。
  - 明确 report-only rating split completed。
  - 明确 report-only `blocker_details` completed and targeted artifact-validated。
  - 明确 repeatability artifact integration deterministic fixture-validated，但 real artifact validation 仍是 partial / not accepted。
  - 明确当前 blocker 仍是 opening performance instability / `perf_rating=regress`，Phase 4 继续 blocked。
  - 没有跑新训练，没有修改生产代码，没有改搜索、训练、特征、参数或 selection gate。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`

### 交接证据链
- 32 局最好组合仍 `regress`。
- 64 局 decision run 仍 `regress`。
- `blocker_details` 真实 perf-regress artifact 验证通过。
- repeatability evidence 已区分 `timing_sensitive` 和 stable tree inflation。
- repeatability real artifact validation 只部分通过，未接受为完整 artifact validation。
- deterministic fixture validation 通过：Pester `25 passed / 0 failed`。

### Blocked 解除条件
- `GamesPerSeed=32` 至少达到 `watch`。
- `GamesPerSeed=64` 不 `regress`。
- 否则不进入 Phase 4。

### 禁止事项
- 不进入 Phase 4。
- 不扩 N-Tuple / threat / mobility / stage 特征。
- 不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move。
- 不改训练、搜索、特征、参数或 selection gate。
- 不放宽 selection，不新增 `perf_watch`。
- 不让 `strength_rating` 覆盖 `perf_rating`。
- 不让 `repeatability_status` 参与 gate。

## 会话：2026-05-05（P0 Surakarta capture-rule correctness incident）

### 阶段 3B：P0 棋规正确性事故
- **状态：** in_progress
- **开始时间：** 2026-05-05
- **执行的操作：**
  - 接收用户人工验收反馈：疑似 Surakarta 吃子规则回归，优先级从 Phase 3 训练/selection 推进切换为 P0 rule correctness incident。
  - 明确 Phase 3 remains implementation complete, trend validation failed；Phase 4 blocked。
  - 明确疑似回归之后的 training / selection artifacts 全部 provisional / suspect，直到 rule correctness 恢复。
  - 更新 `task_plan.md`、`findings.md`、`progress.md`，建立事故记录与边界。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 当前约束
- 不进入 Phase 4。
- 不扩特征，不调搜索参数，不放宽 selection。
- 不继续跑训练 sweep。
- 不把现有 Phase 3 结果当作可信通过依据。
- 用户补充大赛规则后，审计口径更新：
  - 吃子必须经过至少一个完整弧线，路径中不可以有本方棋子阻挡。
  - 反向同路必须对称。
  - 终局为一方棋子被吃光，或双方都不能再吃子时按剩余棋子数判胜。
  - 现有 `max_no_capture_round` / `no_capture_ply` 只能作为代码状态字段测试，不能替代大赛终局规则。

## 测试结果（2026-05-05 P0 incident）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| 事故文档封存 | `task_plan.md` / `findings.md` / `progress.md` | 记录 Phase 3/4 状态、新 blocker、suspect artifact 边界 | 已记录 | 通过 |
| 最小吃子复现 | Black: `(2,1)`, `(2,2)`, `(2,3)`；White: `(3,3)`；Black to move | 固化 board / side / expected / actual / parity | 修复前 traditional `LEGAL_CAPTURE_MOVE`，bitboard `ILLIGAL_CAPTURE_MOVE` / capture-only missing | 通过 |
| 大赛规则口径记录 | 用户补充规则 | 更新审计口径，避免误用训练终局 | 已记录 | 通过 |

## 会话：2026-05-05（P0 capture-rule fix / verification）

### 阶段 3B：P0 棋规正确性事故收敛
- **状态：** capture regression fixed and verified；Phase 4 继续 blocked
- **执行的操作：**
  - 恢复 `task_plan.md`、`findings.md`、`progress.md`，并运行 planning session catch-up。
  - 复核当前 diff，确认 P0 修复范围集中在 `include/surakarta_bitboard.h`、`include/surakarta_masks.h`、`src/surakarta_bitboard_selftest.cpp` 以及事故文档。
  - 用 traditional movegen 作为本次 golden oracle，固化用户人工发现的最小漏吃局面。
  - 审计 bitboard capture tables、`BitboardGenerator::Build()`、`IsCaptureLegal()`、`GenerateMoves()`、qsearch capture-only source、`MakeMove()` / `UnmakeMove()`、`no_capture_ply` reset、terminal reason 相关路径。
  - 最小修复：`kMaxCaptureVariants` 从 `4` 改为 `8`，重新生成静态 masks。
  - 未改 PVS、qsearch 参数、aspiration、move ordering、LMR、Null Move、特征、训练参数或 selection gate。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_bitboard.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_masks.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard_selftest.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### 失败局面
- Board：
  - Black: `(2,1)`, `(2,2)`, `(2,3)`
  - White: `(3,3)`
  - Empty: all other squares
- Side to move：Black。
- Expected legal capture：`(2,2) -> (3,3)`。
- Actual before fix：traditional `LEGAL_CAPTURE_MOVE`；bitboard `ILLIGAL_CAPTURE_MOVE`，capture-only movegen missing。
- Disagreement：traditional 与 bitboard 不一致。

### 根因与修复
- 根因：`include/surakarta_bitboard.h::kMaxCaptureVariants` 旧值 `4` 导致 `src/surakarta_bitboard.cpp::BitboardGenerator::Build()` 静默截断 capture path variants。golden `(2,2)->(3,3)` 实际有 `8` 条 variants；前 4 条被己方棋子阻挡后，bitboard 没有机会检查后续合法路径。
- 修复：`kMaxCaptureVariants=8`，重新生成 `include/surakarta_masks.h`，并新增 capture-rule selftest。
- 相关定位：`git blame` 显示旧常量来自 `49dc0aa chore: capture trusted baseline snapshot`；近期 Phase 3 qsearch/selection/report-only 改动不是直接根因。

### 测试结果（2026-05-05 P0 fix）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| clean rebuild | VS DevCmd + CMake/Ninja clean + build `surakarta-bitboard-selftest surakarta-benchmark` | 避免 Windows + Ninja 增量 ABI/旧 DLL 问题 | exit `0`；clean 删除 22 个旧产物，重新构建通过 | 通过 |
| bitboard selftest | `build-p0-rule\\bin\\surakarta-bitboard-selftest.exe` | 新增 capture-rule tests 和既有 selftest 全过 | exit `0`，`[PASS] surakarta-bitboard-selftest` | 通过 |
| workflow Pester | `Invoke-Pester -Script scripts\\workflow.tests.ps1 -PassThru -Quiet` | workflow 测试全过 | `25 passed / 0 failed`；负向测试异常为预期输出 | 通过 |
| CTest 初跑 | `ctest --test-dir build-p0-rule --output-on-failure` | CTest 全过或暴露缺口 | `surakarta-bitboard-selftest` 通过；`surakarta-dev-session-selftest.exe` 未构建导致 Not Run | 已诊断 |
| CTest 复跑 | 先 build `surakarta-dev-session-selftest`，再 `ctest --test-dir build-p0-rule --output-on-failure` | C++ test registry 全过 | `2/2 passed` | 通过 |

### 剩余边界
- Phase 3 remains implementation complete, trend validation failed。
- Phase 4 blocked。
- 疑似规则回归之后的 training / selection / repeatability / artifact-validation 产物全部保持 suspect/provisional。
- terminal audit 发现 legacy `max_no_capture_round` / `no_capture_ply` 与用户给出的大赛第 6 条不是同一件事；本轮只修复人工发现的 capture generation 回归，没有把该 legacy 终局语义拿来当大赛规则放行依据。后续若要严格落地“双方都不能再吃子”，需要单独改终局语义并补 golden cases。

## 会话：2026-05-05（post-fix rule correctness hardening）

### 阶段 3B：规则正确性加固，Phase 4 继续 blocked
- **状态：** verified；Phase 4 继续 blocked
- **执行的操作：**
  - 新增 `surakarta-mask-capacity-failfast` CTest，先确认旧逻辑会因静默截断而让预期失败测试变红。
  - 将 `BitboardGenerator::Build()` capture variant 写入改为容量不足时 fail fast，不允许继续静默截断。
  - 为 mask exporter 增加异常捕获，容量不足时返回非零并输出诊断，不让 Windows 未捕获异常把 CTest 搞成崩溃式失败。
  - 新增 `TestCaptureVariantCapacityInvariant`，扫描动态生成表和静态预计算表，证明当前所有 capture variant count 都 `<= kMaxCaptureVariants`，当前最大值为 `8`。
  - 新增 `TestRuleCorrectnessGoldenParity`，扩大 traditional vs bitboard parity 覆盖：中心、边、角、内环、外环、多路径、己方阻挡、敌方目标、无目标、普通移动。
  - 保留用户 P0 局面：Black to move；黑子 `(2,1)`, `(2,2)`, `(2,3)`；白子 `(3,3)`；`(2,2)->(3,3)` 必须为合法 capture。
  - 没有跑训练 sweep，没有扩特征，没有调搜索，没有放宽 selection。
- **创建/修改的文件：**
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_bitboard.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard_selftest.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_mask_export.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\CMakeLists.txt`
  - `D:\\Dev\\Code\\surakarta_traditional\\task_plan.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\findings.md`
  - `D:\\Dev\\Code\\surakarta_traditional\\progress.md`

### Rule correctness gate
- Rule correctness selftest failed => 禁止 training。
- Rule correctness selftest failed => 禁止 selection。
- Rule correctness selftest failed => 禁止 release。
- Rule correctness selftest failed => 禁止 Phase 4。

### Historical artifact isolation
- 以下产物继续保持 `suspect` / `provisional`：
  - `20260503-234333`
  - `20260504-123006`
  - `20260504-162204`
  - `20260504-163241`
  - `20260504-205525`
  - `phase3-repeatability-evidence-20260504`

### Separate rule-semantics debt
- legacy `max_no_capture_round` / `no_capture_ply` 不是大赛第 6 条“双方都不能再吃子”的严格实现。
- 本轮不偷改终局语义。
- 后续若要严格对齐大赛规则，需要单独设计 golden cases 和 terminal semantics 修改。

### 已完成的中间验证
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| capacity CTest RED | `surakarta-mask-capacity-failfast`，旧静默截断实现 | 预期失败测试应红 | 程序成功退出导致 CTest failed | 通过 |
| capacity CTest GREEN | `surakarta-mask-capacity-failfast`，fail-fast 实现 | 容量不足时非零退出，CTest `WILL_FAIL` 通过 | `1/1 tests passed` | 通过 |
| bitboard selftest | `build-p0-rule\\bin\\surakarta-bitboard-selftest.exe` | 新增 parity/capacity tests 全过 | `[PASS] surakarta-bitboard-selftest` | 通过 |

### 最终验证（2026-05-05 post-fix hardening）
| 测试 | 输入 | 预期结果 | 实际结果 | 状态 |
|------|------|---------|---------|------|
| clean rebuild | `Invoke-SurakartaBuild -BuildDir build-p0-rule -CleanFirst -Targets surakarta-bitboard-selftest,surakarta-benchmark,surakarta-dev-session-selftest,surakarta-mask-capacity-failfast` | 干净重建所有规则门禁目标 | exit `0`；clean 删除 27 个旧产物并完成 26 个构建步骤；仅有既有 MSVC warning | 通过 |
| bitboard selftest | `build-p0-rule\\bin\\surakarta-bitboard-selftest.exe` | 新增 rule correctness / capacity invariant 与既有 selftest 全过 | exit `0`；`[PASS] surakarta-bitboard-selftest`；执行 `TestRuleCorrectnessGoldenParity`、P0 golden 和 `TestCaptureVariantCapacityInvariant` | 通过 |
| CTest | `ctest --test-dir build-p0-rule --output-on-failure` | 注册 C++ tests 全过，容量 fail-fast 按 `WILL_FAIL` 生效 | `3/3 passed`：`surakarta-bitboard-selftest`、`surakarta-dev-session-selftest`、`surakarta-mask-capacity-failfast` | 通过 |
| Pester | `Invoke-Pester -Script scripts\\workflow.tests.ps1 -PassThru -Quiet` | workflow tests 全过 | `TotalCount=25`，`PassedCount=25`，`FailedCount=0`；红色异常为负向测试预期输出 | 通过 |
| diff whitespace | `git diff --check` | 无 whitespace error | exit `0`；仅 CRLF/LF 换行提示 | 通过 |

### 最终结论
- 当前不存在任何 capture variant count > `8`；动态生成表与静态预计算表的最大值均为 `8`。
- `BitboardGenerator::Build()` 已禁止静默截断，容量不足时 fail fast。
- Phase 3 仍是 `implementation complete, trend validation failed`。
- Phase 4 仍 blocked。

## 会话：2026-05-22（阶段 B1：大赛终局语义与规则完整性）

### 状态
- **阶段 B1：** completed。
- **是否可进入 B2：** 可以进入阶段 B2（可信基线重建）。
- **仍不允许：** 不跑训练 sweep，不扩特征，不调 PVS / qsearch / LMR / Null Move / selection gate。

### 执行的操作
- 读取并审计用户指定的任务文档、README、答辩文档、rule manager、bitboard、search、selftest、utils 相关源码。
- 先补 RED tests，确认旧实现会把 legacy no-capture counter 当成 national terminal，且 dev-session status 语义不符合 B1 要求。
- 最小设计落地：
  - traditional 用 `SurakartaPieceCanCaptureUtil::CanCaptureOpponentPiece(PieceColor)` 判双方 capture availability。
  - bitboard 新增 `HasAnyCaptureMove(position, color)` 和 `IsNationalStalemateTerminal(position)`。
  - search 复用 bitboard national stalemate helper，不再用 `no_capture_ply >= max_no_capture_round` 判大赛终局。
  - 平子 `STALEMATE` winner 使用 `SurakartaPlayer::NONE`。
  - legacy no-capture 字段保留为状态字段，但不再作为 national terminal gate。
- 修复 `SurakartaGame::Move` / `SurakartaApplyMoveWithGameInfoUtil::ApplyMove` 的判终局顺序：合法落子后再 `JudgeEnd`，保证终局判断看走后棋盘。
- 发现并修正初始局面误判终局：直接用“双方当前没有 capture move”会把开局判死；共享 helper 增加“材料已低于初始满子状态”的棋盘状态保护。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\include\surakarta_bitboard.h`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_rule_manager_impl.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_rule_manager_bitboard.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_search.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_game.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_utils.cpp`
- `D:\Dev\Code\surakarta_traditional\src\main.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_dev_session_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

### 新增/修改测试
- `TestEvaluateBitboardTerminalParity`：
  - 一方 0 子 => `CHECKMATE`。
  - 双方都不能 capture 且黑多 / 白多 / 平子 => `STALEMATE` + 正确 winner。
  - legacy counter 越界但仍有 capture available => 非 terminal。
  - traditional / bitboard rule manager / `EvaluateBitboardTerminal` parity。
- `TestSearchNationalTerminalSemantics`：
  - national terminal 根节点不返回 best move，score 按材料。
  - legacy counter alone 不阻止搜索。
- `TestTerminalStatusUsesNationalNoCaptureSemantics`：
  - dev-session 能显示 national stalemate terminal。
  - legacy counter alone 不会让可吃子局面变 terminal。

### legacy 行为处理
- 已替换：
  - `SurakartaRuleManagerImpl::JudgeEnd` 的 `num_round_ - last_captured_round_ >= max_no_capture_round_` terminal gate。
  - `EvaluateBitboardTerminal` 的 `position.no_capture_ply > position.max_no_capture_round` terminal gate。
  - search `EvaluateTerminal` 的 `position.no_capture_ply >= position.max_no_capture_round` terminal gate。
- 已保留：
  - `no_capture_ply` / `max_no_capture_round` 字段、序列化和 make/unmake 更新。
  - 训练 summary 的历史字段名未重命名，避免扩大 manifest / workflow 兼容面；其 national terminal 来源已改为双方不能 capture。
- `game6.txt`：
  - 保留为历史 manual acceptance、perf endgame、pre/post capture checkmate fixture。
  - 不再作为大赛第 6 条“双方都不能再吃子”的严格证明。

### 验证结果
| 测试 | 命令 | 结果 |
|------|------|------|
| build | `cmake --build build-b1-core --target surakarta-bitboard-selftest surakarta-dev-session-selftest surakarta-benchmark` | exit `0` |
| bitboard selftest | `build-b1-core\bin\surakarta-bitboard-selftest.exe` | exit `0`，`[PASS] surakarta-bitboard-selftest` |
| dev-session selftest | `build-b1-core\bin\surakarta-dev-session-selftest.exe` | exit `0`，`[PASS] surakarta-dev-session-selftest` |
| Pester workflow | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet` | `26 passed / 0 failed`；负向测试异常为预期输出 |
| CTest 初跑 | `ctest --test-dir build-b1-core --output-on-failure` | 2 个 selftest passed；`surakarta-mask-capacity-failfast.exe` 未构建导致 Not Run |
| CTest 复跑 | 先 build `surakarta-mask-capacity-failfast`，再 `ctest --test-dir build-b1-core --output-on-failure` | `3/3 passed` |

### 结论
- 大赛第 6 条 national terminal 语义已落地到 traditional、bitboard、search、CLI trace、dev-session status。
- 阶段 B1 可以封口。
- 下一阶段是 B2 可信基线重建；Phase 3 / Phase 4 状态不因 B1 自动放行，别一高兴就开调参，那叫刚修好刹车就上高速飙车。

## 会话：2026-05-22（阶段 B2：可信基线重建）

### 状态
- **阶段 B2：** completed for acceptance。
- **perf baseline：** gate failed but classified；未更新 `test/test_data/perf-baseline.json`。
- **是否可进入下一阶段：** 可以进入 Phase 3 post-B1 fresh trend validation 准备。
- **仍不允许：** 不跑训练 sweep，不扩特征，不调搜索参数，不放宽 selection gate，不把 Phase 3 标通过，不启动 Phase 4。

### 执行的操作
- 读取用户指定的 `task_plan.md`、`findings.md`、`progress.md`、`docs/national_competition_technical_roadmap.md`、`README.md`、`scripts/workflow.ps1`、`scripts/perf-runner.ps1`、`scripts/benchmark-train.ps1`、`scripts/benchmark-select.ps1`、`scripts/common.ps1`、`test/test_data/perf-baseline.json`。
- 运行 `git status --short` 和 `git rev-parse HEAD`，确认当前改动来自 B1 代码与记忆文件，`docs/national_competition_technical_roadmap.md` 仍为 untracked；未提交、未回滚。
- 尝试 Debug / Release 默认 workflow；两者均因 GUI FetchContent 下载 `glfw` 网络失败停在 configure 阶段。
- 按降级路径使用 `build-b1-core` 复核 B1 后 core build、自测和 CTest。
- 运行 Pester workflow 复核。
- 运行 `perf-runner.ps1`，读取并分类 `perf-gate.json` / `perf-summary.txt`。
- 运行 `benchmark-train.ps1` 并检查 manifest / hash / seed / 参数。
- 运行 `benchmark-select.ps1 -GamesPerSeed 2` 作为 selection 入口 smoke；明确不作为 Phase 3 通过证据。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

### 验证结果
| 检查 | 命令 | 结果 | 状态 |
|------|------|------|------|
| 工作区 | `git status --short` | B1 源码与记忆文件 modified；`docs/national_competition_technical_roadmap.md` untracked | 已记录 |
| commit | `git rev-parse HEAD` | `6063d9acbb24dc9397e07e99bffa4e94b8af55f6` | 已记录 |
| Debug workflow | `pwsh ./scripts/workflow.ps1 -Configuration Debug` | CMake configure failed；GitHub `glfw 3.4.zip` download connection reset | 环境性失败 |
| Release workflow | `pwsh ./scripts/workflow.ps1 -Configuration Release` | CMake configure failed；GitHub `glfw 3.4.zip` download timeout | 环境性失败 |
| fallback build | `Invoke-SurakartaBuild -BuildDir build-b1-core -Targets ...` | `ninja: no work to do` | 通过 |
| bitboard selftest | `build-b1-core\bin\surakarta-bitboard-selftest.exe` | `[PASS] surakarta-bitboard-selftest` | 通过 |
| dev-session selftest | `build-b1-core\bin\surakarta-dev-session-selftest.exe` | `[PASS] surakarta-dev-session-selftest` | 通过 |
| CTest | `Invoke-SurakartaCTest -BuildDir build-b1-core` | `3/3 passed` | 通过 |
| Pester | `Invoke-Pester -Script 'scripts\workflow.tests.ps1' -PassThru -Quiet` | `TotalCount=26`，`PassedCount=26`，`FailedCount=0` | 通过 |
| perf gate | `pwsh ./scripts/perf-runner.ps1` | failed，`15` regressions；artifact `build-local\perf\20260522-145419` | 已分类 |
| training smoke | `pwsh ./scripts/benchmark-train.ps1` | `[PASS] benchmark training smoke gate`；artifact `build-local\training\20260522-145835` | 通过 |
| selection smoke | `pwsh ./scripts/benchmark-select.ps1 -GamesPerSeed 2` | `[PASS] benchmark selection scorecard`；artifact `build-local\training\20260522-150225` | 入口通过 |

### perf gate 分类
- artifact：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`。
- `perf-gate.json`：`status=fail`，`failure_count=15`。
- failure split：`performance_regression=14`，`score_drift=1`，`best_move_drift=0`。
- `middlegame`：全部通过。
- `endgame`：9 项 tiny-position `node_ratio_tiny_position` fail；best move 和 score 全部不变，节点增加来自 B1 后 `game6.txt` 不再按 legacy no-capture counter early terminal。
- `opening`：best move 不漂移；depth 7 1T score `12 -> 0`，depth 7/8 有搜索树增长，符合 B1 terminal semantics 变化后旧 baseline 不再可信。
- 处理建议：不要直接更新 baseline；先由用户确认，再生成 post-B1 perf baseline。这个失败不是“赶紧调参”的信号，别把体温计摔了说病好了。

### training smoke 产物
- session root：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-145835`。
- `repro-summary.json` / `repro-summary.txt` 存在，`reproducible=True`。
- `weights-manifest.json`：`purpose=training`，`git_commit=6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，artifact count `8`。
- 参数：seed `20260423`，games `4`，depth `4`，checkpoint_every `2`，`alpha=0.005`，`lambda=0.5`，`epsilon=0.02`，terminal_reward `1200`。
- bootstrap hash：`D87852D8E639182E479F4756FCFE63A1FDA992C01C163E6FC9D5C2D228F3AC51`。
- candidate hash：`05A30986FD8A2BD85951B274D193673373F008DAF7D82F777CD566E88F99D61C`。
- candidate 与 bootstrap 不同；run-1/run-2 hash 可复现；checkpoint manifests 存在。

### selection smoke 产物
- session root：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-150225`。
- `selection-summary.json` / `selection-summary.txt` 存在。
- `weights-manifest.json`：`git_commit=6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，artifact count `9`。
- summary：`overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`，C/B/D=`15/12/3`，worst performance ratio `1.343612`。
- 结论：这只是 `GamesPerSeed=2` 入口 smoke，不能写成 Phase 3 趋势通过。

### B2 结论
- B2 可以封口。
- 默认 workflow 的 GUI FetchContent 环境失败已记录，core 等价验证通过。
- Pester 通过。
- perf gate 失败已分类为 post-B1 terminal semantics 导致旧 baseline 失效；baseline 未更新。
- benchmark-train smoke 通过，manifest 可追溯。
- selection smoke 已运行但不代表 Phase 3 通过。
- 下一步：准备 Phase 3 post-B1 fresh trend validation；先确认是否重建 post-B1 perf baseline，再谈 32/64 局趋势验证。

## 会话：2026-05-22（阶段 B2.1：post-B1 perf baseline candidate）

### 状态
- **阶段 B2.1：** candidate generated；validation failed for direct official replacement。
- **正式 baseline：** 未修改 `test/test_data/perf-baseline.json`，`git diff --name-status -- test/test_data/perf-baseline.json` 无输出。
- **是否建议正式替换：** 暂不建议。candidate 完整但基于单次来源 run，计时基线偏瘦，复跑出现 timing-only gate fail。
- **仍不允许：** 不跑训练 sweep，不跑 `GamesPerSeed=32/64`，不调 PVS/qsearch/LMR/Null Move/aspiration，不扩特征，不改 selection gate，不把 B2.1 写成 Phase 3 趋势通过。

### 执行的操作
- 读取并复核 `task_plan.md`、`findings.md`、`progress.md`、`scripts/perf-runner.ps1`、`test/test_data/perf-baseline.json`、旧 run `20260522-145419` 的 `perf-gate.json` / `perf-summary.json` / `perf-summary.txt`。
- 运行 `git status --short`，确认 B1 源码与记忆文件仍 modified，`docs/national_competition_technical_roadmap.md` 仍 untracked。
- 复核旧 perf failure：`failure_count=15`，`best_move_drift=0`，`score_drift=1`，`performance_regression=14`；case 分布为 `endgame=9`、`opening=6`、`middlegame=0`。
- 从 `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419\perf-summary.json` 生成 post-B1 candidate baseline，并从每条 `stdout_log` 补回可用 benchmark 原始字段。
- 用 candidate baseline 连续复跑 `perf-runner.ps1` 2 次，并读取两个新 run 的 `perf-gate.json` / `perf-summary.txt` 做归因。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\perf-baseline.candidate.json`
- `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\candidate-summary.txt`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

### Candidate 信息
| 项 | 结果 |
|---|---|
| candidate path | `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\perf-baseline.candidate.json` |
| source run | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419` |
| source summary | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419\perf-summary.json` |
| git commit | `6063d9acbb24dc9397e07e99bffa4e94b8af55f6` |
| matrix | 27 条完整：`opening/middlegame/endgame` × depth `6/7/8` × threads `1/2/4` |
| semantic note | post-B1 national terminal semantics；legacy no-capture counter 不再作为 terminal gate |

### Candidate 复跑结果
| 复跑 | run dir | 结果 | 归因 |
|---|---|---|---|
| #1 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729` | failed，1 regression | `middlegame d7 t4 search_ratio=1.405796`；node_ratio `1.011121`；0 best move drift，0 score drift |
| #2 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913` | failed，4 regressions | `opening d7 t2` 和 `middlegame d7/d8` 的 `search_seconds` 超阈；最大 node_ratio `1.069597`；0 best move drift，0 score drift |

### 结论
- 旧正式 baseline 已确认是 pre-B1 语义，不适合作为 post-B1 gate。
- candidate 的 best move 与 score 在两次复跑中稳定；node ratio 没有系统性超过阈值。
- tiny endgame 在 candidate 下不再失败，说明 B1 后 legacy early terminal 节点数误杀已经消失。
- 失败集中在 `search_seconds`，其中 `middlegame d7 t1` 出现节点完全相同但耗时比 `2.059995` 的情况，归类为 timing-sensitive / 单次来源 baseline 不够保守。
- 当前不建议进入 B2.2 正式替换；更合理的下一步是用户确认后生成多样本 post-B1 baseline candidate，再做正式替换与 gate 复验。别用一个瘦得跟纸片似的计时样本去当门神，门没守住还怪门神脾气不好。

## 会话：2026-05-22（阶段 B2.1b：多样本保守聚合 post-B1 perf baseline candidate）

### 状态
- **阶段 B2.1b：** completed。
- **正式 baseline：** 未修改 `test/test_data/perf-baseline.json`。
- **perf-runner：** 未修改 `scripts/perf-runner.ps1`，未放宽阈值。
- **是否建议进入 B2.2：** 建议。aggregate candidate 已连续 2 次通过 `perf-runner.ps1 -BaselinePath <aggregate>`，可作为正式替换候选进入 B2.2。
- **仍不允许：** 不跑训练 sweep，不跑 `GamesPerSeed=32/64`，不调搜索参数，不扩特征，不改 selection gate，不进入 Phase 4。

### 执行的操作
- 读取用户指定文件：`task_plan.md`、`findings.md`、`progress.md`、`scripts/perf-runner.ps1`、`test/test_data/perf-baseline.json`、B2.1 candidate summary/json、`20260522-151729` / `20260522-151913` gate JSON。
- 确认 `test/test_data/perf-baseline.json` 与 `scripts/perf-runner.ps1` 无本轮 diff。
- 用 B2.1 单次 candidate baseline 复跑 5 次 `perf-runner.ps1`，保留每次 `perf-summary.json` / `perf-gate.json`。
- 将 8 个 post-B1 perf summary 聚合成 aggregate candidate，聚合前强制检查 27 个 key 的 `best_move` / `score` 稳定性。
- 用 aggregate candidate 连续复跑 `perf-runner.ps1` 2 次，读取 gate JSON 统计 drift、performance fail、最大 search/node ratio。
- 更新 `task_plan.md`、`findings.md`、`progress.md`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json`
- `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\aggregate-summary.txt`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

### 输入 run 列表
| run | 来源 | gate |
|---|---|---|
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419` | 原始 post-B1 source run | old baseline fail 15 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729` | B2.1 candidate rerun | fail 1 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913` | B2.1 candidate rerun | fail 4 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163919` | B2.1b 新增样本 | fail 1 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163943` | B2.1b 新增样本 | fail 3 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164009` | B2.1b 新增样本 | pass 0 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164037` | B2.1b 新增样本 | pass 0 |
| `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164101` | B2.1b 新增样本 | fail 1 |

### 聚合策略与稳定性
- `best_move` / `score`：27/27 key 在 8 样本中全部一致；未触发停止条件。
- `search_seconds`：取 max，避免单次低计时样本导致后续误杀。
- `nodes`：取 max，避免 baseline 低估 post-B1 搜索树；后续若节点继续超阈，仍应按真实 tree instability 处理。
- `qnodes` / TT / 剪枝 / aspiration 等诊断计数取 max；`nps` 取 min；`pv` 取 max-search 样本代表值。
- `aggregate-summary.txt` 已写入每个条目的 min/median/max `search_seconds`、min/max `nodes`、稳定性、timing-sensitive 和 tree-variable 分类。

### Aggregate candidate 复跑结果
| 复跑 | run dir | 结果 | 关键指标 |
|---|---|---|---|
| #1 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164507` | pass，`failure_count=0` | `best_move_drift=0`，`score_drift=0`，max `node_ratio=1.000000`，max `search_ratio=0.933802` |
| #2 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164532` | pass，`failure_count=0` | `best_move_drift=0`，`score_drift=0`，max `node_ratio=1.057175`，max `search_ratio=1.220779` |

### 结论
- aggregate candidate 兼容 `perf-runner.ps1` baseline 格式，矩阵完整 27 条。
- B2.1 的单次 timing baseline 误杀在 aggregate candidate 下未复现。
- nodes 没有系统性超阈；opening 多线程仍有 tree-variable watch 条目，但本轮复跑没有形成 gate failure。
- 建议进入 B2.2 正式 baseline 替换评审/执行；本轮不替换正式 baseline。
- Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。

## 会话：2026-05-22（阶段 B2.2：正式 perf baseline 替换与 gate 复验）

### 状态
- **阶段 B2.2：** completed。
- **正式 baseline：** 已用 `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json` 替换 `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`。
- **perf-runner：** 未修改 `scripts/perf-runner.ps1`，未放宽阈值。
- **仍不允许：** 不跑训练 sweep，不跑 `GamesPerSeed=32/64`，不调搜索参数，不扩特征，不改 selection gate，不进入 Phase 4。

### 执行的操作
- 读取并复核 `task_plan.md`、`findings.md`、`progress.md`，确认阶段 A/B1 已封口、B2.1b aggregate candidate 已连续两次通过候选 gate、Phase 3 仍 failed、Phase 4 仍 blocked。
- 运行 `git status --short`，并确认 `test/test_data/perf-baseline.json` 与 `scripts/perf-runner.ps1` 起始无 diff。
- 校验 aggregate candidate 文件存在、JSON 可解析，`matrix` 展开为 27 个组合，`results` 为 27 条唯一 case/depth/thread。
- 将 aggregate candidate 原样复制到正式 `test/test_data/perf-baseline.json`；source/target SHA256 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- 替换后检查 diff：只有 `test/test_data/perf-baseline.json` 变化，`scripts/perf-runner.ps1` 无 diff。
- 连续运行两次默认正式 `pwsh ./scripts/perf-runner.ps1`，均未传 `-BaselinePath`。
- 运行 Pester workflow、`build-b1-core` 等价 CTest、`git diff --check`。
- 更新 `task_plan.md`、`findings.md`、`progress.md` 记录 B2.2 证据链。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

### 正式 gate 结果
| 复跑 | run dir | 结果 | 关键指标 |
|---|---|---|---|
| #1 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165717` | pass | baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max `node_ratio=1.001406`，max `search_ratio=1.070239` |
| #2 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165815` | pass | baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max `node_ratio=1.000294`，max `search_ratio=1.249836` |

### 其他验证结果
| 检查 | 命令 | 结果 |
|---|---|---|
| Pester workflow | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet` | `26 passed / 0 failed`；负向测试 exception 为预期输出 |
| CTest 裸命令 | `ctest --test-dir build-b1-core --output-on-failure` | 当前 PATH 中无 `ctest`，未作为测试失败处理 |
| CTest 等价封装 | `. ./scripts/common.ps1; Invoke-SurakartaCTest -BuildDir build-b1-core` | `3/3 passed` |
| diff whitespace | `git diff --check` | exit `0`；仅 LF/CRLF warning，无 whitespace error |

### 结论
- B2.2 正式 baseline 替换与 gate 复验已封口。
- post-B1 aggregate baseline 已成为正式 `perf-runner.ps1` 默认 gate baseline，两次默认 gate 均通过。
- Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。
- 下一步可以进入 Phase 3 post-B1 fresh trend validation 准备，但本轮没有进入 Phase 3，没有跑 32/64 局，没有训练 sweep，也没有调搜索或评估特征。

## 会话：2026-05-25（阶段 3.36：受控诊断 dry-run 执行）

- 复核 `git rev-parse HEAD`，当前 HEAD 仍为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，分支为 `codex/phase2-baseline-trace`。
- 复核 Phase 3.35 / 3.34 / 3.33 / 3.31 输入 manifest，JSON 解析均通过；Phase 3.33 继续保持 `objective_implemented=true`、`default_off=true`、`selection_gate_eligible=false`。
- 运行 `surakarta-bitboard-selftest` 自测构建入口，输出 `BUILD_AND_DRY_RUN_SELFTEST_OK_PHASE3_36`。
- 生成 Phase 3.36 三件套：`guarded-diagnostic-dry-run-execution-report.md`、`guarded-diagnostic-dry-run-execution-report.json`、`guarded-diagnostic-dry-run-execution-manifest.json`。
- 复核输出目录 `phase_output_bin_count=0`，`training_executed=false`，`selection_executed=false`，`production_candidate=false`，`selection_gate_eligible=false`，`phase3_status=not_passed`，`phase4_status=blocked`。
- 结论：允许进入 Phase 3.37 guarded limited training plan review，但不允许直接训练。

## 会话：2026-05-25（阶段 3.39：guarded limited training dry-run execution）

### 状态
- **阶段 3.39：** completed。
- **执行边界：** default-off / isolated / quarantine-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution`。
- **仍不允许：** active objective probe、selection、`GamesPerSeed=64`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录起始脏工作区：`src\`、`include\`、`tools\` 等已有历史改动存在；本轮不认领这些 diff。
- 按原检查点 2 构建 `bitboard-train` target 失败，根因是当前 CMake target 只有 `surakarta-benchmark` 和 `surakarta-bitboard-selftest`，不存在 `bitboard-train` target。
- 改用现有入口 `surakarta-benchmark.exe bitboard-train` 构建并运行 `surakarta-bitboard-selftest`，取得 `BUILD_TRAIN_AND_SELFTEST_OK_PHASE3_39`。
- 输入 manifest 链可解析；原粗正则命中的是 report 字段 `opening_safe_objective_enabled`，补充 flag scan 确认无 `--opening*` / `--*objective*` activation flags，取得 `INPUT_CHAIN_AND_DEFAULT_OFF_CLI_LIMIT_OK_PHASE3_39`。
- 执行 default-off quarantine dry-run：1 game、depth 1、seed `20260423`、checkpoint every 1，输出全部在 Phase 3.39 quarantine 子目录，取得 `DEFAULT_OFF_QUARANTINE_TRAINING_DRY_RUN_OK_PHASE3_39`。
- 生成 Phase 3.39 report/json/manifest 三件套，并验证状态位与 no-selection/candidate 文件名检查，取得 `STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_39`。
- 保护路径复核：`tools` 和三个 benchmark/perf 脚本无 diff；`src/include` diff 仍为历史脏改动，需要继续单独审查，不属于本轮新增。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\guarded-limited-training-dry-run-execution-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\guarded-limited-training-dry-run-execution-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\guarded-limited-training-dry-run-execution-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\quarantine-default-off-run\default-off-training-summary.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\quarantine-default-off-run\default-off-trained-weights.bin`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution\quarantine-default-off-run\checkpoints\checkpoint-1.bin`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.39 只证明 default-off training path 可以在隔离 quarantine 目录中最小运行。
- 这不是 selection，不是 production candidate，不触发 gate，也不表示 Phase 3 passed。
- active scoped objective probe 因当前 CLI 无已评审 activation flags 继续 blocked。
- 建议下一阶段进入 Phase 3.40 `active scoped interface plan review`；不建议进入 blocker review，除非用户坚持原检查点命令必须字面存在 `bitboard-train.exe`。

## 会话：2026-05-26（阶段 3.40：active scoped interface plan review）

### 状态
- **阶段 3.40：** completed。
- **执行边界：** interface-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review`。
- **仍不允许：** 新增 CLI flag、修改 `main.cpp`、实现接口、训练、selection、`GamesPerSeed=64`、active probe、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录起始脏工作区：`src\`、`include\`、`tools\` 等已有历史改动存在；本轮不认领这些 diff。
- 复核 Phase 3.39 report/manifest、Phase 3.38 manifest、Phase 3.33 manifest，JSON 均可解析并输出 `input-json-ok`。
- 按原检查点 3 运行粗正则失败；定位到命中行只是 `opening_safe_objective_enabled` 输出字段，不是 CLI activation flag。
- 补充真实 `--flag` 扫描：`--opening`、`--opening-safe`、`--*objective`、`--active*objective` 均为 0，取得 `ACTIVE_INTERFACE_STILL_NOT_IMPLEMENTED_OK_PHASE3_40`。
- 生成 Phase 3.40 三件套：`active-scoped-interface-plan-review.md`、`active-scoped-interface-plan-review.json`、`active-scoped-interface-plan-review-manifest.json`。
- 验证 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_40`。
- 验证文案黑名单和 no-bin，`phase3_40_bin_count=0`，取得 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_40`。
- 保护路径复核：`tools` 和三个 benchmark/perf 脚本无 diff；`src/include` diff 仍为历史脏改动，需要继续单独审查，不属于本轮新增。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review\active-scoped-interface-plan-review.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review\active-scoped-interface-plan-review.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review\active-scoped-interface-plan-review-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.40 只完成 future active scoped interface 的计划评审。
- 推荐 future 最小接口继续绑定 `surakarta-benchmark.exe bitboard-train`，采用受审 config 文件入口，default-off、显式启用、opening-only scoped、report-only、quarantine-only。
- active probe 仍 blocked，接口仍未实现，Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.41 `guarded active interface skeleton`；不建议 blocker review，除非用户要求原粗正则检查必须字面无匹配。

## 会话：2026-05-26（阶段 3.41：guarded active interface skeleton）

### 状态
- **阶段 3.41：** completed。
- **执行边界：** interface skeleton / default-off / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton`。
- **仍不允许：** active probe、训练战役、selection、`GamesPerSeed=64`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 复核 Phase 3.40 report/manifest、Phase 3.39 manifest、Phase 3.33 manifest，JSON 均可解析并输出 `input-json-ok`。
- 在 `surakarta-benchmark.exe bitboard-train` 路径下实现单一 `--config <path>` config-gated skeleton；未新增散乱 objective toggle。
- 在 `include\surakarta_search.h` / `src\surakarta_search.cpp` 中新增 `ActiveObjectiveConfigSkeleton` 与 parser，默认关闭、无效 config 拒绝、valid scoped config skeleton-only。
- 在 `src\surakarta_bitboard_selftest.cpp` 中新增 Phase 3.41 selftest，覆盖 config 缺省关闭、invalid config rejection、valid scoped config skeleton-only/no active probe。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，运行 selftest，取得 `BUILD_AND_SELFTEST_OK_PHASE3_41`。
- 扫描接口骨架，取得 `CONFIG_GATED_INTERFACE_SKELETON_OK_PHASE3_41`。
- 生成 Phase 3.41 三件套：`guarded-active-interface-skeleton-report.md`、`guarded-active-interface-skeleton-report.json`、`guarded-active-interface-skeleton-manifest.json`。
- 验证 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_41`。
- 验证文案黑名单和 no-bin，`phase3_41_bin_count=0`，取得 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_41`。
- 保护路径复核：`tools` 和三个 benchmark/perf 脚本无 diff；允许代码路径 diff 限于本轮允许文件叠加历史脏改动，需要继续在最终提交前人工拆分。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\src\main.cpp`
- `D:\Dev\Code\surakarta_traditional\include\surakarta_search.h`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_search.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton\guarded-active-interface-skeleton-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton\guarded-active-interface-skeleton-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton\guarded-active-interface-skeleton-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.41 只实现 default-off config-gated skeleton。
- 接口已实现，但 active probe 未运行，仍不能训练/selection/promotion。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.42 `guarded active interface verification package`，不是 active probe。

## 会话：2026-05-26（阶段 3.42：guarded active interface verification package）

### 状态
- **阶段 3.42：** completed。
- **执行边界：** verification-only / default-off / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package`。
- **仍不允许：** active probe、训练战役、selection、`GamesPerSeed=64`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools` diff。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，运行 selftest，取得 `BUILD_AND_SELFTEST_OK_PHASE3_42`。
- 复核 Phase 3.41 report/manifest、Phase 3.40 manifest、Phase 3.39 manifest，JSON 均可解析；Phase 3.41 状态位符合预期，取得 `INPUT_CHAIN_AND_PHASE3_41_INTERFACE_OK_PHASE3_42`。
- 静态扫描确认 `--config` 单一入口存在，无散乱 objective toggle；确认 `ActiveObjectiveConfigSkeleton` 与 selftest markers，取得 `ACTIVE_INTERFACE_STATIC_BOUNDARY_OK_PHASE3_42`。
- 生成 Phase 3.42 三件套：`guarded-active-interface-verification-package.md`、`guarded-active-interface-verification-package.json`、`guarded-active-interface-verification-package-manifest.json`。
- 验证 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_42`。
- 验证文案黑名单、no-bin 和 protected scripts/tools diff，取得 `WORDING_NO_BIN_AND_PROTECTED_DIFF_OK_PHASE3_42`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package\guarded-active-interface-verification-package.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package\guarded-active-interface-verification-package.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package\guarded-active-interface-verification-package-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.42 只验证 Phase 3.41 default-off config-gated skeleton。
- 接口已存在，但 active probe 未运行，仍不能训练/selection/promotion。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.43 `active scoped probe dry-run plan review`，不是 probe execution。

## 会话：2026-05-26（阶段 3.43：active scoped probe dry-run plan review）

### 状态
- **阶段 3.43：** completed。
- **执行边界：** probe-dry-run-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review`。
- **仍不允许：** active probe execution、本轮训练、selection、`GamesPerSeed=64`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools` diff。
- 复核 Phase 3.42 report/manifest、Phase 3.41 manifest、Phase 3.39 manifest，JSON 均可解析并输出 `input-json-ok`。
- 生成 Phase 3.43 三件套：`active-scoped-probe-dry-run-plan-review.md`、`active-scoped-probe-dry-run-plan-review.json`、`active-scoped-probe-dry-run-plan-review-manifest.json`。
- 计划明确 future config 文件样例、命令模板、输入 artifact、quarantine 输出目录、参数上限、监控指标、hard reject、rollback triggers 和停止条件。
- 验证 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_43`。
- 验证文案黑名单和 no-bin，`phase3_43_bin_count=0`，取得 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_43`。
- 保护路径与代码路径复核取得 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_43`；protected scripts/tools 无 tracked diff，`src/include` diff 为历史脏改动和前序阶段改动。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review\active-scoped-probe-dry-run-plan-review.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review\active-scoped-probe-dry-run-plan-review.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review\active-scoped-probe-dry-run-plan-review-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.43 只完成 future active scoped probe 的 dry-run 计划评审。
- Active probe 仍未执行，future probe 也不是正式训练或 selection。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.44 `guarded active scoped probe dry-run execution`。

## 会话：2026-05-26（阶段 3.44：guarded active scoped probe dry-run execution）

### 状态
- **阶段 3.44：** blocker evidence captured。
- **执行边界：** active scoped probe dry-run / quarantine-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution`。
- **仍不允许：** 正式训练、selection、`GamesPerSeed=64`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools` diff。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，运行 selftest，取得 `BUILD_BENCHMARK_AND_SELFTEST_OK_PHASE3_44`。
- 生成 Phase 3.44 active scoped config：`active-scoped-probe-config.json`。
- 执行原检查点无输出路径命令失败，错误为 `output weight path is required`。
- 使用显式 quarantine 输出路径复跑，命令完成；输出路径为 `quarantine-active-scoped-probe\probe-quarantine-output.weights`，stdout JSON 为 `quarantine-active-scoped-probe\probe-command-output.json`。
- 观测 JSON：`active_interface_config_valid=true`、`active_interface_skeleton_enabled=true`、`active_interface_scoped_config=true`、`active_objective_probe_executed=false`、`selection_gate_eligible=false`。
- 生成 Phase 3.44 三件套：`guarded-active-scoped-probe-dry-run-execution-report.md`、`guarded-active-scoped-probe-dry-run-execution-report.json`、`guarded-active-scoped-probe-dry-run-execution-manifest.json`。
- 验证文案黑名单和 protected paths，取得 `WORDING_AND_PROTECTED_DIFF_OK_PHASE3_44`。
- 验证 blocker 状态和隔离，取得 `BLOCKER_STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44`；原成功态 `STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44` 未取得，因为 active objective probe 未执行。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\active-scoped-probe-config.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\quarantine-active-scoped-probe\probe-command-output.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\quarantine-active-scoped-probe\probe-command-error.txt`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\quarantine-active-scoped-probe\probe-quarantine-output.weights`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\guarded-active-scoped-probe-dry-run-execution-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\guarded-active-scoped-probe-dry-run-execution-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution\guarded-active-scoped-probe-dry-run-execution-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.44 捕获到 blocker：当前 active scoped config 只能进入 skeleton state，不能执行 active objective probe。
- 本轮不是正式训练或 selection；candidate flag false，selection gate eligible false。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.45 `active scoped probe evidence review / blocker review`。

## 会话：2026-05-26（阶段 3.45：active scoped probe evidence review / blocker review）

### 状态
- **阶段 3.45：** completed。
- **执行边界：** blocker review / quarantine evidence / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_45_active_scoped_probe_blocker_review`。
- **仍不允许：** 重新 probe、训练、selection、`GamesPerSeed=64`、生成新权重或 `.bin`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools` diff。
- 复核 Phase 3.44 blocker 输入：config、report、manifest、probe command JSON、probe stderr，取得 `BLOCKER_INPUTS_OK_PHASE3_45`。
- 复核 Phase 3.44 blocker 状态：skeleton enabled true、active probe false、hard reject true、rollback true，取得 `PHASE3_44_BLOCKER_STATUS_CONFIRMED`。
- 生成 Phase 3.45 三件套：`active-scoped-probe-blocker-review.md`、`active-scoped-probe-blocker-review.json`、`active-scoped-probe-blocker-review-manifest.json`。
- 评审结论明确 root cause：active scoped config 已 accepted，但只进入 skeleton state，未接到 report-only active probe path。
- 评审结论明确 output artifact 风险：`probe-quarantine-output.weights` 仅是 quarantine evidence，不能进入 selection、release 或 promotion。
- 验证 Phase 3.45 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_45`。
- 验证文案黑名单与 protected paths，取得 `WORDING_AND_PROTECTED_DIFF_OK_PHASE3_45`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_45_active_scoped_probe_blocker_review\active-scoped-probe-blocker-review.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_45_active_scoped_probe_blocker_review\active-scoped-probe-blocker-review.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_45_active_scoped_probe_blocker_review\active-scoped-probe-blocker-review-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.45 完成 blocker review。
- Active probe 未执行；output weight path 是当前执行语义阻塞点；quarantine weights 不能 promotion。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.46 active probe wiring plan review，并把 no-output probe mode 作为强制执行安全项。

## 会话：2026-05-26（阶段 3.46：active probe wiring and no-output mode plan review）

### 状态
- **阶段 3.46：** completed。
- **执行边界：** wiring-plan-only / no-output-mode-plan-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_46_active_probe_wiring_no_output_mode_plan_review`。
- **仍不允许：** probe、训练、selection、`GamesPerSeed=64`、生成新权重或 `.bin`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools` diff。
- 复核 Phase 3.45 blocker review、Phase 3.44 blocker evidence、Phase 3.41 skeleton 输入链，JSON 均可解析并输出 `input-json-ok`。
- 生成 Phase 3.46 三件套：`active-probe-wiring-no-output-mode-plan-review.md`、`active-probe-wiring-no-output-mode-plan-review.json`、`active-probe-wiring-no-output-mode-plan-review-manifest.json`。
- 方案明确 future 最小 wiring 改动面、config 语义、no-output probe mode、输出隔离、验证矩阵和 rollback triggers。
- 验证 Phase 3.46 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_46`。
- 验证文案黑名单与 no weight-like artifact，取得 `WORDING_AND_NO_WEIGHT_ARTIFACT_OK_PHASE3_46`。
- 保护路径与代码路径复核取得 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_46`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_46_active_probe_wiring_no_output_mode_plan_review\active-probe-wiring-no-output-mode-plan-review.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_46_active_probe_wiring_no_output_mode_plan_review\active-probe-wiring-no-output-mode-plan-review.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_46_active_probe_wiring_no_output_mode_plan_review\active-probe-wiring-no-output-mode-plan-review-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.46 完成 wiring/no-output mode plan review。
- Active probe 仍未执行；当前缺口是 active probe wiring 与 no-output mode。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.47 `guarded active probe wiring skeleton`，不是 probe execution。

## 会话：2026-05-26（阶段 3.47：guarded active probe wiring skeleton）

### 状态
- **阶段 3.47：** completed。
- **执行边界：** wiring skeleton / no-output mode skeleton / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_47_guarded_active_probe_wiring_skeleton`。
- **仍不允许：** 真实 active probe、正式训练、selection、`GamesPerSeed=64`、生成 `.bin`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮只认领允许文件中的 Phase 3.47 改动，不认领既有 `src/include/tools` 历史 diff。
- 按 TDD 路径补充 selftest：覆盖 default-off、invalid config、valid scoped config、no-output mode、active probe not executed、weight artifact suppressed。
- 实现 `ActiveObjectiveConfigSkeleton` 的 wiring/no-output/weight-suppressed 状态位。
- 实现 config-gated no-output probe mode skeleton：`probe_only=true` + `no_output_weights=true` 时允许无 output weight path，且不保存 weights/checkpoints。
- `active_objective_probe_executed` 保持 `false`，`selection_gate_eligible` 保持 `false`。
- `src\main.cpp` 输出 summary 新增 wiring/no-output/weight-suppressed 状态字段；没有新增散乱 objective CLI toggle。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，运行 selftest，取得 `BUILD_AND_SELFTEST_OK_PHASE3_47`。
- 静态扫描取得 `WIRING_SKELETON_AND_NO_OUTPUT_BOUNDARY_OK_PHASE3_47`。
- 生成 Phase 3.47 三件套：`guarded-active-probe-wiring-skeleton-report.md`、`guarded-active-probe-wiring-skeleton-report.json`、`guarded-active-probe-wiring-skeleton-manifest.json`。
- 验证 Phase 3.47 manifest 状态位，取得 `STATUS_FLAGS_OK_PHASE3_47`。
- 验证文案黑名单、no-bin/no-weight-like artifact、protected paths，取得 `WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_47`。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\include\surakarta_search.h`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_search.cpp`
- `D:\Dev\Code\surakarta_traditional\src\main.cpp`
- `D:\Dev\Code\surakarta_traditional\src\surakarta_bitboard_selftest.cpp`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_47_guarded_active_probe_wiring_skeleton\guarded-active-probe-wiring-skeleton-report.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_47_guarded_active_probe_wiring_skeleton\guarded-active-probe-wiring-skeleton-report.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_47_guarded_active_probe_wiring_skeleton\guarded-active-probe-wiring-skeleton-manifest.json`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 结论
- Phase 3.47 只完成 wiring skeleton 与 no-output mode skeleton。
- 这仍不是真实 active probe；`active_objective_probe_executed=false`。
- no-output mode skeleton 抑制 candidate/checkpoint/production weights 输出。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
- 建议下一阶段进入 Phase 3.48 `guarded active probe wiring verification package`，不是 probe execution。

## 会话：2026-05-26（阶段 3.48：guarded active probe wiring verification package）

### 状态
- **阶段 3.48：** completed。
- **执行边界：** verification-only / no probe execution / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package`。
- **仍不允许：** 真实 active objective probe、训练、selection、`GamesPerSeed=64`、生成 `.bin/.weights/candidate/checkpoint`、promotion、gate/rating/scorecard/perf threshold 修改、Phase 4。

### 执行的操作
- 复核分支 `codex/phase2-baseline-trace`，HEAD 为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，匹配用户锚定 commit。
- 记录工作区仍有历史脏改动；本轮不认领既有 `src/include/tools/test` diff。
- 生成 Phase 3.48 三件套：`guarded-active-probe-wiring-verification-package.md`、`guarded-active-probe-wiring-verification-package.json`、`guarded-active-probe-wiring-verification-package-manifest.json`。
- 创建 `verify_phase3_48.ps1`，验证分支/HEAD、构建、自测、Phase 3.47 manifest、源码边界、Phase 3.48 状态、输出目录 artifact 边界、禁词和 protected tracked diff。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，运行 selftest。
- 未执行 `bitboard-train`，未执行 active objective probe，未训练，未 selection。

### 创建/修改的文件
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package\guarded-active-probe-wiring-verification-package.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package\guarded-active-probe-wiring-verification-package.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package\guarded-active-probe-wiring-verification-package-manifest.json`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package\verify_phase3_48.ps1`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`

### 验证结果
- `BUILD_AND_SELFTEST_OK_PHASE3_48`
- `PHASE3_47_MANIFEST_OK_PHASE3_48`
- `SOURCE_BOUNDARY_OK_PHASE3_48`
- `STATUS_FLAGS_OK_PHASE3_48`
- `WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_48`

### 结论
- Phase 3.48 只验证 Phase 3.47 skeleton/no-output 边界。
- `active_objective_probe_executed=false` 仍是硬状态。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。
