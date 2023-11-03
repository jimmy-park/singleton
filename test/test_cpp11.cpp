#include <iostream>

#include <singleton.hpp>

class Foo : public Singleton<Foo> {
public:
    Foo() { std::cout << "Foo\n"; }
    ~Foo() { std::cout << "~Foo\n"; }

    void Hello() const { std::cout << "Hello\n"; }
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is defined
    // Foo foo;

    // C++11
    // Simple, but default constructor only
    Foo::GetInstance().Hello();
    Foo::GetInstance().Hello();

    return 0;
}
