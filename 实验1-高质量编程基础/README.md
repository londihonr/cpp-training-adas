# 实验1-高质量编程基础

对应需求：`2-需求文档/需求1-基本控制指令.md`（初始化与查询、M/L/R、批量执行）。
对应课件：`3-实验课件/实验1-高质量编程基础.pptx`。

## 目录结构

```
实验1-高质量编程基础/
├── CMakeLists.txt          # 顶层构建：C++17 + enable_testing
├── include/
│   └── Executor.hpp        # 对外接口（Pose、Executor 抽象类、工厂）
├── src/
│   ├── ExecutorImpl.hpp    # 实现类声明，仅实现目录内可见
│   ├── ExecutorImpl.cpp    # M/L/R 与批量执行的具体实现
│   ├── Executor.cpp        # 工厂 Executor::NewExecutor
│   └── CMakeLists.txt      # 生成静态库 training
├── tests/
│   ├── ExecutorTest.cpp    # 需求1的测试防护网（19 条用例）
│   ├── PoseEq.hpp          # Pose 的 == / PrintTo，便于断言与失败定位
│   └── CMakeLists.txt      # 生成测试可执行文件 training_main
└── script/
    ├── build.bat / build.sh
    └── build_and_run.bat / build_and_run.sh
```

## 实现要点

- `ExecutorImpl` 用 PIMPL 式分层：外部只依赖 `include/Executor.hpp`，实现细节留在 `src/`，
  符合课件“头文件最小可见原则”；`Execute`/`Query` 均为 `noexcept` 且不抛异常。
- 位置更新按课件的正交分解实现：X 轴对应 E/W，Y 轴对应 N/S，M 在 4 个朝向下分别
  `x+1 / x-1 / y+1 / y-1`；L、R 只改变 heading。
- 批量指令在 `Execute` 内逐字符分发；多次调用 `Execute` 会在同一状态上累加。
- 未初始化时工厂默认值为 `(0, 0, 'N')`，与需求一致。
- 代码以 `-Wall -Wextra -Werror -Wnon-virtual-dtor -Woverloaded-virtual` 编译零告警。

## 测试防护网（19 条，第 29 页正交分解表全覆盖）

| 场景 | 用例（tests/ExecutorTest.cpp） |
| --- | --- |
| 初始化 / 默认值 | `should_return_init_pose_when_without_command`、`should_return_default_pose_when_without_init_and_command`、`should_return_init_pose_when_command_is_empty` |
| M × 4 朝向 | `should_return_x_plus_1_..._facing_is_E`、`should_return_x_minus_1_..._facing_is_W`、`should_return_y_plus_1_..._facing_is_N`、`should_return_y_minus_1_..._facing_is_S` |
| L × 4 朝向 | `should_return_facing_N/W/S/E_given_command_is_L_...` |
| R × 4 朝向 | `should_return_facing_S/W/N/E_given_command_is_R_...` |
| 批量执行 | `should_return_expected_pose_given_batch_commands`（`MLMMRM` → `(-2,2,'N')`）、`should_keep_position_given_command_only_contains_turns`、`should_accumulate_pose_across_multiple_execute_calls` |
| 用例独立性（FIRST） | `should_not_share_state_between_executors` |

用例命名沿用课件“业务场景清晰”的 `should_..._given_...` 风格，正文用 given/when/then 结构。

## 构建与运行

依赖：CMake ≥ 3.14、支持 C++17 的 g++（如 MinGW-w64 8.1）、GoogleTest 1.12.1（课程提供版本）。

本目录**不携带第三方框架源码**，按课程约定把 `cpp-training-start.zip` 里的
`tests/googletest` 整个目录拷贝到本目录的 `tests/` 下即可；若本机已安装 GoogleTest，
`tests/CMakeLists.txt` 会自动回退到 `find_package(GTest)`。

```bat
:: Windows
script\build_and_run.bat
```

```bash
# Linux / macOS
bash script/build_and_run.sh
```

手动构建：

```bash
cmake -S . -B build -G "MinGW Makefiles"   # Linux/macOS 用 -G "Unix Makefiles"
cmake --build build -j 4
ctest --test-dir build --output-on-failure
```

### 注意：工程路径不要含中文

Windows 下 `mingw32-make` 无法解析非 ASCII 路径（实测会报
`No rule to make target .../实验1-.../src/Executor.cpp`），这与代码无关。
按课件第 38 页的建议，在 ASCII 路径下开发构建，例如 `D:\workspace\cpp-training`；
或改用 Ninja / MSVC 生成器。本实现已在 ASCII 路径下完整构建并跑通全部 19 条用例。

## 与后续实验的衔接

本阶段刻意保持 `ExecutorImpl` 简单（switch 分发），把“命令封装为类 + 继承”留给实验2、
“PoseHandler 抽离 + STL/表驱动”留给实验3、“std::function 与原子动作编排”留给实验4，
以便后续实验有真实的重构对象，而不是无代码可重构。
