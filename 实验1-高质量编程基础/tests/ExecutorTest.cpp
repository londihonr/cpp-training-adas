#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 需求1（基本控制指令）的开发者测试。
// 用例按正交分解法覆盖：初始化/默认值、M 的 4 个朝向、L 的 4 个朝向、
// R 的 4 个朝向、批量指令，以及用例之间的独立性（FIRST 原则）。
namespace adas {

// ---------------- 初始化与查询 ----------------

TEST(ExecutorTest, should_return_init_pose_when_without_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // when
    // then
    const Pose target{0, 0, 'E'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor());
    ASSERT_NE(executor, nullptr);
    // when
    // then
    const Pose target{0, 0, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_init_pose_when_command_is_empty)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({3, -4, 'S'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("");
    // then
    const Pose target{3, -4, 'S'};
    EXPECT_EQ(target, executor->Query());
}

// ---------------- 移动指令 M ----------------

TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("M");
    // then
    const Pose target{1, 0, 'E'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_x_minus_1_given_command_is_M_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("M");
    // then
    const Pose target{-1, 0, 'W'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_y_plus_1_given_command_is_M_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("M");
    // then
    const Pose target{0, 1, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_y_minus_1_given_command_is_M_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("M");
    // then
    const Pose target{0, -1, 'S'};
    EXPECT_EQ(target, executor->Query());
}

// ---------------- 左转指令 L ----------------

TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("L");
    // then
    const Pose target{0, 0, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_L_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("L");
    // then
    const Pose target{0, 0, 'W'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_S_given_command_is_L_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("L");
    // then
    const Pose target{0, 0, 'S'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_L_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("L");
    // then
    const Pose target{0, 0, 'E'};
    EXPECT_EQ(target, executor->Query());
}

// ---------------- 右转指令 R ----------------

TEST(ExecutorTest, should_return_facing_S_given_command_is_R_and_facing_is_E)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("R");
    // then
    const Pose target{0, 0, 'S'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_R_and_facing_is_S)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("R");
    // then
    const Pose target{0, 0, 'W'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_N_given_command_is_R_and_facing_is_W)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("R");
    // then
    const Pose target{0, 0, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_R_and_facing_is_N)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("R");
    // then
    const Pose target{0, 0, 'E'};
    EXPECT_EQ(target, executor->Query());
}

// ---------------- 批量指令与用例独立性 ----------------

TEST(ExecutorTest, should_return_expected_pose_given_batch_commands)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("MLMMRM");
    // then
    const Pose target{-2, 2, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_keep_position_given_command_only_contains_turns)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({1, 2, 'N'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("LLRR");
    // then
    const Pose target{1, 2, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_accumulate_pose_across_multiple_execute_calls)
{
    // given
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // when
    executor->Execute("MM");
    executor->Execute("L");
    executor->Execute("M");
    // then
    const Pose target{2, 1, 'N'};
    EXPECT_EQ(target, executor->Query());
}

TEST(ExecutorTest, should_not_share_state_between_executors)
{
    // given
    std::unique_ptr<Executor> executorA(Executor::NewExecutor({0, 0, 'N'}));
    std::unique_ptr<Executor> executorB(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executorA, nullptr);
    ASSERT_NE(executorB, nullptr);
    // when
    executorA->Execute("MM");
    // then
    const Pose targetA{0, 2, 'N'};
    const Pose targetB{0, 0, 'N'};
    EXPECT_EQ(targetA, executorA->Query());
    EXPECT_EQ(targetB, executorB->Query());
}

} // namespace adas
