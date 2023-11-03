#include <iostream>

#include <singleton_dclp.hpp>

class Foo : public SingletonDclp<Foo> {
public:
    Foo(int n)
        : n_ { n }
    {
        std::cout << "Foo initialized with " << n << "\n";
    }
    ~Foo() { std::cout << "~Foo\n"; }

    int Get() const { return n_; }

private:
    int n_;
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is defined
    // Foo foo;

    // C++17
    // Support user-defined constructors
    Foo::Construct(17);
    Foo::GetInstance()->Get();
    Foo::Destruct();

    // Allow re-initialization
    Foo::Construct(71);
    Foo::Destruct();

    return 0;
}
