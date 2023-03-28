add_executable(main test_main.cpp)
target_link_libraries(main PRIVATE
    singleton::singleton
    singleton::singleton-dclp
)
add_test(test_main main)
