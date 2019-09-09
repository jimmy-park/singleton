# Singleton with CRTP (Curiously Recurring Template Pattern)

It's simple, thread-safe and naive.

## Usage

### Basic

```cpp
#include "singleton.h"

class A : public Singleton<A> {
public:
    void RunAnywhere() {}
};

int main()
{
    A::GetInstance().RunAnywhere();
}
```

### Advanced

```cpp
#include <iostream>
#include <string>

#include "singleton.h"

template <typename T>
class Animal : public Singleton<T> {
public:
    void WhoAmI(const std::string& name)
    {
        std::cout << name << ": ";
    }
};

class Dog : public Animal<Dog> {
public:
    void Speak()
    {
        WhoAmI("Puppy");
        std::cout << "Bark!" << std::endl;
    }
};

class Cat : public Animal<Cat> {
public:
    void Speak()
    {
        WhoAmI("Kitty");
        std::cout << "Meow~" << std::endl;
    }
};

int main()
{
    Dog::GetInstance().Speak(); // Puppy: Bark!
    Cat::GetInstance().Speak(); // Kitty: Meow~
}
```

### Online Example

[Static polymorphism #1](https://godbolt.org/z/xIom78)
[Static polymorphism #2](https://godbolt.org/z/sD6gmn)

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/component/content/article/42-blog/functional/273-c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
