cmake_minimum_required(VERSION 2.6)

project (radio-pirc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(STREAMRIPPER_LIB_PATH "${PROJECT_SOURCE_DIR}/../libstreamripper/")
set(STREAMRIPPER_SRC_PATH "${PROJECT_SOURCE_DIR}/../libstreamripper/src/")

#SET(CMAKE_BUILD_TYPE debug)
#SET(USE_GTK3 "1")

option(WITH_MPD_HOST_CHANGE "Let users of the web frontend change the MPD Host" ON)

find_package(LibMPDClient REQUIRED)
find_package(Threads REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(CURL REQUIRED)
find_package(Sqlite3 REQUIRED)
#find_package(GTK3 REQUIRED)
#find_package(WebKit3 REQUIRED)
#find_package(LibSoup2 REQUIRED)
#find_package(MySQL REQUIRED)

MESSAGE(STATUS "GLIB2_INCLUDE_DIRS: " ${GLIB2_INCLUDE_DIRS})

configure_file(src/config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${STREAMRIPPER_LIB_PATH} ${STREAMRIPPER_SRC_PATH} ${GLIB2_INCLUDE_DIRS})
#${GTK3_INCLUDE_DIRS} ${GTK3_LIBRARIES} ${WEBKIT_INCLUDE_DIR} ${LIBSOUP2_INCLUDE_DIRS})

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -D__UNIX__")
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic")

file(GLOB RESOURCES
	RELATIVE ${PROJECT_SOURCE_DIR}
	htdocs/js/*
	htdocs/assets/*
	htdocs/css/*.min.css
	htdocs/fonts/*
	htdocs/*.html
)

add_executable(mkdata htdocs/mkdata.c)
get_target_property(MKDATA_EXE mkdata LOCATION)

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/assets.c
    COMMAND ${MKDATA_EXE} ${RESOURCES} > ${PROJECT_BINARY_DIR}/assets.c
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS ${RESOURCES} mkdata
)

set(SOURCES
    src/ympd.c
    src/http_server.c
    src/mpd_client.c
    src/mongoose.c
    src/json_encode.c
    src/streamripper.c
    src/sqlitedb.c
    src/sql.c
    src/mpd_utils.c
	src/radio.c
    assets.c
)

add_executable(radio-pirc ${SOURCES})
target_link_libraries(radio-pirc ${LIBMPDCLIENT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} streamripper mad ogg vorbis config id3tag sqlite3 curl)
#-logg -lvorbis -lmad -lid3tag -lz -lglib-2.0
install(TARGETS radio-pirc DESTINATION bin)
install(FILES radio-pirc.1 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)