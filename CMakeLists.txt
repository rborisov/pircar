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

add_subdirectory( libstreamripper )
add_subdirectory( indi-pirc )
add_subdirectory( browser-pirc )
add_subdirectory( radio-pirc )

if( UNIX )
	add_subdirectory( debian )
endif()

include( CPack )
