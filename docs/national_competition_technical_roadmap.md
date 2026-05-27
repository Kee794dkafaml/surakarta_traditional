# Surakarta Traditional 项目总纲与国赛技术路线图

## 2026-05-26 Phase 3.62 commit staging plan 补记

- Phase 3.62 已完成 commit staging plan and tracked evidence materialization review。
- 本阶段不 stage、不 commit、不 stash、不 reset、不 checkout、不 clean；不执行 active probe、dry-run、training、selection、promotion 或 `GamesPerSeed=64`。
- 当前锚点已确认：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- `build-local` evidence package 被 `.gitignore:4:/build-local/` 忽略；后续提交默认只纳入 tracked 文档中的摘要、SHA256、路径索引和复跑命令，不强行搬运大包。
- 建议提交分组：objective diagnostic code 仅做人工 partial hunk staging；Phase 3 diagnostic evidence docs 单独提交；archive ownership review 以 tracked 摘要方式提交。
- 必须排除或单独确认：bitboard/rule semantics 历史脏改动、perf baseline、untracked `tools\` 和所有非 objective diagnostic hunk。
- 状态继续固定：`phase3_diagnostic_objective_blocker_closed=true`、`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一步只建议人工授权后的 staging execution campaign 或 Phase 4 precondition planning；不得直接进入 training、selection 或 promotion。

## 2026-05-26 Phase 3.61 evidence archive ownership review 补记

- Phase 3.61 已完成 evidence archive and commit preparation ownership review。
- 本阶段只做 Phase 3.50-3.60 证据归档索引、提交准备分组、排除清单、风险清单和 hunk 级归属判断。
- 本阶段未 stage、未 commit、未 stash、未 reset、未 checkout、未清理未跟踪文件；未执行 active probe、dry-run、training、selection 或 promotion。
- 最终状态继续固定：`phase3_diagnostic_objective_blocker_closed=true`、`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 推荐提交分组：objective diagnostic code 需要 partial staging；Phase 3 evidence docs 可单独成组；Phase 3.61 ownership review 可单独成组。
- 必须排除或单独确认：national terminal/rule semantics、perf baseline、untracked `tools\`、非 objective diagnostic 的历史脏改动。

## 2026-05-26 Phase 3.60 final evidence consolidation 补记

- Phase 3.60 已完成 final evidence consolidation and Phase 4 precondition map。
- 本阶段只汇总 Phase 3.47-3.59 证据链，形成最终证据索引、状态矩阵、剩余缺口清单和 Phase 4 前置条件地图。
- 本阶段不得执行 active probe、dry-run、training、selection、promotion 或 `GamesPerSeed=64`。
- 当前锚点已确认：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- Phase 4 precondition map 只允许列条件和规划边界，不授权训练、selection 或 promotion。
- 最终状态固定为：`phase3_diagnostic_objective_blocker_closed=true`、`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一步只建议 Phase 4 precondition planning 或 Phase 3 evidence archive/commit preparation。

## 2026-05-26 Phase 3.58-3.59 objective evidence review 补记

- Phase 3.58-3.59 已完成 objective evidence review and Phase 3 completion gate decision。
- 本阶段只允许审查 Phase 3.56-3.57 objective diagnostic evidence、文档化分层结论并生成 verify 包。
- 本阶段不得进入新的 active probe dry-run、training、selection、promotion 或 `GamesPerSeed=64`。
- 当前锚点已确认：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- Phase 3.56-3.57 证据复核通过：`active_objective_probe_executed=true`、report-only/no-output、artifact suppression、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- Completion gate 口径已分层：`phase3_diagnostic_objective_blocker_closed=true`；但 `phase3_status=not_passed`、`phase4_status=blocked`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一步只建议 Phase 3.60 final evidence consolidation 或 Phase 4 precondition planning；不得直接进入 training、selection 或 promotion。

## 2026-05-26 Phase 3.56-3.57 objective evidence 补记

- Phase 3.56-3.57 已完成 objective-execution report-only blocker closure and guarded verification campaign。
- 本阶段只做最小代码闭环：`RunBitboardTraining` 在 guarded report-only/no-output path 下设置 `active_objective_probe_executed=true`，并输出 `report_only_objective_diagnostic_executed_no_output`。
- 自测补强覆盖 default-off false、invalid config false、guarded no-output true、artifact suppression、consumer isolation、ordinary output path false。
- 隔离 dry-run exit code `0`，stderr 为空；stdout 显示 `active_objective_probe_executed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- 输出目录未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一步只允许 Phase 3.58 objective evidence review / Phase 3 completion gate review；不进入 training、selection 或 promotion。

## 2026-05-26 Phase 3.55 evidence interpretation 补记

- Phase 3.55 已完成 dry-run evidence interpretation and objective-execution blocker closure package。
- 本阶段不修改生产代码，不再次执行 active probe，不训练，不 selection，不 promotion。
- Phase 3.53-3.54 证据解释为：CLI command path runnable，guarded report-only/no-output path 可运行，artifact suppression 有效。
- 同一证据没有形成 objective execution evidence：`active_objective_probe_executed=false`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- 只读源码复核显示 no-output path 在保存权重、checkpoint 和训练循环前返回；当前没有 report-only objective diagnostic helper。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`、`active_objective_probe_executed=false`。
- 下一步只允许 Phase 3.56 objective-execution blocker closure plan，或单独授权后的最小实现任务；不能进入 training、selection 或 promotion。

## 2026-05-26 Phase 3.53-3.54 readiness + dry-run 补记

- Phase 3.53-3.54 已完成 guarded report-only probe readiness closure and dry-run evidence campaign。
- 重新运行 build、benchmark selftest、Phase 3.52 verify；build exit code `0`，selftest 输出 `[PASS] surakarta-bitboard-selftest`，Phase 3.52 verify 输出全部 marker。
- 已执行一次 `surakarta-benchmark.exe bitboard-train --config <guarded-report-only-no-output-config.json> --format json`；不提供 `--output`，配置固定 `report_only=true`、`probe_only=true`、`no_output_weights=true`、`selection_gate_eligible=false`。
- dry-run exit code `0`，stderr 为空；stdout 显示 `active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- 输出目录未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- `active_objective_probe_executed=false` 仍保持；本阶段只能证明 guarded report-only/no-output command path 可运行，不证明 objective execution，不改变 selection gate。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- 下一步只允许 Phase 3.55 evidence review / diagnostics interpretation / continued blocker closure；不进入 training、selection 或 promotion。

## 2026-05-26 Phase 3.52 minimal implementation 补记

- Phase 3.52 已完成 report-only active probe path minimal implementation。
- 本轮只补最小代码通路和自测：summary 增加 `active_interface_report_only_probe_path`，no-output guarded path 报告 implementation-only 状态，CLI summary 同步输出。
- 自测覆盖 default-off、invalid config、guarded no-output config、artifact suppression、consumer isolation。
- `surakarta-benchmark.exe bitboard-selftest` 作为验收入口转发到同目录 selftest 可执行文件。
- 本轮未修改 `tools`、`scripts`、`test\test_data`，未执行真实 probe、training、selection、promotion 或 `GamesPerSeed=64`。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。
- 下一步只允许 Phase 3.53 guarded execution readiness review，不直接进入 training 或 selection。

## 2026-05-26 Phase 3.51 implementation plan 补记

- Phase 3.51 已完成 report-only active probe path implementation plan。
- 本轮只制定后续最小实现计划，不修改生产代码，不执行 probe、training、selection 或 `GamesPerSeed=64`。
- 最小未来边界：沿用 `surakarta-benchmark.exe bitboard-train --config <path>`，保持 default-off、opening-only、report-only、quarantine-only、`probe_only=true`、`no_output_weights=true`、`selection_gate_eligible=false`。
- Future Phase 3.52 只可在单独授权后做最小实现任务，目标是把当前 skeleton/no-output boundary 接到 report-only diagnostic route；不得接 selection/rating/scorecard/perf threshold/promotion/release。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。

## 2026-05-26 Phase 3.50A acceptance audit addendum 补记

- Phase 3.50A 已完成 acceptance audit addendum；本轮不改变 Phase 3.50 blocker closure 结论，也不进入 Phase 3.51。
- `verify_phase3_50.ps1` 已输出验收要求 marker：`PHASE3_49_BLOCKER_INPUT_OK_PHASE3_50`、`STATUS_FLAGS_OK_PHASE3_50`、`WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_50`。
- Addendum 明确历史 scoped diff 口径：当前 `src/include/test_data` diff 是历史脏状态，不是 Phase 3.50A 新增改动；验证脚本会检查该集合是否漂移。
- Protected scripts 与 `tools` 文件已记录文件级 SHA256，避免用模糊话术糊弄验收。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。

## 2026-05-26 Phase 3.50 blocker closure plan 补记

- Phase 3.50 已完成 guarded active probe blocker closure plan 与 3.51 readiness gate。
- 本轮只做分析、计划、manifest、验证脚本和进度文档更新；不执行 probe、training、selection 或 `GamesPerSeed=64`。
- 输入证据链维持原结论：Phase 3.47 只证明 wiring/no-output skeleton，Phase 3.48 只做 verification-only，Phase 3.49 明确 `ready_for_guarded_probe=false`。
- 当前 blocker：真实 report-only active probe path 未被证明；accepted config path 仍只证明 skeleton/no-output boundary。
- Phase 3.51 可开工，但只限 report-only active probe path implementation plan 或单独授权的最小实现任务；当前不能直接开真实 probe。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`ready_for_guarded_probe=false`。

## 2026-05-26 Phase 3.49 readiness review 补记

- Phase 3.49 已完成 guarded active probe execution readiness review。
- 本轮只做 readiness-only 审查，不执行 probe、training、selection 或 `GamesPerSeed=64`。
- Phase 3.47 证明 wiring skeleton 与 no-output mode skeleton 存在；Phase 3.48 证明该边界已被 verification-only 复核。
- Readiness 结论：`ready_for_guarded_probe=false`。
- Blocker：当前证据链没有证明真实 report-only probe path 已可运行；只能说明下一步要先补执行路径证明，不能直接开跑。
- 状态继续保持 `phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`。

## 2026-05-26 Phase 3.40 active scoped interface plan review

### 状态
- **Phase 3.40：** completed as interface-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review`。
- **证据包：** `active-scoped-interface-plan-review.md`、`active-scoped-interface-plan-review.json`、`active-scoped-interface-plan-review-manifest.json`。

### 结论
- Phase 3.40 只完成 future active scoped interface 的计划评审：入口形态、默认关闭语义、允许参数、禁止参数、隔离输出、验证矩阵和 rollback triggers。
- 当前可用训练入口仍是 `surakarta-benchmark.exe bitboard-train`；当前没有 active scoped objective activation flags。本阶段未新增 CLI flag，未修改代码，未训练，未 selection，未生成 `.bin`。
- Future 最小接口建议使用受审 config 文件入口，继续绑定到既有 `bitboard-train` subcommand；默认关闭，必须显式启用，scope 限 opening-only，输出必须 quarantine-only。
- `hard_reject`、opening guard、root-cost、acceptance companion 继续只能 report-only；不得进入 gate、rating、scorecard 或 perf threshold。
- manifest 状态保持 `interface_plan_only=true`、`interface_implemented=false`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`bin_generated=false`、`production_candidate=false`、`selection_gate_eligible=false`。
- `phase3_status=not_passed`，`phase4_status=blocked`。下一步可进入 Phase 3.41 guarded active interface skeleton；不是直接 active probe。

## 2026-05-25 Phase 3.39 guarded limited training dry-run execution

