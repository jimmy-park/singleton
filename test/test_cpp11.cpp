#include <atomic>
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

int main()
{
    const auto count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (auto i = 0u; i < count; ++i) {
        threads.emplace_back([] { Counter::GetInstance().Add(); });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHECK(init == 1);
    CHECK(Counter::GetInstance().GetCount() == count);
}
