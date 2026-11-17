#include "ExecutorImpl.hpp"

#include <new>

namespace adas {
// 实验1-高质量编程基础：工厂方法返回需求1的完整实现。
// 调用方负责在不再使用时 delete 返回的 executor。
Executor* Executor::NewExecutor(const Pose& pose) noexcept
{
    return new (std::nothrow) ExecutorImpl(pose);
}
} // namespace adas
