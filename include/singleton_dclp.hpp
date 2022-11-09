#ifndef SINGLETON_DCLP_HPP_
#define SINGLETON_DCLP_HPP_

#include <atomic>
#include <mutex>

#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
template <typename Derived>
class SingletonDclp {
public:
    static Derived* GetInstance()
    {
        // Double-checked locking pattern (DCLP)
        auto* instance = instance_.load(std::memory_order_acquire);
        if (!instance) {
            std::lock_guard lock { mutex_ };

            instance = instance_.load(std::memory_order_relaxed);
            if (!instance) {
                instance = new Derived;
                instance_.store(instance, std::memory_order_release);
            }
        }

        return instance;
    }

    static void DestroyInstance()
    {
        if (auto* instance = instance_.exchange(nullptr, std::memory_order_acq_rel))
            delete instance;
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
    static Derived* GetInstance()
    {
        struct Dummy : public Derived {
            void ProhibitConstructFromDerived() const override { }
        };

        // Double-checked locking pattern (DCLP)
        auto* instance = instance_.load(std::memory_order_acquire);
        if (!instance) {
            std::lock_guard lock { mutex_ };

            instance = instance_.load(std::memory_order_relaxed);
            if (!instance) {
                instance = new Dummy;
                instance_.store(instance, std::memory_order_release);
            }
        }

        return instance;
    }

    static void DestroyInstance()
    {
        if (auto* instance = instance_.exchange(nullptr, std::memory_order_acq_rel))
            delete instance;
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