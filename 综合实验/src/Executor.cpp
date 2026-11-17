#include "ExecutorImpl.hpp"

#include <new>

namespace adas {
// 综合实验：工厂按车辆类型创建实现。
// 调用方负责在不再使用时 delete 返回的 executor。
Executor* Executor::NewExecutor(const Pose& pose, VehicleType type) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose, type);
}
} // namespace adas
