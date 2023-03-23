#include <iostream>

#include <singleton.hpp>
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

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is ON
    // Foo foo; Bar bar;

    // Simple, C++11, but require default constructor only
    Foo::GetInstance().Hello();

    // Support constructor (C++17)
    Bar::Construct(42);
    Bar::GetInstance()->Get();
    Bar::DestroyInstance();

    return 0;
}