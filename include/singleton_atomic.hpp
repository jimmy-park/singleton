#ifndef SINGLETON_ATOMIC_HPP_
#define SINGLETON_ATOMIC_HPP_

#include <atomic>
#include <mutex>
#include <utility>

#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
template <typename Derived>
class SingletonAtomic {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
        if (!instance_.load(std::memory_order_acquire)) {
            std::unique_lock lock { mutex_ };

            if (!instance_.load(std::memory_order_relaxed)) {
                instance_.store(new Derived { std::forward<Args>(args)... }, std::memory_order_release);
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
    ~SingletonAtomic() = default;

private:
    struct Deleter {
        ~Deleter()
        {
            if (auto* ptr = instance.load(std::memory_order_acquire); ptr) {
                delete ptr;
            }
        }

        std::atomic<Derived*> instance { nullptr };
    };

    inline static Deleter deleter_;
    inline static auto& instance_ { deleter_.instance };
    inline static std::mutex mutex_;
};
#else
template <typename Derived>
class SingletonAtomic {
public:
    template <typename... Args>
    static void Construct(Args&&... args)
    {
        struct Dummy : public Derived {
            using Derived::Derived;
            void ProhibitConstructFromDerived() const override { }
        };

        if (!instance_.load(std::memory_order_acquire)) {
            std::unique_lock lock { mutex_ };

            if (!instance_.load(std::memory_order_relaxed)) {
                instance_.store(new Dummy { std::forward<Args>(args)... }, std::memory_order_release);
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
    virtual ~SingletonAtomic() = default;

private:
    struct Deleter {
        ~Deleter()
        {
            if (auto* ptr = instance.load(std::memory_order_acquire); ptr) {
                delete ptr;
            }
        }

        std::atomic<Derived*> instance { nullptr };
    };

    virtual void ProhibitConstructFromDerived() const = 0;

    inline static Deleter deleter_;
    inline static auto& instance_ { deleter_.instance };
    inline static std::mutex mutex_;
};
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

#endif // SINGLETON_ATOMIC_HPP_
