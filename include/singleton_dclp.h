#ifndef SINGLETON_DCLP_H_
#define SINGLETON_DCLP_H_

#include <atomic>
#include <mutex>

template <typename Derived>
class Singleton {
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
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;
    virtual ~Singleton() = default;

private:
    virtual void ProhibitConstructFromDerived() const = 0;

    inline static std::atomic<Derived*> instance_ { nullptr };
    inline static std::mutex mutex_;
};

#endif // SINGLETON_DCLP_H_
