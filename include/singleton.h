#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance()
    {
        static Derived instance;
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;    
    ~Singleton() = default;
};

#endif // SINGLETON_H_
