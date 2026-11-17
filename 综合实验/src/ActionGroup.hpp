#pragma once
#include <list>

namespace adas {
class PoseHandler;

// 原子动作：把每个动作拆到“一格的移动 / 90 度的转向 / 状态切换”这一粒度，
// 复杂指令（如 TR、加速指令）由这些原子动作编排而成。
enum class ActionType : unsigned {
    FORWARD_1_STEP_ACTION = 0,
    BACKWARD_1_STEP_ACTION,
    TURNLEFT_ACTION,
    REVERSE_TURNLEFT_ACTION,
    TURNRIGHT_ACTION,
    REVERSE_TURNRIGHT_ACTION,
    BE_FAST_ACTION,
    BE_REVERSE_ACTION,
};

// 原子动作序列：负责收集与批量执行。
class ActionGroup final {
public:
    ActionGroup() = default;
    explicit ActionGroup(const std::list<ActionType>& actions) noexcept;
    ~ActionGroup() = default;

public:
    void PushAction(const ActionType actionType) noexcept;
    void DoOperate(PoseHandler& poseHandler) const noexcept;

private:
    std::list<ActionType> actions;
};
} // namespace adas
