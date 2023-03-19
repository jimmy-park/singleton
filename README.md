# C++11/17 Template Singleton Pattern

Implement a thread-safe singleton class using [Curiously Recurring Template Pattern (CRTP)](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

## Usage

### C++11

Rely on initialization of static local variable

```cpp
#include <singleton.hpp>

class Foo : public Singleton<Foo> {
public:
    void Bar() {}
};

int main()
{
    // Compile error when SINGLETON_INJECT_ABSTRACT_CLASS is ON
    // Foo foo;

    Foo::GetInstance().Bar();
}
```

### C++17

Implement based on [Double-Checked Locking Pattern (DCLP)](https://en.wikipedia.org/wiki/Double-checked_locking)

Use this version when you need to control the destruction order manually or initialize with parameters

- A few C++17 features are used
  - Inline static member variable
  - If statements with initializer

```cpp
#include <singleton_dclp.hpp>

class Foo : public SingletonDclp<Foo> {
public:
    Foo(int n) : n_ { n } {}
    void Bar() {}

private:
    int n_;
};

int main()
{
    Foo::Construct(42);
    Foo::GetInstance()->Bar();
    Foo::DestroyInstance();
}
```

#### Caveats

- Do not call `GetInstance()` before `Construct()` and after `DestroyInstance()`
- Must call `DestroyInstance()` before terminating program
- `Construct()` is no-op once a singleton instance is created

### Run in online compiler

[Compiler Explorer](https://godbolt.org/z/G7j3P17zE)

### CMake Integration

Require CMake 3.23+ due to `target_sources(FILE_SET)`

```CMake
include(FetchContent)

set(SINGLETON_INJECT_ABSTRACT_CLASS ON) # default : OFF
set(SINGLETON_INSTALL ON)               # default : OFF

FetchContent_Declare(
    singleton
    URL https://github.com/jimmy-park/singleton/archive/main.tar.gz
)
FetchContent_MakeAvailable(singleton)

# If you're using CPM.cmake
# CPMAddPackage(
#     NAME singleton
#     URL https://github.com/jimmy-park/singleton/archive/main.tar.gz
#     OPTIONS
#     "SINGLETON_INJECT_ABSTRACT_CLASS ON"
#     "SINGLETON_INSTALL ON"
# )

add_executable(main main.cpp)
target_link_libraries(main PRIVATE singleton::singleton) # or singleton::singleton-dclp
```

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/component/content/article/42-blog/functional/273-c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
- [Vorbrodt's C++ Blog: Singleton Pattern](https://vorbrodt.blog/2020/07/10/singleton-pattern/)
- [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)
- [CppCon 2015: Fedor Pikus PART 1 “Live Lock-Free or Deadlock (Practical Lock-free Programming)"](https://www.youtube.com/watch?v=lVBvHbJsg5Y)
