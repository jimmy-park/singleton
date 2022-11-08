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

# Create a singleton target
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_compile_definitions(${PROJECT_NAME} INTERFACE "$<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>")
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton.hpp
)

# Create a singleton_dclp target
add_library(${PROJECT_NAME}_dclp INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME}_dclp ALIAS ${PROJECT_NAME}_dclp)
target_compile_features(${PROJECT_NAME}_dclp INTERFACE cxx_std_17)
target_compile_definitions(${PROJECT_NAME}_dclp INTERFACE "$<$<BOOL:${SINGLETON_INJECT_ABSTRACT_CLASS}>:SINGLETON_INJECT_ABSTRACT_CLASS>")
target_sources(${PROJECT_NAME}_dclp INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${SINGLETON_INCLUDE_DIR}
    FILES ${SINGLETON_INCLUDE_DIR}/singleton_dclp.hpp
)

# Install/Export targets
include(GNUInstallDirs)
install(
    TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_dclp
    EXPORT ${PROJECT_NAME}_exports
    FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
export(
    TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_dclp
    NAMESPACE ${PROJECT_NAME}::
    APPEND FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_exports.cmake
)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(apps)
endif()