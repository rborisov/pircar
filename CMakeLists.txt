project (pirc-pack)
cmake_minimum_required (VERSION 2.8)
 
set(VERSION "1.0.1")

#set (BIN_DIR "bin")
#set (UTILS_DIR "utils")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

#set(CMAKE_SYSTEM_PROCESSOR "i686")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" CACHE STRING "c++ flags")
#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32" CACHE STRING "c flags")

set (INSTALL_DIR "/usr")

#add_subdirectory ( indi-pirc )
#add_subdirectory ( browser-pirc )
add_subdirectory ( radio-pirc )

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "pircar")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Roman Borisov")
set(CPACK_PACKAGE_VENDOR "rborisov")
set(CPACK_INSTALL_PREFIX "${INSTALL_DIR}")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CPACK_INSTALL_PREFIX}")
#set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)

