# 任务计划：Surakarta Traditional 校赛到国赛技术路线图

## 2026-05-26 Phase 3.62 commit staging plan and tracked evidence materialization

### 状态
- **Phase 3.62：** completed as commit staging plan + tracked evidence materialization review campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_62_commit_staging_plan`。
- **执行边界：** 只生成可执行但不执行的 staging/commit 方案、tracked 证据摘要、pathspec/hunk 边界和 verify 包。
- **生成产物：** `commit-staging-plan.md`、`commit-staging-plan.json`、`commit-staging-plan-manifest.json`、`verify_phase3_62.ps1`。
- **Git 边界：** 未 stage、未 commit、未 stash、未 reset、未 checkout、未 clean；`git diff --cached --name-only` 必须为空。
- **archive 策略：** `build-local` 被 `.gitignore:4:/build-local/` 忽略；不建议强行纳入大包，只在 tracked 文档保留摘要、SHA256、路径索引和复跑命令。
- **状态保持：** `phase3_diagnostic_objective_blocker_closed=true`；`phase3_status=not_passed`；`phase4_status=blocked`；`selection_gate_eligible=false`；`production_candidate=false`；`training_executed=false`；`selection_executed=false`。
- **提交分组：** code hunk commit 只能 partial stage 四个 mixed code 文件；docs commit 纳入四个项目文档；archive ownership commit 只纳入 tracked 摘要，除非未来人工批准 archive 策略。
- **排除清单：** `include\surakarta_bitboard.h`、`src\surakarta_bitboard.cpp`、`src\surakarta_dev_session_selftest.cpp`、`src\surakarta_game.cpp`、`src\surakarta_rule_manager_bitboard.cpp`、`src\surakarta_rule_manager_impl.cpp`、`src\surakarta_utils.cpp`、`test\test_data\perf-baseline.json`、`tools\`。
- **输入证据 hash：** Phase 3.60 md/json/manifest 为 `8401DAA284D78CE0D724BAF544B3850A00C15AB8D4BEF9ED73BB5647B018594D`、`98860F86063F10C1921A1771FDA97E0DAE2CD4412286CA06AEF3F2CFD5EC8FE1`、`3BBEE003BD9B766A49C0B62539CDA5421757D1E1602F5C7CB7E34D189BE326D6`；Phase 3.61 md/json/manifest 为 `EEAEA4929BF7F2FBD35880BE5484657671DB3E4C4B70C78A58128C03D9B67C11`、`8C3CB1FB0BEBD2EAFEAB7F339D9AFE3A8AE9344953A060971238F9F7B6A0CC0A`、`3C2C71F85635B7D8FFC0E74681308EB0C108599F58CEE7DD35E38865DED75BBF`。
- **下一阶段：** 只建议人工授权后的 staging execution campaign 或 Phase 4 precondition planning；不得直接进入 training/selection/promotion。

## 2026-05-26 Phase 3.61 evidence archive and commit preparation ownership review

### 状态
- **Phase 3.61：** completed as evidence archive + commit preparation ownership review campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_61_evidence_archive_and_commit_preparation`。
- **执行边界：** 只做 Phase 3.50-3.60 证据归档索引、变更归属审查、提交准备清单、排除清单和风险清单。
- **生成产物：** `archive-and-commit-preparation.md`、`archive-and-commit-preparation.json`、`archive-and-commit-preparation-manifest.json`、`verify_phase3_61.ps1`。
- **状态保持：** `phase3_diagnostic_objective_blocker_closed=true`；`phase3_status=not_passed`；`phase4_status=blocked`；`selection_gate_eligible=false`；`production_candidate=false`；`training_executed=false`；`selection_executed=false`。
- **代码归属结论：** `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 需要 partial staging；不能 whole-file 纳入 Phase 3.50-3.60 commit group。
- **排除清单：** `include\surakarta_bitboard.h`、`src\surakarta_bitboard.cpp`、`src\surakarta_dev_session_selftest.cpp`、`src\surakarta_game.cpp`、`src\surakarta_rule_manager_bitboard.cpp`、`src\surakarta_rule_manager_impl.cpp`、`src\surakarta_utils.cpp`、`test\test_data\perf-baseline.json`、`tools\`。
- **验证摘要：** Phase 3.61 verify 输出 `PHASE3_61_VERIFY_OK`；Phase 3.60 verify 仍输出 `PHASE3_60_VERIFY_OK`；禁用产物扫描为空；protected diff/hash 未漂移。
- **下一阶段：** 只建议 commit staging plan 或 Phase 4 precondition planning；不得直接进入 training/selection/promotion。

## 2026-05-26 Phase 3.60 final evidence consolidation and Phase 4 precondition map

### 状态
- **Phase 3.60：** completed as final evidence consolidation + Phase 4 precondition map campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_60_final_evidence_consolidation`。
- **执行边界：** 只汇总 Phase 3.47-3.59 证据链，形成最终证据索引、状态矩阵、剩余缺口清单和 Phase 4 前置条件地图。
- **禁止事项：** 不执行 active probe、dry-run、training、selection、promotion 或 `GamesPerSeed=64`；不生成 `.bin/.weights/candidate/checkpoint/weights-manifest`；不修改 `src`、`include`、`tools`、`scripts`、`test\test_data`。
- **锚点：** 当前分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，已匹配用户要求。
- **生成产物：** `final-evidence-consolidation.md`、`final-evidence-consolidation.json`、`final-evidence-consolidation-manifest.json`、`verify_phase3_60.ps1`。
- **证据矩阵结论：** Phase 3.47-3.55 只证明 skeleton/verification/readiness/plan/path 或 blocker 解释；Phase 3.56-3.57 形成 guarded report-only/no-output objective diagnostic evidence；Phase 3.58-3.59 分层关闭 diagnostic objective blocker。
- **最终状态：** `phase3_diagnostic_objective_blocker_closed=true`；`phase3_status=not_passed`；`phase4_status=blocked`；`selection_gate_eligible=false`；`production_candidate=false`；`training_executed=false`；`selection_executed=false`。
- **验证摘要：** Phase 3.60 verify 输出 `PHASE3_60_VERIFY_OK`；Phase 3.58-3.59 verify 仍输出 `PHASE3_58_59_VERIFY_OK`；禁用产物扫描为空；protected diff/hash 未漂移。
- **下一阶段：** 只建议 Phase 4 precondition planning 或 Phase 3 evidence archive/commit preparation；仍不得直接进入 training/selection/promotion。

## 2026-05-26 Phase 3.58-3.59 objective evidence review and completion gate

### 状态
- **Phase 3.58-3.59：** completed as objective evidence review + Phase 3 completion gate decision campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_58_59_objective_evidence_review_and_completion_gate`。
- **执行边界：** 只做 Phase 3.56-3.57 objective diagnostic evidence 审查、completion gate 判断、manifest/report 文档化和 verify 包。
- **禁止事项：** 不执行新的 active probe dry-run、training、selection、promotion 或 `GamesPerSeed=64`；不生成 `.bin/.weights/candidate/checkpoint/weights-manifest`；不修改 `src`、`include`、`tools`、`scripts`、`test\test_data`。
- **锚点：** 当前分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，已匹配用户要求。
- **生成产物：** `report.md`、`report.json`、`manifest.json`、`verify_phase3_58_59.ps1`。
- **复核结论：** Phase 3.56-3.57 guarded dry-run objective evidence 满足 diagnostic objective blocker closure 条件：`active_objective_probe_executed=true`、report-only/no-output、artifact suppression、`selection_gate_eligible=false`。
- **Gate 决策：** `phase3_diagnostic_objective_blocker_closed=true`；`phase3_status=not_passed`、`phase4_status=blocked`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- **验证摘要：** build exit code `0`；selftest `[PASS]`；Phase 3.56-3.57 verify 输出 `PHASE3_56_57_VERIFY_OK`；Phase 3.58-3.59 verify 输出 `PHASE3_58_59_VERIFY_OK`。
- **下一阶段：** 只建议 Phase 3.60 final evidence consolidation 或 Phase 4 precondition planning；仍不得执行 Phase 4/training/selection/promotion。

## 2026-05-26 Phase 3.56-3.57 objective-execution report-only blocker closure

### 状态
- **Phase 3.56-3.57：** completed as minimal objective diagnostic evidence implementation + guarded verification。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_56_57_objective_execution_report_only_blocker_closure`。
- **生成产物：** `session-baseline-sha256.json`、`guarded-report-only-no-output-objective-config.json`、`logs\dry-run-stdout.json`、`logs\dry-run-stderr.txt`、`logs\dry-run-exit-code.txt`、`logs\forbidden-artifact-scan.txt`、`logs\protected-diff-scan.txt`、`report.md`、`report.json`、`manifest.json`、`verify_phase3_56_57.ps1`。

### 实现结论
- 修改前已记录四个允许代码文件 session-baseline SHA256；本轮代码改动实际落在 `src\surakarta_search.cpp` 与 `src\surakarta_bitboard_selftest.cpp`，未改 `include\surakarta_search.h` 与 `src\main.cpp`。
- TDD RED 已观察到 guarded no-output evidence 缺失；GREEN 后 `surakarta-benchmark.exe bitboard-selftest` 输出 `[PASS]`。
- 最小实现：`RunBitboardTraining` 只在 `active_interface_report_only_probe_path=true`、config valid、`selection_gate_eligible=false` 时设置 `active_objective_probe_executed=true`，并输出 status `report_only_objective_diagnostic_executed_no_output`。
- 自测覆盖 default-off false、invalid config false、guarded no-output true、artifact suppression、consumer isolation、ordinary output path false。
- 隔离 dry-run exit code `0`，stderr 为空；stdout 显示 `active_objective_probe_executed=true`、`active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- 输出目录扫描未发现 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- 状态保持：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一阶段只允许 Phase 3.58 objective evidence review / Phase 3 completion gate review；不能 training、selection 或 promotion。把诊断灯点亮了不等于车能上赛道，别一脚油门干沟里。

## 2026-05-26 Phase 3.55 dry-run evidence interpretation and blocker closure

### 状态
- **Phase 3.55：** completed as evidence interpretation + objective-execution blocker closure package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_55_dry_run_evidence_interpretation_and_blocker_closure`。
- **生成产物：** `report.md`、`report.json`、`manifest.json`、`verify_phase3_55.ps1`、`logs\forbidden-artifact-scan.txt`、`logs\protected-diff-scan.txt`。

### 结论
- Phase 3.53-3.54 dry-run 证据解释为：command path runnable，且 guarded report-only/no-output 与 artifact suppression 已成立。
- 未形成 objective execution 证据；`active_objective_probe_executed=false` 仍是硬状态。
- 只读源码复核显示：parser 只设置 skeleton/no-output/artifact-suppression 状态；`RunBitboardTraining` 设置 `active_interface_report_only_probe_path` 后仍无条件写入 `active_objective_probe_executed=false`；no-output path 在 weight save、checkpoint save、training loop 之前提前返回。
- Blocker 收束为：当前缺少一个在 no-output early return 前运行的 report-only objective diagnostic helper，因此只有 path-entry evidence，没有 objective diagnostic execution evidence。
- 本轮未修改 `src`、`include`、`tools`、`scripts`、`test\test_data`，未再次执行 active probe，未训练、未 selection、未 promotion、未跑 `GamesPerSeed=64`。
- 状态保持：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`、`active_objective_probe_executed=false`。
- 下一阶段 go/no-go：只允许 Phase 3.56 objective-execution blocker closure plan，或在单独授权后做最小实现任务；training/selection/promotion 继续 no-go。别把“门能打开”讲成“车已经跑完赛道”，这俩差着一台发动机呢。

## 2026-05-26 Phase 3.53-3.54 guarded report-only probe readiness and dry-run

### 状态
- **Phase 3.53-3.54：** completed as readiness closure + guarded report-only/no-output dry-run evidence campaign。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_53_54_guarded_report_only_probe_readiness_and_dry_run`。
- **生成产物：** `guarded-report-only-no-output-config.json`、`logs\dry-run-stdout.json`、`logs\dry-run-stderr.txt`、`logs\dry-run-exit-code.txt`、`logs\forbidden-artifact-scan.txt`、`logs\protected-diff-scan.txt`、`guarded-report-only-probe-readiness-and-dry-run-report.md`、`guarded-report-only-probe-readiness-and-dry-run-report.json`、`guarded-report-only-probe-readiness-and-dry-run-manifest.json`、`verify_phase3_53_54.ps1`。

### 结论
- 已重新运行 build、`surakarta-benchmark.exe bitboard-selftest`、Phase 3.52 verify，均为 exit code `0`；Phase 3.52 verify 输出全部 5 个 marker。
- 已执行一次 `surakarta-benchmark.exe bitboard-train --config <guarded-report-only-no-output-config.json> --format json`，不提供 `--output`，配置固定 `report_only=true`、`probe_only=true`、`no_output_weights=true`、`selection_gate_eligible=false`。
- dry-run exit code `0`，stderr 为空，stdout 显示 `active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- `active_objective_probe_executed=false` 仍保持；因此本阶段只证明 guarded report-only/no-output command path 可运行且不落禁用产物，不证明 objective execution，也不改变 gate 资格。
- 输出目录扫描未发现 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- 本轮未修改 `src`、`include`、`tools`、`scripts`、`test\test_data`；保护范围 diff 仍为历史 scoped 集合，没有相对 Phase 3.52 汇报发生非预期漂移。
- 状态保持：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`、`active_objective_probe_executed=false`。
- 下一阶段只允许 Phase 3.55 evidence review / diagnostics interpretation / continued blocker closure；不能 training、selection 或 promotion。能跑通一条无输出安全通路，不等于能把车开去比赛，别上头。

## 2026-05-26 Phase 3.52 report-only active probe path minimal implementation

### 状态
- **Phase 3.52：** completed as minimal implementation / no execution permission。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_52_report_only_active_probe_path_minimal_implementation`。
- **生成产物：** `session-baseline-sha256.json`、`report-only-active-probe-path-minimal-implementation-report.md`、`report-only-active-probe-path-minimal-implementation-report.json`、`report-only-active-probe-path-minimal-implementation-manifest.json`、`verify_phase3_52.ps1`。

### 实现结论
- 最小新增 `TrainingSummary.active_interface_report_only_probe_path`，仅在 accepted scoped config + no-output mode + artifact suppressed 条件下为 true。
- `RunBitboardTraining` 在 no-output guarded path 仍提前返回，不进入训练循环，不保存 weights/checkpoints，不改变 `active_objective_probe_executed=false` 与 `selection_gate_eligible=false`。
- `src\main.cpp` JSON/text summary 增加 `active_interface_report_only_probe_path`；同时补 `surakarta-benchmark.exe bitboard-selftest` 转发入口以满足验收命令。
- 自测 `TestActiveObjectiveConfigSkeletonDefaultInvalidAndScopedConfig` 覆盖 default-off、invalid config、guarded no-output config、artifact suppression、consumer isolation 和 report-only path indicator。
- 本轮未修改 `tools`、`scripts`、`test\test_data`，未执行真实 probe、training、selection、promotion 或 `GamesPerSeed=64`。
- 状态保持：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。
- 下一步只允许 Phase 3.53 guarded execution readiness review；不能直接 training/selection。别一看灯接上了就拿去跑高速，先验线路，别整活。

## 2026-05-26 Phase 3.51 report-only active probe path implementation plan

### 状态
- **Phase 3.51：** completed as implementation-plan-only / planning-only / no execution。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_51_report_only_active_probe_path_implementation_plan`。
- **生成产物：** `report-only-active-probe-path-implementation-plan.md`、`report-only-active-probe-path-implementation-plan.json`、`report-only-active-probe-path-implementation-plan-manifest.json`、`verify_phase3_51.ps1`。

### 计划结论
- Phase 3.51 基于 Phase 3.50/3.50A 验收结论，只把后续最小实现任务写成可单独授权的计划。
- Future Phase 3.52 最小边界限定为继续使用 `surakarta-benchmark.exe bitboard-train --config <path>`，保持 default-off、opening-only、report-only、quarantine-only、`probe_only=true`、`no_output_weights=true`、`selection_gate_eligible=false`。
- Future 改动面仅可在单独授权后触及 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 的最小诊断路径、自测和 summary 字段。
- 禁止新增自由 objective toggle；禁止保存 weight-like artifact；禁止接入 selection/rating/scorecard/perf threshold/promotion/release。
- 本轮未改生产代码，未执行 probe/training/selection，未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- 状态保持：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。
- 结论：Phase 3.52 可作为单独授权的最小实现任务开启；Phase 3.51 本身不是实现完成，也不是执行许可。图纸归图纸，别拿图纸去参加路跑，鞋都没穿呢。

## 2026-05-26 Phase 3.50A acceptance audit addendum

### 状态
- **Phase 3.50A：** completed as acceptance audit addendum / no conclusion change / no Phase 3.51 entry。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan`。
- **新增/更新产物：** `guarded-active-probe-acceptance-audit-addendum.md`、`guarded-active-probe-acceptance-audit-addendum-manifest.json`、`verify_phase3_50.ps1`。

### 验收补强结论
- `verify_phase3_50.ps1` 已对齐验收 marker：`PHASE3_49_BLOCKER_INPUT_OK_PHASE3_50`、`STATUS_FLAGS_OK_PHASE3_50`、`WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_50`。
- Addendum 明确区分历史 scoped diff 与本轮新增边界：当前 `src/include/test_data` diff 作为历史脏状态记录，不作为 Phase 3.50A 输出；若验证时 diff 集合偏离记录清单，脚本会失败。
- tools 与 protected scripts hash 已给出文件级明细，不再使用笼统“prior evidence chain”口径。
- Phase 3.50 结论不变：`ready_for_guarded_probe=false`、Phase 3 `not_passed`、Phase 4 `blocked`。
- 本轮仍未改 `src/include/tools/scripts/test_data`，未执行 probe/training/selection，未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。这轮就是补验收凭证，别顺手把验收单当开工令，整那出就离谱了。

## 2026-05-26 Phase 3.50 guarded active probe blocker closure plan

### 状态
- **Phase 3.50：** completed as blocker closure plan / readiness gate / no execution。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_50_guarded_active_probe_blocker_closure_plan`。
- **生成产物：** `guarded-active-probe-blocker-closure-plan.md`、`guarded-active-probe-blocker-closure-plan.json`、`guarded-active-probe-blocker-closure-plan-manifest.json`、`verify_phase3_50.ps1`。

### Closure 结论
- Phase 3.47 证明 wiring skeleton 与 no-output mode skeleton 存在，但 `active_objective_probe_executed=false`。
- Phase 3.48 只做 verification-only，没有执行 probe、training 或 selection。
- Phase 3.49 已判定 `ready_for_guarded_probe=false`，不能直接开真实 guarded probe。
- Phase 3.50 将 blocker 收束为：真实 report-only active probe path 尚未被证据链证明，accepted config 仍只证明 skeleton/no-output boundary。
- Phase 3.51 可开工，但只能限定为 report-only active probe path implementation plan 或单独授权的最小实现任务；当前真实 guarded probe execution 仍是 no-go。
- 状态继续固定：`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。
- 本轮未改 `src/include/tools/scripts/test_data`，未执行 probe/training/selection，未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。别把“堵点清单写明白了”吹成“车已经开出收费站”，这事儿咱不整虚的。

## 2026-05-26 Phase 3.49 guarded active probe execution readiness review

### 状态
- **Phase 3.49：** completed as readiness-only / no execution / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_49_guarded_active_probe_execution_readiness_review`。
- **生成产物：** `guarded-active-probe-execution-readiness-review.md`、`guarded-active-probe-execution-readiness-review.json`、`guarded-active-probe-execution-readiness-review-manifest.json`、`verify_phase3_49.ps1`。

### Readiness 结论
- 复核分支 `codex/phase2-baseline-trace` 与 HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，锚点一致。
- Phase 3.47 manifest 证明 wiring skeleton 与 no-output mode skeleton 已存在，但 `active_objective_probe_executed=false`。
- Phase 3.48 manifest 证明上一轮只是 verification-only，未执行 probe、training、selection，状态仍为 `phase3_status=not_passed`、`phase4_status=blocked`。
- Phase 3.49 readiness 结论固定为 `ready_for_guarded_probe=false`。
- Blocker：当前证据链只证明 skeleton/no-output guard，不证明真实 report-only probe 路径已经可运行并被验证。
- 不建议进入 Phase 3.50 guarded active probe execution；除非先单独补齐并验证真实 report-only probe path，且继续满足 no-output、quarantine-only、protected consumer 隔离。
- 本轮未执行 probe/training/selection，未跑 `GamesPerSeed=64`，未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- 结论维持：Phase 3 `not_passed`，Phase 4 `blocked`。别把“线接到空开旁边了”说成“电机能安全开转”，这锅咱不背。

