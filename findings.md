# 发现与决策

## 2026-05-26 Phase 3.62 commit staging plan and tracked evidence materialization

### 本轮边界
- 本轮只做 commit staging plan、tracked evidence summary、pathspec/hunk boundary 和验证包。
- 禁止 stage、commit、stash、reset、checkout、clean；禁止 active probe、dry-run、training、selection、promotion、`GamesPerSeed=64`。
- 禁止修改 `src`、`include`、`tools`、`scripts`、`test\test_data`；本轮只允许四个项目文档与 Phase 3.62 专属输出目录。

### 关键发现
- 锚点仍匹配：`codex/phase2-baseline-trace` / `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- `git diff --cached --name-only` 为空；当前没有 staging area 污染。
- `git check-ignore -v` 显示 Phase 3.60/3.61 evidence markdown 均命中 `.gitignore:4:/build-local/`，所以后续提交准备不能默认把本地 evidence package 整包入仓。
- 合理 archive 策略：tracked docs 保存证据摘要、SHA256、路径索引、复跑命令；大量 `build-local` 证据包保留为本地复核材料，除非未来单独批准 archive policy。
- Phase 3.61 的 ownership review 仍成立：四个 mixed code 文件只能 partial stage，不能 whole-file 纳入 objective diagnostic commit group。

### Staging 分组判断
- `feat: add guarded report-only objective diagnostic evidence`：仅建议人工 partial stage `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 中 active config、no-output、summary、objective marker 和 selftest hunk。
- `docs: consolidate phase 3 diagnostic evidence chain`：建议纳入 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 的 Phase 3.50-3.62 tracked 摘要。
- `docs: prepare phase 3 evidence archive ownership review`：建议纳入 ownership/archive/staging 边界摘要；`build-local` 大包不强塞，别一把梭把本地垃圾桶也当行李托运。
- 必须排除或单独确认：`include\surakarta_bitboard.h`、`src\surakarta_bitboard.cpp`、`src\surakarta_dev_session_selftest.cpp`、`src\surakarta_game.cpp`、`src\surakarta_rule_manager_bitboard.cpp`、`src\surakarta_rule_manager_impl.cpp`、`src\surakarta_utils.cpp`、`test\test_data\perf-baseline.json`、`tools\`。

## 2026-05-26 Phase 3.61 evidence archive and commit preparation ownership review

### 本轮边界
- 本轮只做归档索引和提交准备 ownership review，不 stage、不 commit、不 stash、不 reset、不 checkout、不清理未跟踪文件。
- 禁止修改 `src`、`include`、`tools`、`scripts`、`test\test_data`；只允许更新四个项目文档与 Phase 3.61 专属输出目录。
- 禁止执行 active probe、dry-run、training、selection、promotion 或 `GamesPerSeed=64`。

### 归属发现
- Phase 3.50-3.60 证据目录完整，可作为 evidence archive 索引；若 `build-local` 受 ignore 策略影响，后续 staging plan 必须显式决定是否 force-add 证据目录。
- 建议提交分组 A：`feat: add guarded report-only objective diagnostic evidence`，但只可 partial stage `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp` 中 objective diagnostic 相关 hunk。
- 建议提交分组 B：`docs: consolidate phase 3 diagnostic evidence chain`，包含 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 和经 archive policy 接受的 Phase 3.50-3.60 证据目录。
- 建议提交分组 C：`docs: prepare phase 3 evidence archive ownership review`，包含 Phase 3.61 专属输出目录。
- 必须排除：national rule/terminal semantics、perf baseline、untracked `tools\`、其他历史 `src/include` 脏改动；这些不是 Phase 3.50-3.60 objective diagnostic commit 的东西，硬塞进去就是把锅扣成铁帽子。

### Hunk 判断
- `include\surakarta_search.h`：active objective config 和 active interface summary 字段属于 Phase 3.41-3.56 诊断链；opening-safe drift penalty 字段属于更早历史，需人工确认或单独提交。
- `src\surakarta_search.cpp`：config parser、no-output path、report-only path、guarded objective evidence status 属于诊断链；national terminal semantics 和 earlier opening-safe objective plumbing 不应 whole-file 混入。
- `src\main.cpp`：`--config`、active interface summary 输出、benchmark selftest wrapper 属于诊断链；trace terminal national semantics 属于历史规则语义。
- `src\surakarta_bitboard_selftest.cpp`：`TestActiveObjectiveConfigSkeletonDefaultInvalidAndScopedConfig` 及 Phase 3.52/3.56 断言属于诊断链；terminal golden tests 和 earlier opening-safe objective tests 需排除或单独确认。

## 2026-05-26 Phase 3.60 final evidence consolidation and Phase 4 precondition map

### 本轮边界
- 本轮只做 Phase 3.47-3.59 证据链最终汇总，不做新实现、不执行 active probe/dry-run/training/selection/promotion。
- 允许更新四个项目文档与 Phase 3.60 专属输出目录。
- 禁止修改 `src`、`include`、`tools`、`scripts`、`test\test_data`；禁止把 diagnostic blocker closed 外推成 Phase 3 整体通过或 selection gate 可用。

### 初始发现
- 分支和 HEAD 已锚定：`codex/phase2-baseline-trace` / `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 当前工作区仍存在历史脏改动集合；Phase 3.60 verify 必须以 Phase 3.58-3.59 汇报的 protected diff/hash 作为漂移边界。
- Phase 3.60 的核心任务不是“再证明一次能跑”，而是把从 skeleton 到 objective diagnostic blocker closure 的证据链整理成可审查索引。证据整理当发车许可，那就是把目录页当毕业证，咋看都不硬气。

### 证据链发现
- Phase 3.47-3.52 均保持 `active_objective_probe_executed=false`，证明范围停在 skeleton、verification、readiness、plan 或 report-only path indicator。
- Phase 3.53-3.54 证明 guarded report-only/no-output command path runnable，且 artifact suppression 成立，但 objective marker 仍为 false。
- Phase 3.55 将 false marker 解释为 objective execution blocker。
- Phase 3.56-3.57 首次形成 guarded report-only/no-output objective diagnostic evidence：`active_objective_probe_executed=true`，同时 `games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`。
- Phase 3.58-3.59 将 diagnostic objective blocker 分层判定为 closed，但继续固定 `phase3_status=not_passed`、`phase4_status=blocked`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。

### Final consolidation 发现
- Phase 3.60 已把 Phase 3.47-3.59 证据链整理为状态矩阵、剩余缺口清单和 Phase 4 precondition map。
- 最终允许结论只有：`phase3_diagnostic_objective_blocker_closed=true`。
- 仍不能外推：Phase 3 整体未过、Phase 4 仍 blocked、selection gate 不可用、没有 production-facing artifact。
- Phase 4 precondition map 只列条件：证据归档、diagnostic closure 范围确认、protected diff ownership review、rule correctness/selftest entry condition、先写计划再谈执行、未来执行需单独授权。

## 2026-05-26 Phase 3.58-3.59 objective evidence review and completion gate

### 本轮边界
- 本轮只审查 Phase 3.56-3.57 objective diagnostic evidence 是否足以关闭 Phase 3 diagnostic objective blocker。
- 允许更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 与 Phase 3.58-3.59 专属输出目录。
- 禁止修改 `src`、`include`、`tools`、`scripts`、`test\test_data`，也禁止把诊断证据升级成 production artifact。

### 初始发现
- 分支和 HEAD 已锚定：`codex/phase2-baseline-trace` / `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 当前工作区存在历史脏改动：`include`、`src`、`test\test_data`、`tools` 相关状态需要作为隔离风险记录，本轮不认领。
- Phase 3.56-3.57 记录的核心待复核字段是 `active_objective_probe_executed=true`、report-only/no-output、artifact suppression、`selection_gate_eligible=false`、`training_executed=false`、`selection_executed=false`。
- Completion gate 决策必须分层表达：diagnostic blocker 可以按证据 closed；Phase 4、training、selection、promotion 仍 blocked/no-go。把诊断灯当发车信号，那就是拿体温计当油门，挺有创意但不靠谱。

### Gate 发现
- Phase 3.56-3.57 dry-run stdout 与 report/manifest 一致：`active_objective_probe_executed=true`、`active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`。
- 同一证据保持 `games_completed=0`、`output_weights=""`、`checkpoint_count=0`、`selection_gate_eligible=false`，因此只能关闭 diagnostic objective blocker，不能升级为训练或 selection 证据。
- Phase 3.58-3.59 manifest 固定分层状态：`phase3_diagnostic_objective_blocker_closed=true`、`phase3_status=not_passed`、`phase4_status=blocked`、`production_candidate=false`、`training_executed=false`、`selection_executed=false`。
- Protected tracked diff 仍是历史集合，且本阶段 verify 固化了当前 SHA256；`tools` 仍按历史未跟踪目录处理并记录 hash，不作为本轮输出。

## 2026-05-26 Phase 3.56-3.57 objective-execution report-only blocker closure

### 本轮边界
- 本轮允许最小代码改动，仅闭合 `active_objective_probe_executed=false` blocker。
- 禁止修改 `tools`、`scripts`、`test\test_data`、selection/rating/scorecard/perf threshold。
- 允许一次隔离 guarded report-only/no-output dry-run；禁止 training、selection、promotion、`GamesPerSeed=64`。

### 关键发现
- Phase 3.55 blocker 可用最小 summary evidence 闭合：不需要触碰搜索质量、eval、tuple、feature、weight 或 selection 逻辑。
- 真正需要变化的是 `RunBitboardTraining` 的 no-output report-only summary：从 implementation-only status 改为 guarded diagnostic evidence status，并只在 no-output artifact-suppressed path 下置 true。
- 默认 config、invalid config、ordinary output path 都必须保留 false；本轮 selftest 已补 ordinary output path false，防止普通训练/导出路径误置 true。
- dry-run 证明 true marker 与 no-output 边界能同时成立：`active_objective_probe_executed=true`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- 即使 true marker 成立，`selection_gate_eligible=false` 与 `production_candidate=false` 仍是硬边界；下一步只能评审 objective evidence，不进 training/selection。

## 2026-05-26 Phase 3.55 dry-run evidence interpretation and blocker closure

### 本轮边界
- 本轮只做证据解释、只读源码审查、blocker closure 文档化和 verify 包。
- 禁止再次执行 active probe、training、selection、promotion、`GamesPerSeed=64`。
- 禁止修改 `src`、`include`、`tools`、`scripts`、`test\test_data`。

### 关键发现
- Phase 3.53-3.54 已证明 CLI guarded config path 可运行：`active_interface_report_only_probe_path=true`、`active_interface_no_output_probe_mode=true`、`active_interface_weight_artifact_suppressed=true`。
- 同一证据也证明 objective execution 仍未发生：`active_objective_probe_executed=false`、`games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- `src\surakarta_search.cpp` 中 `ParseActiveObjectiveConfigSkeleton` 保持 `active_objective_probe_executed=false`；`RunBitboardTraining` 在 summary 中再次固定 false。
- `no_output_probe_mode` 分支在 weight bootstrap/save、checkpoint save、training loop 之前返回，这解释了为什么不会产生权重，也解释了为什么当前没有 objective diagnostic execution evidence。
- 最小下一步不是训练，而是 Phase 3.56：先定义 report-only objective diagnostic helper 的语义、状态位切换条件、自测矩阵和 consumer isolation，再单独授权实现。

## 2026-05-26 Phase 3.53-3.54 guarded report-only probe readiness and dry-run

### 本轮边界
- 本轮只允许构建、自测、Phase 3.52 verify，以及一次 guarded report-only/no-output dry-run。
- 本轮只写 Phase 3.53-3.54 输出目录和四个允许文档；不改 `src`、`include`、`tools`、`scripts`、`test\test_data`。
- 禁止 training、selection、promotion、`GamesPerSeed=64`、`.bin/.weights/candidate/checkpoint/weights-manifest`。

### 关键发现
- Phase 3.52 verify 重新通过，证明最小实现仍满足 readiness review 前置条件。
- no-output guarded path 在源码中位于保存权重和训练循环之前返回；dry-run 实测也显示 `games_completed=0`、`output_weights=""`、`checkpoint_count=0`。
- dry-run stdout 证明 `active_interface_report_only_probe_path=true` 且 artifact suppression 生效；输出目录禁用产物扫描为空。
- 运行时仍给出 `active_objective_probe_executed=false` 和 status `report_only_active_probe_path_implemented_no_execution`；因此结论不能升级成 objective execution、selection eligibility 或 Phase 4 放行。
- 保护路径 diff 与 Phase 3.51/3.52 记录的历史 scoped 集合一致；本轮没有新增 `src/include/tools/scripts/test_data` 漂移。

## 2026-05-26 Phase 3.52 report-only active probe path minimal implementation

### 本轮边界
- 本轮允许最小修改 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`。
- 本轮禁止修改 `tools`、`scripts`、`test\test_data`、selection/rating/scorecard/perf threshold。
- 本轮未运行真实 probe、training、selection、promotion 或 `GamesPerSeed=64`。
- 已在 Phase 3.52 输出目录记录四个已脏允许代码文件的 session-baseline SHA256。

### 关键发现
- Phase 3.47 skeleton 已能接受 `probe_only=true` + `no_output_weights=true`，但缺少可审计的 report-only path indicator。
- 本轮最小实现不需要触碰搜索质量、eval、tuple、feature、weight 或训练逻辑；只补 summary/status 和自测。
- `active_interface_report_only_probe_path` 只表达“受保护 report-only path 已接线”，不表示执行许可或 candidate 资格。
- `games_completed=0`、`output_weights_path.empty()`、`selection_gate_eligible=false`、`active_objective_probe_executed=false` 仍是自测硬边界。
- `surakarta-benchmark.exe bitboard-selftest` 是为满足验收入口补的同目录 selftest 转发，不进入训练或 probe 路径。

## 2026-05-26 Phase 3.51 report-only active probe path implementation plan

### 本轮边界
- 本轮只做 implementation plan，不改生产代码，不进入执行阶段。
- 只新增 Phase 3.51 输出目录三件套与 `verify_phase3_51.ps1`，并更新四个允许文档。
- 不改 `src`、`include`、`tools`、protected scripts、selection/rating/scorecard/perf threshold。
- 不执行 probe、training、selection、promotion 或 `GamesPerSeed=64`。

### 关键发现
- 当前架构已经有 `ActiveObjectiveConfigSkeleton`、`probe_only`、`no_output_weights`、summary 输出与 selftest skeleton 覆盖，但真实 report-only diagnostic route 仍未落地。
- Phase 3.52 的最小实现应围绕现有 `--config` 入口和 no-output skeleton 做最小连接，不新增散乱 CLI toggle。
- Future helper 必须只写 report fields，不改变 weights、traces、search/eval、tuple/features 或 consumer gate。
- selftest 缺口集中在 future diagnostic-only summary、artifact suppression、invalid config hard stop、consumer isolation。
- Phase 3.51 不能改变 readiness：`ready_for_guarded_probe=false` 继续保持；Phase 3 和 Phase 4 状态不升级。

## 2026-05-26 Phase 3.50A acceptance audit addendum

### 本轮边界
- 本轮只补 Phase 3.50 acceptance audit addendum，不改变 Phase 3.50 结论，不进入 Phase 3.51。
- 只更新 Phase 3.50 输出目录内 addendum/manifest/verify 脚本，以及四个允许文档。
- 不改 `src`、`include`、`tools`、protected scripts、selection/rating/scorecard/perf threshold。
- 不执行 active probe、training、selection、promotion 或 `GamesPerSeed=64`。

### 关键发现
- 原 Phase 3.50 verify marker 与验收期望名不完全对齐；本轮已对齐为 `PHASE3_49_BLOCKER_INPUT_OK_PHASE3_50` 和 `WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_50`，同时保留 `STATUS_FLAGS_OK_PHASE3_50`。
- 当前保护范围存在历史 scoped diff：`include`、`src`、`test/test_data` 下若干文件；本轮验收口径是记录该集合并要求复验时集合不漂移。
- `tools` 目录当前在 `git status --short` 中是未跟踪目录；本轮不编辑其中任何文件，且 addendum 给出每个 tool 文件的 SHA256。
- protected scripts hash 固定为：`perf-runner.ps1=5652BFA0...DB84`、`benchmark-select.ps1=3612E412...9DB3A`、`benchmark-train.ps1=B09D4D07...A0A82`。
- Phase 3.50A 不给 Phase 3.51 扩设计、不改变 readiness：`ready_for_guarded_probe=false` 继续是硬状态。

## 2026-05-26 Phase 3.50 guarded active probe blocker closure plan

### 本轮边界
- 本轮只做 Phase 3.50 blocker closure plan 与 Phase 3.51 readiness gate。
- 只修改允许文件：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 与 Phase 3.50 输出目录。
- 不改 `src`、`include`、`tools`、三个 protected scripts、selection gate/rating/scorecard/perf threshold。
- 不执行 active probe、training、selection，不跑 `GamesPerSeed=64`，不生成权重、候选或 checkpoint。

### 关键发现
- Phase 3.47 与 Phase 3.48 的证据链只能证明 skeleton/no-output boundary，不能证明真实 report-only active probe path。
- Phase 3.49 readiness review 已明确 `ready_for_guarded_probe=false`，因此 Phase 3.50 不能转成执行阶段。
- 当前 blocker 是执行路径证明缺失，不是 manifest 字段缺失；硬把它写 ready，那就是拿路障当终点线，挺热闹但方向错了。
- 最小后续边界必须继续使用 `surakarta-benchmark.exe bitboard-train --config <path>`，default-off、opening-only、report-only、quarantine-only，且保持 `selection_gate_eligible=false` 与 `production_candidate=false`。
- Phase 3.51 可以开工，但只能是 report-only probe path implementation plan 或单独授权的最小实现任务；不能直接执行 probe、训练或 selection。
- 失败退出规则固定为：锚点漂移、输入 manifest 漂移、出现 forbidden artifact、需要改 protected consumer、需要训练/selection/promotion/`GamesPerSeed=64`、或任何文案把 skeleton 验证偷换成更强状态。

## 2026-05-26 Phase 3.49 guarded active probe execution readiness review

### 本轮边界
- 本轮只做 Phase 3.49 readiness review，判断是否具备进入后续真实 guarded probe 执行阶段的条件。
- 只修改允许文件：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 与 Phase 3.49 输出目录。
- 不改 `src`、`include`、`tools`、三个 protected scripts、selection gate/rating/scorecard/perf threshold。
- 不执行 probe、training、selection，不跑 `GamesPerSeed=64`，不生成权重、候选或 checkpoint。

### 关键发现
- Phase 3.47 manifest 已满足输入边界：`wiring_skeleton_implemented=true`、`no_output_probe_mode_implemented=true`、`active_objective_probe_executed=false`。
- Phase 3.48 manifest 已满足 verification input：`verification_only=true`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`。
- 源码事实仍是 skeleton/no-output guard：summary 写出 `active_objective_probe_executed=false`，没有已证明的真实 report-only probe path。
- Readiness package 固定 `ready_for_guarded_probe=false`、`phase3_status=not_passed`、`phase4_status=blocked`、`selection_gate_eligible=false`、`production_candidate=false`。
- protected hash 复核结论：`tools/*`、三个 protected scripts、selection gate/rating/scorecard、perf threshold before/after 一致；tracked diff 为空。
- 当前 blocker 不是“文档不够漂亮”，而是执行路径没被证明。硬写 ready 就是拿说明书当发动机，东北话讲：这玩意儿可不能靠嗓门启动。

## 2026-05-26 Phase 3.48 guarded active probe wiring verification package

