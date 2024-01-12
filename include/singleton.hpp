#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <type_traits>

template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance() noexcept(std::is_nothrow_default_constructible<Derived>::value)
    {
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
        static Derived instance;
#else
        struct Dummy : public Derived {
            void ProhibitConstructFromDerived() const noexcept override { }
        };
        static Dummy instance;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
    ~Singleton() = default;
#else
    virtual ~Singleton() = default;

private:
    virtual void ProhibitConstructFromDerived() const noexcept = 0;
#endif // SINGLETON_INJECT_ABSTRACT_CLASS
};

#endif // SINGLETON_HPP_
