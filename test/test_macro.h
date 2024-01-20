#ifndef SINGLETON_TEST_MACRO_
#define SINGLETON_TEST_MACRO_

#include <iostream>

#define CHECK_IMPL(file, line, expr)                         \
    do {                                                     \
        if (!(expr)) {                                       \
            std::cout << "Failed : " << #expr                \
                      << " (" << file << ':' << line << ')'; \
            return -1;                                       \
        }                                                    \
    } while (false)

#define CHECK(expr) CHECK_IMPL(__FILE__, __LINE__, expr)

#endif // SINGLETON_TEST_MACRO_
