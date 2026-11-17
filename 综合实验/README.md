# 综合实验

对应需求：`需求4-支持掉头功能.md` + `课后综合作业-支持不同的车辆类型.md`。
对应课件：`3-实验课件/实验4-综合实战.pptx`。

本阶段任务：完成泛型与接口相关实践、原子动作编排、TR 掉头、目录整理，
并完成课件末尾的跑车 / Bus 扩展作业（普通、B、F、B+F 四种状态下的 M/L/R 行为及测试）。

## 目录结构

```
综合实验/
├── CMakeLists.txt
├── include/Executor.hpp          # 对外接口 + VehicleType + 带车型的工厂
├── src/
│   ├── Singleton.hpp             # 泛型单体模板
│   ├── Point.hpp / .cpp          # 位置值对象
│   ├── Direction.hpp / .cpp      # 方向编码与取模转向
│   ├── PoseHandler.hpp / .cpp    # 车状态
│   ├── ActionGroup.hpp / .cpp    # 8 种原子动作 + 批量执行
│   ├── Command.hpp               # 各指令的原子动作编排 + 车型差异
│   ├── CmderFactory.hpp / .cpp   # 单体指令工厂：字符/TR -> Cmder
│   ├── ExecutorImpl.hpp / .cpp   # 取指令列表并依次执行
│   ├── Executor.cpp              # 工厂（按 VehicleType 创建）
│   └── CMakeLists.txt
├── tests/
│   ├── ExecutorTurnRoundTest.cpp # 需求4（7 条）
│   ├── VehicleTypeTest.cpp       # 普通/跑车/Bus × M/L/R × 4 状态（39 条）
│   ├── PoseEq.hpp / .cpp
│   └── CMakeLists.txt
└── script/
```

## 设计要点

- **面向接口 + 泛型单体**：`Singleton<T>` 模板复用单例实现，`CmderFactory` 作为单体只构建一次指令表；
  每次 `Execute` 不再重建表（这正是实验3-1 遗留的性能问题）。
- **指令工厂**：`GetCmders` 负责把指令字符串解析成 `Cmder` 列表。
  `TR` 是两字符指令，采用“最长匹配”优先识别 `TR`，再回退到单字符指令表；
  否则 `TR` 会被误拆成 `T` + `R`（右转）。
- **原子动作编排**：把每个动作拆到“移动一格 / 转向 90 度 / 切换状态”的粒度
  （`FORWARD_1_STEP`、`BACKWARD_1_STEP`、`TURNLEFT`、`REVERSE_TURNLEFT`、`TURNRIGHT`、
  `REVERSE_TURNRIGHT`、`BE_FAST`、`BE_REVERSE`），复杂指令只负责“编排”动作序列，
  `ActionGroup::DoOperate` 负责统一执行。
- **TR 语义（以需求4为准）**：不受倒车状态 B 影响；F 或 B+F 状态下走
  “前进 1 格 → 左转 → 前进 1 格 → 左转”的加速掉头规则。
- **车型差异收敛为数据**：跑车/Bus 的差异只有三项——`M` 的格数、`L/R` 附带的移动格数、
  移动发生在转向之前还是之后，全部放进 `VehicleBehaviour`，指令代码里不再出现按车型分支的 if-else。
  加速语义统一为“每个 M/L/R 至少多走一格，M 的格数翻倍”
  （普通/跑车/Bus 的 `M` 分别从 1/2/1 格变为 2/4/2 格）。
- **目录整理**：头文件（`include/`）只放客户需要的最小接口，实现细节全部在 `src/`；
  `src` 内部按“状态 / 动作 / 指令 / 工厂 / 执行器”分层。

## 测试（46 条）

| 套件 | 覆盖内容 |
| --- | --- |
| `ExecutorTurnRoundTest`（7 条） | TR 普通、F、B、B+F 四种状态；`TRTR` 回到原位；批量中的 TR；TR 不被误拆为 T+R |
| `VehicleTypeTest`（39 条） | 普通 / 跑车 / Bus × {无状态, B, F, B+F} × {M, L, R}，共 36 条；TR 与车型无关 2 条；车型 + 状态组合批量 1 条 |

车型相关用例与课件第 55 页作业的预期结果逐条对应，例如：
`跑车 FL = (1,1,'N')`、`Bus BFL = (-2,0,'S')`、`跑车 FM = 4 格`。

## 构建与运行

```bat
script\build_and_run.bat
```

```bash
bash script/build_and_run.sh
```

依赖与 GoogleTest 的放置方式同前；Windows 下请把工程放在 ASCII 路径构建。
