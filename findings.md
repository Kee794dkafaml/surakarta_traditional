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
- 当前仓库没有看到 TD/self-play 训练闭环：
  - 只有 `NTupleEvaluator::LoadWeights()` 和权重文件校验
  - 没有发现训练器、数据集导出、自对弈生成器、权重落盘器或训练脚本
- 搜索自测护栏已经比较扎实：
  - `surakarta_bitboard_selftest.cpp` 覆盖了搜索 parity、共享 node budget、共享 stop request、多线程等价、PV 合法性、aspiration 统计、benchmark JSON 输出、静态 mask parity

## 技术决策
| 决策 | 理由 |
|------|------|
| 下一阶段不再把“实现 PVS/TT/LMR/Null Move”列为主任务 | 这些能力代码里已经有了，再写就是自欺欺人 |
| 下一阶段重点应转向“强度提升闭环”而非“搜索框架从零搭建” | 现在最大的价值在于调优、训练、验证，而不是重复搭骨架 |
| 下一阶段必须在“搜索工程提纯”与“TD 训练闭环”之间明确主次 | 两者都重要，但先后顺序不同，任务清单会完全不同 |

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

---
*每执行2次查看/浏览器/搜索操作后更新此文件*
*防止视觉信息丢失*