### 状态
- **Phase 3.39：** completed as default-off dry-run / isolated / quarantine-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution`。
- **证据包：** `guarded-limited-training-dry-run-execution-report.md`、`guarded-limited-training-dry-run-execution-report.json`、`guarded-limited-training-dry-run-execution-manifest.json`。

### 结论
- 本阶段执行了一次最小 default-off training dry-run：`games=1`、`depth=1`、`seed=20260423`，权重和 checkpoint 均隔离在 Phase 3.39 quarantine 子目录。
- 当前工程没有 `bitboard-train` CMake target 或 `bitboard-train.exe`；实际训练入口是既有 `surakarta-benchmark.exe bitboard-train`，本阶段未新增 CLI、未修改代码。
- 当前 CLI 无已评审 objective activation flags；active objective probe 未执行，也不应在 Phase 3.39 里硬塞。
- manifest 状态保持 `training_dry_run_executed=true`、`default_off=true`、`active_objective_probe_executed=false`、`selection_executed=false`、`production_candidate=false`、`selection_gate_eligible=false`、`quarantine_only=true`。
- 未运行 selection，未跑 `GamesPerSeed=64`，未 promotion，未修改 gate/rating/scorecard/perf threshold。
- `phase3_status=not_passed`，`phase4_status=blocked`。下一步可进入 Phase 3.40 active scoped interface plan review；这只是接口计划评审许可，不是 active probe 或 Phase 4 许可。

## 2026-05-25 Phase 3.38 guarded limited training dry-run plan

### 状态
- **Phase 3.38：** completed as training-dry-run-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_38_guarded_limited_training_dry_run_plan`。
- **证据包：** `guarded-limited-training-dry-run-plan.md`、`guarded-limited-training-dry-run-plan.json`、`guarded-limited-training-dry-run-plan-manifest.json`。

### 结论
- Phase 3.38 只审 future Phase 3.39 dry-run execution 方案，不执行训练、不执行 selection、不生成 `.bin`。
- future 输出必须隔离在 Phase 3.39 专属目录，权重和 checkpoint 只能进入 quarantine 子目录，不能覆盖历史 session 或任何候选/基线/发布产物。
- 当前 CLI 只支持 default-off training surface；active scoped objective probe 需要单独接口授权。
- 允许进入 Phase 3.39 guarded limited training dry-run execution，范围限定为 default-off / isolated / quarantine-only。
- `phase3_status=not_passed`，`phase4_status=blocked`，`selection_gate_eligible=false`。

## 2026-05-25 Phase 3.37 guarded limited training plan review

### 状态
- **Phase 3.37：** completed as training-plan-only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_37_guarded_limited_training_plan_review`。
- **证据包：** `guarded-limited-training-plan-review.md`、`guarded-limited-training-plan-review.json`、`guarded-limited-training-plan-review-manifest.json`。

### 结论
- Phase 3.37 只审 future guarded limited training 方案，不执行训练、不执行 selection、不生成 `.bin`。
- 最小未来边界：单 seed `20260423`、最多 4 局、depth 4、checkpoint every 2、TD 默认参数，opening objective 只允许显式 scoped 小权重诊断 probe。
- 必须先进入 Phase 3.38 guarded limited training dry-run plan；Phase 3.37 本身不给实际训练许可。
- `phase3_status=not_passed`，`phase4_status=blocked`，`selection_gate_eligible=false`。

## 2026-05-25 Phase 3.35 guarded diagnostic dry-run plan

### 状态
- **Phase 3.35：** completed as dry-run plan only / report-only / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_35_guarded_diagnostic_dry_run_plan`。
- **证据包：** `guarded-diagnostic-dry-run-plan.md`、`guarded-diagnostic-dry-run-plan.json`、`guarded-diagnostic-dry-run-plan-manifest.json`。

### 结论
- Phase 3.35 只规划 future guarded diagnostic dry-run：观察 default-off / inactive enabled / active scoped / hard reject report-only 四类样本的诊断字段。
- objective 已实现且 default-off，`training_loss_modified=true` 是计划内实现事实；但本阶段没有执行 dry-run，没有训练，没有 selection，没有 `.bin`，没有 gate 消费者变化。
- 下一步只建议 Phase 3.36 guarded diagnostic dry-run execution plan 或 blocker review；不能直接进入训练流水线。

## 2026-05-25 Phase 3.34 guarded implementation verification package

### 状态
- **Phase 3.34：** completed as verification-only / default-off / not selection eligible package。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_34_guarded_implementation_verification_package`。
- **证据包：** `guarded-implementation-verification-package.md`、`guarded-implementation-verification-package.json`、`guarded-implementation-verification-package-manifest.json`。

### 结论
- Phase 3.33 的 guarded objective path 已按 default-off 边界复核；`training_loss_modified=true` 是计划内实现事实，不是事故。
- 该路径仍未进入训练、selection 或 gate，`selection_gate_eligible=false`，`production_candidate=false`。
- Phase 3.34 未新增代码逻辑，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- 下一阶段只建议 Phase 3.35 guarded diagnostic dry-run plan；不能直接训练或把 candidate 资格往 true 上掰。

## 2026-05-25 Phase 3.33 guarded minimum-objective implementation

### 状态
- **Phase 3.33：** completed as guarded implementation / default-off / not selection eligible。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_33_guarded_minimum_objective_implementation`。
- **证据包：** `guarded-minimum-objective-implementation-report.md`、`guarded-minimum-objective-implementation-report.json`、`guarded-minimum-objective-implementation-manifest.json`。
- **代码边界：** 只在 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\surakarta_bitboard_selftest.cpp`、`src\main.cpp` 内完成最小路径和自测。
- **default-off：** 新 option 默认 false/zero/empty；CLI/default path 不启用 opening-safe objective。
- **最小公式：** active scoped path 计算 `opening_drift_penalty_weight * opening_drift_penalty_unsafe_rank_degradation * opening_drift_penalty_root_cost_multiplier`。
- **training loss：** active scoped path 会影响 TD target；默认关闭和 inactive path 经自测保持等价。
- **report-only：** hard reject、root-cost 和 opening guard 口径仍只做 companion 证据，不进入 rating、scorecard、perf threshold 或 selection gate。
- **禁改范围：** 未修改 `tools\*`、三个 benchmark/perf 脚本、selection gate。
- **未执行：** 未运行训练流程，未跑 selection，未跑 `GamesPerSeed=64`，未生成阶段 `.bin`。
- **状态边界：** Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。这阶段是把受控电闸装上，不是直接拉满功率开炼。

### 下一步
- 允许进入 verification/diagnostic review，重点复查 default-off 等价、active scoped 诊断、hard reject report-only 和 protected path diff。
- 不允许训练、selection、`.bin` 或 gate 改动；任何扩大范围都要重新授权。

## 2026-05-24 Phase 3.32 minimum objective implementation plan review

### 状态
- **Phase 3.32：** completed as implementation-plan-only / design-only / report-only review。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_32_minimum_objective_implementation_plan_review`。
- **证据包：** `minimum-objective-implementation-plan-review.md`、`minimum-objective-implementation-plan-review.json`、`minimum-objective-implementation-plan-review-manifest.json`。
- **未来最小改动面：** 后续单独授权时只允许围绕 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`、必要时 `scripts\workflow.tests.ps1`、Phase 输出目录和四个项目文档。
- **禁止面：** `tools\*`、三个 benchmark/perf 脚本、selection gate、rating、scorecard、perf threshold、promotion、自博弈、搜索参数、eval/search、tuple/features、训练产物、`.bin`、Phase 4。
- **验证矩阵：** default-off、inactive equivalence、formula helper、active scoped fixture、hard reject、report-only wiring、CLI/report、forbidden path guard、artifact guard。
- **回滚线：** 禁改路径变化、默认值偏离 disabled/zero/empty/false、disabled path 改行为、companion 被 forbidden consumer 消费、训练/selection/`GamesPerSeed=64`/`.bin` 出现，全部停止。
- **hard reject：** 仍只进入 `report-only acceptance companion`；不得进入 rating、scorecard、perf threshold 或 selection gate。
- **下一步：** 可在单独授权后进入 guarded minimum-objective work；本阶段只完成计划评审，不是动代码许可。
- **状态边界：** Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。先把施工方案钉牢，再谈动扳手；不然就是边开车边装方向盘。

## 2026-05-24 Phase 3.31 minimum objective formula design review

### 状态
- **Phase 3.31：** completed as design-only / formula-design-only / report-only review。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_31_minimum_objective_formula_design_review`。
- **证据包：** `minimum-objective-formula-design-review.md`、`minimum-objective-formula-design-review.json`、`minimum-objective-formula-design-review-manifest.json`。
- **公式边界：** 最小设计公式为 `opening_safe_penalty = opening_drift_penalty_weight * unsafe_rank_degradation * bounded_root_cost_multiplier * opening_scope_mask`，仅作为未来实现前的设计合同。
- **输入输出：** JSON 已列出 default-off 配置、root-child relationship evidence、bounded companion status、hard reject companion 输入，以及 scope/rank/cost/penalty/diagnostics 输出语义。
- **hard reject：** 只进入 `report-only acceptance companion`；不进入 selection gate、rating、scorecard、perf threshold 或 training loss。
- **不可实现项：** 不实现 objective，不改 training loss / TD target / trace / self-play / eval/search / weight update，不训练，不 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`。
- **下一步：** 允许进入 implementation plan review，但只限计划评审；不授权动代码实现。
- **状态边界：** Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。别把“公式写清楚了”当“模型能打了”，这俩差着十万八千里。

## 2026-05-24 Phase 3.30 skeleton validation evidence package

### 状态
- **Phase 3.30：** completed as skeleton validation evidence package / report-only summary。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_30_skeleton_validation_evidence_package`。
- **证据包：** `skeleton-validation-evidence-package.json`、`skeleton-validation-evidence-package-manifest.json`。
- **汇总结论：** Phase 3.25 default-off skeleton、Phase 3.26 inactive equivalence、Phase 3.27 scoped activation no-op、Phase 3.28 report-only wiring、Phase 3.29 hard reject fixture 证据链完成收束。
- **下一步许可：** `allow_phase3_31_minimum_objective_formula_design_review=true`，只允许公式设计评审；不允许实现、不允许 training loss 修改、不允许训练/selection/`.bin`。
- **代码边界：** 本阶段未修改 `src\*`、`include\*`、`tools\*` 或三个 benchmark/perf 脚本。
- **状态边界：** Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`；别把账本装订完成当成比赛赢了，账本不是奖杯。

## 2026-05-24 Phase 3.29 hard reject fixture proof

### 状态
- **Phase 3.29：** completed as hard reject fixture proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_29_hard_reject_fixture_proof`。
- **证据包：** `hard-reject-fixture-proof-report.json`、`hard-reject-fixture-proof-manifest.json`。
- **代码边界：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 selftest-only hard reject fixture matrix；不实现 opening objective 公式。
- **hard reject 覆盖：** Phase 3.22 / 3.24 定义的 final `20260423` side-effect、qnodes increase + root over-neutralization、任一 final seed new best + qnodes increase、非 opening regress、C/B/D/fixed/perf 恶化、default-off/equivalence 失败、companion 被 gate 消费、diagnostic `.bin` promotion、`all_negative` / tuple-shrinkage-first 均有 fixture 条目。
- **report-only：** hard reject 只进入 `report-only evidence`；不进入 selection gate、rating、scorecard、perf threshold 或 training loss。
- **禁改范围：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold 未修改。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成阶段 `.bin`。
- **验证入口：** `VsDevCmd.bat -arch=x64 -host_arch=x64` + `D:\Dev\Code\surakarta_traditional\build-vs-release` + target/exe `surakarta-bitboard-selftest`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.28 report-only diagnostics wiring proof

