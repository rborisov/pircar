cmake_minimum_required(VERSION 2.8)

project (streamripper)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

#include_directories(${PROJECT_SOURCE_DIR}/src)
set(SR_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libstreamripper.dir/")
include(ExternalProject)
ExternalProject_Add(
    libstreamripper
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/src
    CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/configure --prefix=${SR_TARGET_DIR}
    PREFIX ${SR_TARGET_DIR}
    BUILD_COMMAND make
    INSTALL_DIR ${CMAKE_BINARY_DIR}
    BINARY_DIR ${CMAKE_BINARY_DIR}
    #BUILD_IN_SOURCE 1
    )
