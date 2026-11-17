#include "CmderFactory.hpp"

namespace adas {
namespace {
constexpr std::size_t ToIndex(const VehicleType vehicleType) noexcept
{
    return static_cast<std::size_t>(vehicleType);
}

constexpr char kTurnRoundFirstChar = 'T';
constexpr char kTurnRoundSecondChar = 'R';
} // namespace

CmderFactory::CmderFactory() noexcept
{
    for (std::size_t index = 0; index < kVehicleTypeCount; ++index) {
        const auto vehicleType = static_cast<VehicleType>(index);
        const auto behaviour = GetVehicleBehaviour(vehicleType);
        auto& cmderMap = cmderMaps[index];
        cmderMap.emplace('M', MoveCommand(behaviour));
        cmderMap.emplace('L', TurnLeftCommand(behaviour));
        cmderMap.emplace('R', TurnRightCommand(behaviour));
        cmderMap.emplace('F', FastCommand());
        cmderMap.emplace('B', ReverseCommand());
    }
}

CmderList CmderFactory::GetCmders(const VehicleType vehicleType, const std::string& commands) const noexcept
{
    CmderList cmders;
    const auto& cmderMap = cmderMaps[ToIndex(vehicleType)];

    // 最长匹配：先识别两字符的 TR，再回退到单字符指令。
    for (std::size_t index = 0; index < commands.size(); ++index) {
        const auto command = commands[index];
        if (command == kTurnRoundFirstChar && index + 1 < commands.size()
            && commands[index + 1] == kTurnRoundSecondChar) {
            cmders.push_back(TurnRoundCommand());
            ++index;
            continue;
        }

        const auto it = cmderMap.find(command);
        if (it != cmderMap.end()) {
            cmders.push_back(it->second);
        }
    }
    return cmders;
}
} // namespace adas
