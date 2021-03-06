cmake_minimum_required(VERSION 2.6)

project (radio-pirc)
set(USERNAME "rborisov")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(STREAMRIPPER_LIB_PATH "${PROJECT_SOURCE_DIR}/../libstreamripper/")
set(STREAMRIPPER_SRC_PATH "${PROJECT_SOURCE_DIR}/../libstreamripper/src/")
set(CONFIG_LIB_PATH "${PROJECT_SOURCE_DIR}/../libconfig/")

#SET(CMAKE_BUILD_TYPE debug)

option(WITH_MPD_HOST_CHANGE "Let users of the web frontend change the MPD Host" ON)

find_package(LibMPDClient REQUIRED)
find_package(Threads REQUIRED)
find_package(GLIB2 REQUIRED)
find_package(CURL REQUIRED)
find_package(Sqlite3 REQUIRED)

MESSAGE(STATUS "GLIB2_INCLUDE_DIRS: " ${GLIB2_INCLUDE_DIRS})

configure_file(src/config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/src/" ${STREAMRIPPER_LIB_PATH} 
    ${STREAMRIPPER_SRC_PATH} ${GLIB2_INCLUDE_DIRS})

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -D__UNIX__")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -pedantic -DMONGOOSE_ENABLE_DEBUG")

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
install(TARGETS radio-pirc DESTINATION bin)
#install(FILES radio-pirc.1 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
install(FILES "${STREAMRIPPER_LIB_PATH}src/.libs/libstreamripper.so" DESTINATION lib)
install(FILES "${STREAMRIPPER_LIB_PATH}src/.libs/libstreamripper.so.0" DESTINATION lib)
install(FILES "${STREAMRIPPER_LIB_PATH}src/.libs/libstreamripper.so.0.0.0" DESTINATION lib)
#install(FILES "${CONFIG_LIB_PATH}lib/.libs/libconfig.so.9.1.3" DESTINATION lib)
#install(FILES "${CONFIG_LIB_PATH}lib/.libs/libconfig.so" DESTINATION lib)
#install(FILES "${CONFIG_LIB_PATH}lib/.libs/libconfig.so.9" DESTINATION lib)
install(FILES "${PROJECT_SOURCE_DIR}/files/ympd.conf" DESTINATION "/home/${USERNAME}/rcarmedia")
#install(FILES "${PROJECT_SOURCE_DIR}/files/mpd.conf" DESTINATION src)

#set(DBRADIO
#    tools/dbradio.c
#    src/sqlitedb.c
#    src/sql.c
#    src/radio.c
#    src/streamripper.c
#)

#add_executable(dbradio ${DBRADIO})
#target_link_libraries(dbradio ${SQLITE3_LIBRARIES} config streamripper mad ogg vorbis id3tag)
