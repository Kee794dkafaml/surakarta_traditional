# 进度日志

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
