#pragma once
#include "Executor.hpp"
#include "PoseHandler.hpp"

namespace adas {
// 实验3-1：ExecutorImpl 只剩两个职责——分派指令、暴露查询，
// 车状态和数据变更全部下沉到 PoseHandler 与 Command 层。
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
    PoseHandler poseHandler;
};
} // namespace adas