### 状态
- **Phase 3.28：** completed as report-only diagnostics wiring proof / marker / evidence only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_28_report_only_diagnostics_wiring_proof`。
- **证据包：** `report-only-diagnostics-wiring-proof-report.json`、`report-only-diagnostics-wiring-proof-manifest.json`。
- **代码边界：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加 selftest-only wiring proof；不实现 opening objective 公式。
- **训练语义：** TD target、training loss、eligibility trace、self-play move selection、eval/search、权重更新均不变。
- **report-only wiring：** hard reject、opening guard、root-cost companion、acceptance companion 只进入 `report-only evidence chain`；不进入 selection gate、rating、scorecard、perf threshold 或 training loss。
- **禁改范围：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold 未修改。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成阶段 `.bin`。
- **验证入口：** `VsDevCmd.bat -arch=x64 -host_arch=x64` + `D:\Dev\Code\surakarta_traditional\build-vs-release` + target `surakarta-bitboard-selftest`；实际自测产物名为 `surakarta-bitboard-selftest.exe`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.27 scoped activation no-op fixture

### 状态
- **Phase 3.27：** completed as scoped activation no-op fixture / explicit enabled skeleton / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_27_scoped_activation_noop_fixture`。
- **证据包：** `scoped-activation-noop-fixture-report.md`、`scoped-activation-noop-fixture-report.json`、`scoped-activation-noop-fixture-manifest.json`。
- **代码边界：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 fixture 增加显式 enabled skeleton no-op case。
- **训练语义：** TD target、training loss、eligibility trace、self-play move selection、eval/search、权重更新均不变。
- **验证语义：** enabled skeleton path 与 baseline training step 的 weights、traces、current value、target value、td error、abs weight delta、changed weight count 一致。
- **禁改范围：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold 未修改。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成阶段 `.bin`。
- **验证入口备注：** 当前可用入口仍是 `VsDevCmd.bat -arch=x64 -host_arch=x64` + `build-vs-release` + target `surakarta-bitboard-selftest`；用户给定命令中的 `build-local` 和 underscore target 不是当前仓库 CMake 入口。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.26 scoped inactive-equivalence verification

### 状态
- **Phase 3.26：** completed as scoped inactive-equivalence verification / default-off / no-op verification only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_26_scoped_inactive_equivalence_verification`。
- **证据包：** `scoped-inactive-equivalence-verification-report.md`、`scoped-inactive-equivalence-verification-report.json`、`scoped-inactive-equivalence-verification-manifest.json`。
- **代码边界：** 仅在 `src\surakarta_bitboard_selftest.cpp` 既有 inactive equivalence fixture 增加 Phase 3.26 marker。
- **训练语义：** TD target、training loss、eligibility trace、self-play move selection、eval/search、权重更新均不变。
- **禁改范围：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold 未修改。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成阶段 `.bin`。
- **验证入口：** 用户给定 `Community` CMake 路径在本机不存在；实际入口为先导入 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat`，再使用 `D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` 构建 `surakarta-bitboard-selftest`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.25 guarded implementation skeleton

### 状态
- **Phase 3.25：** completed as guarded implementation skeleton / default-off / no-op。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_25_guarded_implementation_skeleton`。
- **代码边界：** 只新增 opening-safe 训练选项骨架、inactive metadata 和 selftest；不实现 opening objective 公式。
- **训练语义：** TD target、training loss、eligibility trace、self-play move selection、eval/search、权重更新均不变。
- **report-only：** 输出 `opening_safe_objective_enabled`、`opening_drift_penalty_active`、`opening_drift_penalty_value`、`opening_drift_penalty_scope_status`、`inactive_path_equivalent`、`hard_reject_triggered`、`hard_reject_reasons`，不进入 gate/rating/scorecard/perf threshold。
- **禁改范围：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold 未修改。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.24 guarded implementation dry-run plan

### 状态
- **Phase 3.24：** completed as design-only / dry-run-plan-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_24_guarded_implementation_dry_run_plan`。
- **生成产物：** `guarded-implementation-dry-run-plan.md`、`guarded-implementation-dry-run-plan.json`、`guarded-implementation-dry-run-plan-manifest.json`。
- **结论：** `allow_phase3_25_guarded_implementation_skeleton=true`；只允许进入 guarded implementation skeleton，不允许本阶段实现。
- **dry-run 最小拆分：** preflight hash snapshot、default-off skeleton tests/options、inactive equivalence fixtures、inactive no-op hook、scoped activation fixture、report-only diagnostics、hard reject fixtures、dry-run evidence package。
- **future allowed touch：** `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`、必要时 `scripts\workflow.tests.ps1`、Phase 输出目录和四个项目文档。
- **forbidden touch：** `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold、tuple/feature expansion、搜索参数、`.bin` 生成或 promotion、Phase 4。
- **验证口径：** before/after hash、default-off verification matrix、inactive path equivalence matrix、hard reject report-only fixture、禁改路径监控和 rollback trigger 全部写入 plan。
- **hard reject：** final `20260423` side-effect、非 opening drift/score regress、C/B/D 或性能恶化、default-off/equivalence 失败、companion 被 gate 消费、diagnostic `.bin` promotion、`all_negative` 或 tuple shrinkage-first，只能 report-only，不进 gate。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.23 opening-safe objective implementation plan review

### 状态
- **Phase 3.23：** completed as design-only / implementation-plan-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_23_implementation_plan_review`。
- **生成产物：** `opening-safe-objective-implementation-plan-review.md`、`opening-safe-objective-implementation-plan-review.json`、`opening-safe-objective-implementation-plan-review-manifest.json`。
- **结论：** `allow_phase3_24_guarded_implementation_dry_run_plan=true`；只允许进入 guarded implementation dry-run plan，不允许本阶段实现。
- **future 最小改动面：** 后续获批时仅限 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`，必要时 `scripts\workflow.tests.ps1`。
- **禁止面：** 不改 `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard、perf threshold、搜索参数、tuple/feature expansion、`.bin` promotion，不进入 Phase 4。
- **验证口径：** default-off 必须默认关闭；inactive equivalence 必须证明 `ApplyTrainingStep`、`RunBitboardTraining`、CLI defaults、selection summary 在关闭时等价。
- **hard reject：** final `20260423` new best / qnodes increase / root over-neutralization 等条件进入 report-only acceptance companion，不进入 selection gate。
- **rollback：** 禁改路径变化、默认开启、inactive equivalence 失败、gate/rating/scorecard 变化、companion 被 gate 消费、非 opening 被影响、final side-effect 出现或未建模，均停止并回滚。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.22 opening-safe objective minimum prototype design

