#ifndef SINGLETON_TEST_MACRO_
#define SINGLETON_TEST_MACRO_

#include <iostream>

#define CHECK(expr)                            \
    do {                                       \
        if (!(expr)) {                         \
            std::cout << "Failed : " << #expr; \
            return -1;                         \
        }                                      \
    } while (false)

#endif // SINGLETON_TEST_MACRO_
