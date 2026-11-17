#pragma once
#include "Executor.hpp"
#include "PoseHandler.hpp"

namespace adas {
// 综合实验：ExecutorImpl 只剩下“取指令列表 -> 逐个执行原子动作编排”的职责，
// 车状态在 PoseHandler，指令解析在 CmderFactory，动作执行在 ActionGroup。
class ExecutorImpl final : public Executor {
public:
    ExecutorImpl(const Pose& pose, VehicleType vehicleType) noexcept;
    ~ExecutorImpl() noexcept override = default;
    ExecutorImpl(const ExecutorImpl&) = delete;
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override;

private:
    PoseHandler poseHandler;
    VehicleType vehicleType;
};
} // namespace adas
