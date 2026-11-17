#include "ExecutorImpl.hpp"

#include <new>

namespace adas {
// 实验3-1-面向对象编程：工厂方法返回分层重构后的实现。
// 调用方负责在不再使用时 delete 返回的 executor。
Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);
}
} // namespace adas
