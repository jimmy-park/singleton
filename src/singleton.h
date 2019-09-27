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
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif // SINGLETON_H_