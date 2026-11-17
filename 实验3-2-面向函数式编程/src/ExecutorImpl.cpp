#include "ExecutorImpl.hpp"

#include <unordered_map>

#include "Command.hpp"

namespace adas {
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : poseHandler(pose)
{
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    // 表驱动 + 初始化语义：std::function 可拷贝，因此可以直接用初始化列表建表。
    const std::unordered_map<char, Command> cmderMap{
        {'M', MoveCommand()},
        {'L', TurnLeftCommand()},
        {'R', TurnRightCommand()},
        {'F', FastCommand()},
        {'B', ReverseCommand()},
    };

    for (const auto command : commands) {
        const auto it = cmderMap.find(command);
        if (it != cmderMap.end()) {
            it->second(poseHandler);
        }
    }
}

Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
} // namespace adas
