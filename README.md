# singleton

[![CI](https://github.com/jimmy-park/singleton/actions/workflows/ci.yaml/badge.svg)](https://github.com/jimmy-park/singleton/actions/workflows/ci.yaml)
[![CodeQL](https://github.com/jimmy-park/singleton/actions/workflows/codeql.yaml/badge.svg)](https://github.com/jimmy-park/singleton/actions/workflows/codeql.yaml)

Implement thread-safe singleton classes using [Curiously Recurring Template Pattern (CRTP)](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

## CMake Options

| Option                            | Default | Description                                           |
| --------------------------------- | ------- | ----------------------------------------------------- |
| `SINGLETON_INJECT_ABSTRACT_CLASS` | `OFF`   | Prevent construction of derived class at compile time |
| `SINGLETON_INSTALL`               | `OFF`   | Install headers and CMake targets                     |

## Usage

### Build

```sh
cmake --list-presets all                    # List all CMake presets
cmake --preset windows                      # Configure
cmake --build --preset windows              # Build
ctest --preset windows                      # Test
cmake --build --preset windows -t install   # Install
```

### Integration

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
target_link_libraries(main PRIVATE 
    singleton::singleton        # C++11
    singleton::singleton-dclp   # C++17
    singleton::singleton-atomic # C++20
)
```

## Example

### C++11

Rely on initialization of static local variable

```cpp
#include <singleton.hpp>

struct Foo : public Singleton<Foo> {
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

Use this version when you need to initialize with parameters or control the destruction order manually

- C++17 features
  - Inline static member variable
  - If statements with initializer
  - `std::shared_mutex`

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
    Foo::Construct(17);
    Foo::GetInstance()->Bar();
    Foo::Destruct();
}
```

#### Caveats

- `GetInstance()` must be called between `Construct()` and `Destruct()`
- Don't forget to call `Destruct()` before terminating program

### C++20

Use `std::atomic::wait()` to block `GetInstance()` during construction

```cpp
#include <singleton_atomic.hpp>

class Foo : public SingletonAtomic<Foo> {
public:
    Foo(int n) {}
    void Bar() {}
};

int main()
{
    std::jthread t { [] {
        std::this_thread::sleep_for(std::chrono::seconds { 1 });
        Foo::Construct(20);
    } };                        // Construct in another thread
    Foo::GetInstance()->Bar();  // Block until construction is finished
}
```

## Reference

- [What is the curiously recurring template pattern (CRTP)?](https://stackoverflow.com/questions/4173254/what-is-the-curiously-recurring-template-pattern-crtp/4173298#4173298)
- [C++ is Lazy: CRTP](https://www.modernescpp.com/index.php/c-is-still-lazy)
- [Thread-Safe Initialization of a Singleton](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
- [Vorbrodt's C++ Blog: Singleton Pattern](https://vorbrodt.blog/2020/07/10/singleton-pattern/)
- [Double-Checked Locking is Fixed In C++11](https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/)
- [CppCon 2015: Fedor Pikus PART 1 “Live Lock-Free or Deadlock (Practical Lock-free Programming)"](https://www.youtube.com/watch?v=lVBvHbJsg5Y)
