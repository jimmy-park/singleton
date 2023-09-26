#include <chrono>
#include <iostream>
#include <thread>

#include <singleton_atomic.hpp>

class Foo : public SingletonAtomic<Foo> {
public:
    Foo(int n)
        : n_ { n }
    {
        std::cout << "Foo initialized with " << n << "\n";
    }
    ~Foo() { std::cout << "~Foo\n"; }

    void Get() const { std::cout << "Foo::Get " << n_ << "\n"; }

private:
    int n_;
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is defined
    // Foo foo; Bar bar;

    // C++20
    // Support delayed construction
    // Release automatically at program termination
    std::thread t { [] {
        std::this_thread::sleep_for(std::chrono::seconds { 1 });
        Foo::Construct(20);
    } };
    Foo::GetInstance()->Get();
    t.join();

    return 0;
}
