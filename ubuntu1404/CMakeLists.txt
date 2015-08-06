cmake_minimum_required(VERSION 2.6)

project (browser-pirc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")

install(FILES "${PROJECT_SOURCE_DIR}/mpd" 
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
		    GROUP_EXECUTE GROUP_READ
		    WORLD_EXECUTE WORLD_READ
	DESTINATION bin)
install(CODE "execute_process(COMMAND sudo chown rborisov ${PROJECT_SOURCE_DIR}/mpd.conf)")
install(FILES "${PROJECT_SOURCE_DIR}/mpd.conf" DESTINATION ../etc)
install(FILES "${PROJECT_SOURCE_DIR}/ympd.conf" DESTINATION ../etc)
#install(CODE "execute_process(COMMAND chown rborisov ${DESTINATION}../etc/mpd.conf)")
