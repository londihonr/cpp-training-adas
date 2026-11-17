#include "ActionGroup.hpp"

#include <array>
#include <cstddef>
#include <functional>

#include "PoseHandler.hpp"

namespace adas {
namespace {
class ForwardAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Forward();
    }
};

class BackwardAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Backward();
    }
};

class TurnLeftAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnLeft();
    }
};

// 反左转 = 右转（倒车状态下左转的等价操作）
class ReverseTurnLeftAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnRight();
    }
};

class TurnRightAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnRight();
    }
};

// 反右转 = 左转（倒车状态下右转的等价操作）
class ReverseTurnRightAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.TurnLeft();
    }
};

class BeFastAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Fast();
    }
};

class BeReverseAction final {
public:
    void operator()(PoseHandler& poseHandler) const noexcept
    {
        poseHandler.Reverse();
    }
};

constexpr std::size_t kActionCount = 8;

using Action = std::function<void(PoseHandler&)>;

const std::array<Action, kActionCount>& GetActions() noexcept
{
    // 动作表只生成一次：下标即 ActionType 的枚举值。
    static const std::array<Action, kActionCount> actions{
        ForwardAction(),
        BackwardAction(),
        TurnLeftAction(),
        ReverseTurnLeftAction(),
        TurnRightAction(),
        ReverseTurnRightAction(),
        BeFastAction(),
        BeReverseAction(),
    };
    return actions;
}
} // namespace

ActionGroup::ActionGroup(const std::list<ActionType>& actions) noexcept : actions(actions)
{
}

void ActionGroup::PushAction(const ActionType actionType) noexcept
{
    actions.push_back(actionType);
}

void ActionGroup::DoOperate(PoseHandler& poseHandler) const noexcept
{
    const auto& actionsImpl = GetActions();
    for (const auto actionType : actions) {
        actionsImpl[static_cast<std::size_t>(actionType)](poseHandler);
    }
}
} // namespace adas
