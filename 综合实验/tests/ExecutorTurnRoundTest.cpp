#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 需求4（掉头 TR）的测试：TR = 左转90度 -> 前进1格 -> 左转90度。
// 按需求4与课件约定：
//   - TR 不受倒车状态 B 影响；
//   - F（或 B+F）状态下走“前进1格 -> 左转 -> 前进1格 -> 左转”的加速掉头规则。
namespace adas {

TEST(ExecutorTurnRoundTest, should_turn_round_normally)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("TR");
    EXPECT_EQ((Pose{0, 1, 'W'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_turn_round_when_fast)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FTR");
    EXPECT_EQ((Pose{1, 1, 'W'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_turn_round_while_backing)
{
    // 需求4：TR 不受倒车状态影响，结果与 TR 完全一致。
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BTR");
    EXPECT_EQ((Pose{0, 1, 'W'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_turn_round_when_fast_and_backing)
{
    // B+F 叠加时按加速掉头规则执行，且倒车不改变 TR 的走向。
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BFTR");
    EXPECT_EQ((Pose{1, 1, 'W'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_return_original_pose_given_command_is_TRTR)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("TRTR");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_turn_round_in_middle_of_batch_commands)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("MTR");
    EXPECT_EQ((Pose{1, 1, 'W'}), executor->Query());
}

TEST(ExecutorTurnRoundTest, should_not_treat_turn_round_as_turn_right_given_command_is_TR)
{
    // 若把 TR 误拆成 T + R（右转），朝向会变成 N 而不是 W。
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("TR");
    EXPECT_EQ((Pose{0, 1, 'W'}), executor->Query());
}

} // namespace adas