## 2026-05-26 Phase 3.48 guarded active probe wiring verification package

### 状态
- **Phase 3.48：** completed as verification-only / no probe execution / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_48_guarded_active_probe_wiring_verification_package`。
- **生成产物：** `guarded-active-probe-wiring-verification-package.md`、`guarded-active-probe-wiring-verification-package.json`、`guarded-active-probe-wiring-verification-package-manifest.json`、`verify_phase3_48.ps1`。

### 验证结论
- 复核分支 `codex/phase2-baseline-trace` 与 HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，锚点一致。
- Phase 3.47 manifest 固定为 `wiring_skeleton_implemented=true`、`no_output_probe_mode_implemented=true`、`active_objective_probe_executed=false`。
- Phase 3.48 manifest 固定为 `verification_only=true`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest` 并运行 selftest；验证脚本输出 `BUILD_AND_SELFTEST_OK_PHASE3_48`。
- 源码边界确认未出现 `active_objective_probe_executed` 的 true 赋值/输出；selftest 保留 no-output 与未运行真实 probe 的证据，输出 `SOURCE_BOUNDARY_OK_PHASE3_48`。
- Phase 3.48 输出目录未生成 `.bin`、`.weights`、candidate-like、checkpoint-like、weights-manifest-like 文件。
- `tools` 与 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1` 的 tracked diff 为空。
- 本轮未执行 active objective probe、未训练、未 selection、未跑 `GamesPerSeed=64`，未改代码逻辑/schema/CLI/objective/训练逻辑。
- 结论维持：Phase 3 `not_passed`，Phase 4 `blocked`。骨架验证完了也只是骨架，别把脚手架当楼盘交付，这事儿东北老乡都得笑出声。

## 2026-05-26 Phase 3.40 active scoped interface plan review

### 状态
- **Phase 3.40：** completed as interface-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review`。
- **生成产物：** `active-scoped-interface-plan-review.md`、`active-scoped-interface-plan-review.json`、`active-scoped-interface-plan-review-manifest.json`。

### 计划结论
- 固定事实继续成立：当前训练入口是 `surakarta-benchmark.exe bitboard-train`，没有独立 `bitboard-train.exe` 或 CMake target；当前没有已评审 active objective activation flags。
- 本阶段没有新增 CLI flag、没有修改 `main.cpp`、没有实现接口、没有改训练逻辑、没有训练、没有 selection、没有 `.bin`。
- 原检查点 3 的粗正则仍会命中 `opening_safe_objective_enabled` 输出字段；补充真实 `--flag` 扫描确认 `--opening*`、`--opening-safe`、`--*objective*`、`--active*objective` 数量均为 0。
- Future 最小接口建议保留既有 `surakarta-benchmark.exe bitboard-train` 入口，并优先采用单个受审 config 文件入口，例如 future `--opening-safe-objective-config <json>`，避免一堆自由 CLI toggle 把边界拧散。
- Future config 必须 default-off、显式启用、`mode=active_scoped`、opening-only scoped、report-only、`selection_gate_eligible=false`，并强制 output/checkpoint 在 phase quarantine root 下。
- 允许参数被限制为小权重 `0 < opening_drift_penalty_weight <= 0.01`、opening fixture rank delta 来源、root-cost multiplier cap、bounded ply window、seed allowlist `20260423`、最多 4 局、depth 最多 4。
- 禁止参数包括默认启用、gate/rating/scorecard/perf threshold toggle、production artifact path、promotion switch、`GamesPerSeed=64`、phase quarantine 外输出、feature/tuple shrinkage-first、search/eval 行为 switch。
- 验证矩阵覆盖 default-off no config、disabled config、zero-weight config、active-scoped fixture only、hard reject fixture、quarantine path guard、forbidden consumer scan。
- rollback triggers 覆盖锚点不一致、默认关闭变化、接口默认 enabled/non-zero、forbidden consumer 消费、计划阶段出现训练/selection/`.bin`、quarantine 逃逸、`selection_gate_eligible=true` 或状态位漂移。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。
- **下一步：** 允许进入 Phase 3.41 `guarded active interface skeleton`；不允许直接 active probe 或 Phase 4。别把接口设计图当施工许可证，图纸画完还没到砸墙那步。

## 2026-05-25 Phase 3.39 guarded limited training dry-run execution

### 状态
- **Phase 3.39：** completed as default-off training dry-run / isolated / quarantine-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution`。
- **生成产物：** `guarded-limited-training-dry-run-execution-report.md`、`guarded-limited-training-dry-run-execution-report.json`、`guarded-limited-training-dry-run-execution-manifest.json`。
- **Quarantine dry-run 产物：** `quarantine-default-off-run\default-off-training-summary.json`、`quarantine-default-off-run\default-off-trained-weights.bin`、`quarantine-default-off-run\checkpoints\checkpoint-1.bin`。

### 执行结论
- 锚定 HEAD 复核通过：`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，分支 `codex/phase2-baseline-trace`。
- 原检查点 2 的 `bitboard-train` CMake target 不存在；实际现有训练入口是 `surakarta-benchmark.exe bitboard-train`，本阶段未新增或改动 CLI。
- 输入 manifest 链可解析；`src\main.cpp` 中粗正则命中的是 JSON/text report 字段 `opening_safe_objective_enabled`，补充 flag scan 确认没有 `--opening*` 或 `--*objective*` activation flags。
- 实际 dry-run 使用 `surakarta-benchmark.exe bitboard-train --output <quarantine>\default-off-trained-weights.bin --games 1 --depth 1 --seed 20260423 --checkpoint-every 1 --checkpoint-dir <quarantine>\checkpoints --format json`。
- dry-run summary：`games_completed=1`、`opening_safe_objective_enabled=false`、`opening_drift_penalty_active=false`、`opening_drift_penalty_scope_status=inactive`、`inactive_path_equivalent=true`、`hard_reject_triggered=false`。
- manifest 状态位固定为 `training_dry_run_executed=true`、`default_off=true`、`active_objective_probe_executed=false`、`selection_executed=false`、`production_candidate=false`、`selection_gate_eligible=false`、`quarantine_only=true`、`phase3_status=not_passed`、`phase4_status=blocked`。
- 未运行 selection，未跑 `GamesPerSeed=64`，未 promotion，未改 gate/rating/scorecard/perf threshold，未改 `src\`、`include\`、`tools\`、`scripts\`。
- 允许进入 Phase 3.40 `active scoped interface plan review`，但只允许做接口计划评审；active objective probe 仍 blocked，不能靠脑补把没有的 CLI flag 拧出来。

## 2026-05-25 Phase 3.38 guarded limited training dry-run plan

### 状态
- **Phase 3.38：** completed as training-dry-run-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_38_guarded_limited_training_dry_run_plan`。
- **生成产物：** `guarded-limited-training-dry-run-plan.md`、`guarded-limited-training-dry-run-plan.json`、`guarded-limited-training-dry-run-plan-manifest.json`。

### 计划结论
- 本阶段只设计 future Phase 3.39 guarded limited training dry-run execution；未运行训练，未运行 selection，未生成 `.bin`。
- Phase 3.39 输出必须隔离到 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution`，并拆成 `inputs`、`logs`、`weights-quarantine`、`checkpoints-quarantine`、`reports`。
- 现有 `scripts\benchmark-train.ps1` 会写 `build-local\training\<timestamp>`，不满足 Phase 3.39 隔离要求；计划模板改为 future 直调 `surakarta-benchmark.exe bitboard-train`，并把权重和 checkpoint 都留在 quarantine 子目录。
- 当前 CLI 未暴露 opening objective activation flags，所以 Phase 3.39 只允许 default-off 隔离 dry-run；active scoped objective probe 需要单独接口授权，不能偷塞进这个阶段。
- 参数上限继承 Phase 3.37：seed `20260423`、最多 4 局、depth 4、checkpoint every 2、`alpha=0.005`、`lambda=0.5`、`epsilon=0.02`、`terminal_reward=1200`、`td_error_clip=0`。
- 允许进入 Phase 3.39 guarded limited training dry-run execution 或 blocker review；不允许 selection、`GamesPerSeed=64`、promotion、gate 变更或 Phase 4。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

## 2026-05-25 Phase 3.37 guarded limited training plan review

### 状态
- **Phase 3.37：** completed as training-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_37_guarded_limited_training_plan_review`。
- **生成产物：** `guarded-limited-training-plan-review.md`、`guarded-limited-training-plan-review.json`、`guarded-limited-training-plan-review-manifest.json`。

### 计划结论
- 本阶段只基于 Phase 3.36 dry-run 证据设计未来最小 guarded limited training 边界；未执行训练，未执行 selection，未生成 `.bin`。
- future actual probe 的上限被压到单 seed `20260423`、最多 4 局、depth 4、checkpoint every 2、TD 默认参数、opening objective 显式 scoped 小权重上限 `0.01`。
- 前置条件包括锚定 HEAD 一致、Phase 3.36 JSON 可解析、default-off 和 rule-correctness 证据保留、protected scripts/tools/gate 不变、`selection_gate_eligible=false`。
- rollback triggers 包括锚点不一致、证据缺失、代码/脚本/gate 新 diff、default-off 变化、hard reject 非 report-only、selection 或 `GamesPerSeed=64` 出现、状态位漂移、NaN/Inf 或 manifest 缺失。
- 当前只允许进入 Phase 3.38 guarded limited training dry-run plan 或 blocker review；不允许直接训练。拿路线审批当油门许可，这毛病这轮继续按住。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

## 2026-05-25 Phase 3.35 guarded diagnostic dry-run plan

### 状态
- **Phase 3.35：** completed as dry-run plan only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_35_guarded_diagnostic_dry_run_plan`。
- **生成产物：** `guarded-diagnostic-dry-run-plan.md`、`guarded-diagnostic-dry-run-plan.json`、`guarded-diagnostic-dry-run-plan-manifest.json`。

### 计划结论
- future diagnostic dry-run 只允许读取 Phase 3.34 / 3.33 / 3.32 / 3.31 证据链和现有诊断能力，观察 default-off、inactive enabled、active scoped、hard reject report-only 四类样本。
- 计划指标包括 `opening_safe_objective_enabled`、`opening_drift_penalty_active`、`opening_drift_penalty_value`、`opening_drift_penalty_scope_status`、`inactive_path_equivalent`、`hard_reject_triggered`、`hard_reject_reasons`、`selection_gate_eligible`、memory-only TD-error delta、zero-persistence confirmation 和 `.bin` count。
- 本阶段不实现新逻辑，不新增诊断脚本，不训练、不 selection、不跑 `GamesPerSeed=64`、不生成 `.bin`、不改 gate/rating/scorecard/perf threshold。
- 下一步只允许进入 Phase 3.36 guarded diagnostic dry-run execution plan 或 blocker review；仍不允许直接开练。

## 2026-05-25 Phase 3.34 guarded implementation verification package

### 状态
- **Phase 3.34：** completed as verification-only / default-off / not selection eligible package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_34_guarded_implementation_verification_package`。
- **生成产物：** `guarded-implementation-verification-package.md`、`guarded-implementation-verification-package.json`、`guarded-implementation-verification-package-manifest.json`。

### 验证结论
- 复核锚定 HEAD：`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，分支 `codex/phase2-baseline-trace`。
- Phase 3.33 manifest 状态位符合预期：`objective_implemented=true`、`default_off=true`、`training_loss_modified=true`、`selection_gate_eligible=false`、`production_candidate=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- `surakarta-bitboard-selftest` Release 构建与运行通过，覆盖 default-off、inactive equivalence、active scoped fixture、hard reject report-only 和 `selection_gate_eligible=false`。
- 本阶段不新增 objective 逻辑，不训练、不 selection、不跑 `GamesPerSeed=64`、不生成 `.bin`、不修改 gate。
- 下一步只允许进入 Phase 3.35 guarded diagnostic dry-run plan；仍不允许训练、selection、`.bin` 或 gate 改动。

## 2026-05-25 Phase 3.33 guarded minimum-objective implementation

### 状态
- **Phase 3.33：** completed as guarded implementation / default-off / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_33_guarded_minimum_objective_implementation`。
- **生成产物：** `guarded-minimum-objective-implementation-report.md`、`guarded-minimum-objective-implementation-report.json`、`guarded-minimum-objective-implementation-manifest.json`。
- **代码改动面：** 仅限 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\surakarta_bitboard_selftest.cpp`、`src\main.cpp`。
- **default-off 机制：** 新增 opening-safe objective options 默认均为 false、zero 或 empty；既有调用不传 options 时走 disabled 默认对象。
- **objective 实现边界：** 仅实现最小 scoped formula path：`weight * unsafe_rank_degradation * root_cost_multiplier`；只在显式 enabled、root case 非空、ply window 有效、输入为正且无 hard reject 时进入 `active_scoped`。
- **training loss：** active scoped path 会从 TD target 扣除 penalty，因此 manifest 记录 `training_loss_modified=true`；default-off 与 inactive path 经自测证明 checked 行为等价。
- **hard reject：** `hard_reject_report_only` 只作为 report-only companion 诊断，强制零 penalty，不进入 rating、scorecard、perf threshold 或 selection gate。
- **未执行：** 未运行训练流程，未运行 selection，未运行 `GamesPerSeed=64`，Phase 3.33 输出目录未生成 `.bin`。
- **禁改范围：** 未修改 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`，未修改 gate。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 验证闭环
- `surakarta-bitboard-selftest` Release 构建与自测通过，关键行：`BUILD_AND_SELFTEST_OK`。
- default-off 与 protected script scan 通过，关键行：`DEFAULT_OFF_AND_GATE_ISOLATION_OK`。
- Phase 3.33 report/manifest JSON 可解析，状态位符合要求。
- Phase 3.33 输出目录文案 guard 和 no-bin 检查通过。
- protected path diff 检查通过。
- 允许下一阶段进入 verification/diagnostic review；不允许训练、selection、`.bin` 或 gate 改动。

## 2026-05-24 Phase 3.32 minimum objective implementation plan review

### 状态
- **Phase 3.32：** completed as minimum objective implementation plan review / implementation-plan-only / design-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_32_minimum_objective_implementation_plan_review`。
- **生成产物：** `minimum-objective-implementation-plan-review.md`、`minimum-objective-implementation-plan-review.json`、`minimum-objective-implementation-plan-review-manifest.json`。
- **未来最小改动面：** 仅限后续单独授权时的 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`、必要时 `scripts\workflow.tests.ps1`、Phase 输出目录和四个项目文档。
- **禁止改动面：** `tools\*`、三个 benchmark/perf 脚本、selection gate、rating、scorecard、perf threshold、promotion、自博弈走法、搜索参数、eval/search、tuple/features、训练产物、`.bin`、Phase 4。
- **测试矩阵：** default-off、inactive equivalence、formula helper、active scoped fixture、hard reject、report-only wiring、CLI/report、forbidden path guard、artifact guard。
- **validation order：** branch/head -> input JSON parse -> hash snapshot -> red checks -> default-off/inactive checks -> hard reject/report-only consumer checks -> CLI/report checks -> forbidden path compare -> no training/selection/`.bin` -> future evidence package。
- **rollback triggers：** 禁改路径变化、默认值非 disabled/zero/empty/false、disabled path 行为变化、companion 被 rating/scorecard/perf threshold/selection gate 消费、benchmark/perf 脚本变化、训练/selection/`GamesPerSeed=64`/`.bin` 出现、非 opening drift、final `20260423` side-effect 未进入 report-only、tuple/feature/search/opening book 抢跑、inactive equivalence 证据不足。
- **hard reject：** 只能进入 `report-only acceptance companion`，字段限 `report_only`、`hard_reject_triggered`、`hard_reject_reasons`、`selection_result_unchanged`、`gate_not_modified`、`selection_gate_eligible=false`。
- **准入结论：** 可在单独授权后进入 future guarded minimum-objective work；本阶段本身只完成计划评审。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 验证闭环
- Phase 3.31 report/manifest、Phase 3.30 skeleton package、Phase 3.29 hard reject report 四个输入 JSON 均可解析。
- Phase 3.32 JSON report/manifest 可解析。
- manifest 状态位：`implementation_plan_only=true`、`objective_implemented=false`、`training_loss_modified=false`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- Phase 3.32 输出目录 `.bin` 数量为 0。
- 本阶段未新增修改 `src\*`、`include\*`、`tools\*`、`scripts\*`，未修改 gate。

## 2026-05-24 Phase 3.31 minimum objective formula design review

### 状态
- **Phase 3.31：** completed as minimum objective formula design review / design-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_31_minimum_objective_formula_design_review`。
- **生成产物：** `minimum-objective-formula-design-review.md`、`minimum-objective-formula-design-review.json`、`minimum-objective-formula-design-review-manifest.json`。
- **公式边界：** 只定义未来最小公式语义：`opening_safe_penalty = opening_drift_penalty_weight * unsafe_rank_degradation * bounded_root_cost_multiplier * opening_scope_mask`。
- **输入/输出语义：** JSON 已明确 default-off 配置字段、root-child relationship evidence、bounded root-cost status、hard reject companion 输入，以及 scope、rank degradation、cost multiplier、penalty、diagnostics 输出字段。
- **hard reject：** 仅映射到 `report-only acceptance companion` 的 `hard_reject_triggered` / `hard_reject_reasons`；不进入 selection gate、rating、scorecard、perf threshold 或 training loss。
- **非目标项：** 不实现 objective、不改 training loss、不改 TD target / eligibility trace / self-play / eval/search / weight update、不训练、不 selection、不跑 `GamesPerSeed=64`、不生成或推广 `.bin`、不进入 Phase 4。
- **准入结论：** 允许进入下一阶段 implementation plan review，但只限 plan review；不授权实现。
- **Phase 3：** `not_passed`。
- **Phase 4：** `blocked`。

### 验证闭环
- Phase 3.22、Phase 3.24、Phase 3.30 report 和 Phase 3.30 manifest 四个输入 JSON 均可解析。
- Phase 3.31 JSON report/manifest 可解析。
- manifest 状态位：`design_only=true`、`formula_design_only=true`、`objective_implemented=false`、`training_loss_modified=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- Phase 3.31 输出目录 `.bin` 数量为 0。
- 本阶段未修改 `src\*`、`include\*`、`tools\*`、`scripts\*`，未修改 gate。

## 2026-05-24 Phase 3.30 skeleton validation evidence package

### 状态
- **Phase 3.30：** completed as skeleton validation evidence package / report-only summary。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_30_skeleton_validation_evidence_package`。
- **证据包：** `skeleton-validation-evidence-package.json`、`skeleton-validation-evidence-package-manifest.json`。
- **汇总范围：** Phase 3.25 default-off skeleton、Phase 3.26 inactive equivalence、Phase 3.27 scoped activation no-op、Phase 3.28 report-only wiring、Phase 3.29 hard reject fixture。
- **结论：** `allow_phase3_31_minimum_objective_formula_design_review=true`，但只允许进入 formula design review；不允许 implementation、不允许改 training loss、不允许训练/selection/`.bin`、不允许 Phase 3 passed 或 Phase 4 unblocked。
- **代码边界：** 本阶段未修改 `src\*`、`include\*`、`tools\*`、三个 benchmark/perf 脚本。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.30 输出目录未生成 `.bin`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- 10 个输入 JSON 均可解析。
- Phase 3.30 JSON report/manifest 可解析。
- manifest 状态位：`report_only=true`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- `src\*`、`include\*`、`tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1` before/after hash 一致。
- Phase 3.30 输出目录 `.bin` 数量为 0。

## 2026-05-24 Phase 3.29 hard reject fixture proof

