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
