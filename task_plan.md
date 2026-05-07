# 任务计划：Phase 4 方案 B 战术绞肉机路线规划

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
- **separate rule-semantics debt：** legacy `max_no_capture_round` / `no_capture_ply` 不是大赛第 6 条“双方都不能再吃子”的严格实现；本轮不偷改终局语义。后续若要严格对齐，需要单独设计 golden cases 并修改 terminal semantics。

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
