cmake_minimum_required(VERSION 2.6)

project (indicator-test)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

find_package(Threads REQUIRED)
find_package(Sqlite3 REQUIRED)

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${SQLITE3_INCLUDE_DIR})
link_directories(${SQLITE3_LIBRARIES})

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall") 
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic")

get_target_property(MKDATA_EXE mkdata LOCATION)

set(SOURCES
    main.c
)

set(CMAKE_BUILD_TYPE Debug)

add_executable(db-test ${SOURCES})
target_link_libraries(db-test ${CMAKE_THREAD_LIBS_INIT} ${SQLITE3_LIBRARIES})

#install(TARGETS db-test DESTINATION bin)
