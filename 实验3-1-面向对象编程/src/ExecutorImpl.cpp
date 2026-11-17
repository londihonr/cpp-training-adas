#include "ExecutorImpl.hpp"

#include <memory>
#include <unordered_map>

#include "Command.hpp"

namespace adas {
ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : poseHandler(pose)
{
}

void ExecutorImpl::Execute(const std::string& commands) noexcept
{
    // 表驱动：指令字符 -> 指令对象，新增指令只需在表中增加一行。
    // 注意 unique_ptr 不可拷贝，因此用 emplace 而不是初始化列表构造。
    std::unordered_map<char, std::unique_ptr<ICommand>> cmderMap;
    cmderMap.emplace('M', std::make_unique<MoveCommand>());
    cmderMap.emplace('L', std::make_unique<TurnLeftCommand>());
    cmderMap.emplace('R', std::make_unique<TurnRightCommand>());
    cmderMap.emplace('F', std::make_unique<FastCommand>());

    for (const auto command : commands) {
        const auto it = cmderMap.find(command);
        if (it != cmderMap.end()) {
            it->second->DoOperate(poseHandler);
        }
    }
}

Pose ExecutorImpl::Query() const noexcept
{
    return poseHandler.Query();
}
} // namespace adas
