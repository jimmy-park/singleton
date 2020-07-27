#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance()
    {
        struct Dummy : Derived {
            void ProhibitConstructFromDerived() { }
        };

        static Dummy instance;
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    virtual ~Singleton() = default;

private:
    virtual void ProhibitConstructFromDerived() = 0;
};

#endif // SINGLETON_H_