### 状态
- **Phase 3.29：** completed as hard reject fixture proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_29_hard_reject_fixture_proof`。
- **证据包：** `hard-reject-fixture-proof-report.json`、`hard-reject-fixture-proof-manifest.json`。
- **源码改动：** 仅在 `src\surakarta_bitboard_selftest.cpp` 的既有 opening-safe fixture 中增加 Phase 3.29 hard reject fixture proof marker 和 selftest-only fixture matrix。
- **覆盖范围：** 覆盖 Phase 3.22 / 3.24 定义的 11 条 hard reject 条件，包括 final `20260423` non-baseline new best、qnodes increase + root over-neutralization、side-effect signature、任一 final seed new best + qnodes increase、非 opening drift/score regress、C/B/D/fixed score/perf 恶化、default-off/equivalence 失败、companion 被 gate 消费、diagnostic `.bin` promotion、`all_negative` 或 tuple shrinkage-first。
- **report-only：** 每条 hard reject fixture 的 sink 都固定为 `report-only evidence`，selection gate、rating、scorecard、perf threshold 均不消费。
- **实现范围：** 未实现 objective，未修改 training loss，未改变 TD target、eligibility trace、自博弈走法、eval/search 或权重更新语义。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.29 输出目录未生成 `.bin`。
- **禁改范围：** 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- TDD red check：Phase 3.29 前 `Phase 3.29` marker 不存在，预期失败。
- 本机实盘验证使用 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat -arch=x64 -host_arch=x64`、`D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`、`D:\Dev\Code\surakarta_traditional\build-vs-release`、target `surakarta-bitboard-selftest`、exe `surakarta-bitboard-selftest.exe`。
- `surakarta_search.cpp` source scan 未发现 `training_loss`、`loss +=`、opening TD error objective 或 opening penalty weight-update 实现。
- gate/benchmark scripts scan 未发现 `hard.reject|acceptance.companion|opening.guard|root.cost.companion`。
- Phase 3.29 JSON report/manifest 可解析。
- Phase 3.29 输出目录 `.bin` 数量为 0。

## 2026-05-24 Phase 3.28 report-only diagnostics wiring proof

### 状态
- **Phase 3.28：** completed as report-only diagnostics wiring proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_28_report_only_diagnostics_wiring_proof`。
- **证据包：** `report-only-diagnostics-wiring-proof-report.json`、`report-only-diagnostics-wiring-proof-manifest.json`。
- **源码改动：** 仅在 `src\surakarta_bitboard_selftest.cpp` 的既有 opening-safe fixture 中增加 Phase 3.28 report-only wiring proof marker 和 selftest-only evidence matrix。
- **证明范围：** hard reject、opening guard、root-cost companion、acceptance companion 的允许出口固定为 `report-only evidence chain`；selection gate、rating、scorecard、perf threshold、training loss 均不消费这些 diagnostics。
- **实现范围：** 未实现 objective，未修改 training loss，未改变 TD target、eligibility trace、自博弈走法、eval/search 或权重更新语义。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.28 输出目录未生成 `.bin`。
- **禁改范围：** 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- TDD red check：Phase 3.28 前 `report-only wiring` marker 不存在，预期失败。
- 本机实盘验证使用 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat -arch=x64 -host_arch=x64`、`D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`、`D:\Dev\Code\surakarta_traditional\build-vs-release`、target `surakarta-bitboard-selftest`，自测通过。
- 用户命令块里的 CMake build dir 已按实际 `CMakeCache.txt` 所在目录固定；但 exe filter 仍写旧 underscore 名 `surakarta_bitboard_selftest.exe`，实际 target 产物为 `surakarta-bitboard-selftest.exe`。按实际产物名定位后完整验证通过。
- gate/benchmark scripts scan 未发现 `opening.safe|hard.reject|acceptance.companion|root.cost.companion|opening.guard`。
- Phase 3.28 JSON report/manifest 可解析。
- Phase 3.28 输出目录 `.bin` 数量为 0。

## 2026-05-24 Phase 3.27 scoped activation no-op fixture

### 状态
- **Phase 3.27：** completed as scoped activation no-op fixture / explicit enabled skeleton / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_27_scoped_activation_noop_fixture`。
- **证据包：** `scoped-activation-noop-fixture-report.md`、`scoped-activation-noop-fixture-report.json`、`scoped-activation-noop-fixture-manifest.json`。
- **源码改动：** 仅在 `src\surakarta_bitboard_selftest.cpp` 的既有 opening-safe fixture 中增加显式 enabled skeleton no-op case。
- **显式启用项：** `opening_safe_objective_enabled=true`、`opening_drift_penalty_weight=0.0`、`opening_drift_penalty_max_ply_window=1`、`opening_drift_penalty_root_case_id="phase3_27_scoped_activation_noop_fixture"`、`opening_drift_penalty_emit_diagnostics=true`。
- **等价比较：** weights、traces、current value、target value、td error、abs weight delta、changed weight count 均与 baseline training step 一致。
- **实现范围：** 未实现 objective，未修改 training loss，未改变 TD target、eligibility trace、自博弈走法、eval/search 或权重更新语义。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.27 输出目录未生成 `.bin`。
- **禁改范围：** 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- TDD red check：Phase 3.27 前 `activation no-op` marker 不存在。
- 本机等价验证使用 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat -arch=x64 -host_arch=x64`、`D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`、`build-vs-release`、target `surakarta-bitboard-selftest`，自测通过。
- 用户给定字面命令仍有执行层问题：`build-local` 不是 CMake build dir；实际 target 是 `surakarta-bitboard-selftest` 不是 `surakarta_bitboard_selftest`；`VsDevCmd.bat -no_logo` 在本 shell 中选到 x86 libs，x64 build 需要 `-arch=x64 -host_arch=x64`。路径这回对了，门牌号对了，但屋里接线还是歪的。
- source scan 未发现 `training_loss`、`loss +=`、opening TD error objective 或 opening penalty weight-update 实现。
- Phase 3.27 JSON report/manifest 可解析。
- Phase 3.27 输出目录 `.bin` 数量为 0。

## 2026-05-24 Phase 3.26 scoped inactive-equivalence verification

### 状态
- **Phase 3.26：** completed as scoped inactive-equivalence verification / default-off / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_26_scoped_inactive_equivalence_verification`。
- **证据包：** `scoped-inactive-equivalence-verification-report.md`、`scoped-inactive-equivalence-verification-report.json`、`scoped-inactive-equivalence-verification-manifest.json`。
- **源码改动：** 仅在 `src\surakarta_bitboard_selftest.cpp` 的既有 inactive equivalence fixture 中补 Phase 3.26 明确 marker。
- **实现范围：** 未实现 objective，未修改 training loss，未改变 TD target、eligibility trace、自博弈走法、eval/search 或权重更新语义。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.26 输出目录未生成 `.bin`。
- **禁改范围：** 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- 构建入口硬编码为 VS CMake：`C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`。
- `surakarta_bitboard_selftest` Release 构建通过。
- `surakarta_bitboard_selftest.exe` 通过，并覆盖 `Phase 3.26 inactive equivalence default-off disabled path no-op marker`。
- source scan 未发现 `training_loss`、`loss +=`、opening TD error objective 或 opening penalty weight-update 实现。
- Phase 3.26 JSON report/manifest 可解析。
- Phase 3.26 输出目录 `.bin` 数量为 0。
- 用户要求读取的 Phase 3.25 `.md` 报告不存在；实际 Phase 3.25 目录只有 JSON report 和 manifest，已在 Phase 3.26 证据包记录。清单写了文件，盘上没有文件，这事儿别装没看见。
- 用户给定字面验证命令因 `C:\Program Files\Microsoft Visual Studio\2022\Community\...\cmake.exe` 不存在失败；本机实际 VS CMake 为 `D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`，且必须先导入 `VsDevCmd.bat`，等价验证通过。

## 2026-05-24 Phase 3.25 guarded implementation skeleton

### 状态
- **Phase 3.25：** completed as guarded implementation skeleton / default-off / no-op。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_25_guarded_implementation_skeleton`。
- **实现范围：** 只新增 `TrainingOptions` opening-safe 默认关闭字段、inactive summary/report 字段、`ApplyTrainingStep` default-off overload 参数和 selftest inactive equivalence fixture。
- **未实现：** 未实现 objective 公式，未改变 TD target、training loss、eligibility trace、move selection、eval/search 或权重更新语义。
- **CLI：** 默认路径不启用 opening-safe skeleton；只输出 inactive report-only metadata。
- **禁改范围：** 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold。
- **训练/selection：** 未运行训练，未运行 selection，未运行 `GamesPerSeed=64`。
- **新 `.bin`：** Phase 3.25 输出目录未生成 `.bin`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 验证闭环
- 构建验证：`build-vs-release` Release 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest` 通过。
- 自测验证：`surakarta-bitboard-selftest.exe` 通过，新增 `TestOpeningSafeObjectiveDefaultOffAndInactiveEquivalence`。
- default-off 检查：header 默认字段为 disabled/zero/empty；source 未出现 opening objective loss 公式；main 默认路径未启用；selftest 覆盖 default off / inactive equivalence / opening safe。
- 产物检查：Phase 3.25 JSON report 和 manifest 可解析；输出目录 `.bin` 数量为 0。

## 2026-05-24 Phase 3.24 guarded implementation dry-run plan

### 状态
- **Phase 3.24：** completed as design-only / dry-run-plan-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_24_guarded_implementation_dry_run_plan`。
- **生成产物：** `guarded-implementation-dry-run-plan.md`、`guarded-implementation-dry-run-plan.json`、`guarded-implementation-dry-run-plan-manifest.json`。
- **结论：** `allow_phase3_25_guarded_implementation_skeleton=true`，但只允许进入 guarded implementation skeleton；本阶段仍不允许 implementation。
- **dry-run 最小步骤：** preflight hash snapshot -> default-off skeleton tests -> default-off options -> inactive equivalence fixtures -> inactive no-op hook -> scoped activation fixture -> report-only diagnostics -> hard reject fixtures -> dry-run evidence package。
- **Phase 3.25 exact allowed future touch list：** `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`、必要时仅用于 default-off CLI/report 测试的 `scripts\workflow.tests.ps1`，以及 Phase 3.25 输出目录和四个项目文档。
- **forbidden touch list：** `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、selection gate/rating/scorecard/perf threshold、tuple/feature expansion、搜索参数、`.bin` 生成或 promotion、Phase 4。
- **hash 核验：** 每步 before/after hash allowed files；每步 after hash forbidden paths；最终聚合核验 `src\`、`include\`、`tools\` 和三个禁改脚本。Phase 3.24 manifest 已记录本阶段 before/after restricted hashes 一致。
- **default-off 矩阵：** 覆盖 `TrainingOptions` 默认值、CLI 默认值、三个 benchmark/perf 脚本 hash、`ApplyTrainingStep`、`RunBitboardTraining`、诊断禁用、selection summary、manifest gate eligibility。
- **inactive equivalence 矩阵：** 覆盖 `ApplyTrainingStep`、`RunBitboardTraining`、CLI defaults、self-play、eval/search、selection artifacts、report-only companion 隐式调用、forbidden paths。
- **hard reject fixtures：** final `20260423` non-baseline new best、qnodes increase + root over-neutralization、final side-effect signature、任一 final seed new best + qnodes increase、非 opening drift/score regress、C/B/D/fixed score/performance 恶化、default-off/inactive equivalence 失败、companion 被 gate 消费、diagnostic `.bin` promotion、`all_negative` 或 tuple shrinkage-first。
- **report-only companion：** 只允许输出 `hard_reject_triggered` / `hard_reject_reasons` / `selection_result_unchanged` / `gate_not_modified` / `selection_gate_eligible=false`，不得写入 rating、scorecard、perf gate 或 training loss。
- **rollback：** 禁改路径变化、默认开启、inactive equivalence 失败、disabled path 行为变化、gate/rating/scorecard/perf threshold 变化、companion 被消费、hard reject 不能 report-only 表达、非 opening 被影响、`.bin`/训练/selection/64 局出现、Phase 3/4 状态被错误推进，均立即回滚。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **生产代码：** 本阶段未修改 `src\` / `include\`。
- **tools：** 本阶段未修改。
- **禁改脚本：** 本阶段未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Phase 3.25 建议
- 下一步只允许做 **guarded implementation skeleton**：先写 default-off / inactive equivalence skeleton，不训练、不 selection、不生成 `.bin`、不改 gate。别刚拿到骨架许可就往 loss 里塞肉馅，容易包成事故。

## 2026-05-24 Phase 3.23 opening-safe objective implementation plan review

### 状态
- **Phase 3.23：** completed as design-only / implementation-plan-only / report-only implementation plan review。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_23_implementation_plan_review`。
- **生成产物：** `opening-safe-objective-implementation-plan-review.md`、`opening-safe-objective-implementation-plan-review.json`、`opening-safe-objective-implementation-plan-review-manifest.json`。
- **结论：** `allow_phase3_24_guarded_implementation_dry_run_plan=true`，但只允许进入 guarded implementation dry-run plan；本阶段仍不允许 implementation。
- **future implementation 最小允许改动面：** 仅限后续获批时的 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`，必要时 `scripts\workflow.tests.ps1`，以及文档和 Phase 输出目录。
- **禁止面：** 不改 `tools\*`，不改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`，不改 selection gate / rating / scorecard / perf threshold，不改搜索参数，不扩 tuple/features，不生成或推广 `.bin`，不进入 Phase 4。
- **default-off 验证：** future options 必须默认关闭，脚本默认值不得启用 objective；关闭时不得改变 TD target、自博弈走法、eval/search、manifest 兼容性、selection policy 或脚本默认行为。
- **inactive equivalence：** future implementation 必须先证明 `ApplyTrainingStep` / `RunBitboardTraining` / CLI defaults / selection summary 在全关闭时等价；不通过则停止。
- **hard reject companion：** hard reject 条件进入 report-only acceptance companion，只输出 `hard_reject_triggered` / `hard_reject_reasons` 等审计字段，不进入 `overall_rating`、`strength_rating`、`perf_rating`、scorecard、perf gate 或 training loss。
- **hard reject 条件：** final `20260423` non-baseline new best、qnodes increase + root over-neutralization、final side-effect ratio regress、任一 final seed new best + qnodes increase、非 opening drift/score regress、C/B/D 或性能恶化、default-off 或 inactive equivalence 失败、companion 被 gate 消费、diagnostic `.bin` promotion、`all_negative` 或 tuple shrinkage-first。
- **validation order：** before hash -> exact touch list -> failing inactive tests -> default-off skeleton -> inactive equivalence -> scoped activation fixtures -> minimal hook -> hard reject fixtures -> report-only companion proof -> after hash -> 停止在训练/selection/`.bin` 之前。
- **rollback：** 禁改路径变化、默认开启、inactive equivalence 失败、gate/rating/scorecard 变化、companion 被 gate 消费、hard reject 无法 report-only 表达、非 opening 被影响、final `20260423` side-effect 出现或未建模，均必须回滚。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **生产代码：** 本阶段未修改 `src\` / `include\`。
- **tools：** 本阶段未修改。
- **禁改脚本：** 本阶段未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Phase 3.24 建议
- 下一步只允许做 **guarded implementation dry-run plan**：明确默认关闭字段、inactive equivalence 测试、hard reject fixtures、report-only companion 接线证明、禁改路径 before/after hash 与 rollback。
- Phase 3.24 仍不得训练、selection、生成 `.bin`、改 gate 或进入 Phase 4。

## 2026-05-24 Phase 3.22 opening-safe objective minimum prototype design

### 状态
- **Phase 3.22：** completed as design-only / report-only minimum prototype design。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_22_objective_minimum_prototype_design`。
- **生成产物：** `opening-safe-objective-minimum-prototype-design.md`、`opening-safe-objective-minimum-prototype-design.json`、`opening-safe-objective-minimum-prototype-manifest.json`。
- **设计结论：** 允许进入 `Phase 3.23 implementation plan review`，但仍不允许 implementation。
- **最小 objective 目标：** default-off、opening-only、soft penalty，约束 unsafe baseline root-order degradation / costly PV migration。
- **非目标：** 不硬锁 baseline best move，不作为 selection gate，不承诺修复 final，不生成 candidate `.bin`，不替代 32/64 selection。
- **default-off：** 未来配置必须默认关闭；默认路径不得改变 TD target、自博弈走法、eval/search、manifest、selection policy 或脚本默认值。
- **inactive equivalence：** 未来实现前必须证明 `ApplyTrainingStep` / `RunBitboardTraining` 在全关闭时与当前训练路径等价。
- **opening drift penalty 语义：** 只惩罚 unsafe rank degradation / costly drift 的关系变化，不奖励固定 baseline move。
- **hard reject：** final `20260423` non-baseline new best、qnodes increase、root over-neutralization 是硬拒绝条件。
- **companion 位置：** candidate acceptance guard 继续是 selection 后 report-only wrapper；root-cost companion 继续旁路诊断，不进入 gate / loss。
- **tuple shrinkage：** 仍排在 opening drift penalty 之后；Phase 3.5 无 single tuple dominance，Phase 3.9/3.20 已显示 final side-effect 风险，先做 shrinkage 属于拿砂纸修发动机，方向不对。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **生产代码：** 本阶段未修改 `src\` / `include\`。
- **tools：** 本阶段未修改。
- **禁改脚本：** 本阶段未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Phase 3.23 建议
- 下一步仅建议做 **implementation plan review**：列 exact file touch list、default-off option names、inactive equivalence tests、final `20260423` hard reject fixtures、rollback plan。
- Phase 3.23 仍不得训练、selection、生成 `.bin`、改 gate 或进入 Phase 4。

## 2026-05-24 Phase 3.21 opening-safe objective readiness review

### 状态
- **Phase 3.21：** completed as readiness review / design gate only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_21_objective_readiness_review`。
- **生成产物：** `objective-readiness-review.md`、`objective-readiness-review.json`、`objective-readiness-review-manifest.json`。
- **readiness 结论：** `conditionally_ready`。
- **允许进入：** 仅允许进入 `Phase 3.22 opening-safe objective minimum prototype design`，不是实现。
- **Phase 3.20 评审：** root-cost companion 足以支持 readiness review；不足以支持直接修改 training loss。
- **cp2 含义：** `cp2 strong explanation` 支持把未来 objective 设计目标限定为 unsafe opening root-order drift / costly PV migration，而不是硬锁 baseline best move。
- **final 风险：** `final partial explanation` 仍带 `20260423` final side-effect / root over-neutralization 风险，必须作为未来 objective rejection criterion。
- **实施顺序：** candidate acceptance guard companion -> root-cost companion -> opening drift penalty -> distributed tuple shrinkage 仍成立。
- **guard companion：** candidate acceptance guard 必须先作为 report-only wrapper 接好，不进入 rating/scorecard。
- **root-cost companion：** 继续只作为旁路诊断，不进入 selection gate / training loss。
- **opening drift penalty 边界：** 默认关闭、软惩罚 unsafe rank degradation / costly drift，不奖励固定 baseline move。
- **distributed tuple shrinkage：** 仍排在 opening drift penalty 之后，只能后续设计 conservative core / C2-focused stable 小集合，禁止 all_negative。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **生产代码：** 本阶段未修改 `src\` / `include\`。
- **tools：** 本阶段未修改。
- **禁改脚本：** 本阶段未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Blocked 条件
- guard wrapper 不能保持 report-only 或会改 selection summary/rating/scorecard。
- root-cost companion 被接入 selection gate、perf gate 或 training loss。
- 未来 objective 不能默认关闭，或 inactive path 无等价测试。
- final `20260423` side-effect 没有硬拒绝标准。
- 设计试图硬锁 baseline best move、先做 tuple shrinkage、使用 all_negative，或把 diagnostic-neutralized `.bin` 当 candidate。

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


## 2026-05-24 Phase 3.19 report-only guard zero-mutation dry-run

### 状态
- **Phase 3.19：** completed as report-only companion integration dry-run / zero-mutation verification。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation`。
- **生成产物：** `candidate-acceptance-guard-summary.md`、`candidate-acceptance-guard-summary.json`、`candidate-acceptance-guard-manifest.json`、`guard-zero-mutation-report.md`、`guard-zero-mutation-report.json`、`guard-zero-mutation-manifest.json`。
- **wrapper dry-run：** 运行 Phase 3.16 `tools\candidate-acceptance-guard-runner.ps1`，`RunnerOutputDir` 指向 Phase 3.19 输出目录，`GuardOutputDir` 指向既有 Phase 3.13 guard 目录。
- **zero-mutation 结果：** `checked_artifact_count=126`，`changed_artifact_count=0`，`unchanged_artifact_count=126`，`changed_artifacts=[]`。
- **selection/gate：** `selection_result_unchanged=true`，`gate_not_modified=true`；`selection-summary.json`、rating、scorecard、原始 training/selection/guard/runner artifact SHA256 before/after 全部不变。
- **范围边界：** 只写 Phase 3.19 输出目录；未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未修改 gate 阈值。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### Phase 3.20 建议
- 若继续推进，建议进入 **root-cost diagnostic companion dry-run**：继续 report-only，验证 root-cost companion 只新增旁路报告，不写 selection summary/rating/scorecard。
- 仍暂缓训练 loss / opening drift penalty / tuple clipping 实际改造。现在 zero-mutation 只是证明旁路不乱动东西，不是证明训练目标可以开拆，别把门锁试了两下就开始砸承重墙。

