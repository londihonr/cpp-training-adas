#include "ExecutorImpl.hpp"

#include <algorithm>

#include "CmderFactory.hpp"
#include "Singleton.hpp"

namespace adas {
ExecutorImpl::ExecutorImpl(const Pose& pose, const VehicleType vehicleType) noexcept
    : poseHandler(pose), vehicleType(vehicleType)
{
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    // 单体工厂只构建一次指令表；每个 Cmder 在执行时才读取当前状态，
    // 因此 F/B 造成的状态变化会被后续指令看到。
    const auto cmders = Singleton<CmderFactory>::Instance().GetCmders(vehicleType, commands);
    std::for_each(cmders.begin(), cmders.end(),
        [this](const Cmder& cmder) noexcept { cmder(poseHandler).DoOperate(poseHandler); });
}

Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
} // namespace adas
