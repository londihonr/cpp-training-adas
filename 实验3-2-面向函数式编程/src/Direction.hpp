#pragma once
#include "Point.hpp"

namespace adas {
// 实验3-2：方向抽象。
// 4 个方向被编码为 0..3，左转/右转退化为 (index + 3) % 4 与 (index + 1) % 4，
// 状态流转从 4 个 if-else 分支变成一次取模运算。
class Direction final {
public:
    static const Direction& GetDirection(const char heading) noexcept;

public:
    Direction(const unsigned index, const char heading) noexcept;
    ~Direction() = default;

public:
    const Point& Move() const noexcept;
    const Direction& LeftOne() const noexcept;
    const Direction& RightOne() const noexcept;
    char GetHeading() const noexcept;

private:
    unsigned index;
    char heading;
};
} // namespace adas