## 2026-05-24 Phase 3.18 opening-safe objective pre-implementation plan

### 状态
- **Phase 3.18：** completed as pre-implementation plan only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_18_objective_preimplementation_plan`。
- **生成产物：** `opening-safe-objective-preimplementation-plan.md`、`opening-safe-objective-preimplementation-plan.json`、`opening-safe-objective-preimplementation-manifest.json`。
- **范围边界：** 只做 opening-safe training objective / candidate acceptance companion / root-cost diagnostic 的未来最小实施计划；未实现训练 objective，未修改训练逻辑，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未进入 Phase 4，未修改 gate 阈值。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 推荐实施顺序
1. `candidate acceptance guard companion integration`：让 Phase 3.16 wrapper 成为 selection 后旁路报告，必须保持 `overall_rating` / `strength_rating` / `perf_rating` / scorecard 不变。
2. `root-cost diagnostic acceptance companion`：把 `nodes_ratio`、`qnodes_ratio`、`qnodes_per_node_ratio`、`aspiration_retry_delta` 作为训练后 report-only 诊断证据，不进入 loss。
3. `opening drift penalty prototype`：未来才考虑默认关闭的轻量训练 objective，约束 baseline rank 降级，但不能硬锁 baseline best move。
4. `distributed tuple delta clipping / shrinkage prototype`：最高风险、最后优先级；只允许 conservative core / C2-focused stable 小集合实验设计，禁止 all_negative neutralization，禁止把 neutralized 权重当 candidate。

### Phase 3.19 建议
- 推荐优先做 **report-only companion integration 的最小 wrapper 接入计划或 dry-run**：证明 wrapper 可在 selection 后运行，且 `selection-summary.json` 哈希和所有 rating/scorecard 字段不变。
- 暂缓 opening drift penalty / tuple clipping 的实际代码改造。现在就动 loss 或削 tuple，属于证据链刚搭好就开始拆承重墙，挺刺激，但不靠谱。

## 2026-05-24 Phase 3.17 report-only evidence package consolidation

### 状态
- **Phase 3.17：** completed as report-only evidence package consolidation。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_17_evidence_package`。
- **生成产物：** `phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。
- **范围边界：** 只汇总 Phase 3.1-3.16 既有证据链；不新增诊断逻辑，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不进入 Phase 4，不修改 gate 阈值。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

### 证据包核心结论
- Phase 3.1 selection result 仍为 `regress`：`overall_rating=strength_rating=perf_rating=regress`，scorecard `pass/watch/regress=0/0/3`。
- Phase 3.2-3.3 证明 opening 是唯一硬 fixed-position blocker；cp2 first drift 不是 aspiration retry 导致。
- Phase 3.5 证明 no single tuple dominance；36 个 comparison dominance `yes/watch/no=0/0/36`。
- Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference；`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`，K5/K10 在 `3/3` seed 成立。
- Phase 3.9 对 cp2 first drift/root-order inflation 有强解释；final 只能 partial explanation，`20260423` 有 root over-neutralization / side-effect 风险。
- Phase 3.13 guard 计数固定为 cp2 `explain=6/watch=6/regress=0/unknown=0`，final `explain=0/watch=8/regress=4/unknown=0`。
- Phase 3.16 wrapper 明确 `selection_result_unchanged=true`、`gate_not_modified=true`、`report_only=true`、`selection_gate_eligible=false`。

### 下一步
- 推荐进入 **Phase 3.18：opening-safe objective pre-implementation plan**。
- Phase 3.18 仍应先做计划/设计，不直接修改训练 loss，不直接跑训练或 selection。别把证据包一装订就当通关文牒，这玩意儿是病历本，不是毕业证。

## 2026-05-21 当前总纲任务

### 目标
基于当前仓库真实状态，完成一次全项目复核，更新项目记忆文件，并生成面向“校赛可用 -> 国赛竞争力”的长期棋力优化技术路线图。输出必须区分已完成能力、当前阻塞、短期校赛交付边界和长期国赛优化路径。

### 当前阶段定位
- **短期版本：** 校赛可用 GUI 人机对战版本，以 `bitboard + fixed-depth PVS + 多线程根搜索 + D10/T10 Match preset` 为核心。
- **证据链现状：** bitboard / dev-session / Pester / CTest / GUI smoke / P0 golden parity 都已经有了；manual acceptance 已在 `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321` 形成可追溯封口证据，`result.txt` 为 `PASS`，必需截图齐全。
- **长期主线：** 国赛棋力提升路线已完成阶段 B1（大赛终局语义与规则完整性）和阶段 B2（可信基线重建）。B2.1 单次 post-B1 perf baseline candidate 已证明旧 baseline 失效但计时基线偏瘦；B2.1b 多样本保守聚合 candidate 已连续 2 次通过候选 gate；B2.2 已用该 aggregate candidate 正式替换 `test/test_data/perf-baseline.json`，并连续 2 次通过默认正式 perf gate。可以进入 Phase 3 post-B1 fresh trend validation 准备；Phase 3 仍未通过，Phase 4 特征扩展继续 blocked。
- **核心阻塞：** 训练/selection 趋势未稳定，历史 Phase 3 仍是 `implementation complete, trend validation failed`；P0 capture-rule 已修复并加固，但 P0 之后需要重新建立可信训练基线。
- **Phase 3.1 最新结果：** fresh post-B1 `GamesPerSeed=32` selection 已完成，run root 为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`；`overall_rating=strength_rating=perf_rating=regress`，scorecard `pass/watch/regress=0/0/3`，C/B/D=`12/16/2`，`score_regressions=1`，`best_move_changes=3`，平均/最差性能比 `1.622708/1.864728`。按 gate 立即停止，不跑 `GamesPerSeed=64`，Phase 3 继续 failed，Phase 4 继续 blocked。
- **Phase 3.2 最新归因：** opening 是唯一硬 fixed-position blocker。checkpoint-2 起三个 seed 均出现 opening best move drift 和 search timing 超阈，node inflation 在 checkpoint-4/6 跟随出现；最终三 seed 收敛到 `(3, 1) -> (2, 2) (B)`，但最终 move 不是最早 drift。`20260423` 的 final score regression 是晚期才出现，`20260424/20260425` final score_delta 为正仍 regress，说明 strength/perf blocker 耦合但不可互相抵消。下一步应做 opening-specific 诊断/最小 ablation，不直接调参数，不进入 Phase 4。
- **Phase 3.3 最新诊断：** opening root diagnostics 已完成，产物在 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics`。现有 CLI 支持 `--search-diagnostics`，但不提供 active tuple / weight attribution。cp2 首次 drift 三 seed 完全同型：`(5, 1) -> (4, 2) (B)` 变成 depth-4 root order rank 1，baseline `(0, 1) -> (1, 2) (B)` 退到 order rank 2；observed score 同为 rank 1/bound-tie，非明确全宽 score 压低。qnodes 主要集中到新的 PV root move，cp2 无 aspiration retry，因此首次 drift / qnode inflation 不能归因于 retry。后期最终 move `(3, 1) -> (2, 2) (B)` 跨 seed 收敛，final candidate 中该 move 均为 order rank 1，baseline 仍是 score-rank 1 的 bound-tie但成为便宜的 non-PV 子树。下一步只支持 report-only diagnostic extension / 最小 ablation 设计，不支持直接进入 Phase 4。
- **Phase 3.4 最新设计：** opening active-tuple attribution and minimal ablation design 已完成为可行性分析；未重新训练、未跑 64、未进入 Phase 4、未改 `src` / `include` / selection gate / 搜索参数 / 三个 benchmark/perf 脚本。本轮接手复核确认实际 cp2 权重 header 为 `magic=0x5455504C`、`tuple_set_hash=0x1D4A6EBF2BAF6484`、`weight_count=16605`，且 3 seed 的 `bootstrap/cp2/final` 目标权重均可由 manifest 追溯。结论：现有 CLI 单独不能输出 active tuple attribution，但权重格式与 tuple 编码足够简单，可以用非生产离线小脚本完成 27 行 summary + 1215 行 tuple detail 的 opening root child static attribution；若要避免脚本复制 tuple descriptor，可在下一阶段选择 report-only `bitboard-attrib` / `bitboard-eval --explain-tuples` 扩展，默认关闭且不改变搜索、训练、selection 行为。推荐最小 ablation 顺序为 attribution-only -> child-position static ablation；只有少数 tuple 明确主导时，才考虑复制权重到临时文件做 offline neutralization 并只跑 opening diagnostics。
- **Phase 3.5 最新执行：** opening attribution-only 离线诊断已完成，脚本为 `D:\Dev\Code\surakarta_traditional\tools\opening-attribution.ps1`，输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution`。脚本从 `weights-manifest.json` 解析 3 seed × `bootstrap/cp2/final` 共 9 个 artifact；所有 artifact 文件存在、SHA256 pass、header pass，tuple hash `0x1D4A6EBF2BAF6484`、weight count `16605`、active tuple count `45`。输出 summary `27` 行，tuple detail `1215` 行，top-deltas `1440` 行。static eval delta 支持解释 drift 方向：cp2 阶段三 seed 的 cp2 drift child 均比 baseline child 更偏向 Black（`-10 -> -23`，delta `-13`）；final 阶段 final drift child 相对 baseline child delta 分别为 `-105/-138/-150`。未发现少数 tuple 主导，36 个 comparison 中 dominance `yes/watch/no=0/0/36`，max top1/top3 abs share `0.165094/0.376866`。结论：足够进入 child-position static ablation 设计；不支持 offline neutralization，不进入 Phase 4。
- **Phase 3.6 最新设计：** child-position static ablation design 已完成为文档设计；未执行 ablation、未新增脚本、未生成权重、未重新训练、未跑 `GamesPerSeed=64`、未进入 Phase 4。设计要求只在内存/表格层 neutralize stage-vs-bootstrap 的 induced pair tuple deltas：主 comparison 是 cp2 onset `baseline_child -> cp2_drift_child` 和 final gate blocker `baseline_child -> final_drift_child`，辅以 final residual `baseline_child -> cp2_drift_child`、final transition `cp2_drift_child -> final_drift_child`、bootstrap/cp2 negative controls。TopK 采用 `1/3/5/10/20/all_negative` 累积曲线，不直接抓 top1；原因是 Phase 3.5 dominance 全 `no`，top1/top3 abs share 最高只有 `0.165094/0.376866`，这不是“一刀切一个 tuple”的局面。当前仍不允许 offline neutralization；只有静态 ablation 显示小而稳定的 tuple 集合跨 seed 抹平 cp2/final drift 偏好，才允许另行设计 offline neutralization。
- **Phase 3.7 最新执行：** child-position static ablation report-only/table-only 已完成，新增非生产只读脚本 `D:\Dev\Code\surakarta_traditional\tools\child-position-static-ablation.ps1`，输出目录 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`。输出 summary `7` 行、selected tuple detail `174` 行、TopK sweep `126` 行，manifest 保留 attribution manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`、tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`、tuple set hash `0x1D4A6EBF2BAF6484`、weight count `16605`、git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`：K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring；controls 全部 pass。这足够进入 offline neutralization **设计评审**，但不得直接执行 neutralization、不得写权重、不得跑 search/training/64、不得进入 Phase 4。Phase 3 仍未通过，Phase 4 仍 blocked。
- **Phase 3.8 最新设计评审：** offline neutralization design review 已完成为纯文档设计；本轮 `weights_written=false`、`neutralization_executed=false`、`offline_neutralization_execution_allowed=false`。Phase 3.7 的 `C1`/`C2` success 满足进入设计评审条件，但只允许下一步进入 Phase 3.9 offline diagnostics execution，不允许替代 selection gate。推荐 neutralization variants：conservative core `5,10,32,41`（解释 cp2 onset + final drift 的共享核心）、C1-only K10 `4,5,6,10,11,27,32,36,38,41`（只解释 cp2 onset）、C2-focused stable K10 `5,10,32,41,43`（解释 final gate blocker）、optional union K10 `4,5,6,10,11,27,32,36,38,41,43`（riskier，只做压力对照）。C2 2/3 support set `0,5,6,10,11,32,33,37,41,43,44` 只作为解释/审计候选，不直接当首选写权重集合。Phase 3 仍未通过，Phase 4 仍 blocked。
- **Phase 3.9 最新执行：** offline neutralization diagnostics execution 已完成为 report-only diagnostics，脚本为 `D:\Dev\Code\surakarta_traditional\tools\offline-neutralization-diagnostics.ps1`，输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization`。脚本从 `weights-manifest.json` 解析 cp2=`checkpoint`/`games_completed=2` 与 final=`candidate` artifact，并按 `selection-summary.json` 校验 Phase 3.1 参数；所有源权重文件存在、SHA256/header/tuple hash/weight count/长度通过。产物包括 `neutralization-manifest.json`、`neutralization-summary.csv`、`neutralization-root-diagnostics.csv`、`neutralization-weight-deltas.csv`、24 个临时 `diagnostic-neutralized.bin`、30 个 per-run diagnostics JSON。cp2 阶段四个 variant 均在 3/3 seed 将 best move 从 `(5, 1) -> (4, 2) (B)` 拉回 baseline `(0, 1) -> (1, 2) (B)`，baseline root order `2->1`，nodes/qnodes 下降，足够解释 cp2 first drift/root order inflation。final 阶段只支持 partial root-cost explanation：`20260425` 成本明显下降，但 `20260423` 被推到新 best move `(4, 1) -> (3, 2) (B)` 且 qnodes 上升，属于 over-neutralization/side-effect 信号；不能写成修复，不能替代 selection gate。Phase 3 仍未通过；Phase 4 仍 blocked。
- **Phase 3.10 最新总结：** report-only root-order/root-cost explanation summary 已完成为文档更新；未训练、未跑 selection、未跑 `GamesPerSeed=64`、未新增 neutralized `.bin`、未修改生产代码或三个 benchmark/perf 脚本。证据链结论是：Phase 3.1 selection gate 仍为 `regress`；Phase 3.2-3.3 已把唯一硬 blocker 定位到 opening first drift/root order inflation；Phase 3.5 排除 single tuple dominance；Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference；Phase 3.9 证明 cp2 first drift/root order inflation 可被 offline neutralization 强解释。final candidate 只能写 partial explanation：`20260425` root cost 下降，但 `20260423` 出现 1/3 root over-neutralization、新 best move `(4, 1) -> (3, 2) (B)` 和 qnodes 上升。conservative_core / c2_focused_stable_k10 风险低于 c1_only_k10 / union_k10_riskier；后两者出现 static over-neutralization 或更大 tuple 覆盖，只能做压力对照。下一步建议 Phase 3.11 design-only：opening-safe training objective / root-order regularization / candidate acceptance guard；不推荐直接扩 N-Tuple、直接 all_negative neutralization、直接 rerun 64 games、调 gate 阈值、或把 neutralized 权重当 candidate。Phase 3 仍未通过；Phase 4 仍 blocked。
- **Phase 3.11 最新设计：** design-only 已完成；只更新文档，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未修改 production code / `tools\` / 三个 benchmark/perf 脚本。设计覆盖三类候选方向：`candidate acceptance guard`、`opening-safe training objective`、`root-order regularization`。推荐顺序是先做 design/report-only guard，把 cp2 first drift、baseline root order 降级、root cost inflation、final new best move 副作用写成候选接受前的诊断桥；再考虑最小训练目标约束，限制 opening root drift-favoring distributed tuple deltas；最后才考虑生产搜索或特征扩展。opening-safe objective 和 root-order regularization 都可能影响 selection gate，但必须先经 Phase 3.12 design-to-diagnostic bridge 验证，不得直接实现训练改造。仍不推荐直接扩 N-Tuple / threat / mobility / stage 特征、直接 all_negative neutralization、直接重训并跑 64、调 selection gate 阈值、或把 Phase 3.9 neutralized 权重当 production candidate。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.12 最新设计桥：** report-only opening guard diagnostic spec 已完成为文档设计；只更新四个文档，未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未新增诊断脚本、未进入 Phase 4。Phase 3.12 定义下一步 diagnostics 如何消费 `weights-manifest.json`、Phase 3.3 opening diagnostics、Phase 3.7 static ablation summary/sweep、Phase 3.9 neutralization summary/root diagnostics，并建议输出 `opening-guard-spec.md` 或文档章节、`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。guard 指标覆盖 opening best-move、root-order、root-cost、static-drift、final-candidate side-effect；状态语义只有 `explain` / `watch` / `regress` / `unknown`，且全部是 report-only attribution，不参与 `overall_rating` / `strength_rating` / `perf_rating`，不得替代 pass/watch/regress gate。下一步 Phase 3.13 推荐优先 B：实现 report-only opening guard diagnostics 脚本；A 仍可继续 design-only 设计 opening-safe training objective 最小改造点。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.13-B 最新执行：** report-only opening guard diagnostics 脚本已实现为 `tools\opening-guard-diagnostics.ps1`，输出目录为 `build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`。产物包括 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`；metrics 共 `24` 行，覆盖 3 seed × cp2/final × 4 neutralization variant，字段包含 `seed/stage/artifact_role/variant/baseline_best_move/candidate_best_move/best_move_status/baseline_rank/candidate_rank/root_order_status/nodes_ratio/qnodes_ratio/qnodes_per_node_ratio/aspiration_retry_delta/static_support_status/over_neutralization_status/guard_status/explanation_scope`。cp2 guard status 为 `explain=6/watch=6/regress=0/unknown=0`，确认 conservative_core 与 c2_focused_stable_k10 强解释 cp2 first drift/root-order inflation，c1_only_k10/union 因 static over-neutralization 标 watch；final guard status 为 `explain=0/watch=8/regress=4/unknown=0`，确认 final 只能 partial explanation，`20260423` 四个 variant 均因 non-baseline new best move/root over-neutralization 记 regress。所有 variant 聚合 guard_status 均为 `regress`，这是 report-only 风险摘要，不是 selection gate。本轮未修改 `src\` / `include\`，未修改三个 benchmark/perf 脚本，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate、未进入 Phase 4。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.14 最新设计：** opening-safe training objective 最小改造方案已完成为 design-only 文档更新；只更新四个文档，未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate、未进入 Phase 4。基于 Phase 3.13-B guard 结果，推荐顺序为：1) candidate acceptance guard integration，先把 opening guard 作为 candidate acceptance 的 report-only companion；2) root-cost regularization as diagnostic，把 nodes/qnodes/qnodes_per_node inflation 放进训练后验收诊断，不直接进 loss；3) opening drift penalty，对已知 opening root case 的 baseline rank 降级施加轻量惩罚但绝不硬锁 baseline move；4) distributed tuple delta clipping / shrinkage，只对 Phase 3.7/3.9 证明的 distributed drift tuple set 做最小 shrinkage，明确不能 all_negative neutralization。Phase 3.15 推荐 A：先实现 candidate acceptance guard integration 的 report-only hook/spec；暂缓 B：直接改训练 loss。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.15-A 最新设计：** candidate acceptance guard integration 的 report-only hook/spec 已完成为文档设计；只更新四个文档，未实现 hook，未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate。hook 位置定义为 candidate 生成后、selection gate 写出 `selection-summary.json/txt` 后，或最终 summary 汇总阶段的旁路 runner；它只读取 `RunRoot`、`weights-manifest.json`、opening diagnostics root、`tools\opening-guard-diagnostics.ps1`、selection summary / scorecard artifact，并输出 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`、可选 `candidate-acceptance-guard-summary.md`。guard 状态 `explain/watch/regress/unknown` 只作为 report-only evidence，不改变 `overall_rating`、`strength_rating`、`perf_rating` 或 scorecard `pass/watch/regress`。Phase 3.16 推荐实现 report-only hook runner/wrapper，但仍不改 selection gate；继续暂缓训练 loss/objective 改造。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.16 最新执行：** report-only candidate acceptance guard runner/wrapper 已完成。新增非生产脚本 `D:\Dev\Code\surakarta_traditional\tools\candidate-acceptance-guard-runner.ps1`，只在 selection 已完成后读取 `selection-summary.json/txt`、既有 Phase 3.13 opening guard 产物和 `tools\opening-guard-diagnostics.ps1`；本次复用既有 `phase3_13_opening_guard`，未覆盖原始 guard 产物。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_16_guard_runner`，生成 `candidate-acceptance-guard-summary.md/json` 与 `candidate-acceptance-guard-manifest.json`。summary/manifest 明确 `report_only=true`、`selection_gate_eligible=false`、`production_candidate=false`、`selection_result_unchanged=true`、`gate_not_modified=true`、`phase3_status=not_passed`、`phase4_status=blocked`。cp2 guard 计数为 `explain=6/watch=6/regress=0/unknown=0`，final 为 `explain=0/watch=8/regress=4/unknown=0`；四个 variant 聚合仍均为 `regress`。本轮未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.17 最新执行：** report-only evidence package consolidation 已完成。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_17_evidence_package`，生成 `phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。manifest 记录输入文件路径与 SHA256、输出文件路径与 SHA256、当前 git commit、`generated_at`，并声明 `report_only=true`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。本轮只汇总 Phase 3.1-3.16 关键证据链；未新增诊断逻辑，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未进入 Phase 4，未修改 gate 阈值，未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本。下一步建议 Phase 3.18：opening-safe objective pre-implementation plan，仍先做计划，不直接改训练 loss。Phase 3 still not passed；Phase 4 still blocked。
- **Phase 3.18 最新执行：** opening-safe objective pre-implementation plan 已完成。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_18_objective_preimplementation_plan`，生成 `opening-safe-objective-preimplementation-plan.md`、`opening-safe-objective-preimplementation-plan.json`、`opening-safe-objective-preimplementation-manifest.json`。计划拆解四个未来方向：candidate acceptance guard companion integration、root-cost diagnostic acceptance companion、opening drift penalty prototype、distributed tuple delta clipping / shrinkage prototype；其中前两项优先 report-only，后两项暂缓且必须默认关闭、严格测试、可回滚。本轮未实现训练 objective，未修改训练逻辑，未训练、未 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未进 Phase 4、未改 gate、未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本。Phase 3.19 推荐优先做 report-only companion integration 的最小 wrapper 接入计划或 dry-run，暂缓 opening drift penalty / tuple clipping 实际代码改造。Phase 3 still not passed；Phase 4 still blocked。
- **硬边界：** 不把 pre-fix / suspect training artifacts 当作强度证明；不在规则语义和训练趋势没站稳前扩 N-Tuple / threat / mobility / stage 特征；不为了好看放宽 selection gate。

