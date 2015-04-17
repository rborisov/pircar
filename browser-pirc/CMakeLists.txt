cmake_minimum_required(VERSION 2.6)

project (browser-pirc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(LibMPDClient REQUIRED)
find_package(LibSoup REQUIRED)

pkg_check_modules(PC_APPINDICATOR REQUIRED appindicator-0.1)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(WEBKIT REQUIRED webkit-1.0)

find_path(WEBKIT_INCLUDE_DIR webkit/webkit.h
    PATH_SUFFIXES "webkitgtk-1.0")
find_library(WEBKIT_LIBRARY 
    NAMES webkitgtk-1.0)

mark_as_advanced(APPINDICATOR_INCLUDE_DIR APPINDICATOR_LIBRARY)

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} 
    ${WEBKIT_INCLUDE_DIR} ${LIBSOUP_INCLUDE_DIRS} ${GTK2_INCLUDE_DIRS})

link_directories(${WEBKIT_LIBRARY} ${LIBSOUP_LIBRARIES} ${GTK2_LIBRARY_DIRS})

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall") 
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic")

get_target_property(MKDATA_EXE mkdata LOCATION)

set(SOURCES
    main.c
)

add_executable(browser-pirc ${SOURCES})
target_link_libraries(browser-pirc ${CMAKE_THREAD_LIBS_INIT} ${GTK2_LIBRARIES} 
    ${WEBKIT_LIBRARY})

install(TARGETS browser-pirc DESTINATION bin)
