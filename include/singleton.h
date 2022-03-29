#ifndef SINGLETON_H_
#define SINGLETON_H_

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
    ~Singleton() = default;

private:
    virtual void ProhibitConstructFromDerived() const = 0;
};

#endif // SINGLETON_H_
