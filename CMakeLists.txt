cmake_minimum_required(VERSION 3.23)

project(
    singleton
    VERSION 1.2.0
    DESCRIPTION "C++11 template singleton pattern"
    HOMEPAGE_URL "https://github.com/jimmy-park/singleton"
    LANGUAGES CXX
)

# Custom options
option(SINGLETON_INJECT_ABSTRACT_CLASS "Prevent construction of derived class itself" OFF)
option(SINGLETON_INSTALL "Install CMake targets" OFF)

set(SINGLETON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(SINGLETON_INJECT_ABSTRACT_CLASS)
    set(SINGLETON_DEFINITIONS "SINGLETON_INJECT_ABSTRACT_CLASS")
else()
    set(SINGLETON_DEFINITIONS "$<$<CONFIG:Debug>:SINGLETON_INJECT_ABSTRACT_CLASS>")
endif()

# Create a singleton target
add_library(singleton INTERFACE)
add_library(singleton::singleton ALIAS singleton)
target_compile_features(singleton INTERFACE cxx_std_11)
target_compile_definitions(singleton INTERFACE ${SINGLETON_DEFINITIONS})
target_sources(singleton INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton.hpp
)

# Create a singleton-dclp target
add_library(singleton-dclp INTERFACE)
add_library(singleton::singleton-dclp ALIAS singleton-dclp)
target_compile_features(singleton-dclp INTERFACE cxx_std_17)
target_compile_definitions(singleton-dclp INTERFACE ${SINGLETON_DEFINITIONS})
target_sources(singleton-dclp INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton_dclp.hpp
)

# Install/Export targets
if(SINGLETON_INSTALL)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/singleton-config-version.cmake
        COMPATIBILITY SameMajorVersion
    )
    install(
        TARGETS singleton singleton-dclp
        EXPORT singletonTargets
        FILE_SET HEADERS
    )
    install(
        EXPORT singletonTargets
        NAMESPACE singleton::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/singleton
    )
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/singleton-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/singleton-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/singleton
    )
endif()

# Run a simple test
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(SINGLETON_TEST_COMPILE_OPTIONS "/W4 /WX /permissive-")
else()
    set(SINGLETON_TEST_COMPILE_OPTIONS "-Wall -Wextra -Werror -pedantic")
endif()

set(SINGLETON_TEST_DIR ${CMAKE_BINARY_DIR})
set(SINGLETON_TEST_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/apps/main.cpp)
try_run(
    SINGLETON_TEST_RUN_RESULT
    SINGLETON_TEST_COMPILE_RESULT
    ${SINGLETON_TEST_DIR}
    ${SINGLETON_TEST_SOURCE}
    CMAKE_FLAGS
    "-DINCLUDE_DIRECTORIES=${SINGLETON_INCLUDE_DIR}"
    "-DCOMPILE_DEFINITIONS=${SINGLETON_TEST_COMPILE_OPTIONS}"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(NOT SINGLETON_TEST_COMPILE_RESULT)
    message(FATAL_ERROR "Failed to compile")
elseif(SINGLETON_TEST_RUN_RESULT STREQUAL "FAILED_TO_RUN"
    AND CMAKE_HOST_SYSTEM_NAME STREQUAL CMAKE_SYSTEM_NAME)
    message(FATAL_ERROR "Failed to run")
endif()

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(apps)
endif()