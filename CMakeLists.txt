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
    BASE_DIRS include
    FILES include/singleton.hpp
)

# Create a singleton-dclp target
add_library(singleton-dclp INTERFACE)
add_library(singleton::singleton-dclp ALIAS singleton-dclp)
target_compile_features(singleton-dclp INTERFACE cxx_std_17)
target_compile_definitions(singleton-dclp INTERFACE ${SINGLETON_DEFINITIONS})
target_sources(singleton-dclp INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES include/singleton_dclp.hpp
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

if(PROJECT_IS_TOP_LEVEL AND NOT CMAKE_CROSSCOMPILING)
    include(CTest)
    add_subdirectory(test)
endif()
