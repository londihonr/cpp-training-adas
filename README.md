# C++ 企业软件开发实践（华为共建课程）实验作业

学号 / 姓名：20260001 / 张三

本仓库是课程四个实验（共 5 个工程目录）的完整实现，按实验顺序逐步演进：
同一个 ADAS `Executor` 组件从过程式代码重构为面向对象、再到函数式与面向泛型的设计。

## 实验与目录

| 实验 | 目录 | 课件 | 主要任务 | 测试 |
| --- | --- | --- | --- | --- |
| 实验1 | `实验1-高质量编程基础/` | 实验1-高质量编程基础.pptx | 初始化/查询、M/L/R、批量执行 + 开发者测试 | 19 |
| 实验2 | `实验2-面向对象编程/` | 实验2-面向对象编程.pptx | 封装命令、`ICommand` 继承与多态、F 加速 | 25 |
| 实验3（上） | `实验3-1-面向对象编程/` | 实验3-1-面向对象编程.pptx | 抽离 `PoseHandler` 解耦循环依赖、表驱动 + STL/智能指针 | 21 |
| 实验3（下） | `实验3-2-面向函数式编程/` | 实验3-2-面向函数式编程.pptx | `std::function` 与 `operator()`、状态编码简化、B 倒车与 B/F 组合 | 20 |
| 实验4 | `综合实验/` | 实验4-综合实战.pptx | 泛型单体工厂、原子动作编排、TR 掉头、跑车/Bus 扩展 | 46 |

合计 **131 条测试用例，全部通过**（MinGW-w64 g++ 8.1 / C++17 / GoogleTest 1.12.1）。

## 演进脉络

| 阶段 | 指令分派方式 | 状态承载 | 新增能力 |
| --- | --- | --- | --- |
| 实验1 | `switch` 过程式分发 | `ExecutorImpl::pose` | M / L / R |
| 实验2 | `unique_ptr<ICommand>` 多态 | `ExecutorImpl::pose` + `fast` | F |
| 实验3-1 | 表驱动 `unordered_map<char, unique_ptr<ICommand>>` | `PoseHandler` | 无（纯重构，回归守护） |
| 实验3-2 | 表驱动 `unordered_map<char, std::function<...>>` | `PoseHandler`（`Point` + `Direction`） | B、B+F |
| 实验4 | 单体工厂 `CmderFactory` + 原子动作 `ActionGroup` | `PoseHandler` | TR、跑车、Bus |

## 构建与测试

每个实验目录都是独立可构建的工程，步骤相同：

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
cmake -S . -B build -G "MinGW Makefiles"      # Linux/macOS 用 -G "Unix Makefiles"
cmake --build build -j 4
ctest --test-dir build --output-on-failure
```

约定：

- GoogleTest 使用课程提供版本（`cpp-training-start.zip` 的 `tests/googletest`），
  按课程要求**未把第三方框架源码放入仓库**；构建前把它拷贝到对应实验目录的 `tests/` 下即可。
  若本机已安装 GoogleTest，`tests/CMakeLists.txt` 会自动回退到 `find_package(GTest)`。
- Windows 下 `mingw32-make` 无法解析含中文的工程路径（会报
  `No rule to make target .../实验1-.../src/Executor.cpp`），
  请按课件第 38 页在 ASCII 路径（如 `D:\workspace\cpp-training`）构建，或改用 Ninja / MSVC 生成器。
  仓库内代码已在该限制下验证通过。

## 每个实验目录的内容

```
<实验目录>/
├── CMakeLists.txt          # 顶层构建（C++17 + ctest）
├── README.md               # 本阶段任务、设计要点、测试清单
├── include/                # 对外接口（最小可见）
├── src/                    # 实现（按状态/动作/指令/工厂分层）
├── tests/                  # GoogleTest 用例 + PoseEq 辅助
└── script/                 # build / build_and_run
```
