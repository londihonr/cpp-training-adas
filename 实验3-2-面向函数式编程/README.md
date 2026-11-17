# 实验3-2-面向函数式编程

对应需求：`需求3-支持倒车功能.md`（新增 B 指令及 B/F 组合）。
对应课件：`3-实验课件/实验3-2-面向函数式编程.pptx`。

本阶段任务：用 `std::function`、函数对象与 `operator()` 改造命令，保证对象生命周期安全；
简化状态变化；完成 B 指令与 B/F 组合的测试。

## 目录结构

```
实验3-2-面向函数式编程/
├── CMakeLists.txt
├── include/Executor.hpp
├── src/
│   ├── Point.hpp / .cpp         # 位置值对象（operator+= / -=）
│   ├── Direction.hpp / .cpp     # 方向抽象：编码 0..3 + 取模转向
│   ├── PoseHandler.hpp / .cpp   # Point + Direction + fast/reverse 状态
│   ├── Command.hpp              # 函数对象形式的命令（operator()）
│   ├── ExecutorImpl.hpp / .cpp  # 表驱动分派（std::function）
│   ├── Executor.cpp             # 工厂
│   └── CMakeLists.txt
├── tests/
│   ├── ExecutorReverseTest.cpp  # 需求3 + M/L/R/F 回归（20 条）
│   ├── PoseEq.hpp / .cpp
│   └── CMakeLists.txt
└── script/
```

## 设计要点

- **删除 `ICommand` 继承体系**：命令改为带 `operator()` 的轻量函数对象，由
  `std::function<void(PoseHandler&)>` 统一存放，`Execute` 里只剩一次查表调用：
  没有虚函数表、没有堆上的命令对象。
- **生命周期安全**：命令对象不捕获任何引用/指针（无 Lambda 捕获），
  避免了“保存了指向已销毁对象的捕获”这类悬垂引用问题；`std::function` 可拷贝，
  因此指令表可以直接用初始化列表构建（对比实验3-1 的 `emplace`）。
- **状态变化简化**：`Direction` 把 4 个方向编码为 `0..3`（E→S→W→N），
  左转变成 `(index + 3) % 4`、右转变成 `(index + 1) % 4`；
  移动变成 `point += facing->Move()`，倒车变成 `point -= facing->Move()`。
  原来的“4 方向 × 3 操作”共 12 个 if-else 分支被压缩成两次取模和一次向量加减。
- **B 指令语义**：倒车状态下前进/后退互换、左转/右转互换；B 与 F 可叠加，
  加速 + 倒车时 `M` 倒退两格、`L` 先倒退一格再右转、`R` 先倒退一格再左转。

## 测试（20 条）

| 状态 | 用例 |
| --- | --- |
| `B` | `BM` × 4 朝向（E/N/S/W 的倒退方向）、`BL`（E→S）、`BR`（E→N） |
| `BF` | `BFM`（倒退 2 格）、`BFL`（倒退 1 格 + 右转）、`BFR`（倒退 1 格 + 左转） |
| 状态取消 | `BBM`（E/N 两个朝向）、`FFM` |
| 跨调用 | `B` 之后单独 `Execute("M")` 仍保持倒车状态 |
| 回归 | `M`、`L`、`R`、`FM`、`FL`、`FR`、批量 `MLMMRM` |

测试用例与课件“B&F 指令正交分解的测试用例”表逐格对应。

## 构建与运行

```bat
script\build_and_run.bat
```

```bash
bash script/build_and_run.sh
```

依赖与 GoogleTest 的放置方式同前；Windows 下请把工程放在 ASCII 路径构建。

## 与前后实验的关系

本阶段的 `PoseHandler` / `Point` / `Direction` 与表驱动结构会被综合实验（实验4）直接复用，
实验4 在此基础上加入单体指令工厂与原子动作编排（TR、跑车/Bus 扩展）。
