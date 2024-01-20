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

#define CHECK_EQ_IMPL(file, line, lhs, rhs)                    \
    do {                                                       \
        if (lhs != rhs) {                                      \
            std::cout << "Failed : " << #lhs << " == " << #rhs \
                      << " which is " << lhs << " == " << rhs  \
                      << " (" << file << ':' << line << ')';   \
            return -1;                                         \
        }                                                      \
    } while (false)

#define CHECK(expr) CHECK_IMPL(__FILE__, __LINE__, expr)
#define CHECK_EQ(lhs, rhs) CHECK_EQ_IMPL(__FILE__, __LINE__, lhs, rhs)

#endif // SINGLETON_TEST_MACRO_
