#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance()
    {
#ifdef DEBUG
        struct Dummy : public Derived {
            void ProhibitConstructFromDerived() const override { }
        };

        static Dummy instance;
#else
        static Derived instance;
#endif // DEBUG
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;
    ~Singleton() = default;

#ifdef DEBUG
private:
    virtual void ProhibitConstructFromDerived() const = 0;
#endif // DEBUG
};

#endif // SINGLETON_H_
