#include <iostream>

template <typename T>
class Singleton {
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    void Interface()
    {
        static_cast<T*>(this)->Implementation();
    }

    void Implementation()
    {
        std::cout << "Implementation Base" << std::endl;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

class A : public Singleton<A> {
public:
    void Implementation()
    {
        std::cout << "Implementation A" << std::endl;
    }
};

class B : public Singleton<B> {
public:
    void Implementation()
    {
        std::cout << "Implementation B" << std::endl;
    }
};

class C : public Singleton<C> {
};

template <typename T>
void Execute(T& base)
{
    base.Interface();
}

int main()
{
    Execute(A::GetInstance()); // Implementation A
    Execute(B::GetInstance()); // Implementation B
    Execute(C::GetInstance()); // Implementation Base
}