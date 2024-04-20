#include <atomic>
#include <latch>
#include <thread>
#include <vector>

#include <singleton_dclp.hpp>

#include "test_macro.h"

static std::atomic_uint32_t init { 0 };
static std::atomic_uint32_t deinit { 0 };

class Counter : public SingletonDclp<Counter> {
public:
    Counter() { ++init; }
    ~Counter() { ++deinit; }

    void Add() { ++count_; }
    std::uint32_t GetCount() const { return count_; }

private:
    std::atomic_uint32_t count_ { 0 };
};

int main()
{
    const auto count = std::thread::hardware_concurrency();
    std::latch block { static_cast<std::ptrdiff_t>(count) };
    std::vector<std::thread> threads;

    for (auto i = 0u; i < count; ++i) {
        threads.emplace_back([&block] {
            block.arrive_and_wait();
            Counter::Construct();
            Counter::GetInstance()->Add();
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHECK_EQ(init, 1);
    CHECK_EQ(deinit, 0);
    CHECK_EQ(Counter::GetInstance()->GetCount(), count);

    Counter::Destruct();

    CHECK_EQ(init, 1);
    CHECK_EQ(deinit, 1);
    CHECK_EQ(Counter::GetInstance(), nullptr);

    Counter::Construct();

    CHECK_EQ(init, 2);
    CHECK_EQ(deinit, 1);
    CHECK_EQ(Counter::GetInstance()->GetCount(), 0);

    Counter::Destruct();

    CHECK_EQ(init, 2);
    CHECK_EQ(deinit, 2);
    CHECK_EQ(Counter::GetInstance(), nullptr);
}
