#ifndef SINGLETON_DCLP_HPP_
#define SINGLETON_DCLP_HPP_

#include <cassert>

#include <atomic>
#include <mutex>
#include <utility>

#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
template <typename Derived>
class SingletonDclp {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
        if (auto* instance = instance_.load(std::memory_order_acquire); !instance) {
            std::lock_guard lock { mutex_ };

            if (instance = instance_.load(std::memory_order_relaxed); !instance) {
                instance_.store(new Derived { std::forward<Args>(args)... }, std::memory_order_release);
            }
        }
    }

    static void Destruct()
    {
        if (auto* instance = instance_.load(std::memory_order_acquire); instance) {
            std::lock_guard lock { mutex_ };

            if (instance = instance_.load(std::memory_order_relaxed); instance) {
                delete instance;
                instance_.store(nullptr, std::memory_order_release);
            }
        }
    }

    static Derived* GetInstance()
    {
        auto* instance = instance_.load(std::memory_order_acquire);
        assert(instance);
        return instance;
    }

protected:
    SingletonDclp() = default;
    SingletonDclp(const SingletonDclp&) = delete;
    SingletonDclp(SingletonDclp&&) noexcept = delete;
    SingletonDclp& operator=(const SingletonDclp&) = delete;
    SingletonDclp& operator=(SingletonDclp&&) noexcept = delete;
    ~SingletonDclp() = default;

private:
    inline static std::atomic<Derived*> instance_ { nullptr };
    inline static std::mutex mutex_;
};
#else
template <typename Derived>
class SingletonDclp {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
        struct Dummy : public Derived {
            using Derived::Derived;
            void ProhibitConstructFromDerived() const override { }
        };

        if (auto* instance = instance_.load(std::memory_order_acquire); !instance) {
            std::lock_guard lock { mutex_ };

            if (instance = instance_.load(std::memory_order_relaxed); !instance) {
                instance_.store(new Dummy { std::forward<Args>(args)... }, std::memory_order_release);
            }
        }
    }

    static void Destruct()
    {
        if (auto* instance = instance_.load(std::memory_order_acquire); instance) {
            std::lock_guard lock { mutex_ };

            if (instance = instance_.load(std::memory_order_relaxed); instance) {
                delete instance;
                instance_.store(nullptr, std::memory_order_release);
            }
        }
    }

    static Derived* GetInstance()
    {
        auto* instance = instance_.load(std::memory_order_acquire);
        assert(instance);
        return instance;
    }

protected:
    SingletonDclp() = default;
    SingletonDclp(const SingletonDclp&) = delete;
    SingletonDclp(SingletonDclp&&) noexcept = delete;
    SingletonDclp& operator=(const SingletonDclp&) = delete;
    SingletonDclp& operator=(SingletonDclp&&) noexcept = delete;
    virtual ~SingletonDclp() = default;

private:
    virtual void ProhibitConstructFromDerived() const = 0;

    inline static std::atomic<Derived*> instance_ { nullptr };
    inline static std::mutex mutex_;
};
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

#endif // SINGLETON_DCLP_HPP_