### 状态
- **Phase 3.22：** completed as design-only / report-only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_22_objective_minimum_prototype_design`。
- **生成产物：** `opening-safe-objective-minimum-prototype-design.md`、`opening-safe-objective-minimum-prototype-design.json`、`opening-safe-objective-minimum-prototype-manifest.json`。
- **设计结论：** 允许进入 `Phase 3.23 implementation plan review`，但不允许进入 implementation。
- **最小 objective：** default-off / opening-only / soft penalty，约束 unsafe baseline root-order degradation 和 costly PV migration。
- **硬边界：** 不硬锁 baseline best move；final `20260423` side-effect / root over-neutralization 是 hard reject；inactive path 必须证明与当前训练路径等价。
- **companion：** candidate acceptance guard 继续 report-only wrapper；root-cost companion 继续旁路诊断，不进入 gate / loss。
- **tuple shrinkage：** 仍排在 opening drift penalty 之后，禁止 all_negative，禁止 diagnostic-neutralized `.bin` candidate。
- **未来最小改动面：** 只有后续实现获批时才可能涉及 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`，必要时 `scripts\workflow.tests.ps1`。
- **禁止面：** 不改 `tools\`，不改三个 benchmark/perf 脚本，不改 selection gate/rating/scorecard，不改 perf threshold，不扩特征，不进 Phase 4。
- **未执行：** 未实现 objective，未修改 training loss，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

## 2026-05-24 Phase 3.21 opening-safe objective readiness review

### 状态
- **Phase 3.21：** completed as readiness review / design gate only。
- **输出目录：** `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_21_objective_readiness_review`。
- **生成产物：** `objective-readiness-review.md`、`objective-readiness-review.json`、`objective-readiness-review-manifest.json`。
- **readiness 结论：** `conditionally_ready`。
- **允许进入：** `Phase 3.22 opening-safe objective minimum prototype design`；不允许进入 implementation。
- **设计含义：** cp2 strong explanation 支持围绕 unsafe opening root-order drift / costly PV migration 设计 default-off soft penalty；final partial explanation 要求把 `20260423` side-effect 写成硬拒绝条件。
- **实施顺序：** candidate acceptance guard companion -> root-cost diagnostic companion -> opening drift penalty -> distributed tuple shrinkage 仍成立。
- **边界：** guard/root-cost 继续 report-only，不进 selection gate；opening drift penalty 只可先设计；tuple shrinkage 仍后置且禁止 all_negative。
- **未执行：** 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate，未进入 Phase 4。
- **生产代码：** 未修改 `src\` / `include\`。
- **tools：** 未修改。
- **禁改脚本：** 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- **Phase 3：** still not passed。
- **Phase 4：** still blocked。

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


> 生成时间：2026-05-21
> 范围：基于当前仓库真实代码、文档、记忆文件和本轮自动检查结果；不修改生产代码、不改棋规、不改搜索、不改训练、不放宽 selection gate。
> 当前定位：校赛可用版本已经成立；国赛水平仍需要按阶段补齐规则语义、可信基线、训练稳定性、评估特征、搜索时间管理和长期评测体系。

> 2026-05-22 状态更新：阶段 A、B1、B2.2 已封口；大赛第 6 条 national terminal semantics 已落地；`test/test_data/perf-baseline.json` 已替换为 post-B1 aggregate official baseline，且两次默认 perf gate 通过。Phase 3.1 fresh post-B1 `GamesPerSeed=32` selection 已完成，结果 `overall_rating=strength_rating=perf_rating=regress`；按停止条件不跑 64。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-22 Phase 3.2 状态更新：已完成 opening blocker root-cause attribution。opening best move drift 在三个 seed 的 checkpoint-2 即出现，search timing 超阈同步出现，node inflation 在 checkpoint-4/6 跟随；最终三 seed 均收敛到 `(3, 1) -> (2, 2) (B)`，但该最终 move 不是最早 drift。结论是不进入 64 局评审、不进入 Phase 4，下一步先做 opening-specific 诊断/最小 ablation。

> 2026-05-22 Phase 3.3 状态更新：已完成 opening root diagnostics and minimal attribution。产物在 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics`。cp2 首次 drift 三 seed 均为 `(5, 1) -> (4, 2) (B)`，root order rank `1`，baseline 退到 order rank `2`，但 observed score 是 rank-1/bound-tie，不是明确全宽 score 压低；cp2 无 aspiration retry，首次 qnode inflation 不能归因于 retry。后期 final candidate 均收敛到 `(3, 1) -> (2, 2) (B)`，qnodes 主要集中在当前 PV root move。现有 CLI 无 active tuple / weight attribution，下一步只能先做 report-only diagnostic extension / 最小 ablation 设计。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-22 Phase 3.4 状态更新：已完成 opening active-tuple attribution and minimal ablation design。现有 CLI 单独不能输出 active tuple attribution，但权重格式为 24-byte header + 16605 个 int32，tuple descriptor / offset / side-to-move perspective 已审清，可以不改生产代码用离线小脚本完成 9 个权重 × 3 个 opening root child position 的 report-only static attribution。若要避免脚本复制 tuple descriptor，下一阶段可做默认关闭的 `bitboard-attrib` / `bitboard-eval --explain-tuples` 诊断扩展；它不得改变搜索、训练、selection 行为。推荐最小 ablation 为 attribution-only，其次 child-position static ablation；Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-22 Phase 3.5 状态更新：已完成 opening attribution-only execution。新增非生产离线脚本 `tools\opening-attribution.ps1`，从 `weights-manifest.json` 解析 3 seed × `bootstrap/cp2/final` 共 9 个 artifact，并对 baseline/cp2 drift/final drift child positions 输出 static attribution。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution`；summary `27` 行、tuple detail `1215` 行、active tuple count 全部 `45`，artifact hash/header 全部 pass。static eval delta 能解释 cp2/final drift child 相对 baseline child 的偏好方向，但未发现少数 tuple 主导（36 个 comparison 全部 dominance `no`）。这足够进入 child-position static ablation 设计，不支持 offline neutralization，不进入 Phase 4。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-22 Phase 3.6 状态更新：已完成 child-position static ablation design，未执行 ablation、未新增脚本、未生成权重、未重新训练、未跑 `GamesPerSeed=64`、未进入 Phase 4。下一步若执行 static ablation，应比较 baseline child、cp2 drift child、final drift child 的 cp2 onset、final blocker、final residual 和 cp2-to-final transition；neutralization 单元必须是 stage-vs-bootstrap 的 `induced_pair_delta < 0`，而不是直接抓 same-stage top1。TopK 采用 `1/3/5/10/20/all_negative` 累积曲线，因为 Phase 3.5 dominance 全 `no`，top1/top3 abs share 最高只有 `0.165094/0.376866`。当前仍不支持 offline neutralization；Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-22 Phase 3.7 状态更新：已完成 child-position static ablation report-only/table-only execution。新增非生产只读脚本 `tools\child-position-static-ablation.ps1`，输出目录 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`；summary `7` 行、selected tuple detail `174` 行、TopK sweep `126` 行。主 comparison `C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`：K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring；control 全部 pass。结果足够进入 offline neutralization 设计评审，但不允许直接写权重或执行 neutralization，不跑 search/training/64，不进入 Phase 4。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-23 Phase 3.8 状态更新：已完成 offline neutralization design review。本轮只做文档设计，`weights_written=false`、`neutralization_executed=false`，未写 `.bin`，未创建 `phase3_8` neutralized 权重目录，未运行 `bitboard-search` / training / selection，未修改 production code / benchmark scripts / selection gate。Phase 3.7 的 C1/C2 success 满足进入设计评审条件，但下一步若执行也只能是 Phase 3.9 offline diagnostics，不得作为 production candidate，不得替代 selection gate。推荐 variants 为 conservative core `5,10,32,41`、C1-only K10 `4,5,6,10,11,27,32,36,38,41`、C2-focused stable K10 `5,10,32,41,43`，optional union K10 `4,5,6,10,11,27,32,36,38,41,43` 必须标记为 riskier。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-23 Phase 3.9 状态更新：已完成 offline neutralization diagnostics execution。新增非生产脚本 `tools\offline-neutralization-diagnostics.ps1`，输出目录 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization`；临时 `.bin` 24 个，diagnostics JSON 30 个，summary 24 行，root diagnostics 24 行，weight deltas 348 行。cp2 阶段四个 variant 均在 3/3 seed 把 best move 从 `(5, 1) -> (4, 2) (B)` 拉回 baseline `(0, 1) -> (1, 2) (B)`，baseline root order `2->1`，nodes/qnodes 下降，支持 cp2 first drift/root order inflation 的 report-only explanation。final 阶段只支持 partial root-cost explanation：`20260425` 成本下降，但 `20260423` 被推到新 best move `(4, 1) -> (3, 2) (B)` 且 qnodes 上升，属于 over-neutralization/side-effect。Phase 3.9 不替代 selection gate，不是 production candidate，不解锁 Phase 4；Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-23 Phase 3.10 状态更新：已完成 report-only root-order/root-cost explanation summary，只更新文档，不训练、不跑 selection、不跑 `GamesPerSeed=64`、不新增 neutralized `.bin`、不修改 production code 或 benchmark scripts。证据链明确：Phase 3.1 gate 仍 `regress`；Phase 3.2-3.3 定位 opening 是唯一硬 blocker；Phase 3.5 排除 single tuple dominance；Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference；Phase 3.9 强解释 cp2 first drift/root order inflation。final 只能写 partial explanation：`20260425` root cost 可下降，但 `20260423` 有 1/3 root over-neutralization、新 best move `(4, 1) -> (3, 2) (B)` 和 qnodes 上升。conservative_core / c2_focused_stable_k10 风险低于 c1_only_k10 / union_k10_riskier。下一步只建议 Phase 3.11 design-only：opening-safe training objective / root-order regularization / candidate acceptance guard；不直接扩 N-Tuple、不 all_negative、不 rerun 64、不调 gate、不把 neutralized 权重当 candidate。Phase 3 仍未通过，Phase 4 仍 blocked。

> 2026-05-23 Phase 3.11 状态更新：已完成 design-only：opening-safe training objective / root-order regularization / candidate acceptance guard。只更新文档，未修改 production code、`tools\` 或 benchmark scripts，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未修改 gate 阈值、未进入 Phase 4。推荐顺序是先做 design/report-only guard，再考虑最小 opening-safe training objective，最后才考虑生产搜索或特征扩展。candidate acceptance guard 短期只做 report-only；opening-safe objective 和 root-order regularization 未来可能影响 selection gate，但必须先经 Phase 3.12 design-to-diagnostic bridge 验证。明确不推荐直接扩 N-Tuple / threat / mobility / stage、直接 all_negative neutralization、直接重训跑 64、调 gate 阈值、或把 Phase 3.9 neutralized 权重当 production candidate。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-23 Phase 3.12 状态更新：已完成 design-to-diagnostic bridge。Phase 3.12 只把 Phase 3.11 的 design/report-only guard 转成可执行但尚不实现的 opening guard diagnostic spec；只更新文档，未修改 production code、`tools\` 或 benchmark scripts，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未新增诊断脚本、未改 gate 阈值、未进入 Phase 4。spec 定义最小输入 `weights-manifest.json`、Phase 3.3 opening diagnostics、Phase 3.7 ablation summary/sweep、Phase 3.9 neutralization summary/root diagnostics；建议输出 `opening-guard-spec.md` 或文档章节、`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`；字段覆盖 `seed`、`stage`、`artifact_role`、best move、rank、root cost、static support、over-neutralization、`guard_status`、`explanation_scope`。guard 状态只有 `explain` / `watch` / `regress` / `unknown`，且明确不是 selection gate，只能用于 Phase 3 归因和 Phase 3.13 设计/实现选择。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.13-B 状态更新：已实现 report-only opening guard diagnostics。新增非生产脚本 `tools\opening-guard-diagnostics.ps1`，输出目录 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`，生成 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。metrics 共 24 行，覆盖 3 seed × cp2/final × 4 neutralization variant；cp2 guard status 为 `explain=6/watch=6/regress=0/unknown=0`，final 为 `explain=0/watch=8/regress=4/unknown=0`。summary 明确 `cp2_strong_explanation_verified=true`、`final_partial_explanation_verified=true`；四个 variant 聚合 `guard_status` 均为 `regress`，原因是 final `20260423` 存在 non-baseline new best move / root over-neutralization。该 guard 仍是 report-only，不是 selection gate，不改 gate、不训练、不 selection、不跑 64、不生成 `.bin`、不进入 Phase 4。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.14 状态更新：已完成 design-only opening-safe training objective 最小改造方案。只更新文档，未修改 production code、`tools\` 或 benchmark scripts，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate、未进入 Phase 4。推荐顺序为：1) candidate acceptance guard integration，把 Phase 3.13 opening guard 作为 candidate acceptance 的 report-only companion；2) root-cost regularization as diagnostic，把 nodes/qnodes/qnodes_per_node inflation 放进训练后验收诊断，不直接进 loss；3) opening drift penalty，对 baseline rank 降级做轻量惩罚但不硬锁 baseline best move；4) distributed tuple delta clipping / shrinkage，只对 Phase 3.7/3.9 已证明的 distributed drift tuple set 做最小 shrinkage，禁止 all_negative neutralization。Phase 3.15 推荐先做 A：candidate acceptance guard integration 的 report-only hook/spec；暂缓 B：直接改训练 loss。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.15-A 状态更新：已完成 candidate acceptance guard integration 的 report-only hook/spec。只更新文档，未实现 hook，未修改 production code、`tools\` 或 benchmark scripts，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate。hook 设计为 selection 写出 `selection-summary.json/txt` 后的旁路 runner，读取 `RunRoot`、`weights-manifest.json`、opening diagnostics root、`tools\opening-guard-diagnostics.ps1`、selection summary / scorecard artifact，输出 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`，可选 `candidate-acceptance-guard-summary.md`。guard 的 `explain/watch/regress/unknown` 只用于 report-only evidence，不改变 `overall_rating`、`strength_rating`、`perf_rating` 或 scorecard `pass/watch/regress`。Phase 3.16 推荐实现 report-only hook runner/wrapper，仍不改 selection gate；暂缓训练 loss/objective 改造。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.16 状态更新：已实现 report-only candidate acceptance guard runner/wrapper。新增非生产脚本 `tools\candidate-acceptance-guard-runner.ps1`，在 selection 已完成后旁路读取 `selection-summary.json/txt`、既有 Phase 3.13 opening guard 产物和 `tools\opening-guard-diagnostics.ps1`，输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_16_guard_runner`。本次复用既有 `phase3_13_opening_guard`，未覆盖原始 guard 产物；生成 `candidate-acceptance-guard-summary.md`、`candidate-acceptance-guard-summary.json`、`candidate-acceptance-guard-manifest.json`。summary/manifest 明确 `report_only=true`、`selection_gate_eligible=false`、`production_candidate=false`、`selection_result_unchanged=true`、`gate_not_modified=true`、`phase3_status=not_passed`、`phase4_status=blocked`。cp2 为 `explain=6/watch=6/regress=0/unknown=0`，final 为 `explain=0/watch=8/regress=4/unknown=0`，四个 variant 聚合仍均为 `regress`。本轮未修改 `src\` / `include\`，未修改 benchmark scripts，未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.17 状态更新：已完成 report-only evidence package consolidation。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_17_evidence_package`，生成 `phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。证据包汇总 Phase 3.1 selection regress、Phase 3.2-3.3 opening blocker、Phase 3.5 no single tuple dominance、Phase 3.7 distributed tuple static weakening、Phase 3.9 cp2 strong explanation/final partial explanation、Phase 3.13 opening guard counts、Phase 3.16 wrapper `selection_result_unchanged=true` / `gate_not_modified=true`。manifest 记录输入/输出 path 与 SHA256、当前 git commit、`generated_at`，并声明 `report_only=true`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。本轮未新增诊断逻辑，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未进入 Phase 4，未修改 gate 阈值，未修改 `src\` / `include\` / `tools\` / benchmark scripts。下一步建议 Phase 3.18：opening-safe objective pre-implementation plan，仍先做计划，不直接改训练 loss。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.18 状态更新：已完成 opening-safe objective pre-implementation plan。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_18_objective_preimplementation_plan`，生成 `opening-safe-objective-preimplementation-plan.md`、`opening-safe-objective-preimplementation-plan.json`、`opening-safe-objective-preimplementation-manifest.json`。本阶段只制定未来最小实施计划，未实现训练 objective，未修改训练逻辑，未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未进入 Phase 4，未修改 gate 阈值，未修改 `src\` / `include\` / `tools\` / benchmark scripts。推荐实施顺序为：1) candidate acceptance guard companion integration；2) root-cost diagnostic acceptance companion；3) opening drift penalty prototype；4) distributed tuple delta clipping / shrinkage prototype。前两项继续 report-only，后两项暂缓实际代码改造且必须默认关闭、可测试、可回滚。Phase 3.19 推荐优先做 report-only companion integration 的最小 wrapper 接入计划或 dry-run；暂缓 opening drift penalty / tuple clipping。Phase 3 still not passed，Phase 4 still blocked。

> 2026-05-24 Phase 3.19 状态更新：已完成 report-only companion integration dry-run / zero-mutation verification。输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation`，生成 `candidate-acceptance-guard-summary.md/json`、`candidate-acceptance-guard-manifest.json`、`guard-zero-mutation-report.md/json`、`guard-zero-mutation-manifest.json`。本轮运行 Phase 3.16 wrapper，`RunnerOutputDir` 指向 Phase 3.19 目录，`GuardOutputDir` 指向既有 Phase 3.13 guard 目录；复核 selection summary / rating summary、`weights-manifest.json`、`bootstrap.bin`、`candidate.bin`、`checkpoints\checkpoint-*.bin`、Phase 3.13 guard 原始产物和 Phase 3.16 runner 原始产物，结果 `checked_artifact_count=126`、`changed_artifact_count=0`、`unchanged_artifact_count=126`、`changed_artifacts=[]`。`selection_result_unchanged=true`，`gate_not_modified=true`，wrapper 只写 Phase 3.19 输出目录；未训练、未跑 selection、未跑 `GamesPerSeed=64`、未生成 `.bin`、未改 gate，未修改 `src\` / `include\` / `tools\` / benchmark scripts。Phase 3 still not passed，Phase 4 still blocked。Phase 3.20 建议进入 root-cost diagnostic companion dry-run，仍暂缓训练 loss 改造。

## 0. 结论先行

当前项目不是“还没做 AI”，而是已经有一套可用的工程化棋类引擎骨架：

- 已有 6x6 bitboard、预计算 quiet/capture masks、Surakarta 圆弧吃子 path variants。
- 已有 fixed-depth PVS/Alpha-Beta、iterative deepening、aspiration window、TT、qsearch、move ordering、killer/history、LMR、Null Move、Lazy SMP 风格根并行。
- 已有 N-Tuple evaluator、TD/self-play 训练 CLI、trace、eval、multi-seed selection 和 manifest。
- 已有 Dear ImGui GUI，人机对战、D10/T10 Match preset、搜索状态展示、Last Move/PV、Undo Human Turn。
- 已有比较完整的 C++ selftest、CTest、Pester workflow、perf gate、training smoke、selection artifact 链路。

但国赛路线不能装糊涂：

- Phase 3 训练趋势仍是 `implementation complete, trend validation failed`；最新 Phase 3.1 `GamesPerSeed=32` 为 `regress`，不能进入 64 局评审。
- Phase 4 特征扩展继续 blocked。
- P0 capture-rule regression 已修复并加固，但 P0 之前/疑似回归期间的训练产物仍只能当历史诊断，不能当强度证据。
- B1 已将大赛第 6 条“双方都不能再吃子”终局语义落到 traditional、bitboard、search、training/dev-session 相关路径；legacy `max_no_capture_round` / `no_capture_ply` 仍保留为状态/兼容字段，但不再作为 national terminal gate。
- B2.2 已把 post-B1 aggregate baseline 设为正式 perf baseline；这只证明 perf 基线可信，不证明训练趋势通过。
- 当前校赛强度主要来自 `bitboard + fixed-depth PVS + 多线程根搜索`，不是来自已验证稳定的 TD 权重。

一句话：校赛版本可以稳住，国赛版本要按证据链往上堆，不能靠一把梭调参硬冲。硬冲也不是不行，就是容易冲成烟花，挺亮，没棋力。

## 1. 项目当前状态

### 1.1 当前版本定位

当前版本建议定义为：

**Surakarta Traditional 校赛 GUI 对战版本**

它的核心能力是：

- 规则管理：traditional rule manager 与 bitboard rule manager 并存。
- 高性能 movegen：bitboard + 静态 masks + capture variants。
- 搜索 AI：fixed-depth PVS 搜索，支持多线程根搜索。
- GUI 对战：人工输入对手走法，引擎自动给出我方走法。
- 工程验证：CTest / Pester / selftest / GUI smoke / perf baseline / training smoke。

### 1.2 本轮验证结果

| 检查项 | 入口 | 结果 |
|---|---|---|
| bitboard 自测 | `build\bin\surakarta-bitboard-selftest.exe` | `[PASS]` |
| GUI session 自测 | `build\bin\surakarta-dev-session-selftest.exe` | `[PASS]` |
| PowerShell workflow 测试 | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet` | `26 passed / 0 failed` |
| CTest | `Invoke-SurakartaCTest -BuildDir build` | `8/8 passed` |

