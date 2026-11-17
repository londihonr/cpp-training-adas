#pragma once
#include <memory>

#include "Executor.hpp"

namespace adas {
// 实验2：面向对象封装 + 继承/多态。
// 指令的处理逻辑被封装为 ICommand 的派生类，ExecutorImpl 只负责分派，
// 因此新增指令时不必再修改 Move/TurnLeft/TurnRight 的实现。
class ExecutorImpl final : public Executor {
public:
    explicit ExecutorImpl(const Pose& pose) noexcept;
    ~ExecutorImpl() noexcept override = default;
    ExecutorImpl(const ExecutorImpl&) = delete;
    ExecutorImpl& operator=(const ExecutorImpl&) = delete;

public:
    void Execute(const std::string& commands) noexcept override;
    Pose Query() const noexcept override;

private:
    // 指令抽象接口：所有指令都以“操作执行器状态”的形式实现。
    class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void DoOperate(ExecutorImpl& executor) const noexcept = 0;
    };

    class MoveCommand final : public ICommand {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            if (executor.IsFast()) {
                executor.Move();
            }
            executor.Move();
        }
    };

    class TurnLeftCommand final : public ICommand {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            if (executor.IsFast()) {
                executor.Move();
            }
            executor.TurnLeft();
        }
    };

    class TurnRightCommand final : public ICommand {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            if (executor.IsFast()) {
                executor.Move();
            }
            executor.TurnRight();
        }
    };

    class FastCommand final : public ICommand {
    public:
        void DoOperate(ExecutorImpl& executor) const noexcept override
        {
            executor.Fast();
        }
    };

private:
    void Move() noexcept;
    void TurnLeft() noexcept;
    void TurnRight() noexcept;
    void Fast() noexcept;
    bool IsFast() const noexcept;

private:
    Pose pose;
    bool fast{false};
};
} // namespace adas
