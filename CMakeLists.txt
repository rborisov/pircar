cmake_minimum_required( VERSION 2.8 )

project( CMakeDebHelper )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set( CPACK_PACKAGE_NAME "pirc" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Package summary" )
set( CPACK_PACKAGE_VENDOR "rborisov" )
set( CPACK_PACKAGE_VERSION_MAJOR "0" )
set( CPACK_PACKAGE_VERSION_MINOR "1" )
set( CPACK_PACKAGE_VERSION_PATCH "0" )
set( CPACK_WARN_ON_ABSOLUTE_INSTALL_DESTINATION TRUE )

# This is needed to set specific CPack-values when the DEB-generator is run
set( CPACK_PROJECT_CONFIG_FILE "${CMAKE_SOURCE_DIR}/CPackConfig.cmake" )

include(ExternalProject)
ExternalProject_Add(
    libstreamripper
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libstreamripper
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/libstreamripper/configure --prefix=${CMAKE_SOURCE_DIR}/libstreamripper
    PREFIX ${CMAKE_SOURCE_DIR}/libstreamripper
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    )

add_subdirectory( indi-pirc )
add_subdirectory( browser-pirc )
add_subdirectory( radio-pirc )

if( UNIX )
	add_subdirectory( debian )
endif()

include( CPack )