这些结果能说明当前工程底座健康，但不能说明训练趋势已经通过。测试过和变强了是两码事，这个别混。

### 1.3 已完成能力清单

#### 规则与走法生成

- `include/surakarta_bitboard.h` 定义 6x6 bitboard、Position、Move、MoveList、BitboardTables。
- `src/surakarta_bitboard.cpp` 实现 quiet/capture masks、capture path variant、合法性检查、路径重建、movegen、make/unmake、Zobrist。
- `include/surakarta_masks.h` 保存静态预计算表，避免运行时重复构建。
- `src/surakarta_mask_export.cpp` 可重新生成 masks。
- `surakarta-mask-capacity-failfast` 确保 capture variant 容量不足时 fail fast。

#### 搜索

- `include/surakarta_search.h` 定义 `SearchLimits`、`SearchStats`、`SearchResult`、TT、N-Tuple、training/eval options。
- `src/surakarta_search.cpp` 实现 PVS、qsearch、TT、iterative deepening、aspiration、Null Move、LMR、多线程根搜索、N-Tuple eval、TD 训练。
- 当前搜索是 fixed-depth，不是真正 time-managed search。`SearchLimits` 没有 movetime / soft deadline 字段。

#### 训练与评测

- `bitboard-train` 支持 self-play、checkpoint、alpha/lambda/epsilon、terminal reward、TD error clipping、near-terminal curriculum。
- `bitboard-trace` 支持 TD step 级诊断。
- `bitboard-eval` 支持 candidate vs baseline 的固定局面和 match summary。
- `scripts/benchmark-train.ps1` 是 TD/self-play smoke gate。
- `scripts/benchmark-select.ps1` 是 multi-seed selection gate，输出 pass/watch/regress、strength/perf split、blocker details。

#### GUI 与校赛使用

- `src/surakarta_gui_main.cpp` 实现 Dear ImGui GUI、棋盘渲染、Depth/Threads preset、Last Move/PV/Debug stats。
- `src/surakarta_dev_session.cpp` 管理 GUI session、人类走法、异步引擎搜索、undo。
- Match preset 为 `Depth 10 / Threads 10`，这是 GUI 校赛配置，不是训练配置，也不是 Phase 4 放行信号。

#### 工程化

- `scripts/workflow.ps1` 负责配置、构建、CTest 和进程清理。
- `scripts/perf-runner.ps1` 负责 opening/middlegame/endgame、depth 6/7/8、threads 1/2/4 的 perf baseline gate。
- `scripts/workflow.tests.ps1` 覆盖 workflow、perf、training、selection、manifest、repeatability artifact 的脚本逻辑。
- `README.md` 已把 default regression、manual acceptance、training smoke、Phase 4B selection work 分开。

## 2. 关键问题与当前风险

### 2.1 Phase 3 训练趋势未通过

历史 evidence 显示：

- GamesPerSeed=32 多组参数仍 `regress`。
- GamesPerSeed=64 decision run 仍按现有规则判 `overall_rating=regress`。
- 训练信号从 terminal TD spike 转移到 opening search-tree inflation。
- `strength_rating` 可能出现 watch，但 `perf_rating=regress` 仍然阻塞整体通过。

结论：不能进入 Phase 4。别看有时候候选权重赢几盘就开始飘，固定局面性能和搜索树膨胀还在报警，这就像发动机响声不对还非要上赛道，真不用这么虎。

### 2.2 P0 规则事故已经修，但证据链要重建

P0 capture-rule regression 根因是 capture variant 容量不足导致合法路径被截断。当前已修复为：

- `kMaxCaptureVariants=8`
- 动态 Build 容量不足时 fail fast
- `TestCaptureVariantOverflowGolden`
- `TestRuleCorrectnessGoldenParity`
- `TestCaptureVariantCapacityInvariant`
- CTest 中 `surakarta-mask-capacity-failfast`

但是，P0 前后的训练/selection artifact 不能混用。国赛路线必须在 P0 修复后重新建立 baseline 和 selection evidence。

### 2.3 终局语义状态

2026-05-22 阶段 B1 已完成 national terminal semantics 修复：不再用 legacy no-capture counter 代替“双方都不能再吃子”，并补齐 traditional / bitboard / search / dev-session golden 与 parity 证据。

后续风险已经转移：不能再把 pre-B1 / suspect / provisional training artifacts 当成强度证据；必须用 post-B1 fresh selection 重新验证训练趋势。

### 2.4 缺少真正 time management

当前实战依赖 fixed-depth preset。优点是可复现，缺点是极端局面耗时不可严格封顶。

国赛阶段需要新增：

- `movetime`
- soft deadline
- hard deadline
- aspiration retry 的时间保护
- root move 完成度保护
- GUI 显示剩余/耗时信息

这应放在稳定评估函数之后做，否则时间管理会掩盖评估和搜索树问题。

## 3. 国赛水平的定义

“国赛水平”不能写成玄学口号，要定义成可验收目标：

### 3.1 最低工程标准

- 规则 golden cases 全通过。
- traditional / bitboard / search terminal semantics 一致。
- CTest、Pester、GUI smoke、manual acceptance 全通过。
- perf gate 不 regress。
- 所有 release artifact 可追溯到 git commit、seed、参数、hash。

### 3.2 最低棋力标准

- 新候选版本在 post-fix baseline 上通过 GamesPerSeed=32 至少 `watch`。
- GamesPerSeed=64 不 `regress`。
- 固定局面无系统性 best move drift / score regression。
- opening、capture-heavy、threat-heavy、endgame 都没有稳定 search-tree inflation。
- candidate 在 engine-vs-engine ladder 中对校赛 baseline 有统计显著优势。

### 3.3 理想国赛标准

- 有稳定训练权重，不依赖 bootstrap 手工评估。
- 有阶段感知、威胁感知、机动性、圆弧路线相关特征。
- 搜索有稳定 time management。
- 有长期自对弈 ladder、SPRT/Elo、置信区间。
- 有可复现实验报告，能说明每次增强带来的收益和代价。

## 4. 分阶段技术路线

### 阶段 A：校赛冻结与规则底座再验收

**目标：** 保住校赛可用版本，避免赛前引入大改风险。

**主要任务：**

- 冻结当前 GUI Match preset：`Depth 10 / Threads 10`。
- 赛前只允许修复 P0/P1 级规则、崩溃、GUI 操作问题。
- 跑 `workflow.ps1`、CTest、Pester、GUI smoke。
- 执行 manual acceptance：普通移动、合法吃子、非法吃子、P0 golden、加载局面、悔棋、人类执黑/执白。
- 记录比赛机器硬件、耗时、温度/风扇状态、D10/T10 实战响应。

**验收门槛：**

- CTest 全过。
- Pester 全过。
- GUI smoke 全过。
- manual acceptance 有归档。
- P0 golden 局面明确通过。

**禁止事项：**

- 不扩训练特征。
- 不调 LMR/Null Move/qsearch。
- 不换默认权重。
- 不把实验性 TD 权重用于校赛默认。

### 阶段 B：大赛终局语义与规则完整性

**目标：** 严格对齐比赛规则，尤其是“双方都不能再吃子”的终局语义。

**主要任务：**

- 明确比赛规则文本对应的内部判定函数。
- 增加 `HasAnyCaptureMove(Color)` 或等价接口，避免用 no-capture counter 代替。
- 为双方不能吃子、单方不能吃子、仍可吃子、棋子被吃光等场景写 golden cases。
- traditional rule manager 与 bitboard rule manager 同步。
- search terminal evaluation 同步。
- training terminal reason 同步。
- GUI status 同步。

**验收门槛：**

- traditional/bitboard parity 全过。
- search depth 1/2 对终局局面返回一致。
- training summary terminal reason 正确。
- GUI 显示终局状态正确。

**风险：**

- 终局规则会影响训练数据分布，改完后必须重跑 baseline，不能沿用旧训练结论。

### 阶段 C：post-fix 可信基线重建

**目标：** 把 P0 之后的世界重新标定一遍。

**主要任务：**

