cmake_minimum_required(VERSION 3.23)

project(
    singleton
    VERSION 1.0.0
    DESCRIPTION "C++11 template singleton pattern"
    HOMEPAGE_URL "https://github.com/jimmy-park/singleton"
    LANGUAGES CXX
)

# Custom options
option(SINGLETON_INJECT_ABSTRACT_CLASS "Prevent construction of derived class itself" OFF)

set(SINGLETON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SINGLETON_DEFINITIONS "$<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>")

# Create a singleton target
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_compile_definitions(${PROJECT_NAME} INTERFACE ${SINGLETON_DEFINITIONS})
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton.hpp
)

# Create a dclp target
add_library(dclp INTERFACE)
add_library(${PROJECT_NAME}::dclp ALIAS dclp)
target_compile_features(dclp INTERFACE cxx_std_17)
target_compile_definitions(dclp INTERFACE ${SINGLETON_DEFINITIONS})
target_sources(dclp INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton_dclp.hpp
)

# Install/Export targets
include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME} dclp
    FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
export(
    TARGETS ${PROJECT_NAME} dclp
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
)

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