### 本轮执行清单
- [x] 读取并复核 `README.md`、`docs/school_competition_defense.md`、根目录记忆文件和近期 git 状态
- [x] 复核核心源码模块：bitboard movegen、search、training/eval CLI、GUI/session、CTest/Pester workflow
- [x] 运行代表性自动检查：bitboard selftest、dev-session selftest、Pester workflow、CTest
- [x] 更新 `findings.md`：记录当前项目现状、风险、路线图判断和验证结果
- [x] 更新 `progress.md`：记录本轮检查过程与测试结果
- [x] 生成 `docs/national_competition_technical_roadmap.md`
- [x] 更新本文件为当前路线规划入口

### 国赛路线阶段

#### 阶段 A：校赛冻结与规则底座再验收
- [x] 保持校赛版本只做必要风险修复，不在赛前混入训练/搜索大改
- [x] 补齐 manual acceptance 证据：归档 `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321`，覆盖默认 D10/T10 GUI、人机对战普通移动、加载局面、legal/illegal target、capture gate、white capture path、`CHECKMATE`、legacy `STALEMATE`。
- [x] 阶段 A 校赛冻结证据链封口：`checklist.md` 已填写，`result.txt` 为 `PASS`，必需截图 `opening-move.png`、`game1-targets.png`、`game6-checkmate.png`、`game6-stalemate.png` 齐全，并保留场景 3/4 额外截图。
- [x] 2026-05-22 接手复核：manual acceptance 归档断言通过，缺失文件为 0，`result.txt` 为 `PASS` 且无 `PENDING`，`checklist.md` 无空 `Actual` / `Conclusion`，必需和旁证截图均存在且非空；源码、脚本、GUI 无本轮 diff。
- [x] 将大赛第 6 条“双方都不能再吃子”与 legacy `no_capture_ply` 语义拆开验收；阶段 B1 已完成代码修复与验证。
- [x] 终局语义修复已先补 RED tests，再修改 traditional/bitboard/search/dev-session 路径。

#### 阶段 B1：大赛终局语义与规则完整性
- [x] traditional `JudgeEnd` 不再用 `num_round_ - last_captured_round_ >= max_no_capture_round_` 判大赛终局。
- [x] bitboard `EvaluateBitboardTerminal` 不再用 `position.no_capture_ply > position.max_no_capture_round` 判大赛终局。
- [x] search `EvaluateTerminal` 不再用 `position.no_capture_ply >= position.max_no_capture_round` 判大赛终局。
- [x] 新增共享 bitboard helper：`HasAnyCaptureMove(position, color)` 与 `IsNationalStalemateTerminal(position)`。
- [x] 终局语义：一方 0 子 => `CHECKMATE`，双方不能 capture 且已进入非满子材料状态 => `STALEMATE`，winner 按剩余棋子数，多者胜，平子为 `SurakartaPlayer::NONE`。
- [x] legacy `no_capture_ply` / `max_no_capture_round` 保留为状态字段，用于 make/unmake、训练状态和兼容序列化；不再作为 national terminal gate。
- [x] `game6.txt` 保留为历史 manual acceptance / checkmate 和 perf endgame fixture，不再作为大赛第 6 条“双方都不能再吃子”的严格证明；B1 改用代码内 national-rule golden positions。
- [x] 验证通过：`surakarta-bitboard-selftest`、`surakarta-dev-session-selftest`、Pester workflow、CTest。
- **状态：** completed；可以进入阶段 B2。

#### 阶段 B2：可信基线重建
- [x] B1 后默认回归复核：`workflow.ps1` Debug/Release 均因 GitHub `glfw` FetchContent 网络问题在 configure 阶段失败；降级使用 B1 core build 验证 selftest / CTest / Pester。
- [x] 将本阶段 workflow fallback、perf、training smoke、selection smoke 产物统一记录 git commit、命令、结果、状态；training / selection 权重 manifest 均带 seed、参数、hash。
- [x] B1 后 perf baseline gate 已运行并分类：当前 baseline 是 pre-B1 legacy terminal 语义，`perf-runner.ps1` fail 属于 B1 合理语义变化导致旧 baseline 不可信；未更新 `test/test_data/perf-baseline.json`。
- [x] B1 后 TD/self-play smoke 已通过，candidate/bootstrap/checkpoint manifest 可追溯。
- [x] Selection 入口仅做 `GamesPerSeed=2` smoke；结果不作为 Phase 3 趋势通过证据。
- [x] B2.1 post-B1 perf baseline candidate 已生成：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\perf-baseline.candidate.json`，来源 run 为 `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`，矩阵完整 27 条。
- [x] B2.1 candidate 连续复跑 2 次：`20260522-151729` fail 1 个 timing-only `performance_regression`；`20260522-151913` fail 4 个 timing-only `performance_regression`；0 个 best move drift，0 个 score drift，endgame 不再被旧 early terminal 节点数误杀。
- [x] B2.1b 多样本保守聚合 candidate 已生成：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json`；输入包含 `20260522-145419`、`20260522-151729`、`20260522-151913`、`20260522-163919`、`20260522-163943`、`20260522-164009`、`20260522-164037`、`20260522-164101` 共 8 次 perf summary。
- [x] B2.1b 聚合策略：27 个 case/depth/thread 全部要求 `best_move` / `score` 跨样本一致；`search_seconds=max`，`nodes=max`，`qnodes/TT/剪枝计数=max`，`nps=min`；`aggregate-summary.txt` 记录每项 min/median/max timing、min/max nodes、稳定性和 timing-sensitive / tree-variable 分类。
- [x] B2.1b aggregate candidate 连续复跑 2 次：`20260522-164507` pass，`20260522-164532` pass；两次均 `failure_count=0`、`best_move_drift=0`、`score_drift=0`；最大 node ratio `1.057175`，最大 search ratio `1.220779`，未见系统性节点超阈。
- [x] B2.2 正式 baseline 替换与 gate 复验：completed。已用 `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json` 替换 `test/test_data/perf-baseline.json`；替换后默认 `pwsh ./scripts/perf-runner.ps1` 连续 2 次 pass，均确认使用正式 baseline 文件。
- [ ] GUI preset calibration 另行执行。
- [ ] 建立固定局面集 v2：opening、capture-heavy、threat-heavy、loop-path、endgame、terminal-semantics
- **状态：** completed through B2.2 official baseline replacement；Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。

#### 阶段 C：训练稳定性恢复

##### Phase 3 post-B1 fresh trend validation 准备（2026-05-22）
- [x] 复核 B2.2 正式 baseline：`test/test_data/perf-baseline.json` 与 aggregate candidate SHA256 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`；两次默认 `perf-runner.ps1` gate 已通过。
- [x] 复核 selection / training 默认参数：Seeds `20260423,20260424,20260425`，`GamesPerSeed=32`，`Depth=4`，`CheckpointEvery=2`，`Alpha=0.005`，`Lambda=0.5`，`Epsilon=0.02`，`EpsilonPlies=6`，`TerminalReward=1200`，`TdErrorClip=0`，`TerminalOnlyWarmup=0`，`NearTerminalCurriculum=0`。
- [x] 复核 `build-vs-train-release\bin\surakarta-benchmark.exe`：文件存在，可启动并输出 CLI usage；本阶段未重建。
- [x] 复核可用训练产物：`20260522-145835` 是 `benchmark-train.ps1` smoke / reproducibility 产物；`20260522-150225` 是 `GamesPerSeed=2` selection smoke，`overall_rating=regress`、`strength_rating=watch`、`perf_rating=regress`，只能证明入口和 artifact 链路可用，不能作为 Phase 3 强度证据。
- [x] 封口正式验证路线：不再补跑更小规模预检；下一步经用户确认后直接跑 fresh post-B1 `GamesPerSeed=32` selection。若 32 局 `overall_rating=regress`，立即停止，不跑 64；若 32 局至少 `watch`，才允许进入 `GamesPerSeed=64`。
- **准备状态：** completed；只完成方案封口，尚未运行正式 `GamesPerSeed=32/64` trend validation。
- **正式 build：** `D:\Dev\Code\surakarta_traditional\build-vs-train-release`。
- **Phase 3 watch/pass 口径：** `GamesPerSeed=32` 至少 `watch` 只允许进入 64，不等于 Phase 3 pass；`GamesPerSeed=64` 不得 `regress`，才允许讨论 Phase 3 从 failed 升级为 watch/pass，并讨论 Phase 4 是否仍 blocked。
- **继续 blocked 口径：** 任一正式 32/64 结果出现 `overall_rating=regress`、`strength_rating=regress`、`perf_rating=regress`、固定局面负向 score regression、无法解释的 opening best move drift / search-tree inflation、manifest/hash/参数不可追溯、或混入 pre-B1/suspect/provisional artifacts，Phase 3 保持 failed，Phase 4 继续 blocked。
- [x] Phase 3.1 fresh post-B1 `GamesPerSeed=32` selection 已运行：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`，结果 `overall_rating=regress`，按停止条件不跑 64。
- [ ] 只在 fresh post-fix artifact 上重跑 GamesPerSeed=64 selection（当前 blocked，需用户另行确认且 32 局至少 watch；本次不满足）
- [ ] 先修复或解释 opening search-tree inflation，再谈 Phase 4
- [ ] 训练指标必须同时看 strength、perf、best move drift、score regression、TD error、weight delta
- [ ] Phase 4 解除条件仍为：GamesPerSeed=32 至少 `watch`，GamesPerSeed=64 不 `regress`
- [x] Phase 3.9 offline neutralization diagnostics 已执行；结果只能进入 report-only root-order/root-cost explanation 总结，不能解锁 Phase 4，不能替代 selection gate
- [x] Phase 3.10 report-only root-order/root-cost explanation summary 已完成；只更新文档，不新增训练/selection/权重，不进入 Phase 4
- [x] Phase 3.11 design-only：设计 opening-safe training objective / root-order regularization / candidate acceptance guard；只更新文档，不训练、不 selection、不进入 Phase 4
- [x] Phase 3.12 design-to-diagnostic bridge：把 Phase 3.11 设计转成 report-only diagnostic spec，不直接实现训练改造
- [x] Phase 3.13-B report-only opening guard diagnostics 脚本已实现；仍不进入 Phase 4，不训练、不 selection、不生成 `.bin`
- [x] Phase 3.14 design-only：基于 opening guard report 设计 opening-safe training objective 最小改造方案；只更新文档，不训练、不 selection、不生成 `.bin`
- [x] Phase 3.15-A design-only：candidate acceptance guard integration report-only hook/spec 已完成；不实现 hook，不改 selection gate
- [ ] Phase 3.16 推荐：实现 report-only hook runner/wrapper；仍不改 selection gate，暂缓训练 loss/objective 改造

##### Phase 3.15-A candidate acceptance guard integration report-only hook/spec（2026-05-24）
- **状态：** completed as design-only hook/spec；不是实现，不是训练，不是 selection，不是 gate 改造。
- **本轮边界：** 只更新四个文档；仅读取 `tools\opening-guard-diagnostics.ps1` 与 `scripts\benchmark-select.ps1` / `scripts\benchmark-train.ps1` / `scripts\perf-runner.ps1` 的接口；未修改 `src\` / `include\` / `tools\` / 三个 benchmark/perf 脚本；未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`。
- **Hook 位置：** 推荐在 `benchmark-select.ps1` 完成 candidate 生成、评估、scorecard 聚合并写出 `selection-summary.json` / `selection-summary.txt` 之后，由外层 wrapper 或后处理 runner 旁路运行 opening guard。备选位置是最终 summary 汇总阶段追加 companion summary。不得插入 `Get-SelectionScorecard`、`Get-SelectionAggregateRating`、`overall_rating`、`strength_rating`、`perf_rating` 或 scorecard pass/watch/regress 计算路径。
- **输入 contract：** `RunRoot` 指向 selection session root；`weights-manifest.json` 用于 artifact/seed/stage/hash 追溯；opening diagnostics root 指向 `RunRoot\opening-diagnostics`；Phase 3.13 script 为 `tools\opening-guard-diagnostics.ps1`；selection summary / scorecard artifact 为 `selection-summary.json` / `selection-summary.txt`，只读，用来在 companion report 中引用主 selection 结果。
- **最小命令示例：** `pwsh -NoProfile -ExecutionPolicy Bypass -File .\tools\opening-guard-diagnostics.ps1 -RunRoot <run-root> -OutputDir <guard-output-dir>`。
- **输出 contract：** 必需输出 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`；可选输出 `candidate-acceptance-guard-summary.md`，用于把 selection result 与 guard report 并列展示。输出目录建议为 `<run-root>\opening-diagnostics\candidate_acceptance_guard` 或 `<run-root>\opening-diagnostics\phase3_16_candidate_acceptance_guard`，不得覆盖 selection summary。
- **状态语义：** `explain` 表示 guard 现象可解释；`watch` 表示风险或证据不足；`regress` 表示 report-only 风险 note；`unknown` 表示 artifact 缺失或不可判定。这四个状态只用于 evidence，不改变 `overall_rating`、`strength_rating`、`perf_rating`、seed scorecard rating、或 scorecard `pass/watch/regress` 计数。
- **Failure behavior：** guard artifact 缺失时 selection result 保持原样，companion `guard_status=unknown`；guard 脚本失败时不得 retroactively 改写主 selection，wrapper 只能记录 `guard_execution_status=failed`；manifest 不合规时 guard report invalid，不能作为证据；final `regress` 只能写入 risk note，不得自动 reject candidate。
- **Traceability：** manifest 必须记录 commit、input hash、output hash、script path、command line、`generated_at`、`RunRoot`、selection summary hash、weights manifest hash，并继续声明 `production_candidate=false`、`selection_gate_eligible=false`、`report_only=true`、`phase3_status=not_passed`、`phase4_status=blocked`。
- **禁止事项：** 不把 opening guard 写成 blocking gate；不修改 gate 阈值；不把 guard result 回写 `selection-summary.json` 的 rating 字段；不把 final `regress` 自动当 candidate reject；不把 missing guard artifact 当 selection failure。
- **Phase 3.16 建议：** 推荐实现 report-only hook runner/wrapper，只在 selection 后生成附加报告并校验 traceability；暂缓训练 loss/objective 改造。
- **判定：** opening guard integration 是旁路 report-only companion；Phase 3 still not passed；Phase 4 still blocked。

##### Phase 3.14 opening-safe training objective minimal design（2026-05-24）
- **状态：** completed as design-only；不是实现，不是训练，不是 selection，不是 Phase 4。
- **输入证据：** Phase 3.13-B `opening-guard-metrics.csv` / `opening-guard-summary.json` / `opening-guard-manifest.json`；Phase 3.9 `neutralization-summary.csv` / `neutralization-root-diagnostics.csv`；Phase 3.7 `opening-child-static-ablation-summary.csv` / `opening-child-static-ablation-sweep.csv`。
- **证据摘要：** Phase 3.13-B metrics 共 `24` 行；cp2 为 `explain=6/watch=6/regress=0/unknown=0`，final 为 `explain=0/watch=8/regress=4/unknown=0`。cp2 first drift/root-order inflation 可强解释；final 只能 partial explanation，`20260423` 四个 variant 都有 root over-neutralization / non-baseline new best move。
- **推荐顺序 1：candidate acceptance guard integration。** 目标是把 Phase 3.13 opening guard 接成 candidate acceptance 的 report-only companion，输入为 guard metrics/summary/manifest 与 selection summary；改造点是验收报告层 hook/spec，不改变现有 pass/watch/regress gate；最小实现范围是报告聚合、manifest 声明和 CI/本地命令说明；风险是被误读为新 gate，验证方式是确认 `overall_rating` / `strength_rating` / `perf_rating` 不被改写。短期不需要生产代码变更。
- **推荐顺序 2：root-cost regularization as diagnostic。** 目标是约束 opening root-order inflation 伴随的 `nodes_ratio` / `qnodes_ratio` / `qnodes_per_node_ratio` 膨胀，输入为 Phase 3.13 root-cost 字段和 Phase 3.3/3.9 root diagnostics；改造点是训练后 candidate acceptance diagnostic，不建议直接进 loss；最小实现范围是阈值口径、watch/regress 归因和趋势表；风险是把搜索成本噪声误当训练目标，验证方式是复跑 report-only guard 并检查 cost signal 是否随 candidate 稳定。短期不需要生产代码变更，若未来接 pipeline 可能只动诊断/脚本层。
- **推荐顺序 3：opening drift penalty。** 目标是轻量惩罚已知 opening root case 中 baseline move rank 从 rank 1 降级、drift move 夺 rank 1 且不可解释的情形；输入为 Phase 3.13 rank/status、Phase 3.7 distributed tuple support 和 Phase 3.9 cp2/final side-effect；改造点是训练 objective 或训练后候选评分的轻量 penalty。最小实现范围应只覆盖固定 opening guard cases，惩罚 rank degradation / costly PV drift，不硬锁 baseline best move；风险是过拟合 opening case 或把 baseline move 写成教条；验证方式是 cp2 drift 降低、final 不出现 new best side-effect、非 opening fixed positions 不退化。未来若真正进 objective 需要生产训练代码变更。
- **推荐顺序 4：distributed tuple delta clipping / shrinkage。** 目标是对 Phase 3.7/3.9 证明的 distributed drift tuple set 做最小 clipping/shrinkage，输入为 K5/K10 tuple support、neutralization variants 与 root side-effect；改造点是训练权重更新后的 post-update shrinkage 或 objective-local clipping。最小实现范围只允许 conservative core / C2-focused stable 这类小集合先做实验设计，不允许 all_negative neutralization；风险最高，可能削弱真实棋力或制造 final root over-neutralization；验证方式是 static support 仍成立、cp2 root order 恢复、final `20260423` 不再漂到 non-baseline new best。未来需要生产训练/权重更新代码变更。
- **明确不推荐：** 不直接 all_negative neutralization；不直接扩 N-Tuple / threat / mobility / stage；不直接调 gate 阈值；不直接跑 `GamesPerSeed=64`；不直接把 Phase 3.9 neutralized 权重当 candidate。
- **Phase 3.15 建议：** 推荐 A，先实现 candidate acceptance guard integration 的 report-only hook/spec；暂缓 B 直接改训练 loss。理由是 guard 证据链刚稳定，先把它挂到候选验收旁路能防止诊断、训练和 selection 搅成一锅粥。
- **判定：** opening guard 不是 selection gate；Phase 3 still not passed；Phase 4 still blocked。

##### Phase 3.1 GamesPerSeed=32 selection validation（2026-05-22）
- **状态：** completed, validation failed。
- **执行命令：** 使用正式 build `D:\Dev\Code\surakarta_traditional\build-vs-train-release`、Seeds `20260423,20260424,20260425`、`GamesPerSeed=32`、Depth `4`、CheckpointEvery `2`、`alpha=0.005`、`lambda=0.5`、`epsilon=0.02`、`epsilon_plies=6`、`terminal_reward=1200`、`td_error_clip=0`、`terminal_only_warmup=0`、`near_terminal_curriculum=0`。初次按外层 `pwsh` 逗号数组写法调用时 `Seeds` 被当成单字符串，参数绑定失败且未进入 run；随后用同一组参数显式传 PowerShell 数组完成正式执行。
- **产物：** `selection-summary.json` / `selection-summary.txt` 位于 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- **总评：** `overall_rating=regress`，`strength_rating=regress`，`perf_rating=regress`；scorecard `pass/watch/regress=0/0/3`；candidate/baseline/draws = `12/16/2`；`score_regressions=1`；`best_move_changes=3`；`average_performance_ratio=1.622708`；`worst_performance_ratio=1.864728`。
- **seed 结果：** `20260423` 为 `regress/regress/regress`，C/B/D=`4/5/1`，1 个 score regression，1 个 best move change，max perf `1.343750`；`20260424` 为 `regress/regress/regress`，C/B/D=`4/6/0`，max perf `1.659647`；`20260425` 为 `regress/regress/regress`，C/B/D=`4/5/1`，max perf `1.864728`。
- **固定局面归因：** blocker 全集中在 `opening`。三个 seed opening best move 均从 baseline `(0, 1) -> (1, 2) (B)` 漂到 candidate `(3, 1) -> (2, 2) (B)`；`20260423` 还有 opening score delta `-78`。opening node ratio 分别为 `1.343750`、`1.659647`、`1.694973`，`20260425` opening search ratio `1.864728`。`capture-heavy`、`threat-heavy`、`endgame`、`no-capture-critical` 未出现 best move drift 或 score regression；除 `20260425 capture-heavy` 轻微 node ratio `1.004457` 外，其余关键非 opening case node ratio 均为 `1.0`。
- **追溯性：** `weights-manifest.json` purpose=`selection`，git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，artifact `54` 个（bootstrap `3`、candidate `3`、checkpoint `48`），所有 artifact 文件存在且 SHA256 与 manifest 匹配；summary 参数与本轮指定参数一致；正式 perf baseline SHA256 仍为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- **判定：** 32 局已经 `overall_rating=regress`，同时 `strength_rating=regress`、`perf_rating=regress`、存在负向 score regression 与 unexplained opening best move drift / search-tree inflation。立即停止，不跑 64；Phase 3 继续 `implementation complete, trend validation failed`；Phase 4 继续 blocked。

##### Phase 3.2 opening blocker root-cause attribution（2026-05-22）
- **状态：** completed as attribution；不是修复阶段。
- **输入产物：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`；读取 `selection-summary.json`、`selection-summary.txt`、`weights-manifest.json` 和 48 个 checkpoint eval summary。
- **追溯性：** checkpoint scorecard 与 eval files 对齐，`mismatch_count=0`；manifest `purpose=selection`，artifact count `54`，`bootstrap=3`、`candidate=3`、`checkpoint=48`，`artifact_missing=0`，`hash_mismatch=0`；参数与 Phase 3.1 指定值一致。
- **首次事件：** 三 seed opening best move drift 均在 cp2 首次出现，先漂到 `(5, 1) -> (4, 2) (B)`；最终 move `(3, 1) -> (2, 2) (B)` 首次出现位置分别为 `20260425` cp14、`20260424` cp22、`20260423` cp32。`search_ratio > 1.25` 三 seed 均从 cp2 出现；`node_ratio > 1.25` 为 `20260423` cp6、`20260424/20260425` cp4。
- **score regression：** `20260423` opening score_delta 负值首次出现在 cp30，并在 cp32 持续；`20260425` 只在 cp6-cp12 为负，cp14 后恢复为正；`20260424` 无负向 score_delta。
- **非 opening 对比：** `capture-heavy`、`threat-heavy`、`endgame`、`no-capture-critical` 48 个 checkpoint 均无 best move drift、无 score regression；node ratio 基本稳定，不是当前主 blocker。
- **归因：** Phase 3.1 regress 的主固定局面问题是 opening root move instability。search-tree inflation 跟随 opening candidate root move 改变，但不是只由最终 move 引起；strength regress 与 perf regress 有共同 opening 扰动背景，但 gate 证据相互独立，不能用正向 score_delta 或个别胜负信号覆盖。
- **下一步：** 继续 opening-specific root move diagnostics 和最小 ablation；暂不支持直接调 `alpha/lambda/epsilon` 再跑，不支持放宽 perf gate，不进入 Phase 4。
- **判定：** Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 仍 blocked。

