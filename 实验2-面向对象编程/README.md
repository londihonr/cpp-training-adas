# 实验2-面向对象编程

对应需求：`2-需求文档/需求1-基本控制指令.md`、`2-需求文档/需求2-支持加速功能.md`。
对应课件：`3-实验课件/实验2-面向对象编程.pptx`。

本阶段任务：封装移动及转向命令，建立抽象接口 `ICommand` 与继承/多态关系；新增 F 加速指令并补齐测试。

## 目录结构

```
实验2-面向对象编程/
├── CMakeLists.txt
├── include/Executor.hpp        # 对外接口，与实验1相同
├── src/
│   ├── ExecutorImpl.hpp        # ICommand 抽象类 + 4 个命令类（内嵌）
│   ├── ExecutorImpl.cpp        # 只做“命令选择 + 调用 DoOperate”
│   ├── Executor.cpp            # 工厂
│   └── CMakeLists.txt
├── tests/
│   ├── ExecutorTest.cpp        # 需求1 回归（17 条）
│   ├── ExecutorFastTest.cpp    # 需求2 加速指令（8 条）
│   ├── PoseEq.hpp / PoseEq.cpp # Pose 相等比较（声明与定义分离）
│   └── CMakeLists.txt
└── script/                     # build / build_and_run
```

## 设计要点

- **封装**：实验1 中 4 个方向 × 3 条指令的 if-else 被拆进 `MoveCommand` / `TurnLeftCommand` /
  `TurnRightCommand` / `FastCommand`，每个类只实现自己的 `DoOperate`。
- **继承与多态**：`ICommand` 定义统一接口，`Execute` 里只剩“选对象 + 调用”，
  新增指令不再需要改动 `Move/TurnLeft/TurnRight` 的实现。
- **F 状态**：`ExecutorImpl` 用 `bool fast` 保存状态；`MoveCommand` 在加速时先走一格再走一格
  （逐格前进，不跳跃），`TurnLeft/TurnRightCommand` 在加速时先走一格再转向。
- **PoseEq 拆分**：引入第二个测试文件后，`operator==` 若仍写在 `ExecutorTest.cpp` 里会重复定义，
  因此按课件拆成 `PoseEq.hpp`（声明）+ `PoseEq.cpp`（定义），并由 `aux_source_directory` 自动加入测试目标。
- **重构不改变行为**：命令对象虽然改为多态实现，需求1 的行为由 `ExecutorTest.cpp` 全量回归守住。

## 测试（25 条）

| 套件 | 覆盖内容 |
| --- | --- |
| `ExecutorTest`（17 条） | 初始化/默认值、M×4 朝向、L×4 朝向、R×4 朝向、批量指令、跨调用累加、用例独立性 |
| `ExecutorFastTest`（8 条） | `FM`（E/W 两个方向）、`FL`（E/N）、`FR`、`FFM` 取消加速、`FFL`、加速状态跨 Execute 保持 |

用例名沿用 `should_..._given_...` 的业务场景命名，正文为 given/when/then 结构。

## 构建与运行

依赖：CMake ≥ 3.14、支持 C++17 的 g++、GoogleTest 1.12.1（课程提供版本，
把 `cpp-training-start.zip` 的 `tests/googletest` 拷到本目录 `tests/` 下即可；已安装则自动 `find_package`）。

```bat
script\build_and_run.bat
```

```bash
bash script/build_and_run.sh
```

> Windows 下 `mingw32-make` 无法解析含中文的工程路径，请把工程放在 ASCII 路径（如 `D:\workspace\cpp-training`）构建，或改用 Ninja / MSVC 生成器。

## 与前后实验的关系

实验1 的“过程式 if-else 分发”在这里被替换为“多态命令对象”，为实验3-1 的
`PoseHandler` 抽离与表驱动分派提供改造对象。
