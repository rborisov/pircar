cmake_minimum_required(VERSION 2.6)

project (indi-pirc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

#cc -o browser browser3.o 
#-lwebkitgtk-1.0 -lgtk-x11-2.0 -lsoup-2.4 -ljavascriptcoregtk-1.0 -lgdk-x11-2.0
#-latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0
#-lfontconfig -lfreetype -lgio-2.0 -lgobject-2.0 -lglib-2.0  

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(LibMPDClient REQUIRED)
#find_package(LibSoup REQUIRED)

pkg_check_modules(PC_APPINDICATOR REQUIRED appindicator-0.1)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
#pkg_check_modules(WEBKIT REQUIRED webkit-1.0)
pkg_check_modules(WNCK REQUIRED libwnck-1.0)

find_path(APPINDICATOR_INCLUDE_DIR NAMES libappindicator/app-indicator.h
    HINTS ${PC_APPINDICATOR_INCLUDEDIR} ${PC_APPINDICATOR_INCLUDE_DIRS}
    PATH_SUFFIXES libappindicator-0.1)

find_library(APPINDICATOR_LIBRARY NAMES appindicator)
if(APPINDICATOR_FOUND)
    set(APPINDICATOR_LIBRARIES ${APPINDICATOR_LIBRARY})
    set(APPINDICATOR_INCLUDE_DIRS ${APPINDICATOR_INCLUDE_DIR})
endif()

#find_path(WEBKIT_INCLUDE_DIR webkit/webkit.h
#    PATH_SUFFIXES "webkitgtk-1.0")
#find_library(WEBKIT_LIBRARY 
#    NAMES webkitgtk-1.0)

mark_as_advanced(APPINDICATOR_INCLUDE_DIR APPINDICATOR_LIBRARY)

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} 
    ${APPINDICATOR_INCLUDE_DIR} ${GTK2_INCLUDE_DIRS} ${WNCK_INCLUDE_DIRS}
    ${LIBMPDCLIENT_INCLUDE_DIRS})
#${WEBKIT_INCLUDE_DIR} ${LIBSOUP_INCLUDE_DIRS} ${GTK2_INCLUDE_DIRS})

link_directories(${APPINDICATOR_LIBRARY} ${GTK2_LIBRARY_DIRS} ${WNCK_LIBRARIES}
    ${LIBMPDCLIENT_LIBRARIES})
#${WEBKIT_LIBRARY} ${LIBSOUP_LIBRARIES} ${GTK2_LIBRARY_DIRS})

include(CheckCSourceCompiles)

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -DWNCK_I_KNOW_THIS_IS_UNSTABLE") 
set(CMAKE_C_FLAGS_DEBUG "-ggdb -pedantic -DWNCK_I_KNOW_THIS_IS_UNSTABLE")

get_target_property(MKDATA_EXE mkdata LOCATION)

set(SOURCES
    main.c
    rpc-utils.c
)

add_executable(indi-pirc ${SOURCES})
target_link_libraries(indi-pirc ${CMAKE_THREAD_LIBS_INIT} ${GTK2_LIBRARIES} ${WNCK_LIBRARIES}
    ${APPINDICATOR_LIBRARY} ${LIBMPDCLIENT_LIBRARIES}) 
#${WEBKIT_LIBRARY})

install(TARGETS indi-pirc DESTINATION bin)
