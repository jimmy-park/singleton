#include <chrono>
#include <iostream>
#include <thread>

#include <singleton.hpp>
#include <singleton_atomic.hpp>
#include <singleton_dclp.hpp>

class Foo : public Singleton<Foo> {
public:
    Foo() { std::cout << "Foo\n"; }
    ~Foo() { std::cout << "~Foo\n"; }

    void Hello() const { std::cout << "Hello\n"; }
};

class Bar : public SingletonDclp<Bar> {
public:
    Bar(int n)
        : n_ { n }
    {
        std::cout << "Bar initialized with " << n << "\n";
    }
    ~Bar() { std::cout << "~Bar\n"; }

    int Get() const { return n_; }

private:
    int n_;
};

class Baz : public SingletonAtomic<Baz> {
public:
    Baz(int n)
        : n_ { n }
    {
        std::cout << "Baz initialized with " << n << "\n";
    }
    ~Baz() { std::cout << "~Baz\n"; }

    void Get() const { std::cout << "Baz::Get " << n_ << "\n"; }

private:
    int n_;
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is defined
    // Foo foo; Bar bar; Baz baz;

    // C++11
    // Simple, but default constructor only
    Foo::GetInstance().Hello();

    // C++17
    // Support user-defined constructors
    Bar::Construct(17);
    Bar::GetInstance()->Get();
    Bar::Destruct();

    // C++20
    // Support delayed construction
    // Release automatically at program termination
    std::thread t { [] {
        std::this_thread::sleep_for(std::chrono::seconds { 1 });
        Baz::Construct(20);
    } };
    Baz::GetInstance()->Get();
    t.join();

    return 0;
}