##### Phase 3.3 opening root diagnostics and minimal attribution（2026-05-22）
- **状态：** completed as read-only diagnostics；不是修复阶段。
- **输入产物：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- **诊断 CLI：** `bitboard-search` usage 支持 `--weights`、`--depth`、`--threads`、`--format json`、`--case-id`、`--search-diagnostics`；未见 `--debug-search-tree`；现有 JSON 无 active tuple / weight attribution。
- **输出产物：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics`，包含 18 个 per-weight JSON、`opening-diagnostics-summary.csv`、`opening-root-moves-depth4-final-attempt.csv`、`opening-aspiration-attempts.csv`、`opening-diagnostics-summary.md`。
- **cp2 首次 drift：** 三 seed 均为 `(5, 1) -> (4, 2) (B)`；depth-4 final attempt 中该 move order rank `1`，baseline order rank `2`；observed score 同为 score rank `1` / bound-tie，不是明确的全宽 score 排序压低。
- **最终收敛：** `20260425` cp14、`20260424` cp22、`20260423` cp32 首次出现 `(3, 1) -> (2, 2) (B)`；三个 final candidate 均以该 move 为 best，final candidate 中它都是 order rank `1`，baseline order rank `2/2/3`。
- **qnodes 归因：** cp2 无 aspiration retry，`(5,1)->(4,2)` root qnodes `355`，baseline root qnodes `38`；final candidate 中 `(3,1)->(2,2)` root qnodes 分别为 `358/356/440`，baseline root qnodes `16/16/18`。主要膨胀集中在当前 PV root move。
- **aspiration：** retry 只在后期放大成本；cp2 首次 drift `aspiration_retries=0`，不能解释最早 drift。final candidate retry qnodes 为 `163/720/233`，可解释一部分甚至超过相对 bootstrap 的 qnode delta，但这是后期放大器，不是最早 root move instability 的起因。
- **判定：** Phase 3 仍未通过；Phase 4 仍 blocked。不补生产代码；若要继续，只能先写 report-only diagnostic extension / 最小 ablation 设计。

##### Phase 3.4 opening active-tuple attribution and minimal ablation design（2026-05-22）
- **状态：** completed as design / feasibility analysis；不是正式修复阶段。
- **本轮边界：** 未重新训练；未跑 `GamesPerSeed=64`；未进入 Phase 4；未放宽 selection gate；未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；未修改搜索参数、PVS、qsearch、aspiration、move ordering、LMR、Null Move；未改 `src` / `include`。
- **代码审计结论：** `NTupleWeightHeader` 后接 `16605` 个 little-endian `int32` 权重；当前 tuple set hash 为 `0x1D4A6EBF2BAF6484`。`TupleDescriptors()` 共有 `45` 个 tuple，active index 为 `TupleOffsets()[tuple_id] + EncodeTuple(position, tuple)`；编码三态为 empty `0`、black `1`、white `2`。`MaterialBalance(position)=(black_count-white_count)*100`，`EvaluateWeightsForSideToMove` 先求 black-perspective `material + tuple`，再按 `SideToMove()` 黑方保留、白方取负。
- **可行性判定：** 现有 CLI 单独不能输出 material / active tuple / tuple contribution；但可以不改生产代码，用离线小脚本解析权重、复刻 tuple descriptor、构造 opening root child position 并输出 attribution。这样足够用于 Phase 3.4 report-only 诊断，但脚本复制 descriptor 有漂移风险，必须校验 header hash / weight_count。
- **最小 attribution 数据集：** 3 个 seed × `bootstrap/cp2/final` 共 9 个权重，对 3 个 opening root child position 做 static attribution：baseline `(0,1)->(1,2) (B)`、cp2 drift `(5,1)->(4,2) (B)`、final drift `(3,1)->(2,2) (B)`；summary 预期 `27` 行，tuple detail 预期 `27 * 45 = 1215` 行。
- **输出表字段：** `seed`、`stage`、`root_move`、`child_side_to_move`、`material_score`、`tuple_score`、`total_static_score`、`active_tuple_count`、`top_positive_tuple_deltas`、`top_negative_tuple_deltas`、`bootstrap_vs_stage_delta`、`baseline_child_vs_drift_child_delta`、`top_tuple_dominance` / `是否存在少数 tuple 主导排序变化`。
- **推荐最小 ablation：** 先做 `attribution-only`，再做 `child-position static ablation`（内存中把 top-k stage-vs-bootstrap delta 归零并比较 child static eval），暂不直接跑 search。只有 attribution 显示少数 tuple 主导时，才复制 candidate weights 到临时文件做 `offline neutralization`，并只跑 `bitboard-search opening --search-diagnostics` 复核 root diagnostics。
- **report-only CLI 备选：** 若下一阶段不接受脚本复制 tuple descriptor，可新增 `bitboard-attrib` 或 `bitboard-eval --explain-tuples`，最小改动范围为 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp` 和相应 test/Pester；扩展默认关闭，只读输出 JSON，不改变搜索、训练、selection 行为。
- **风险口径：** tuple attribution 解释的是静态 eval 贡献，不等于 PVS 全宽精确分，不等于棋力提升；不能把 PVS bound-tie 当全宽排序，不能把 qnodes 高直接解释成 qsearch bug，也不能因为 opening 能解释就解除 Phase 4 blocked。
- **判定：** Phase 3 仍未通过；Phase 4 仍 blocked。

##### Phase 3.5 opening attribution-only execution（2026-05-22）
- **状态：** completed as attribution-only execution；不是修复、不是 ablation、不是训练阶段。
- **本轮边界：** 未重新训练；未跑 `GamesPerSeed=64`；未进入 Phase 4；未修改 `src` / `include` 生产代码；未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；未修改 selection gate；未生成改写后的 candidate weights；未执行 offline neutralization。
- **脚本与输入：** 新增非生产离线脚本 `D:\Dev\Code\surakarta_traditional\tools\opening-attribution.ps1`。输入 run root 为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`，脚本读取 `weights-manifest.json`、`selection-summary.json`、`opening-diagnostics\opening-diagnostics-summary.md`、`task_plan.md`、`findings.md`、`progress.md`。
- **artifact 解析：** 不手搓 checkpoint 路径；从 manifest 解析 seed `20260423/20260424/20260425` 的 `bootstrap`、checkpoint `games_completed=2`、`candidate`。9 个 artifact 均校验文件存在、SHA256 pass、seed/参数匹配 Phase 3.1 summary、权重 header pass。
- **权重硬校验：** `magic=0x5455504C`、`version=1`、`tuple_set_hash=0x1D4A6EBF2BAF6484`、`weight_count=16605`、文件长度 `66444 = 24 + 16605 * 4`，header 后正好 `16605` 个 little-endian int32。
- **输出产物：** `opening-child-attribution-summary.csv`、`opening-child-attribution-tuples.csv`、`opening-child-attribution-summary.json`、`opening-child-attribution-top-deltas.csv`、`attribution-manifest.json`，全部位于 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution`。
- **规模验收：** summary `27` 行；tuple detail `1215` 行；active tuple count 异常 `0`；top-deltas `1440` 行；manifest 记录 script path、run root、output dir、seeds、stages、root moves、tuple hash、weight count、generated_at、git commit。
- **static eval 结论：** 三个 opening child 均为 White to move，material score 均为 `0`，total static score 为 black-perspective tuple sum 的取负。cp2 阶段三 seed 一致：baseline child `-10`，cp2 drift child `-23`，final drift child `4`，说明 cp2 drift child 相对 baseline child 对 Black 更优。final 阶段 final drift child 相对 baseline child delta 为：`20260423: 64 -> -41`（`-105`）、`20260424: -359 -> -497`（`-138`）、`20260425: -214 -> -364`（`-150`），解释了 final drift child 更容易被静态排序推前的方向。
- **dominance 结论：** 未发现少数 tuple 主导 opening child static eval 变化；36 个 comparison 中 dominance `yes/watch/no=0/0/36`，max top1 abs share `0.165094`，max top3 abs share `0.376866`。这锅不是一两个 tuple 单独背的，别硬抓替罪羊。
- **下一步：** 足够进入 child-position static ablation 设计；当前不支持直接做 offline neutralization，不支持复制/改写 candidate weights，不支持放宽 gate 或进入 Phase 4。
- **判定：** Phase 3 仍未通过；Phase 4 仍 blocked。

##### Phase 3.6 child-position static ablation design（2026-05-22）
- **状态：** completed as design only；不是 ablation execution，不是 neutralization，不是训练。
- **本轮边界：** 不写代码；不改 `src` / `include`；不改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；不重新训练；不跑 `GamesPerSeed=64`；不进入 Phase 4；不生成改写后的 candidate weights；不执行 offline neutralization。
- **输入证据：** 已读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`，以及 Phase 3.5 attribution 目录下的 `opening-child-attribution-summary.csv`、`opening-child-attribution-tuples.csv`、`opening-child-attribution-top-deltas.csv`、`attribution-manifest.json`。
- **child positions：** `baseline_child=(0,1)->(1,2) (B)`、`cp2_drift_child=(5,1)->(4,2) (B)`、`final_drift_child=(3,1)->(2,2) (B)`。主比较为 cp2 阶段 `baseline_child -> cp2_drift_child` 和 final 阶段 `baseline_child -> final_drift_child`；辅比较为 final 阶段 `baseline_child -> cp2_drift_child`、`cp2_drift_child -> final_drift_child`；control 为 bootstrap 下两组 drift child 不应天然优于 baseline，以及 cp2 下 final drift child 不应被误判为 cp2 首漂原因。
- **neutralization 单元：** 不直接用 same-stage child-vs-child top-deltas 砍权重；应从 tuple detail 派生 `stage_delta(child, tuple)=contribution(stage)-contribution(bootstrap)`，再计算 `induced_pair_delta=stage_delta(to_child)-stage_delta(from_child)`。当 `to_child` 是 drift child 且 `induced_pair_delta < 0` 时，该 tuple delta 才是 drift-favoring neutralization candidate。
- **TopK 口径：** 对每个 seed/comparison 按 `abs(induced_pair_delta)` 排序，剔除 `0`，并稳定 tie-break 到 `tuple_id/absolute_index`；输出 `K=1/3/5/10/20/all_negative` 的累积曲线。`top1` 只能做诊断点，不作为主结论；Phase 3.5 已显示 dominance `yes/watch/no=0/0/36`，直接抓 top1 就是拿镊子修发动机，劲儿用错地方。
- **判定口径：** success 只表示 static child preference 被少量稳定 tuple delta 解释，不表示 Phase 3 通过；failure 表示 TopK/all_negative 不能显著削弱 drift 偏好或 control 失败；inconclusive 表示削弱不稳定、seed 分裂、需要过大 K 或只能解释 final 不能解释 cp2 onset。
- **升级条件：** 只有主 comparison 在至少 `2/3` seed、最好 `3/3` seed 中由 `K<=10` 或最多 `K<=20` 的稳定 tuple 集合抹平大部分 induced pair delta，并且 control 不报警，才允许进入 offline neutralization 的设计评审。否则停止，不复制权重、不调参、不跑 64。
- **判定：** Phase 3 仍未通过；Phase 4 仍 blocked。

##### Phase 3.7 child-position static ablation execution（2026-05-22）
- **状态：** completed as report-only/table-only static ablation；不是 offline neutralization，不是训练，不是 Phase 4。
- **本轮边界：** 未修改 `src` / `include`；未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；未重新训练；未跑 `GamesPerSeed=64`；未运行 search diagnostics；未修改 selection gate；未生成或改写 candidate weights；未执行 offline neutralization。
- **脚本与输出：** 新增 `D:\Dev\Code\surakarta_traditional\tools\child-position-static-ablation.ps1`；输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`，包含 `opening-child-static-ablation-summary.csv`、`opening-child-static-ablation-tuples.csv`、`opening-child-static-ablation-sweep.csv`、`opening-child-static-ablation-summary.json`、`opening-child-static-ablation-manifest.json`。
- **追溯性：** attribution manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`；tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`；tuple set hash `0x1D4A6EBF2BAF6484`；weight count `16605`；git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- **执行 comparison：** `C1_cp2_onset`、`C2_final_gate_blocker`、`C3_final_cp2_residue`、`C4_final_transition`、`N1_bootstrap_cp2_sanity`、`N1_bootstrap_final_sanity`、`N2_cp2_final_negative_control`。
- **TopK 结论：** `C1` 和 `C2` 都不是 top1/top3 能单独解释，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring。`C1` K10 tuple 三 seed 完全一致：`4,5,6,10,11,27,32,36,38,41`；`C2` K10 的 `3/3` tuple 为 `5,10,32,41,43`，`2/3` tuple 为 `0,5,6,10,11,32,33,37,41,43,44`。
- **dominance=no 解释：** dominance 全 `no` 不否定 ablation；本轮曲线反而证明是 distributed tuple deltas 累积效应。它只证明 static child preference 可解释，不证明 PVS/root search 会恢复。
- **升级/停止判定：** 足够进入 offline neutralization 设计评审；必须停止在执行前，不写 neutralized weights，不替换生产权重，不跑 search/training/64，不改 gate，不进入 Phase 4。
- **判定：** Phase 3 仍未通过；Phase 4 仍 blocked。

