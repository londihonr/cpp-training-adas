#pragma once

namespace adas {
// 实验3-2：把位置的二维变化封装成值对象，
// 使 4 个方向上的位置变化统一为一个模型（Point 的加减），消除 if-else 分支。
class Point final {
public:
    Point(const int x, const int y) noexcept;
    Point(const Point& rhs) noexcept = default;
    Point& operator=(const Point& rhs) noexcept = default;
    ~Point() = default;

public:
    Point& operator+=(const Point& rhs) noexcept;
    Point& operator-=(const Point& rhs) noexcept;

public:
    int GetX() const noexcept;
    int GetY() const noexcept;

private:
    int x;
    int y;
};
} // namespace adas