### 本轮边界
- 本轮只做 Phase 3.48 verification package，验证 Phase 3.47 wiring skeleton/no-output 边界。
- 只修改允许文件：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md` 与 Phase 3.48 输出目录。
- 不改 `src`、`include`、`tools`、三个 protected scripts、selection gate/rating/scorecard/perf threshold。
- 不执行 active objective probe，不训练，不 selection，不跑 `GamesPerSeed=64`，不生成权重、候选或 checkpoint。

### 关键发现
- Phase 3.47 manifest 已满足 verification input：`wiring_skeleton_implemented=true`、`no_output_probe_mode_implemented=true`、`active_objective_probe_executed=false`。
- 当前 skeleton/no-output 边界的正确结论是“可以验证 wiring/no-output guard”，不是“可以推进训练或 selection”。
- `surakarta-bitboard-selftest` 仍是本阶段唯一运行验证；它覆盖 no-output mode、weight artifact suppressed、probe-not-run evidence。
- 输出目录三件套固定 blocked/not-passed 状态位：`verification_only=true`、`active_objective_probe_executed=false`、`training_executed=false`、`selection_executed=false`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。
- protected hash 复核结论：`tools/*` 聚合 hash、三个 protected scripts、selection gate/rating/scorecard、perf threshold before/after 一致；tracked diff 为空。
- 下一步如果要推进，只能先单独评审真实 probe 执行边界；不能从验证包直接滑进训练/selection。现在这阶段就像验收消防门能不能关上，不能验完就宣布商场开业，别整那套。

## 2026-05-26 Phase 3.40 active scoped interface plan review

### 本轮边界
- 本轮执行 Phase 3.40：只做 active scoped interface plan review，不实现接口，不新增 CLI flag，不运行 active probe。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_40_active_scoped_interface_plan_review`。
- 只新增 Phase 3.40 三件套并更新四个项目文档；不改 `src`、`include`、`tools`、`scripts`、gate/rating/scorecard/perf threshold。

### 关键发现
- Phase 3.39 default-off quarantine dry-run 证明既有 `surakarta-benchmark.exe bitboard-train` 能在隔离目录内最小运行，但不提供 active probe 接口。
- 当前工程仍无独立 `bitboard-train.exe` 或 CMake target；Phase 3.40 方案必须围绕既有 subcommand 设计，不能凭空捏入口。
- 检查点 3 的粗正则命中 report 字段 `opening_safe_objective_enabled`，不是 CLI activation flag；真实 `--flag` 扫描为 0，确认当前 active scoped interface 仍未实现。
- Future 最小接口建议采用 config-file activation surface：默认关闭，显式启用，opening-only scoped，report-only，quarantine-only，`selection_gate_eligible=false`。
- hard reject / opening guard / root-cost / acceptance companion 继续只能作为 report-only 证据，不能接入 gate、rating、scorecard 或 perf threshold。
- Phase 3.40 允许进入 Phase 3.41 `guarded active interface skeleton`，但不允许直接跑 active probe。现在要是直接开 probe，就跟图纸刚打印完就抡锤砸承重墙，热闹是真热闹，靠谱是真不靠谱。

## 2026-05-25 Phase 3.39 guarded limited training dry-run execution

### 本轮边界
- 本轮执行 Phase 3.39：只做 default-off / isolated / quarantine-only 最小训练 dry-run。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_39_guarded_limited_training_dry_run_execution`。
- 不改 `src`、`include`、`tools`、`scripts`、gate/rating/scorecard/perf threshold；不跑 selection、`GamesPerSeed=64` 或 promotion。

### 关键发现
- 当前锚定 commit 复核通过：分支 `codex/phase2-baseline-trace`，HEAD `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- 原检查点 2/4 写法里的 `bitboard-train` target / `bitboard-train.exe` 与当前工程不匹配；CMake help 和 `src\CMakeLists.txt` 均显示实际 target 为 `surakarta-benchmark`，训练入口是 `surakarta-benchmark.exe bitboard-train`。
- `src\main.cpp` 中 `opening_safe_objective_enabled` 是输出字段，不是 activation flag；补充扫描确认没有 `--opening*`、`--opening-safe`、`--*objective*`、`--enable*objective`、`--objective*enable` 这类 CLI activation flags。
- Phase 3.39 dry-run summary 可解析，关键默认关闭证据为 `opening_safe_objective_enabled=false`、`opening_drift_penalty_active=false`、`opening_drift_penalty_value=0.0`、`opening_drift_penalty_scope_status=inactive`、`inactive_path_equivalent=true`。
- Phase 3.39 quarantine 输出只落在 Phase 3.39 输出目录下；权重和 checkpoint 名称不含 `selection` 或 `candidate`。
- 结论：允许进入 Phase 3.40 `active scoped interface plan review`，但只允许评审接口计划；active objective probe 仍 blocked。没有接口还想 active probe，那就是拿筷子修发动机，姿势挺忙，结果全靠玄学。

## 2026-05-25 Phase 3.38 guarded limited training dry-run plan

### 本轮边界
- 本轮执行 Phase 3.38：只做 guarded limited training dry-run plan，不执行训练，不执行 selection，不生成 `.bin`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_38_guarded_limited_training_dry_run_plan`。
- 只新增 Phase 3.38 三件套并更新四个项目文档；不改 `src`、`include`、`tools`、protected scripts、gate/rating/scorecard/perf threshold。

### 关键发现
- Phase 3.37 给出的最小边界可以转成 future Phase 3.39 dry-run execution 模板，但必须强制输出隔离。
- `scripts\benchmark-train.ps1` 当前没有 output root 参数，会自动写 `build-local\training\<timestamp>`；Phase 3.39 若要满足隔离红线，不能把它当主入口。
- `bitboard-train` 当前 CLI 没有 opening objective activation flags；因此 Phase 3.39 只能做 default-off quarantined limited dry-run。active scoped objective 不是靠写计划能变出来的，硬写就是画饼糊墙。
- future dry-run 需要先导出 bootstrap quarantine weight，再基于它跑最多 4 局 limited probe，所有权重与 checkpoints 均留在 Phase 3.39 quarantine 子目录。
- 允许进入 Phase 3.39 guarded limited training dry-run execution；范围限定为 default-off / isolated / quarantine-only。

### 工作区决策
- `src` / `include` / `tools` 的既有 diff 继续按历史脏状态处理，不记为 Phase 3.38 成果。
- Phase 3.38 本轮可归属改动限定为输出目录三件套、`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。

## 2026-05-25 Phase 3.37 guarded limited training plan review

### 本轮边界
- 本轮执行 Phase 3.37：只做 guarded limited training plan review，不执行训练，不执行 selection，不生成 `.bin`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_37_guarded_limited_training_plan_review`。
- 只新增 Phase 3.37 三件套并更新四个项目文档；不改 `src`、`include`、`tools`、protected scripts、gate/rating/scorecard/perf threshold。

### 关键发现
- Phase 3.36 是本轮必要前置证据：它证明 diagnostic dry-run 已完成，同时 `training_executed=false`、`selection_executed=false`、`bin_generated=false`、`selection_gate_eligible=false`。
- future 最小训练只能是诊断隔离 probe：单 seed、最多 4 局、depth 4、checkpoint every 2，不做 selection、不做 promotion、不碰 gate。
- opening objective 的未来启用必须显式 scoped；default-off control 仍是第一比较对象，hard reject 仍只能零 penalty 并进入 report-only 诊断。
- 监控重点不是“看起来变强没”，而是参数可追溯、TD error/target delta/weight delta、NaN/Inf、scope hit、hard reject、artifact quarantine 和 `selection_gate_eligible=false`。想拿小样本吹强度，那就是拿火柴当探照灯，亮是亮，照不了路。
- 允许进入 Phase 3.38 guarded limited training dry-run plan；不允许从 Phase 3.37 直接启动训练。

### 工作区决策
- `src` / `include` / `tools` 的既有 diff 继续按历史脏状态处理，不记为 Phase 3.37 成果。
- Phase 3.37 本轮可归属改动限定为输出目录三件套、`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。

## 2026-05-25 Phase 3.35 guarded diagnostic dry-run plan

### 本轮边界
- 本轮执行 Phase 3.35：只设计 future guarded diagnostic dry-run，不执行 dry-run，不新增脚本，不改代码。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_35_guarded_diagnostic_dry_run_plan`。
- 只写 dry-run plan 三件套和四个项目文档；禁止训练、selection、`GamesPerSeed=64`、`.bin`、gate/rating/scorecard/perf threshold 改动。

### 关键发现
- Phase 3.34 已确认 Phase 3.33 objective path 是 default-off 且 not selection eligible；Phase 3.35 只能在这个边界内规划诊断。
- future dry-run 应覆盖 default-off control、inactive enabled control、active scoped sample、hard reject report-only sample。
- hard reject 仍不能作为 gate 输入，只能作为报告字段；`selection_gate_eligible` 必须继续为 false。
- 下一步可以是 Phase 3.36 guarded diagnostic dry-run execution plan 或 blocker review。直接训练这个念头很刺激，但现在拿它开跑就是硬把护栏当跳板，别整。

## 2026-05-25 Phase 3.34 guarded implementation verification package

### 本轮边界
- 本轮执行 Phase 3.34：验证 Phase 3.33 default-off guarded objective 实现边界，不新增 objective 逻辑。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_34_guarded_implementation_verification_package`。
- 只写验证包和项目文档；不训练、不 selection、不跑 `GamesPerSeed=64`、不生成 `.bin`、不改 gate。

### 关键发现
- Phase 3.33 第一次真正落地 default-off objective path，且 `training_loss_modified=true` 符合计划；但默认关闭、未进入训练/selection/gate，candidate 资格仍为 false。
- default-off / inactive equivalence / active scoped / hard reject report-only 边界由 `surakarta-bitboard-selftest` 覆盖并通过。
- hard reject 仍只是 report-only acceptance companion，不影响 rating、scorecard、perf threshold 或 selection gate。
- 允许进入 Phase 3.35 guarded diagnostic dry-run plan；禁止直接开练。刀磨亮了归磨亮了，别拿自己腿当砧板。

## 2026-05-25 Phase 3.33 guarded minimum-objective implementation

### 本轮边界
- 本轮执行 Phase 3.33：把 Phase 3.32 的计划收束为 default-off、可回滚的最小 opening-safe objective 代码路径。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_33_guarded_minimum_objective_implementation`。
- 只触碰允许代码面和四个文档；`tools\*`、三个 benchmark/perf 脚本和 gate 不动。
- 不运行训练流程，不运行 selection，不跑 `GamesPerSeed=64`，不生成阶段 `.bin`。
- Phase 3 保持 `not_passed`；Phase 4 保持 `blocked`。

### 核心发现
- 最小公式能安全落在 `ApplyTrainingStep` 的 guarded active path 上，但必须以 `opening_safe_objective_enabled=false` 和 zero/empty defaults 托底。
- inactive path 的技术关键不是“没报错”，而是 weights、traces、target/current/TD error、weight delta、changed count 这些 checked 字段与 baseline 对齐。这个已经由 selftest 覆盖。
- active scoped fixture 用 alpha-zero 验证公式贡献和诊断，不把验证动作扩成真实训练。这里要是顺手开练，那就不是工程，是玄学加速器。
- hard reject 必须短路为 `hard_reject_report_only` 和零 penalty；它只留下审计痕迹，不给 gate、rating、scorecard、perf threshold 喂料。
- trace CLI 崩溃的根因是 stale `surakarta-benchmark.exe` 没按当前 `ApplyTrainingStep` ABI 重新链接；重建 target 后自测通过。

### 实现决策
- `TrainingOptions` 新增 opening-safe objective 输入字段，全部默认 disabled/zero/empty/false。
- `TrainingStepResult` / `TrainingSummary` 增加 report-only diagnostic fields，`selection_gate_eligible=false` 固定守边界。
- `BuildOpeningSafeObjectiveSkeletonState` 计算 zero-effect 状态和 `active_scoped` penalty。
- `ApplyTrainingStep` 只在 active scoped 时扣减 TD target；默认关闭和 inactive 状态不改 checked 行为。
- 下一阶段允许 verification/diagnostic review；不允许训练、selection、`.bin`、gate 或候选资格推进。

## 2026-05-24 Phase 3.32 minimum objective implementation plan review

### 本轮边界
- 本轮执行 Phase 3.32：把 Phase 3.31 的最小公式设计转换成 future guarded work 的可审查计划。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_32_minimum_objective_implementation_plan_review`。
- 只新增 Phase 3.32 三件套并更新四个项目文档。
- 不修改 `src\*`、`include\*`、`tools\*`、`scripts\*`，不改 training loss，不改权重更新，不改自博弈/搜索/selection，不训练，不生成 `.bin`。
- Phase 3 保持 `not_passed`；Phase 4 保持 `blocked`。

### 核心发现
- Phase 3.31 的公式设计已经足够转换成执行计划，但最小改动面必须被压到训练 option/report contract、受控公式 helper、default-off CLI/report、自测 fixture 这些点上。
- 后续如果单独授权，核心路径只能是：先证明 default-off 和 inactive equivalence，再加纯 helper，再考虑 guarded loss hook。顺序反过来就是先把药倒锅里再问配方，离谱。
- hard reject、acceptance companion、root-cost companion、opening guard 仍只能作为 report-only 输入；不得进入 rating、scorecard、perf threshold 或 selection gate。
- 测试矩阵必须覆盖 default-off、inactive equivalence、formula helper、active scoped fixture、hard reject、report-only wiring、CLI/report、forbidden path guard、artifact guard。
- rollback 线必须比实现欲望更硬：任何禁改路径变化、默认值偏离 disabled/zero/empty/false、disabled path 改行为、companion 被 forbidden consumer 吃掉、训练/selection/`.bin` 出现，都必须停止。
- 下一阶段可在单独授权后进入 future guarded minimum-objective work；但 Phase 3.32 自身没有给训练、selection、Phase 4 或产物推广开口子。

### 计划决策
- future allowed surface 继承 Phase 3.24/3.31 的最小集合，并继续排除 `tools\*` 和三个 benchmark/perf 脚本。
- `scripts\workflow.tests.ps1` 只在 CLI/report default-off 断言确有必要时可进入 future touch list；不能扩散成 benchmark 或 gate 改动。
- `selection_gate_eligible=false` 继续作为 companion 输出的固定状态。
- Phase 3.32 结论只允许下一阶段在重新授权下做 guarded minimum-objective work，不允许把本阶段写成落地许可。

## 2026-05-24 Phase 3.31 minimum objective formula design review

### 本轮边界
- 本轮执行 Phase 3.31：把 Phase 3.22 / 3.24 / 3.30 的 skeleton 证据链升级为 minimum objective formula design review。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_31_minimum_objective_formula_design_review`。
- 只新增 Phase 3.31 三件套并更新四个项目文档。
- 不修改 `src\*`、`include\*`、`tools\*`、`scripts\*`，不实现 objective，不修改 training loss，不训练，不 selection，不生成 `.bin`。
- Phase 3 保持 `not_passed`；Phase 4 保持 `blocked`。

### 核心发现
- Phase 3.22 给出的最小公式语义可以收束为 `opening_safe_penalty = opening_drift_penalty_weight * unsafe_rank_degradation * bounded_root_cost_multiplier * opening_scope_mask`。
- 公式边界必须同时满足 default-off 显式启用、非零权重、已知 opening root case、max-ply window、baseline/drift child 可识别、unsafe rank degradation、bounded costly migration、无 aspiration retry explanation、无 hard reject。
- 任何边界条件缺失时，公式有效值必须为 `0`，且不得改变 TD target、eligibility trace、自博弈、eval/search、weight update、selection policy、脚本默认值或 manifest 兼容性。
- hard reject 仍是 `report-only acceptance companion` 的审计输出，只能写 `hard_reject_triggered` 和 `hard_reject_reasons` 这一类字段。要是把它塞进 gate/rating/scorecard/perf threshold，那就不是评审，是偷换裁判，得拦。
- 不可实现项清单已经固化：objective implementation、training loss change、TD target change、eligibility trace change、self-play move selection change、eval/search change、weight update change、selection gate/rating/scorecard/perf threshold input、tuple shrinkage、feature expansion、opening book behavior、`.bin` generation/promotion、training/selection/`GamesPerSeed=64`、Phase 4 entry。
- 下一阶段只允许进入 implementation plan review，不授权实现。先把施工图审明白，再谈动代码；否则这活儿容易从“公式设计”滑成“loss 里硬塞偏方”。

### 设计决策
- 输入字段分三类：default-off options、root-child relationship evidence、bounded companion status。
- 输出字段分三类：formula diagnostic fields、report-only hard reject fields、selection unchanged guard fields。
- `unsafe_rank_degradation` 必须基于 child relationship 和 rank change，不得硬编码固定 move label。
- `bounded_root_cost_multiplier` 必须来自 companion status，不得直接吃 raw timing。
- `selection_gate_eligible=false` 是本阶段固定边界。

## 2026-05-24 Phase 3.30 skeleton validation evidence package

### 本轮边界
- 本轮执行 Phase 3.30：只汇总 Phase 3.25-3.29 skeleton validation 证据并更新文档。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_30_skeleton_validation_evidence_package`。
- 不修改 `src\*`、`include\*`、`tools\*` 或三个 benchmark/perf 脚本。
- 不实现 objective，不修改 training loss，不新增 fixture，不训练，不 selection，不生成 `.bin`。
- Phase 3 still not passed；Phase 4 still blocked。

### 核心发现
- Phase 3.25-3.29 的骨架链路证据已经闭合：default-off、inactive equivalence、scoped activation no-op、report-only wiring、hard reject fixture 都有 JSON evidence。
- 所有输入 manifest/report 共同保持 `objective_formula_implemented=false`、`training_loss_modified=false`、`training_executed=false`、`selection_executed=false`、`bin_generated=false`、`production_candidate=false`、`selection_gate_eligible=false`。
- Phase 3.30 只允许下一步进入 **Phase 3.31 minimum objective formula design review**。这个是设计评审，不是实现许可；别把“可以画图纸”听成“可以上电焊”，那就是手欠。
- Phase 3 仍未通过，Phase 4 仍 blocked；任何 training loss、训练、selection、`.bin` 或 Phase 4 入口都必须继续拦住。

### Hash 决策
- `src\*` 聚合 hash：`03DE634F02DE75B614291C95883F9A7F34FE043A41336EA71A5B141B31613B3F`，before/after 一致。
- `include\*` 聚合 hash：`47277A231BDD82540172BD7630E397B88B3F666BCFC7901D6342B6C194B7E824`，before/after 一致。
- `tools\*` 聚合 hash：`A780E44B3CB444582F5711991C9ED5D35240C8A1CBA939AEA93E04426A55C0BA`，before/after 一致。
- `scripts\perf-runner.ps1` hash：`5652BFA038A232ABFEC1462D3B62D7A804F9F5998B68E2067507F4136376DB84`，unchanged。
- `scripts\benchmark-select.ps1` hash：`3612E4123EF6BE94E0ECEAA69F80D3E9BF511C7617651B422D480DB1AF09DB3A`，unchanged。
- `scripts\benchmark-train.ps1` hash：`B09D4D07002F23464BB4E2139BFCEAE2F5DC0DA326470E2D513F85023DCA0A82`，unchanged。

## 2026-05-24 Phase 3.29 hard reject fixture proof

### 本轮边界
- 本轮执行 Phase 3.29：只增加 hard reject fixture proof / marker / evidence。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_29_hard_reject_fixture_proof`。
- 不实现 objective，不修改 training loss，不改变权重更新语义，不新增训练/selection 路径，不生成阶段 `.bin`，不触碰 gate/rating/scorecard/perf threshold。
- 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 核心发现
- Phase 3.22 的 `hard_reject_conditions` 与 Phase 3.24 的 `hard_reject_fixtures` 共同定义了 11 条 hard reject 条件；它们的用途是 fixture/evidence，不是 selection gate。
- Phase 3.28 已证明 hard reject / opening guard / root-cost companion / acceptance companion 只进入 `report-only evidence chain`。
- Phase 3.29 在 `TestOpeningSafeObjectiveDefaultOffAndInactiveEquivalence` 内增加 selftest-only fixture matrix，逐条断言来源为 `Phase 3.22/3.24`、sink 为 `report-only evidence`、gate effect 为 `none`。
- marker 为 `Phase 3.29 hard reject fixture proof marker: report-only final 20260423`。
- `surakarta_search.cpp` 未出现 training loss/objective 实现痕迹；这轮就是证据钉钉子，不是往训练里塞锤子。

### Hash 决策
- `src\surakarta_bitboard_selftest.cpp` hash：`51CE4A8DA95C7AC264248DB856B4308709C2957344D6F406CB1950E2D01C19E3` -> `E33C891D707B60A91103A208698A7E6E6E2467781BFCD8938FAE91A13102ED9C`。
- `tools\*` 本轮实盘聚合 hash：`A780E44B3CB444582F5711991C9ED5D35240C8A1CBA939AEA93E04426A55C0BA`，before/after 一致。
- `scripts\perf-runner.ps1` hash：`5652BFA038A232ABFEC1462D3B62D7A804F9F5998B68E2067507F4136376DB84`，unchanged。
- `scripts\benchmark-select.ps1` hash：`3612E4123EF6BE94E0ECEAA69F80D3E9BF511C7617651B422D480DB1AF09DB3A`，unchanged。
- `scripts\benchmark-train.ps1` hash：`B09D4D07002F23464BB4E2139BFCEAE2F5DC0DA326470E2D513F85023DCA0A82`，unchanged。

### 验证环境发现
- 实际 CMake build dir 继续固定为 `D:\Dev\Code\surakarta_traditional\build-vs-release`。
- target 与 exe 名统一为 `surakarta-bitboard-selftest` / `surakarta-bitboard-selftest.exe`；这次没有再把 underscore 旧名塞回验证命令。

## 2026-05-24 Phase 3.28 report-only diagnostics wiring proof

### 本轮边界
- 本轮执行 Phase 3.28：只增加 report-only diagnostics wiring proof / marker / evidence。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_28_report_only_diagnostics_wiring_proof`。
- 不实现 objective，不修改 training loss，不改变权重更新语义，不新增训练/selection 路径，不生成阶段 `.bin`，不触碰 gate/rating/scorecard/perf threshold。
- 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 核心发现
- Phase 3.24 明确要求 report-only diagnostics 和 hard reject fixtures 不进入 gate；Phase 3.26/3.27 已证明 default-off 与显式 activation skeleton 均 no-op。
- Phase 3.28 在 `TestOpeningSafeObjectiveDefaultOffAndInactiveEquivalence` 内增加 selftest-only wiring matrix：`hard reject`、`opening guard`、`root-cost companion`、`acceptance companion` 只允许进入 `report-only evidence chain`。
- 自测显式断言 `selection_gate_consumes_diagnostics=false`、`rating_consumes_diagnostics=false`、`scorecard_consumes_diagnostics=false`、`perf_threshold_consumes_diagnostics=false`。
- marker 为 `Phase 3.28 report-only diagnostics wiring proof marker`，满足验证脚本的 `report.only.*wiring|wiring.*report.only` 检查。

### Hash 决策
- `src\surakarta_bitboard_selftest.cpp` hash：`E6AA731A48CEABFD76443794B7E1F21CDB6C9E3AEAC5A1D17B314006982ED505` -> `51CE4A8DA95C7AC264248DB856B4308709C2957344D6F406CB1950E2D01C19E3`。
- `tools\*` 本轮实盘聚合 hash：`A780E44B3CB444582F5711991C9ED5D35240C8A1CBA939AEA93E04426A55C0BA`，before/after 一致。
- `scripts\perf-runner.ps1` hash：`5652BFA038A232ABFEC1462D3B62D7A804F9F5998B68E2067507F4136376DB84`，unchanged。
- `scripts\benchmark-select.ps1` hash：`3612E4123EF6BE94E0ECEAA69F80D3E9BF511C7617651B422D480DB1AF09DB3A`，unchanged。
- `scripts\benchmark-train.ps1` hash：`B09D4D07002F23464BB4E2139BFCEAE2F5DC0DA326470E2D513F85023DCA0A82`，unchanged。

### 验证环境发现
- 实际 CMake build dir 固定为 `D:\Dev\Code\surakarta_traditional\build-vs-release`，该目录含 `CMakeCache.txt`。
- target 名为 `surakarta-bitboard-selftest`，产物名为 `surakarta-bitboard-selftest.exe`。
- 用户命令块中 `$testExe` filter 的旧 underscore 名会导致构建后找不到 exe；按 hyphen 产物名定位后，`REPORT_ONLY_WIRING_OK` 与 `VALIDATION_OK_PHASE3_28_REPORT_ONLY_WIRING` 均通过。别让一个下划线把整条证据链绊个跟头，丢人但好修。

## 2026-05-24 Phase 3.27 scoped activation no-op fixture

### 本轮边界
- 本轮执行 Phase 3.27：只增加 scoped activation no-op fixture 和证据包。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_27_scoped_activation_noop_fixture`。
- 不实现 objective，不修改 training loss，不改变权重更新语义，不新增训练/selection 路径，不生成阶段 `.bin`，不触碰 gate/rating/scorecard/perf threshold。
- 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 核心发现
- Phase 3.25 / 3.26 证据链确认 skeleton 仍是 default-off / no-op；Phase 3.27 只验证显式 enabled skeleton 仍不改变训练 step 语义。
- 新 fixture 在 `TestOpeningSafeObjectiveDefaultOffAndInactiveEquivalence` 内比较 baseline training step 与显式 activation skeleton path。
- 显式 activation options 打开 enabled、diagnostics、root case id 和 ply window，但 penalty weight 为 `0.0`，当前 skeleton 不计算 objective，也不接入 TD target 或权重更新。
- 比较字段为 weights、traces、current value、target value、td error、abs weight delta、changed weight count。
- marker 为 `Phase 3.27 scoped activation no-op`。

### Hash 决策
- `src\surakarta_bitboard_selftest.cpp` hash：`492FE4F421DC4BCF766D9F5163F21219F68E077EE03059CFFDC7CCD706A2DBB6` -> `E6AA731A48CEABFD76443794B7E1F21CDB6C9E3AEAC5A1D17B314006982ED505`。
- `tools\*` 继续按 Phase 3.25 manifest 聚合 hash 记录为 `CC9462DAE45009755A41F7F684DCA61D639C05ECFB17C234A6F94F3731934BB5`，before/after 一致。
- `scripts\perf-runner.ps1` hash：`5652BFA038A232ABFEC1462D3B62D7A804F9F5998B68E2067507F4136376DB84`，unchanged。
- `scripts\benchmark-select.ps1` hash：`3612E4123EF6BE94E0ECEAA69F80D3E9BF511C7617651B422D480DB1AF09DB3A`，unchanged。
- `scripts\benchmark-train.ps1` hash：`B09D4D07002F23464BB4E2139BFCEAE2F5DC0DA326470E2D513F85023DCA0A82`，unchanged。

### 验证环境发现
- `build-local` 当前没有 `CMakeCache.txt` / `build.ninja`，不是 CMake build dir。
- 用户字面验证命令实际失败点为 `Error: not a CMake build directory (missing CMakeCache.txt)`。
- CMake target 实际为 `surakarta-bitboard-selftest`，不是 `surakarta_bitboard_selftest`。
- `VsDevCmd.bat -no_logo` 在本 shell 中使用 x86 library path，导致 x64 object 链接 x86 libs；可用本机入口必须显式加 `-arch=x64 -host_arch=x64`。

## 2026-05-24 Phase 3.26 scoped inactive-equivalence verification

### 本轮边界
- 本轮执行 Phase 3.26：只做 scoped inactive-equivalence verification 和证据包。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_26_scoped_inactive_equivalence_verification`。
- 不实现 objective，不修改 training loss，不改变权重更新语义，不新增训练/selection 路径，不生成阶段 `.bin`，不触碰 gate/rating/scorecard/perf threshold。
- 未触碰 `tools\*`、`scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 核心发现
- Phase 3.25 的 JSON report/manifest 明确为 default-off / no-op skeleton，`objective_formula_implemented=false`、`training_loss_modified=false`。
- 用户列出的 Phase 3.25 `guarded-implementation-skeleton-report.md` 在实际目录中不存在；目录内只有 `guarded-implementation-skeleton-report.json` 和 `guarded-implementation-skeleton-manifest.json`。
- 既有 selftest 已比较 default API 与 explicit default-off options 下的 weights、traces、current value、target value、td error、abs weight delta、changed weight count。
- Phase 3.26 只补一个明确 marker：`Phase 3.26 inactive equivalence default-off disabled path no-op marker`，避免验证靠旧字符串碰运气。
- `src\surakarta_search.cpp` source scan 未发现 `training_loss`、`loss +=`、`td_error = ... opening` 或 `weight ... opening ... penalty` 形态的 objective/loss 实现。

### Hash 决策
- `tools\*` 继续按 Phase 3.25 manifest 聚合 hash 记录为 `CC9462DAE45009755A41F7F684DCA61D639C05ECFB17C234A6F94F3731934BB5`，before/after 一致。
- `scripts\perf-runner.ps1` hash：`5652BFA038A232ABFEC1462D3B62D7A804F9F5998B68E2067507F4136376DB84`，unchanged。
- `scripts\benchmark-select.ps1` hash：`3612E4123EF6BE94E0ECEAA69F80D3E9BF511C7617651B422D480DB1AF09DB3A`，unchanged。
- `scripts\benchmark-train.ps1` hash：`B09D4D07002F23464BB4E2139BFCEAE2F5DC0DA326470E2D513F85023DCA0A82`，unchanged。

### 验证环境发现
- 用户给定的 Community CMake 绝对路径不存在，字面命令失败在 preflight。
- 本机 VS 安装路径由 `vswhere` 定位为 `D:\Dev\App\VS2022\VS2022`。
- 可用 CMake 为 `D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`。
- 裸 PowerShell 使用该 CMake 仍会因 MSVC include 环境未初始化失败；必须导入 `D:\Dev\App\VS2022\VS2022\Common7\Tools\VsDevCmd.bat` 后构建。这个才是 Phase 3.26 的本机可用硬编码构建入口。

## 2026-05-24 Phase 3.25 guarded implementation skeleton

### 本轮边界
- 本轮执行 Phase 3.25：按 Phase 3.24 dry-run plan 建立 guarded implementation skeleton。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_25_guarded_implementation_skeleton`。
- 只允许 default-off / no-op skeleton；未实现 objective 公式，未修改 training loss，未改变权重更新语义。
- 未触碰 `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`。
- Phase 3 still not passed；Phase 4 still blocked。

### 实现决策
- `TrainingOptions` 新增 opening-safe skeleton 字段，全部默认关闭、零值或空值。
- `ApplyTrainingStep` 接收 default-off options 参数，但 disabled path 只构造 inactive skeleton state，不参与 TD target、delta、trace 或 weight update。
- `TrainingSummary` / checkpoint summary 增加 report-only inactive metadata，默认 `opening_drift_penalty_active=false`、`opening_drift_penalty_value=0`、`inactive_path_equivalent=true`。
- CLI 默认路径不启用 skeleton，只打印 inactive metadata；未新增任何默认开启开关。
- selftest 覆盖 default-off 字段和 inactive equivalence，比较 weights、traces、current/target value、td error、weight delta、changed weight count。

### 验证结论
- Release 构建和 `surakarta-bitboard-selftest.exe` 通过。
- default-off 检查通过：header 有 disabled defaults；source 未出现 opening objective loss；main 默认路径未启用；selftest 含 default off / inactive equivalence / opening safe。
- JSON report/manifest 可解析。
- Phase 3.25 输出目录 `.bin` 数量为 0。
- 本阶段只是把安全插座装上，电还没接；谁现在说 Phase 3 过了，那属于拿空壳当发动机吹。

## 2026-05-24 Phase 3.24 guarded implementation dry-run plan

### 本轮边界
- 本轮执行 Phase 3.24：基于 Phase 3.23 implementation plan review，产出 future guarded implementation dry-run 的详细计划。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_24_guarded_implementation_dry_run_plan`。
- 生成产物：`guarded-implementation-dry-run-plan.md`、`guarded-implementation-dry-run-plan.json`、`guarded-implementation-dry-run-plan-manifest.json`。
- 只做 design-only / dry-run-plan-only / report-only；不实现 objective，不修改 training loss，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不改 gate。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 计划决策
- Phase 3.24 结论为 `allow_phase3_25_guarded_implementation_skeleton=true`。
- 该允许只针对 guarded implementation skeleton，不授权训练、不授权 selection、不授权 `.bin`、不授权 gate 修改、不授权 Phase 3 passed。
- future skeleton 必须按最小任务拆分推进：preflight hash snapshot、default-off skeleton tests、default-off options、inactive equivalence fixtures、inactive no-op hook、scoped activation fixture、report-only diagnostics、hard reject fixtures、dry-run evidence package。
- exact allowed future touch list 限定为 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`、必要时 `scripts\workflow.tests.ps1`、Phase 3.25 输出目录和四个项目文档。
- 禁止继续覆盖 `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard/perf threshold、tuple/feature expansion、搜索参数、`.bin` 路径和 Phase 4。
- before/after hash 核验必须逐步执行；dirty worktree 不是豁免理由。历史脏归历史脏，新增越界就是新增越界，别拿泥地当迷彩服。

### 验证与风险
- default-off verification matrix 覆盖配置默认值、CLI 默认值、脚本 hash、`ApplyTrainingStep`、`RunBitboardTraining`、诊断禁用、selection summary 和 manifest gate eligibility。
- inactive path equivalence matrix 覆盖训练 step、训练 loop、CLI、自博弈、eval/search、selection artifacts、report-only companion 和 forbidden paths。
- hard reject fixture 必须只由 report-only acceptance companion 消费，输出 `hard_reject_triggered` / `hard_reject_reasons`，不得进入 `overall_rating`、`strength_rating`、`perf_rating`、scorecard、perf gate 或 training loss。
- rollback 触发器包括禁改路径 hash 变化、默认开启、inactive equivalence 失败、disabled path 行为变化、gate/rating/scorecard 变化、companion 被 gate 消费、`.bin`/训练/selection/64 局出现、Phase 3/4 状态错误推进。
- 最高风险仍是 default-off 路径误改训练、hard reject 偷偷变 gate、final `20260423` side-effect 被忽略、baseline move 被硬锁、tuple shrinkage 抢跑，以及脏工作区掩盖越界 diff。

## 2026-05-24 Phase 3.23 opening-safe objective implementation plan review

### 本轮边界
- 本轮执行 Phase 3.23：基于 Phase 3.18 / 3.21 / 3.22 证据链，评审 future minimal opening-safe objective implementation plan。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_23_implementation_plan_review`。
- 生成产物：`opening-safe-objective-implementation-plan-review.md`、`opening-safe-objective-implementation-plan-review.json`、`opening-safe-objective-implementation-plan-review-manifest.json`。
- 只做 design-only / implementation-plan-only / report-only；不实现 objective，不修改 training loss，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不改 gate。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 评审决策
- Phase 3.23 允许进入 `Phase 3.24 guarded implementation dry-run plan`，字段为 `allow_phase3_24_guarded_implementation_dry_run_plan=true`。
- 该允许只针对 dry-run plan，不授权实现、不授权训练、不授权 selection、不授权 `.bin`、不授权 gate 修改。
- future implementation 最小改动面只允许后续获批时触及 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`，必要时 `scripts\workflow.tests.ps1`。
- `tools\*`、三个 benchmark/perf 脚本、selection gate/rating/scorecard、perf threshold、搜索参数、tuple descriptor / feature expansion、`.bin` promotion、Phase 4 全部继续禁止。
- default-off 是硬前提；inactive path equivalence 是任何 active fixture 之前的硬门槛。
- hard reject 条件必须进入 report-only acceptance companion，不能进入 selection gate。也就是说它是审计刹车，不是偷偷换裁判，整明白这点比多写十个字段都重要。
- report-only companion 可写 `hard_reject_triggered` / `hard_reject_reasons` / `selection_gate_eligible=false` / `gate_not_modified=true`，不得影响 `overall_rating`、`strength_rating`、`perf_rating`、seed rating、scorecard、perf gate 或 training loss。

### 风险结论
- 最高风险仍是 default-off 路径误改训练、hard reject 被偷偷消费成 gate、final `20260423` side-effect 被忽略、baseline move 被硬锁、tuple shrinkage 抢跑。
- rollback 标准必须以 before/after hash、inactive equivalence、gate/rating/scorecard 零修改和 hard reject report-only 表达为主。
- Phase 3.24 若推进，第一件事不是写 loss，而是把 dry-run plan 的测试顺序、文件所有权和停止条件钉死。否则就是计划评审刚过就想拧训练发动机，手挺快，容易把锅盖拧飞。

## 2026-05-24 Phase 3.22 opening-safe objective minimum prototype design

### 本轮边界
- 本轮执行 Phase 3.22：基于 Phase 3.18 / 3.20 / 3.21 证据链，设计最小 opening-safe objective prototype。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_22_objective_minimum_prototype_design`。
- 生成产物：`opening-safe-objective-minimum-prototype-design.md`、`opening-safe-objective-minimum-prototype-design.json`、`opening-safe-objective-minimum-prototype-manifest.json`。
- 只做 design-only；不实现 objective，不修改 training loss，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不改 gate。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 设计决策
- Phase 3.22 允许进入 `Phase 3.23 implementation plan review`，但不允许直接 implementation。
- 最小 objective 是 default-off / opening-only / soft penalty 设计，目标是 unsafe baseline root-order degradation 与 costly PV migration。
- objective 非目标包括：不硬锁 baseline best move、不作为 selection gate、不承诺修 final、不改搜索参数、不生成 candidate `.bin`、不替代 32/64 selection。
- 输入依赖必须只读：selection summary、weights manifest、opening diagnostics、opening guard、root-cost companion 与相关 manifest。
- default-off 是硬边界；默认配置不能改变 TD target、自博弈、eval/search、manifest、selection policy 或脚本默认值。
- inactive path 等价验证是进入任何实现前的硬门槛：`ApplyTrainingStep`、`RunBitboardTraining`、CLI defaults、脚本默认值、selection summary 均不得在关闭时产生行为变化。
- opening drift penalty 只能惩罚关系变化：baseline rank 变差 + drift child 成为 rank 1 / cost-dominant + root-cost 迁移；不能奖励某个固定 baseline move。
- final `20260423` side-effect / root over-neutralization 是 hard reject，不是 watch，不是“后面再说”。
- candidate acceptance guard companion 必须继续是 report-only wrapper；root-cost companion 必须继续旁路诊断，不进 gate / loss。
- distributed tuple shrinkage 仍不能先做，因为无 single tuple dominance，且 neutralization 已暴露 final side-effect；只能在 opening drift penalty 之后再评审 conservative core / C2-focused stable 小集合。

### 未来最小改动面
- 若后续实现获批，最小可能触及 `include\surakarta_search.h`、`src\surakarta_search.cpp`、`src\main.cpp`、`src\surakarta_bitboard_selftest.cpp`，以及必要时 `scripts\workflow.tests.ps1`。
- 禁止面仍包括 `tools\`、三个 benchmark/perf 脚本、selection gate/rating/scorecard、perf threshold、搜索参数、tuple feature expansion、`.bin` promotion、Phase 4 特征扩展。

## 2026-05-24 Phase 3.21 opening-safe objective readiness review

### 本轮边界
- 本轮执行 Phase 3.21：基于 Phase 3.1-3.20 证据链评审是否具备开始设计最小 opening-safe training objective prototype 的条件。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_21_objective_readiness_review`。
- 生成产物：`objective-readiness-review.md`、`objective-readiness-review.json`、`objective-readiness-review-manifest.json`。
- 只做 readiness review / design gate；不修改 training loss，不实现 objective，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不改 gate。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 结论
- Phase 3.21 readiness 结论为 `conditionally_ready`。
- 允许进入的下一步仅是 **minimum opening-safe objective prototype design task**，不是 implementation。
- Phase 3.20 root-cost companion 足以支持 readiness review：cp2 root-cost 12/12 explain，441 个 artifact zero-mutation，且 final mixed 风险被机器可读地暴露。
- Phase 3.20 不足以支持直接改 loss：final 仍有 8 watch / 4 regress，且所有 variant 都有 `20260423` final regress component。

### 对 objective 的含义
- `cp2 strong explanation` 表明 future objective 应围绕 unsafe opening root-order drift / costly PV migration 做软约束，而不是“训练必须下 baseline best move”。
- `final partial explanation` 是风险约束：`20260423` final side-effect / non-baseline new best / qnodes increase 必须写入未来 rejection criteria。cp2 能解释，不等于 final 治好了；这锅要是混着炖，报告看着香，实际容易糊。
- 推荐实施顺序仍成立：candidate acceptance guard companion -> root-cost diagnostic companion -> opening drift penalty -> distributed tuple shrinkage。
- candidate acceptance guard companion 必须先作为 report-only wrapper 接入；root-cost companion 继续只做旁路诊断，不进入 selection gate。
- opening drift penalty 最小边界是 default-off、soft penalty、只惩罚 unsafe rank degradation / costly drift、不硬锁 baseline best move。
- distributed tuple shrinkage 仍应排在 opening drift penalty 之后，且后续只考虑 conservative core / C2-focused stable 小集合；禁止 all_negative 和 diagnostic-neutralized `.bin` candidate。

### 必须 blocked 的条件
- wrapper 或 root-cost companion 触碰 selection rating / scorecard / gate。
- final `20260423` side-effect 没有硬拒绝标准。
- default-off 和 inactive-path equivalence test 无法设计。
- 设计先做 tuple shrinkage、all_negative、或把 neutralized diagnostic 权重升格 candidate。
- 任何步骤要求训练、selection、`GamesPerSeed=64`、`.bin`、gate threshold change 或 Phase 4 entry。

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

### 本轮边界
- 本轮执行 Phase 3.19：验证 Phase 3.16 candidate acceptance guard runner 作为 selection 后旁路报告运行时，不会修改 selection summary、rating、scorecard 或任何原始 training/selection artifact。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation`。
- 生成产物：`candidate-acceptance-guard-summary.md/json`、`candidate-acceptance-guard-manifest.json`、`guard-zero-mutation-report.md/json`、`guard-zero-mutation-manifest.json`。
- 只做 wrapper dry-run 和验证报告；不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不修改 gate，不进入 Phase 4。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- Phase 3 still not passed；Phase 4 still blocked。

### 验证输入与命令
- Run root：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- Phase 3.13 guard：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`。
- wrapper command：

```powershell
pwsh -NoProfile -ExecutionPolicy Bypass -File "D:\Dev\Code\surakarta_traditional\tools\candidate-acceptance-guard-runner.ps1" -RunRoot "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134" -GuardOutputDir "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard" -RunnerOutputDir "D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_19_guard_zero_mutation" -OpeningGuardScript "D:\Dev\Code\surakarta_traditional\tools\opening-guard-diagnostics.ps1" -Force
```

### Zero-mutation 结论
- 已检查 artifact 覆盖 selection summary / rating summary、run root 与 per-seed `weights-manifest.json`、`bootstrap.bin`、`candidate.bin`、`checkpoints\checkpoint-*.bin` 及其 manifest、Phase 3.13 guard 原始产物、Phase 3.16 runner 原始产物。
- `checked_artifact_count=126`。
- `changed_artifact_count=0`。
- `unchanged_artifact_count=126`。
- `changed_artifacts=[]`。
- `selection_result_unchanged=true`。
- `gate_not_modified=true`。
- wrapper 只新增/覆盖 Phase 3.19 输出目录内的 report-only companion artifacts。
- `report_only=true`、`production_candidate=false`、`selection_gate_eligible=false`、`phase3_status=not_passed`、`phase4_status=blocked`。

### 决策
- Phase 3.16 wrapper 可以作为 selection 后旁路报告 dry-run 使用；本轮证据只证明零修改，不证明 candidate 可接受。
- Phase 3 仍未通过，不能把 guard report 写成 gate pass。
- Phase 4 仍 blocked，不能扩 N-Tuple / threat / mobility / stage 特征。
- Phase 3.20 建议做 root-cost diagnostic companion dry-run，继续验证旁路报告零修改；训练 loss 改造继续暂缓。说白了，这次只是证明旁路没把厨房炸了，不代表菜已经能上国宴。

## 2026-05-24 Phase 3.18 opening-safe objective pre-implementation plan

### 本轮边界
- 本轮执行 Phase 3.18：基于 Phase 3.1-3.17 证据包，制定 opening-safe training objective / candidate acceptance companion / root-cost diagnostic 的未来最小实施计划。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_18_objective_preimplementation_plan`。
- 生成产物：`opening-safe-objective-preimplementation-plan.md`、`opening-safe-objective-preimplementation-plan.json`、`opening-safe-objective-preimplementation-manifest.json`。
- 只做 pre-implementation plan，不实现代码，不修改训练逻辑，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不进入 Phase 4。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未修改 gate 阈值，未把 opening guard、runner、evidence package 或本计划写成 selection gate。
- Phase 3 still not passed；Phase 4 still blocked。

### 计划输入
- Phase 3.17 evidence package：`phase3-evidence-package.md/json/manifest.json`。
- Phase 3.13 opening guard summary：cp2 `explain=6/watch=6/regress=0/unknown=0`，final `explain=0/watch=8/regress=4/unknown=0`。
- Phase 3.16 guard runner summary/manifest：`selection_result_unchanged=true`、`gate_not_modified=true`、`report_only=true`、`selection_gate_eligible=false`。
- 三个脚本接口只读复核：`benchmark-train.ps1` 负责训练 smoke/repro；`benchmark-select.ps1` 的 gate 在 `Get-SelectionScorecard` / aggregate rating 路径；`perf-runner.ps1` 是独立 perf gate。
- 训练实现只读定位：`TrainingOptions` / `ApplyTrainingStep` / `RunBitboardTraining` 位于 `include\surakarta_search.h` 与 `src\surakarta_search.cpp`，CLI 参数位于 `src\main.cpp`，相关自测在 `src\surakarta_bitboard_selftest.cpp`。

### 推荐实施顺序与结论
1. **Candidate acceptance guard companion integration**：优先级最高。未来目标是让 Phase 3.16 wrapper 成为 selection 后旁路报告；不得改变 `overall_rating`、`strength_rating`、`perf_rating`、seed rating 或 scorecard。
2. **Root-cost diagnostic acceptance companion**：第二优先级。未来目标是把 `nodes_ratio`、`qnodes_ratio`、`qnodes_per_node_ratio`、`aspiration_retry_delta` 作为训练后诊断证据；优先 report-only，不进入 loss。
3. **Opening drift penalty prototype**：第三优先级。未来若实施，只能默认关闭，轻量约束 baseline rank 降级；不能硬锁 baseline best move。
4. **Distributed tuple delta clipping / shrinkage prototype**：最高风险、最后优先级。未来只允许 conservative core / C2-focused stable 小集合；不得 all_negative neutralization，不得把 Phase 3.9 diagnostic-neutralized `.bin` 当 candidate。

### 风险判断
- companion integration 的主要风险是被误读或误接成 selection gate；停止条件是 `selection-summary.json` 哈希变化或 rating/scorecard 被写入。
- root-cost diagnostic 的主要风险是把 noisy search timing 或 root-cost 指标直接塞进 loss / gate；停止条件是任何 root-cost status 被 `Get-SelectionScorecard` 消费。
- opening drift penalty 的主要风险是过拟合单个 opening case、冻结 baseline move、压制真实强着；停止条件是非 opening 固定局面 drift、fixed score regression 或 final side-effect 复现。
- tuple clipping/shrinkage 的主要风险最高：可能削掉真实有用评估信号并制造 root over-neutralization；停止条件是 final 任一 seed 出现 non-baseline new best 且 qnodes 上升，或 C/B/D、score、performance 任一恶化。

### Phase 3.19 建议
- 推荐优先实现或进一步计划 **report-only companion integration 的最小 wrapper 接入 dry-run**，核心证明是 selection 后运行 wrapper 不改变 `selection-summary.json` 哈希、不改变任何 rating/scorecard。
- 暂缓 opening drift penalty / tuple clipping 的实际代码改造。这个阶段要先把旁路证据管道焊牢，别上来就改训练 loss，容易把锅从诊断层直接端进发动机里。

## 2026-05-24 Phase 3.17 report-only evidence package consolidation

### 本轮边界
- 本轮执行 Phase 3.17：整理 Phase 3.1-3.16 的关键证据链，生成可审计 evidence package。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_17_evidence_package`。
- 生成产物：`phase3-evidence-package.md`、`phase3-evidence-package.json`、`phase3-evidence-package-manifest.json`。
- 只做 report-only 汇总，不新增诊断逻辑，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`，不进入 Phase 4。
- 未修改 `src\` / `include\`。
- 未修改 `tools\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未修改 gate 阈值，未把 report-only guard 或 evidence package 写成 selection gate。
- Phase 3 still not passed；Phase 4 still blocked。

### Evidence package 输入与追溯
- 输入覆盖四个项目记忆/路线图文件：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 输入覆盖 Phase 3.1 selection summary、Phase 3.3 opening diagnostics、Phase 3.5 attribution、Phase 3.7 static ablation、Phase 3.9 offline neutralization、Phase 3.13 opening guard、Phase 3.16 candidate acceptance guard runner。
- manifest 记录所有输入文件 path / SHA256 / length / lines，记录两个主要输出文件 path / SHA256，并按既有 Phase 3.16 manifest 约定对 self manifest hash 使用生成后外部校验说明，避免自引用 stale hash。
- 当前 git commit：`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。

### 汇总结论
- Phase 3.1 selection result 仍为 `regress`：`overall_rating=strength_rating=perf_rating=regress`，scorecard `pass/watch/regress=0/0/3`，C/B/D=`12/16/2`，`score_regressions=1`，`best_move_changes=3`，平均/最差性能比 `1.622708/1.864728`。
- Phase 3.2-3.3 opening 是唯一硬 fixed-position blocker；cp2 first drift 三 seed 同型为 `(5, 1) -> (4, 2) (B)`，baseline `(0, 1) -> (1, 2) (B)` 退到 root order rank 2；cp2 无 aspiration retry。
- Phase 3.5 no single tuple dominance：dominance `yes/watch/no=0/0/36`，active tuple count `45`，tuple set hash `0x1D4A6EBF2BAF6484`，weight count `16605`。
- Phase 3.7 distributed tuple deltas 可静态削弱 drift preference：`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`，K5/K10 在 `3/3` seed 成立，K1/K3 不足。
- Phase 3.9 cp2 强解释：四个 variant 在 `3/3` seed 将 best move 拉回 baseline，baseline root order `2->1`，nodes/qnodes 下降。final 只能 partial explanation：`20260425` 成本下降，但 `20260423` 出现 root over-neutralization / non-baseline new best move / qnodes 上升。
- Phase 3.13 guard 计数：cp2 `explain=6/watch=6/regress=0/unknown=0`；final `explain=0/watch=8/regress=4/unknown=0`；四个 variant 聚合仍均为 `regress`。
- Phase 3.16 wrapper 保持 `selection_result_unchanged=true`、`gate_not_modified=true`，并声明 `report_only=true`、`production_candidate=false`、`selection_gate_eligible=false`。

### 决策
- Phase 3 仍未通过，不能跑 `GamesPerSeed=64` 去硬凑热闹。
- Phase 4 仍 blocked，不能扩 N-Tuple / threat / mobility / stage 特征。
- 下一步建议进入 Phase 3.18：opening-safe objective pre-implementation plan。这个阶段仍应先写计划，明确 objective 候选、输入证据、验证夹具、失败模式和验收标准；不直接改训练 loss。现在就改 loss，那叫诊断单还没看完就开药，挺豪横，容易翻车。

## 2026-05-24 Phase 3.16 report-only candidate acceptance guard runner/wrapper

### 本轮边界
- 本轮实现 Phase 3.16：新增非生产 wrapper `tools\candidate-acceptance-guard-runner.ps1`。
- wrapper 只读取既有 selection summary / scorecard artifact、既有 Phase 3.13 opening guard 输出和 `tools\opening-guard-diagnostics.ps1`。
- 本次复用 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`，未覆盖 Phase 3.13 原始产物。
- 只生成 Phase 3.16 runner 自己的 report-only 产物；不修改 `selection-summary.json`、`selection-summary.txt`、scorecard、`overall_rating`、`strength_rating`、`perf_rating`。
- 未修改 `src\` / `include\`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未改 gate。
- Phase 3 still not passed；Phase 4 still blocked。

### 输入与输出
- Run root：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- Guard output：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`。
- Runner output：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_16_guard_runner`。
- 生成产物：`candidate-acceptance-guard-summary.md`、`candidate-acceptance-guard-summary.json`、`candidate-acceptance-guard-manifest.json`。

### Wrapper 行为
- 校验 `RunRoot` 存在。
- 校验 `tools\opening-guard-diagnostics.ps1` 存在。
- 发现并读取实际 selection artifact：`selection-summary.json` / `selection-summary.txt`；scorecard 位于 `selection-summary.json`。
- 若 `GuardOutputDir` 已有 `opening-guard-summary.json` 与 `opening-guard-manifest.json`，复用既有 guard 产物。
- 若 `GuardOutputDir` 缺少必需 guard 文件且目录非空，直接停止，避免覆盖 Phase 3.13 原始产物。
- `-Force` 只允许覆盖 Phase 3.16 runner 自己的 summary/manifest 文件，不允许覆盖 guard 原始产物。

### 结果摘要
| stage | explain | watch | regress | unknown |
|---|---:|---:|---:|---:|
| cp2 | 6 | 6 | 0 | 0 |
| final | 0 | 8 | 4 | 0 |

| variant | guard_status | explain | watch | regress | unknown |
|---|---|---:|---:|---:|---:|
| c1_only_k10 | regress | 0 | 5 | 1 | 0 |
| c2_focused_stable_k10 | regress | 3 | 2 | 1 | 0 |
| conservative_core | regress | 3 | 2 | 1 | 0 |
| union_k10_riskier | regress | 0 | 5 | 1 | 0 |

### 结论
- Phase 3.16 只是 candidate acceptance 的 report-only companion，不是 selection gate。
- `selection_result_unchanged=true`，`gate_not_modified=true`。
- `report_only=true`、`selection_gate_eligible=false`、`production_candidate=false` 已写入 summary 和 manifest。
- cp2 仍可强解释；final 仍只是 partial explanation，且存在 side-effect regress 风险。
- 四个 variant 聚合均为 `regress`，不能被包装成 candidate pass。想拿这个当放行条，那就是把体检报告贴门口当门票，挺能糊弄但不管用。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-24 Phase 3.15-A candidate acceptance guard integration report-only hook/spec

### 本轮边界
- 本轮只做 Phase 3.15-A hook/spec 文档设计，不实现 hook，不改训练，不改 selection，不改 gate。
- 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 只读 `tools\opening-guard-diagnostics.ps1` 与 `scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`、`scripts\perf-runner.ps1` 的接口；未修改它们。
- 未修改 `src\` / `include\`，未修改 `tools\`，未修改三个 benchmark/perf 脚本。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`。
- Phase 3 still not passed；Phase 4 still blocked。

### Hook 位置
- 首选位置：selection 已完成 candidate 生成、candidate eval、checkpoint eval、scorecard 聚合，并写出 `selection-summary.json` / `selection-summary.txt` 之后，由外层 report-only wrapper 或 post-selection runner 启动 opening guard。
- 备选位置：最终 summary 汇总阶段旁路生成 `candidate-acceptance-guard-summary.md`，只追加 companion artifact，不覆盖原 summary。
- 禁止位置：不得插入 `Get-SelectionScorecard`、`Get-SelectionAggregateRating`、`overall_rating`、`strength_rating`、`perf_rating`、seed rating 或 scorecard `pass/watch/regress` 计算路径。这个东西要是插进 gate 里，就从体检报告变成拦路杆了，性质完全变味。

### 输入 contract
| 输入 | 说明 |
|---|---|
| `RunRoot` | selection session root，例如 `build-local\training\20260522-172134`。 |
| `weights-manifest.json` | 只读；解析 seed、artifact role、checkpoint/candidate、path/hash、commit。 |
| opening diagnostics root | 只读；通常是 `<RunRoot>\opening-diagnostics`，提供 Phase 3.3 / 3.7 / 3.9 / 3.13 guard 输入或输出目录。 |
| Phase 3.13 opening guard script | `tools\opening-guard-diagnostics.ps1`；未来 wrapper 只能调用它或等价 report-only runner。 |
| selection summary / scorecard artifact | `selection-summary.json` / `selection-summary.txt`；只读引用主 selection 结果。 |

最小命令示例：

```powershell
pwsh -NoProfile -ExecutionPolicy Bypass -File .\tools\opening-guard-diagnostics.ps1 -RunRoot <run-root> -OutputDir <guard-output-dir>
```

### 输出 contract
- 必需：`opening-guard-metrics.csv`。
- 必需：`opening-guard-summary.json`。
- 必需：`opening-guard-manifest.json`。
- 可选：`candidate-acceptance-guard-summary.md`，用于并列展示主 selection result 和 guard evidence。
- 推荐目录：`<RunRoot>\opening-diagnostics\candidate_acceptance_guard`；若按阶段执行，可用 `<RunRoot>\opening-diagnostics\phase3_16_candidate_acceptance_guard`。
- 输出不得覆盖 `selection-summary.json`、`selection-summary.txt`、`weights-manifest.json` 或任何 `.bin`。

### 状态语义
- `explain`：现象可由 Phase 3.3 / 3.7 / 3.9 / 3.13 证据解释；不表示 candidate pass。
- `watch`：存在风险或证据不足；不改变主 selection 结果。
- `regress`：guard 观察到 report-only 风险，尤其 final side-effect；只能写 risk note，不得自动 reject candidate。
- `unknown`：artifact 缺失、manifest 不合规或字段不可得；不能默认放行，也不能覆盖 selection result。
- 这些状态不改变 `overall_rating`、`strength_rating`、`perf_rating`、seed scorecard rating、scorecard `pass/watch/regress` 计数。

### Failure behavior
| 场景 | 行为 |
|---|---|
| guard artifact 缺失 | 主 selection result 不被覆盖；companion 写 `guard_status=unknown` / `guard_execution_status=missing_artifact`。 |
| guard 脚本失败 | 主 selection 不应被 retroactively 改写；wrapper 只记录失败、退出码、stderr/stdout path。 |
| manifest 不合规 | guard report 标记 invalid，不能作为证据；selection summary 保持原样。 |
| final `regress` | 写入 risk note 和 guard summary；不得自动 reject candidate，不得改 scorecard。 |
| selection summary 缺失 | guard companion 可标 `unknown`，但不得伪造 selection result。 |

### Traceability
- `opening-guard-manifest.json` 或未来 wrapper manifest 必须记录：commit、input hash、output hash、script path、command line、`generated_at`、`RunRoot`、selection summary hash、weights manifest hash、guard script hash。
- 必须保留并显式声明：`production_candidate=false`、`selection_gate_eligible=false`、`report_only=true`、`phase3_status=not_passed`、`phase4_status=blocked`。
- `candidate-acceptance-guard-summary.md` 若存在，只能引用主 selection result，不能派生新的 pass/watch/regress gate。

### Phase 3.16 建议
- 推荐：实现 report-only hook runner/wrapper，在 selection 后生成附加 opening guard report，并校验 manifest/traceability。
- 暂缓：训练 loss/objective 改造。
- 理由：先把旁路 evidence chain 固定住，再谈 objective；要不然就是诊断还没挂牌，手已经伸进发动机里了。

## 2026-05-24 Phase 3.14 design-only opening-safe training objective minimal design

### 本轮边界
- 本轮只做 Phase 3.14 design-only 文档更新：设计 opening-safe training objective 的最小改造方案。
- 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 未修改 `src\` / `include\`，未修改 `tools\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未生成 `.bin`，未修改 gate 阈值，未进入 Phase 4。
- opening guard 仍是 report-only evidence companion，不是 selection gate，不替代现有 pass/watch/regress gate。
- Phase 3 still not passed；Phase 4 still blocked。

### 输入证据
- Phase 3.13-B `opening-guard-metrics.csv`：`24` 行，覆盖 3 seed × cp2/final × 4 neutralization variant。
- Phase 3.13-B `opening-guard-summary.json`：cp2 `explain=6/watch=6/regress=0/unknown=0`；final `explain=0/watch=8/regress=4/unknown=0`；`cp2_strong_explanation_verified=true`；`final_partial_explanation_verified=true`。
- Phase 3.13-B `opening-guard-manifest.json`：声明 `production_candidate=false`、`selection_gate_eligible=false`、`report_only=true`、`phase3_status=not_passed`、`phase4_status=blocked`。
- Phase 3.9 neutralization summary/root diagnostics：cp2 四个 variant 都能把 first drift 拉回 baseline 并降低 root cost；final `20260423` 出现 root over-neutralization / non-baseline new best move，final 只能 partial explanation。
- Phase 3.7 static ablation summary/sweep：`C1_cp2_onset` 与 `C2_final_gate_blocker` 都是 distributed tuple delta，不是 top1/top3 单点问题；K5/K10 跨 seed 有稳定削弱，controls pass。

### 候选方案与推荐排序

| 排序 | 方案 | 目标 | 输入证据 | 改造点 | 最小实现范围 | 风险 | 如何验证 | 为什么不是 Phase 4 | 是否需要生产代码变更 |
|---:|---|---|---|---|---|---|---|---|---|
| 1 | candidate acceptance guard integration | 把 Phase 3.13 opening guard 接入 candidate acceptance 报告旁路，让每个候选权重带 opening drift/root-order/root-cost/static/final side-effect 证据。 | Phase 3.13 guard metrics/summary/manifest；Phase 3.1 selection summary；Phase 3.3/3.7/3.9 事实源。 | 验收报告层 hook/spec，输出 report-only guard companion；不改变 selection gate。 | 定义 report-only acceptance companion 字段、manifest 声明、命令约束和 CI/本地报告位置。 | 最大风险是被误读成新 gate 或绕过现有 gate。 | 确认 guard 输出不写 `overall_rating` / `strength_rating` / `perf_rating`，不改变 pass/watch/regress gate，且 manifest 继续声明 `selection_gate_eligible=false`。 | 它只提高 Phase 3 归因和候选审计质量，不新增棋力特征。 | 短期不需要；若接进现有 pipeline 可能只需要脚本/报告层变更。 |
| 2 | root-cost regularization as diagnostic | 约束 opening root-order inflation 带来的 nodes/qnodes/qnodes_per_node 膨胀。 | Phase 3.13 `nodes_ratio`、`qnodes_ratio`、`qnodes_per_node_ratio`、`aspiration_retry_delta`；Phase 3.3/3.9 root diagnostics。 | 作为 training acceptance diagnostic，不建议直接进 loss。 | 定义 cost watch/regress 口径、趋势表和 candidate 后验收要求。 | 搜索成本受 move ordering / bound / retry 影响，直接进 loss 容易把噪声当目标。 | guard 复跑时 cp2/final cost signal 与 best-move/root-order 证据一致；非 opening cases 不被牵连。 | 它仍是 Phase 3 训练稳定性验收，不是新特征扩展。 | 短期不需要生产代码变更；未来接 pipeline 也优先诊断/脚本层。 |
| 3 | opening drift penalty | 对已知 opening root case 的 baseline rank 降级施加轻量惩罚，同时允许可解释的新 best move。 | Phase 3.13 rank/status；Phase 3.7 distributed tuple support；Phase 3.9 cp2 强解释与 final side-effect。 | 训练 objective 或候选评分中增加轻量 rank degradation penalty。 | 只覆盖 opening guard cases；惩罚“不安全 rank degradation / costly PV drift”，不奖励固定 baseline move。 | 过拟合 opening case；把 baseline move 固化成教条；误伤真实改进。 | cp2 drift 降低，final 不产生 non-baseline new best side-effect，non-opening fixed positions 不退化。 | 它仍是在修 Phase 3 opening blocker，不是 Phase 4 特征扩张。 | 未来若真正进入 objective，需要生产训练代码变更。 |
| 4 | distributed tuple delta clipping / shrinkage | 对 Phase 3.7/3.9 证明的 distributed drift tuple set 做最小 clipping/shrinkage。 | K5/K10 stable tuple support；conservative_core / c2_focused_stable_k10 风险低于 c1_only_k10 / union；final `20260423` side-effect。 | 训练权重更新后的局部 shrinkage 或 objective-local clipping。 | 只允许 conservative core / C2-focused stable 小集合实验设计；禁止 all_negative neutralization。 | 风险最高，可能削弱真实棋力或制造 root over-neutralization。 | static support 仍成立，cp2 baseline rank 恢复，final `20260423` 不再漂到 non-baseline new best。 | 它仍是 Phase 3 opening drift 修复实验，不能解锁 Phase 4。 | 未来需要生产训练/权重更新代码变更。 |

### 不推荐路线
- 不直接 all_negative neutralization：Phase 3.7 已证明是 distributed tuple delta，但 all_negative 太粗，会把解释性诊断变成乱削权重。
- 不直接扩 N-Tuple / threat / mobility / stage：Phase 3 opening blocker 没过，扩特征就是把地基裂缝上面继续盖楼。
- 不直接调 gate 阈值：gate 是防线，不是装饰。
- 不直接跑 `GamesPerSeed=64`：Phase 3.1 已在 32 局 `regress`，停止条件已经触发。
- 不直接把 neutralized 权重当 candidate：Phase 3.9/3.13 已发现 final side-effect，特别是 `20260423` root over-neutralization。

### Phase 3.15 建议
- 推荐 A：先实现 candidate acceptance guard integration 的 report-only hook/spec。
- 暂缓 B：直接改训练 loss。
- 理由：Phase 3.13-B 已经有可复跑 opening guard 报告，先把它作为 candidate acceptance companion 固定下来，能稳定证据链；直接改 loss 会把归因、训练和 selection 搅在一起，容易越改越玄乎。

## 2026-05-24 Phase 3.13-B report-only opening guard diagnostics

### 本轮边界
- 本轮实现 Phase 3.13-B：新增非生产诊断脚本 `tools\opening-guard-diagnostics.ps1`。
- 只读取既有 Phase 3.3 / 3.7 / 3.9 产物并生成 report-only opening guard 报告；不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`。
- 只允许新增脚本、生成 `phase3_13_opening_guard` 输出目录、更新四个文档；未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- guard 状态只使用 `explain` / `watch` / `regress` / `unknown`；这些状态只能用于 Phase 3 归因，不是 selection gate，不改变 `overall_rating` / `strength_rating` / `perf_rating`。
- Phase 3 still not passed；Phase 4 still blocked。

### 输入与输出
- 输入文档：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 输入 artifact：`weights-manifest.json`、Phase 3.3 `opening-diagnostics-summary.md` / `opening-diagnostics-summary.csv`、Phase 3.7 `opening-child-static-ablation-summary.csv` / `opening-child-static-ablation-sweep.csv`、Phase 3.9 `neutralization-manifest.json` / `neutralization-summary.csv` / `neutralization-root-diagnostics.csv`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard`。
- 输出产物：`opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。
- manifest 记录输入/output SHA256、commit、脚本路径、执行命令、`generated_at`，并声明 `production_candidate=false`、`selection_gate_eligible=false`、`report_only=true`、`phase3_status=not_passed`、`phase4_status=blocked`。

### metrics schema
- 必需字段已落地：`seed`、`stage`、`artifact_role`、`variant`、`baseline_best_move`、`candidate_best_move`、`best_move_status`、`baseline_rank`、`candidate_rank`、`root_order_status`、`nodes_ratio`、`qnodes_ratio`、`qnodes_per_node_ratio`、`aspiration_retry_delta`、`static_support_status`、`over_neutralization_status`、`guard_status`、`explanation_scope`。
- 额外保留审计字段：`source_best_move`、`original_baseline_rank`、`source_nodes`、`candidate_nodes`、`source_qnodes`、`candidate_qnodes`、`root_cost_status`、`static_over_neutralization`、`root_over_neutralization`、`original_diagnostic_best_move`、`original_diagnostic_nodes`、`original_diagnostic_qnodes`。

### 结果摘要
| stage | explain | watch | regress | unknown |
|---|---:|---:|---:|---:|
| cp2 | 6 | 6 | 0 | 0 |
| final | 0 | 8 | 4 | 0 |

| variant | guard_status | explain | watch | regress | unknown |
|---|---|---:|---:|---:|---:|
| conservative_core | regress | 3 | 2 | 1 | 0 |
| c2_focused_stable_k10 | regress | 3 | 2 | 1 | 0 |
| c1_only_k10 | regress | 0 | 5 | 1 | 0 |
| union_k10_riskier | regress | 0 | 5 | 1 | 0 |

### 诊断结论
- cp2 强解释成立：conservative_core 与 c2_focused_stable_k10 在 3/3 seed 均把 best move 拉回 baseline，baseline rank 回到 1，nodes/qnodes 降低，`cp2_strong_explanation_verified=true`。
- c1_only_k10 与 union_k10_riskier 在 cp2 root 层也能拉回 baseline，但因 static over-neutralization 标为 `watch`；这不是修复失败，是提醒它们风险更大，别拿来当默认生产候选。
- final 只能 partial explanation：`20260425` 有 root-cost 改善，`20260424` 主要是 watch；`20260423` 四个 variant 均出现 non-baseline new best move / root over-neutralization，final `regress=4`。
- 每个 variant 的聚合 `guard_status=regress` 是因为 final `20260423` 副作用按最高风险聚合；这不等于 selection regress 的替代判定，更不能拿来放行或封杀 candidate。

### 下一步建议
- 建议 Phase 3.14 不进入 Phase 4，继续在 Phase 3 内处理：要么设计最小 opening-safe training objective，要么先补 opening guard 脚本的字段/fixture 验证。
- 当前更务实的是先补 guard 脚本验证夹具或固定输入断言，因为脚本刚落地，先把报告稳定性钉住，再动训练目标。不然又是锅还没洗就开始炖，味儿肯定串。

## 2026-05-23 Phase 3.12 design-to-diagnostic bridge

### 本轮边界
- 本轮只做 Phase 3.12 report-only diagnostic spec 文档设计，不写代码，不新增诊断脚本，不训练，不跑 selection，不跑 `GamesPerSeed=64`，不生成 `.bin`。
- 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 未修改 `src\` / `include\`，未修改 `tools\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 这个 guard 只能给 Phase 3 归因和下一步设计提供证据，不得替代 pass/watch/regress gate，不得改 selection gate 阈值，不得把 report-only 结果包装成 candidate acceptance 放行。想把体检表当通行证，那就是东北话讲的：净整那没用的。
- Phase 3 still not passed；Phase 4 still blocked。

### 最小输入 artifact
| artifact | 用途 |
|---|---|
| `build-local\training\20260522-172134\weights-manifest.json` | 解析 seed、stage、artifact role、权重 path/hash、tuple hash、weight count，确认 baseline/cp2/final 可追溯。 |
| `opening-diagnostics\opening-diagnostics-summary.md` 及对应 CSV | Phase 3.3 root best move、baseline/cp2/final drift move rank、nodes/qnodes、aspiration retry 的事实源。 |
| `phase3_7_static_ablation\opening-child-static-ablation-summary.csv` | Phase 3.7 distributed tuple support、C1/C2 是否 success、control 是否 pass 的事实源。 |
| `phase3_7_static_ablation\opening-child-static-ablation-sweep.csv` | K5/K10 weakening ratio、sign flip/tie、over-neutralization 风险的事实源。 |
| `phase3_9_offline_neutralization\neutralization-summary.csv` | Phase 3.9 original vs neutralized best move、root cost、side-effect 归因事实源。 |
| `phase3_9_offline_neutralization\neutralization-root-diagnostics.csv` | baseline rank 是否恢复、drift move 是否退位、nodes/qnodes/aspiration retry delta 的事实源。 |
| `phase3_9_offline_neutralization\neutralization-manifest.json` | 输入 hash、variant、production_candidate=false、selection_gate_eligible=false 等边界字段事实源。 |

### 建议输出产物
- `opening-guard-spec.md` 或路线图中的同名章节：记录字段定义、状态语义、watch/regress 解释口径和禁止事项。
- `opening-guard-metrics.csv`：逐 seed / stage / artifact_role 输出字段级 guard metrics。
- `opening-guard-summary.json`：聚合 seed-level 与 stage-level guard_status，不输出 selection pass/fail。
- `opening-guard-manifest.json`：记录输入文件 path/hash、git commit、diagnostic mode、`report_only=true`、`selection_gate_modified=false`、`training_executed=false`、`selection_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。

### 字段级输出草案
| 字段 | 说明 |
|---|---|
| `seed` | `20260423` / `20260424` / `20260425`。 |
| `stage` | `bootstrap` / `cp2` / `final`，未来可扩展 checkpoint 名称，但不得改 gate。 |
| `artifact_role` | `baseline` / `candidate` / `checkpoint` / `neutralized_diagnostic` / `control`。 |
| `baseline_best_move` | Phase 3.3 bootstrap opening best move，当前为 `(0, 1) -> (1, 2) (B)`。 |
| `candidate_best_move` | 当前 stage 或 diagnostic artifact 的 opening best move。 |
| `best_move_status` | `explain`=baseline 保持或 drift 可由既有证据解释；`watch`=非 baseline 但仍 tie/bound 或只 partial explanation；`regress`=non-baseline new best move 且不可解释或出现 side-effect；`unknown`=输入缺失。 |
| `baseline_rank` | baseline move 在 candidate/root diagnostics 中的 order rank。 |
| `candidate_rank` | candidate/drift move 的 order rank。 |
| `root_order_status` | `explain`=baseline-like 或 neutralization 后 baseline rank 恢复；`watch`=baseline 降级但 score-rank tie/bound；`regress`=drift move rank 1 且 baseline 降级伴随成本膨胀；`unknown`=rank 不可得。 |
| `nodes_ratio` | candidate nodes / comparable baseline nodes；用于 root-cost guard，不单独作为 selection gate。 |
| `qnodes_ratio` | candidate qnodes / comparable baseline qnodes。 |
| `aspiration_retry_delta` | candidate retries - comparable baseline retries。 |
| `static_support_status` | 来自 Phase 3.7：`explain`=C1/C2 K5/K10 distributed support 且 controls pass；`watch`=K5/K10 只部分支持；`regress`=controls fail 或只靠 all_negative；`unknown`=无 ablation 输入。 |
| `over_neutralization_status` | `explain`=未出现 root/static over-neutralization；`watch`=static over-neutralization 但 root 未新漂移；`regress`=root over-neutralization 或 non-baseline new best move；`unknown`=无 neutralization 输入。 |
| `guard_status` | 逐行最高风险聚合：`regress` > `watch` > `unknown` > `explain`；仍然只是 report-only。 |
| `explanation_scope` | `cp2_first_drift` / `root_order_inflation` / `root_cost_partial` / `final_side_effect` / `insufficient_evidence`。 |

### Guard 指标与状态语义
- `opening best-move guard`：检查 baseline move 是否保持、candidate move 是否 drift、new best move 是否可解释。cp2 从 baseline `(0, 1) -> (1, 2) (B)` 漂到 `(5, 1) -> (4, 2) (B)` 可写 `explain`，因为 Phase 3.7/3.9 能解释 cp2 first drift；final `20260423` 被 neutralization 推到 `(4, 1) -> (3, 2) (B)` 必须写 `regress` side-effect，不能说修好了。
- `root-order guard`：检查 baseline move rank 是否降级、drift move rank 是否升到 rank 1、root order 是否 baseline-like。cp2 原始 baseline order rank `2` / drift rank `1` 是 `watch/regress` 证据；Phase 3.9 neutralized cp2 baseline rank `2->1` 是 `explain` 证据。
- `root-cost guard`：输出 `nodes_ratio`、`qnodes_ratio`、`qnodes_per_nodes_ratio`、`aspiration_retry_delta`。watch 条件建议为任一 cost ratio `> 1.10` 或 qnodes/nodes 明显上升但 best move 可解释；regress 条件建议为 nodes/qnodes 任一 `> 1.25`、qnodes/nodes 同向上升且伴随 drift rank 1，或 retry delta 不能解释成本膨胀。该阈值只用于 guard 归因分级，不得写入 selection gate。
- `static-drift guard`：消费 Phase 3.7 的 distributed tuple support，K5/K10 weakening ratio 是核心指标。`explain` 要求 C1/C2 主 comparison 在 K5 或 K10 跨 `3/3` seed 有削弱/翻回 tie 且 controls pass；若只靠 K1/K3 或 all_negative 才有效，最多 `watch`，出现 over-neutralization signature 则 `regress`。
- `final-candidate side-effect guard`：检查是否出现 non-baseline new best move、root over-neutralization、cp2 可解释但 final 不可解释。Phase 3.9 的 `20260425` root cost 下降是 `explain/watch`，但 `20260423` new best move 和 qnodes 上升是 `regress` side-effect。

### 不是 selection gate
- `explain`：说明现象可由 Phase 3.3/3.7/3.9 证据链解释，不能表示 candidate 通过。
- `watch`：说明现象有风险或证据不足，需要下一步诊断，不改变 `overall_rating`。
- `regress`：说明 guard 观察到不可接受的诊断风险，但仍不替代正式 selection 的 `regress` 判定；正式 gate 仍由现有 selection policy 决定。
- `unknown`：说明输入 artifact 不足或字段不可得，不能默认放行。

### Phase 3.13 建议
- 可选 A：继续 design-only，设计 opening-safe training objective 的最小改造点，重点定义 objective 输入、惩罚项、避免过拟合 opening case 的 controls。
- 可选 B：实现 report-only opening guard diagnostics 脚本，读取上述 artifact 并生成 `opening-guard-metrics.csv`、`opening-guard-summary.json`、`opening-guard-manifest.json`。
- 推荐优先 B：Phase 3.12 已把字段和状态语义定清，下一步先把 guard 变成可复跑的报告产物，比继续堆设计更能暴露字段缺口；但 B 仍不进入 Phase 4，不训练，不跑 selection，不改 gate。

## 2026-05-23 Phase 3.11 design-only opening-safe training objective / root-order guard

### 本轮边界
- 本轮只做 Phase 3.11 design-only 文档更新，不实现代码，不训练，不跑 selection，不跑 `GamesPerSeed=64`。
- 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 未修改 `src\` / `include\`，未修改 `tools\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未生成新的 `.bin`，未修改 selection gate 阈值，未进入 Phase 4，未把 Phase 3.9 neutralized 权重写成 candidate 或 production 方案。

### 设计基线
- Phase 3.1 selection gate 仍为 `regress`，不能跑 64。
- Phase 3.2-3.3 已定位 opening 是唯一硬 blocker：cp2 first drift/root order inflation 出现在 checkpoint-2，cp2 无 aspiration retry，qnodes 集中到新的 PV root move。
- Phase 3.5 排除 single tuple dominance：36 个 comparison dominance 全 `no`，top1/top3 abs share 最高 `0.165094/0.376866`。
- Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference：K5/K10 跨 `3/3` seed 成立，K1/K3 不足，controls pass。
- Phase 3.9 证明 cp2 first drift/root order inflation 可被 offline neutralization 强解释：四个 variant 均 `3/3` seed 回到 baseline best move，root order `2->1`，nodes/qnodes 下降。
- final 只能 partial explanation：`20260425` root cost 可降，但 `20260423` 有 1/3 root over-neutralization、新 best move `(4, 1) -> (3, 2) (B)` 和 qnodes 上升。

### 方向一：candidate acceptance guard
| 项 | 设计 |
|---|---|
| 目标 | 在候选权重进入正式 selection 前，用 report-only opening diagnostics 暴露 root drift / root-cost 风险。 |
| 作用阶段 | candidate acceptance / selection preflight；先做报告，不改现有 gate。 |
| Phase 3 证据 | Phase 3.3 的 cp2 root order inflation；Phase 3.9 的 cp2 root order `2->1` 与成本下降；final `20260423` 新 best move 副作用。 |
| 预期能解决 | 让 cp2 first drift、baseline root order 降级、qnodes/root cost inflation、final new best move 在候选接受前可见。 |
| 不能解决 | 不能阻止训练产生 drift-favoring tuple deltas，不能证明棋力提升，不能替代 32/64 selection。 |
| 风险 | guard 过窄会漏掉非 opening 问题；guard 过宽会变成换皮 gate；若直接纳入 pass/fail 就是在偷改阈值。 |
| 最小验证 | Phase 3.12 先定义 report-only JSON/TXT 字段：root best move、baseline rank、root cost、qnodes、new-best-move side effect；不改变 `overall_rating`。 |
| 是否需要生产代码变更 | Phase 3.12 不需要；未来若集成到 selection summary 可能需要脚本/诊断层变更。 |
| 是否可能影响 selection gate | 短期不影响；未来若升级为 acceptance guard，会影响候选晋级，但必须单独评审，不得调阈值。 |

### 方向二：opening-safe training objective
| 项 | 设计 |
|---|---|
| 目标 | 在训练目标层限制 opening baseline child 相对 drift child 的不安全偏移，减少 stage-vs-bootstrap 的 drift-favoring distributed tuple deltas。 |
| 作用阶段 | TD/self-play training objective；不是 candidate 后处理。 |
| Phase 3 证据 | Phase 3.5 cp2 drift static delta `-13`、final drift static delta `-105/-138/-150`；Phase 3.7 K5/K10 分布式削弱曲线。 |
| 预期能解决 | 降低 cp2 first drift 和 root order inflation 复发概率，让训练别把 opening tie/bound 局面推成更贵 PV 子树。 |
| 不能解决 | 不保证 final candidate 完全恢复，不解决 match strength C/B/D 落后，不替代 fixed-position 和 self-play selection。 |
| 风险 | 过拟合单一 opening case；压制真实强着学习；把 final partial explanation 误当全局修复；训练 objective 一改就必须重走证据链。 |
| 最小验证 | Phase 3.12 先把 objective 写成离线可计算指标：对 Phase 3.5/3.7 表格模拟约束项，看是否只压 drift-favoring deltas 而不触发 controls。 |
| 是否需要生产代码变更 | 未来实现会需要训练代码变更；Phase 3.11/3.12 不需要。 |
| 是否可能影响 selection gate | 会；任何实现后的权重都必须从 `GamesPerSeed=32` 重新开始，32 至少 watch 才能讨论 64。 |

### 方向三：root-order regularization
| 项 | 设计 |
|---|---|
| 目标 | 把 root child ordering 稳定性作为训练或候选验收的显式约束，避免轻微 static delta 把 opening tie/bound 推成高成本 PV。 |
| 作用阶段 | 可作为 training checkpoint regularization，也可先作为 candidate acceptance diagnostics。 |
| Phase 3 证据 | Phase 3.3 cp2 `aspiration_retries=0`，root order 从 baseline rank 1 变成 drift rank 1 / baseline rank 2；Phase 3.9 neutralization 后 rank `2->1` 且 nodes/qnodes 下降。 |
| 预期能解决 | root order inflation、PV root move 迁移、qsearch 成本被新 PV 子树放大的问题。 |
| 不能解决 | 不能保证新 best move 一定错误，不能修复 final `20260423` 副作用，不能解释非 opening case 或全局棋力。 |
| 风险 | 把 baseline move 固化成教条，阻止真实强着替换；bound-tie 情况下 rank 太敏感；final 已证明新 best move 副作用需要单独分类。 |
| 最小验证 | Phase 3.12 先定义 root-order/rank/cost delta、safe/tie/new-best-move 分类，并在 cp2/final 原始与 neutralized diagnostics 上回放。 |
| 是否需要生产代码变更 | report-only 版本不需要；训练期 regularization 未来可能需要。 |
| 是否可能影响 selection gate | 未来若纳入 acceptance guard 会影响候选晋级；当前只作为设计和诊断，不改 gate。 |

### 推荐顺序
1. 优先 `design/report-only guard`：先把 opening root drift、root order inflation、final new best move side effect 的拒绝条件设计清楚。
2. 再考虑最小 `opening-safe training objective` 约束：只约束 Phase 3 已证明的 distributed tuple drift 机制，别一上来换训练宇宙。
3. 最后才考虑生产搜索或特征扩展：Phase 4 仍 blocked，直接扩 N-Tuple / threat / mobility / stage 特征就是把锅盖扣上继续炖，香不香不知道，糊不糊肯定难查。

### 明确不推荐
- 不直接扩 N-Tuple / threat / mobility / stage 特征。
- 不直接 all_negative neutralization。
- 不直接重训并跑 `GamesPerSeed=64`。
- 不调 selection gate 阈值。
- 不把 Phase 3.9 neutralized 权重当 production candidate。

### Phase 3.12 建议
- 下一步建议 Phase 3.12 design-to-diagnostic bridge。
- 目标是把 Phase 3.11 的三类设计转成 report-only diagnostic spec：字段、artifact schema、fixed-case checks、通过/停止解释口径。
- Phase 3.12 仍不允许直接实现 training objective，不允许 selection，不允许跑 64，不允许进入 Phase 4。
- Phase 3 still not passed；Phase 4 still blocked。

## 2026-05-23 Phase 3.10 report-only root-order/root-cost explanation summary

### 本轮边界
- 本轮只做 Phase 3.10 证据链总结和文档更新。
- 只更新 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未新增 neutralized `.bin`，未把 Phase 3.9 写成 production candidate。
- Phase 3.10 是解释总结，不是修复总结；把化验单写漂亮不能让病自动好，这点别犯迷糊。

### 证据链结论
- Phase 3.1 selection gate 仍为 `regress`：`overall_rating=strength_rating=perf_rating=regress`，按 gate 停止，不能跑 64。
- Phase 3.2-3.3 将唯一硬 blocker 定位到 opening：cp2 首次 drift 三 seed 同型，best move 从 baseline `(0, 1) -> (1, 2) (B)` 漂到 `(5, 1) -> (4, 2) (B)`；root order 从 baseline rank 1 变为 drift move rank 1 / baseline rank 2，且 cp2 `aspiration_retries=0`，所以 first drift/root order inflation 不是 retry 背锅。
- Phase 3.5 排除 single tuple dominance：27 行 summary、1215 行 tuple detail、1440 行 top-deltas 全部 hash/header 可追溯；36 个 comparison 的 dominance 全为 `no`，top1/top3 abs share 最高只有 `0.165094/0.376866`。这不是“一刀砍一个 tuple 就完事”的局面。
- Phase 3.7 证明 distributed tuple deltas 可静态削弱 drift preference：`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`；K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部推回 tie/非 drift-favoring；controls 全部 pass。
- Phase 3.9 对 cp2 有强解释力：四个 variant 均在 `3/3` seed 把 cp2 best move 从 `(5, 1) -> (4, 2) (B)` 拉回 baseline `(0, 1) -> (1, 2) (B)`，baseline root order `2->1`，nodes/qnodes 下降，且 aspiration retries 仍为 `0`。
- final candidate 只能写 partial explanation：`20260425` 在 conservative core / C2-focused stable K10 下 root cost 明显下降，但 `20260424` 基本不变，`20260423` 出现 1/3 root over-neutralization，被推到新 best move `(4, 1) -> (3, 2) (B)`，nodes/qnodes 从 `1978/1123` 升到 `2456/1461` 或 `2533/1532`。
- conservative_core / c2_focused_stable_k10 比 c1_only_k10 / union_k10_riskier 风险更低：前两者在 cp2 无 root/static over-neutralization，且 tuple 集合更小、更聚焦；后两者 cp2 static ablated delta 推到 `+1`，union 还扩大 tuple 覆盖，只能当 riskier 压力对照。

### 当前判定
- opening first drift / root order inflation 的来源可解释为 post-training distributed tuple deltas 改变 root child ordering，并把 qnodes 集中到新的 PV root move；cp2 阶段证据强。
- final 的边界是 partial root-cost explanation：存在 tuple-induced static component 和部分 root-cost coupling，但 PVS/root search 没有稳定恢复，还引入新 best move 副作用。
- Phase 3 仍未通过；Phase 4 仍 blocked。
- 下一步推荐 Phase 3.11 design-only：设计 `opening-safe training objective`、`root-order regularization`、`candidate acceptance guard`。
- 明确不推荐：直接扩 N-Tuple 特征、直接 all_negative neutralization、直接 rerun 64 games、调 gate 阈值、把 neutralized 权重当 candidate。

## 2026-05-23 Phase 3.9 offline neutralization diagnostics execution

### 本轮边界
- 本轮执行 Phase 3.9 offline neutralization diagnostics execution，只做 report-only diagnostics。
- 新增非生产诊断脚本 `D:\Dev\Code\surakarta_traditional\tools\offline-neutralization-diagnostics.ps1`。
- 未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未训练，未跑 selection，未跑 `GamesPerSeed=64`，未改 gate 阈值，未进入 Phase 4。
- 临时 neutralized `.bin` 只写入 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization\weights-temp`，文件名均包含 `diagnostic-neutralized`；未覆盖 `bootstrap.bin`、`checkpoint-*.bin`、`candidate.bin`。
- 所有 neutralized artifact 和 manifest 均标记 `production_candidate=false`、`selection_gate_eligible=false`。这玩意儿就是诊断切片，不是候选权重，别拿它冒充通关文牒。

### 输入与校验
- 必读文件已纳入脚本输入 manifest：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`、`weights-manifest.json`、`selection-summary.json`、`opening-diagnostics-summary.md`、Phase 3.5 attribution 四个文件、Phase 3.7 static ablation 五个文件。
- artifact 解析严格来自 `weights-manifest.json`：cp2 按 `artifact_kind=checkpoint` 且 `checkpoint.games_completed=2`；final 按 `artifact_kind=candidate`。
- Phase 3.1 参数事实源是 `selection-summary.json`：`games_per_seed=32`、`depth=4`、`checkpoint_every=2`、`alpha=0.005`、`lambda=0.5`、`epsilon=0.02`、`epsilon_plies=6`、`terminal_reward=1200.0`、`td_error_clip=0.0`。manifest artifact 必须逐项匹配，否则脚本立即停止；本轮校验通过。
- 权重 header 校验通过：24 byte little-endian header，`magic=0x5455504C`、`version=1`、`tuple_set_hash=0x1D4A6EBF2BAF6484`、`weight_count=16605`、payload 为 16605 个 int32，文件长度 `66444`。

### 输出产物
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization`。
- 主要文件：
  - `neutralization-manifest.json`
  - `neutralization-summary.csv`
  - `neutralization-root-diagnostics.csv`
  - `neutralization-weight-deltas.csv`
  - `diagnostics-json\*.json`
  - `weights-temp\**\*.diagnostic-neutralized.bin`
- 规模：临时 `.bin` 24 个；diagnostics JSON 30 个；summary 24 行；root diagnostics 24 行；weight deltas 348 行。
- diagnostics 命令固定为 `build-vs-train-release\bin\surakarta-benchmark.exe bitboard-search --depth 4 --threads 1 --format json --case-id opening --weights <path> --search-diagnostics`，不传 `--file`，即默认 opening。

### 核心结果
- cp2 阶段：四个 variant 在 3/3 seed 都把 original best move `(5, 1) -> (4, 2) (B)` 拉回 baseline `(0, 1) -> (1, 2) (B)`，baseline root order rank `2->1`，aspiration retries 保持 `0`。
- cp2 conservative core / C2-focused stable K10：nodes `1797->1569`，qnodes `1277->1063`，无 root over-neutralization。
- cp2 C1-only K10 / union K10 riskier：nodes `1797->1722`，qnodes `1277->1219`，但 static ablated delta 推到 `+1`，标记 static over-neutralization；root 层未引入新 best move。
- final 阶段：只能写 partial explanation，不是修复。`20260425` 在 conservative core / C2-focused stable K10 下 nodes/qnodes 明显下降（例如 conservative core `2495->1619`、`1485->1080`），但 `20260424` 基本不变。
- final `20260423` 是风险点：所有 variant 都把 best move 从 `(3, 1) -> (2, 2) (B)` 推到新 move `(4, 1) -> (3, 2) (B)`，并增加 nodes/qnodes；conservative core 和 C2-focused stable K10 为 `1978->2456`、`1123->1461`，C1-only/union 为 `1978->2533`、`1123->1532`。这是 root over-neutralization/side-effect，不允许包装成 gate 修复。

### 结论
- Phase 3.9 足够进入 report-only root-order/root-cost explanation 总结：cp2 first drift/root order inflation 可以由小 tuple set offline neutralization 解释并缓解。
- final gate blocker 只得到部分解释：它支持 tuple-induced static component 与部分 root-cost coupling，但 PVS/root search 没有稳定恢复，且 `20260423` 出现新 root drift。
- Phase 3 仍未通过；Phase 4 仍 blocked。
- Phase 3.9 不替代 selection gate，不允许进入 `GamesPerSeed=64`，不允许当 production candidate。

## 2026-05-23 Phase 3.8 offline neutralization design review

### 本轮边界
- 本轮只做 offline neutralization design review，不执行 neutralization。
- 未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`，未运行 `bitboard-search` / training / selection，未进入 Phase 4。
- 未修改 selection gate 阈值，未写任何 `.bin` 权重文件，未创建 `phase3_8` neutralized 权重目录。
- 本阶段明确记录：`weights_written=false`，`neutralization_executed=false`，`offline_neutralization_execution_allowed=false`。

### 已读取证据
- 已读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 已读取 Phase 3.5 attribution 目录：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`
- 已读取 Phase 3.7 static ablation 目录：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-sweep.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-summary.json`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation\opening-child-static-ablation-manifest.json`

### 是否满足进入设计评审
- 满足进入 **offline neutralization 设计评审** 的条件。
- Phase 3.5 证据链：summary `27` 行，tuple detail `1215` 行，top-deltas `1440` 行；artifact hash/header pass；tuple set hash `0x1D4A6EBF2BAF6484`，weight count `16605`。
- Phase 3.5 static preference：cp2 阶段三 seed 的 cp2 drift child 相对 baseline child delta 均为 `-13`；final 阶段 final drift child 相对 baseline child delta 为 `-105/-138/-150`。这解释了 drift direction，但 dominance 全 `no`，不是一两个 tuple 单独背锅。
- Phase 3.7 证据链：summary `7` 行、tuple detail `174` 行、TopK sweep `126` 行；attribution manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`；tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`。
- Phase 3.7 主 comparison：`C1_cp2_onset` 与 `C2_final_gate_blocker` 均为 `success`；K1/K3 在两者中均 `0/3` seed success，K5/K10 均 `3/3` seed success；controls 全部 `control_pass`；`global_decision=success_for_offline_neutralization_design_review`。
- 这不满足执行 neutralization 的条件。设计评审只是把下一步怎么做写清楚，别把“可以画施工图”听成“楼已经盖好了”，那就扯远了。

### 应覆盖哪些 tuple
- `C1 K10`：`4,5,6,10,11,27,32,36,38,41`。这是 cp2 onset 的主集合，三 seed 完全一致，预期解释 checkpoint-2 first drift。
- `C2 stable K10 core`：`5,10,32,41,43`。这是 final gate blocker 的 `3/3` seed 稳定核心，预期解释 final drift。
- `C2 2/3 support set`：`0,5,6,10,11,32,33,37,41,43,44`。这是 final blocker 的支持/审计集合，不建议作为首轮默认 neutralization 写权重集合。
- `C1/C2 shared conservative core`：`5,10,32,41`。这是 C1 K10 与 C2 stable K10 的交集，适合做最保守的 cross-stage 解释力测试。

### C1/C2 是否同一 tuple set
- 不建议强行使用同一 tuple set。
- 推荐按 stage / comparison 分 variant：cp2 stage 用 C1-oriented set，final stage 用 C2-oriented set；同时保留一个 shared conservative core variant 检查共同机制。
- 原因：`C1` 目标是 cp2 onset，`C2` 目标是 final gate blocker；二者有共享 tuple，但不是完全同一个病灶。硬塞同一 set 容易把 cp2 和 final 两个阶段搅成一锅粥，最后啥都能解释，等于啥也没解释。

### 推荐 design variants
| variant | tuple set | stage/comparison | 预期解释 |
|---|---|---|---|
| conservative core | `5,10,32,41` | shared / cross-stage | 同时削弱 cp2 onset 与 final drift 的共享部分；最保守，解释力干净 |
| C1-only K10 | `4,5,6,10,11,27,32,36,38,41` | cp2 / `C1_cp2_onset` | 解释 cp2 checkpoint-2 first drift；不要求解释 final drift |
| C2-focused stable K10 | `5,10,32,41,43` | final / `C2_final_gate_blocker` | 解释 final gate blocker；不要求解释 cp2 onset |
| optional union K10 | `4,5,6,10,11,27,32,36,38,41,43` | shared stress check | riskier，只做压力对照；若只有它有效，说明需要谨慎，不能直接放行 |

### 推荐 K 值
- 推荐围绕 `K10` 做设计，但执行时优先比较 conservative core、C1-only K10、C2-focused stable K10，而不是无脑取 all_negative。
- 不是 `K1/K3`：Phase 3.7 明确显示 `C1`、`C2` 的 K1/K3 均为 `0/3` seed success；这不是单点 tuple 主导，top1/top3 拿不住。
- 不是直接 `all_negative`：C1 K10 已等价 all_negative，且 `over_neutralization=True`；C2 all_negative 会写 `15-17` 个 tuple，并把 ablated delta 推到 `+29/+53/+30`。这叫诊断过头，不叫更科学。
- K10 的价值是覆盖稳定分布式效应，同时仍能分辨 variant；all_negative 会把差异抹平，最后只剩“砍多了确实会变”，这废话含量太高。

### 临时权重输出设计
- Phase 3.8 本轮不写权重，`weights_written=false`。
- 如果 Phase 3.9 真执行，只能复制源权重到 opening diagnostics 下的临时目录：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization\weights-temp\<variant>\seed-<seed>\<stage>\`
- 命名必须显式诊断用途：
  - `seed-20260423.cp2.c1-k10.diagnostic-neutralized.bin`
  - `seed-20260423.final.c2-stable-k10.diagnostic-neutralized.bin`
  - `seed-20260423.final.conservative-core.diagnostic-neutralized.bin`
- 禁止命名为 `candidate.bin`、`checkpoint-2.bin`、`bootstrap.bin`；禁止写入 `selection\seed-*` 下任何 bootstrap/checkpoint/candidate 目录。临时权重不得作为 production candidate。

### neutralized 权重 manifest 字段
- 必填追溯字段：phase、mode、generated_at、git_commit、source attribution manifest SHA256、source static-ablation manifest SHA256、source weight path/SHA256、source artifact kind、header magic/version/tuple_set_hash/weight_count。
- 必填 variant 字段：variant id、comparison id、stage、seed、K policy、tuple ids、absolute indices、encoded indices、from/to root move、expected explanation target。
- 必填改写字段：original weight values、neutralized values、removed deltas、per-tuple induced_pair_delta、total removed delta、expected ablated pair delta。
- 必填边界字段：`weights_written`、`neutralization_executed`、`production_candidate=false`、`selection_gate_modified=false`、`training_executed=false`、`selection_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。
- Phase 3.8 的设计文档必须继续保持 `weights_written=false`、`neutralization_executed=false`；只有 Phase 3.9 execution manifest 才能把它们改成 true，而且仍必须是 diagnostics-only。

### 后续最小 diagnostics
- 只允许 offline diagnostics：header/hash/manifest 校验、static attribution 复算、opening fixed-case depth-4 root diagnostics / search diagnostics。
- 最小对比维度：原始 bootstrap/cp2/final 权重 vs temp diagnostic-neutralized 权重；只看 opening root child 的 best move、root order、observed score、nodes/qnodes、aspiration retries。
- 不允许 training、selection、`GamesPerSeed=64`、production candidate gate、Phase 4 feature work。

### 什么结果才有解释力
- conservative core 能同时削弱 `C1` 与 `C2` 的 static/root-order preference，说明共享 tuple 机制可信。
- C1-only K10 让 cp2 stage 的 baseline child 回到 tie/非 drift-favoring，同时 cp2 drift root 的 qnodes/node inflation 下降，说明 cp2 onset 有解释力。
- C2-focused stable K10 让 final stage 的 final drift child 不再稳定压过 baseline child，且 final qnodes/node inflation 下降，说明 final gate blocker 有解释力。
- 结果必须跨 `20260423/20260424/20260425` 至少 `2/3` seed 成立，最好 `3/3`；controls 不报警；manifest/hash/header 全部一致。
- 若只在 static 表格里翻转，但 PVS/root search 仍漂，结论只能写“static component explains part of drift”，不能写“neutralization fixed Phase 3”。

### 必须停止的结果
- 需要 all_negative 或超过 K10 才能看到效果。
- C1 解释不了 cp2 onset，或者 C2 只能解释 final 但引入新 opening root drift。
- controls 失效，bootstrap 天然偏向 drift，或 cp2 阶段 final drift child 被误判为首漂移。
- temp 权重污染 selection seed 目录，或者文件名/manifest 让它看起来像 production candidate。
- hash/header/tuple_set_hash/weight_count 不一致。
- 有人想用 neutralization 结果替代 selection gate、放宽阈值、跑 64、调 `alpha/lambda/epsilon`、调 PVS/qsearch、或进入 Phase 4。这个时候必须停，别把诊断报告当通行证，技术债不吃这套。

### 结论
- 建议进入 Phase 3.9 execution，但范围只能是 offline neutralization diagnostics execution。
- 不建议、也不允许进入 Phase 4。
- 不允许用 neutralization 结果替代 selection gate。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-22 Phase 3.7 child-position static ablation execution

### 本轮边界
- 本轮执行 child-position static ablation 的 report-only/table-only 版本，不改生产代码。
- 未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`，未运行 search diagnostics，未进入 Phase 4。
- 未修改 selection gate 阈值，未生成或改写 candidate weights，未执行 offline neutralization。

### 输入与追溯性
- 已读取 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- 已读取 Phase 3.5 attribution 输入：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`
- Phase 3.6 设计结论对应的四段已复核：comparison 集合、`induced_pair_delta` 口径、TopK 策略、success/failure/inconclusive 与 offline neutralization 升级/停止条件。
- 输入 manifest hash `F25096DAC043D064AC3F153D0DC4C1865EE8A3B0F4DD05B6BECA408AC7132086`；tuple detail hash `ACE1ACDF436271C7C99C8382ECCB17B5F2828046662AA00C57B668A1E1F7C10E`；tuple set hash `0x1D4A6EBF2BAF6484`；weight count `16605`；git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。

### 脚本与输出
- 新增非生产只读诊断脚本：`D:\Dev\Code\surakarta_traditional\tools\child-position-static-ablation.ps1`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation`。
- 输出文件：
  - `opening-child-static-ablation-summary.csv`
  - `opening-child-static-ablation-tuples.csv`
  - `opening-child-static-ablation-sweep.csv`
  - `opening-child-static-ablation-summary.json`
  - `opening-child-static-ablation-manifest.json`
- 规模：summary `7` 行，selected tuple detail `174` 行，TopK sweep `126` 行。manifest 明确记录 `weights_written=false`、`candidate_weights_written=false`、`offline_neutralization_executed=false`、`search_executed=false`、`training_executed=false`、`games_per_seed_64_executed=false`、`phase4_entered=false`。

### 执行的 comparison
| comparison id | 类型 | stage | from child | to child | 判定 |
|---|---|---|---|---|---|
| `C1_cp2_onset` | main | `cp2` | baseline child | cp2 drift child | `success` |
| `C2_final_gate_blocker` | main | `final` | baseline child | final drift child | `success` |
| `C3_final_cp2_residue` | auxiliary | `final` | baseline child | cp2 drift child | `success`，只支持路径解释 |
| `C4_final_transition` | auxiliary | `final` | cp2 drift child | final drift child | `success`，只支持路径解释 |
| `N1_bootstrap_cp2_sanity` | control | `bootstrap` | baseline child | cp2 drift child | `success`，control pass |
| `N1_bootstrap_final_sanity` | control | `bootstrap` | baseline child | final drift child | `success`，control pass |
| `N2_cp2_final_negative_control` | control | `cp2` | baseline child | final drift child | `success`，control pass |

### TopK 曲线结果
- `C1_cp2_onset`：三 seed stage pair delta 全为 `-13`，bootstrap pair delta 全为 `0`，training-induced pair delta 全为 `-13`。K1/K3 不足，K5 已在 `3/3` seed 达到 `>=60%` 削弱，K10/all_negative 推回 `+1`，说明 cp2 首漂移由多 tuple 累积导致，不是 top1 单点一刀切。
- `C2_final_gate_blocker`：stage pair delta 为 `-105/-138/-150`，bootstrap pair delta 全为 `+18`，training-induced pair delta 为 `-123/-156/-168`。K1/K3 不足，K5 在 `3/3` seed 达到 `>=60%` 削弱，K10 全部翻到非 drift-favoring（`+22/+37/+20`），all_negative 明显过度中和。
- `C3_final_cp2_residue`：final 权重中 cp2 drift child 仍有残余偏好，K3 起 `3/3` seed 达标；这是解释 final 残留路径，不是 offline neutralization 主依据。
- `C4_final_transition`：final drift child 相对 cp2 drift child 的 transition 也可被 TopK 解释，K3 起 `3/3` seed 达标；这是解释后期收敛路径，不是单独放行依据。
- controls 未报警：bootstrap 下 baseline->cp2 为 `0`，baseline->final 为 `+18`；cp2 阶段 baseline->final 为 `+14`，仍不支持 final drift child 是 cp2 首漂移原因。

### 被选入 ablation 的 tuple
- `C1_cp2_onset` 的 K10 三 seed 完全一致：`4,5,6,10,11,27,32,36,38,41`；其中 top negative rank 为 `6/10/32/41`（各 `-2`），后续为 `4/5/11/27/36/38`（各 `-1`）。
- `C2_final_gate_blocker` 的 K10 具有稳定核心：`3/3` tuple 为 `5,10,32,41,43`；`2/3` tuple 为 `0,5,6,10,11,32,33,37,41,43,44`。
- `C3` 的 K10 `3/3` tuple 为 `5,6,10,26,31,32,41`；`C4` 的 K10 `3/3` tuple 为 `3,8,9,33,43`。
- 这些 tuple 都来自 stage-vs-bootstrap 的 `induced_pair_delta < 0`，不是直接抓 same-stage top1。直接抓 top1 这事现在证据不支持，硬抓就是把复杂病灶当青春痘挤，手法挺猛，方向不对。

### dominance=no 时结果是否有效
- 有效，但边界很窄：Phase 3.5 dominance 全 `no` 只说明没有少数 tuple 单点主导，不说明 distributed ablation 没意义。
- Phase 3.7 的 K1/K3 不足、K5/K10 才稳定削弱/翻转，正好说明这是分布式累积效应。
- 这只能证明 static child preference 的训练诱导部分能被小到中等 TopK tuple 集合解释；它不能证明 PVS 全宽 root score 会恢复，不能证明 search-tree inflation 会消失，更不能让 Phase 3 通过。

### 是否足够进入 offline neutralization 设计
- 足够进入 **offline neutralization 设计评审**：主 comparison `C1` 与 `C2` 均在 `3/3` seed 由 `K5` 达到 `>=60%` 削弱，并由 `K10` 推回 tie/非 drift-favoring；control 全部 pass；tuple 集合存在 `2/3`、`3/3` seed 稳定核心。
- 不足以直接执行 offline neutralization：K10/all_negative 已出现过度中和，且当前仍只是 table-only static ablation，没有 search/PVS root diagnostics 复核。
- 下一步若做，也只能先写 offline neutralization 设计：限定复制临时权重到 diagnostics 目录、只做 opening diagnostics、保留 manifest/hash/header、不得替换生产权重、不得训练、不得改 gate。

### 必须停止的事项
- 现在必须停止在“设计评审”边界前：不写 neutralized weights，不执行 offline neutralization，不跑 search，不跑训练，不跑 `GamesPerSeed=64`，不进入 Phase 4。
- 不能把 `success_for_offline_neutralization_design_review` 解读成 Phase 3 通过。这个帽子要是乱戴，后面就成了拿诊断报告当治疗证明，挺吓人。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-22 Phase 3.6 child-position static ablation design

### 本轮边界
- 本轮只做 child-position static ablation 设计，不执行 ablation。
- 未写代码，未新增脚本，未修改 `src\` / `include\`，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未重新训练，未跑 `GamesPerSeed=64`，未进入 Phase 4。
- 未生成改写后的 candidate weights，未执行 offline neutralization。

### 已读取证据
- `task_plan.md`
- `findings.md`
- `progress.md`
- `docs\national_competition_technical_roadmap.md`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-summary.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-tuples.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\opening-child-attribution-top-deltas.csv`
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution\attribution-manifest.json`

### Phase 3.5 输入事实
- attribution manifest：git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，tuple hash `0x1D4A6EBF2BAF6484`，weight count `16605`，seeds `20260423/20260424/20260425`，stages `bootstrap/cp2/final`。
- child positions：`baseline_child=(0,1)->(1,2) (B)`，`cp2_drift_child=(5,1)->(4,2) (B)`，`final_drift_child=(3,1)->(2,2) (B)`。
- summary `27` 行；tuple detail `1215` 行；top-deltas `1440` 行；每个 child position active tuple count `45`。
- cp2 阶段：三 seed 均为 baseline child `-10`，cp2 drift child `-23`，final drift child `4`；cp2 drift 相对 baseline 的 static delta 为 `-13`。
- final 阶段：final drift child 相对 baseline child 的 static delta 为 `20260423=-105`、`20260424=-138`、`20260425=-150`。
- bootstrap 阶段：baseline child 与 cp2 drift child 打平 `-6 -> -6`；final drift child 为 `12`，即 bootstrap 下 final drift child 不优于 baseline。
- dominance：36 个 comparison 全部 `no`，max top1/top3 abs share 为 `0.165094/0.376866`。

### 应比较哪些 child positions
| comparison id | stage | from child | to child | 目的 | 原始 delta |
|---|---|---|---|---|---|
| `C1_cp2_onset` | `cp2` | baseline child | cp2 drift child | 解释 checkpoint-2 首次 drift | 三 seed 均 `-13`；bootstrap pair delta `0`，training-induced pair delta `-13` |
| `C2_final_gate_blocker` | `final` | baseline child | final drift child | 解释最终 selection blocker | final pair delta `-105/-138/-150`；bootstrap pair delta `+18`；training-induced pair delta `-123/-156/-168` |
| `C3_final_cp2_residue` | `final` | baseline child | cp2 drift child | 看早期 drift child 是否在 final 权重中仍残留偏好 | final pair delta `-74/-44/-98`；bootstrap pair delta `0` |
| `C4_final_transition` | `final` | cp2 drift child | final drift child | 解释从早期 cp2 drift 切到 final drift 的静态偏好 | final pair delta `-31/-94/-52`；bootstrap pair delta `+18` |
| `N1_bootstrap_sanity` | `bootstrap` | baseline child | cp2/final drift child | control：bootstrap 不应天然支持 drift 结论 | baseline->cp2 为 `0`，baseline->final 为 `+18` |
| `N2_cp2_final_negative_control` | `cp2` | baseline child | final drift child | control：final drift child 不应被误当成 cp2 首漂移原因 | cp2 pair delta `+14`，仍不支持 final drift child |

`C1` 和 `C2` 是主 comparison；`C3/C4` 是解释 final 收敛路径的辅 comparison；`N1/N2` 只做 control，不作为升级 offline neutralization 的依据。

### 每个 comparison neutralize 哪些 tuple deltas
neutralization 单元不是“直接把同 stage 两个 child 的贡献差砍掉”，也不是直接抓 `opening-child-attribution-top-deltas.csv` 的 top1。正确口径是从 `opening-child-attribution-tuples.csv` 派生 stage-vs-bootstrap 的 pair-induced delta：

```text
stage_delta(child, tuple) = side_to_move_contribution(stage, child, tuple)
                          - side_to_move_contribution(bootstrap, child, tuple)

induced_pair_delta(tuple) = stage_delta(to_child, tuple)
                          - stage_delta(from_child, tuple)
```

当 `to_child` 是 drift child，且 child 均为 White to move 时，更低的 `total_static_score` 更偏向 Black；因此 `induced_pair_delta < 0` 才是 drift-favoring tuple delta，static ablation 应在内存/表格层把这些 selected deltas 置零。

| comparison | neutralize 对象 |
|---|---|
| `C1_cp2_onset` | `cp2` 相对 `bootstrap` 的 `baseline_child -> cp2_drift_child` induced pair deltas，且只选 `induced_pair_delta < 0`。三 seed 完全一致；top negative tuple id 为 `6/10/32/41`（各 `-2`），随后 `4/5/11/27/36/38`（各 `-1`）。 |
| `C2_final_gate_blocker` | `final` 相对 `bootstrap` 的 `baseline_child -> final_drift_child` induced pair deltas，且只选 `induced_pair_delta < 0`。top10 中跨三 seed 都出现的 tuple id 为 `10/32/41/43/5`；出现于 `2/3` seed 的有 `0/6/11/33/37/44`。per-seed ranking 仍必须保留，不能用全局 union 糊过去。 |
| `C3_final_cp2_residue` | `final` 相对 `bootstrap` 的 `baseline_child -> cp2_drift_child` induced pair deltas，选 `induced_pair_delta < 0`；用于判断早期 drift child 在 final 权重里是否还有残余静态偏好。 |
| `C4_final_transition` | `final` 相对 `bootstrap` 的 `cp2_drift_child -> final_drift_child` induced pair deltas，选 `induced_pair_delta < 0`；用于解释 final drift child 为什么后来压过 cp2 drift child。 |
| `N1/N2 controls` | 默认不 neutralize，只输出原始 pair delta 与 induced pair delta。如果 control 出现 drift-favoring 异常，停止解释，先查数据或假设。 |

### TopK 策略
- 每个 seed/comparison 独立排序：按 `abs(induced_pair_delta)` 从大到小，过滤 `0`，tie-break 使用 `tuple_id`、active index、root move label，保证可复现。
- 输出 TopK sweep：`K=1/3/5/10/20/all_negative`。`K=1` 只作为曲线起点，不作为主结论。
- 同时输出 seed-stable 视图：tuple id / active index 在 `2/3` 和 `3/3` seed 中出现的频率，避免被单 seed 噪声牵着跑。
- 为什么不是直接抓 top1：Phase 3.5 已证明 dominance 全 `no`，top1/top3 abs share 最高只有 `0.165094/0.376866`。这说明变化是多 tuple 小幅累积，不是一颗螺丝单独飞出来。直接抓 top1，容易得到“没翻盘所以没因果”的假阴性，太莽了。

### dominance=no 时 ablation 是否有意义
有意义，但意义变窄：
- 它能测试 distributed tuple deltas 的累计效应，判断 static child preference 是否真由训练后的权重变化推出来。
- 它能区分“没有单 tuple 主导”和“static attribution 根本解释不了 drift”。这俩不是一回事，别混成一锅。
- 它不能证明 PVS 全宽 root score 会恢复，不能证明 search-tree inflation 会消失，更不能让 Phase 3 通过。
- 它不支持直接 offline neutralization；dominance=no 下盲改权重，基本就是拿扫帚修主板，动静大，依据薄。

### ablation 输出应包含哪些 CSV/JSON
建议输出目录仅作为未来设计，不在本轮创建：
- `opening-child-static-ablation-comparisons.csv`：comparison id、stage、from/to child、原始 pair delta、bootstrap pair delta、training-induced pair delta、control 标记。
- `opening-child-static-ablation-tuples.csv`：每个 selected tuple 的 seed、comparison id、tuple id、from/to active index、from/to stage delta、induced pair delta、rank、TopK membership、cells。
- `opening-child-static-ablation-sweep.csv`：每个 seed/comparison/K 的 neutralized sum、ablated pair delta、delta reduction ratio、sign flip/tie、over-neutralization 标记。
- `opening-child-static-ablation-summary.csv`：按 comparison 聚合 `3/3`、`2/3` seed 结果、最小 flip K、最大解释比例、结论 `success/failure/inconclusive`。
- `opening-child-static-ablation-summary.json`：CSV summary 的结构化版本，保留阈值、TopK policy、comparison definitions。
- `opening-child-static-ablation-manifest.json`：输入文件路径与 SHA256、attribution manifest hash、git commit、tuple hash、weight count、TopK policy、生成时间、明确声明 `weights_written=false`、`ablation_executed=false`、`offline_neutralization=false`。

### 成功、失败、无结论判定口径
- `success`：`C1` 和 `C2` 至少在 `2/3` seed、最好 `3/3` seed 中，用 `K<=10` 或最多 `K<=20` 的 drift-favoring induced deltas 将原始 drift pair delta 削弱 `>=60%`，或推回 tie/非 drift-favoring；control `N1/N2` 不报警；selected tuple 集合有跨 seed 稳定性。
- `failure`：`K=20` 甚至 `all_negative` 仍不能削弱 `>=30%`，或 neutralization 方向错误，或 control 出现异常，或需要改代码/改权重才能解释。
- `inconclusive`：削弱在 `30%-60%`，或只有 `1/3` seed 成立，或只解释 final 不能解释 cp2 onset，或需要几乎全部 45 个 active tuple 才能翻转。无结论不是坏事，至少能阻止继续玄学拧螺丝。

### 哪些结果才允许升级到 offline neutralization 设计
只有同时满足以下条件，才允许进入下一阶段的 offline neutralization 设计评审：
- `C1_cp2_onset` 与 `C2_final_gate_blocker` 都达到 success 或非常接近 success。
- tuple 集合小且稳定：优先 `K<=10`，最多 `K<=20`；核心 tuple 在 `2/3` 或 `3/3` seed 重复出现。
- control 不显示 bootstrap 天然偏向 drift，也不显示 cp2 阶段 final drift child 已经是首漂移原因。
- manifest/hash/header/tuple hash/weight count 全部可追溯。
- 设计仍限定为复制临时权重到 diagnostics 目录、只跑 opening diagnostics；不得替换生产权重，不得训练，不得改 selection gate。

### 哪些结果必须停止
- 需要 neutralize 大半 active tuple 或全部 `45` 个 tuple 才能看到变化。
- tuple 集合完全 seed-specific，没有稳定重叠。
- 只能解释 final drift，解释不了 cp2 onset。
- static ablation 能翻转但 control 也翻转，说明假设污染。
- 数据 manifest/hash/header 不一致。
- 下一步建议变成调 `alpha/lambda/epsilon`、调 qsearch/PVS、跑 `GamesPerSeed=64`、扩 Phase 4 特征，或者复制改写 candidate weights 直接试。这个时候就该刹车，别越诊断越上头。

### 结论
- Phase 3.6 只完成 child-position static ablation design，没有执行 ablation。
- 设计上应比较 baseline/cp2/final 三个 child position 的主 drift、残余 drift 和 transition，不应只盯 top1。
- 当前证据仍不支持 offline neutralization。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-22 Phase 3.5 opening attribution-only execution

### 本轮边界
- 本轮是 attribution-only 离线诊断，不是修复、不是 ablation、不是训练。
- 未重新训练，未跑 `GamesPerSeed=64`，未进入 Phase 4。
- 未修改 `src\` / `include\` 生产代码，未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未修改 selection gate 阈值，未生成改写后的 candidate weights，未执行 offline neutralization。

### 脚本与输入
- 新增脚本：`D:\Dev\Code\surakarta_traditional\tools\opening-attribution.ps1`。
- RunRoot：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution`。
- 已读取输入：
  - `RunRoot\weights-manifest.json`
  - `RunRoot\selection-summary.json`
  - `RunRoot\opening-diagnostics\opening-diagnostics-summary.md`
  - `task_plan.md`
  - `findings.md`
  - `progress.md`
- artifact 路径不手搓；脚本从 `weights-manifest.json` 解析 seed `20260423/20260424/20260425` 的 `bootstrap`、checkpoint `games_completed=2`、`candidate`。

### 校验结果
- 9 个 artifact 均通过文件存在与 SHA256 校验。
- artifact seed、stage、games/depth/alpha/lambda/epsilon/epsilon_plies/terminal_reward/td_error_clip 等参数与 Phase 3.1 summary 一致。
- 权重文件硬校验均通过：
  - `magic=0x5455504C`
  - `version=1`
  - `tuple_set_hash=0x1D4A6EBF2BAF6484`
  - `weight_count=16605`
  - 文件长度 `66444 = 24 + 16605 * 4`
  - header 后正好 `16605` 个 little-endian int32
- attribution summary 行数为 `27`，tuple detail 行数为 `1215`，每行 `active_tuple_count=45`。

### 输出文件
- `opening-child-attribution-summary.csv`
- `opening-child-attribution-tuples.csv`
- `opening-child-attribution-summary.json`
- `opening-child-attribution-top-deltas.csv`
- `attribution-manifest.json`

`attribution-manifest.json` 记录了 script path、script sha256、run root、output dir、seeds、stages、root moves、tuple_set_hash、weight_count、generated_at、git commit、input files、output files 和 row-count/hash/header checks。

### Static Eval 发现
- 三个 root child position 都是 Black quiet move 后的 White to move child；material score 均为 `0`。
- static eval 规则为 black-perspective `material + tuple_score`，White to move 时 `total_static_score=-black_perspective`。
- bootstrap 三 seed 相同：baseline child `-6`，cp2 drift child `-6`，final drift child `12`。这说明 bootstrap 自身并不支持 final drift child 优于 baseline child。
- cp2 阶段三 seed 相同：baseline child `-10`，cp2 drift child `-23`，final drift child `4`。cp2 drift child 相对 baseline child delta 为 `-13`，方向上对 Black 更优，能解释 cp2 drift 倾向。
- final 阶段 final drift child 相对 baseline child 的 static eval delta：
  - `20260423`: `64 -> -41`，delta `-105`
  - `20260424`: `-359 -> -497`，delta `-138`
  - `20260425`: `-214 -> -364`，delta `-150`
- final 阶段三个 seed 均让 final drift child 比 baseline child 对 Black 更优，方向上能解释最终 root drift 倾向。但这是 child-position static attribution，不等于 PVS 全宽 score，也不替代 Phase 3 gate。

### Tuple Delta Dominance
- 36 个 comparison 覆盖：
  - 同 seed / 同 root move：`cp2` vs `bootstrap`
  - 同 seed / 同 root move：`final` vs `bootstrap`
  - 同 seed / 同 stage：cp2 drift child vs baseline child
  - 同 seed / 同 stage：final drift child vs baseline child
- dominance heuristic 结果：`yes/watch/no = 0/0/36`。
- max top1 abs share 为 `0.165094`，max top3 abs share 为 `0.376866`。
- 结论：没有发现少数 tuple 主导 opening child static eval 变化；变化更像多 tuple 小幅累积。东北话讲，不是一颗螺丝把车干散架了，是一堆小松动凑一起开始响。

### 结论
- Phase 3.5 attribution-only execution 已完成。
- static eval delta 足以解释 cp2 drift child 和 final drift child 相对 baseline child 更容易被推前的方向。
- 现有证据足够进入 child-position static ablation 设计。
- 现有证据不足以直接进入 offline neutralization，也不足以解除 Phase 4 blocked。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-22 Phase 3.4 opening active-tuple attribution and minimal ablation design

### 本轮边界
- 本轮是设计/可行性分析，不是正式修复；未重新训练，未跑 `GamesPerSeed=64`，未进入 Phase 4。
- 未放宽 selection gate，未修改搜索参数、PVS、qsearch、aspiration、move ordering、LMR、Null Move。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未修改 `src\` / `include\` 生产代码；只读审计权重、评估、active tuple 和 CLI 代码。

### 输入复核
- 规划与路线图：`task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- Phase 3.3 诊断产物：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\opening-diagnostics-summary.md`
  - `opening-diagnostics-summary.csv`
  - `opening-root-moves-depth4-final-attempt.csv`
  - `opening-aspiration-attempts.csv`
- 结论未变：cp2 首次 drift 是 `(5,1)->(4,2)` 的 root order/PV tie-bound 翻转，非 aspiration retry；final 收敛到 `(3,1)->(2,2)`，qnodes 主要集中在当前 PV root move。
- 本轮接手复核补充：`weights-manifest.json` 中 3 seed 的 `bootstrap`、`checkpoint-2.bin`、`candidate` 目标权重均存在；实际读取 `20260423\checkpoints\checkpoint-2.bin` 确认文件长度 `66444` bytes，等于 `24 + 16605 * 4`。

### 权重与评估代码审计
- `include\surakarta_search.h` 定义 `NTupleWeightHeader` / `NTupleWeights` / `NTupleEvaluator`；已有 `NTupleWeights::EnumerateActiveWeightIndices`，但没有 CLI 输出入口。
- 权重二进制格式：
  - header 序列化字段为 `magic uint32`、`version uint16`、`reserved uint16`、`tuple_set_hash uint64`、`weight_count uint32`，实际文件头因结构体对齐为 `24` bytes。
  - 当前 `magic=0x5455504C`，`version=1`，`tuple_set_hash=0x1D4A6EBF2BAF6484`，`weight_count=16605`。
  - header 后是 `16605` 个 little-endian `int32` 权重；训练内部用 `double`，保存时 `QuantizeWeight` clamp 到 `[-4096,4096]` 并四舍五入。
- tuple set：
  - `src\surakarta_search.cpp::TupleDescriptors()` 固定 `45` 个 tuple，长度为 4 或 6。
  - `TupleOffsets()` 按每个 tuple 的 `3^length` span 累加，合计 `16605`。
  - `EncodeTuple(position, tuple)` 对 tuple 内 square 依次三态编码：empty `0`、black `1`、white `2`，低位优先累加 `state * factor`。
  - active weight index 为 `offsets[tuple_id] + encoded_code`；每个 position 固定 `45` 个 active tuple。
- material 与 tuple 合成：
  - `src\surakarta_bitboard.cpp::MaterialBalance(position)` 为 `(black_count - white_count) * 100`。
  - `EvaluateWeightsForSideToMove` / `NTupleEvaluator::Evaluate` 先以 black perspective 累加 `material + active tuple weights`，再按 side-to-move 翻转：Black to move 原样返回，White to move 返回负值。
  - opening 三个 root candidate 都是黑方 quiet move，child position 的 `child_side_to_move=White`；material 静态项预计仍为 `0`，tuple contribution 需要按 White side-to-move 取负后再比较。

### 是否能不改生产代码做 attribution
- **现有 CLI 单独不能。** `bitboard-search --search-diagnostics` 输出 root move / aspiration / qsearch diagnostics，不输出 material、active tuple ids、tuple contribution；`bitboard-eval` 也没有 explain-tuples 字段。
- **不改生产代码可以。** 因为权重格式、tuple descriptor、编码和 side-to-move perspective 都能从代码中确定，可以写一个非生产离线脚本解析 `.bin` 权重、构造 opening root child positions、枚举 active tuple 并输出 contribution。
- 这个方案的主要风险是脚本复制 `TupleDescriptors()`，后续 tuple set 改动时会漂；必须在脚本里强制校验 `tuple_set_hash`、`weight_count`、文件长度，并优先从 manifest 取 `weights_path`，不要手搓 checkpoint 路径。东北话讲，能离线掰明白，但别装成引擎亲口说的，差一层复刻风险。

### 最小 attribution 数据集
权重集合共 `9` 个：

| seed | stage | weights path |
|---|---|---|
| `20260423` | `bootstrap` | `build-local\training\20260522-172134\selection\seed-20260423\bootstrap.bin` |
| `20260423` | `cp2` | `build-local\training\20260522-172134\selection\seed-20260423\checkpoints\checkpoint-2.bin` |
| `20260423` | `final` | `build-local\training\20260522-172134\selection\seed-20260423\candidate.bin` |
| `20260424` | `bootstrap` | `build-local\training\20260522-172134\selection\seed-20260424\bootstrap.bin` |
| `20260424` | `cp2` | `build-local\training\20260522-172134\selection\seed-20260424\checkpoints\checkpoint-2.bin` |
| `20260424` | `final` | `build-local\training\20260522-172134\selection\seed-20260424\candidate.bin` |
| `20260425` | `bootstrap` | `build-local\training\20260522-172134\selection\seed-20260425\bootstrap.bin` |
| `20260425` | `cp2` | `build-local\training\20260522-172134\selection\seed-20260425\checkpoints\checkpoint-2.bin` |
| `20260425` | `final` | `build-local\training\20260522-172134\selection\seed-20260425\candidate.bin` |

Root child positions 共 `3` 个，均从默认 opening root 应用黑方 quiet move 得到：

| root_move label | move |
|---|---|
| `baseline_child` | `(0,1)->(1,2) (B)` |
| `cp2_drift_child` | `(5,1)->(4,2) (B)` |
| `final_drift_child` | `(3,1)->(2,2) (B)` |

预期输出规模：
- summary：`9 weights * 3 child positions = 27` 行。
- tuple detail：`27 * 45 = 1215` 行。

### 输出表字段设计
`opening-child-attribution-summary.csv`：
- `seed`
- `stage`
- `weights_path`
- `root_move`
- `child_side_to_move`
- `material_score`
- `tuple_score`
- `total_static_score`
- `active_tuple_count`
- `top_positive_tuple_deltas`
- `top_negative_tuple_deltas`
- `bootstrap_vs_stage_delta`
- `baseline_child_vs_drift_child_delta`
- `top_tuple_dominance`
- `dominance_notes`

`opening-child-attribution-tuples.csv`：
- `seed`
- `stage`
- `root_move`
- `child_side_to_move`
- `tuple_id`
- `tuple_squares`
- `tuple_offset`
- `tuple_code`
- `active_weight_index`
- `raw_weight_value`
- `signed_contribution`
- `bootstrap_raw_weight_value`
- `bootstrap_signed_contribution`
- `signed_delta_vs_bootstrap`
- `baseline_child_signed_contribution`
- `signed_delta_vs_baseline_child`

少数 tuple 主导判定建议只做 report-only heuristic：
- `top3_abs_delta_share >= 0.60` 或 `top1_abs_delta_share >= 0.35` => `yes`。
- `top3_abs_delta_share >= 0.40` => `watch`。
- 否则 `no`。

### 下一步可执行的离线脚本方案
建议下一阶段新增非生产诊断脚本，例如：

```powershell
pwsh -NoProfile -File .\tools\opening-attribution.ps1 `
  -RunRoot 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134' `
  -OutputDir 'D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_4_attribution' `
  -Seeds 20260423,20260424,20260425 `
  -Stages bootstrap,cp2,final `
  -RootMoves 'baseline:(0,1)->(1,2)','cp2:(5,1)->(4,2)','final:(3,1)->(2,2)'
```

脚本职责：
- 从 `weights-manifest.json` 或固定 run layout 解析 9 个权重路径。
- 读取 `.bin` 权重并校验 header。
- 构造默认 opening board，应用 3 个 root move 得到 child position。
- 输出 summary CSV、tuple detail CSV、JSON manifest 和 dominance report。

建议产物路径：
- `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_4_attribution\opening-child-attribution-summary.csv`
- `...\opening-child-attribution-tuples.csv`
- `...\opening-child-attribution-summary.json`
- `...\attribution-manifest.json`

### report-only CLI 备选设计
如果下一阶段要求 attribution 复用引擎内部 tuple code，不接受离线脚本复制 descriptor，则新增默认关闭的 report-only extension：

入口二选一：
- `surakarta-benchmark.exe bitboard-attrib --weights <weights.bin> --file <board-file> --root-move "0,1,1,2" --format json`
- 或 `bitboard-eval --explain-tuples --candidate <weights.bin> ...`

最小代码改动范围：
- `include\surakarta_search.h`：新增 attribution DTO / explain API 声明。
- `src\surakarta_search.cpp`：复用 `TupleDescriptors()`、`TupleOffsets()`、`EncodeTuple()` 实现 material / tuple explain；不触碰 `SearchController::Search`、PVS、qsearch、move ordering。
- `src\main.cpp`：增加 CLI parser 与 JSON 输出。
- `scripts\workflow.tests.ps1` 或 C++ selftest：只加 report-only 输出测试；不改 selection/perf gate。

建议 JSON schema：

```json
{
  "case_id": "opening",
  "weights_path": "...",
  "root_move": "(0,1)->(1,2) (B)",
  "child_side_to_move": "white",
  "material_score": 0,
  "tuple_score": 0,
  "total_static_score": 0,
  "active_tuple_count": 45,
  "tuples": [
    {
      "tuple_id": 0,
      "squares": [0, 1, 6, 7],
      "code": 42,
      "active_weight_index": 42,
      "raw_weight_value": 12,
      "signed_contribution": -12
    }
  ]
}
```

测试策略：
- 对 bootstrap/default opening 检查 `material_score + tuple_score == NTupleEvaluator::Evaluate(child_position)`。
- 对 Black/White side-to-move 各测一例，确认 sign flip。
- 对 3 个 root moves 确认 `active_tuple_count=45`。
- 对错误 magic/version/hash/count 的权重文件确认拒绝。
- CLI 默认不输出 attribution；只有显式 `bitboard-attrib` 或 `--explain-tuples` 才输出。

### 最小 ablation 设计
推荐顺序：

1. `attribution-only`：只报告 tuple contribution 和 delta，不改权重、不跑 search。优先级最高，当前就该先干这个。
2. `child-position static ablation`：在脚本内把 top-k stage-vs-bootstrap tuple delta 归零，比较 baseline child / drift child 的 static eval delta 是否回落；不写临时权重，不跑 search。
3. `offline neutralization`：仅当少数 tuple 明确主导时，复制 candidate weights 到 `opening-diagnostics\phase3_4_ablation\*.bin`，把 drift 相关 top-k tuple 的 stage-vs-bootstrap delta 归零，再只跑 `bitboard-search --case-id opening --search-diagnostics`。这一步仍不改生产权重、不改搜索参数。
4. `root-order controlled experiment`：现有 CLI 不支持固定 root order；本阶段记录为后续设计项，不改搜索。

### 风险判断
- 不要把 PVS bound-tie 当全宽精确分。Phase 3.3 已经说明 observed score rank 不能直接解释 root score 排序。
- 不要把 root order 问题误判为搜索 bug。当前证据更像训练权重扰动 opening root ranking 后，PVS/PV 子树成本被放大。
- 不要把 qnodes 高解释成必须调 qsearch。qnodes 是症状和成本定位，不是调参许可证。
- 不要把 tuple attribution 直接等同于棋力提升。它只能解释静态 eval 拉偏来源。
- 不要因为能解释 opening 就解除 Phase 4 blocked。Phase 3 仍未通过，Phase 4 仍 blocked。

### 结论
- Phase 3.4 设计/可行性分析完成。
- 可以不改生产代码完成第一版 active tuple attribution；现有 CLI 单独不够，需要离线小脚本。
- 若追求长期权威诊断，应做默认关闭的 report-only diagnostic extension，但这属于下一阶段可选项。
- 推荐最小 ablation 类型是 `attribution-only`，其次是 `child-position static ablation`；不要直接做 search 参数或 qsearch 调整。
- Phase 3 仍未通过；Phase 4 仍 blocked。

## 2026-05-22 Phase 3.3 opening root diagnostics and minimal attribution

### 本轮边界
- 本轮是只读 opening root diagnostics，不是修复；未重新训练、未跑 `GamesPerSeed=64`、未进入 Phase 4。
- 未修改搜索参数、训练算法、评估特征、selection gate 阈值。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`。
- 未修改 `src\` / `include\` 生产代码；仅使用现有 `bitboard-search --search-diagnostics`。

### 诊断 CLI 与产物
- `build-vs-train-release\bin\surakarta-benchmark.exe` usage 确认 `bitboard-search` 支持 `--weights`、`--depth`、`--threads`、`--format json`、`--case-id`、`--search-diagnostics`。
- `bitboard-search --help` 本身返回 `Unknown option: --help`；改用顶层 usage 和实际探针确认能力。
- usage 未列出 `--debug-search-tree`；现有 JSON 也没有 active tuple / weight attribution 字段。
- 诊断输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics`。
- 生成 per-weight JSON 共 `18` 个：每 seed 的 `bootstrap`、`cp2`、`cp4`、`cp6`、首次最终 move checkpoint、`final`。
- 汇总文件：
  - `opening-diagnostics-summary.csv`
  - `opening-root-moves-depth4-final-attempt.csv`
  - `opening-aspiration-attempts.csv`
  - `opening-diagnostics-summary.md`

### Root ranking 时间线摘要
说明：`order` 是 depth=4 final exact attempt 的搜索顺序，1-based；`score-rank` 是按 observed score 降序的 dense rank。PVS null-window 下非 PV move 的 score 经常是 bound，不等于全宽精确分。

| seed | stage | best move | baseline `(0,1)->(1,2)` | cp2 drift `(5,1)->(4,2)` | final drift `(3,1)->(2,2)` | 结论 |
|---|---|---|---|---|---|---|
| `20260423` | bootstrap | `(0,1)->(1,2)` | order `1`, score-rank `1` | order `15`, score-rank `1` | order `9`, score-rank `1` | bootstrap baseline PV |
| `20260423` | cp2 | `(5,1)->(4,2)` | order `2`, score-rank `1`, score `4` | order `1`, score-rank `1`, score `4` | order `9`, score-rank `1`, score `4` | 首次 drift 是 order/PV 翻转，不是明确 score 压低 |
| `20260423` | cp32/final | `(3,1)->(2,2)` | order `3`, score-rank `1`, score `-78` | order `15`, score-rank `1`, score `-78` | order `1`, score-rank `1`, score `-78` | final move 成为 PV；baseline 仍是 bound-tie |
| `20260424` | bootstrap | `(0,1)->(1,2)` | order `1`, score-rank `1` | order `15`, score-rank `1` | order `9`, score-rank `1` | bootstrap baseline PV |
| `20260424` | cp2 | `(5,1)->(4,2)` | order `2`, score-rank `1`, score `4` | order `1`, score-rank `1`, score `4` | order `9`, score-rank `1`, score `4` | 与另外两 seed 完全同型 |
| `20260424` | cp22 | `(3,1)->(2,2)` | order `11`, score-rank `1`, score `80` | order `15`, score-rank `1`, score `80` | order `9`, score-rank `1`, score `80` | final move 已 score-rank `1`，但 order 还不是第一 |
| `20260424` | final | `(3,1)->(2,2)` | order `2`, score-rank `1`, score `413` | order `15`, score-rank `1`, score `413` | order `1`, score-rank `1`, score `413` | final move 收敛为 PV root |
| `20260425` | bootstrap | `(0,1)->(1,2)` | order `1`, score-rank `1` | order `15`, score-rank `1` | order `9`, score-rank `1` | bootstrap baseline PV |
| `20260425` | cp2 | `(5,1)->(4,2)` | order `2`, score-rank `1`, score `4` | order `1`, score-rank `1`, score `4` | order `9`, score-rank `1`, score `4` | 与另外两 seed 完全同型 |
| `20260425` | cp14 | `(3,1)->(2,2)` | order `2`, score-rank `1`, score `128` | order `15`, score-rank `1`, score `128` | order `1`, score-rank `1`, score `128` | 最早收敛到 final move |
| `20260425` | final | `(3,1)->(2,2)` | order `2`, score-rank `1`, score `262` | order `15`, score-rank `1`, score `262` | order `1`, score-rank `1`, score `262` | final move 收敛为 PV root |

### qnodes / aspiration 归因
- cp2 首次 drift 三 seed 完全一致：total nodes/qnodes `1797/1277`，相对 bootstrap `1472/987` 增加 `+325/+290`；`aspiration_retries=0`。
- cp2 depth-4 root move：`(5,1)->(4,2)` nodes/qnodes `409/355`，baseline `(0,1)->(1,2)` 只有 `64/38`，final drift `(3,1)->(2,2)` 只有 `35/10`。膨胀集中在当前 PV root move。
- final candidate depth-4 root move：
  - `20260423`: `(3,1)->(2,2)` `412/358`，baseline `38/16`。
  - `20260424`: `(3,1)->(2,2)` `406/356`，baseline `38/16`。
  - `20260425`: `(3,1)->(2,2)` `541/440`，baseline `40/18`。
- 后期 aspiration retry 是放大器，不是最早起因：
  - cp2 `retry_qnodes=0`，解释不了首次 drift / qnode inflation。
  - `20260423 final` retry qnodes `163`，相对 bootstrap qnode delta `+136`。
  - `20260424 final` retry qnodes `720`，相对 bootstrap qnode delta `+509`。
  - `20260425 final` retry qnodes `233`，相对 bootstrap qnode delta `+498`。
  - 这些比例可超过 100%，说明“retry 成本”与“相对 bootstrap delta”不是可直接相加的因果百分比；它只能说明后期 retry 成本很重，不能倒推出最早 drift 是 retry 造成的。

### 必答结论
- **cp2 首次 drift 是不是 root score 排序已经翻转？** 不是明确的全宽 score 翻转。三 seed cp2 中 `(5,1)->(4,2)` 与 baseline observed score 都是 `4`、score-rank 都是 `1`；但 `(5,1)->(4,2)` order rank 从 bootstrap 的 `15` 升到 `1`，baseline 退到 `2`。这更像 root move ordering / PV tie-bound 翻转。要说“score 已经实锤压低 baseline”，证据不够，别硬吹。
- **`(5,1)->(4,2)` 和 `(3,1)->(2,2)` 分别排第几？** cp2 三 seed：`(5,1)->(4,2)` order rank `1`，`(3,1)->(2,2)` order rank `9`，二者 score-rank 都是 `1`。final candidate 三 seed：`(3,1)->(2,2)` order rank `1`；`(5,1)->(4,2)` order rank `15`；二者 score-rank 仍多为 bound-tie `1`。
- **baseline move 是被压低，还是接近第一但 tie-break 输？** cp2 是接近第一 / bound-tie 输掉 PV/order。cp4 的 `20260424/20260425` 出现过直接 score 差：baseline `0`，`(1,1)->(2,2)` 为 `2`。final candidate 里 baseline 仍是 score-rank `1` 的 bound-tie，但成为便宜 non-PV 子树；现有 diagnostics 不能证明 final 阶段 baseline 全宽 score 被压低。
- **node/qnode 膨胀主要集中在哪个 root move？** 主要集中在当前 PV root move。cp2 是 `(5,1)->(4,2)`；final candidate 是 `(3,1)->(2,2)`。个别中间点如 `20260424 cp22` 最大 qnodes 在 `(0,1)->(0,2)`，但 final drift 本身也已是高 qnodes 子树。
- **aspiration retry 能解释多少膨胀？** 解释不了 cp2 首次 drift，因为 cp2 retry 为 `0`。后期能解释大量额外成本，尤其 `20260424 final` 的 retry qnodes `720` 很重，但这是后期放大，不是 root instability 的第一触发。
- **三个 seed 是否同一种 root ranking 模式？** 早期 cp2 是同一种模式；中期分叉；后期又收敛到同一个 final move。模式不是每个 checkpoint 完全同步，但不是随机乱漂。
- **final candidate 共同 move 是否同源收敛？** 现有证据支持“opening root ranking 同源收敛”这一弱结论：三个 seed 都从同一 cp2 初始漂移开始，最终都把 `(3,1)->(2,2)` 推为 PV root。没有 active tuple / weight attribution，不能进一步实锤到具体 tuple 或权重局部拉偏。

### 下一步建议
- 支持做最小 ablation 的前置设计：只围绕 opening root choice / opening active tuple 做 report-only 诊断或 ablation 设计。
- 当前不支持直接调 `alpha/lambda/epsilon`、不支持放宽 perf gate、不支持改 search 参数、不支持进入 Phase 4。
- 如果要解释“哪个 tuple / 哪段权重把 opening active tuples 拉偏”，现有 CLI 证据不足；下一步应先写 report-only diagnostic extension design，不要立刻补生产代码。东北话讲，这时候上手改代码就是把显微镜当锤子，响是挺响，证据没了。

## 2026-05-22 Phase 3.2 opening blocker root-cause attribution

### 本轮边界
- 本轮是根因归因，不是修复；未重新训练、未开新 sweep、未跑 `GamesPerSeed=64`、未进入 Phase 4。
- 只读取既有 Phase 3.1 产物：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- 未修改 `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1`，未改搜索参数、训练算法、评估特征或 selection gate 阈值。

### 产物与追溯性复核
- `selection-summary.json`、`selection-summary.txt`、`weights-manifest.json` 均存在于 `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- 解析 checkpoint eval 文件共 `48` 个：3 个 seed × 16 个 checkpoint；与 `selection-summary.json` 内嵌 checkpoint scorecard 交叉校验，`mismatch_count=0`。
- `weights-manifest.json`：`manifest_version=1`，`purpose=selection`，`git_commit=6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，artifact count `54`，其中 `bootstrap=3`、`candidate=3`、`checkpoint=48`。
- 按 manifest 的 `weights_path` / `binary_sha256` 复核，`artifact_missing=0`，`hash_mismatch=0`。
- 参数与 Phase 3.1 指定值一致：`games=32`、`depth=4`、`checkpoint_every=2`、`alpha=0.005`、`lambda=0.5`、`epsilon=0.02`、`epsilon_plies=6`、`terminal_reward=1200`、`td_error_clip=0`、`terminal_only_warmup=0`、`near_terminal_curriculum=0`。

### 字段来源确认
- `scripts\benchmark-select.ps1` 中 `score_delta = candidate.score - baseline.score`，`best_move_changed = candidate.best_move -ne baseline.best_move`，`score_regressions` 统计 `score_delta < 0`。
- 同脚本中非 tiny 固定局面 `performance_ratio` 取 `search_seconds` ratio、`node_ratio` 和可用 `qnodes_ratio` 的最大值；tiny position 以 node/qnode ratio 为主，避免微秒计时误杀。
- `candidate_wins`、`baseline_wins`、`draws` 来自 `EvalReport.match_summary`，再聚合到 per-seed 和 overall scorecard。
- `src\main.cpp` 的 `BuildSearchReport` 将 `SearchResult` 的 `stats.nodes`、`best_move`、`score` 和实测 `search_seconds` 写入 `position_reports`；eval JSON 同时输出 `match_summary`。
- `src\surakarta_search.cpp` 中 `SearchResult.best_move`、`score`、`stats` 来自 root search；节点和 qnodes 在 search/qsearch 中累积；`EvaluateWeightsForSideToMove` 是 material + N-Tuple 权重并按 side-to-move 翻转。

### Checkpoint 时间线表
| seed | cp | rating | strength | perf | C/B/D | score_reg | bm_changes | opening_candidate | opening_baseline | opening_delta | search_ratio | node_ratio | perf_ratio |
|---|---:|---|---|---|---|---:|---:|---|---|---:|---:|---:|---:|
| `20260423` | 2 | `regress` | `watch` | `regress` | 5/4/1 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 4 | 1.442543 | 1.220788 | 1.442543 |
| `20260423` | 4 | `watch` | `watch` | `watch` | 5/4/1 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 1 | 0.959472 | 1.097147 | 1.097147 |
| `20260423` | 6 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(2, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 78 | 1.787208 | 1.903533 | 1.903533 |
| `20260423` | 8 | `regress` | `watch` | `regress` | 5/5/0 | 0 | 1 | `(4, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 232 | 1.642257 | 1.506114 | 1.642257 |
| `20260423` | 10 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(4, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 235 | 1.250591 | 1.322011 | 1.322011 |
| `20260423` | 12 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(4, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 330 | 1.763185 | 1.485054 | 1.763185 |
| `20260423` | 14 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(4, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 241 | 1.644186 | 1.492527 | 1.644186 |
| `20260423` | 16 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(4, 1) -> (5, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 102 | 2.928058 | 2.550951 | 2.928058 |
| `20260423` | 18 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(4, 1) -> (5, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 305 | 2.324324 | 2.023098 | 2.324324 |
| `20260423` | 20 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(4, 1) -> (5, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 319 | 2.345209 | 2.178668 | 2.345209 |
| `20260423` | 22 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 186 | 2.537760 | 2.030571 | 2.537760 |
| `20260423` | 24 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 327 | 2.312655 | 1.875679 | 2.312655 |
| `20260423` | 26 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 160 | 1.366834 | 1.424592 | 1.424592 |
| `20260423` | 28 | `regress` | `watch` | `regress` | 5/5/0 | 0 | 1 | `(3, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 69 | 1.685921 | 1.577446 | 1.685921 |
| `20260423` | 30 | `regress` | `regress` | `regress` | 4/5/1 | 1 | 1 | `(4, 1) -> (3, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -70 | 1.862934 | 2.377038 | 2.377038 |
| `20260423` | 32 | `regress` | `regress` | `regress` | 4/5/1 | 1 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -78 | 1.249097 | 1.343750 | 1.343750 |
| `20260424` | 2 | `regress` | `watch` | `regress` | 5/4/1 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 4 | 1.313239 | 1.220788 | 1.313239 |
| `20260424` | 4 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 2 | 1.450299 | 1.334239 | 1.450299 |
| `20260424` | 6 | `regress` | `regress` | `watch` | 4/5/1 | 0 | 0 | `(0, 1) -> (1, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 30 | 1.119658 | 1.025815 | 1.119658 |
| `20260424` | 8 | `watch` | `watch` | `watch` | 4/4/2 | 0 | 0 | `(0, 1) -> (1, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 26 | 1.103659 | 1.036685 | 1.103659 |
| `20260424` | 10 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 0 | `(0, 1) -> (1, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 12 | 1.348692 | 1.226223 | 1.348692 |
| `20260424` | 12 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 0 | `(0, 1) -> (1, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 18 | 1.382260 | 1.383152 | 1.383152 |
| `20260424` | 14 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 16 | 1.807939 | 1.504076 | 1.807939 |
| `20260424` | 16 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 17 | 1.393758 | 1.256793 | 1.393758 |
| `20260424` | 18 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 0 | `(0, 1) -> (1, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 14 | 1.942857 | 1.623641 | 1.942857 |
| `20260424` | 20 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 12 | 1.673177 | 1.486413 | 1.673177 |
| `20260424` | 22 | `regress` | `watch` | `regress` | 5/5/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 80 | 1.486293 | 1.413723 | 1.486293 |
| `20260424` | 24 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 147 | 1.668814 | 1.762908 | 1.762908 |
| `20260424` | 26 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 161 | 2.449328 | 2.039402 | 2.449328 |
| `20260424` | 28 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 108 | 2.378213 | 1.658288 | 2.378213 |
| `20260424` | 30 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 364 | 2.559567 | 1.769701 | 2.559567 |
| `20260424` | 32 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 413 | 1.801956 | 1.659647 | 1.801956 |
| `20260425` | 2 | `regress` | `watch` | `regress` | 5/4/1 | 0 | 1 | `(5, 1) -> (4, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 4 | 1.579812 | 1.220788 | 1.579812 |
| `20260425` | 4 | `regress` | `watch` | `regress` | 4/4/2 | 0 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 2 | 1.338631 | 1.334239 | 1.338631 |
| `20260425` | 6 | `regress` | `regress` | `watch` | 5/4/1 | 1 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -8 | 1.075995 | 0.985054 | 1.075995 |
| `20260425` | 8 | `regress` | `regress` | `watch` | 5/4/1 | 1 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -13 | 1.057841 | 0.983696 | 1.057841 |
| `20260425` | 10 | `regress` | `regress` | `watch` | 5/4/1 | 1 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -16 | 1.210000 | 1.090353 | 1.210000 |
| `20260425` | 12 | `regress` | `regress` | `watch` | 4/4/2 | 1 | 1 | `(1, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | -4 | 1.157113 | 1.148098 | 1.157113 |
| `20260425` | 14 | `watch` | `watch` | `watch` | 5/4/1 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 128 | 1.161049 | 1.158967 | 1.161049 |
| `20260425` | 16 | `regress` | `watch` | `regress` | 5/4/1 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 101 | 1.580574 | 1.567255 | 1.580574 |
| `20260425` | 18 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 341 | 1.707286 | 1.480299 | 1.707286 |
| `20260425` | 20 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 314 | 1.669523 | 1.489130 | 1.669523 |
| `20260425` | 22 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 213 | 1.308166 | 1.663043 | 1.663043 |
| `20260425` | 24 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 380 | 1.547914 | 1.553668 | 1.553668 |
| `20260425` | 26 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 234 | 1.645433 | 1.500679 | 1.645433 |
| `20260425` | 28 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 363 | 1.992054 | 1.690897 | 1.992054 |
| `20260425` | 30 | `regress` | `regress` | `regress` | 4/6/0 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 314 | 1.517382 | 1.654891 | 1.654891 |
| `20260425` | 32 | `regress` | `regress` | `regress` | 4/5/1 | 0 | 1 | `(3, 1) -> (2, 2) (B)` | `(0, 1) -> (1, 2) (B)` | 262 | 1.985770 | 1.694973 | 1.985770 |

### 首次事件
| seed | first drift | first final move `(3,1)->(2,2)` | first score_delta < 0 | first node_ratio > 1.25 | first search_ratio > 1.25 | first perf_rating=regress | first rating=regress | first watch/pass -> regress |
|---|---|---|---|---|---|---:|---:|---:|
| `20260423` | cp2 `(5, 1) -> (4, 2) (B)` | cp32 | cp30 `-70` | cp6 `1.903533` | cp2 `1.442543` | 2 | 2 | 6 |
| `20260424` | cp2 `(5, 1) -> (4, 2) (B)` | cp22 | none | cp4 `1.334239` | cp2 `1.313239` | 2 | 2 | 10 |
| `20260425` | cp2 `(5, 1) -> (4, 2) (B)` | cp14 | cp6 `-8` | cp4 `1.334239` | cp2 `1.579812` | 2 | 2 | 16 |

### 事件先后关系
- 三个 seed 在 checkpoint-2 就出现 opening best move drift，且最早都漂到 `(5, 1) -> (4, 2) (B)`，不是最终的 `(3, 1) -> (2, 2) (B)`。
- 三个 seed 在 checkpoint-2 同时出现 `search_ratio > 1.25`；稳定 `node_ratio > 1.25` 稍后出现：`20260423` 在 cp6，`20260424/20260425` 在 cp4。
- 最终候选 opening move `(3, 1) -> (2, 2) (B)` 出现更晚：`20260425` cp14、`20260424` cp22、`20260423` cp32。不能简单写成“最终 move 导致全部膨胀”；更准确是训练早期扰动 opening root move ordering，多个 drift move 都会进入更贵搜索树，最后三 seed 收敛到同一个 drift move。
- `20260423` 的最终 `score_delta=-78` 是晚期出现：cp30 `-70`、cp32 `-78`，不是从训练早期持续存在。
- `20260425` 的负向 score_delta 只在 cp6-cp12 出现，cp14 起恢复为正；`20260424` 全程没有 opening 负向 score_delta。
- `20260424/20260425` 最终 score_delta 为正但仍 `regress`，说明正向 fixed-position score 不能抵消 match strength blocker 和 perf blocker。

### 非 opening 局面对比
| case | rows | best_move_changes | score_regressions | max_search_ratio | max_node_ratio | max_perf_ratio |
|---|---:|---:|---:|---:|---:|---:|
| `capture-heavy` | 48 | 0 | 0 | 1.402363 | 1.004457 | 1.402363 |
| `endgame` | 48 | 0 | 0 | 1.468750 | 1.000000 | 1.000000 |
| `no-capture-critical` | 48 | 0 | 0 | 1.720000 | 1.000000 | 1.000000 |
| `threat-heavy` | 48 | 0 | 0 | 1.414286 | 1.000000 | 1.000000 |

- 四类非 opening 局面 48 个 checkpoint 全部没有 best move drift，也没有 score regression。
- `threat-heavy`、`endgame`、`no-capture-critical` 的 node ratio 全程 `1.0`；这些小局面即使 search_seconds ratio 偶发超过 1.25，也按 tiny-position 规则主要看 node/qnode，不构成当前主 blocker。
- `capture-heavy` 最大 node ratio 只有 `1.004457`，虽有个别 search_seconds ratio 抖动，但没有 tree inflation、best move drift 或 score regression。

### 根因归因结论
- Phase 3.1 `regress` 的主固定局面 blocker 是 opening fixed-position drift：baseline opening move 稳定为 `(0, 1) -> (1, 2) (B)`，最终三个 seed 均漂到 `(3, 1) -> (2, 2) (B)`。
- opening drift 跨 seed 一致，但不是一步到位；早期先漂到其他 candidate moves，后期才收敛到同一最终 move。
- search-tree inflation 与 opening root move drift 强相关：search timing 超阈与 drift 最早在 cp2 同步出现，node inflation 紧随其后；它主要跟随“candidate root move 改变”这类现象，而不是只跟随最终那个 `(3, 1) -> (2, 2)`。
- strength regress 和 perf regress 是耦合但不可互相替代的两个 blocker。证据是多个 checkpoint 出现 `strength=watch, perf=regress`，也有 `strength=regress, perf=watch`；最终三 seed 又同时表现为 C/B/D 落后和 opening performance 超阈。说白了，这不是一个指标挂了另一个指标没事，俩门都在报警，别拿一边的正分去糊另一边。
- 目前证据不足以支持直接调 `alpha/lambda/epsilon` 再跑；checkpoint 时间线只能证明 opening root choice 在训练早期即被扰动，还不能证明具体是哪一个 TD 参数导致。
- 证据支持先做 opening-specific 诊断/约束或最小 ablation，再考虑训练策略设计；不支持进入 Phase 4，不支持放宽 perf gate。

### 下一步建议
- 下一步应继续 opening-specific 诊断，并做最小 ablation，而不是直接回到大矩阵调参。
- 推荐诊断方向：固定 opening 局面对每个 checkpoint 输出 root move candidate 排序、root move 分数、nodes/qnodes/aspiration diagnostics；对比 active tuple 权重 delta 与 opening move 排序变化；必要时做只针对 opening active tuple / opening root choice 的最小 ablation。
- 在证明 root move drift 来源之前，不应进入 Phase 4 特征扩展；Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。

## 2026-05-22 Phase 3.1 fresh post-B1 GamesPerSeed=32 selection validation

### 预检结论
- 本轮读取了 `task_plan.md`、`findings.md`、`progress.md`、`docs\national_competition_technical_roadmap.md`。
- `git status --short` 仍显示既有 B1/B2.2 改动：多处源码、`task_plan.md`、`findings.md`、`progress.md`、`test/test_data/perf-baseline.json` modified，`docs/national_competition_technical_roadmap.md` untracked；本轮未提交、未回滚、未清理。
- 正式 baseline hash 复核通过：`test\test_data\perf-baseline.json` SHA256 为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- `build-vs-train-release\bin\surakarta-benchmark.exe` 存在。
- `scripts\perf-runner.ps1`、`scripts\benchmark-select.ps1`、`scripts\benchmark-train.ps1` 无本轮 diff；未修改 selection gate 阈值。

### 执行与产物
- 初次按用户给出的外层 `pwsh ./scripts/benchmark-select.ps1 ... -Seeds 20260423,20260424,20260425` 形式调用时，`Seeds` 被传成单个字符串，PowerShell 参数绑定失败；该失败发生在脚本参数转换阶段，未生成 session，不构成 selection run。
- 正式执行使用同一参数值，通过 `pwsh -NoProfile -Command "& './scripts/benchmark-select.ps1' ... -Seeds @(20260423,20260424,20260425) ..."` 显式传数组，脚本 exit `0` 并输出 `[PASS] benchmark selection scorecard`。这里的 `[PASS]` 只代表脚本产物链路完成，不代表趋势通过。
- 新 training session root：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134`。
- Summary 路径：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\selection-summary.json`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\selection-summary.txt`
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\weights-manifest.json`

### Selection 结果
| 指标 | 结果 |
|---|---|
| overall_rating | `regress` |
| strength_rating | `regress` |
| perf_rating | `regress` |
| scorecard_pass / watch / regress | `0 / 0 / 3` |
| candidate_wins / baseline_wins / draws | `12 / 16 / 2` |
| score_regressions | `1` |
| best_move_changes | `3` |
| average_performance_ratio | `1.622708` |
| worst_performance_ratio | `1.864728` |

### Per-seed 结果
| seed | rating | strength_rating | perf_rating | C/B/D | score_regressions | best_move_changes | max_perf | blocker_reason |
|---|---|---|---|---|---:|---:|---:|---|
| `20260423` | `regress` | `regress` | `regress` | `4/5/1` | 1 | 1 | `1.343750` | candidate wins 4 < baseline wins 5；1 fixed position score regression；performance ratio 1.344 > 1.25 |
| `20260424` | `regress` | `regress` | `regress` | `4/6/0` | 0 | 1 | `1.659647` | candidate wins 4 < baseline wins 6；performance ratio 1.660 > 1.25 |
| `20260425` | `regress` | `regress` | `regress` | `4/5/1` | 0 | 1 | `1.864728` | candidate wins 4 < baseline wins 5；performance ratio 1.865 > 1.25 |

### 固定局面 drift / 性能归因
- `opening` 是唯一 blocker 类别。三个 seed 均发生 best move drift：baseline 为 `(0, 1) -> (1, 2) (B)`，candidate 均为 `(3, 1) -> (2, 2) (B)`。
- `opening` 负向 score regression 出现在 `20260423`：score delta `-78`。`20260424` / `20260425` score delta 分别为 `+413` / `+262`，但不抵消 gate，因为胜负与性能仍回退。
- `opening` node ratio 分别为 `1.343750`、`1.659647`、`1.694973`，均超过 `1.25`；`20260425` opening search_seconds ratio 为 `1.864728`，`20260423` 为 `1.280226`。
- `capture-heavy` 未出现 best move drift 或 score regression；`20260425` node ratio `1.004457`，不构成异常。
- `threat-heavy`、`endgame`、`no-capture-critical` 未出现 best move drift、score regression 或 node inflation；tiny position 主要按 node ratio 判定，均为 `1.0`。

### Manifest / hash / 参数追溯
- `weights-manifest.json`：purpose=`selection`，git commit `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- artifact 总数 `54`：bootstrap `3`、candidate `3`、checkpoint `48`。
- candidate hashes：
  - `20260423`: `786FABDE37039A19BA51728F0A0DDA21342C5257F4E937D0EF1A96C52E2CD526`
  - `20260424`: `EF1A033C1545F50F839434890E64F5666B99C03350D658AD77025C3119FA05FF`
  - `20260425`: `E768CCDC97ED8CBA469B14680467F09D0AA96CA312872FA1B0989EF44680EACA`
- 三个 bootstrap hash 均为 `D87852D8E639182E479F4756FCFE63A1FDA992C01C163E6FC9D5C2D228F3AC51`。
- 所有 artifact 文件存在，manifest 内 SHA256 与实际文件 hash 匹配；必需字段和 checkpoint 元数据完整；summary 参数与本轮指定参数一致，无 seed / 参数 mismatch。

### 判定
- `GamesPerSeed=32` 已完成，但结果是明确 `overall_rating=regress`。
- 同时存在 `strength_rating=regress`、`perf_rating=regress`、负向 score regression、opening best move drift 和 opening search-tree inflation。
- 按规则立即停止，不跑 `GamesPerSeed=64`；当前只允许汇报 blocker，不允许进入下一轮 64 评审。Phase 3 继续 `implementation complete, trend validation failed`；Phase 4 继续 blocked。

## 2026-05-22 阶段 A 接手复核

### 结论
- 接手后重新验证 `D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321`：run root 存在，`checklist.md`、`commands.txt`、`git-commit.txt`、`result.txt`、`screenshots\` 均存在，缺失文件数为 0。
- `result.txt` 包含 `Manual acceptance result: PASS`，且不再包含 `PENDING`。
- `checklist.md` 无空 `Actual:` / `Conclusion:` 字段，6 个正式场景均有人工观察结论。
- 必需截图 `opening-move.png`、`game1-targets.png`、`game6-checkmate.png`、`game6-stalemate.png` 均存在且非空；旁证截图 `game1-capture-gate.png`、`game2-white-capture-a.png`、`game2-white-capture-b.png` 也存在且非空。
- `src/`、`include/`、`scripts/`、`test/`、`README.md`、`docs/school_competition_defense.md` 无本轮 diff；`src/surakarta_gui_main.cpp` 无 diff。没有留下鼠标注入、`SendInput`、`PostMessage` 或 GUI acceptance-only 自动动作。

### 边界
- 阶段 A 仍可判完成，可以切入阶段 B：大赛终局语义与规则完整性。
- 阶段 B 尚未开始；本轮没有修改终局语义、搜索、训练、特征或 selection gate。
- Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 仍 blocked。

## 2026-05-21 阶段 A manual acceptance 封口

### 结论
- 本轮已按人工 manual acceptance 正路完成校赛冻结证据包，不再尝试鼠标注入、`SendInput`、`PostMessage`，也没有给 GUI 加 acceptance-only 自动动作。此前短暂准备的 GUI 自动场景代码已撤回，`src/surakarta_gui_main.cpp` 无内容 diff。
- 正式归档目录：`D:\Dev\Code\surakarta_traditional\build-local\manual-acceptance\20260521-231321`。
- `scripts/manual-acceptance.ps1 -Depth 10 -Threads 10` 生成归档 scaffold 并启动 GUI。第一次运行因旧 `surakarta-gui.exe` 进程占用输出文件导致 `LNK1104`，关闭旧 GUI 后原命令重跑成功。
- `checklist.md` 已填写每个场景的 `Actual` / `Conclusion`；`result.txt` 已从 `PENDING` 更新为 `Manual acceptance result: PASS`。
- 必需归档内容齐全：`checklist.md`、`commands.txt`、`git-commit.txt`、`result.txt`、`screenshots\`。
- 必需截图齐全：`opening-move.png`、`game1-targets.png`、`game6-checkmate.png`、`game6-stalemate.png`；额外保留 `game1-capture-gate.png`、`game2-white-capture-a.png`、`game2-white-capture-b.png` 作为场景 3/4 旁证。

### 场景事实
| 场景 | 结果 | 证据 |
|---|---|---|
| Opening Quiet Move | PASS | `opening-move.png` 显示人类走子后引擎继续响应，D10/T10 搜索统计落在 GUI 侧栏 |
| `game1.txt` Legal And Illegal Targets | PASS | `game1-targets.png` 显示加载局面、选择 `(2,2)`、合法目标列表/高亮；用户完成并确认 |
| `game1.txt` Capture Gate | PASS | 合法 capture 成功；重载后非法 `(4,5)->(5,1)` 不执行；`game1-capture-gate.png` 保留旁证 |
| `game2.txt` White Capture Paths | PASS | Human White 下两条白方 capture path 均接受；`game2-white-capture-a.png` / `game2-white-capture-b.png` 保留旁证 |
| `game6.txt` Terminal Capture | PASS | `game6-checkmate.png` 显示 `Game Ended: CHECKMATE` |
| `game6.txt` legacy no_capture_limit gate | PASS | `game6-stalemate.png` 显示 `Game Ended: STALEMATE` |

### 边界
- 阶段 A 校赛冻结证据链可判完成，可以进入阶段 B：大赛终局语义与规则完整性。
- 阶段 B 尚未开始；本轮没有修改终局语义、搜索、训练、特征或 selection gate。
- `game6.txt` 的 `STALEMATE` 仍是 legacy `no_capture_ply / max_no_capture_round` 手工门禁，不等于大赛第 6 条“双方都不能再吃子”的严格语义证明。
- Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 仍 blocked；旧 suspect/provisional 训练产物仍不能作为强度证据。

## 2026-05-21 校赛冻结证据链与终局语义复核

### 复核结论
- 本节为阶段 A 封口前的复核结论；已被上方 `20260521-231321` manual acceptance 归档更新。当前不再是“没有归档”，正式 PASS 证据包已经落盘。
- `scripts/manual-acceptance.ps1` 生成的是 scaffold：会创建 `checklist.md`、`commands.txt`、`git-commit.txt`、`result.txt`，其中 `result.txt` 初始内容为 `PENDING`。本轮已人工完成 checklist、补齐截图，并将 `result.txt` 更新为 `PASS`。
- P0 capture-rule golden 已经足够证明之前的漏吃子回归修复到位：`TestCaptureVariantOverflowGolden`、`TestIllegalCaptureVariantBlocked`、`TestRuleCorrectnessGoldenParity`、`TestCaptureVariantCapacityInvariant`、`TestSearchDepthOneSeesGoldenCapture`、`TestCaptureMakeUnmakeAndNoCaptureReset` 都在，足以作为 capture-rule regression 的证据。
- 但 P0 golden 不等于大赛终局语义证据。当前 production 终局仍是 legacy `no_capture_ply / max_no_capture_round` 计数，`JudgeEnd`、`EvaluateBitboardTerminal`、`EvaluateTerminal` 都还没有把“双方都不能再吃子”做成状态级门禁。
- 仓库里已经有 `SurakartaPieceCanCaptureUtil::CanCaptureOpponentPiece(PieceColor)`，说明“判断某方是否还能吃子”的能力并不缺；缺的是把它接进终局语义和相关 golden case。
- 因此现在不能把阶段 A 当封口完成。下一步应先补 manual acceptance 归档证据，然后再进入大赛终局语义的最小代码修复与 parity 测试补强。

## 2026-05-21 全项目复核与国赛路线图

### 本轮需求复述
- 用户要求对整个项目做全面检查，更新“记忆文件”和“路线规划文件”，并生成一份完整总纲：记录项目规划、当前开发现状、未来优化方向，以及从校赛版本通往国赛棋力水平的分阶段技术路线。
- 本轮任务性质是文档与规划，不是生产代码改动；因此不改搜索、训练、棋规、GUI 逻辑，不放宽 gate。

### 当前真实定位
- 当前仓库是 `D:\Dev\Code\surakarta_traditional`，工作区检查前为 clean。
- 项目已经不是“从零实现 AI”的状态。已具备 bitboard 表示、静态/动态 mask、Surakarta 圆弧吃子 variants、PVS/Alpha-Beta、iterative deepening、aspiration window、TT、qsearch、move ordering、killer/history、LMR、Null Move、Lazy SMP 根并行、N-Tuple 评估、TD/self-play 训练 CLI、multi-seed selection、Dear ImGui GUI、Undo Human Turn、CTest/Pester workflow。
- 当前校赛版本定位仍是：以 `bitboard + fixed-depth PVS + 多线程根搜索 + GUI Match preset D10/T10` 为核心的可用对战版本。
- 当前长期国赛路线不能从 Phase 4 特征扩展直接开干。Phase 3 的训练/selection 仍是 `implementation complete, trend validation failed`，Phase 4 继续 blocked。

### 自动检查结果
| 检查项 | 命令/入口 | 结果 |
|---|---|---|
| bitboard 自测 | `build\bin\surakarta-bitboard-selftest.exe` | `[PASS] surakarta-bitboard-selftest` |
| dev-session 自测 | `build\bin\surakarta-dev-session-selftest.exe` | `[PASS] surakarta-dev-session-selftest` |
| Pester workflow | `Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet` | `26 passed / 0 failed` |
| CTest | `Invoke-SurakartaCTest -BuildDir build` | `8/8 passed` |

### 关键风险判断
- **规则风险已缓解但不能忘：** P0 capture-rule regression 已修复，`kMaxCaptureVariants=8`、capacity invariant、fail-fast、golden parity 都在；但这只能证明当前规则底座更稳，不等于训练趋势通过。
- **终局语义仍有 debt：** legacy `max_no_capture_round` / `no_capture_ply` 不是大赛第 6 条“双方都不能再吃子”的严格实现。后续若要冲国赛，必须单独设计终局语义 golden cases，不能拿旧 no-capture 计数糊弄。
- **训练闭环还没站稳：** 历史 GamesPerSeed=32/64 selection 仍 `regress`，opening search-tree inflation 是核心 blocker；P0 之后的可信训练基线需要重建。
- **校赛和国赛目标要分层：** 校赛优先稳定可演示和可操作；国赛要靠规则完备、可复现实验、稳定评估函数、搜索时间管理、长期 ladder 评测一层层顶上去。

### 路线图决策
| 决策 | 理由 |
|---|---|
| 新增独立国赛路线图文档 | 现有 `school_competition_defense.md` 偏答辩素材，不适合作为长期优化总纲 |
| 保留 Phase 4 blocked 判断 | 当前训练趋势未通过，硬扩特征就是把不稳定放大 |
| 先重建 post-fix baseline，再重启训练 sweep | P0 修复前后的训练产物不能混用，否则证据链脏了 |
| 国赛路线分为规则底座、基线、训练、特征、搜索、评测发布六大阶段 | 这样每一阶段都有验收口径，不会写成空泛口号 |

### 新增/更新文件
- 新增：`docs/national_competition_technical_roadmap.md`
- 更新：`task_plan.md`
- 更新：`findings.md`
- 更新：`progress.md`

## 需求
- 用户要的不是继续实现，而是基于现有仓库，对照“方案 B：战术绞肉机”蓝图给出下一阶段任务。
- 任务必须基于真实代码现状，不能把已完成能力重新包装成“下一阶段”。
- 输出应能直接指导 Phase 4 后续实施与优先级排序。

## 研究发现
- 当前仓库真实根目录是 `D:\\Dev\\Code\\surakarta_traditional`，不是旧路径里的 `surakarta-core-main` 子目录。
- 搜索层已经具备较完整骨架：
  - `SearchLimits` 已支持 `max_depth`、`threads`、`aspiration_window`、`node_limit`、`enable_null_move`、`enable_lmr`
  - `SearchStats` / `SearchSnapshot` 已有性能与剪枝统计字段
  - `SearchController` 已实现 TT、迭代加深、aspiration windows、null move、LMR、PV、Lazy SMP 风格根并行
  - `NTupleEvaluator` 已存在，且支持从外部权重文件加载
- 位棋盘走法生成已经是预计算导向：
  - `include/surakarta_masks.h` 内含静态 `BitboardTables`
  - `surakarta_bitboard.cpp` 里有 `BitboardGenerator::Build()` 动态构建逻辑，默认使用静态表
  - 圆弧吃子合法性、路径重构、capture variants 都已预计算
- 基准与性能脚本已经存在：
  - `main.cpp` 提供 `bitboard-search` / `bitboard-benchmark` CLI
  - `scripts/perf-runner.ps1` 会跑 opening/middlegame/endgame、depth 6/7/8、threads 1/2/4
- 已有性能基线不是嘴炮，是落了盘的：
  - `build-local/perf/20260423-114445/perf-summary.txt` 显示 middlegame depth 8 时 1T 约 `2.72M NPS`，4T 约 `7.29M NPS`
  - opening 深度 8 的多线程缩放一般，2T/4T 对 wall-clock 提升不稳定
  - 极小 endgame 局面多线程明显赔本，线程管理开销远大于搜索量
- 当前仓库已经具备 TD/self-play 训练闭环，但还处在早期可观察阶段：
  - `RunBitboardTraining` 支持 `alpha`、`lambda`、`epsilon`、`epsilon_plies`、seed、checkpoint、权重输入/输出
  - `benchmark-select.ps1` 已提供多 seed selection 入口，能汇总 candidate vs bootstrap 的胜负、固定局面 best_move/score、nodes/search_seconds 与 pass/watch/regress
  - 新增小节点固定局面性能判定修正：节点数低于 500 时用 `node_ratio` 避免微秒级耗时噪声误杀
  - 现有长训练观察显示 `alpha=0.0035, lambda=0.5, epsilon=0.05` 在 `GamesPerSeed=32/64` 下仍不稳定，candidate 胜负随训练局数增加恶化
- 搜索自测护栏已经比较扎实：
  - `surakarta_bitboard_selftest.cpp` 覆盖了搜索 parity、共享 node budget、共享 stop request、多线程等价、PV 合法性、aspiration 统计、benchmark JSON 输出、静态 mask parity

## 技术决策
| 决策 | 理由 |
|------|------|
| 下一阶段不再把“实现 PVS/TT/LMR/Null Move”列为主任务 | 这些能力代码里已经有了，再写就是自欺欺人 |
| 下一阶段重点应转向“强度提升闭环”而非“搜索框架从零搭建” | 现在最大的价值在于调优、训练、验证，而不是重复搭骨架 |
| 下一阶段必须在“搜索工程提纯”与“TD 训练闭环”之间明确主次 | 两者都重要，但先后顺序不同，任务清单会完全不同 |
| 长期路线先补训练质量，再做搜索增强 | 当前搜索骨架够用，训练权重还不能稳定优于 bootstrap，贸然加搜索花活会放大评估问题 |

## 遇到的问题
| 问题 | 解决方案 |
|------|---------|
| 用户蓝图描述与仓库实际状态有错位 | 通过直接阅读 `surakarta_search.*`、`surakarta_bitboard.*`、`perf-runner.ps1` 做现状校正 |

## 资源
- `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_search.h`
- `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_search.cpp`
- `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_masks.h`
- `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard.cpp`
- `D:\\Dev\\Code\\surakarta_traditional\\scripts\\perf-runner.ps1`
- `D:\\Dev\\Code\\surakarta_traditional\\src\\main.cpp`

## 视觉/浏览器发现
- 本轮未使用浏览器或图像工具。

## 2026-05-02 阶段 3 regress triage

### Selection 公平性
- 最新验收目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260502-113208`。
- `GamesPerSeed=32`，`Depth=4`，`CheckpointEvery=2`，`alpha=0.01`，`lambda=0.7`，`epsilon=0.1`，`epsilon_plies=6`。
- `bitboard-eval` 对 candidate 与 baseline 使用同一个 `SearchLimits`，并经 `NormalizeSingleThreadLimits` 强制 `threads=1`；没有 time 参数参与 selection，只有 depth/threads/node_limit 等 SearchLimits 字段。
- 每个固定局面都会跑 candidate 黑方、candidate 白方各一局，颜色对称；本轮 5 个 case 共 10 局/seed，不存在明显先后手只给 candidate 或 baseline 的偏置。
- `candidate_wins` / `baseline_wins` 方向未发现写反：`PlayEvaluationMatchGame` 根据 `winner == candidate_color` 记 `candidate_win`，否则记 `baseline_win`。
- fixed position score regression 的符号未发现整体写反：score 是同一 side-to-move 下的 search score，selection 用 `candidate_score - baseline_score < 0` 判回退；本轮 opening 在 20260423 为 `-125`，但 20260424/20260425 分别为 `+117` / `+149`，不是全局反号。
- performance ratio 是 `candidate / baseline`，非 tiny position 使用 `max(search_seconds ratio, node ratio)`，`>1.25` 判 regress；tiny position 节点数低于 500 时使用 `node_ratio_tiny_position`。

### 训练退步证据
- candidate 从当前 seed 的 `bootstrap.bin` 初始化：`benchmark-select.ps1` 先用 `bitboard-train --games 0` 导出 bootstrap，再用 `--weights bootstrap.bin` 训练 candidate。
- checkpoint 级别最早从 `checkpoint-2` 就开始 regress。`20260423/20260425` 的 checkpoint-2 candidate wins 领先但因性能比 regress；`20260424` 的 checkpoint-2 胜负持平但性能比 regress。到 checkpoint-4 起已经出现胜负或 score/best move 退步。
- weight manifest 中已有 `alpha` / `lambda` / `epsilon` / `epsilon_plies` / `games` / `depth` / `seed`；`GamesPerSeed` 以 `games` 表达，`CheckpointEvery` 只在 `selection-summary.json` 有 `checkpoint_every`，未写入单个 weight manifest artifact。
- TD error / weight delta 没有 NaN 或崩到 clamp，但幅度偏激：最终三 seed `avg_abs_td_error` 约 `70.89-74.62`，`max_abs_td_error` 到 `1815.36`；`avg_abs_weight_delta` 约 `31.26-33.03`，`max_abs_weight_delta` 到 `739.33`。以 `alpha=0.01` 看，这是训练参数偏猛的信号。
- 终局类型没有集中在异常终局：三 seed 都是 checkmate 为主（29-30/32），no-capture 2-3，no-legal/ply-cap 为 0；没看到 no-legal 或 ply-cap 爆炸。

### Opening best move 漂移
- baseline opening 三个 seed 均为 `(0, 1) -> (1, 2) (B)`，score `0`，nodes `1472`。
- seed `20260423` candidate opening：best move `(3, 1) -> (2, 2) (B)`，score `-125`，nodes `1554`，time `0.000870s`；baseline time `0.000828s`。
- seed `20260424` candidate opening：best move `(4, 1) -> (3, 2) (B)`，score `117`，nodes `1841`，time `0.001259s`；baseline time `0.000843s`。
- seed `20260425` candidate opening：best move `(1, 1) -> (2, 2) (B)`，score `149`，nodes `1397`，time `0.000764s`；baseline time `0.000867s`。
- 三个 seed 的 opening best move 全部漂移，但 score delta 有负有正；更像评估函数被 TD 权重真实拉偏，而不是固定局面 case / side-to-move / score perspective 整体反号。

### 20260424 性能复跑
- 对 `20260424` final candidate/baseline 重跑 `bitboard-eval` 5 次。
- opening：节点稳定为 `1841/1472`，node_ratio `1.250679`，search_ratio 约 `1.26-1.41`，平均 `1.32`；这是真实性能/搜索树退步信号，不是纯计时噪声。
- threat-heavy：节点稳定为 `548/548`，node_ratio `1.0`，search_ratio 约 `0.74-1.05`，平均 `0.97`；原始 `1.62585` 属于亚毫秒计时噪声，之前因为节点数 548 略高于 tiny-position 500 门槛，仍进入 `max_search_or_node_ratio` 而触发 regress。

### 结论建议
- 不是 selection 统计方向 bug，也不是 fixed-position score perspective 全局 bug。
- 不是 PVS / Null Move / LMR / move ordering 的调参问题，本轮不应碰搜索参数。
- 主要结论是训练本身确实退步，并且 `alpha=0.01, lambda=0.7, epsilon=0.1` 对当前 TD 更新偏激；TD 更新/权重写入未发现硬 bug，但 weight delta 峰值过大，需要作为下一轮根因重点。
- Phase 4 必须继续 blocked，直到 GamesPerSeed=32 不再 regress。现在硬往特征扩展走，就是把没拧紧的轮子装跑车上，挺热闹但不解决问题。

## 2026-05-02 Phase 3 training stabilization sweep

### Manifest 修复
- 单个 artifact manifest 已补 `checkpoint_every` 字段，覆盖 `bootstrap`、`candidate`、`checkpoint`。
- Pester 已覆盖 `benchmark-train.ps1` 与 `benchmark-select.ps1` 的 sidecar manifest：`Invoke-Pester -Script scripts/workflow.tests.ps1 -PassThru -Quiet` 结果 `17 passed / 0 failed`。
- 实测 sweep 产物中 candidate manifest 包含 `checkpoint_every=2`，例如 `phase3-stability-sweep-20260502-115207/a0p001_l0p3_e0p02/selection/seed-20260423/candidate.bin.manifest.json`。

### Sweep 设置
- sweep 目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-stability-sweep-20260502-115207`。
- 矩阵：`alpha = 0.001, 0.0025, 0.005`；`lambda = 0.3, 0.5`；`epsilon = 0.02, 0.05`；seeds = `20260423, 20260424, 20260425`；`GamesPerSeed=32`；`CheckpointEvery=2`。
- selection 判级未放宽，仍按 candidate/baseline 胜负、fixed score regression、performance ratio 判 `pass/watch/regress`。

### Sweep 汇总
| 参数 | rating | C/B/D | opening drift | avg/max opening node ratio | score regressions | earliest regress | avg/max weight delta |
|------|--------|-------|---------------|----------------------------|-------------------|------------------|----------------------|
| a=0.001 l=0.5 e=0.05 | regress | 17/13/0 | 2/3 | 1.305933 / 1.441576 | 3 | 2 | 3.078693 / 54.549169 |
| a=0.0025 l=0.3 e=0.02 | regress | 17/13/0 | 2/3 | 1.693614 / 1.833560 | 3 | 2 | 7.294747 / 140.974217 |
| a=0.001 l=0.3 e=0.02 | regress | 17/13/0 | 3/3 | 1.066802 / 1.169837 | 3 | 2 | 3.018361 / 60.815844 |
| a=0.0025 l=0.5 e=0.02 | regress | 15/14/1 | 2/3 | 1.247056 / 1.355299 | 1 | 2 | 7.126993 / 158.694278 |
| a=0.001 l=0.3 e=0.05 | regress | 16/14/0 | 3/3 | 1.147871 / 1.209918 | 2 | 2 | 2.819185 / 53.699720 |
| a=0.005 l=0.5 e=0.02 | regress | 15/15/0 | 2/3 | 1.236639 / 1.350543 | 0 | 4 | 14.647395 / 311.331578 |
| a=0.001 l=0.5 e=0.02 | regress | 13/15/2 | 3/3 | 1.265851 / 1.366848 | 2 | 2 | 2.874072 / 55.101383 |
| a=0.005 l=0.3 e=0.05 | regress | 13/16/1 | 1/3 | 1.574049 / 2.301630 | 0 | 2 | 14.759791 / 325.820476 |
| a=0.0025 l=0.5 e=0.05 | regress | 13/16/1 | 2/3 | 1.244112 / 1.298234 | 0 | 2 | 6.968307 / 157.408408 |
| a=0.005 l=0.5 e=0.05 | regress | 13/16/1 | 3/3 | 1.415987 / 1.699728 | 0 | 4 | 14.699810 / 340.570291 |
| a=0.005 l=0.3 e=0.02 | regress | 13/17/0 | 3/3 | 1.533514 / 2.112092 | 0 | 2 | 13.419176 / 328.419467 |
| a=0.0025 l=0.3 e=0.05 | regress | 13/17/0 | 3/3 | 1.569746 / 1.862772 | 1 | 2 | 7.741415 / 145.551346 |

### 最稳参数与判断
- 全部 12 组仍为 regress，没有一组达到 watch，更别提 pass。
- `alpha=0.005, lambda=0.5, epsilon=0.02` 是最稳候选：`15/15/0`，fixed score regressions 为 `0`，opening drift 为 `2/3`，earliest regress 延后到 `checkpoint-4`，weight delta 比原始 `0.01/0.7/0.1` 明显降低。
- `alpha=0.001, lambda=0.3, epsilon=0.02` 的 opening node ratio 最好（avg/max `1.066802/1.169837`）且胜负 `17/13/0`，但 3 个 seed 都有 fixed score regression；在当前 selection 规则下不是更稳，只是另一个方向的症状更轻。
- 已将默认训练参数改为 `alpha=0.005, lambda=0.5, epsilon=0.02`，但这不是验收通过，只是避免默认继续使用已确认过激的参数。

### 根因倾向
- lower alpha 让 weight delta 近似按比例下降，说明权重 write/read 链路大概率不是主因；如果读写错了，参数缩放不会这么规整。
- 所有组合仍在 checkpoint-2 或 checkpoint-4 过早 regress，说明问题不只是 `alpha=0.01` 太大；reward scaling、TD target 与 side-to-move perspective 的组合仍有嫌疑。
- opening best move/score 对极小权重更新也敏感，且低 alpha 仍出现 fixed score regression，下一轮应优先审 TD 更新公式与 reward scaling，而不是扩特征或调搜索。

## 2026-05-03 Phase 3 learning signal isolation

### 隔离实验结果
- Lambda ablation：
  - `alpha=0.005, lambda=0.0, epsilon=0.02, GamesPerSeed=32`：overall `regress`，C/B/D=`13/17/0`，opening drift `3/3`，avg/max weight delta `17.505413 / 411.202624`。
  - `alpha=0.0025, lambda=0.0, epsilon=0.02, GamesPerSeed=32`：overall `regress`，C/B/D=`14/16/0`，opening drift `3/3`，avg/max weight delta `8.235030 / 151.602491`。
  - 结论：`lambda=0` 没救回趋势，eligibility trace 传播不是唯一主因。
- Exploration ablation：
  - `alpha=0.005, lambda=0.5, epsilon=0.0`：overall `regress`，C/B/D=`12/18/0`，score regressions `0`，best move changes `0`，opening drift `0/3`。
  - `alpha=0.005, lambda=0.0, epsilon=0.0`：overall `regress`，C/B/D=`12/18/0`，score regressions `0`，best move changes `3`，opening drift `3/3`。
  - 结论：epsilon 噪声会放大 opening drift，但不是全部退步根因。
- No-training baseline：
  - `alpha=0` 下 candidate/bootstrap hash 完全一致，fixed score/move drift 为 `0`，opening best move 不漂，match C/B/D=`5/5/0`。
  - 结论：artifact / bootstrap / selection 链路干净，排除 E。

### TD trace 摘要
- 新增 `bitboard-trace` CLI 输出 per-step JSON/CSV，当前 trace 文件：
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-learning-signal-isolation-20260503\\td-trace-seed-20260423-games2.json`
- trace 设置：从 bootstrap 起跑，`seed=20260423`，`games=2`，`alpha=0.005`，`lambda=0.5`，`epsilon=0.02`，`depth=4`。
- 摘要：共 `83` 个 TD step；terminal_reason 分布 `none=81`、`terminal=2`。
- 前几步非终局信号仍温和：game 0 ply 0 为 black，`reward=0`，`value_before=-6`，`bootstrap_value=6`，`td_error=12`，`trace_norm=6.71`，`weight_delta_norm=0.40`。
- 终局步信号很猛：game 0 最后一步 `td_error=1042.31`，`weight_delta_norm=60.90`；game 1 最后一步 `td_error=1259.32`，`weight_delta_norm=76.24`。
- 结论：训练信号不是局部符号错，而是终局稀疏目标把少量样本的权重更新拉得过猛，32 局短训很容易被最后几步带偏。

### Opening attribution
- seed `20260423` fresh candidate opening：baseline search score `0`，candidate search score `-47`，score_delta `-47`，node_ratio `2.561141`。
- root static score baseline `-6`、candidate `-5`，static delta 只有 `+1`；search score delta 明显大于 root 静态评估差，说明漂移主要藏在 searched child positions / 搜索路径里。
- 贡献变化最大的权重集中在少量 tuple / index：tuple `14` index `1134` delta `+24`，tuple `13` index `1053` delta `+24`，tuple `10` index `810` delta `-18`，tuple `27` index `3483` delta `+12`，tuple `24` index `2024` delta `-12`。

### 分类判断
- A 不是主因：`lambda=0` 仍 regress。
- B 是次因：`epsilon=0` 能收住一部分 opening drift，但 match 仍 regress。
- C 是主结论：terminal reward 太稀疏、终局 TD spike 过大，下一步应先做 near-terminal curriculum / terminal target shaping。
- D 是后续归因线索：opening 贡献显示少数 tuple 权重漂移明显，但 root static delta 小，必须继续看 child-position attribution。
- E 排除：`alpha=0` 链路稳定。
- F 只能作为验收口径调整：Phase 3 现在应先证明 watch，不该把 32 局 regress 强行放行成 Phase 4。

## 2026-05-03 Phase 3 reward stabilization

### 参数与脚本修复
- 新增/保留训练稳定化参数：`terminal_reward`、`td_error_clip`、`terminal_only_warmup`、`near_terminal_curriculum`；这些字段已进入 `bitboard-train --format json`、`bitboard-trace` JSON/CSV、artifact manifest 和 selection/session summary。
- 修复 `benchmark-train.ps1` / `benchmark-select.ps1` 的子进程等待竞态：用 `$process.WaitForExit()` 替代 `Wait-Process -Id $process.Id`，避免短命进程先退出后 PID 查询失败。
- 不改 selection 判级，不改 PVS / Null Move / LMR / move ordering，不扩特征。

### Reward scale sweep
- sweep 根目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-reward-stabilization-20260503-230759`。
- 固定参数：`Alpha=0.005`，`Lambda=0.5`，`Epsilon=0.02`，`GamesPerSeed=32`，`CheckpointEvery=2`。

| TerminalReward | rating | C/B/D | opening drift | opening node avg/max | score regressions | earliest regress | terminal td_error avg/max | terminal weight_delta_norm avg/max |
|---:|---|---:|---:|---:|---:|---|---:|---:|
| 1200 | regress | 12/18/0 | 2 | 1.978487 / 2.561141 | 2 | checkpoint-2 | 752.134161 / 1864.461084 | 44.707753 / 112.344753 |
| 1000 | regress | 13/17/0 | 3 | 2.099411 / 3.322011 | 0 | checkpoint-2 | 544.016343 / 1853.583385 | 32.531343 / 111.040280 |
| 500 | regress | 16/13/1 | 2 | 1.193840 / 1.432065 | 0 | checkpoint-2 | 273.101783 / 759.792977 | 16.242052 / 45.762549 |
| 250 | regress | 14/16/0 | 1 | 1.615716 / 2.412364 | 0 | checkpoint-2 | 151.378295 / 472.760704 | 9.043178 / 28.495989 |
| 100 | regress | 16/14/0 | 3 | 1.414629 / 1.538723 | 1 | checkpoint-2 | 149.371373 / 683.730785 | 8.946000 / 40.857401 |

### TD error clipping sweep
- sweep 根目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-td-clip-stabilization-20260503-233908`。
- 选择 `TerminalReward=500` 作为最稳 reward：它在 reward sweep 中 C/B/D=`16/13/1`、score regressions=`0`、opening avg 最低，但 max 仍 regress。

| TerminalReward | TdErrorClip | rating | C/B/D | opening drift | opening node avg/max | score regressions | earliest regress | terminal td_error avg/max | terminal weight_delta_norm avg/max |
|---:|---:|---|---:|---:|---:|---:|---|---:|---:|
| 500 | 500 | regress | 14/15/1 | 3 | 1.163496 / 1.272418 | 0 | checkpoint-2 | 254.661929 / 500.000000 | 15.152409 / 30.702877 |
| 500 | 250 | regress | 16/13/1 | 2 | 1.143569 / 1.295516 | 0 | checkpoint-2 | 198.352949 / 250.000000 | 11.815307 / 15.515945 |
| 500 | 100 | regress | 16/14/0 | 2 | 1.319973 / 1.626359 | 0 | checkpoint-2 | 89.276156 / 100.000000 | 5.310823 / 6.174140 |

### Curriculum 最小诊断
- 诊断目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-near-terminal-curriculum-20260503-235739`。
- 实现了非默认近终局课程样本：`near_terminal_curriculum` 或 `terminal_only_warmup` 大于 0 时，前 N 局从一手可吃掉最后敌子的短样本开始，并交替黑/白视角。
- `games=8` 局部诊断：`terminal_checkmate=8`，`positions_evaluated=8`，`update_count=8`，全部为一步终局；trace 显示黑方 root value `-18.0 -> 27.0 -> 83.25`，白方 root value `-59.25 -> -14.25 -> 42.0`，说明局部 terminal reward 可以被学进去。
- `games=32` 诊断：`terminal_checkmate=31`，`terminal_no_capture_limit=1`，`positions_evaluated=72`，说明局部 reward 能学，但学到后段仍会影响搜索路径，不能把 curriculum 诊断偷换成完整 self-play 强度通过。

### 结论
- Reward scale 和 clipping 都能压 terminal spike；`TdErrorClip=250` 把 terminal td_error max 压到 `250`，weight_delta_norm max 压到 `15.515945`。
- 32 局 selection 仍然全部 `regress`，最接近 watch 的 `TerminalReward=500, TdErrorClip=250` 仍因 opening node ratio max `1.295516` 超门槛。
- 不跑 `GamesPerSeed=64`。该轮结束时 Phase 4 仍 blocked；后续 2026-05-04 attribution 将当前 blocker 更新为 opening search-tree inflation。

## 2026-05-04 Phase 3 opening node-ratio attribution

### 范围
- 本轮没有继续泛化 sweep，只分析当前最好组合：`Alpha=0.005`，`Lambda=0.5`，`Epsilon=0.02`，`TerminalReward=500`，`TdErrorClip=250`。
- selection 根目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333`。
- 原始结果：overall `regress`，C/B/D=`16/13/1`，fixed score regressions=`0`，opening node ratio avg/max=`1.143569 / 1.295516`。

### Opening case 原始归因
| seed | baseline best / score / nodes / time | candidate best / score / nodes / time | node ratio | score delta | best move drift |
|---:|---|---|---:|---:|---|
| 20260423 | `(0, 1) -> (1, 2) (B)` / `0` / `1472` / `0.001056` | `(0, 1) -> (1, 2) (B)` / `59` / `1653` / `0.001766` | `1.122962` | `+59` | no |
| 20260424 | `(0, 1) -> (1, 2) (B)` / `0` / `1472` / `0.001025` | `(1, 1) -> (2, 2) (B)` / `189` / `1490` / `0.001081` | `1.012228` | `+189` | yes |
| 20260425 | `(0, 1) -> (1, 2) (B)` / `0` / `1472` / `0.001309` | `(1, 1) -> (2, 2) (B)` / `74` / `1907` / `0.002237` | `1.295516` | `+74` | yes |

### 5 次重跑稳定性
- 使用 `bitboard-eval --candidate ... --baseline ... --depth 4 --format json` 重跑；该子命令内部经 `NormalizeSingleThreadLimits` 固定 `threads=1`，CLI 不接受 `--threads`。
- 3 个 seed 的 5 次 opening nodes 完全稳定：
  - `20260423`: baseline `1472`，candidate `1653`，node ratio `1.122962`。
  - `20260424`: baseline `1472`，candidate `1490`，node ratio `1.012228`。
  - `20260425`: baseline `1472`，candidate `1907`，node ratio `1.295516`。
- 判断：`1.295516` 是稳定搜索树变胖，不是单次搜索路径偶发，也不是 time 抖动。

### Root move score 对比
- 用 opening 后一手子局面做 depth 3 子搜索，root move score 取 child score 的相反数；只比较 score，不用这些子搜索 nodes 替代 selection nodes。
- baseline top 5：`0, 0, -6, -6, -6`，best-minus-fifth=`6`。
- candidate `20260423` top 5：`59, 59, 50, 43, 35`，best-minus-fifth=`24`。
- candidate `20260424` top 5：`189, 189, 186, 164, 162`，best-minus-fifth=`27`。
- candidate `20260425` top 5：`74, 74, 59, 50, 46`，best-minus-fifth=`28`。

### Candidate top 5 vs baseline same moves
| seed | move | candidate score | baseline score | delta |
|---:|---|---:|---:|---:|
| 20260423 | `(0, 1) -> (1, 2) (B)` | `59` | `0` | `+59` |
| 20260423 | `(1, 1) -> (2, 2) (B)` | `59` | `-6` | `+65` |
| 20260423 | `(2, 1) -> (2, 2) (B)` | `50` | `-18` | `+68` |
| 20260423 | `(3, 1) -> (2, 2) (B)` | `43` | `-18` | `+61` |
| 20260423 | `(0, 1) -> (0, 2) (B)` | `35` | `-12` | `+47` |
| 20260424 | `(1, 1) -> (2, 2) (B)` | `189` | `-6` | `+195` |
| 20260424 | `(2, 1) -> (2, 2) (B)` | `189` | `-18` | `+207` |
| 20260424 | `(0, 1) -> (1, 2) (B)` | `186` | `0` | `+186` |
| 20260424 | `(3, 1) -> (2, 2) (B)` | `164` | `-18` | `+182` |
| 20260424 | `(1, 1) -> (1, 2) (B)` | `162` | `0` | `+162` |
| 20260425 | `(0, 1) -> (1, 2) (B)` | `74` | `0` | `+74` |
| 20260425 | `(1, 1) -> (2, 2) (B)` | `74` | `-6` | `+80` |
| 20260425 | `(2, 1) -> (2, 2) (B)` | `59` | `-18` | `+77` |
| 20260425 | `(4, 1) -> (3, 2) (B)` | `50` | `-6` | `+56` |
| 20260425 | `(2, 1) -> (3, 2) (B)` | `46` | `-18` | `+64` |

### 搜索树膨胀判断
- “candidate 把多个开局 move 评得太接近”不是充分解释：candidate 的 top 2 确实并列，但 baseline 也 top 2 并列，且 baseline top 5 更挤（best-minus-fifth 只有 `6`）。
- 更像是 candidate 权重整体抬高多个 opening child subtree，导致 PV/应手变化后部分子树更贵。root stats 支持这个判断：
  - baseline root：nodes `1472`，qnodes `1010`，root_best_updates `5`，aspiration retries `0`。
  - candidate `20260425` root：nodes `1907`，qnodes `1273`，root_best_updates `8`，fail_highs `1`，aspiration retries `1`。
- `20260425` 是最终 max node ratio 的单 seed 来源，但不是单次评估偶发。可以作为分析建议讨论 selection 是否采用 multi-seed aggregate opening node ratio，而不是单点 max 一票否决；本轮不改判级、不放宽 selection。

### 更新后的 blocker
- reward spike 已压住：terminal td_error max 从千级被 `TdErrorClip=250` 固定到 `250`，terminal weight_delta_norm max 降到 `15.515945`。
- 当前 blocker 从 terminal TD spike 转移到 opening search-tree inflation。
- Phase 4 继续 blocked：`GamesPerSeed=32` 仍未达到 watch/pass，且 opening max node ratio 仍越过阈值。

### Targeted search-tree inflation diagnosis
- 新增仅 debug flag 控制的搜索诊断：`bitboard-search --search-diagnostics` / `--debug-search-tree`。默认关闭，不改变 PVS / aspiration / qsearch / selection 行为。
- 目标局面：opening，seed `20260425`，权重目录 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333\\selection\\seed-20260425`。
- root PV：
  - baseline: `(0, 1) -> (1, 2) (B) | (3, 4) -> (4, 3) (W) | (0, 0) -> (0, 1) (B) | (2, 5) -> (3, 4) (W)`。
  - candidate: `(1, 1) -> (2, 2) (B) | (0, 4) -> (1, 3) (W) | (0, 1) -> (1, 1) (B) | (1, 3) -> (2, 3) (W)`。

| side | score | nodes | qnodes | fail_highs | aspiration_retries | root_best_updates |
|---|---:|---:|---:|---:|---:|---:|
| baseline | 0 | 1472 | 1010 | 0 | 0 | 5 |
| candidate | 74 | 1907 | 1273 | 1 | 1 | 8 |

#### Root top 5, accepted depth-4 attempt
| side | move | score | nodes | qnodes | fail_highs | root_best_updates |
|---|---|---:|---:|---:|---:|---:|
| baseline | `(0, 1) -> (1, 2) (B)` | 0 | 410 | 357 | 0 | 1 |
| baseline | `(0, 1) -> (0, 2) (B)` | 0 | 40 | 19 | 0 | 0 |
| baseline | `(1, 1) -> (0, 2) (B)` | 0 | 38 | 16 | 0 | 0 |
| baseline | `(1, 1) -> (1, 2) (B)` | 0 | 42 | 18 | 0 | 0 |
| baseline | `(1, 1) -> (2, 2) (B)` | 0 | 41 | 18 | 0 | 0 |
| candidate | `(1, 1) -> (2, 2) (B)` | 74 | 560 | 469 | 0 | 1 |
| candidate | `(2, 1) -> (2, 2) (B)` | 74 | 46 | 22 | 0 | 0 |
| candidate | `(1, 1) -> (0, 2) (B)` | 74 | 40 | 18 | 0 | 0 |
| candidate | `(1, 1) -> (1, 2) (B)` | 74 | 38 | 15 | 0 | 0 |
| candidate | `(0, 1) -> (0, 2) (B)` | 74 | 33 | 12 | 0 | 0 |

#### Aspiration attempts
| side | depth | attempt | window | score | outcome | nodes | qnodes |
|---|---:|---:|---|---:|---|---:|---:|
| baseline | 1 | 1 | `[-1000000,1000000]` | 6 | exact | 16 | 16 |
| baseline | 2 | 1 | `[-26,38]` | -6 | exact | 62 | 46 |
| baseline | 3 | 1 | `[-38,26]` | 12 | exact | 443 | 394 |
| baseline | 4 | 1 | `[-20,44]` | 0 | exact | 951 | 554 |
| candidate | 1 | 1 | `[-1000000,1000000]` | 64 | exact | 16 | 16 |
| candidate | 2 | 1 | `[32,96]` | 52 | exact | 47 | 31 |
| candidate | 3 | 1 | `[20,84]` | 84 | fail_high | 111 | 77 |
| candidate | 3 | 2 | `[20,148]` | 86 | exact | 416 | 355 |
| candidate | 4 | 1 | `[54,118]` | 74 | exact | 1317 | 794 |

#### Qsearch source
- aggregate qnodes delta 是 `1273 - 1010 = +263`。
- aspiration retry 贡献 `+77` qnodes / `+111` nodes，约占 qnodes inflation 的 `29.3%`，不是全部来源。
- accepted depth-4 attempt 自身为 `794/554 = +240` qnodes，是主要来源。
- root move 聚合显示 PV 改变是核心：
  - `(1, 1) -> (2, 2) (B)`：baseline `18` qnodes，candidate `469` qnodes，delta `+451`，candidate capture continuation count `9`。
  - `(0, 1) -> (1, 2) (B)`：baseline `357` qnodes，candidate `173` qnodes，delta `-184`。
- 单条 qsearch source 最大只有 `2` qnodes；这是很多 quiescence entry 分布式变多，不是某一条 capture line 爆炸。
- candidate PV root `(1, 1) -> (2, 2) (B)` 下的主要 child-prefix 聚合：
  - `(0, 4) -> (1, 3) (W)`：`108` qnodes / `104` entries / `4` capture continuations。
  - `(0, 4) -> (0, 3) (W)`：`57` qnodes / `53` entries / `4` capture continuations。
  - `(0, 4) -> (1, 3) (W) | (0, 1) -> (1, 1) (B)`：`44` qnodes / `42` entries / `2` capture continuations。
  - `(0, 4) -> (1, 3) (W) | (0, 0) -> (1, 1) (B)`：`26` qnodes / `24` entries / `2` capture continuations。
  - `(0, 4) -> (0, 3) (W) | (0, 1) -> (1, 1) (B)`：`26` qnodes / `24` entries / `2` capture continuations。

#### A-E 判断
- A：部分成立。aspiration retry 确实存在，但只解释约三成 qnode inflation；不是主因。
- B：主因。PV 从 baseline 的 `(0, 1) -> (1, 2)` 切到 candidate 的 `(1, 1) -> (2, 2)`，进入更贵的 depth-4 / qsearch 子树。
- C：主因之一。candidate PV root 的 qnodes `469`，比 baseline 同招 `18` 大太多，并出现 `9` 个 capture continuation；qsearch/capture continuation 明显变多。
- D：证据不足，不作为主因。accepted depth-4 candidate 第一招就是最终 PV，root_best_updates 在该 attempt 只有 `1`，不像 move ordering 被候选分数拖坏后到处补搜。
- E：强度有收益但不能直接放行。当前 aggregate match `16/13/1`、fixed score regressions `0`，opening score delta `+74`；但 max node ratio `1.295516` 是真实稳定性能退步。

#### Selection 判级建议，仅记录不修改
- 当前 selection 把结果判为 `regress` 符合现有规则。`benchmark-select.ps1` 的 seed 级 scorecard 规则是：candidate wins 少于 baseline、fixed score regression 大于 `0`、或 max performance ratio 大于 `1.25`，任一命中即 `regress`；session 级 `overall_rating` 只要任一 seed `regress` 就整体 `regress`。在当前诊断口径下，即使不看亚毫秒 search_seconds，seed `20260425` opening node ratio `1907/1472 = 1.295516` 已超过 `1.25`，所以按现有规则一票触发 `regress`。
- 现有规则保护的风险不是“分数难看”这么简单，而是搜索树被评估权重真实放大：5 次重跑 nodes 完全稳定，说明这不是计时抖动；candidate PV 切到 `(1, 1) -> (2, 2) (B)` 后进入更贵 qsearch 子树，qnodes 从 `1010` 增到 `1273`，aspiration retry 只解释约 `29.3%` 的 qnode inflation。这个 gate 防的是训练权重表面强度领先，但把固定 opening 搜索成本抬高，后续再叠 Phase 4 特征和搜索调参就容易把问题放大，搁东北话说就是车还抖呢先别上高速。
- 如果未来要改规则，候选方向之一是把 `overall_rating` 拆成 `strength_rating` 与 `perf_rating`：`strength_rating` 只承载 aggregate match、fixed score/best-move 等棋力观察；`perf_rating` 单独承载 node/search_seconds/qsearch inflation。最终 gate 仍可派生一个保守 `overall_rating`，但原因不再糊成一坨。
- 第二个候选方向是把“单点 max node ratio 超阈”从直接 `regress` 降为 `perf_watch`，但只应处理孤立的、可复现的 tactical subtree inflation；如果是多 seed、多局面、score regression、aggregate 胜负倒退或 qsearch 全面膨胀，仍应直接 `regress`。
- 最低允许条件建议很硬：aggregate 胜负不得退步（最好 candidate wins 明确领先），fixed score regressions 必须为 `0`，超阈只能来自单 seed 的单个 fixed position performance max，且需要记录 repeat-run nodes 稳定、PV/qsearch attribution、没有 release 搜索参数变更。少一个条件都别放，别整“看起来差不多”那套玄学。
- 是否需要 `64` 局或更多 seed：需要，但要说清楚 `64` 局不是单独万能钥匙。`GamesPerSeed=64` 可以验证训练趋势没有在更长训练里翻车；更多 seed 才能证明单点 performance max 不是分布性问题。若要把这种情况从 `regress` 降到 `watch`，建议至少要求 `GamesPerSeed=64` 不退步，并增加 seed 覆盖后仍只有单 seed/单局面越过 performance 阈值。
- 本轮不修改 selection 判级，不新增 `perf_watch`，不拆 `overall_rating`。当前代码状态仍按现有规则判 `regress`。Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 仍 blocked。

## 2026-05-04 Phase 3 decision run, GamesPerSeed=64

### 设置
- 目的：policy evidence run，不是正式验收放行；`GamesPerSeed=32` 在现有规则下仍是 `regress`。
- 参数：`Alpha=0.005`，`Lambda=0.5`，`Epsilon=0.02`，`TerminalReward=500`，`TdErrorClip=250`，`GamesPerSeed=64`，`CheckpointEvery=2`。
- selection 根目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006`。
- 本轮不改搜索、不改特征、不放宽 selection。

### Final scorecard, existing rules
| metric | value |
|---|---:|
| overall_rating | `regress` |
| candidate/baseline/draws | `16/12/2` |
| fixed score regressions | `0` |
| best move changes | `2` |
| opening node ratio avg/max | `1.122056 / 1.245924` |
| average/worst performance ratio | `1.268240 / 1.386007` |

| seed | rating | C/B/D | score regressions | best move changes | max performance ratio | reason |
|---:|---|---:|---:|---:|---:|---|
| 20260423 | `regress` | `5/4/1` | 0 | 1 | `1.368354` | opening `search_ratio` over threshold |
| 20260424 | `regress` | `5/4/1` | 0 | 1 | `1.386007` | opening `search_ratio` over threshold |
| 20260425 | `watch` | `6/4/0` | 0 | 0 | `1.050360` | candidate leads but performance above parity |

### Opening attribution
| seed | candidate/baseline nodes | node ratio | search ratio | score delta | best move drift |
|---:|---:|---:|---:|---:|---|
| 20260423 | `1834/1472` | `1.245924` | `1.368354` | `+37` | yes |
| 20260424 | `1661/1472` | `1.128397` | `1.386007` | `+192` | yes |
| 20260425 | `1460/1472` | `0.991848` | `0.934524` | `+144` | no |

- 不再是 seed `20260425` opening 单点 max node ratio 触发 regress。`20260425` final seed rating 已降为 `watch`，opening best move 回到 baseline move，node ratio 低于 1。
- 本轮 overall `regress` 来自 `20260423` / `20260424` 的 opening performance ratio，且触发项主要是 `search_seconds` ratio；opening node ratio max `1.245924` 没超过 `1.25`。
- 5 次 final opening 重跑显示 nodes 完全稳定：`20260423=1834/1472`、`20260424=1661/1472`、`20260425=1460/1472`。search ratio 仍有时间波动：`20260423` 平均 `1.443641`，5 次中 4 次超过 `1.25`；`20260424` 平均 `1.246232`，5 次中 2 次超过 `1.25`；`20260425` 平均 `1.011826`，0 次超过 `1.25`。

### Checkpoint trend
- final strength signal 保持甚至略好于 32 局：`16/12/2` vs 32 局 `16/13/1`，fixed score regressions 仍为 `0`。
- checkpoint 趋势没有明显更稳定。64 局每 2 局 checkpoint 中，多数 aggregate checkpoint 仍有 `2` 或 `3` 个 seed 处于 `regress`；没有 checkpoint 达到全 seed `pass`。
- 中后段仍会掉到 aggregate strength 退步：checkpoint-50 为 `14/15/1`，checkpoint-62 为 `13/15/2`。final checkpoint 回到 `16/12/2`，说明强度信号能恢复，但训练轨迹仍抖，没资格叫稳定。
- 结论：64 局支持“strength signal 不是 32 局纯偶然”，但不支持“Phase 3 趋势稳定通过”。这玩意儿不是起飞，是在跑道上左右晃但没熄火。

### Decision
- 不进入 Phase 4。现有规则下 64 仍 `regress`，`task_plan.md` 继续保持 Phase 3 `implementation complete, trend validation failed`。
- 不应停止讨论 gate policy 后完全退回训练稳定性，因为 64 局 strength 没退步；但也不能把它硬放行。
- 建议下一步进入 Phase 3 的 report-only rating split 实现：新增/派生 `strength_rating` 与 `perf_rating`，保持现有 `overall_rating` 保守不变。目标是把“强度保持”和“性能/计时/节点风险”分开看清楚，不是放宽 selection。东北话说，先把表盘分清楚，别拿一个灯同时表示油量、水温和发动机炸没炸。

## 2026-05-04 Phase 3 report-only rating split

### 实现口径
- `selection-summary.json` / `selection-summary.txt` 现在输出 report-only 字段：`strength_rating`、`perf_rating`、`blocker_reason`。
- `strength_rating` 只看强度诊断：`candidate_wins` / `baseline_wins` / `draws`、fixed position score regression；best move drift 只作为 `watch` 信号，不用于放行。
- `perf_rating` 只看性能诊断：现有 `performance_ratio` 仍由 search_seconds / nodes 口径派生，超过既有 `1.25` 阈值判 `regress`；`qnodes_ratio` 与 aspiration retry 字段如 eval JSON 已提供则随 position metrics 透出，但不新增 gate。
- `blocker_reason` 来自现有 `rating` 的原因聚合，用来解释为什么 `overall_rating` 没过。

### 边界
- `overall_rating` 保持现有规则不变：seed 级任一 `regress` 仍使 session `regress`，没有让 `strength_rating=pass` 覆盖 `perf_rating=regress`。
- 没有新增 `perf_watch` 放行，没有修改 `pass` / `watch` / `regress` gate 语义，没有调 node/search/qnode 阈值。
- 没有修改搜索生产逻辑、训练算法、特征、PVS、qsearch、aspiration、move ordering、LMR 或 Null Move。
- Phase 3 仍是 `implementation complete, trend validation failed`；64 局 decision run 结论 `overall_rating=regress` 继续成立，Phase 4 继续 blocked。

## 2026-05-04 Phase 3 report-only performance instability attribution

### 实现口径
- `selection-summary.json` 现在在顶层、per-seed 层和 seed `scorecard` 层输出 machine-readable `blocker_details`。
- 每条 perf blocker detail 包含：`seed`、`position`、`category`、`metric`、`baseline_value`、`candidate_value`、`ratio`、`threshold`、`trigger`、`repeatability`。
- `metric` 覆盖旧 perf gate 已使用的 `search_seconds_ratio` / `node_ratio`；如果同一已触发 perf regress 的 position 同时提供 `qnodes_ratio` 且超阈，也作为诊断明细记录。
- `selection-summary.txt` 增加简短 `Perf blocker details` 表格，列为 `seed | category | metric | ratio | threshold | verdict`。
- 当前没有自动重跑证据接入 summary，因此 `repeatability` 默认为 `unknown`；后续若 eval report 提供 `repeatability` 或 `repeatability_evidence`，summary 会引用该字段。

### 边界
- `blocker_reason` 继续保留人类可读摘要，但不再是唯一诊断来源。
- `overall_rating` 沿用旧规则：seed 级 `regress` 仍使整体 `regress`；没有新增 `perf_watch`，没有让 `strength_rating` 覆盖 `perf_rating`。
- `qnodes_ratio` 不单独引入新的 gate；它只在旧规则已经由同一 position 的 search/node 性能触发 regress 时作为归因补充。
- 本轮没有修改搜索生产逻辑、训练算法、特征、搜索参数或 selection 阈值。Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 继续 blocked。

### 验证
- 先补 Pester 红灯：新增 `blocker_details` 断言后，`benchmark-select.ps1` 相关用例失败，集中在缺失结构化明细。
- 实现后运行 `Invoke-Pester -Script 'scripts\\workflow.tests.ps1' -PassThru -Quiet`，结果 `23 passed / 0 failed`。
- Pester 覆盖了单 seed 多 metric 超阈、`search_seconds_ratio` 超阈、`node_ratio` 超阈、`blocker_reason` 与 `blocker_details` 并存，以及旧 `overall_rating` 兼容。

## 2026-05-04 Phase 3 blocker evidence pack

### 当前状态
- Phase 3 状态保持为 `implementation complete, trend validation failed`。
- Report-only rating split 已完成：`strength_rating` / `perf_rating` / `blocker_reason` 只用于诊断，`overall_rating` 继续按旧规则保守派生。
- Report-only perf `blocker_details` 已完成并通过 targeted artifact validation；这只证明真实产物落盘路径，不代表 selection 趋势通过。
- 当前 blocker 仍是 opening performance instability / `perf_rating=regress`。
- Phase 4 继续 blocked。

### 证据索引
| 证据 | 产物目录 | 结论 |
|---|---|---|
| 32 局最好组合 | `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333` | `overall_rating=regress`，C/B/D=`16/13/1`，fixed score regressions=`0`，opening max node ratio=`1.295516`，Phase 3 trend validation failed |
| 64 局 decision run | `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006` | `overall_rating=regress`，C/B/D=`16/12/2`，fixed score regressions=`0`，opening node max 未越阈但 `20260423/20260424` opening `search_seconds_ratio` 触发 regress |
| quick selection smoke | `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-162204` | `overall_rating=watch`、`strength_rating=watch`、`perf_rating=watch`；未触发 final perf regress，顶层/per-seed final `blocker_details` 为空，`selection-summary.txt` 表格为 `(none)`；smoke validation failed / inconclusive |
| targeted perf-regress artifact validation | `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241` | `overall_rating=regress`、`strength_rating=watch`、`perf_rating=regress`；顶层、per-seed、final scorecard `blocker_details` 均非空；`selection-summary.txt` 的 `Perf blocker details` 表格非空 |

### Targeted artifact validation 明细
- 产物目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`。
- `overall_rating=regress`。
- `strength_rating=watch`。
- `perf_rating=regress`。
- 顶层 `blocker_details` 数量：`3`。
- per-seed `blocker_details` 总数：`3`。
- final scorecard `blocker_details` 总数：`3`。
- `selection-summary.txt` 中 `Perf blocker details` 表格非空。
- Pester 复跑结果：`23 passed / 0 failed`。

### blocker_details 示例
| seed | category / position | metric | ratio | threshold | verdict |
|---:|---|---|---:|---:|---|
| 20260423 | opening | `search_seconds_ratio` | `1.469059` | `1.25` | regress |
| 20260425 | opening | `search_seconds_ratio` | `2.159236` | `1.25` | regress |
| 20260425 | opening | `node_ratio` | `1.295516` | `1.25` | regress |

每条真实产物中的 detail 都包含 `seed`、`position`、`category`、`metric`、`baseline_value`、`candidate_value`、`ratio`、`threshold`、`trigger`、`repeatability`。

### 禁止事项
- 不进入 Phase 4。
- 不扩 N-Tuple / threat / mobility / stage 特征。
- 不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move。
- 不放宽 selection，不新增 `perf_watch` 放行。
- 不让 `strength_rating` 覆盖 `perf_rating`。
- 不把 report-only `blocker_details` 标成 Phase 3 completed；它只是 attribution evidence 完成。

## 2026-05-04 Phase 3 repeatability evidence pass

### 范围
- 目标产物：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`。
- 复核 blocker：`20260423` opening `search_seconds_ratio=1.469059`，`20260425` opening `search_seconds_ratio=2.159236`，`20260425` opening `node_ratio=1.295516`。
- 使用已有工具，不改搜索生产逻辑、不改训练算法、不新增特征、不修改 selection gate、不新增 `perf_watch`，不让 `strength_rating` 覆盖 `perf_rating`。
- 证据输出目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504`。
  - `opening-repeatability-runs.csv/json`：`bitboard-eval` 复跑记录，复现 selection opening 的 nodes/search_seconds；该输出不包含 qnodes。
  - `opening-bitboard-search-repeatability-runs.csv/json`：`bitboard-search --weights ... --depth 4 --threads 1 --format json` 复跑记录，补充 qnodes。
  - `opening-bitboard-search-repeatability-summary.json`：按 seed 汇总的 repeatability 摘要。

### Repeatability 表格
`bitboard-search` 与 `bitboard-eval` 对 opening root 的 nodes 一致，以下表格使用 `bitboard-search` 的 nodes/qnodes/time 结果。

#### seed 20260423 opening
| run | baseline nodes/qnodes/sec | candidate nodes/qnodes/sec | node_ratio | qnodes_ratio | time_ratio |
|---:|---:|---:|---:|---:|---:|
| 1 | `1472/1010/0.000988` | `1653/1138/0.001024` | `1.122962` | `1.126733` | `1.036437` |
| 2 | `1472/1010/0.000969` | `1653/1138/0.001228` | `1.122962` | `1.126733` | `1.267286` |
| 3 | `1472/1010/0.000849` | `1653/1138/0.001222` | `1.122962` | `1.126733` | `1.439340` |
| 4 | `1472/1010/0.000894` | `1653/1138/0.000983` | `1.122962` | `1.126733` | `1.099553` |
| 5 | `1472/1010/0.001027` | `1653/1138/0.001026` | `1.122962` | `1.126733` | `0.999026` |
| 6 | `1472/1010/0.000942` | `1653/1138/0.000987` | `1.122962` | `1.126733` | `1.047771` |
| 7 | `1472/1010/0.000946` | `1653/1138/0.001023` | `1.122962` | `1.126733` | `1.081395` |
| 8 | `1472/1010/0.000881` | `1653/1138/0.001028` | `1.122962` | `1.126733` | `1.166856` |
| 9 | `1472/1010/0.000909` | `1653/1138/0.001091` | `1.122962` | `1.126733` | `1.200220` |
| 10 | `1472/1010/0.001218` | `1653/1138/0.001030` | `1.122962` | `1.126733` | `0.845649` |

#### seed 20260425 opening
| run | baseline nodes/qnodes/sec | candidate nodes/qnodes/sec | node_ratio | qnodes_ratio | time_ratio |
|---:|---:|---:|---:|---:|---:|
| 1 | `1472/1010/0.000904` | `1907/1273/0.001172` | `1.295516` | `1.260396` | `1.296460` |
| 2 | `1472/1010/0.000881` | `1907/1273/0.001417` | `1.295516` | `1.260396` | `1.608400` |
| 3 | `1472/1010/0.000861` | `1907/1273/0.001131` | `1.295516` | `1.260396` | `1.313589` |
| 4 | `1472/1010/0.001061` | `1907/1273/0.001231` | `1.295516` | `1.260396` | `1.160226` |
| 5 | `1472/1010/0.000919` | `1907/1273/0.001253` | `1.295516` | `1.260396` | `1.363439` |
| 6 | `1472/1010/0.000926` | `1907/1273/0.001232` | `1.295516` | `1.260396` | `1.330454` |
| 7 | `1472/1010/0.000918` | `1907/1273/0.001575` | `1.295516` | `1.260396` | `1.715686` |
| 8 | `1472/1010/0.000917` | `1907/1273/0.001438` | `1.295516` | `1.260396` | `1.568157` |
| 9 | `1472/1010/0.001042` | `1907/1273/0.001213` | `1.295516` | `1.260396` | `1.164107` |
| 10 | `1472/1010/0.001363` | `1907/1273/0.001227` | `1.295516` | `1.260396` | `0.900220` |

### blocker repeatability 结论
| blocker | 文档层 repeatability | 结论 |
|---|---|---|
| `20260423 opening search_seconds_ratio=1.469059 > 1.25` | 10 次复跑 nodes 固定为 `1653/1472=1.122962`，qnodes 固定为 `1138/1010=1.126733`，均未越过 `1.25`；time ratio 范围 `0.845649-1.439340`，10 次中 `2` 次越过 `1.25` | timing-sensitive perf instability；有稳定的小幅 tree/qtree 增长，但 selection blocker 主要依赖亚毫秒 search_seconds 波动，不是 stable over-threshold tree inflation |
| `20260425 opening search_seconds_ratio=2.159236 > 1.25` | 10 次复跑 nodes 固定为 `1907/1472=1.295516`，qnodes 固定为 `1273/1010=1.260396`，time ratio 范围 `0.900220-1.715686`，10 次中 `7` 次越过 `1.25` | search_seconds 仍有 timing noise，但该 seed 同时存在稳定越阈的 tree/qtree inflation；time blocker 有稳定结构性支撑 |
| `20260425 opening node_ratio=1.295516 > 1.25` | 10 次复跑 nodes 固定为 `1907/1472=1.295516`，qnodes 固定为 `1273/1010=1.260396` | stable search-tree inflation；qsearch 也稳定越阈，支持 Phase 4 继续 blocked |

### 状态判断
- 文档层 repeatability 已不再是 `unknown`：真实 summary 暂未自动接入 repeatability 字段，但 findings/progress 已记录对应证据。
- `20260423` 更偏 timing-sensitive perf instability，不能当作稳定越阈 node/qnode inflation。
- `20260425` 是 stable search-tree inflation，nodes 与 qnodes 都稳定超过 `1.25`。
- Phase 3 仍是 `implementation complete, trend validation failed`。
- Phase 4 继续 blocked。
- selection gate 不变；本轮没有生产逻辑改动。

## 2026-05-04 Phase 3 repeatability artifact integration

### 实现口径
- `selection-summary.json` 的 perf `blocker_details` 现在接入文档层 repeatability 证据，但只作为 report-only 诊断输出。
- 每条 blocker detail 保留 `repeatability` 短状态，并新增 `repeatability_status`、`repeatability_runs`、`repeatability_exceeded_count`、`repeatability_notes`。
- 证据匹配必须命中 `seed + position + metric + rounded ratio`，防止同 seed 后续新产物被硬贴旧证据；匹配不上统一保持 `repeatability=unknown`。
- `selection-summary.txt` 的 `Perf blocker details` 表格新增 `repeatability` 简短列。

### 已接入的 repeatability 证据
| blocker | repeatability_status | runs | exceeded_count | 说明 |
|---|---|---:|---:|---|
| `20260423 opening search_seconds_ratio=1.469059` | `timing_sensitive` | `10` | `2` | nodes/qnodes 稳定但未越过 `1.25`，time ratio 只有 2/10 次越阈 |
| `20260425 opening search_seconds_ratio=2.159236` | `stable_tree_inflation_with_timing_noise` | `10` | `7` | search_seconds 有 timing noise，但同时有稳定越阈 tree/qtree inflation 支撑 |
| `20260425 opening node_ratio=1.295516` | `stable_tree_inflation` | `10` | `10` | nodes/qnodes 均稳定越过 `1.25` |

### 边界
- `overall_rating` 旧规则不变；`repeatability_status` 不参与 gate。
- 没有新增 `perf_watch` 放行，没有让 `timing_sensitive` 自动降为 watch，没有让 `strength_rating` 覆盖 `perf_rating`。
- `stable_tree_inflation` 只解释 blocker 来源，不新增额外 gate；这字段是诊断，不是判官，别看见名字硬加戏。
- 本轮没有修改搜索生产逻辑、训练算法、特征、搜索参数或 selection 阈值。
- Pester 结果：`24 passed / 0 failed`。
- Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 继续 blocked。

### Targeted real artifact validation follow-up
- 最新真实产物目录：`D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525`。
- 评级：`overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`。
- 顶层 `blocker_details` 实际只有 `2` 条：
  - `20260425 opening search_seconds_ratio=1.413497`，`repeatability=unknown`。
  - `20260425 opening node_ratio=1.295516`，`repeatability=stable_tree_inflation`。
- 验收结论：
  - Repeatability artifact integration remains implemented and unit-tested。
  - Real artifact validation is partial / not accepted。
  - `20260425 opening node_ratio=1.295516` 的 `stable_tree_inflation` 真实产物落盘路径已验证。
  - `20260425 opening search_seconds_ratio` 从已知证据 ratio `2.159236` 漂到 `1.413497`，正确保持 `repeatability=unknown`，没有硬贴旧证据。
  - `20260423` timing blocker 未在本轮真实产物中出现。
  - 三条 known blocker 的完整真实产物验证未达成。
- 这不是放行信号：Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 继续 blocked；不新增 `perf_watch`，不让 `strength_rating` 覆盖 `perf_rating`，`repeatability_status` 不参与 gate。
- Pester 复跑仍为 `24 passed / 0 failed`；输出里的预期异常文本来自负向测试，不影响结果。
- 不再通过反复刷真实训练 run 去凑三条 blocker；`search_seconds_ratio` 存在 timing noise，真实 run 不适合作为精确 ratio 命中的唯一验收方式。

### Deterministic artifact fixture validation
- 新增 Pester fixture 用例：`validates repeatability artifact formatting with deterministic fixture inputs`。
- fixture 通过 fake selection harness 生成临时 `selection-summary.json` / `selection-summary.txt`，不跑真实训练 run。
- fixture 覆盖 4 条 blocker：
  - `20260423 opening search_seconds_ratio=1.469059` => `timing_sensitive`。
  - `20260425 opening search_seconds_ratio=2.159236` => `stable_tree_inflation_with_timing_noise`。
  - `20260425 opening node_ratio=1.295516` => `stable_tree_inflation`。
  - `20260430 opening search_seconds_ratio=1.400000` => `unknown`。
- JSON 验证字段：`repeatability`、`repeatability_status`、`repeatability_runs`、`repeatability_exceeded_count`、`repeatability_notes`。
- TXT 验证 `Perf blocker details` 表格包含 `repeatability` 列，三条 known fixture 输出正确状态，未匹配 blocker 输出 `unknown`。
- Gate guard：`overall_rating=regress`、`strength_rating=pass`、`perf_rating=regress`；`timing_sensitive` 不自动降为 watch，`stable_tree_inflation` 不新增额外 gate，`unknown` 不影响判级，`strength_rating` 不覆盖 `perf_rating`，`repeatability_status` 不参与 gate。
- Pester 结果：`25 passed / 0 failed`；预期异常文本来自负向测试，不影响结果。
- 该验证只证明 artifact formatting / repeatability mapping 是确定性的，不代表 Phase 3 趋势通过；Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 继续 blocked。

## 2026-05-04 Phase 3 freeze / handoff summary

### 最终交接状态
- Phase 3：`implementation complete, trend validation failed`。
- Report-only rating split：completed。
- Report-only perf `blocker_details`：completed and targeted artifact-validated。
- Repeatability artifact integration：implemented, unit-tested, and deterministic fixture-validated。
- Real artifact validation：partial / not accepted。
- 当前 blocker：opening performance instability / `perf_rating=regress`。
- Phase 4：blocked。

### 证据链
| 证据 | 结论 |
|---|---|
| 32 局最好组合 `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333` | 仍为 `overall_rating=regress`；current blocker 没解除 |
| 64 局 decision run `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006` | 仍为 `overall_rating=regress`；强度信号保持但性能趋势不稳 |
| Targeted perf-regress artifact `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241` | `blocker_details` 真实 perf-regress artifact 验证通过 |
| Repeatability evidence | 已区分 `timing_sensitive` 与 stable tree/qtree inflation；`20260425 opening node_ratio=1.295516` 是 stable tree inflation |
| Targeted real artifact `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525` | 只部分验证 repeatability artifact；real artifact validation partial / not accepted |
| Deterministic fixture validation | Pester `25 passed / 0 failed`；验证 artifact formatting / repeatability mapping，不代表 Phase 3 trend pass |

### 禁止事项
- 不进入 Phase 4。
- 不扩 N-Tuple / threat / mobility / stage 特征。
- 不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move。
- 不改训练、搜索、特征、参数或 selection gate。
- 不放宽 selection。
- 不新增 `perf_watch`。
- 不让 `strength_rating` 覆盖 `perf_rating`。
- 不让 `repeatability_status` 参与 gate。

### 解除 blocked 条件
- `GamesPerSeed=32` 至少达到 `watch`。
- `GamesPerSeed=64` 不 `regress`。
- 两条同时满足前，不进入 Phase 4。别拿 `25 passed` 当趋势验收，这不是一回事，硬凑就是技术版掩耳盗铃。

## 2026-05-05 P0 Surakarta capture-rule correctness incident

### 事故状态
- 用户人工验收发现疑似 Surakarta 吃子规则回归：某些合法吃子无法正确生成或执行。
- 当前优先级切换为 P0 rule correctness incident。本轮目标是定位并修复棋规回归，不是优化、不调参、不扩特征。
- Phase 3 remains implementation complete, trend validation failed。
- Phase 4 blocked，且在 rule correctness 恢复前不得进入 Phase 4。
- Existing training / selection artifacts after the suspected regression are provisional / suspect until rule correctness is restored。尤其是疑似回归之后生成的 Phase 3 training、selection、repeatability、artifact-validation 结果不能作为可信通过依据。

### 硬边界
- 不进入 Phase 4。
- 不扩 N-Tuple / threat / mobility / stage 特征。
- 不调 PVS、qsearch 参数、aspiration、move ordering、LMR、Null Move。
- 不放宽 selection，不新增放行逻辑，不继续训练 sweep。
- 不把现有 Phase 3 结果当作可信通过依据。

### 本轮棋规审计范围
- move generation、capture path generation。
- loop / corner / arc traversal。
- own piece blocking、opponent piece detection。
- multiple-loop path legality。
- apply move / undo move。
- capture removes target piece。
- no-capture counter reset。
- terminal no-legal-move reason。
- traditional movegen 与 bitboard movegen parity。
- qsearch / search depth 1 的 capture-only move source 是否能看到合法吃子，但不调整 qsearch 参数。

### 大赛规则口径（用户提供）
- 每次移动一个棋子，双方轮流走。
- 普通移动：每个棋子可向 8 个方向移动一格，目标格必须无棋子。
- 吃子：必须经过至少一个完整弧线，且移动路径中不可以有本方棋子阻挡。
- 反向同路对称：黑子能沿某路径吃白子时，白子沿同一路径的相反方向也应能吃黑子。
- 一方棋子全部被吃掉时，有剩余棋子方获胜。
- 当双方都不能再吃掉对方棋子时，剩余棋子多的一方获胜。

### 规则口径影响
- 当前代码历史上存在 `max_no_capture_round` / `no_capture_ply` 训练终局口径；这不是大赛规则第 6 条本身，后续 audit 不能把“无吃子计数到阈值”偷换成“双方都不能吃子”。
- 当前测试仍需覆盖 no-capture counter reset，因为搜索/训练状态里有该字段；但大赛终局验收要单独按“双方是否还能吃子”判断。
- 吃子路径阻挡需按“大赛规则：本方棋子阻挡”审计；如果现有实现把其他情况也当阻挡，必须明确是额外约束还是规则误读，不能混成玄学。

### 待固化复现证据
- 最小失败局面：已固化为 C++ selftest golden case。
- board position：
  - Black: `(2,1)`, `(2,2)`, `(2,3)`
  - White: `(3,3)`
  - 其余为空。
- side to move：Black。
- expected legal capture：`(2,2) -> (3,3)`，Black captures White。
- actual before fix：traditional `SurakartaRuleManagerImpl::JudgeMove` 返回 `LEGAL_CAPTURE_MOVE`；bitboard `SurakartaRuleManagerBitboard::JudgeMove` 返回 `ILLIGAL_CAPTURE_MOVE`，`GenerateMoves(position, captures, true)` 缺失该 capture。
- traditional/bitboard disagreement：是。该局面用 traditional 作为 oracle；traditional 的 step-by-step traversal 会继续尝试后续路径，bitboard 旧静态表只保留前 4 条 path variants。

### 根因
- 根因文件：
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_bitboard.h`
  - `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard.cpp`
  - `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_masks.h`
- 根因函数/结构：
  - `surakarta::bitboard::BitboardTables`
  - `surakarta::bitboard::BitboardGenerator::Build()`
  - `surakarta::bitboard::IsCaptureLegal()`
- 具体问题：`kMaxCaptureVariants` 旧值为 `4`，但 golden `(2,2)->(3,3)` 有 `8` 条 capture path variants。`BitboardGenerator::Build()` 中 `if (variant_count < kMaxCaptureVariants)` 静默截断，导致静态 mask 只落了前 4 条路径；当前局面前 4 条均被己方棋子阻挡，后 4 条里存在合法路径，于是 bitboard 判成非法。传统实现按路径遍历不会受这个容量截断影响。
- 引入点定位：`git blame` 显示 `kMaxCaptureVariants=4` 来自 bitboard trusted baseline snapshot `49dc0aa`；本轮改动为未提交修复。近期 Phase 3 的 qsearch、selection、repeatability、no-capture-critical fixture 不是本次漏吃子的直接根因。

### 修复内容
- `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_bitboard.h`：`kMaxCaptureVariants` 从 `4` 调整为 `8`。
- `D:\\Dev\\Code\\surakarta_traditional\\include\\surakarta_masks.h`：重新生成静态 bitboard masks，使 `capture_variant_count[(2,2)][(3,3)] == 8` 并保留所有 path variants。
- `D:\\Dev\\Code\\surakarta_traditional\\src\\surakarta_bitboard_selftest.cpp`：新增/固化 capture-rule golden tests：
  - legal capture must be generated：`TestCaptureVariantOverflowGolden`
  - illegal blocked capture must not be generated：`TestIllegalCaptureVariantBlocked`
  - normal non-capture still works：`TestQuietMoveGoldenStillWorks`
  - capture after make/unmake board remains identical：`TestCaptureMakeUnmakeAndNoCaptureReset`
  - capture resets no-capture counter：`TestCaptureMakeUnmakeAndNoCaptureReset`
  - bitboard/traditional movegen agree on golden positions：`TestCaptureVariantOverflowGolden` 和既有 `TestRuleParity`
  - qsearch / search depth 1 sees capture：`TestSearchDepthOneSeesGoldenCapture`

### 规则审计结论
- move generation / capture-only source：修复后 `GenerateMoves(position, captures, true)` 能生成 golden capture；qsearch 使用同一路径 `GenerateMoves(position, move_list, true)`，未调 qsearch 参数。
- capture path generation / loop traversal：问题在 path variant 容量截断，不在 `NextCaptureStep` 或弧线方向转换本身。
- own piece blocking / opponent detection：bitboard 的 `capture_clear_mask` 仍按 occupancy 检查路径阻挡，目标格由敌方 bitboard 限定；新增 blocked golden case 会在补 `(3,2)` 己方棋子后同时要求 traditional/bitboard 拒绝。
- multiple-loop path legality：golden case 明确要求保留 8 条 variants，覆盖此前前 4 条被阻挡后遗漏后续合法路径的场景。
- apply/undo：`MakeMove` capture 会清源格、清目标格、落子、移除被吃子、重置 `no_capture_ply=0`；`UnmakeMove` 恢复 board、zobrist、eval_cache、side_to_move、ply、no_capture_ply。
- terminal no-legal-move reason：训练 self-play 仍有 `terminal_no_legal_move` 统计；legacy `JudgeEnd` / `EvaluateBitboardTerminal` 当前仍以 `max_no_capture_round` / `no_capture_ply` 表达 `STALEMATE`，这和用户给出的大赛第 6 条“双方都不能再吃子”不是同一个严格规则。这个 audit gap 已记录，未作为 Phase 4 或 Phase 3 放行依据。

### 验证结果
- clean rebuild：
  - 命令：VS DevCmd + CMake/Ninja clean + build `surakarta-bitboard-selftest surakarta-benchmark`
  - 结果：exit `0`，clean 删除 22 个旧产物并重新构建 21 个步骤。
- bitboard selftest：
  - 命令：`D:\\Dev\\Code\\surakarta_traditional\\build-p0-rule\\bin\\surakarta-bitboard-selftest.exe`
  - 结果：exit `0`，`[PASS] surakarta-bitboard-selftest`，新增 `TestCaptureVariantOverflowGolden` / `TestIllegalCaptureVariantBlocked` / `TestQuietMoveGoldenStillWorks` / `TestCaptureMakeUnmakeAndNoCaptureReset` / `TestSearchDepthOneSeesGoldenCapture` 均被执行。
- workflow Pester：
  - 命令：`Invoke-Pester -Script 'scripts\\workflow.tests.ps1' -PassThru -Quiet`
  - 结果：`TotalCount=25`，`PassedCount=25`，`FailedCount=0`。输出中红色 exception 是负向测试的预期异常。
- CTest：
  - 首次 `ctest` 发现 `surakarta-dev-session-selftest.exe` 未构建，`surakarta-bitboard-selftest` 已通过。
  - 构建缺失 target 后复跑：`ctest --test-dir build-p0-rule --output-on-failure`，结果 `2/2 passed`。

### Phase 3 artifact 影响
- Phase 3 仍是 `implementation complete, trend validation failed`；本次棋规修复不能反向证明任何训练趋势通过。
- Phase 4 继续 blocked。
- 疑似规则回归之后的 training / selection 产物必须继续标记为 `suspect` / `provisional`，尤其包括：
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-162204`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504`
- 这些产物可以作为历史诊断材料看，但不能当 Phase 3 trend validation 通过依据。规则底座刚修完，拿旧训练产物放行 Phase 4 就是典型“地基裂了还刷墙”，热闹但离谱。

## 2026-05-05 post-fix rule correctness hardening

### 防静默截断
- `BitboardGenerator::Build()` 的 capture variant 写入已改为 fail fast：当 `variant_count >= kMaxCaptureVariants` 时直接抛出 `runtime_error`，不再静默丢弃后续 path variant。
- 新增 `surakarta-mask-capacity-failfast` CTest：用 `SURAKARTA_CAPTURE_VARIANT_CAPACITY=4` 构建 mask exporter，预期生成失败；这验证未来容量不足会立即暴露，不会悄悄生成被截断的静态表。
- `surakarta-mask-export` 捕获生成异常并返回非零，避免 Windows 下未捕获异常变成不可诊断崩溃。

### Capture variant capacity invariant
- `TestCaptureVariantCapacityInvariant` 会调用 `BitboardGenerator().Build()` 并扫描动态生成表与 `kStaticBitboardTables` 的全部 36x36 from/to。
- 当前发现最大 `capture_variant_count` 为 `8`，不存在任何 capture variant count > `8`。
- `TestCaptureVariantOverflowGolden` 继续固定用户 P0 局面：Black to move，黑子 `(2,1)`, `(2,2)`, `(2,3)`，白子 `(3,3)`，`(2,2)->(3,3)` 必须是合法 capture 且保留 8 条 path variants。

### Rule correctness parity 扩展
- 新增 `TestRuleCorrectnessGoldenParity`，对每个 golden position 同时比较：
  - traditional `GetAllLegalTarget` vs bitboard rule manager `GetAllLegalTarget`
  - traditional 目标集合导出的 move set vs bitboard `GenerateMoves`
  - traditional capture set vs bitboard capture-only `GenerateMoves(..., true)`
  - 指定 `JudgeMove` 结果 parity
- 覆盖范围：中心、边、角、内环、外环、多路径、己方阻挡、敌方目标、无目标、普通移动。
- 该测试只强化棋规正确性，不调 PVS / qsearch / aspiration / move ordering / LMR / Null Move，不扩特征，不放宽 selection。

### Rule correctness gate
- Rule correctness selftest failed => 禁止 training。
- Rule correctness selftest failed => 禁止 selection。
- Rule correctness selftest failed => 禁止 release。
- Rule correctness selftest failed => 禁止 Phase 4。

### Historical artifact isolation
- 以下产物继续标记为 `suspect` / `provisional`，只能用于历史诊断，不能用于 Phase 3 trend validation 通过依据：
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260503-234333`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-123006`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-162204`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-163241`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\20260504-205525`
  - `D:\\Dev\\Code\\surakarta_traditional\\build-local\\training\\phase3-repeatability-evidence-20260504`

### Separate rule-semantics debt
- legacy `max_no_capture_round` / `no_capture_ply` 不是大赛第 6 条“双方都不能再吃子”的严格实现。
- 本轮不偷改终局语义，也不把 legacy no-capture limit 当作 release / Phase 4 放行依据。
- 后续若要严格对齐大赛规则，需要单独设计 golden cases，并修改 `JudgeEnd` / search terminal semantics。

### Phase status
- Phase 3 仍是 `implementation complete, trend validation failed`。
- Phase 4 仍 blocked。

## 2026-05-22 阶段 B1：大赛终局语义与规则完整性

### 设计结论
- 大赛第 6 条终局语义已从 legacy no-capture counter 中拆出：`no_capture_ply` / `max_no_capture_round` 继续保留为状态字段，但不再作为 national terminal gate。
- traditional 侧：`SurakartaRuleManagerImpl::JudgeEnd` 在合法走子后的棋盘上统计黑白剩余棋子；一方 0 子返回 `CHECKMATE`；双方都不能 capture 时返回 `STALEMATE`，winner 按剩余棋子数，多者胜，平子为 `SurakartaPlayer::NONE`。
- bitboard 侧：新增 `HasAnyCaptureMove(position, color)`，并用共享 `IsNationalStalemateTerminal(position)` 表达 national stalemate。该 helper 要求黑白都仍有棋、材料已低于初始满子状态、且黑白双方都没有 capture move；这个满子保护是为了避免开局无立即吃子时被荒唐判死，规则要落地也得讲人话，不然第一步都不用走了。
- search 侧：`EvaluateTerminal` / TD target / evaluation-match final reason / trace terminal helper 复用同一 national stalemate 语义；根节点遇到真实 terminal 时不再生成 best move。
- `game6.txt` 的 legacy `STALEMATE` 证据不再作为大赛第 6 条严格证明；它保留为历史 manual acceptance、perf endgame、pre/post capture checkmate fixture。B1 使用代码内 national-rule golden positions 固化新语义。

### 修改范围
- `include/surakarta_bitboard.h`：新增 `kInitialPiecesPerSide`、`HasAnyCaptureMove(position, color)`、`IsNationalStalemateTerminal(position)` 声明。
- `src/surakarta_bitboard.cpp`：实现指定颜色 capture availability 与 national stalemate helper。
- `src/surakarta_rule_manager_impl.cpp`：移除 `num_round_ - last_captured_round_ >= max_no_capture_round_` 终局 gate，改为棋子数和双方 capture availability。
- `src/surakarta_rule_manager_bitboard.cpp`：`EvaluateBitboardTerminal` 改用 `IsNationalStalemateTerminal`，`JudgeEnd` 不再构造 legacy no-capture 推演局面。
- `src/surakarta_search.cpp`：`EvaluateTerminal`、TD terminal target、evaluation match final reason 和训练 terminal reason 改为 national stalemate 语义；未改搜索参数。
- `src/surakarta_game.cpp` / `src/surakarta_utils.cpp`：合法落子后再判终局，让 `JudgeEnd` 看走后棋盘。
- `src/main.cpp`：trace terminal reason 同步为 `both_sides_cannot_capture`。
- `src/surakarta_bitboard_selftest.cpp` / `src/surakarta_dev_session_selftest.cpp`：新增 B1 终局 golden tests 与 dev-session status regression。

### 测试覆盖
- 一方棋子为 0：`CHECKMATE`，winner 为剩余方。
- 双方都不能 capture，黑方棋子多：`STALEMATE` / winner BLACK。
- 双方都不能 capture，白方棋子多：`STALEMATE` / winner WHITE。
- 双方都不能 capture，棋子数相等：`STALEMATE` / winner NONE。
- legacy no-capture counter 已越界但至少一方还能 capture：非 terminal。
- traditional / bitboard / `EvaluateBitboardTerminal` terminal parity。
- search `EvaluateTerminal` 与 rule manager 语义一致：national terminal 根节点无 best move；legacy counter alone 不阻止搜索出 move。
- GUI/dev-session status 不回退：national stalemate 能显示 terminal；legacy counter alone 不会把可吃子局面判 terminal。

### 验证结果
- build：`build-b1-core` 构建 `surakarta-bitboard-selftest`、`surakarta-dev-session-selftest`、`surakarta-benchmark`、`surakarta-mask-capacity-failfast`，exit `0`。
- bitboard selftest：`build-b1-core\bin\surakarta-bitboard-selftest.exe`，exit `0`，`[PASS] surakarta-bitboard-selftest`。
- dev-session selftest：`build-b1-core\bin\surakarta-dev-session-selftest.exe`，exit `0`，`[PASS] surakarta-dev-session-selftest`。
- Pester：`Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet`，`TotalCount=26`，`PassedCount=26`，`FailedCount=0`；红色 exception 为负向测试预期输出。
- CTest：首次因未构建 `surakarta-mask-capacity-failfast.exe` 出现 Not Run；补构建该 target 后复跑 `ctest --test-dir build-b1-core --output-on-failure`，`3/3 passed`。

### 阶段结论
- 阶段 B1 completed。
- 可以进入阶段 B2：可信基线重建。
- Phase 3 仍是 `implementation complete, trend validation failed`；Phase 4 仍 blocked。
- 仍禁止训练 sweep、特征扩展、搜索参数调整和 selection gate 放宽。B1 解决的是终局语义，不是棋力优化。

## 2026-05-22 阶段 B2：可信基线重建

### 工作区与边界
- 当前 git commit：`6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。
- `git status --short` 显示当前改动仍集中在 B1 代码、B1/B2 记忆文件；`docs/national_competition_technical_roadmap.md` 仍为 untracked。
- 本阶段未提交、未回滚、未更新 `test/test_data/perf-baseline.json`，也没有改 Phase 4 特征、搜索参数、训练参数矩阵或 selection gate。

### 默认回归结论
- `pwsh ./scripts/workflow.ps1 -Configuration Debug` 失败在 configure 阶段：`FetchContent` 下载 `https://github.com/glfw/glfw/archive/refs/tags/3.4.zip` 时连接被 reset。
- `pwsh ./scripts/workflow.ps1 -Configuration Release` 失败在同一类外部依赖下载：GitHub 连接超时。
- 这两个失败都没有进入 B1 代码编译/测试阶段，归类为 GUI FetchContent 网络环境失败。
- 降级验证使用 B1 后已有 core build：`D:\Dev\Code\surakarta_traditional\build-b1-core`，`SURAKARTA_ENABLE_GUI=OFF`。`surakarta-bitboard-selftest`、`surakarta-dev-session-selftest` 和 CTest `3/3` 均通过。
- Pester workflow 复核通过：`26 passed / 0 failed`；红色 exception 是负向 fixture 预期输出。

### Perf gate 分析
- perf run dir：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`。
- 命令：`pwsh ./scripts/perf-runner.ps1`。
- 结果：gate failed，`failure_count=15`；`14` 个 `performance_regression`，`1` 个 `score_drift`，无 `best_move_drift`。
- `middlegame` 全部通过，best move / score / performance gate 没有回归。
- `endgame` 9 项全部因 `node_ratio_tiny_position` 失败，节点从旧 baseline `54/63/72` 增加到 `221-348`，但 best move 仍为 `(5, 1) -> (1, 1) (B)`，score 仍为 `199900`。`game6.txt` 是 legacy no-capture fixture，B1 后不再按 legacy counter early terminal，因此这是规则语义变化造成的真实节点增长，不是计时噪声，也不是搜索调参退步。
- `opening` 无 best move drift；depth 6 4T 只有 `performance_ratio=1.252072`，贴线且偏计时/线程噪声；depth 7 出现节点比 `2.31/1.76/2.13`，depth 7 1T score 从 `12` 变为 `0`；depth 8 1T 节点比 `1.27`。这与 B1 移除 legacy no-capture terminal 后搜索树展开一致。
- 结论：当前 `test/test_data/perf-baseline.json` 是 pre-B1 terminal semantics 下的 baseline。B1 后 perf gate fail 是“旧 baseline 已不可信”的证据，不应被当作真实性能优化退步；但也不能擅自更新 baseline，必须由用户确认后生成 post-B1 baseline。

### Training smoke
- training smoke dir：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-145835`。
- 命令：`pwsh ./scripts/benchmark-train.ps1`。
- 结果：passed，`reproducible=True`。
- 参数：seed `20260423`，games `4`，depth `4`，checkpoint_every `2`，`alpha=0.005`，`lambda=0.5`，`epsilon=0.02`，terminal_reward `1200`。
- session manifest：`weights-manifest.json`，`purpose=training`，`git_commit=6063d9acbb24dc9397e07e99bffa4e94b8af55f6`，artifact count `8`。
- bootstrap hash `D87852D8E639182E479F4756FCFE63A1FDA992C01C163E6FC9D5C2D228F3AC51`；candidate hash `05A30986FD8A2BD85951B274D193673373F008DAF7D82F777CD566E88F99D61C`；candidate 与 bootstrap 不同，run-1/run-2 hash 可复现。

### Selection smoke
- selection smoke dir：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-150225`。
- 命令：`pwsh ./scripts/benchmark-select.ps1 -GamesPerSeed 2`。
- 结果：脚本入口与产物链路通过，summary 写出成功。
- summary：`overall_rating=regress`，`strength_rating=watch`，`perf_rating=regress`，C/B/D=`15/12/3`，worst performance ratio `1.343612`。
- 该 smoke 只证明 selection 入口、manifest、summary、blocker_details 仍可运行；`GamesPerSeed=2` 绝不代表 Phase 3 趋势通过。想拿 2 局 smoke 当强度证据，那就是拿温度计当望远镜，方向都不对。

### B2 结论
- B2 acceptance 条件已满足：默认 workflow 的环境性失败已记录并有 core 等价降级验证；Pester 通过；perf gate 已运行且失败分类清楚；training smoke 通过；selection smoke 仅作入口检查；所有产物路径已记录。
- B2 可以封口，但 post-B1 perf baseline refresh 仍需用户确认后单独执行。
- 下一步可以进入 Phase 3 post-B1 fresh trend validation 准备；Phase 3 仍未通过，Phase 4 仍 blocked。

## 2026-05-22 阶段 B2.1：post-B1 perf baseline candidate

### 工作区确认
- `git status --short` 显示 B1 源码与记忆文件仍为 modified，`docs/national_competition_technical_roadmap.md` 仍为 untracked。
- `git diff --name-status -- test/test_data/perf-baseline.json` 无输出，确认正式 baseline 本轮没有 diff，也没有被覆盖。
- 当前 commit 仍为 `6063d9acbb24dc9397e07e99bffa4e94b8af55f6`。

### 旧 baseline failure 复核
- 旧 perf run：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`。
- `perf-gate.json`：`status=fail`，`failure_count=15`。
- failure split：`best_move_drift=0`，`score_drift=1`，`performance_regression=14`。
- case 分布：`endgame=9`，全部为 tiny-position `node_ratio_tiny_position`；`opening=6`，其中 5 个 `performance_regression` + 1 个 depth 7 1T `score_drift`；`middlegame=0`。
- 结论不变：旧 `test/test_data/perf-baseline.json` 是 pre-B1 terminal semantics baseline，不适合作为 post-B1 gate。继续拿它卡 B1 后结果，等于拿旧尺子量新规则，量出来当然一地鸡毛。

### Candidate 生成
- candidate 文件：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\perf-baseline.candidate.json`。
- candidate summary：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-candidate-20260522-151651\candidate-summary.txt`。
- 来源 run：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`，以 `perf-summary.json` 为索引来源，并从每条 `stdout_log` 补回可用 benchmark 原始字段。
- 矩阵完整性：`opening` / `middlegame` / `endgame` × depth `6/7/8` × threads `1/2/4`，共 27 条；必需字段 `case_id`、`depth`、`threads`、`search_seconds`、`nodes`、`best_move`、`score`、`nps`、`tt_hits` 均存在。
- 语义说明：candidate 是 post-B1 national terminal semantics；legacy no-capture counter 不再作为 terminal gate；正式 baseline 未修改。

### Candidate gate 复跑
- 复跑 #1：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729`，`status=fail`，`failure_count=1`。
- #1 failure：`middlegame depth 7 threads 4`，`performance_ratio=1.405796`，basis=`max_search_or_node_ratio`；`search_ratio=1.405796`，`node_ratio=1.011121`，best move 和 score 均不漂。
- 复跑 #2：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913`，`status=fail`，`failure_count=4`。
- #2 failures：`opening d7 t2 performance_ratio=1.291630 node_ratio=1.063045`；`middlegame d7 t1 performance_ratio=2.059995 node_ratio=1.000000`；`middlegame d7 t2 performance_ratio=1.409518 node_ratio=1.022910`；`middlegame d8 t2 performance_ratio=1.313311 node_ratio=0.995371`。
- 两次复跑均为 `best_move_drift=0`、`score_drift=0`；失败均来自 `search_seconds`，没有系统性 node ratio 超阈。
- endgame 在 candidate baseline 下不再失败：两次复跑最大 endgame `node_ratio` 分别为 `1.000000` 和 `1.069597`，说明 tiny endgame 不再被 pre-B1 legacy early terminal 节点数误杀。

### B2.1 结论
- candidate 作为 post-B1 语义快照是完整的，但基于单次来源 run 的 `search_seconds` baseline 太瘦，复跑会被环境/多线程计时噪声打穿。
- 当前不建议正式替换 `test/test_data/perf-baseline.json`。
- 不建议调阈值、不建议调搜索参数、不建议把这写成 Phase 3 trend validation 通过。
- B2.2（正式 baseline 替换与 gate 复验）暂不应进入，除非用户确认新的正式 baseline 生成口径，例如 post-B1 多样本保守聚合。

## 2026-05-22 阶段 B2.1b：多样本保守聚合 post-B1 perf baseline candidate

### 工作区与边界
- 本轮未修改 `test/test_data/perf-baseline.json`；`git diff --name-status -- test/test_data/perf-baseline.json scripts/perf-runner.ps1` 无输出。
- 本轮未修改 `scripts/perf-runner.ps1`，未改 perf gate 阈值，未调搜索参数，未跑训练 sweep，未跑 `GamesPerSeed=32/64`，未进入 Phase 4。
- 新 candidate 只落在 `D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424`。

### 输入样本
- 原始 post-B1 source run：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-145419`。
- B2.1 candidate 复跑：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151729`、`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-151913`。
- 本轮新增 single-candidate perf runs：
  - `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163919`：fail，`failure_count=1`，`middlegame d6 t4` timing-only。
  - `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-163943`：fail，`failure_count=3`，`opening d6 t2`、`opening d7 t1/t2` timing-sensitive。
  - `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164009`：pass，`failure_count=0`。
  - `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164037`：pass，`failure_count=0`。
  - `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164101`：fail，`failure_count=1`，`middlegame d6 t4` timing-only。
- 共 8 个 `perf-summary.json` 输入，每个 27 条，覆盖 `opening/middlegame/endgame` × depth `6/7/8` × threads `1/2/4`。

### 聚合策略
- 聚合前对 27 个 case/depth/thread 分组检查 `best_move` 和 `score`：8 个样本全部一致，未触发停止条件。
- candidate 字段策略：
  - `search_seconds=max`，降低单次计时偏瘦导致的误杀。
  - `nodes=max`，避免低节点样本低估 post-B1 搜索树；不通过放宽 `search_seconds` 掩盖节点回归。
  - `qnodes`、TT/剪枝/aspiration 等诊断计数取 `max`。
  - `nps=min`，保守记录吞吐；`pv` 取 max-search 样本的代表值，`perf-runner.ps1` 不用它做 gate。
- `aggregate-summary.txt` 已记录输入 run、聚合策略、每个条目的 min/median/max `search_seconds`、min/max `nodes`、best_move/score 稳定性、timing-sensitive 条目和 tree-variable watch 条目。

### 聚合结果
- aggregate candidate：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json`。
- summary：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\aggregate-summary.txt`。
- `baseline_version=1`，`results` 为 27 条，兼容 `perf-runner.ps1` 的 baseline 读取格式。
- `best_move` 稳定性：27/27 stable。
- `score` 稳定性：27/27 stable。
- timing-sensitive 非 tiny 条目 10 个，典型例子：`middlegame d7 t1` 节点固定 `53365/53365`，但 `search_seconds` 范围 `0.021331-0.047075`。
- tree-variable watch 条目 6 个，集中在 opening 多线程：`opening d7 t4` node spread `1.255242`，`opening d8 t4` node spread `1.257587`。这不是放宽 gate 的理由，aggregate candidate 只取观测 max nodes，后续若节点比继续超过阈值仍应按真实搜索树不稳定报告。
- tiny endgame 9 项 `search_seconds` 仍有毫秒以下噪声，但 perf-runner 对 tiny position 使用 `node_ratio_tiny_position`，不把微秒计时当 gate 主证据。

### Aggregate candidate 复跑验证
- 复跑 #1：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164507`，`status=pass`，`failure_count=0`，`best_move_drift=0`，`score_drift=0`，最大 `node_ratio=1.000000`，最大 `search_ratio=0.933802`。
- 复跑 #2：`D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-164532`，`status=pass`，`failure_count=0`，`best_move_drift=0`，`score_drift=0`，最大 `node_ratio=1.057175`，最大 `search_ratio=1.220779`。
- 两次复跑没有 `performance_regression`，没有 best move / score drift，node ratio 未系统性超阈。

### B2.1b 结论
- 多样本保守聚合 candidate 已把 B2.1 的单次 `search_seconds` 偏瘦问题压住，同时没有隐藏 node/tree regression 的证据。
- 当前建议进入 B2.2：以 aggregate candidate 作为正式 `test/test_data/perf-baseline.json` 替换候选，并在替换后做正式 gate 复验。
- 这仍不代表 Phase 3 趋势通过；selection / training 趋势状态保持 `implementation complete, trend validation failed`，Phase 4 继续 blocked。别拿 perf baseline 过了当棋力过了，那是把门牌号当身份证，听着像那么回事，其实不是一回事。

## 2026-05-22 阶段 B2.2：正式 perf baseline 替换与 gate 复验

### 工作区与替换确认
- 起始 `git diff --name-status -- test/test_data/perf-baseline.json scripts/perf-runner.ps1` 无输出，确认正式 baseline 与 `scripts/perf-runner.ps1` 起始均无 diff。
- aggregate candidate 文件存在且 JSON 可解析：`D:\Dev\Code\surakarta_traditional\build-local\perf\post-b1-baseline-aggregate-20260522-164424\perf-baseline.aggregate-candidate.json`。
- candidate `matrix` 为 `opening/middlegame/endgame` × depth `6/7/8` × threads `1/2/4`，展开为 27 个矩阵项；`results` 为 27 条唯一 case/depth/thread。
- 已将 aggregate candidate 原样复制到 `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；source/target SHA256 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- 替换后 diff 范围只包含 `test/test_data/perf-baseline.json`，`scripts/perf-runner.ps1` 仍无 diff；没有放宽阈值、没有调搜索参数、没有改评估特征。

### 聚合策略落盘口径
- `best_move` / `score`：8 个输入 perf run、27 个 case/depth/thread 全稳定；任何 drift 都应停止替换流程。
- `search_seconds=max`，避免单次偏瘦 timing baseline 误杀。
- `nodes=max`，避免低估 post-B1 搜索树；后续若节点比继续超阈，仍按真实 tree instability 处理。
- `qnodes` / TT / 剪枝 / aspiration 等诊断计数取 `max`，`nps=min`。

### 正式 perf gate 复验
| 复跑 | run dir | 结果 | 关键指标 |
|---|---|---|---|
| #1 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165717` | pass | baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max `node_ratio=1.001406`，max `search_ratio=1.070239` |
| #2 | `D:\Dev\Code\surakarta_traditional\build-local\perf\20260522-165815` | pass | baseline path `D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json`；`failure_count=0`，`best_move_drift=0`，`score_drift=0`，`performance_regression=0`，max `node_ratio=1.000294`，max `search_ratio=1.249836` |

### 其他验证
- Pester：`Invoke-Pester -Script scripts\workflow.tests.ps1 -PassThru -Quiet`，`26 passed / 0 failed`；红色 exception 为负向测试预期输出。
- CTest：裸 `ctest --test-dir build-b1-core --output-on-failure` 在当前 PowerShell PATH 中找不到 `ctest`；改用项目封装 `. ./scripts/common.ps1; Invoke-SurakartaCTest -BuildDir build-b1-core`，`3/3 passed`。
- `git diff --check`：exit `0`；仅报告现有 LF/CRLF warning，无 whitespace error。

### 阶段结论
- B2.2 正式 baseline 替换与 gate 复验 completed；`test/test_data/perf-baseline.json` 已成为 post-B1 aggregate official baseline。
- 这只解决 post-B1 perf gate 的可信基线，不代表训练趋势通过。Phase 3 仍是 `implementation complete, trend validation failed`，Phase 4 仍 blocked。
- 下一阶段只可进入 Phase 3 post-B1 fresh trend validation 准备；不应顺手开 32/64、训练 sweep、搜索参数调整或 Phase 4。别刚把秤校准完就拿它当冠军奖杯，那玩意儿不是一个物种。

## 2026-05-22 Phase 3 post-B1 fresh trend validation 准备

### 本轮边界
- 本轮是 Phase 3 准备阶段，只封口验证路线、产物边界和判定口径。
- 未进入 Phase 4；未修改搜索参数、评估特征、训练算法、`scripts/perf-runner.ps1`、selection gate 阈值；未提交、未回滚、未清理现有改动。
- 未运行正式 `GamesPerSeed=32/64` selection。当前 `GamesPerSeed=2` selection smoke 只作为入口验证，不能作为 Phase 3 证据。

### 已确认事实
- B2.2 official baseline 可作为 fresh trend validation 的 perf 基线：`D:\Dev\Code\surakarta_traditional\test\test_data\perf-baseline.json` 与 aggregate source SHA256 均为 `80E70D3500C82F2EC64F825F4C3C83649BFE0E95323D7BEFEF1274B76AF2C0FB`。
- 两次正式默认 perf gate 已确认使用 official baseline，且 `failure_count=0`、`best_move_drift=0`、`score_drift=0`、`performance_regression=0`：`20260522-165717`、`20260522-165815`。
- `build-vs-train-release\bin\surakarta-benchmark.exe` 存在，轻量启动会输出 CLI usage；本轮未重建。
- `benchmark-select.ps1` 默认参数确认：Seeds `20260423,20260424,20260425`；`GamesPerSeed=32`；`Depth=4`；`CheckpointEvery=2`；`Alpha=0.005`；`Lambda=0.5`；`Epsilon=0.02`；`EpsilonPlies=6`；`TerminalReward=1200`；`TdErrorClip=0`；`TerminalOnlyWarmup=0`；`NearTerminalCurriculum=0`。
- `benchmark-train.ps1` 默认 smoke 参数确认：`Games=4`、`Seed=20260423`，其余 TD 参数与 selection 默认一致。
- 可用产物边界：
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-145835`：training smoke / reproducibility 产物，`reproducible=True`，artifact count `8`；不是 strength evidence。
  - `D:\Dev\Code\surakarta_traditional\build-local\training\20260522-150225`：selection smoke，`GamesPerSeed=2`，`overall_rating=regress`、`strength_rating=watch`、`perf_rating=regress`、C/B/D=`15/12/3`、worst performance ratio `1.343612`；只证明入口、manifest 和 summary 链路可用。

### 正式验证路线
- 第一关是 fresh post-B1 `GamesPerSeed=32` selection；当前不建议再做更小规模预检。2 局 smoke 已经证明入口能跑，再跑小样本只会让报告看起来热闹，证据含金量还是薄。
- `GamesPerSeed=32` 若 `overall_rating=regress`：立即停止，不跑 64，进入 blocker 分类。
- `GamesPerSeed=32` 若至少 `watch`：允许进入 `GamesPerSeed=64`。
- `GamesPerSeed=64` 不得 `regress`，才允许讨论 Phase 3 是否从 failed 升级为 watch/pass，以及 Phase 4 是否仍 blocked。
- 即便 32/64 都过，也只能表述为 Phase 3 trend gate 初步恢复；不能写“国赛级”，别刚会走两步就宣布打进世界杯，太上头。

### 推荐命令
```powershell
pwsh ./scripts/benchmark-select.ps1 `
  -BuildDir 'D:\Dev\Code\surakarta_traditional\build-vs-train-release' `
  -Seeds 20260423,20260424,20260425 `
  -GamesPerSeed 32 `
  -Depth 4 `
  -CheckpointEvery 2 `
  -Alpha 0.005 `
  -Lambda 0.5 `
  -Epsilon 0.02 `
  -EpsilonPlies 6 `
  -TerminalReward 1200 `
  -TdErrorClip 0 `
  -TerminalOnlyWarmup 0 `
  -NearTerminalCurriculum 0
```

`GamesPerSeed=64` 只能在 32 局至少 `watch` 后，把上面命令中的 `-GamesPerSeed 32` 改成 `-GamesPerSeed 64` 执行。

### 结果分类表
| 类别 | 判定 | 处理 |
|---|---|---|
| strength regress | `candidate_wins < baseline_wins` 或固定局面 `score_delta < 0` | 停止；不跑 64；优先看训练信号、TD target、terminal reward 与 checkpoint 演化 |
| perf regress | `max_performance_ratio > 1.25` | 停止或 blocked；按 `search_seconds_ratio`、`node_ratio`、`qnodes_ratio` 和 `performance_basis` 拆因 |
| best_move_changes | 固定局面 best move 变化 | 不直接当强度提升；必须按 opening / capture-heavy / threat-heavy / endgame / no-capture-critical 分类解释，opening 尤其敏感 |
| score_delta | candidate score - baseline score | 正向只作为观察信号；负向是 strength blocker；terminal / no-capture-critical 需区分 B1 national terminal semantics 影响 |
| opening search-tree inflation | opening `node_ratio` / `qnodes_ratio` / aspiration retries 异常 | 重点 blocker；若稳定超阈，Phase 4 继续 blocked |
| tiny position timing | 节点很小的局面耗时比异常 | 优先看 node/qnode ratio，不被微秒级 `search_seconds` 带跑偏 |
| TD artifact 问题 | manifest/hash/seed/参数/checkpoint 缺失或不可追溯 | 归为 artifact/infrastructure blocker，不作为 strength/perf 结论 |

### Watch / pass / blocked 口径
- 可升级为 Phase 3 watch candidate：32 局 `overall_rating` 至少 `watch`，且没有 artifact 污染；这只解锁 64，不解锁 Phase 4。
- 可讨论 Phase 3 watch/pass：64 局 `overall_rating` 不为 `regress`，再结合 `strength_rating`、`perf_rating`、best move、score、TD error、weight delta 和 blocker_details 判定。
- 必须继续 blocked：32 局 regress；64 局 regress；任何正式 run 混入 pre-B1 / suspect / provisional artifacts；baseline hash 不匹配；脚本或 threshold 被改；opening tree inflation 仍无法解释；manifest/summary 缺失导致不可复现。

---
*每执行2次查看/浏览器/搜索操作后更新此文件*
*防止视觉信息丢失*

## 2026-05-25 Phase 3.36 诊断 dry-run 证据评审

- 本轮只做 default-off objective path 的 diagnostic dry-run / report-only 评审，未触发 training 或 selection。
- Phase 3.35 / 3.34 / 3.33 / 3.31 输入链已复核，JSON 解析通过，Phase 3.33 继续保持 `objective_implemented=true`、`default_off=true`、`selection_gate_eligible=false`。
- 构建自测已复核通过，输出标记已拿到：`BUILD_AND_DRY_RUN_SELFTEST_OK_PHASE3_36`、`INPUT_CHAIN_OK_PHASE3_36`、`STATUS_FLAGS_OK_PHASE3_36`、`WORDING_GUARD_AND_NO_BIN_OK_PHASE3_36`。
- Phase 3.36 输出目录只生成 report/json/manifest 三件套，`phase_output_bin_count=0`，`rollback_triggered=false`。
- 结论不变：`phase3_status=not_passed`，`phase4_status=blocked`；允许进入 Phase 3.37 guarded limited training plan review，但不允许直接训练。

## 2026-05-26 Phase 3.41 guarded active interface skeleton 证据评审

- 本轮只做 interface skeleton / default-off / not selection eligible；没有进入 active probe、selection、promotion 或 gate 修改。
- 当前训练入口事实继续保持为 `surakarta-benchmark.exe bitboard-train`。
- Phase 3.41 只新增单一 `--config <path>` 文件入口；没有新增 `--opening` / `--objective` / `--active-objective` / `--enable-objective` 这类散乱 toggle。
- `ActiveObjectiveConfigSkeleton` 默认 `default_off=true`、`active_objective_probe_executed=false`、`selection_gate_eligible=false`。
- parser 接受 disabled config 作为 default-off 有效状态；invalid config 直接拒绝；valid scoped config 只设置 skeleton state，不设置 `opening_safe_objective_enabled`。
- selftest 已覆盖 config 缺省关闭、无效 config 拒绝、valid scoped config skeleton-only。
- 验证标记已拿到：`BUILD_AND_SELFTEST_OK_PHASE3_41`、`CONFIG_GATED_INTERFACE_SKELETON_OK_PHASE3_41`、`STATUS_FLAGS_OK_PHASE3_41`、`WORDING_GUARD_AND_NO_BIN_OK_PHASE3_41`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_41_guarded_active_interface_skeleton`。
- 结论不变：Phase 3 仍为 `not_passed`，Phase 4 仍为 `blocked`；建议进入 Phase 3.42 guarded active interface verification package。别门框刚立起来就说房子能住，东北话讲这叫太着急上炕。

## 2026-05-26 Phase 3.42 guarded active interface verification package 证据评审

- 本轮只做 verification-only / default-off / not selection eligible；未新增代码逻辑，未改 schema，未改训练逻辑。
- Phase 3.41 interface skeleton 输入证据确认 `interface_implemented=true`、`default_off=true`、`active_objective_probe_executed=false`、`selection_gate_eligible=false`。
- `src\main.cpp` 中存在单一 `--config` 入口；未检测到散乱 objective CLI toggle。
- `include\surakarta_search.h` 中存在 `ActiveObjectiveConfigSkeleton`；selftest 文本包含 invalid config、valid scoped config、active probe guard markers。
- 验证标记已拿到：`BUILD_AND_SELFTEST_OK_PHASE3_42`、`INPUT_CHAIN_AND_PHASE3_41_INTERFACE_OK_PHASE3_42`、`ACTIVE_INTERFACE_STATIC_BOUNDARY_OK_PHASE3_42`、`STATUS_FLAGS_OK_PHASE3_42`、`WORDING_NO_BIN_AND_PROTECTED_DIFF_OK_PHASE3_42`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_42_guarded_active_interface_verification_package`。
- 结论：接口存在但仍只是 default-off skeleton；允许进入 Phase 3.43 active scoped probe dry-run plan review，不允许直接 probe execution。

## 2026-05-26 Phase 3.43 active scoped probe dry-run plan review 证据评审

- 本轮只做 probe dry-run plan review；未执行 active probe，未改接口，未改 schema，未改 objective。
- Future probe 计划限定为 opening-only、small bounded、quarantine-only、report-only。
- 计划 config 样例固定 `schema_version=1`、`mode=active_scoped`、`scope=opening_root_children_only`、`report_only=true`、`selection_gate_eligible=false`、`max_games=1`、`max_depth=1`、seed allowlist `20260423`。
- 计划命令模板绑定 `surakarta-benchmark.exe bitboard-train --config <path>`，输出只能落在 Phase 3.44 quarantine root。
- Hard reject 覆盖 missing/invalid config、非 opening scope、`report_only=false`、`selection_gate_eligible=true`、参数超限、输出越界。
- Rollback triggers 覆盖 selection/promotion/gate/rating/scorecard/perf threshold 路径被触碰、schema drift、selftest failure、quarantine 外二进制输出。
- 验证标记已拿到：`input-json-ok`、`STATUS_FLAGS_OK_PHASE3_43`、`WORDING_GUARD_AND_NO_BIN_OK_PHASE3_43`、`PROTECTED_AND_CODE_DIFF_REVIEW_REQUIRED_PHASE3_43`。
- 输出目录：`D:\Dev\Code\surakarta_traditional\build-local\training\20260522-172134\opening-diagnostics\phase3_43_active_scoped_probe_dry_run_plan_review`。
- 结论：允许进入 Phase 3.44 guarded active scoped probe dry-run execution；仍不能进入正式训练或 selection。

## 2026-05-26 Phase 3.44 guarded active scoped probe dry-run execution 证据评审

- 本轮只允许 quarantine-only 最小探针；未修改代码、schema、脚本、tools 或 gate。
- 构建和自测通过，取得 `BUILD_BENCHMARK_AND_SELFTEST_OK_PHASE3_44`。
- 使用 Phase 3.43 config 上限：`opening_drift_penalty_weight=0.01`、`max_games=1`、`max_depth=1`、seed allowlist `20260423`。
- 原无输出路径命令被拒绝，错误为 `output weight path is required`；这说明当前 `bitboard-train` 入口还没有 no-output probe 模式。
- 使用显式 quarantine 输出路径复跑后，命令完成，且 JSON 显示 config valid、scoped config true、skeleton enabled true。
- 关键 blocker：JSON 显示 `active_objective_probe_executed=false`、`opening_safe_objective_enabled=false`、`opening_drift_penalty_active=false`、`active_interface_config_status=skeleton_enabled_no_active_probe`。
- 因此 Phase 3.44 不能宣称 active objective probe 已执行；当前接口仍停在 skeleton-only。
- Quarantine 目录内生成 `probe-command-output.json`、`probe-command-error.txt`、`probe-quarantine-output.weights`；未生成 `.bin`，未生成 selection/candidate/checkpoint-like 文件。
- 取得 `WORDING_AND_PROTECTED_DIFF_OK_PHASE3_44` 和 `BLOCKER_STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44`；原成功态 `STATUS_FLAGS_AND_ISOLATION_OK_PHASE3_44` 未取得。
- 建议进入 Phase 3.45 active scoped probe evidence review / blocker review，先评审是否需要 no-output probe mode 或真正 active probe wiring，别继续往训练/selection 上拧。

## 2026-05-26 Phase 3.45 active scoped probe blocker review 证据评审

- 本轮只做 blocker/evidence review；未改代码、schema、CLI 或训练逻辑。
- Phase 3.44 输入证据可读，取得 `BLOCKER_INPUTS_OK_PHASE3_45`。
- Phase 3.44 blocker 状态确认，取得 `PHASE3_44_BLOCKER_STATUS_CONFIRMED`。
- 直接 root cause：accepted active scoped config 只记录 skeleton state，没有接到 report-only active probe path。
- 直接证据：`active_interface_skeleton_enabled=true`、`active_objective_probe_executed=false`、`opening_safe_objective_enabled=false`、`opening_drift_penalty_active=false`、`active_interface_config_status=skeleton_enabled_no_active_probe`。
- 执行语义风险：当前 `bitboard-train` 没有 no-output probe mode，原命令被 `output weight path is required` 拒绝。
- `probe-quarantine-output.weights` 只能作为 quarantine evidence；禁止进入 selection、release、promotion 或 gate。
- 最小补齐方案：Phase 3.46 做 active probe wiring plan review，并把 no-output probe mode 作为强制执行安全项；只在计划接受后再谈实现。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`。

## 2026-05-26 Phase 3.46 active probe wiring and no-output mode plan review 证据评审

- 本轮只做 wiring/no-output mode plan review；未改代码、schema、CLI 或训练逻辑。
- 计划保留单一 `--config <path>` 入口，禁止新增散乱 objective toggle。
- Future wiring 要求 accepted active scoped config 路由到 report-only probe path；`active_objective_probe_executed=true` 只能由真实 probe path 设置。
- Future no-output mode 要求 probe-only 执行不调用 weight save、checkpoint save、candidate manifest save 或 production artifact 代码。
- 计划新增 config 语义仅作为后续评审项：`probe_only=true`、`no_output_weights=true`。
- 验证矩阵覆盖 default-off、invalid config、skeleton-only、probe-wired report-only execution、非 opening scope 拒绝、gate isolation、protected scripts/tools 不变。
- Rollback triggers 覆盖任何 `.bin`、`.weights`、weights manifest、checkpoint、candidate-like artifact，或 active probe 输出接入 gate/rating/scorecard/perf threshold。
- 建议进入 Phase 3.47 guarded active probe wiring skeleton；不允许直接 probe execution。

## 2026-05-26 Phase 3.47 guarded active probe wiring skeleton 证据评审

- 本轮实现 wiring skeleton 与 no-output mode skeleton；没有执行真实 active probe。
- 单一 `--config <path>` 入口保持不变，没有新增 `--opening` / `--objective` / `--active-objective` / `--enable-objective`。
- `ActiveObjectiveConfigSkeleton` 增加 `probe_wiring_skeleton`、`no_output_probe_mode`、`weight_artifact_suppressed` 三个状态位。
- Config parser 接受 `probe_only=true` 与 `no_output_weights=true`，且 `no_output_weights=true` 必须绑定 `probe_only=true`。
- `RunBitboardTraining` 只在 valid no-output probe mode 下放行空 output path；该路径只返回 summary，不保存 weights，不保存 checkpoint。
- Summary/CLI 输出新增 wiring/no-output/weight-suppressed 状态字段，但 `active_objective_probe_executed` 固定为 `false`。
- 自测覆盖 no-output、active probe not executed、weight suppressed；构建和 selftest 已取得 `BUILD_AND_SELFTEST_OK_PHASE3_47`。
- 静态边界已取得 `WIRING_SKELETON_AND_NO_OUTPUT_BOUNDARY_OK_PHASE3_47`；manifest 状态已取得 `STATUS_FLAGS_OK_PHASE3_47`；保护路径和 artifact 边界已取得 `WORDING_ARTIFACT_AND_PROTECTED_DIFF_OK_PHASE3_47`。
- Phase 3 仍 `not_passed`，Phase 4 仍 `blocked`；下一步应进入 Phase 3.48 guarded active probe wiring verification package，先验证骨架，别急着开阀门。
