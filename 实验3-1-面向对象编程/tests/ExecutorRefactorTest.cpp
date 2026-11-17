#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 实验3-1 的行为回归防护网：分层重构（PoseHandler + Command + 表驱动）之后，
// 需求1（M/L/R）与需求2（F）的行为必须与实验1、实验2 完全一致。
namespace adas {

// ---------------- 初始化 ----------------

TEST(ExecutorRefactorTest, should_return_init_pose_when_without_command)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({1, -2, 'E'}));
    ASSERT_NE(executor, nullptr);
    EXPECT_EQ((Pose{1, -2, 'E'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_default_pose_when_without_init_and_command)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor());
    ASSERT_NE(executor, nullptr);
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

// ---------------- 移动 M ----------------

TEST(ExecutorRefactorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{1, 0, 'E'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_x_minus_1_given_command_is_M_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{-1, 0, 'W'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_y_plus_1_given_command_is_M_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{0, 1, 'N'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_y_minus_1_given_command_is_M_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{0, -1, 'S'}), executor->Query());
}

// ---------------- 左转 L ----------------

TEST(ExecutorRefactorTest, should_return_facing_N_given_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_W_given_command_is_L_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'W'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_S_given_command_is_L_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'S'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_E_given_command_is_L_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

// ---------------- 右转 R ----------------

TEST(ExecutorRefactorTest, should_return_facing_S_given_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'S'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_W_given_command_is_R_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'W'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_N_given_command_is_R_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_facing_E_given_command_is_R_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

// ---------------- 加速 F ----------------

TEST(ExecutorRefactorTest, should_return_x_plus_2_given_status_is_fast_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FM");
    EXPECT_EQ((Pose{2, 0, 'E'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_N_and_x_plus_1_given_status_is_fast_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FL");
    EXPECT_EQ((Pose{1, 0, 'N'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_S_and_x_plus_1_given_status_is_fast_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FR");
    EXPECT_EQ((Pose{1, 0, 'S'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_cancel_fast_state_given_command_is_FFM)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FFM");
    EXPECT_EQ((Pose{1, 0, 'E'}), executor->Query());
}

// ---------------- 批量与独立性 ----------------

TEST(ExecutorRefactorTest, should_return_expected_pose_given_batch_commands)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("MLMMRM");
    EXPECT_EQ((Pose{-2, 2, 'N'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_return_expected_pose_given_mixed_fast_batch_commands)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    // F 加速后：M 走 2 格、L/R 先走 1 格再转向
    executor->Execute("FMLMR");
    EXPECT_EQ((Pose{3, 3, 'E'}), executor->Query());
}

TEST(ExecutorRefactorTest, should_not_share_state_between_executors)
{
    std::unique_ptr<Executor> executorA(Executor::NewExecutor({0, 0, 'E'}));
    std::unique_ptr<Executor> executorB(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executorA, nullptr);
    ASSERT_NE(executorB, nullptr);
    executorA->Execute("FMM");
    EXPECT_EQ((Pose{4, 0, 'E'}), executorA->Query());
    EXPECT_EQ((Pose{0, 0, 'E'}), executorB->Query());
}

} // namespace adas