- 重新生成 rule-correct baseline。
- 重新跑 perf baseline：opening/middlegame/endgame，depth 6/7/8，threads 1/2/4。
- 重新校准 GUI preset：D8/T8、D10/T10、必要时 D11/T10。
- 重新跑 training smoke，确认 reproducibility。
- 建立 fixed position suite v2：
  - opening
  - capture-heavy
  - threat-heavy
  - loop-path
  - qsearch-sensitive
  - endgame
  - terminal-semantics

**验收门槛：**

- baseline artifact 全部带 manifest。
- perf gate 全过。
- training smoke 全过。
- fixed position suite v2 写入 selection。

### 阶段 D：训练稳定性恢复

**目标：** 让 TD/self-play 从“有工具链”变成“能稳定提升棋力”。

**2026-05-22 准备状态：**

- B2.2 official perf baseline 已确认可作为 fresh trend validation 基线。
- 正式 build 使用 `build-vs-train-release`。
- 第一关 `GamesPerSeed=32` fresh post-B1 selection 已执行：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- 32 局结果为 `overall_rating=regress`、`strength_rating=regress`、`perf_rating=regress`，scorecard `pass/watch/regress=0/0/3`，C/B/D=`12/16/2`，`score_regressions=1`，`best_move_changes=3`，平均/最差性能比 `1.622708/1.864728`。
- blocker 集中在 `opening`：三个 seed 均 best move drift，node ratio `1.343750/1.659647/1.694973`，`20260425` search ratio `1.864728`；`capture-heavy`、`threat-heavy`、`endgame`、`no-capture-critical` 无关键 drift。
- 按规则已经停止，不跑 `GamesPerSeed=64`。
- Phase 3.2 归因已完成：checkpoint-2 起三个 seed 均出现 opening root move drift 和 search timing 超阈；node inflation 在 `20260423` cp6、`20260424/20260425` cp4 出现；最终 move `(3, 1) -> (2, 2) (B)` 分别在 `20260425` cp14、`20260424` cp22、`20260423` cp32 首次出现。
- Phase 3.3 root diagnostics 已完成：cp2 三 seed 首次 drift 都是 `(5, 1) -> (4, 2) (B)`，depth-4 root order rank `1`，baseline `(0, 1) -> (1, 2) (B)` order rank `2`，observed score 为 rank-1/bound-tie；cp2 `aspiration_retries=0`，说明首次 drift / qnode inflation 不是 retry 造成。final candidate 中 `(3, 1) -> (2, 2) (B)` 均为 order rank `1`，baseline 仍是 bound-tie 但成为低 qnodes non-PV 子树。
- Phase 3.4 attribution/ablation design 已完成：现有 CLI 无 active tuple attribution，但可用离线小脚本在不改生产代码的情况下解析 `.bin` 权重并输出 child-position static attribution；report-only CLI 扩展只是下一阶段备选。最小数据集为 3 seed × `bootstrap/cp2/final` × baseline/cp2 drift/final drift child positions。
- Phase 3.5 attribution-only execution 已完成：`tools\opening-attribution.ps1` 已生成 `opening-child-attribution-summary.csv`、`opening-child-attribution-tuples.csv`、`opening-child-attribution-summary.json`、`opening-child-attribution-top-deltas.csv`、`attribution-manifest.json`；summary `27` 行，tuple detail `1215` 行，artifact hash/header pass。cp2 阶段 cp2 drift child 相对 baseline child static delta 为 `-13`；final 阶段 final drift child 相对 baseline child static delta 为 `-105/-138/-150`。未发现少数 tuple 主导，36 个 comparison 的 dominance 均为 `no`。
- Phase 3.6 child-position static ablation design 已完成：主 comparison 为 cp2 阶段 `baseline_child -> cp2_drift_child` 和 final 阶段 `baseline_child -> final_drift_child`；辅 comparison 为 final 阶段 `baseline_child -> cp2_drift_child`、`cp2_drift_child -> final_drift_child`；control 为 bootstrap sanity 和 cp2 final negative control。若后续执行，只能在表格/内存层 neutralize stage-vs-bootstrap 的 drift-favoring `induced_pair_delta < 0`，不得写权重。
- Phase 3.6 TopK 策略：每个 seed/comparison 独立按 `abs(induced_pair_delta)` 排序，输出 `K=1/3/5/10/20/all_negative` 曲线，并报告 `2/3`、`3/3` seed-stable tuple 集合。dominance=no 仍可做 distributed static ablation，但不能直接升级 offline neutralization。
- Phase 3.7 static ablation execution 已完成：`tools\child-position-static-ablation.ps1` 已生成 `opening-child-static-ablation-summary.csv`、`opening-child-static-ablation-tuples.csv`、`opening-child-static-ablation-sweep.csv`、`opening-child-static-ablation-summary.json`、`opening-child-static-ablation-manifest.json`；输出目录为 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`。manifest 记录 attribution manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`、tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`、tuple set hash `0x1D4A6EBF2BAF6484`、weight count `16605`、git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- Phase 3.7 TopK 结果：`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`。两者 K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring；`C1` K10 tuple 三 seed 完全一致 `4/5/6/10/11/27/32/36/38/41`，`C2` K10 的 `3/3` tuple 为 `5/10/32/41/43`，`2/3` tuple 为 `0/5/6/10/11/32/33/37/41/43/44`。
- Phase 3.7 判定：dominance=no 下 ablation 仍有效，因为曲线显示分布式 tuple delta 累积效应；但它只支持进入 offline neutralization 设计评审，不支持直接执行 neutralization，不证明 PVS/root search 已恢复，不解除 Phase 3/4 阻塞。
- Phase 3.8 offline neutralization design review 已完成：满足进入设计评审条件，但本轮 `weights_written=false`、`neutralization_executed=false`、`offline_neutralization_execution_allowed=false`。当前仍不执行 neutralization，不生成任何 `.bin`，不创建 `phase3_8` neutralized 权重目录。
- Phase 3.8 推荐 candidate tuple sets：`C1 K10 = 4/5/6/10/11/27/32/36/38/41`；`C2 stable K10 core = 5/10/32/41/43`；`C2 2/3 support set = 0/5/6/10/11/32/33/37/41/43/44`。C1 和 C2 不强行共用同一集合，应按 stage / comparison 分 variant；共享 conservative core `5/10/32/41` 用于 cross-stage 解释力测试。
- Phase 3.8 推荐 variants：conservative core `5/10/32/41` 预期同时解释 cp2 onset 与 final drift 的共享部分；C1-only K10 预期解释 cp2 checkpoint-2 first drift；C2-focused stable K10 预期解释 final gate blocker；optional union K10 `4/5/6/10/11/27/32/36/38/41/43` 只做 riskier 压力对照，不能当默认生产候选。
- Phase 3.8 K policy：推荐围绕 K10 variant 比较；不选 K1/K3，因为 C1/C2 在 K1/K3 均 `0/3` seed success；不直接 all_negative，因为 C1 K10 已等价 all_negative 且出现 over-neutralization，C2 all_negative 会写 `15-17` 个 tuple 并把 ablated delta 推过头，解释力会被“砍太多当然会变”污染。
- Phase 3.8 临时权重设计：如果 Phase 3.9 执行，只能复制权重到 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization\weights-temp\<variant>\seed-<seed>\<stage>\`，文件名必须包含 `diagnostic-neutralized`，不得命名为 `candidate.bin` / `checkpoint-2.bin` / `bootstrap.bin`，不得写入 `selection\seed-*` 下任何 bootstrap/checkpoint/candidate。
- Phase 3.8 manifest 设计：必须记录 source/output path 与 SHA256、header fields、tuple_set_hash、weight_count、variant、comparison、stage、seed、K policy、tuple ids、absolute indices、original values、neutralized values、removed deltas、expected explanation target，以及 `production_candidate=false`、`selection_gate_modified=false`、`training_executed=false`、`selection_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。
- Phase 3.8 后续最小 diagnostics：只允许 offline manifest/header/hash 校验、static attribution 复算、opening fixed-case depth-4 root/search diagnostics，对比 best move、root order、observed score、nodes/qnodes、aspiration retries。不得训练、selection、64 局、production candidate gate 或 Phase 4。
- Phase 3.8 解释力口径：conservative core 若同时削弱 C1/C2，说明共享 tuple 机制可信；C1-only K10 只负责 cp2 onset；C2-focused stable K10 只负责 final gate blocker。若 static 表格翻转但 PVS/root search 不恢复，只能写 static component explains part of drift，不能写 Phase 3 fixed。
- Phase 3.8 停止条件：需要 all_negative 或超过 K10 才有效、controls 失效、引入新 root drift、manifest/hash/header 不一致、临时权重污染 selection 目录、或有人试图用 neutralization 替代 selection gate / 放宽阈值 / 调参 / 跑 64 / 进 Phase 4，必须停止。这个边界不能糊，糊了就是拿诊断结果当免死金牌。
- Phase 3.9 offline diagnostics execution 已完成：`tools\offline-neutralization-diagnostics.ps1` 从 `weights-manifest.json` 解析原始 cp2/final artifact，并以 `selection-summary.json` 为 Phase 3.1 参数事实源做硬校验；临时 neutralized 权重全部落在 `opening-diagnostics\phase3_9_offline_neutralization\weights-temp`，均为 `production_candidate=false`、`selection_gate_eligible=false`。
- Phase 3.9 cp2 结论：conservative core、C1-only K10、C2-focused stable K10、union K10 riskier 均在 3/3 seed 把 cp2 best move 拉回 baseline，baseline root order `2->1`，nodes/qnodes 下降。conservative core 与 C2-focused stable K10 的 cp2 nodes/qnodes 为 `1797/1277 -> 1569/1063`，无 root over-neutralization；C1-only/union 为 `1797/1277 -> 1722/1219`，但 static over-neutralization 为 3/3。
- Phase 3.9 final 结论：final blocker 只得到 partial explanation。`20260425` 多个 variant 显著降低 nodes/qnodes，`20260424` 基本不变；`20260423` 全部 variant 都引入新 best move `(4, 1) -> (3, 2) (B)` 并增加 qnodes，说明 offline neutralization 有副作用，不能作为 production candidate。
- Phase 3.9 后续边界：可以进入 report-only root-order/root-cost explanation 总结；不允许写 Phase 3 pass，不允许跑 `GamesPerSeed=64`，不允许把 neutralized 权重送 selection gate，不允许进入 Phase 4。说白了，它解释了病灶一部分，不是治好了，别把化验单当药吃。
- Phase 3.10 report-only explanation summary 已完成：opening first drift/root order inflation 的来源被串成证据链，核心机制是 post-training distributed tuple deltas 改变 opening root child ordering，并把 qnodes/root cost 推向新的 PV root move。cp2 阶段解释力强：Phase 3.9 四个 variant 均在 `3/3` seed 把 `(5, 1) -> (4, 2) (B)` 拉回 baseline `(0, 1) -> (1, 2) (B)`，baseline root order `2->1`，nodes/qnodes 下降。
- Phase 3.10 final 边界：final candidate 只能 partial explanation，不能写修复完成。`20260425` 在 conservative core / C2-focused stable K10 下成本下降，但 `20260423` 出现 1/3 root over-neutralization，被推到新 best move `(4, 1) -> (3, 2) (B)`，qnodes 上升；`20260424` 基本不变。这个边界要写清楚，不然报告就会变成“看起来都解释了”，那就太能忽悠自己了。
- Phase 3.10 variant 风险排序：conservative_core `5/10/32/41` 与 c2_focused_stable_k10 `5/10/32/41/43` 更低风险，因为 tuple 集合小、共享核心明确、cp2 无 root/static over-neutralization；c1_only_k10 与 union_k10_riskier 覆盖更多 tuple，cp2 static ablated delta 推到 `+1`，只能作为 riskier 压力对照。
- Phase 3.10 下一步：推荐 Phase 3.11 design-only，设计 opening-safe training objective、root-order regularization、candidate acceptance guard。仍不推荐直接扩 N-Tuple、直接 all_negative neutralization、直接 rerun 64 games、调 gate 阈值或把 neutralized 权重当 candidate。
- Phase 3.11 design-only 已完成：三个候选方向分别是 candidate acceptance guard、opening-safe training objective、root-order regularization。它们共同目标是防止 opening root drift / root-order inflation 再次把 selection gate 打成 regress，但本阶段只给方案，不实现。
- Phase 3.11 candidate acceptance guard：目标是在候选权重进入正式 selection 前 report-only 暴露 cp2 first drift、baseline root order 降级、root cost inflation、final new best move side effect；作用阶段是 candidate acceptance / selection preflight；依赖 Phase 3.3 root order 证据、Phase 3.9 cp2 neutralization 强解释和 final `20260423` 副作用。它能解决可见性和候选接受边界，不能修训练；风险是过宽后变成偷改 gate；最小验证是 Phase 3.12 先设计 JSON/TXT 字段，不改 `overall_rating`；短期无需生产代码变更；未来若纳入 gate 会影响 selection，但必须单独评审。
- Phase 3.11 opening-safe training objective：目标是在 TD/self-play objective 中限制 opening baseline child 相对 drift child 的不安全偏移；作用阶段是 training objective；依赖 Phase 3.5 static delta 和 Phase 3.7 K5/K10 distributed tuple evidence。它预期减少 cp2 first drift/root order inflation 复发，不能保证 final 全修复或 match strength 变强；风险是过拟合 opening fixed case、压制真实学习信号、把 final partial explanation 误判成 complete fix；最小验证是 Phase 3.12 先离线模拟 objective 指标；未来需要生产训练代码变更，且会影响 selection gate，必须重走 32/64。
- Phase 3.11 root-order regularization：目标是把 root child ordering 稳定性作为训练或候选验收约束；作用阶段可先在 report-only guard，未来才考虑 training checkpoint regularization；依赖 Phase 3.3 cp2 无 retry 与 Phase 3.9 rank `2->1` / nodes/qnodes 下降。它预期解决 root order inflation 和高成本 PV 子树迁移，不能证明新 best move 一定错，也不能解决 final `20260423` 副作用；风险是把 baseline move 固化成教条；最小验证是 Phase 3.12 定义 root-order/rank/cost delta 与 safe/tie/new-best-move 分类；report-only 不需生产代码，未来若进 acceptance 会影响 candidate 晋级。
- Phase 3.11 推荐顺序：优先 design/report-only guard；再考虑最小 training objective 约束；最后才考虑生产搜索或特征扩展。明确不推荐直接扩 N-Tuple / threat / mobility / stage、直接 all_negative neutralization、直接重训并跑 64、调 selection gate 阈值、把 neutralized 权重当 production candidate。
- Phase 3.12 design-to-diagnostic bridge 已完成：opening guard 被定义为 report-only diagnostics，不是 selection gate；只提供 Phase 3 归因和下一步设计证据，不得替代 pass/watch/regress gate。
- Phase 3.12 最小输入 artifact：`weights-manifest.json`、Phase 3.3 `opening-diagnostics-summary.md` / opening diagnostics CSV、Phase 3.7 `opening-child-static-ablation-summary.csv` / `opening-child-static-ablation-sweep.csv`、Phase 3.9 `neutralization-manifest.json` / `neutralization-summary.csv` / `neutralization-root-diagnostics.csv`。
- Phase 3.12 建议输出 artifact：`opening-guard-spec.md` 或本路线图章节、`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`；manifest 必须写 `report_only=true`、`selection_gate_modified=false`、`training_executed=false`、`selection_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。
- Phase 3.12 字段草案：`seed`、`stage`、`artifact_role`、`baseline_best_move`、`candidate_best_move`、`best_move_status`、`baseline_rank`、`candidate_rank`、`root_order_status`、`nodes_ratio`、`qnodes_ratio`、`aspiration_retry_delta`、`static_support_status`、`over_neutralization_status`、`guard_status`、`explanation_scope`。
- Phase 3.12 guard 指标：opening best-move guard、root-order guard、root-cost guard、static-drift guard、final-candidate side-effect guard。状态语义为 `explain` / `watch` / `regress` / `unknown`；这些状态只表达诊断解释强弱和风险，不改变 `overall_rating` / `strength_rating` / `perf_rating`。
- Phase 3.12 watch/regress 口径：root-cost watch 可由 cost ratio `> 1.10` 或 qnodes/nodes 上升触发；root-cost regress 可由 nodes/qnodes 任一 `> 1.25` 且伴随 drift rank 1 或不可由 retry 解释触发。这个阈值只属于 report-only guard，不能偷塞进 selection policy。
- Phase 3.12 static-drift 口径：C1/C2 在 K5/K10 跨 seed 削弱且 controls pass 可写 `explain`；只靠 K1/K3 不足，最多 `watch`；需要 all_negative 或出现 over-neutralization signature 应写 `regress`。
- Phase 3.12 final side-effect 口径：`20260425` root cost 下降只能支持 partial explanation；`20260423` 被推到 non-baseline new best move `(4, 1) -> (3, 2) (B)` 且 qnodes 上升，必须作为 final side-effect `regress` 记录，不能写成修复。
- Phase 3.13 建议：可选 A 继续 design-only，设计 opening-safe training objective 的最小改造点；可选 B 实现 report-only opening guard diagnostics 脚本。推荐优先 B，因为 schema 已清楚，先生成可复跑 guard report 更能暴露真实字段缺口；但仍不进入 Phase 4，不训练，不跑 selection，不生成 `.bin`，不改 gate。
- Phase 3.13-B report-only opening guard diagnostics 已完成：`tools\opening-guard-diagnostics.ps1` 只读取 Phase 3.3 / 3.7 / 3.9 既有产物，输出 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`，未训练、未跑 selection、未生成 `.bin`、未改 gate。
- Phase 3.13-B metrics 结果：cp2 为 `explain=6/watch=6/regress=0/unknown=0`；final 为 `explain=0/watch=8/regress=4/unknown=0`。conservative_core 与 c2_focused_stable_k10 提供 cp2 强解释；c1_only_k10 与 union_k10_riskier 因 static over-neutralization 在 cp2 标 watch。
- Phase 3.13-B final 边界：final partial explanation 成立，但 `20260423` 四个 variant 都触发 root over-neutralization / non-baseline new best move，因此四个 variant 的聚合 `guard_status` 都是 `regress`。这是 report-only 风险摘要，不替代 selection gate，也不表示 Phase 3 通过或失败状态发生变化。
- Phase 3.14 design-only 已完成：基于 Phase 3.13-B guard 产物，opening-safe training objective 的推荐顺序是 `candidate acceptance guard integration`、`root-cost regularization as diagnostic`、`opening drift penalty`、`distributed tuple delta clipping / shrinkage`。
- Phase 3.14 设计边界：candidate acceptance guard integration 与 root-cost regularization 短期应停在 report-only / diagnostic hook，不需要生产代码变更；opening drift penalty 与 distributed tuple clipping 若未来真正进入 objective 或权重更新，需要生产训练代码变更。
- Phase 3.14 不推荐路线：不直接 all_negative neutralization，不直接扩 N-Tuple / threat / mobility / stage，不直接调 gate 阈值，不直接跑 `GamesPerSeed=64`，不直接把 Phase 3.9 neutralized 权重当 candidate。
- Phase 3.15 建议：推荐 A，先实现 candidate acceptance guard integration 的 report-only hook/spec；暂缓 B 直接改训练 loss。guard 仍不是 selection gate，只用于 Phase 3 归因和候选审计。
- Phase 3.15-A report-only hook/spec 已完成：hook 位置是 candidate 生成与 selection gate summary 写出之后，或最终 summary 汇总阶段的旁路 runner；不得插入 selection rating/scorecard 计算路径。
- Phase 3.15-A 输入 contract：`RunRoot`、`weights-manifest.json`、opening diagnostics root、`tools\opening-guard-diagnostics.ps1`、selection summary / scorecard artifact。最小命令示例：`pwsh -NoProfile -ExecutionPolicy Bypass -File .\tools\opening-guard-diagnostics.ps1 -RunRoot <run-root> -OutputDir <guard-output-dir>`。
- Phase 3.15-A 输出 contract：`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`，可选 `candidate-acceptance-guard-summary.md`。manifest 必须记录 commit、input/output hash、script path、command line、`generated_at`，并保留 `production_candidate=false`、`selection_gate_eligible=false`、`report_only=true`。
- Phase 3.15-A failure behavior：guard artifact 缺失时 selection result 不被覆盖且 guard 为 `unknown`；guard 脚本失败不得 retroactively 改写主 selection；manifest 不合规时 guard report invalid；final `regress` 只能写 risk note，不能自动 reject candidate。
- Phase 3.16 建议：实现 report-only hook runner/wrapper，只在 selection 后生成附加报告；仍不改 selection gate，暂缓训练 loss/objective 改造。
- `20260423` 的 final score regression 是晚期事件，cp30 首次负值、cp32 持续；`20260425` 早期负分在 cp14 后恢复；`20260424` 没有负向 score_delta。
- 非 opening checkpoint 全部无 best move drift、无 score regression，node ratio 基本稳定；当前 blocker 不在 capture-heavy / threat-heavy / endgame / no-capture-critical。
- strength regress 与 perf regress 有共同 opening 扰动背景，但 gate 证据不可互相替代；正向 fixed-position score 不能覆盖 C/B/D 落后或 opening performance 超阈。
- 当前没有证据支持直接调 `alpha/lambda/epsilon` 再跑；Phase 3.7 后下一步最多只能做 offline neutralization 设计评审，不能直接复制/改写 candidate weights 执行 neutralization。
- 64 局不得 `regress`，才允许讨论 Phase 3 是否从 failed 升级为 watch/pass；这仍不等于“国赛级”，更不自动解锁 Phase 4。

