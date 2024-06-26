cmake_minimum_required(VERSION 3.23)

project(
    singleton
    VERSION 1.6.0
    DESCRIPTION "C++11/17/20 template singleton pattern"
    HOMEPAGE_URL "https://github.com/jimmy-park/singleton"
    LANGUAGES CXX
)

option(SINGLETON_COMPILE "Build as a static/shared library" OFF)
option(SINGLETON_INJECT_ABSTRACT_CLASS "Prevent construction of derived class itself" OFF)
option(SINGLETON_INSTALL "Install headers and CMake targets" OFF)

include(cmake/CompileOptions.cmake)
include(cmake/UseSanitizer.cmake)

if(SINGLETON_COMPILE)
    set(SCOPE PUBLIC)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/singleton.cpp "#include <singleton.hpp>")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/singleton_dclp.cpp "#include <singleton_dclp.hpp>")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/singleton_atomic.cpp "#include <singleton_atomic.hpp>")
    add_library(singleton ${CMAKE_CURRENT_BINARY_DIR}/singleton.cpp)
    add_library(singleton-dclp ${CMAKE_CURRENT_BINARY_DIR}/singleton_dclp.cpp)
    add_library(singleton-atomic ${CMAKE_CURRENT_BINARY_DIR}/singleton_atomic.cpp)
else()
    set(SCOPE INTERFACE)
    add_library(singleton INTERFACE)
    add_library(singleton-dclp INTERFACE)
    add_library(singleton-atomic INTERFACE)
endif()

# Modify source
if(MINGW OR CYGWIN)
    file(READ ${CMAKE_SOURCE_DIR}/include/singleton_dclp.hpp DCLP)
    string(REPLACE "#include <shared_mutex>\n" "" DCLP "${DCLP}")
    string(REPLACE "std::shared_lock" "std::lock_guard" DCLP "${DCLP}")
    string(REPLACE "std::shared_mutex" "std::mutex" DCLP "${DCLP}")
    file(WRITE ${CMAKE_SOURCE_DIR}/include/singleton_dclp.hpp "${DCLP}")
endif()

# Create a singleton target
add_library(singleton::singleton ALIAS singleton)
target_compile_features(singleton ${SCOPE} cxx_std_11)
target_compile_definitions(singleton ${SCOPE}
    $<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>
)
target_sources(singleton ${SCOPE}
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/singleton.hpp
)

# Create a singleton-dclp target
add_library(singleton::singleton-dclp ALIAS singleton-dclp)
target_compile_features(singleton-dclp ${SCOPE} cxx_std_17)
target_compile_definitions(singleton-dclp ${SCOPE}
    $<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>
)
target_sources(singleton-dclp ${SCOPE}
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/singleton_dclp.hpp
)

# Create a singleton-atomic target
add_library(singleton::singleton-atomic ALIAS singleton-atomic)
target_compile_features(singleton-atomic ${SCOPE} cxx_std_20)
target_compile_definitions(singleton-atomic ${SCOPE}
    $<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>
)
target_sources(singleton-atomic ${SCOPE}
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/singleton_atomic.hpp
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
        TARGETS singleton singleton-dclp singleton-atomic
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

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_CROSSCOMPILING)
    include(CTest)
    add_subdirectory(test)
endif()
