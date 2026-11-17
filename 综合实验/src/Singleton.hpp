#pragma once

namespace adas {
// 泛型单体：C++11 起静态局部变量的初始化是线程安全的，且天然懒加载。
// 通过模板复用，避免为每个需要单体的类重复实现。
template <typename T>
class Singleton final {
public:
    static T& Instance() noexcept
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) noexcept = delete;
    Singleton& operator=(const Singleton&) noexcept = delete;

private:
    Singleton() noexcept = default;
    ~Singleton() noexcept = default;
};
} // namespace adas
