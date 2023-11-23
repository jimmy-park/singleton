#ifndef SINGLETON_TEST_MACRO_
#define SINGLETON_TEST_MACRO_

#define CHECK(expr)    \
    do {               \
        if (!(expr))   \
            return -1; \
    } while (false)

#endif // SINGLETON_TEST_MACRO_
