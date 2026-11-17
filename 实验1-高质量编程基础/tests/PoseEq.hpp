#pragma once
#include <ostream>
#include <tuple>

#include "Executor.hpp"

namespace adas {
// 测试辅助：让 EXPECT_EQ/ASSERT_EQ 可以直接比较 Pose，并在失败时打印可读内容。
inline bool operator==(const Pose& lhs, const Pose& rhs) noexcept
{
    return std::tie(lhs.x, lhs.y, lhs.heading) == std::tie(rhs.x, rhs.y, rhs.heading);
}

inline bool operator!=(const Pose& lhs, const Pose& rhs) noexcept
{
    return !(lhs == rhs);
}

inline void PrintTo(const Pose& pose, std::ostream* os)
{
    *os << "{x=" << pose.x << ", y=" << pose.y << ", heading=" << pose.heading << "}";
}
} // namespace adas
