#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 需求3（倒车指令 B）的正交分解测试：
// 状态 {B, F, BF, BB, FF} × 指令 {M, L, R}，见课件“B&F 指令正交分解的测试用例”表。
// 同时保留 M/L/R/F 的回归用例，确保函数式改造没有破坏既有行为。
namespace adas {

// ---------------- B 状态 ----------------

TEST(ExecutorReverseTest, should_return_x_minus_1_given_status_is_reverse_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BM");
    EXPECT_EQ((Pose{-1, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_y_minus_1_given_status_is_reverse_command_is_M_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BM");
    EXPECT_EQ((Pose{0, -1, 'N'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_y_plus_1_given_status_is_reverse_command_is_M_and_facing_is_S)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'S'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BM");
    EXPECT_EQ((Pose{0, 1, 'S'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_x_plus_1_given_status_is_reverse_command_is_M_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BM");
    EXPECT_EQ((Pose{1, 0, 'W'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_facing_S_given_status_is_reverse_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BL");
    EXPECT_EQ((Pose{0, 0, 'S'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_facing_N_given_status_is_reverse_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BR");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

// ---------------- BF 叠加状态 ----------------

TEST(ExecutorReverseTest, should_return_x_minus_2_given_status_is_reverse_and_fast_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BFM");
    EXPECT_EQ((Pose{-2, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_S_and_x_minus_1_given_status_is_reverse_and_fast_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BFL");
    EXPECT_EQ((Pose{-1, 0, 'S'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_N_and_x_minus_1_given_status_is_reverse_and_fast_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BFR");
    EXPECT_EQ((Pose{-1, 0, 'N'}), executor->Query());
}

// ---------------- 状态取消（BB / FF） ----------------

TEST(ExecutorReverseTest, should_cancel_reverse_state_given_command_is_BBM)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BBM");
    EXPECT_EQ((Pose{1, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_cancel_reverse_state_given_command_is_BBM_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("BBM");
    EXPECT_EQ((Pose{0, 1, 'N'}), executor->Query());
}

TEST(ExecutorReverseTest, should_cancel_fast_state_given_command_is_FFM)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FFM");
    EXPECT_EQ((Pose{0, 1, 'N'}), executor->Query());
}

// ---------------- 状态跨多次 Execute 保持 ----------------

TEST(ExecutorReverseTest, should_keep_reverse_state_across_execute_calls)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("B");
    executor->Execute("M");
    EXPECT_EQ((Pose{-1, 0, 'E'}), executor->Query());
}

// ---------------- M/L/R/F 回归 ----------------

TEST(ExecutorReverseTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("M");
    EXPECT_EQ((Pose{1, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_facing_W_given_command_is_L_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("L");
    EXPECT_EQ((Pose{0, 0, 'W'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_facing_E_given_command_is_R_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("R");
    EXPECT_EQ((Pose{0, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_x_plus_2_given_status_is_fast_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FM");
    EXPECT_EQ((Pose{2, 0, 'E'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_N_and_x_plus_1_given_status_is_fast_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FL");
    EXPECT_EQ((Pose{1, 0, 'N'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_S_and_x_plus_1_given_status_is_fast_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FR");
    EXPECT_EQ((Pose{1, 0, 'S'}), executor->Query());
}

TEST(ExecutorReverseTest, should_return_expected_pose_given_batch_commands)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("MLMMRM");
    EXPECT_EQ((Pose{-2, 2, 'N'}), executor->Query());
}

} // namespace adas
