#include "PoseEq.hpp"

#include <ostream>
#include <tuple>

namespace adas {
bool operator==(const Pose& lhs, const Pose& rhs) noexcept
{
    return std::tie(lhs.x, lhs.y, lhs.heading) == std::tie(rhs.x, rhs.y, rhs.heading);
}

bool operator!=(const Pose& lhs, const Pose& rhs) noexcept
{
    return !(lhs == rhs);
}

void PrintTo(const Pose& pose, std::ostream* os)
{
    *os << "{x=" << pose.x << ", y=" << pose.y << ", heading=" << pose.heading << "}";
}
} // namespace adas
