#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <singleton.hpp>

#include "test_macro.h"

static std::atomic_uint32_t init { 0 };

class Counter : public Singleton<Counter> {
public:
    Counter() { ++init; }

    void Add() { ++count_; }
    std::uint32_t GetCount() const { return count_; }

private:
    std::atomic_uint32_t count_ { 0 };
};

class Latch {
public:
    Latch(unsigned int count)
        : count_ { count }
    {
    }

    void ArriveAndWait()
    {
        const auto current = --count_;

        if (current == 0) {
            cv_.notify_all();
        } else {
            std::unique_lock<std::mutex> lock { mutex_ };
            cv_.wait(lock, [this] { return count_ == 0; });
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic_uint count_;
};

int main()
{
    const auto count = std::thread::hardware_concurrency();
    Latch block { count };
    std::vector<std::thread> threads;

    for (auto i = 0u; i < count; ++i) {
        threads.emplace_back([&block] {
            block.ArriveAndWait();
            Counter::GetInstance().Add();
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHECK_EQ(init, 1);
    CHECK_EQ(Counter::GetInstance().GetCount(), count);
}
