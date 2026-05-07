# 发现与决策

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

---
*每执行2次查看/浏览器/搜索操作后更新此文件*
*防止视觉信息丢失*
