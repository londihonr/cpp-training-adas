#pragma once
#include "Executor.hpp"
#include "PoseHandler.hpp"

namespace adas {
// 实验3-2：ExecutorImpl 只保留分派与查询，命令以 std::function 形式存表。
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
    PoseHandler poseHandler;
};
} // namespace adas
