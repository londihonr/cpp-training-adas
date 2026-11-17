#pragma once
#include "Executor.hpp"

namespace adas {
// 实验1：需求1（初始化、M/L/R、批量执行）的具体实现。
// 只在实现目录内部可见，外部使用方仅依赖 include/Executor.hpp。
class ExecutorImpl final : public Executor {
public:
    explicit ExecutorImpl(const Pose& pose) noexcept;
    ~ExecutorImpl() noexcept override = default;
    ExecutorImpl(const ExecutorImpl&) = delete;
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override;

private:
    void DoCommand(char command) noexcept;
    void MoveOneStep() noexcept;
    void TurnLeft() noexcept;
    void TurnRight() noexcept;

private:
    Pose pose;
};
} // namespace adas
