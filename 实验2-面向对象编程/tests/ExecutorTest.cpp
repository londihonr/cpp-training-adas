#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 需求1（基本控制指令）的回归测试：面向对象重构后，原有行为必须保持不变。
// 用例按正交分解法覆盖：初始化/默认值、M/L/R 各自的 4 个朝向、批量指令。
namespace adas {

// ---------------- 初始化与查询 ----------------

TEST(ExecutorTest, should_return_init_pose_when_without_command)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor());
    ASSERT_NE(executor, nullptr);
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

// ---------------- 移动指令 M ----------------

TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{1, 0, 'E'}), executor->Query());
}

TEST(ExecutorTest, should_return_x_minus_1_given_command_is_M_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{-1, 0, 'W'}), executor->Query());
}

TEST(ExecutorTest, should_return_y_plus_1_given_command_is_M_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{0, 1, 'N'}), executor->Query());
}

TEST(ExecutorTest, should_return_y_minus_1_given_command_is_M_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{0, -1, 'S'}), executor->Query());
}

// ---------------- 左转指令 L ----------------

TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_L_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'W'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_S_given_command_is_L_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'S'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_L_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

// ---------------- 右转指令 R ----------------

TEST(ExecutorTest, should_return_facing_S_given_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'S'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_W_given_command_is_R_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'W'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_N_given_command_is_R_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

TEST(ExecutorTest, should_return_facing_E_given_command_is_R_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

// ---------------- 批量指令与用例独立性 ----------------

TEST(ExecutorTest, should_return_expected_pose_given_batch_commands)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("MLMMRM");
    EXPECT_EQ((Pose{-2, 2, 'N'}), executor->Query());
}

TEST(ExecutorTest, should_accumulate_pose_across_multiple_execute_calls)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("MM");
    executor->Execute("L");
    executor->Execute("M");
    EXPECT_EQ((Pose{2, 1, 'N'}), executor->Query());
}

TEST(ExecutorTest, should_not_share_state_between_executors)
{
    std::unique_ptr<Executor> executorA(Executor::NewExecutor({0, 0, 'N'}));
    std::unique_ptr<Executor> executorB(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executorA, nullptr);
    ASSERT_NE(executorB, nullptr);
    executorA->Execute("MM");
    EXPECT_EQ((Pose{0, 2, 'N'}), executorA->Query());
    EXPECT_EQ((Pose{0, 0, 'N'}), executorB->Query());
}

} // namespace adas
