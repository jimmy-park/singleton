# Singleton with CRTP (Curiously Recurring Template Pattern)

It's simple, thread-safe and naive.

## Usage

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

Static polymorphism example : <https://godbolt.org/z/J-ZcA8>

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/component/content/article/42-blog/functional/273-c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
