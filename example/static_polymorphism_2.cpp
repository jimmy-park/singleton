#include <iostream>

template <typename T>
class Singleton {
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

template <typename T>
class Base : public Singleton<T> {
public:
    void Interface()
    {
        static_cast<T*>(this)->Implementation();
    }

    void Implementation()
    {
        std::cout << "Implementation Base" << std::endl;
    }
};

class Derived1 : public Base<Derived1> {
public:
    void Implementation()
    {
        std::cout << "Implementation Derived" << std::endl;
    }
};

class Derived2 : public Base<Derived2> {
};

int main()
{
    Derived1::GetInstance().Interface(); // Implementation Derived
    Derived2::GetInstance().Interface(); // Implementation Base
}