**主要任务：**

- 只使用 post-fix fresh artifacts。
- 重跑 GamesPerSeed=32，先不扩大参数矩阵。
- 若仍 regress，继续定位：
  - TD target perspective
  - terminal reward scale
  - TD error clipping
  - eligibility trace
  - exploration schedule
  - opening attribution
  - qsearch source inflation
- 将 selection 分为 strength、perf、stability 三条诊断线，但 overall gate 继续保守。
- Phase 3.12 前只允许把 Phase 3.11 guard/objective/regularization 设计转成 report-only diagnostics，不直接改训练目标。
- 每次训练输出：
  - TD error distribution
  - weight delta distribution
  - terminal reason distribution
  - checkpoint comparison
  - fixed position score/best_move/nodes/qnodes/time

**验收门槛：**

- GamesPerSeed=32 至少 `watch`。
- GamesPerSeed=64 不 `regress`。
- opening max node ratio 不稳定超阈。
- fixed score regression 为 0 或有明确可接受解释。

**禁止事项：**

- 不因 `strength_rating=watch` 覆盖 `perf_rating=regress`。
- 不新增 `perf_watch` 偷放行。
- 不让 repeatability status 参与 gate，除非重新设计并评审 selection policy。
- 不把 Phase 3.9 neutralized 权重当 production candidate。
- 不直接 all_negative neutralization。
- 不在 Phase 3 still not passed 时进入 Phase 4 特征扩展。

### 阶段 E：评估函数扩展

**目标：** 在训练稳定后扩展表达能力，提高局面判断质量。

**推荐顺序：**

1. **loop/capture N-Tuple**
   - 捕捉圆弧吃子路线、入口点、可吃目标、阻挡结构。
   - 优先服务 Surakarta 特有战术，不做泛泛棋盘统计。

2. **threat-aware features**
   - 我方可吃、对方可吃、下一手威胁、双重威胁。
   - 必须区分真实 capture path 和普通直线相邻。

3. **mobility features**
   - quiet mobility
   - capture mobility
   - safe mobility
   - opponent reply mobility

