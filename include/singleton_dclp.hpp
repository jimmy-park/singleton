#ifndef SINGLETON_DCLP_HPP_
#define SINGLETON_DCLP_HPP_

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <utility>

template <typename Derived>
class SingletonDclp {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
        using Instance = Derived;
#else
        struct Dummy final : Derived {
            using Derived::Derived;
            void ProhibitConstructFromDerived() const noexcept override { }
        };
        using Instance = Dummy;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

        if (!instance_.load(std::memory_order_acquire)) {
            std::lock_guard lock { mutex_ };

            if (!instance_.load(std::memory_order_relaxed))
                instance_.store(new Instance { std::forward<Args>(args)... }, std::memory_order_release);
        }
    }

    static void Destruct()
    {
        if (instance_.load(std::memory_order_acquire)) {
            std::lock_guard lock { mutex_ };

            if (auto* instance = instance_.load(std::memory_order_relaxed); instance) {
                delete instance;
                instance_.store(nullptr, std::memory_order_release);
            }
        }
    }

    static Derived* GetInstance()
    {
        auto* instance = instance_.load(std::memory_order_acquire);

        if (!instance) {
            std::shared_lock lock { mutex_ };

            instance = instance_.load(std::memory_order_relaxed);
        }

        return instance;
    }

protected:
    SingletonDclp() = default;
    SingletonDclp(const SingletonDclp&) = delete;
    SingletonDclp(SingletonDclp&&) = delete;
    SingletonDclp& operator=(const SingletonDclp&) = delete;
    SingletonDclp& operator=(SingletonDclp&&) = delete;
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
    ~SingletonDclp() = default;
#else
    virtual ~SingletonDclp() = default;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

private:
#ifdef SINGLETON_INJECT_ABSTRACT_CLASS
    virtual void ProhibitConstructFromDerived() const noexcept = 0;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

    inline static std::atomic<Derived*> instance_ { nullptr };
    inline static std::shared_mutex mutex_;
};

#endif // SINGLETON_DCLP_HPP_
