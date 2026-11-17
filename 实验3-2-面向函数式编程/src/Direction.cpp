#include "Direction.hpp"

namespace adas {
namespace {
constexpr unsigned kDirectionCount = 4;

// 编码顺序：E -> S -> W -> N（右转 +1，左转 +3，均对 4 取模）。
const Direction kDirections[kDirectionCount] = {
    Direction(0, 'E'),
    Direction(1, 'S'),
    Direction(2, 'W'),
    Direction(3, 'N'),
};

const char kDefaultHeading = 'N';
} // namespace

Direction::Direction(const unsigned index, const char heading) noexcept : index(index), heading(heading)
{
}

const Direction& Direction::GetDirection(const char heading) noexcept
{
    for (const auto& direction : kDirections) {
        if (direction.GetHeading() == heading) {
            return direction;
        }
    }
    return kDirections[3]; // kDefaultHeading
}

const Point& Direction::Move() const noexcept
{
    // ESWN 四个方向上的单位位移。
    static const Point points[kDirectionCount] = {
        Point(1, 0),
        Point(0, -1),
        Point(-1, 0),
        Point(0, 1),
    };
    return points[index];
}

const Direction& Direction::LeftOne() const noexcept
{
    return kDirections[(index + kDirectionCount - 1) % kDirectionCount];
}

const Direction& Direction::RightOne() const noexcept
{
    return kDirections[(index + 1) % kDirectionCount];
}

char Direction::GetHeading() const noexcept
{
    return heading;
}
} // namespace adas
