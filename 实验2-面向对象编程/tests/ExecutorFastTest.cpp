#include <gtest/gtest.h>

#include <memory>

#include "Executor.hpp"
#include "PoseEq.hpp"

// 需求2（加速指令 F）的正交分解测试：
// 状态 {F, FF} × 指令 {M, L, R}，加“状态跨多次 Execute 保持”的独立性检查。
namespace adas {

TEST(ExecutorFastTest, should_return_x_plus_2_given_status_is_fast_command_is_M_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FM");
    EXPECT_EQ((Pose{2, 0, 'E'}), executor->Query());
}

TEST(ExecutorFastTest, should_return_x_minus_2_given_status_is_fast_command_is_M_and_facing_is_W)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'W'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FM");
    EXPECT_EQ((Pose{-2, 0, 'W'}), executor->Query());
}

TEST(ExecutorFastTest, should_return_N_and_x_plus_1_given_status_is_fast_command_is_L_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FL");
    EXPECT_EQ((Pose{1, 0, 'N'}), executor->Query());
}

TEST(ExecutorFastTest, should_return_W_and_y_plus_1_given_status_is_fast_command_is_L_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FL");
    EXPECT_EQ((Pose{0, 1, 'W'}), executor->Query());
}

TEST(ExecutorFastTest, should_return_S_and_x_plus_1_given_status_is_fast_command_is_R_and_facing_is_E)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FR");
    EXPECT_EQ((Pose{1, 0, 'S'}), executor->Query());
}

TEST(ExecutorFastTest, should_return_y_plus_1_given_command_is_FFM_and_facing_is_N)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor());
    ASSERT_NE(executor, nullptr);
    executor->Execute("FFM");
    EXPECT_EQ((Pose{0, 1, 'N'}), executor->Query());
}

TEST(ExecutorFastTest, should_cancel_fast_state_given_command_is_FF_and_command_is_L)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("FFL");
    EXPECT_EQ((Pose{0, 0, 'N'}), executor->Query());
}

TEST(ExecutorFastTest, should_keep_fast_state_across_execute_calls)
{
    std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
    ASSERT_NE(executor, nullptr);
    executor->Execute("F");
    executor->Execute("M");
    EXPECT_EQ((Pose{2, 0, 'E'}), executor->Query());
}

} // namespace adas
