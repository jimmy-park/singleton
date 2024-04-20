find_package(Threads REQUIRED)

set(SINGLETON_TESTS
    test_cpp11
    test_cpp17
    test_cpp20
)

if(MSVC)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH VS_PATH)
endif()

foreach(test_case IN LISTS SINGLETON_TESTS)
    add_executable(${test_case} ${test_case}.cpp test_macro.h)
    target_compile_features(${test_case} PRIVATE cxx_std_20)
    target_link_libraries(${test_case} PRIVATE Threads::Threads)
    add_test(${test_case} ${test_case})

    if(MSVC)
        set_tests_properties(${test_case}
            PROPERTIES
            ENVIRONMENT "PATH=${VS_PATH}\;%PATH%;ASAN_SYMBOLIZER_PATH=${VS_PATH}"
        )
    endif()
endforeach()

target_link_libraries(test_cpp11 PRIVATE singleton::singleton)
target_link_libraries(test_cpp17 PRIVATE singleton::singleton-dclp)
target_link_libraries(test_cpp20 PRIVATE singleton::singleton-atomic)
