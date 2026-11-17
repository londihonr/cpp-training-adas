#pragma once
#include <array>
#include <cstddef>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include "ActionGroup.hpp"
#include "Command.hpp"
#include "Executor.hpp"

namespace adas {
class PoseHandler;

using Cmder = std::function<ActionGroup(PoseHandler&)>;
using CmderList = std::list<Cmder>;

inline constexpr std::size_t kVehicleTypeCount = 3;

// 单体指令工厂：一次构建好三种车型的指令表，后续只做查表与解析。
class CmderFactory final {
public:
    CmderFactory() noexcept;
    ~CmderFactory() = default;
    CmderFactory(const CmderFactory&) = delete;
    CmderFactory& operator=(const CmderFactory&) = delete;

public:
    CmderList GetCmders(const VehicleType vehicleType, const std::string& commands) const noexcept;

private:
    std::array<std::unordered_map<char, Cmder>, kVehicleTypeCount> cmderMaps;
};
} // namespace adas
