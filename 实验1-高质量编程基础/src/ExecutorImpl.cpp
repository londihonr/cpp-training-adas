#include "ExecutorImpl.hpp"

namespace adas {
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose)
{
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto command : commands) {
        DoCommand(command);
    }
}

Pose ExecutorImpl::Query() const noexcept
{
    return pose;
}

void ExecutorImpl::DoCommand(const char command) noexcept
{
    switch (command) {
        case 'M':
            MoveOneStep();
            break;
        case 'L':
            TurnLeft();
            break;
        case 'R':
            TurnRight();
            break;
        default:
            // 需求1约束：调用方保证传入的指令合法，此处不做异常处理。
            break;
    }
}

void ExecutorImpl::MoveOneStep() noexcept
{
    // 正交分解：X 轴对应 EW 方向，Y 轴对应 NS 方向。
    switch (pose.heading) {
        case 'E':
            ++pose.x;
            break;
        case 'W':
            --pose.x;
            break;
        case 'N':
            ++pose.y;
            break;
        case 'S':
            --pose.y;
            break;
        default:
            break;
    }
}

void ExecutorImpl::TurnLeft() noexcept
{
    switch (pose.heading) {
        case 'E':
            pose.heading = 'N';
            break;
        case 'N':
            pose.heading = 'W';
            break;
        case 'W':
            pose.heading = 'S';
            break;
        case 'S':
            pose.heading = 'E';
            break;
        default:
            break;
    }
}

void ExecutorImpl::TurnRight() noexcept
{
    switch (pose.heading) {
        case 'E':
            pose.heading = 'S';
            break;
        case 'S':
            pose.heading = 'W';
            break;
        case 'W':
            pose.heading = 'N';
            break;
        case 'N':
            pose.heading = 'E';
            break;
        default:
            break;
    }
}
} // namespace adas
