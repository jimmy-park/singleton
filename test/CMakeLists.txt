find_package(Threads REQUIRED)

add_executable(main test_main.cpp)
target_link_libraries(main PRIVATE
    singleton::singleton
    singleton::singleton-dclp
    singleton::singleton-atomic
    Threads::Threads
)
add_test(test_main main)
