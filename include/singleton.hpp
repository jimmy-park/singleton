#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#ifndef SINGLETON_INJECT_ABSTRACT_CLASS
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
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;
    ~Singleton() = default;
};
#else
template <typename Derived>
class Singleton {
public:
    static Derived& GetInstance()
    {
        struct Dummy : public Derived {
            void ProhibitConstructFromDerived() const override { }
        };

        static Dummy instance;
        return instance;
    }

protected:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) noexcept = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) noexcept = delete;
    virtual ~Singleton() = default;

private:
    virtual void ProhibitConstructFromDerived() const = 0;
};
#endif // SINGLETON_INJECT_ABSTRACT_CLASS

#endif // SINGLETON_HPP_