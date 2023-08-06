# singleton

[![CI](https://github.com/jimmy-park/singleton/actions/workflows/ci.yaml/badge.svg)](https://github.com/jimmy-park/singleton/actions/workflows/ci.yaml)
[![CodeQL](https://github.com/jimmy-park/singleton/actions/workflows/codeql.yaml/badge.svg)](https://github.com/jimmy-park/singleton/actions/workflows/codeql.yaml)

Implement a thread-safe singleton class using [Curiously Recurring Template Pattern (CRTP)](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

## CMake Options

| Option                            | Default | Description                                  |
| ---                               | ---     | ---                                          |
| `SINGLETON_COMPILE`               | `OFF`   | Build as a static/shared library             |
| `SINGLETON_INJECT_ABSTRACT_CLASS` | `OFF`   | Prevent construction of derived class itself |
| `SINGLETON_INSTALL`               | `OFF`   | Install headers and CMake targets            |

## Usage

### Build

```sh
# List all presets
cmake --list-presets all

# Use a configure preset
cmake --preset windows

# Use a build preset
# <configure-preset>-[clean|install]
cmake --build --preset windows

# Use a test preset
ctest --preset windows

# Use a build preset for install
# equal to `cmake --build --preset windows --target install`
cmake --build --preset windows-install
```

### Integration

Require CMake 3.23+ due to `target_sources(FILE_SET)`

```CMake
include(FetchContent)

set(SINGLETON_INJECT_ABSTRACT_CLASS ON)
set(SINGLETON_INSTALL ON)

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

## Example

### C++11

Rely on initialization of static local variable

```cpp
#include <singleton.hpp>

struct Foo : Singleton<Foo> {
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

struct Foo : public SingletonDclp<Foo> {
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

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
- [Vorbrodt's C++ Blog: Singleton Pattern](https://vorbrodt.blog/2020/07/10/singleton-pattern/)
- [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)
- [CppCon 2015: Fedor Pikus PART 1 “Live Lock-Free or Deadlock (Practical Lock-free Programming)"](https://www.youtube.com/watch?v=lVBvHbJsg5Y)
