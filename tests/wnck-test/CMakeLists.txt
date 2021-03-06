cmake_minimum_required(VERSION 2.6)

project (wnck-test)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

find_package(Threads REQUIRED)
find_package(PkgConfig)

pkg_check_modules(PC_APPINDICATOR REQUIRED appindicator-0.1)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(WNCK REQUIRED libwnck-3.0)

find_path(APPINDICATOR_INCLUDE_DIR NAMES libappindicator/app-indicator.h
    HINTS ${PC_APPINDICATOR_INCLUDEDIR} ${PC_APPINDICATOR_INCLUDE_DIRS}
    PATH_SUFFIXES libappindicator-0.1)

find_library(APPINDICATOR_LIBRARY NAMES appindicator)
if(APPINDICATOR_FOUND)
    set(APPINDICATOR_LIBRARIES ${APPINDICATOR_LIBRARY})
    set(APPINDICATOR_INCLUDE_DIRS ${APPINDICATOR_INCLUDE_DIR})
endif()

mark_as_advanced(APPINDICATOR_INCLUDE_DIR APPINDICATOR_LIBRARY)

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${GTK2_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIR} ${WNCK_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS} ${APPINDICATOR_LIBRARY} ${WNCK_LIBRARIES})

include(CheckCSourceCompiles)
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -DWNCK_I_KNOW_THIS_IS_UNSTABLE") 
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic")

get_target_property(MKDATA_EXE mkdata LOCATION)

set(SOURCES
    main.c
)

add_executable(wnck-test ${SOURCES})
target_link_libraries(wnck-test ${CMAKE_THREAD_LIBS_INIT} ${GTK2_LIBRARIES} ${APPINDICATOR_LIBRARY} ${WNCK_LIBRARIES})

install(TARGETS wnck-test DESTINATION bin)

set(SOURCES
    main1.c
    )

add_executable(wnck1-test ${SOURCES})
target_link_libraries(wnck1-test ${CMAKE_THREAD_LIBS_INIT} ${GTK2_LIBRARIES} ${APPINDICATOR_LIBRARY} ${WNCK_LIBRARIES})

install(TARGETS wnck1-test DESTINATION bin)
