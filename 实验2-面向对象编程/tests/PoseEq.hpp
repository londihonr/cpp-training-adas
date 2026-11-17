#pragma once
#include <ostream>

#include "Executor.hpp"

namespace adas {
// 测试辅助：Pose 的相等比较声明。
// 定义放在 PoseEq.cpp，避免多个测试文件中重复定义（对应课件中拆出 PoseEq 的动机）。
bool operator==(const Pose& lhs, const Pose& rhs) noexcept;

bool operator!=(const Pose& lhs, const Pose& rhs) noexcept;

void PrintTo(const Pose& pose, std::ostream* os);
} // namespace adas
