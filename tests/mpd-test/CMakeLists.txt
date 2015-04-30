cmake_minimum_required(VERSION 2.6)

project (test-mpd)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

find_package(LibMPDClient REQUIRED)
find_package(Threads REQUIRED)
find_package(GLIB2 REQUIRED)

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} )

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -D__UNIX__")
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic")

set(SOURCES
    main.c
)

add_executable(test-mpd ${SOURCES})
target_link_libraries(test-mpd ${CMAKE_THREAD_LIBS_INIT} ${LIBMPDCLIENT_LIBRARY})
