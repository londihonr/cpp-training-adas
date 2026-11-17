#include "ExecutorImpl.hpp"

#include <new>

namespace adas {
// 实验2-面向对象编程：工厂方法返回封装后的实现。
// 调用方负责在不再使用时 delete 返回的 executor。
Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);
}
} // namespace adas
