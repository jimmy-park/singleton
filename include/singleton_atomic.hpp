#ifndef SINGLETON_ATOMIC_HPP_
#define SINGLETON_ATOMIC_HPP_

#include <atomic>
#include <mutex>
#include <utility>

template <typename Derived>
class SingletonAtomic {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
        using Instance = Derived;
#else
        struct Dummy : public Derived {
            using Derived::Derived;
            void ProhibitConstructFromDerived() const override { }
        };
        using Instance = Dummy;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

        if (!instance_.load(std::memory_order_acquire)) {
            std::unique_lock lock { mutex_ };

            if (!instance_.load(std::memory_order_relaxed)) {
                instance_.store(new Instance { std::forward<Args>(args)... }, std::memory_order_release);
                lock.unlock();
                instance_.notify_all();
            }
        }
    }

    static Derived* GetInstance()
    {
        instance_.wait(nullptr, std::memory_order_acquire);
        return instance_.load(std::memory_order_relaxed);
    }

protected:
    SingletonAtomic() = default;
    SingletonAtomic(const SingletonAtomic&) = delete;
    SingletonAtomic(SingletonAtomic&&) noexcept = delete;
    SingletonAtomic& operator=(const SingletonAtomic&) = delete;
    SingletonAtomic& operator=(SingletonAtomic&&) noexcept = delete;
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
    ~SingletonAtomic() = default;
#else
    virtual ~SingletonAtomic() = default;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

private:
    struct Deleter {
        ~Deleter()
        {
            if (auto* ptr = instance.load(std::memory_order_acquire); ptr)
                delete ptr;
        }

        std::atomic<Derived*> instance { nullptr };
    };

#ifdef SINGLETON_INJECT_ABSTRACT_CLASS
    virtual void ProhibitConstructFromDerived() const = 0;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

    inline static Deleter deleter_;
    inline static auto& instance_ { deleter_.instance };
    inline static std::mutex mutex_;
};

#endif // SINGLETON_ATOMIC_HPP_
