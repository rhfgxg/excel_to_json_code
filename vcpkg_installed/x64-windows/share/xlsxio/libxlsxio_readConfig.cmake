# cmake version configuration file for libxlsxio_read
GET_FILENAME_COMPONENT(XLSXIOREAD_ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)

# find library file
FIND_LIBRARY(xlsxio_read_LIBRARY xlsxio_read PATHS ${XLSXIOREAD_ROOT}/lib /opt/xlsxio/lib PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE} NO_DEFAULT_PATH )
SET(xlsxio_read_LIBRARIES ${xlsxio_read_LIBRARY} -lxlsxio_read unofficial::minizip::minizip expat::expat)

# find include file
FIND_PATH(xlsxio_read_INCLUDE_DIR xlsxio_read.h PATHS ${XLSXIOREAD_ROOT}/include /opt/xlsxio/include NO_DEFAULT_PATH)
SET(xlsxio_read_INCLUDE_DIRS ${xlsxio_read_INCLUDE_DIR})

IF(NOT xlsxio_read_LIBRARY OR NOT xlsxio_read_INCLUDE_DIR)
  # find using pkg_config
  INCLUDE(FindPkgConfig)
  PKG_CHECK_MODULES(xlsxio_read REQUIRED xlsxio_read)
ELSE()
  SET(xlsxio_read_FOUND TRUE)
ENDIF()

IF(NOT xlsxio_read_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find library: xlsxio_read")
ENDIF()
