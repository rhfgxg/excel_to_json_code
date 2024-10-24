# cmake version configuration file for libxlsxio_write
GET_FILENAME_COMPONENT(XLSXIOREAD_ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)

# find library file
FIND_LIBRARY(xlsxio_write_LIBRARY xlsxio_write PATHS ${XLSXIOREAD_ROOT}/lib /opt/xlsxio/lib PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE} NO_DEFAULT_PATH )
SET(xlsxio_write_LIBRARIES ${xlsxio_write_LIBRARY} -lxlsxio_write unofficial::minizip::minizip )

# find include file
FIND_PATH(xlsxio_write_INCLUDE_DIR xlsxio_write.h PATHS ${XLSXIOREAD_ROOT}/include /opt/xlsxio/include NO_DEFAULT_PATH)
SET(xlsxio_write_INCLUDE_DIRS ${xlsxio_write_INCLUDE_DIR})

IF(NOT xlsxio_write_LIBRARY OR NOT xlsxio_write_INCLUDE_DIR)
  # find using pkg_config
  INCLUDE(FindPkgConfig)
  PKG_CHECK_MODULES(xlsxio_write REQUIRED xlsxio_write)
ELSE()
  SET(xlsxio_write_FOUND TRUE)
ENDIF()

IF(NOT xlsxio_write_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find library: xlsxio_write")
ENDIF()
