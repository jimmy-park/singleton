find_package(Threads REQUIRED)

add_executable(test_cpp11 test_cpp11.cpp)
target_link_libraries(test_cpp11 PRIVATE singleton::singleton)
add_test(test_cpp11 test_cpp11)

add_executable(test_cpp17 test_cpp17.cpp)
target_link_libraries(test_cpp17 PRIVATE singleton::singleton-dclp)
add_test(test_cpp17 test_cpp17)

add_executable(test_cpp20 test_cpp20.cpp)
target_link_libraries(test_cpp20 PRIVATE
    singleton::singleton-atomic
    Threads::Threads
)
add_test(test_cpp20 test_cpp20)
