#pragma once
#include "Executor.hpp"

namespace adas {
// 实验3-1：把 ExecutorImpl 的车状态抽离出来。
// 指令只依赖 PoseHandler，不再依赖 ExecutorImpl，从而解开 Command <-> ExecutorImpl 的循环依赖。
class PoseHandler final {
public:
    explicit PoseHandler(const Pose& pose) noexcept;
    PoseHandler(const PoseHandler&) = delete;
    PoseHandler& operator=(const PoseHandler&) = delete;
    ~PoseHandler() = default;

public:
    void Move() noexcept;
    void TurnLeft() noexcept;
    void TurnRight() noexcept;
    void Fast() noexcept;
    bool IsFast() const noexcept;
    Pose Query() const noexcept;

private:
    Pose pose;
    bool fast{false};
};
} // namespace adas
