#pragma once
#include <cstdint>
#include <string>

namespace adas {
struct Pose { std::int32_t x; std::int32_t y; char heading; };
// 此枚举和工厂参数仅为文件样例，不规定学生必须采用这一接口。
enum class VehicleType { Normal, Sport, Bus };

class Executor {
public:
    static Executor* NewExecutor(const Pose& pose = {0, 0, 'N'}, VehicleType type = VehicleType::Normal) noexcept;
    virtual ~Executor() = default;
    virtual void Execute(const std::string& commands) noexcept = 0;
    virtual Pose Query() const noexcept = 0;
};
} // namespace adas