##### Phase 3.8 offline neutralization design review（2026-05-23）
- **状态：** completed as design review only；不是 neutralization execution，不是训练，不是 selection，不是 Phase 4。
- **本轮边界：** 未修改 `src` / `include`；未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；未重新训练；未跑 `GamesPerSeed=64`；未运行 `bitboard-search` / training / selection；未修改 selection gate；未写任何 `.bin`；未创建 `phase3_8` neutralized 权重目录。`weights_written=false`，`neutralization_executed=false`。
- **已读取证据：** `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`；Phase 3.5 attribution 的 `opening-child-attribution-summary.csv`、`opening-child-attribution-tuples.csv`、`opening-child-attribution-top-deltas.csv`、`attribution-manifest.json`；Phase 3.7 static ablation 的 `opening-child-static-ablation-summary.csv`、`opening-child-static-ablation-tuples.csv`、`opening-child-static-ablation-sweep.csv`、`opening-child-static-ablation-summary.json`、`opening-child-static-ablation-manifest.json`。
- **是否满足进入设计评审：** 满足。依据是 `C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`，K1/K3 在主 comparison 中 `0/3` seed success，K5/K10 均 `3/3` seed success；controls 为 `control_pass`；manifest 记录 `tuple_set_hash=0x1D4A6EBF2BAF6484`、`weight_count=16605`、Phase 3.7 `global_decision=success_for_offline_neutralization_design_review`。这只允许设计评审，不允许执行 neutralization。
- **neutralization candidate tuple sets：** `C1 K10 = 4,5,6,10,11,27,32,36,38,41`；`C2 stable K10 core = 5,10,32,41,43`；`C2 2/3 support set = 0,5,6,10,11,32,33,37,41,43,44`。C1/C2 不应强行使用同一 tuple set；应按 stage / comparison 分 variant。共享 core `5,10,32,41` 可作为 conservative cross-stage variant，用来测试共同机制。
- **推荐 design variants：** conservative core `5,10,32,41`，预期同时解释 cp2 onset 和 final drift 的共享部分；C1-only K10 `4,5,6,10,11,27,32,36,38,41`，只针对 cp2 checkpoint-2 first drift；C2-focused stable K10 `5,10,32,41,43`，只针对 final gate blocker；optional union K10 `4,5,6,10,11,27,32,36,38,41,43` 标记为 riskier，只做压力对照，防止把“能翻表格”误当“能修搜索”。C2 2/3 support set 只用于 manifest 解释和审计，不作为默认写权重集合。
- **推荐 K 值：** 主推荐 `K10`，但执行上优先以 conservative core / stage-specific K10 variant 比较，而不是单纯 all_negative。不是 K1/K3，因为 Phase 3.7 主 comparison 中 K1/K3 均不足，C1/C2 都是 `0/3` seed success；不是直接 all_negative，因为 C1 K10 已等价 all_negative 且 `over_neutralization=True`，C2 all_negative 会写到 `15-17` 个 tuple 并把 ablated delta 推到 `+29/+53/+30`，这会把诊断变成大锤抡墙，解释力反而下降。
- **未来临时权重位置设计：** 如 Phase 3.9 真执行，只能复制源权重到 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization\weights-temp\<variant>\seed-<seed>\<stage>\`。文件名必须包含 `diagnostic-neutralized`，例如 `seed-20260423.cp2.c1-k10.diagnostic-neutralized.bin`、`seed-20260423.final.c2-stable-k10.diagnostic-neutralized.bin`；不得命名为 `candidate.bin`、`checkpoint-2.bin`、`bootstrap.bin`，不得写入 `selection\seed-*` 下任何 bootstrap/checkpoint/candidate 目录。
- **未来 manifest 必填字段：** phase、mode、generated_at、git_commit、source attribution/static-ablation manifest SHA256、source weight path/SHA256/header fields、output temp path/SHA256、variant id、comparison id、stage、seed、tuple_set_hash、weight_count、K policy、tuple ids、absolute indices、original values、neutralized values、removed deltas、expected explanation target、`weights_written`、`neutralization_executed`、`production_candidate=false`、`selection_gate_modified=false`、`training_executed=false`、`selection_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。
- **后续最小 diagnostics：** 若进入 Phase 3.9，只允许 offline diagnostics：manifest/header/hash 校验、static attribution 复算、opening fixed-case depth-4 root diagnostics / search diagnostics 对比 baseline/cp2/final root move、root order、observed score、nodes/qnodes/aspiration retries。不得跑 training、selection、GamesPerSeed=64、candidate gate，不得把 neutralized 权重作为 production candidate。
- **有解释力的结果：** conservative core 至少削弱 C1/C2 的 root-order/static preference；C1-only K10 必须只改善 cp2 onset 而不被要求解释 final drift；C2-focused stable K10 必须改善 final gate blocker；search diagnostics 中 baseline child 恢复为 tie/非 drift-favoring，qnodes/node inflation 同步下降，且 controls 不报警。若只静态翻转但 PVS/root search 无变化，只能说明 static component 有解释力，不能说明 blocker 已修。
- **必须停止的结果：** 需要 all_negative 或大于 K10 才见效；C1 解释不了 cp2 onset；C2 只解释 final 但引入新 root drift；controls 失效；临时权重污染 selection seed 目录；manifest/hash/header 不一致；有人试图改 gate、调参、训练、跑 64、或拿 neutralization 结果替代 selection gate。到这一步就刹车，别拿诊断螺丝刀去当通关令牌。
- **判定：** 建议进入 Phase 3.9 offline neutralization execution only；不建议进入 Phase 4。Phase 3 仍未通过；Phase 4 仍 blocked。

##### Phase 3.11 design-only opening-safe objective / root-order guard（2026-05-23）
- **状态：** completed as design-only；不是实现、不是训练、不是 selection、不是 Phase 4。
- **本轮边界：** 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`；未修改 `src` / `include`；未修改 `tools\`；未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`；未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成新的 `.bin`，未修改 selection gate 阈值。
- **证据前提：** Phase 3.1 gate 仍 `regress`；Phase 3.2-3.3 定位 opening 是唯一硬 blocker；Phase 3.5 排除 single tuple dominance；Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference；Phase 3.9 强解释 cp2 first drift/root order inflation，但 final 只有 partial explanation，并存在 `20260423` 1/3 root over-neutralization 与新 best move 副作用。
- **方向 1 - candidate acceptance guard：** 目标是在候选权重进入正式 selection 前先用 report-only opening diagnostics 拦截 root drift 风险；作用阶段是 candidate acceptance / selection preflight。依赖证据是 cp2 baseline root order `2->1` 可被 neutralization 拉回、final `20260423` 副作用说明“静态能解释”不等于“候选可接受”。预期解决候选报告中 cp2 first drift、root order inflation、qnodes/root cost 异常、final new best move 的可见性；不能解决训练本身产生 drift-favoring tuple delta 的问题。主要风险是 guard 过窄只盯 opening，或过宽变成偷改 gate；最小验证方式是 Phase 3.12 先定义 report-only JSON/TXT 字段和固定 opening diagnostics 对照，不改变 pass/watch/regress。生产代码变更：短期不需要；可能影响 selection gate：设计上只报告，未来若纳入 gate 必须单独评审。
- **方向 2 - opening-safe training objective：** 目标是在训练目标层惩罚 opening baseline child 相对 drift child 的不安全偏移，减少 stage-vs-bootstrap 的 drift-favoring distributed tuple deltas；作用阶段是 TD/self-play training objective。依赖证据是 Phase 3.5 的 cp2 `-13` 和 final `-105/-138/-150` static delta，以及 Phase 3.7 K5/K10 分布式削弱曲线。预期解决 cp2 first drift 反复出现、root order 被训练后权重推翻的问题；不能保证 final candidate 完全恢复，也不能替代 match strength。主要风险是过拟合单一 opening fixed case、压低真实学习信号、把 final 的 partial explanation 误当全修复。最小验证方式是先做 design-to-diagnostic bridge，定义 objective 约束如何在 attribution/ablation 表格中被离线模拟；生产代码变更：未来可能需要训练代码变更；可能影响 selection gate：是，若实现后必须重新走 32/64 正式 selection。
- **方向 3 - root-order regularization：** 目标是把 root child ordering 的稳定性作为训练或候选验收的显式约束，避免 bound-tie 局面被轻微 static delta 推成更贵 PV 子树；作用阶段可在 training checkpoint regularization 或 candidate acceptance diagnostics。依赖证据是 Phase 3.3 cp2 无 aspiration retry、qnodes 集中到新 PV root move，Phase 3.9 cp2 root order `2->1` 与 nodes/qnodes 下降同步。预期解决 root order inflation 和 PV root move 迁移造成的 qsearch 成本膨胀；不能解决全局棋力不足、非 opening case、或 final new best move 副作用。主要风险是把 baseline move 固化成教条，阻止真实强着替换，尤其 final 已显示可能出现新 best move。最小验证方式是 Phase 3.12 先定义 root-order/rank/cost delta 报告字段和 safe/tie/new-best-move 分类；生产代码变更：report-only guard 不需要，训练期 regularization 未来可能需要；可能影响 selection gate：未来若进入 acceptance guard 会影响候选晋级，但不得改现有 gate 阈值。
- **推荐顺序：** 第一优先 design/report-only guard；第二才是最小 opening-safe training objective 约束；第三才考虑生产搜索或特征扩展。这个顺序是为了先把“什么会被拒绝”讲明白，再谈“怎么训练得不犯病”，别上来就扩特征，整挺热闹但证据链又糊了。
- **明确不推荐：** 不直接扩 N-Tuple / threat / mobility / stage 特征；不直接 all_negative neutralization；不直接重训并跑 64；不调 selection gate 阈值；不把 Phase 3.9 neutralized 权重当 production candidate。
- **下一步：** 建议进入 Phase 3.12 design-to-diagnostic bridge，只把上述设计转为 report-only diagnostic spec / artifact schema / minimal fixed-case checks；不直接实现 training objective，不跑 selection，不进入 Phase 4。
- **判定：** Phase 3 still not passed；Phase 4 still blocked。

#### 阶段 D：评估函数特征扩展
- [ ] 一次只引入一类特征：loop/capture N-Tuple -> threat-aware -> mobility -> stage-aware
- [ ] 每类特征必须有 ablation、固定局面回归、selection 对照和 manifest 兼容策略
- [ ] 不允许“大杂烩式加特征”，否则回归来源会糊成一锅粥

#### 阶段 E：搜索与时间管理升级
- [ ] 在稳定评估函数上调 move ordering、killer/history、LMR、Null Move、aspiration
- [ ] 增加真实 time management / movetime / soft-hard deadline，解决 fixed-depth 实战极端局面耗时风险
- [ ] 增加更细的 qsearch 控制、重复局面/置换表策略、残局 extension 或安全裁剪

#### 阶段 F：国赛级评测与发布
- [ ] 建立 engine-vs-engine ladder，至少覆盖 baseline、校赛版、每个候选权重、每个搜索参数候选
- [ ] 引入 SPRT/Elo/置信区间，不再只看少量 seed 胜负
- [ ] 固化 release gate：rule correctness -> perf -> selection -> GUI acceptance -> match preset calibration
- [ ] 输出可复现实验报告和赛前锁版包

### 本轮验证结果
| 检查项 | 结果 |
|---|---|
| git status | B1 代码与记忆文件仍为未提交改动；`docs/national_competition_technical_roadmap.md` 仍是 untracked；未创建提交 |
| git commit | `6063d9acbb24dc9397e07e99bffa4e94b8af55f6` |
| `pwsh ./scripts/workflow.ps1 -Configuration Debug` | configure failed：GitHub `glfw 3.4.zip` FetchContent download connection reset；未进入构建 |
| `pwsh ./scripts/workflow.ps1 -Configuration Release` | configure failed：GitHub `glfw 3.4.zip` FetchContent download timeout；未进入构建 |
| fallback build | `build-b1-core` cached Debug core build，`SURAKARTA_ENABLE_GUI=OFF`；`Invoke-SurakartaBuild` no work to do |
| fallback selftests | `build-b1-core\bin\surakarta-bitboard-selftest.exe` `[PASS]`；`build-b1-core\bin\surakarta-dev-session-selftest.exe` `[PASS]` |
| fallback CTest | `Invoke-SurakartaCTest -BuildDir build-b1-core`：`3/3 passed` |
| Pester workflow | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet`：`26 passed / 0 failed`；红色 exception 为负向测试预期输出 |
| perf gate | `pwsh ./scripts/perf-runner.ps1`：failed，`15` regressions；`14` performance + `1` score drift；无 best-move drift；未更新 baseline |
| perf 分类 | `middlegame` 全过；`endgame` 9 项 tiny-position node-ratio fail，best move/score 不变，是 B1 移除 legacy no-capture early terminal 后真实节点增长；`opening` depth 7/8 搜索树增长与 depth 7 score `12 -> 0` 属于 post-B1 terminal semantics 变化后的旧 baseline 失效 |
| B2.1 candidate | `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\perf-baseline.candidate.json`，来源 `20260522-145419\perf-summary.json`，27 条矩阵完整；未修改正式 baseline |
| B2.1 candidate rerun #1 | `pwsh ./scripts/perf-runner.ps1 -BaselinePath <candidate>` => failed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729`；1 个 `performance_regression`：`middlegame d7 t4 search_ratio=1.405796`，node_ratio `1.011121`，best move/score 不漂 |
| B2.1 candidate rerun #2 | `pwsh ./scripts/perf-runner.ps1 -BaselinePath <candidate>` => failed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913`；4 个 `performance_regression`，均为 `search_seconds` 敏感；0 best move drift，0 score drift，最大 node_ratio `1.069597` |
| B2.1b 新增采样 | single candidate baseline 复跑 5 次：`20260522-163919` fail 1、`20260522-163943` fail 3、`20260522-164009` pass、`20260522-164037` pass、`20260522-164101` fail 1；失败仍集中在 `search_seconds`，无 best move/score drift |
| B2.1b aggregate candidate | `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json`；8 样本、27 条完整；`best_move` / `score` 全稳定；正式 baseline 未修改 |
| B2.1b aggregate validation #1 | `pwsh ./scripts/perf-runner.ps1 -BaselinePath <aggregate>` => passed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164507`；`failure_count=0`，max node ratio `1.000000`，max search ratio `0.933802` |
| B2.1b aggregate validation #2 | `pwsh ./scripts/perf-runner.ps1 -BaselinePath <aggregate>` => passed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164532`；`failure_count=0`，max node ratio `1.057175`，max search ratio `1.220779` |
| B2.1b 结论 | aggregate candidate 显著降低单次 timing baseline 误杀；未发现 best move/score 漂移或系统性 node regression；建议进入 B2.2 正式 baseline 替换评审/执行 |
| B2.2 baseline replacement | source `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json` copied to `test/test_data/perf-baseline.json`；source/target SHA256 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`；`scripts/perf-runner.ps1` 无 diff |
| B2.2 official perf gate #1 | `pwsh ./scripts/perf-runner.ps1` => passed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165717`；baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max node ratio `1.001406`，max search ratio `1.070239` |
| B2.2 official perf gate #2 | `pwsh ./scripts/perf-runner.ps1` => passed；run `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165815`；baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max node ratio `1.000294`，max search ratio `1.249836` |
| B2.2 Pester | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet`：`26 passed / 0 failed`；红色 exception 为负向测试预期输出 |
| B2.2 CTest | 裸 `ctest --test-dir build-b1-core --output-on-failure` 因当前 PATH 无 `ctest` 入口未执行；等价封装 `. ./scripts/common.ps1; Invoke-SurakartaCTest -BuildDir build-b1-core`：`3/3 passed` |
| B2.2 diff check | `git diff --check` exit `0`；仅报告现有 LF/CRLF warning，无 whitespace error |
| training smoke | `pwsh ./scripts/benchmark-train.ps1` passed；seed `20260423`，games `4`，depth `4`，`alpha=0.005`，`lambda=0.5`，`epsilon=0.02`，reproducible `True` |
| selection smoke | `pwsh ./scripts/benchmark-select.ps1 -GamesPerSeed 2` passed as script smoke；summary `overall_rating=regress` / `strength_rating=watch` / `perf_rating=regress`，不代表 Phase 3 通过 |

### 产物
- workflow fallback build dir：`D:\Dev\Code\surakarta_traditional\build-b1-core`
- perf run：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`
- B2.1 candidate：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651`
- B2.1 candidate reruns：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913`
- B2.1b additional single-candidate reruns：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163919`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163943`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164009`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164037`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164101`
- B2.1b aggregate candidate：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424`
- B2.1b aggregate candidate reruns：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164507`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164532`
- B2.2 official baseline：`D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`
- B2.2 official gate reruns：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165717`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165815`
- training smoke：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-145835`
- selection smoke：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-150225`
- `D:\Dev\Code\surakarta_traditional\docs\national_competition_technical_roadmap.md`
- `D:\Dev\Code\surakarta_traditional\task_plan.md`
- `D:\Dev\Code\surakarta_traditional\findings.md`
- `D:\Dev\Code\surakarta_traditional\progress.md`

---

# 历史计划：Phase 4 方案 B 战术绞肉机路线规划

## 目标
对照用户给出的“极速 PVS + TD-Learning + Bitboard”蓝图，基于当前仓库现状给出下一阶段的可执行任务分解，并明确优先级、验收口径与边界。

## 当前阶段
阶段 3B / post-fix rule correctness hardening；Phase 4 blocked

## 各阶段

### 阶段 1：现状摸底与差距分析
- [x] 理解用户意图：不是立刻实现，而是给出下一阶段任务
- [x] 确定约束：以当前仓库为基础，不重复规划已经完成的搜索骨架
- [x] 将现状发现记录到 findings.md
- [x] 确认是否还存在高影响的不确定项
- **状态：** completed

### 阶段 2：基线与可追溯性门禁
- [x] 将 perf baseline 固化为独立门禁：opening / middlegame / endgame，depth 6/7/8，threads 1/2/4
- [x] 为 training / selection / release 定义统一的 weights manifest 与 metadata 结构
- [x] 把 git commit、seed、games、depth、alpha、lambda、epsilon、checkpoint 关联到产物
- [x] 明确失败判定：性能退步、最佳着法漂移、计时噪声、权重不可追溯
- **状态：** completed

