#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance()
    {
#ifdef SINGLETON_INJECT_ABSTRACT_CLASS
        struct Dummy : public Derived {
            void ProhibitConstructFromDerived() const override { }
        };

        static Dummy instance;
#else
        static Derived instance;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;
    ~Singleton() = default;

#ifdef SINGLETON_INJECT_ABSTRACT_CLASS
private:
    virtual void ProhibitConstructFromDerived() const = 0;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS
};

#endif // SINGLETON_H_
