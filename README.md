# C++11 Template Singleton Pattern

Implement a thread-safe singleton class using CRTP (Curiously Recurring Template Pattern)

## Usage

### Basic

Rely on initialization of static local variable

```cpp
#include <singleton.hpp>

class Foo : public Singleton<Foo> {
public:
    void RunAnywhere() {}
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is ON
    // Foo foo;

    Foo::GetInstance().RunAnywhere();
}
```

[Compiler Explorer](https://godbolt.org/z/bn9GKGbxf)

### When you need to control the destruction order manually

Implement based on double-checked locking pattern (DCLP)

Require C++17 due to inline static variable

```cpp
#include <singleton_dclp.hpp>

class Foo : public SingletonDclp<Foo> {
public:
    void RunAnywhere() {}
};

int main()
{
    Foo::GetInstance()->RunAnywhere();
    Foo::DestroyInstance();
}
```

### CMake integration

```CMake
# Require CMake 3.23+
include(FetchContent)

set(SINGLETON_INJECT_ABSTRACT_CLASS ON) # default : OFF

FetchContent_Declare(
    singleton
    URL https://github.com/jimmy-park/singleton/archive/1.0.0.zip
)
FetchContent_MakeAvailable(singleton)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE singleton::singleton) # or singleton::dclp
```

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/component/content/article/42-blog/functional/273-c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
- [Vorbrodt's C++ Blog: Singleton Pattern](https://vorbrodt.blog/2020/07/10/singleton-pattern/)
- [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)
- [CppCon 2015: Fedor Pikus PART 1 “Live Lock-Free or Deadlock (Practical Lock-free Programming)"](https://www.youtube.com/watch?v=lVBvHbJsg5Y)
