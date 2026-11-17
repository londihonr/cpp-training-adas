#pragma once
#include "ActionGroup.hpp"
#include "Executor.hpp"
#include "PoseHandler.hpp"

namespace adas {
// 车辆类型差异：把“跑车 / Bus / 普通车”的差异收敛成三个数据项，
// 指令编排代码中不再出现按车型分支的 if-else。
struct VehicleBehaviour final {
    unsigned forwardSteps;  // M 指令走的格数
    unsigned turnSteps;     // L/R 指令附带的前进（或倒车）格数
    bool moveBeforeTurn;    // L/R 指令是先移动再转向，还是先转向再移动
};

constexpr VehicleBehaviour GetVehicleBehaviour(const VehicleType vehicleType) noexcept
{
    switch (vehicleType) {
        case VehicleType::Sport:
            return VehicleBehaviour{2, 1, false}; // 跑车：M 走 2 格；L/R 先转向再走 1 格
        case VehicleType::Bus:
            return VehicleBehaviour{1, 1, true}; // Bus：M 走 1 格；L/R 先走 1 格再转向
        default:
            return VehicleBehaviour{1, 0, true}; // 普通车：L/R 只转向
    }
}

namespace commanddetail {
inline ActionType MoveAction(const PoseHandler& poseHandler) noexcept
{
    return poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION : ActionType::FORWARD_1_STEP_ACTION;
}

inline void PushMoves(ActionGroup& actionGroup, const ActionType moveAction, const unsigned steps) noexcept
{
    for (unsigned i = 0; i < steps; ++i) {
        actionGroup.PushAction(moveAction);
    }
}

// 加速状态：每个 M/L/R 至少多走一格；M 的格数翻倍（跑车 2 格 -> 4 格）。
inline unsigned FastExtraSteps(const unsigned steps) noexcept
{
    return steps == 0 ? 1 : steps;
}

inline ActionGroup BuildTurn(const PoseHandler& poseHandler, const VehicleBehaviour& behaviour, const bool turnLeft) noexcept
{
    ActionGroup actionGroup;
    const auto moveAction = MoveAction(poseHandler);

    // 倒车时左转变右转、右转变左转
    ActionType turnAction = turnLeft ? ActionType::TURNLEFT_ACTION : ActionType::TURNRIGHT_ACTION;
    if (poseHandler.IsReverse()) {
        turnAction = turnLeft ? ActionType::REVERSE_TURNLEFT_ACTION : ActionType::REVERSE_TURNRIGHT_ACTION;
    }

    if (poseHandler.IsFast()) {
        PushMoves(actionGroup, moveAction, FastExtraSteps(behaviour.turnSteps));
    }
    if (behaviour.moveBeforeTurn) {
        PushMoves(actionGroup, moveAction, behaviour.turnSteps);
    }
    actionGroup.PushAction(turnAction);
    if (!behaviour.moveBeforeTurn) {
        PushMoves(actionGroup, moveAction, behaviour.turnSteps);
    }
    return actionGroup;
}
} // namespace commanddetail

// 每个指令只负责“编排”自己的原子动作序列，不关心动作如何执行。
class MoveCommand final {
public:
    explicit MoveCommand(const VehicleBehaviour& behaviour) noexcept : behaviour(behaviour)
    {
    }

    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        ActionGroup actionGroup;
        const auto moveAction = commanddetail::MoveAction(poseHandler);
        if (poseHandler.IsFast()) {
            commanddetail::PushMoves(actionGroup, moveAction, behaviour.forwardSteps);
        }
        commanddetail::PushMoves(actionGroup, moveAction, behaviour.forwardSteps);
        return actionGroup;
    }

private:
    VehicleBehaviour behaviour;
};

class TurnLeftCommand final {
public:
    explicit TurnLeftCommand(const VehicleBehaviour& behaviour) noexcept : behaviour(behaviour)
    {
    }

    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        return commanddetail::BuildTurn(poseHandler, behaviour, true);
    }

private:
    VehicleBehaviour behaviour;
};

class TurnRightCommand final {
public:
    explicit TurnRightCommand(const VehicleBehaviour& behaviour) noexcept : behaviour(behaviour)
    {
    }

    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        return commanddetail::BuildTurn(poseHandler, behaviour, false);
    }

private:
    VehicleBehaviour behaviour;
};

class FastCommand final {
public:
    ActionGroup operator()(PoseHandler&) const noexcept
    {
        ActionGroup actionGroup;
        actionGroup.PushAction(ActionType::BE_FAST_ACTION);
        return actionGroup;
    }
};

class ReverseCommand final {
public:
    ActionGroup operator()(PoseHandler&) const noexcept
    {
        ActionGroup actionGroup;
        actionGroup.PushAction(ActionType::BE_REVERSE_ACTION);
        return actionGroup;
    }
};

// 需求4：TR 掉头。不受倒车状态影响；加速状态下按“前进->左转->前进->左转”执行。
class TurnRoundCommand final {
public:
    ActionGroup operator()(PoseHandler& poseHandler) const noexcept
    {
        ActionGroup actionGroup;
        if (poseHandler.IsFast()) {
            actionGroup.PushAction(ActionType::FORWARD_1_STEP_ACTION);
            actionGroup.PushAction(ActionType::TURNLEFT_ACTION);
            actionGroup.PushAction(ActionType::FORWARD_1_STEP_ACTION);
            actionGroup.PushAction(ActionType::TURNLEFT_ACTION);
        } else {
            actionGroup.PushAction(ActionType::TURNLEFT_ACTION);
            actionGroup.PushAction(ActionType::FORWARD_1_STEP_ACTION);
            actionGroup.PushAction(ActionType::TURNLEFT_ACTION);
        }
        return actionGroup;
    }
};
} // namespace adas
