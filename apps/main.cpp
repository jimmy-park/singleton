#include <iostream>

#include <singleton.hpp>
#include <singleton_dclp.hpp>

class Foo : public Singleton<Foo> {
public:
    Foo() { std::cout << "Foo\n"; }
    ~Foo() { std::cout << "~Foo\n"; }

    void Hello() { std::cout << "Hello\n"; }
};

class Bar : public SingletonDclp<Bar> {
public:
    Bar() { std::cout << "Bar\n"; }
    ~Bar() { std::cout << "~Bar\n"; }

    void Hello() { std::cout << "Hello\n"; }
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is ON
    // Foo foo; Bar bar;

    Foo::GetInstance().Hello();
    Bar::GetInstance()->Hello();
    Bar::DestroyInstance();

    return 0;
}