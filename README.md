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
class Stage : public Singleton<T> {
public:
    void Mic(const std::string& talk)
    {
        std::cout << talk << std::endl;
    }

    void Speaker()
    {
        static_cast<T*>(this)->Speak();
    }
    
    void Speak()
    {
        Mic("White noise");
    }
};

class Dog : public Stage<Dog> {
public:
    void Speak()
    {
        Mic("Puppy: Bark!");
    }
};

class Cat : public Stage<Cat> {
public:
    void Speak()
    {
        Mic("Kitty: Meow~");
    }
};

int main()
{
    Dog::GetInstance().Speaker(); // Puppy: Bark!
    Cat::GetInstance().Speaker(); // Kitty: Meow~
}
```

### Online Example

[Static polymorphism #1](https://godbolt.org/z/xIom78)

[Static polymorphism #2](https://godbolt.org/z/sD6gmn)

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/component/content/article/42-blog/functional/273-c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
