#include "ExecutorImpl.hpp"

#include <memory>
#include <new>

namespace adas {
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose)
{
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    for (const auto command : commands) {
        // 分派与实现分离：这里只决定“用哪个指令对象”，不再关心指令细节。
        std::unique_ptr<ICommand> cmder;
        if (command == 'M') {
            cmder = std::make_unique<MoveCommand>();
        } else if (command == 'L') {
            cmder = std::make_unique<TurnLeftCommand>();
        } else if (command == 'R') {
            cmder = std::make_unique<TurnRightCommand>();
        } else if (command == 'F') {
            cmder = std::make_unique<FastCommand>();
        }

        if (cmder) {
            cmder->DoOperate(*this);
        }
    }
}

Pose ExecutorImpl::Query() const noexcept
{
    return pose;
}

void ExecutorImpl::Move() noexcept
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

void ExecutorImpl::Fast() noexcept
{
    fast = !fast;
}

bool ExecutorImpl::IsFast() const noexcept
{
    return fast;
}
} // namespace adas
