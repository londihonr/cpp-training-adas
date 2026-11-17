#pragma once
#include <ostream>

#include "Executor.hpp"

namespace adas {
// 测试辅助：Pose 的相等比较声明。
bool operator==(const Pose& lhs, const Pose& rhs) noexcept;

bool operator!=(const Pose& lhs, const Pose& rhs) noexcept;

void PrintTo(const Pose& pose, std::ostream* os);
} // namespace adas
