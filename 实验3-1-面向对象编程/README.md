# 实验3-1-面向对象编程

对应需求：`需求1-基本控制指令.md`、`需求2-支持加速功能.md`（本阶段不新增功能，只做结构重构）。
对应课件：`3-实验课件/实验3-1-面向对象编程.pptx`。

本阶段任务：抽离 `PoseHandler` 解耦指令与执行器；用 STL 容器 + 智能指针 + 表驱动重构，保留已有功能。

## 目录结构

```
实验3-1-面向对象编程/
├── CMakeLists.txt
├── include/Executor.hpp
├── src/
│   ├── PoseHandler.hpp / .cpp   # 车状态：位置、朝向、加速状态
│   ├── Command.hpp              # ICommand + 4 个指令类（只依赖 PoseHandler）
│   ├── ExecutorImpl.hpp / .cpp  # 表驱动分派 + 查询
│   ├── Executor.cpp             # 工厂
│   └── CMakeLists.txt
├── tests/
│   ├── ExecutorRefactorTest.cpp # 重构后的行为回归（21 条）
│   ├── PoseEq.hpp / .cpp
│   └── CMakeLists.txt
└── script/
```

## 设计要点

- **解耦循环依赖**：实验2 中 `ICommand::DoOperate(ExecutorImpl&)` 让 Command 依赖 ExecutorImpl，
  而 ExecutorImpl 又持有命令对象，形成循环依赖。本阶段把车状态抽到 `PoseHandler`，
  指令只依赖 `PoseHandler`，依赖方向变成单向。
- **表驱动**：`Execute` 用 `std::unordered_map<char, std::unique_ptr<ICommand>>` 保存
  “字符 → 指令对象”，新增指令只需在表中加一行，`ExecutorImpl` 的主干逻辑不再变化（圈复杂度降到 O(1) 分支）。
- **智能指针**：指令对象用 `std::unique_ptr` 管理；由于 `unique_ptr` 不可拷贝，
  建表必须用 `emplace` 而不能用初始化列表（否则编译报
  `use of deleted function std::pair<...>::pair(const std::pair<...>&)`）——这一点在实验3-2 换成
  `std::function` 后才可以用初始化列表优雅表达。
- **为实验4 留出优化点**：当前每次 `Execute` 都会重建指令表，这正是实验4 用
  “单体工厂 `CmderFactory` + `Singleton`”消除的开销。

## 测试（21 条，全部为回归防护网）

| 覆盖内容 | 说明 |
| --- | --- |
| 初始化 / 默认值 | 2 条 |
| M × 4 朝向 | 4 条 |
| L × 4 朝向 | 4 条 |
| R × 4 朝向 | 4 条 |
| F 加速（FM / FL / FR / FFM） | 4 条 |
| 批量指令（`MLMMRM`、`FMLMR`） | 2 条 |
| 用例独立性 | 1 条 |

重构的验收标准就是“21 条用例与实验1、实验2 的结果完全一致”。

## 构建与运行

```bat
script\build_and_run.bat
```

```bash
bash script/build_and_run.sh
```

依赖与 GoogleTest 的放置方式同前两个实验；Windows 下请把工程放在 ASCII 路径构建。

## 与前后实验的关系

`PoseHandler` 的位置/朝向若继续用 if-else 处理，圈复杂度仍然偏高——这由实验3-2 用
`Point` + `Direction` 的编码与取模运算解决，同时实验3-2 会把 `ICommand` 换成函数对象。
