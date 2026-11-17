#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 课后综合作业：支持不同的车辆类型（跑车、Bus）。
// 覆盖 普通 / B / F / B+F 四种状态下的 M、L、R 行为，并与需求4的 TR 一起验证。
//
// 车型差异（来自课后综合作业）：
//   跑车：M 走 2 格；L = 先左转再前进 1 格；R = 先右转再前进 1 格
//   Bus ：M 走 1 格；L = 先前进 1 格再左转；R = 先前进 1 格再右转
//   普通：M 走 1 格；L/R 只转向
// F 状态下每个 M/L/R 至少多走一格（M 的格数翻倍），B 状态把前进/后退与左/右互换。
namespace adas {
namespace {

void ExpectPoseAfterCommands(const Pose& start, const VehicleType vehicleType, const std::string& commands,
    const Pose& expected)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor(start, vehicleType));
    ASSERT_NE(executor, nullptr);
    executor->Execute(commands);
    EXPECT_EQ(expected, executor->Query());
}

constexpr Pose kStartE{0, 0, 'E'};

} // namespace

// ---------------- 普通车 ----------------

TEST(VehicleTypeTest, normal_should_move_one_step_given_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "M", {1, 0, 'E'});
}

TEST(VehicleTypeTest, normal_should_turn_left_given_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "L", {0, 0, 'N'});
}

TEST(VehicleTypeTest, normal_should_turn_right_given_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "R", {0, 0, 'S'});
}

TEST(VehicleTypeTest, normal_should_backward_one_step_given_status_is_reverse_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BM", {-1, 0, 'E'});
}

TEST(VehicleTypeTest, normal_should_turn_right_given_status_is_reverse_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BL", {0, 0, 'S'});
}

TEST(VehicleTypeTest, normal_should_turn_left_given_status_is_reverse_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BR", {0, 0, 'N'});
}

TEST(VehicleTypeTest, normal_should_move_two_steps_given_status_is_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "FM", {2, 0, 'E'});
}

TEST(VehicleTypeTest, normal_should_move_then_turn_left_given_status_is_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "FL", {1, 0, 'N'});
}

TEST(VehicleTypeTest, normal_should_move_then_turn_right_given_status_is_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "FR", {1, 0, 'S'});
}

TEST(VehicleTypeTest, normal_should_backward_two_steps_given_status_is_reverse_and_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BFM", {-2, 0, 'E'});
}

TEST(VehicleTypeTest, normal_should_backward_then_turn_right_given_status_is_reverse_and_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BFL", {-1, 0, 'S'});
}

TEST(VehicleTypeTest, normal_should_backward_then_turn_left_given_status_is_reverse_and_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Normal, "BFR", {-1, 0, 'N'});
}

// ---------------- 跑车 ----------------

TEST(VehicleTypeTest, sport_should_move_two_steps_given_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "M", {2, 0, 'E'});
}

TEST(VehicleTypeTest, sport_should_turn_then_move_given_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "L", {0, 1, 'N'});
}

TEST(VehicleTypeTest, sport_should_turn_then_move_given_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "R", {0, -1, 'S'});
}

TEST(VehicleTypeTest, sport_should_backward_two_steps_given_status_is_reverse_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BM", {-2, 0, 'E'});
}

TEST(VehicleTypeTest, sport_should_turn_then_backward_given_status_is_reverse_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BL", {0, 1, 'S'});
}

TEST(VehicleTypeTest, sport_should_turn_then_backward_given_status_is_reverse_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BR", {0, -1, 'N'});
}

TEST(VehicleTypeTest, sport_should_move_four_steps_given_status_is_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "FM", {4, 0, 'E'});
}

TEST(VehicleTypeTest, sport_should_move_then_turn_then_move_given_status_is_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "FL", {1, 1, 'N'});
}

TEST(VehicleTypeTest, sport_should_move_then_turn_then_move_given_status_is_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "FR", {1, -1, 'S'});
}

TEST(VehicleTypeTest, sport_should_backward_four_steps_given_status_is_reverse_and_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BFM", {-4, 0, 'E'});
}

TEST(VehicleTypeTest, sport_should_backward_turn_backward_given_status_is_reverse_and_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BFL", {-1, 1, 'S'});
}

TEST(VehicleTypeTest, sport_should_backward_turn_backward_given_status_is_reverse_and_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "BFR", {-1, -1, 'N'});
}

// ---------------- Bus ----------------

TEST(VehicleTypeTest, bus_should_move_one_step_given_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "M", {1, 0, 'E'});
}

TEST(VehicleTypeTest, bus_should_move_then_turn_given_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "L", {1, 0, 'N'});
}

TEST(VehicleTypeTest, bus_should_move_then_turn_given_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "R", {1, 0, 'S'});
}

TEST(VehicleTypeTest, bus_should_backward_one_step_given_status_is_reverse_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BM", {-1, 0, 'E'});
}

TEST(VehicleTypeTest, bus_should_backward_then_turn_given_status_is_reverse_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BL", {-1, 0, 'S'});
}

TEST(VehicleTypeTest, bus_should_backward_then_turn_given_status_is_reverse_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BR", {-1, 0, 'N'});
}

TEST(VehicleTypeTest, bus_should_move_two_steps_then_turn_given_status_is_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "FL", {2, 0, 'N'});
}

TEST(VehicleTypeTest, bus_should_move_two_steps_then_turn_given_status_is_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "FR", {2, 0, 'S'});
}

TEST(VehicleTypeTest, bus_should_move_two_steps_given_status_is_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "FM", {2, 0, 'E'});
}

TEST(VehicleTypeTest, bus_should_backward_two_steps_given_status_is_reverse_and_fast_command_is_M)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BFM", {-2, 0, 'E'});
}

TEST(VehicleTypeTest, bus_should_backward_two_steps_then_turn_given_status_is_reverse_and_fast_command_is_L)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BFL", {-2, 0, 'S'});
}

TEST(VehicleTypeTest, bus_should_backward_two_steps_then_turn_given_status_is_reverse_and_fast_command_is_R)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "BFR", {-2, 0, 'N'});
}

// ---------------- TR 与车型无关 ----------------

TEST(VehicleTypeTest, sport_should_turn_round_same_as_normal)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Sport, "TR", {0, 1, 'W'});
}

TEST(VehicleTypeTest, bus_should_turn_round_same_as_normal)
{
    ExpectPoseAfterCommands(kStartE, VehicleType::Bus, "TR", {0, 1, 'W'});
}

// ---------------- 车型与状态组合的批量指令 ----------------

TEST(VehicleTypeTest, bus_should_follow_expected_pose_given_mixed_commands)
{
    // F 之后 M 走 2 格（y=2）；加速的 L 先走 2 格（y=4）再左转（N -> W）。
    ExpectPoseAfterCommands({0, 0, 'N'}, VehicleType::Bus, "FML", {0, 4, 'W'});
}

} // namespace adas