4. **stage-aware features**
   - 开局：结构和安全。
   - 中局：威胁和机动。
   - 残局：材料、终局、可吃性。

**每类特征的固定流程：**

- 先写 feature extraction tests。
- 再写 weight compatibility / manifest version。
- 再跑 bootstrap parity。
- 再跑 small training smoke。
- 再跑 GamesPerSeed=32 selection。
- 最后做 ablation：只加这一类特征 vs 不加。

**禁止事项：**

- 不一次塞四类特征。
- 不在没有 ablation 的情况下宣布提升。
- 不让特征规模拖垮收敛速度。

### 阶段 F：搜索增强与时间管理

**目标：** 在稳定评估函数上进一步榨搜索强度。

**主要任务：**

- 调整 move ordering：
  - capture ordering
  - TT move priority
  - killer/history aging
  - PV move reuse

- 调整 qsearch：
  - delta pruning 可行性
  - capture path ordering
  - qnode inflation guard

- 调整 LMR：
  - late move threshold
  - reduction depth
  - re-search 条件

- 调整 Null Move：
  - 棋子数门槛
  - depth 门槛
  - zugzwang-like 风险局面保护

- 增加 time management：
  - `SearchLimits.movetime_ms`
  - soft deadline
  - hard deadline
  - iteration boundary stop
  - aspiration retry stop
  - GUI preset 支持 time mode

**验收门槛：**

- fixed-depth strength 不退。
- movetime 下不会超硬 deadline。
- perf gate 不退。
- ladder 有稳定收益。

### 阶段 G：国赛级评测体系

**目标：** 不靠感觉判断棋力，用长期、可复现、可比较的数据判断。

**主要任务：**

- 建立 engine-vs-engine ladder：
  - bootstrap
  - 校赛 baseline
  - 每个候选权重
  - 每个搜索参数候选

- 引入统计指标：
  - Elo estimate
  - SPRT
  - win/draw/loss
  - confidence interval
  - per-color split
  - per-opening split

- 增加局面池：
  - 随机 self-play opening
  - 人工战术题
  - 历史失败局面
  - 终局语义局面
  - P0/P1 regression 局面

- 形成 release report：
  - 版本 commit
  - 参数
  - 权重 hash
  - 测试结果
  - perf 结果
  - ladder 结果
  - 已知风险

**验收门槛：**

- 新版本对校赛 baseline 有统计显著优势。
- 无 P0/P1 回归。
- 所有结果可复现。

## 5. 推荐近期任务顺序

### 校赛前

1. 锁定当前校赛版本，不做大改。
2. 跑完整 workflow、CTest、Pester、GUI smoke。
3. 做 manual acceptance 归档。
4. 用比赛机器验证 D10/T10 响应时间。
5. 准备答辩材料，只讲真实已完成内容。

### 校赛后第一轮

1. 补大赛终局语义 spec。
2. 写终局 golden cases。
3. 修改 traditional/bitboard/search/training/GUI terminal semantics。
4. 重建 post-fix baseline。
5. 重跑 GamesPerSeed=32/64 selection。

### 国赛冲刺主线

1. 训练趋势先过 gate。
2. 再做 loop/capture 特征。
3. 再做 threat/mobility/stage 特征。
4. 再调搜索参数。
5. 最后做 time management 和 release ladder。

## 6. 当前文件责任地图

| 文件/目录 | 责任 |
|---|---|
| `include/surakarta_bitboard.h` | bitboard 数据结构、Position、Move、mask 表结构 |
| `src/surakarta_bitboard.cpp` | movegen、capture legality、path reconstruction、make/unmake |
| `include/surakarta_masks.h` | 静态预计算 mask 表 |
| `src/surakarta_search.cpp` | PVS/qsearch/TT/LMR/Null Move/训练/eval |
| `include/surakarta_search.h` | 搜索、评估、训练、eval API |
| `src/main.cpp` | benchmark/search/train/eval/trace CLI |
| `src/surakarta_gui_main.cpp` | GUI 展示、preset、交互 |
| `src/surakarta_dev_session.cpp` | GUI session 状态机、人类走法、引擎搜索、undo |
| `scripts/workflow.ps1` | 默认构建与 CTest |
| `scripts/perf-runner.ps1` | perf baseline gate |
| `scripts/benchmark-train.ps1` | TD smoke |
| `scripts/benchmark-select.ps1` | multi-seed selection |
| `scripts/workflow.tests.ps1` | PowerShell workflow 单元测试 |
| `test/test_data/perf-baseline.json` | 性能基线 |
| `docs/school_competition_defense.md` | 校赛答辩材料 |
| `task_plan.md` | 当前阶段计划和 gate 状态 |
| `findings.md` | 研究发现与决策 |
| `progress.md` | 会话日志和验证结果 |

## 7. 绝对不要干的事

- 不要把 Phase 3 写成通过。
- 不要把 Phase 4 写成已开始。
- 不要说 TD 已训练出稳定最强权重。
- 不要拿 pre-fix / suspect artifacts 当强度证据。
- 不要把 `no_capture_ply` 当成大赛终局规则第 6 条。
- 不要在训练趋势没过前扩一堆特征。
- 不要为了让报告好看放宽 selection。
- 不要默认 Threads 越多越强，当前根并行不是这么玩的。
- 不要赛前临时大改搜索参数，这叫给自己上强度，不叫提高棋力。

## 8. 最终路线一句话

校赛：锁版本、保规则、保 GUI、保可演示。
国赛：先补规则语义和可信基线，再恢复训练稳定性，再扩评估特征，再调搜索和时间管理，最后用长期 ladder 证明强度。

这条路慢一点，但它是能走到国赛水平的路。上来就“多加几个特征、线程拉满、参数玄学调一调”，那不叫技术路线，叫东北炖大锅，啥都放了，最后谁也尝不出味儿。

## 9. 2026-05-25 Phase 3.36 诊断结论补记

- Phase 3.36 已完成 diagnostic dry-run / report-only 证据评审。
- 结果仍是 `phase3_status=not_passed`、`phase4_status=blocked`，但允许进入 Phase 3.37 guarded limited training plan review。
- 这一步只管计划审查，不管直接训练；`training_executed=false`、`selection_executed=false`、`bin_generated=false` 仍是硬边界。

## 10. 2026-05-26 Phase 3.41 接口骨架补记

- Phase 3.41 已完成 guarded active interface skeleton。
- 入口保持 `surakarta-benchmark.exe bitboard-train`，只新增单一受审 `--config <path>` 文件入口；不新增散乱 objective toggle。
- config schema 约束为 `schema_version=1`、`enabled`、`mode=active_scoped`、`scope=opening_root_children_only`、`report_only=true`、`selection_gate_eligible=false`、bounded `max_games/max_depth` 和 seed allowlist。
- default-off 是硬语义：缺省 config 不启用 skeleton，无效 config 拒绝，valid scoped config 只进入 skeleton state，不触发 active probe。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步只允许 Phase 3.42 guarded active interface verification package。

## 11. 2026-05-26 Phase 3.42 验证包补记

- Phase 3.42 已完成 guarded active interface verification package。
- 本轮只做 verification-only；不新增接口逻辑、不改 schema、不改 objective、不改训练逻辑。
- 构建与 selftest 已复核；输入证据链与 Phase 3.41 状态位已复核。
- 静态边界确认：`--config` 单一入口存在，无散乱 objective toggle，`ActiveObjectiveConfigSkeleton` 与自测 markers 存在。
- 输出目录未生成 `.bin`，protected scripts/tools 无 diff。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步只允许 Phase 3.43 active scoped probe dry-run plan review。

## 12. 2026-05-26 Phase 3.43 probe dry-run 计划补记

- Phase 3.43 已完成 active scoped probe dry-run plan review。
- 本轮只写计划，不执行 probe；不改接口、不改 schema、不改 objective、不改脚本、不改 gate。
- Future probe 限定为 opening-only、small bounded、quarantine-only、report-only。
- 计划约束：`max_games=1`、`max_depth=1`、seed allowlist `20260423`、`report_only=true`、`selection_gate_eligible=false`。
- Phase 3.44 若执行，只能落在独立 quarantine root，并必须记录 config、stdout/JSON summary、manifest、rollback trigger log。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步只允许 guarded active scoped probe dry-run execution，不允许正式训练或 selection。

## 13. 2026-05-26 Phase 3.44 probe dry-run 执行补记

- Phase 3.44 已执行 quarantine-only dry-run 尝试，但结果是 blocker evidence，不是放行。
- 原无输出路径命令被 `bitboard-train` 拒绝：`output weight path is required`。
- 使用显式 quarantine 输出路径复跑后，命令完成，但 JSON 显示 `active_objective_probe_executed=false`。
- 当前接口只能证明 config accepted + skeleton enabled，不能证明 active objective probe 已执行。
- Quarantine 输出没有 `.bin`，没有 selection/candidate/checkpoint-like 文件；protected scripts/tools 无 diff。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步进入 Phase 3.45 evidence review / blocker review。

## 14. 2026-05-26 Phase 3.45 blocker review 补记

- Phase 3.45 已完成 active scoped probe blocker review。
- Root cause：active scoped config accepted 后只进入 skeleton state，未接到 report-only active probe path。
- Output blocker：`bitboard-train` 当前要求 output weight path，缺少 no-output probe mode。
- `probe-quarantine-output.weights` 只能作为 quarantine evidence，不能进入 selection、release、promotion 或 gate。
- 最小补齐方向：Phase 3.46 做 active probe wiring plan review，并把 no-output probe mode 作为强制执行安全项。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`。

## 15. 2026-05-26 Phase 3.46 wiring/no-output 计划补记

- Phase 3.46 已完成 active probe wiring and no-output mode plan review。
- 本轮只做计划评审，不改代码、schema、CLI 或训练逻辑。
- Future skeleton 最小改动面限定在 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- no-output probe mode 是强制安全项：probe-only 不得产 candidate/checkpoint/production weights。
- active probe、hard reject、guard、root-cost、acceptance companion 仍只能 report-only，不能接 gate/rating/scorecard/perf threshold。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步只允许 guarded active probe wiring skeleton。

## 16. 2026-05-26 Phase 3.47 wiring skeleton 补记

- Phase 3.47 已完成 guarded active probe wiring skeleton。
- 本轮只实现骨架：accepted active scoped config 可以路由到 report-only probe wiring skeleton，但不执行真实 active probe。
- no-output probe mode skeleton 已存在：`probe_only=true` 与 `no_output_weights=true` 绑定使用，允许 probe-only 路径不要求 output weight path。
- no-output mode skeleton 不保存 candidate/checkpoint/production weights，不生成 `.bin`，不接 selection/gate。
- `active_objective_probe_executed=false` 仍是硬边界，后续只有真实 probe execution 阶段才能改变。
- 验证已覆盖 default-off、invalid config、valid scoped config、no-output mode、active probe not executed、weight artifact suppressed。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步只允许 guarded active probe wiring verification package。

## 17. 2026-05-26 Phase 3.48 wiring verification package 补记

- Phase 3.48 已完成 guarded active probe wiring verification package。
- 本轮只验证 Phase 3.47 skeleton/no-output 边界；未执行真实 active objective probe、训练、selection 或 `GamesPerSeed=64`。
- Phase 3.47 manifest 复核为 `wiring_skeleton_implemented=true`、`no_output_probe_mode_implemented=true`、`active_objective_probe_executed=false`。
- Phase 3.48 manifest 固定 `verification_only=true`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- 构建 `surakarta-benchmark`、`surakarta-bitboard-selftest` 并运行 selftest；输出目录未生成 `.bin/.weights/candidate/checkpoint/weights-manifest`。
- `tools` 与三个 protected scripts 的 tracked diff 为空；selection gate/rating/scorecard/perf threshold 没有本轮改动。
- Phase 3 仍是 `not_passed`，Phase 4 仍是 `blocked`；下一步必须另行评审执行边界，不能从验证包直接跳到训练或 selection。
