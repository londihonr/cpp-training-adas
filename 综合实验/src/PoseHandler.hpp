#pragma once
#include "Direction.hpp"
#include "Executor.hpp"

namespace adas {
// 实验3-2：车状态全部收敛到这里。
// 位置用 Point 表达、朝向用 Direction 表达，倒车/加速只维护两个布尔状态，
// 位置与朝向变化的圈复杂度从多路 if-else 降为一次取模/一次向量加减。
class PoseHandler final {
public:
    explicit PoseHandler(const Pose& pose) noexcept;
    PoseHandler(const PoseHandler&) = delete;
    PoseHandler& operator=(const PoseHandler&) = delete;
    ~PoseHandler() = default;

public:
    void Forward() noexcept;
    void Backward() noexcept;
    void TurnLeft() noexcept;
    void TurnRight() noexcept;
    void Fast() noexcept;
    void Reverse() noexcept;
    bool IsFast() const noexcept;
    bool IsReverse() const noexcept;
    Pose Query() const noexcept;

private:
    Point point;
    const Direction* facing;
    bool fast{false};
    bool reverse{false};
};
} // namespace adas