### 阶段 3：训练闭环与 selection 稳定
- [ ] 强化 bitboard-train 诊断输出：TD error、weight delta、胜负、终局类型、checkpoint 摘要
- [ ] 支持 checkpoint 级对比与中间产物 eval，避免只看最终权重
- [ ] 将 selection 规则固化为 pass / watch / regress，并区分真实退步与小节点计时噪声
- [ ] 扩大固定局面集：opening、capture-heavy、threat-heavy、残局、无吃子临界局面
- [ ] 先稳定 GamesPerSeed=32/64 的趋势，再谈更大样本
- **状态：** implementation complete, trend validation failed
- **验收结论：** GamesPerSeed=32 overall_rating=regress，3 个 seed 全部 regress，candidate/baseline/draws = 12/18/0；阶段 3 不能标 completed。
- **稳定化进展：** 已完成 12 组小型参数 sweep、reward scale sweep、TD error clipping sweep；全部仍为 regress。当前最接近 watch 的稳定化组合是 `alpha=0.005, lambda=0.5, epsilon=0.02, TerminalReward=500, TdErrorClip=250`，但 opening node ratio max 仍为 `1.295516`。
- **学习信号隔离：** 已完成 lambda / exploration / alpha=0 / TD trace / opening attribution / near-terminal curriculum / opening search-tree targeted diagnosis；terminal TD spike 已被 `TdErrorClip=250` 压住，当前 blocker 转移到 opening search-tree inflation：`20260425` opening nodes 稳定为 `1907/1472`，node ratio `1.295516`。targeted diagnosis 显示主因是 PV 切到 `(1, 1) -> (2, 2) (B)` 后进入更贵 qsearch 子树，aspiration retry 只解释约三成 qnode inflation。32 局 selection 仍 regress，Phase 4 继续 blocked。
- **Decision run：** `GamesPerSeed=64` policy evidence run 仍按现有规则判 `overall_rating=regress`，C/B/D=`16/12/2`，fixed score regressions=`0`，opening node ratio avg/max=`1.122056/1.245924`。强度信号保持，但 regress 源从 `20260425` 单点 node ratio 转为 `20260423/20260424` opening `search_seconds` ratio；趋势仍未稳定，Phase 4 继续 blocked。
- **Report-only rating split：** selection summary 已新增 `strength_rating`、`perf_rating`、`blocker_reason` 诊断字段，只拆表盘不改 gate；`overall_rating` 仍按既有规则保守判级，Phase 3 状态仍为 `implementation complete, trend validation failed`。
- **Report-only perf blocker_details：** selection summary 已新增 machine-readable `blocker_details`，按 seed / category / metric 记录 `search_seconds_ratio`、`node_ratio`、可用时的 `qnodes_ratio` 超阈来源；`blocker_reason` 继续保留给人看。该改动只增强归因解释，不新增放行逻辑，Phase 3 仍是 trend validation failed。
- **Report-only repeatability artifact integration：** selection summary 的 perf `blocker_details` 进一步接入文档层 repeatability 证据，新增 `repeatability` / `repeatability_status` / `repeatability_runs` / `repeatability_exceeded_count` / `repeatability_notes`；仅匹配 `20260423 opening search_seconds_ratio`、`20260425 opening search_seconds_ratio`、`20260425 opening node_ratio`，其余保持 `unknown`。这只是诊断输出，不改 `overall_rating` / `perf_rating` / `strength_rating`，更不新增 `perf_watch`。
- **Quick smoke：** `GamesPerSeed=2` 产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-162204` 只得到 `overall_rating=watch` / `strength_rating=watch` / `perf_rating=watch`，未触发 final perf regress；顶层/per-seed final `blocker_details` 为空，`selection-summary.txt` 表格为 `(none)`，因此 smoke validation failed / inconclusive。
- **Targeted artifact validation：** `GamesPerSeed=32` targeted perf-regress 产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241` 已验证 report-only `blocker_details` 真实落盘：`overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`，顶层/per-seed/final scorecard `blocker_details` 均非空，`selection-summary.txt` 的 `Perf blocker details` 表格非空；Pester 仍为 `23 passed / 0 failed`。
- **Repeatability artifact integration 验收：** Pester 复跑后变成 `24 passed / 0 failed`；repeatability 为 `timing_sensitive` 的 blocker 仍按旧 gate 判 `regress`，`stable_tree_inflation` 只做解释，不加 gate。当前只能标 implemented and unit-tested，不能标 artifact-validated。
- **Latest targeted repeatability artifact validation：** 最新真实产物 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525` 为 `overall_rating=regress` / `strength_rating=watch` / `perf_rating=regress`；`blocker_details` 实际只有 2 条：`20260425 opening search_seconds_ratio=1.413497 repeatability=unknown`、`20260425 opening node_ratio=1.295516 repeatability=stable_tree_inflation`。`20260425 node_ratio` 的 artifact path 已验证；`20260425 search_seconds_ratio` 因 ratio 从 known evidence `2.159236` 漂到 `1.413497` 而正确保持 `unknown`；`20260423` timing blocker 未出现；full three-blocker real artifact validation not achieved。
- **Deterministic artifact fixture validation：** 新增 fake selection harness fixture 用例，确定性生成临时 `selection-summary.json` / `selection-summary.txt`，覆盖 `timing_sensitive`、`stable_tree_inflation_with_timing_noise`、`stable_tree_inflation` 和 `unknown` 四类 repeatability 输出；验证 JSON 五个 repeatability 字段、TXT `repeatability` 列、unknown 不硬贴旧证据，以及 gate guard。Pester 结果 `25 passed / 0 failed`；该验证只证明 artifact formatting / repeatability mapping，不代表 Phase 3 趋势通过。
- **Blocker evidence examples：** known evidence 仍是 `20260423 opening search_seconds_ratio=1.469059 > 1.25`，`20260425 opening search_seconds_ratio=2.159236 > 1.25`，`20260425 opening node_ratio=1.295516 > 1.25`；但 `search_seconds_ratio` 有 timing noise，真实 run 不适合作为精确 ratio 命中的唯一验收方式，不能靠反复刷训练 run 凑三条 blocker。
- **当前边界：** report-only rating split completed；report-only `blocker_details` completed and targeted artifact-validated；report-only repeatability artifact integration is implemented, unit-tested, and deterministic-fixture-validated, but real artifact validation remains partial / not accepted；current blocker 仍是 opening performance instability / `perf_rating=regress`，Phase 4 blocked；不新增 `perf_watch`，不让 `strength_rating` 覆盖 `perf_rating`，`repeatability_status` 不参与 gate。
- **Freeze / handoff summary：** Phase 3 implementation complete，但 trend validation failed；32 局最好组合仍 `regress`，64 局 decision run 仍 `regress`；deterministic fixture validation `25 passed / 0 failed` 已接受，只能证明 artifact formatting / repeatability mapping，不是放行信号。Phase 4 继续 blocked。
- **P0 事故更新：** 用户人工验收发现疑似 Surakarta 吃子规则回归。Phase 3 remains implementation complete, trend validation failed；Phase 4 blocked。疑似规则回归之后产生的 training / selection artifacts 全部 provisional / suspect，直到 rule correctness 恢复并通过新增 capture-rule 测试。当前优先级从训练稳定化切换为棋规正确性修复；不继续训练 sweep，不放宽 selection，不把现有 Phase 3 产物当作可信通过依据。

### 阶段 3B：P0 Surakarta capture-rule correctness incident
- [x] 记录事故状态到 `task_plan.md` / `findings.md` / `progress.md`
- [x] 复现用户人工发现的吃子失败，并固化最小 board position / side-to-move / expected capture / actual missing or wrong capture
- [x] 对比 traditional movegen 与 bitboard movegen；可信一方作为 oracle，不可信则手写 golden cases
- [x] 审计 move generation、capture path generation、loop/corner/arc traversal、own piece blocking、opponent detection、multiple-loop legality
- [x] 审计 apply/undo、capture removes target piece、no-capture counter reset、terminal no-legal-move reason
- [x] 用 git diff / git blame / 最近改动定位引入点，重点看 bitboard movegen、capture generation、qsearch capture-only source、fixture、apply/undo、coordinate transform、side-to-move legality
- [x] 增加 selftest / Pester / C++ 测试覆盖 legal/illegal capture、normal move、make/unmake、no-capture reset、traditional/bitboard parity、qsearch/search depth 1 sees capture
- [x] 最小修复棋规 bug，不改 PVS / qsearch 参数 / aspiration / move ordering / LMR / Null Move，不扩特征，不改 selection gate
- [x] clean rebuild 后运行 `surakarta-bitboard-selftest`、workflow Pester、新增 capture-rule tests
- **状态：** capture regression fixed and verified；Phase 4 仍 blocked
- **事故边界：** Phase 4 继续 blocked；所有疑似回归之后的 training / selection 产物都必须标记为 suspect/provisional，不能用于 Phase 3 趋势通过判定。
- **复现局面：** Black to move；黑子 `(2,1)`, `(2,2)`, `(2,3)`，白子 `(3,3)`；expected legal capture `(2,2) -> (3,3)`；修复前 traditional 为 `LEGAL_CAPTURE_MOVE`，bitboard 为 `ILLIGAL_CAPTURE_MOVE` / capture-only movegen missing。
- **根因：** `include/surakarta_bitboard.h::kMaxCaptureVariants` 旧值 `4` 截断了 `(2,2)->(3,3)` 的 8 条 capture path variants；`src/surakarta_bitboard.cpp::BitboardGenerator::Build()` 受容量限制只写入前 4 条，前 4 条被己方棋子挡住后 bitboard 漏掉后续合法路径。
- **修复：** `kMaxCaptureVariants=8`，重新生成 `include/surakarta_masks.h`，并在 `src/surakarta_bitboard_selftest.cpp` 固化 golden capture tests。
- **验证：** clean rebuild exit 0；`build-p0-rule\\bin\\surakarta-bitboard-selftest.exe` `[PASS]`；`Invoke-Pester -Script scripts\\workflow.tests.ps1 -PassThru -Quiet` 为 `25 passed / 0 failed`；`ctest --test-dir build-p0-rule --output-on-failure` 为 `2/2 passed`。
- **终局审计备注：** 现有 traditional/bitboard 仍按 legacy `max_no_capture_round` / `no_capture_ply` 路径判 `STALEMATE`，训练里另有 `terminal_no_legal_move` 统计；这次没有把它偷换成大赛规则第 6 条的放行依据。后续如果要严格实现“双方都不能再吃子”终局，需要单独改 `JudgeEnd`/search terminal 语义并补 golden cases。
- **Post-fix hardening：** `BitboardGenerator::Build()` 的 capture path variant 写入已从容量不足时静默跳过改为 fail fast；任何实际 variant 数量超过 `kMaxCaptureVariants` 都必须直接失败，不能截断后继续生成。
- **容量 invariant：** `surakarta-bitboard-selftest` 新增 `TestCaptureVariantCapacityInvariant`，动态生成表和静态预计算表都会扫描全部 from/to，要求所有 `capture_variant_count <= kMaxCaptureVariants`；当前最大值为 `8`。
- **扩展 rule correctness parity：** `TestRuleCorrectnessGoldenParity` 对 traditional rule manager、bitboard rule manager、bitboard `GenerateMoves` / capture-only movegen 做 golden parity，覆盖中心、边、角、内环、外环、多路径、己方阻挡、敌方目标、无目标和普通移动；保留用户 P0 局面 `(2,2)->(3,3)` 为合法 capture。
- **规则门禁：** Rule correctness selftest failed => 禁止 training；Rule correctness selftest failed => 禁止 selection；Rule correctness selftest failed => 禁止 release；Rule correctness selftest failed => 禁止 Phase 4。
- **历史 suspect/provisional artifacts：** 以下产物继续只能作为历史诊断材料，不能作为 Phase 3 trend validation 通过依据：`20260503-234333`、`20260504-123006`、`20260504-162204`、`20260504-163241`、`20260504-205525`、`phase3-repeatability-evidence-20260504`。
- **separate rule-semantics debt：** legacy `max_no_capture_round` / `no_capture_ply` 不是大赛第 6 条“双方都不能再吃子”的严格实现。2026-05-22 阶段 B1 已单独对齐该语义，新增 national-rule golden cases，并修改 traditional / bitboard / search / dev-session terminal semantics。旧 `game6.txt` 只保留历史 manual acceptance / perf endgame / checkmate 价值，不再作为大赛第 6 条严格证明。

### 阶段 4：特征扩展与可学习性提升
- [ ] 按单类增量扩展特征，不把 threat、mobility、stage 一锅端
- [ ] 先补圆弧轨迹相关 N-Tuple，再补 threat-aware 特征，再补 mobility，再补阶段感知
- [ ] 为每一类新增特征补 ablation、兼容性和固定局面回归
- [ ] 补齐 quiescence / 连续吃子路径的评估口径
- [ ] 控制特征规模，避免把收敛速度一起拖死
- **状态：** blocked by opening performance instability / `perf_rating=regress` / GamesPerSeed=32 and GamesPerSeed=64 evidence still failing gate。
- **解除条件：** GamesPerSeed=32 至少达到 `watch`，且 GamesPerSeed=64 不 `regress`；否则不进入 Phase 4。当前 32 局最好组合仍 `regress`，64 局 decision run 仍 `regress`。
- **禁止事项：** 不进入 Phase 4；不扩 N-Tuple / threat / mobility / stage 特征；不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move；不改训练、搜索、特征、参数或 selection gate；不放宽 selection；不新增 `perf_watch`；不让 `strength_rating` 覆盖 `perf_rating`；不让 `repeatability_status` 参与 gate。

### 阶段 5：搜索增强与参数调优
- [ ] 在稳定权重上调 move ordering、killer/history 权重
- [ ] 调 Null Move Pruning 的启用条件与深度阈值
- [ ] 调 LMR 的 reduction 规则，尤其是 late move 降深
- [ ] 调 aspiration window 宽度与重试策略
- [ ] 针对残局考虑 extension 或特殊处理
- **状态：** pending

### 阶段 6：工程化与交付
- [ ] 整理 README 中 smoke / selection / release 的职责边界
- [ ] 标准化训练与 selection 的日志、目录结构和产物命名
- [ ] 把 failure handling 写清：训练失败、选择失败、权重不兼容、评估结果缺失
- [ ] 将 release workflow 与 smoke / selection 分离，但保持产物链路可追溯
- [ ] 将最终结论记录到 progress.md
- [ ] 以中文向用户交付下一阶段任务建议
- [ ] 说明推荐顺序与不推荐的误区
- **状态：** pending

## 关键问题
1. 当前仓库里方案 B 已实现到什么深度，哪些蓝图能力已经具备？
2. 下一阶段最值钱的缺口到底是基线与可追溯性、训练闭环，还是特征扩展？
3. 搜索增强应该在什么门槛之后才允许进入调参窗口？

## 已做决策
| 决策 | 理由 |
|------|------|
| 先探索仓库现状，再给下一阶段任务 | 蓝图里很多能力可能已经存在，不能重复造轮子 |
| 规划对象锁定为方案 B，不扩散到方案 A | 用户本轮明确要求对照方案 B 蓝图给下一阶段任务 |
| 先把 perf baseline 和 metadata 门禁前置 | 没有基线与可追溯性，后面的 selection 和 release 都是瞎蒙 |
| 特征扩展按单类增量推进 | 一次塞太多特征，连回归来源都分不清，训练只会更飘 |
| 搜索增强排在稳定权重之后 | 评估函数还没站稳就动搜索，容易把噪声放大成假强度 |
| rating split 只做 report-only 诊断 | 64 局 decision run 仍 `overall_rating=regress`，不能让 strength 信号覆盖 perf regress |

## 遇到的错误
| 错误 | 尝试次数 | 解决方案 |
|------|---------|---------|
| 误把仓库根当成 `surakarta-core-main` 子目录 | 1 | 重新确认 `D:\\Dev\\Code\\surakarta_traditional` 才是真实项目根 |
| PowerShell 在错误工作目录下报“目录名称无效” | 1 | 退回项目根执行探索命令 |

## 备注
- 历史早期备注是“只做规划”；2026-05-05 P0 事故轮已按用户要求修改生产代码和自测，范围仅限棋规回归修复。
- 重大结论已经补进 findings.md，后续只维护和这份计划相关的状态。

## 2026-05-25 Phase 3.36 诊断 dry-run 执行结论

- 已完成 Phase 3.36 guarded diagnostic dry-run execution 的证据包落地。
- 仅做 diagnostic dry-run / report-only / not selection eligible；没有 training run，没有 selection run，没有 `.bin`。
- 自测入口与输入链已复核，`BUILD_AND_DRY_RUN_SELFTEST_OK_PHASE3_36`、`INPUT_CHAIN_OK_PHASE3_36`、`STATUS_FLAGS_OK_PHASE3_36`、`WORDING_GUARD_AND_NO_BIN_OK_PHASE3_36` 均已拿到。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`。
- 下一步只允许 Phase 3.37 guarded limited training plan review；不允许直接训练。

## 2026-05-26 Phase 3.41 guarded active interface skeleton 结论

- 已完成 Phase 3.41 interface skeleton / default-off / not selection eligible 证据包落地。
- 训练入口保持为 `surakarta-benchmark.exe bitboard-train`；新增入口仅为单一受审 `--config <path>` 文件入口，没有增加散乱 objective toggle。
- config skeleton 默认关闭；缺省 config 不进入 skeleton，无效 config 在入口处拒绝，valid scoped config 只记录 skeleton 状态，不触发 active probe。
- 自测覆盖 default config absence、invalid config rejection、valid scoped config skeleton-only，构建与自测取得 `BUILD_AND_SELFTEST_OK_PHASE3_41`。
- 扫描验证取得 `CONFIG_GATED_INTERFACE_SKELETON_OK_PHASE3_41`；Phase 3.41 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_41`；文案/no-bin 取得 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_41`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；下一步允许进入 Phase 3.42 guarded active interface verification package，不允许直接 active probe。

## 2026-05-26 Phase 3.42 guarded active interface verification package 结论

- 已完成 Phase 3.42 verification-only / default-off / not selection eligible 证据包落地。
- 本轮未新增接口逻辑、未改 schema、未改 objective、未改训练逻辑。
- 复核 Phase 3.41 interface skeleton、Phase 3.40 interface plan、Phase 3.39 dry-run evidence 输入链，JSON 均可解析。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，自测取得 `BUILD_AND_SELFTEST_OK_PHASE3_42`。
- Phase 3.41 状态复核取得 `INPUT_CHAIN_AND_PHASE3_41_INTERFACE_OK_PHASE3_42`；静态边界复核取得 `ACTIVE_INTERFACE_STATIC_BOUNDARY_OK_PHASE3_42`。
- Phase 3.42 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_42`；文案/no-bin/protected path 取得 `WORDING_NO_BIN_AND_PROTECTED_DIFF_OK_PHASE3_42`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；下一步允许进入 Phase 3.43 active scoped probe dry-run plan review，不允许直接 probe execution。

## 2026-05-26 Phase 3.43 active scoped probe dry-run plan review 结论

- 已完成 Phase 3.43 probe-dry-run-plan-only / report-only / not selection eligible 证据包落地。
- 本轮未执行 active probe，未训练，未 selection，未生成 `.bin`，未修改代码、schema、脚本、tools 或 gate。
- 复核 Phase 3.42 verification package、Phase 3.41 skeleton、Phase 3.39 dry-run evidence 输入链，JSON 均可解析并输出 `input-json-ok`。
- 计划明确 future config 样例、`surakarta-benchmark.exe bitboard-train --config <path>` 命令模板、Phase 3.44 quarantine 输出目录、参数上限、监控指标、hard reject、rollback triggers 和停止条件。
- Phase 3.43 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_43`；文案/no-bin 取得 `WORDING_GUARD_AND_NO_BIN_OK_PHASE3_43`；保护路径复核取得 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_43`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；下一步允许进入 Phase 3.44 guarded active scoped probe dry-run execution，不允许正式训练或 selection。

## 2026-05-26 Phase 3.44 guarded active scoped probe dry-run execution 结论

- 已执行 Phase 3.44 quarantine-only dry-run 尝试，但结果为 blocker。
- 构建 `surakarta-benchmark` 与 `surakarta-bitboard-selftest`，自测取得 `BUILD_BENCHMARK_AND_SELFTEST_OK_PHASE3_44`。
- 原检查点无输出路径命令被当前入口拒绝：`output weight path is required`。
- 追加使用显式 quarantine 输出路径复跑，命令完成并生成 JSON evidence；输出仅落在 Phase 3.44 quarantine 目录。
- 观测结果为 `active_interface_skeleton_enabled=true`，但 `active_objective_probe_executed=false`；当前实现仍是 skeleton-only，不能如实宣称 active objective probe 已执行。
- 本轮未改代码、schema、脚本、tools 或 gate；未运行 selection，未生成 `.bin`，未生成 selection/candidate/checkpoint-like 文件。
- 取得 `WORDING_AND_PROTECTED_DIFF_OK_PHASE3_44` 和 `BLOCKER_STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44`；原成功态 `STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44` 未取得，原因是 active objective probe 未执行。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_44_guarded_active_scoped_probe_dry_run_execution`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；下一步进入 Phase 3.45 active scoped probe evidence review / blocker review，不进入更大训练或 selection。

## 2026-05-26 Phase 3.45 active scoped probe blocker review 结论

- 已完成 Phase 3.45 blocker review / quarantine evidence / not selection eligible 证据包落地。
- 本轮未改代码、schema、CLI、脚本、tools 或 gate；未重新 probe、未训练、未 selection、未生成新权重或 `.bin`。
- 复核 Phase 3.44 blocker 输入，取得 `BLOCKER_INPUTS_OK_PHASE3_45`。
- 复核 Phase 3.44 blocker 状态，取得 `PHASE3_44_BLOCKER_STATUS_CONFIRMED`。
- Root cause 明确为：`active_interface_skeleton_enabled=true`，但 `active_objective_probe_executed=false`；accepted active scoped config 只进入 skeleton state，没有接到 report-only active probe path。
- 执行语义缺口明确为：`bitboard-train` 当前仍要求 output weight path，缺少 no-output probe mode。
- `probe-quarantine-output.weights` 仅作为 quarantine evidence，禁止 selection、release 或 promotion 使用。
- Phase 3.45 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_45`；文案和 protected path 取得 `WORDING_AND_PROTECTED_DIFF_OK_PHASE3_45`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_45_active_scoped_probe_blocker_review`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；建议 Phase 3.46 进入 active probe wiring plan review，并把 no-output probe mode 作为强制执行安全项。

## 2026-05-26 Phase 3.46 active probe wiring and no-output mode plan review 结论

- 已完成 Phase 3.46 wiring-plan-only / no-output-mode-plan-only / not selection eligible 证据包落地。
- 本轮未改代码、schema、CLI、脚本、tools 或 gate；未执行 probe、训练、selection，未生成新权重或 `.bin`。
- 复核 Phase 3.45 blocker review、Phase 3.44 blocker evidence、Phase 3.41 skeleton 输入链，JSON 均可解析并输出 `input-json-ok`。
- 方案明确 future 最小改动面：`include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- 方案明确 accepted active scoped config 应进入 report-only active probe path，且只有实际 probe path 运行时才能设置 `active_objective_probe_executed=true`。
- 方案明确 no-output probe mode 为强制安全项：probe-only 时不得保存 candidate/checkpoint/production weights，不得要求 weight output path。
- Phase 3.46 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_46`；文案/no-weight artifact 取得 `WORDING_AND_NO_WEIGHT_ARTIFACT_OK_PHASE3_46`；保护路径复核取得 `PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_46`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_46_active_probe_wiring_no_output_mode_plan_review`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；允许进入 Phase 3.47 guarded active probe wiring skeleton，但不允许直接 probe execution。

## 2026-05-26 Phase 3.47 guarded active probe wiring skeleton 结论

- 已完成 Phase 3.47 wiring skeleton / no-output mode skeleton / not selection eligible 证据包落地。
- 本轮只接最小 wiring skeleton：accepted active scoped config 可以进入 report-only probe wiring skeleton，但不会执行真实 active probe。
- no-output probe mode skeleton 已加入：`probe_only=true` + `no_output_weights=true` 时允许不提供 output weight path，并抑制 weight save/checkpoint save 路径。
- `active_objective_probe_executed=false` 是硬状态；selection gate 仍不可用。
- 自测覆盖 default-off、invalid config、valid active scoped config、no-output mode、active probe not executed、weight artifact suppressed。
- 构建与 selftest 取得 `BUILD_AND_SELFTEST_OK_PHASE3_47`；静态边界取得 `WIRING_SKELETON_AND_NO_OUTPUT_BOUNDARY_OK_PHASE3_47`。
- Phase 3.47 manifest 状态取得 `STATUS_FLAGS_OK_PHASE3_47`；文案/no-bin/no-weight/protected path 取得 `WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_47`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_47_guarded_active_probe_wiring_skeleton`。
- 结论维持：`phase3_status=not_passed`，`phase4_status=blocked`；允许进入 Phase 3.48 guarded active probe wiring verification package，不允许直接 probe execution。
