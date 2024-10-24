get_filename_component(VCPKG_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
INCLUDE(CMakeFindDependencyMacro)
IF (OFF)
  FIND_DEPENDENCY(LibZip)
ELSE()
  FIND_DEPENDENCY(unofficial-minizip CONFIG)
ENDIF()
IF (${VCPKG_IMPORT_PREFIX}/share/expat)
  FIND_PATH(EXPAT_INCLUDE_DIR NAMES expat.h NO_DEFAULT_PATH PATHS ${EXPAT_DIR}/include ${EXPAT_DIR})
  FIND_LIBRARY(EXPAT_LIBRARIES NAMES expat libexpat NO_DEFAULT_PATH PATHS ${EXPAT_DIR}/lib ${EXPAT_DIR})
ELSE()
  FIND_DEPENDENCY(expat CONFIG)
ENDIF()

IF(OFF)
  IF(${VCPKG_IMPORT_PREFIX}/share/expat)
    FIND_LIBRARY(EXPATW_LIBRARIES NAMES expatw libexpatw NO_DEFAULT_PATH PATHS ${EXPAT_DIR}/lib ${EXPAT_DIR})
  ELSE()
    FIND_LIBRARY(EXPATW_LIBRARIES NAMES expatw)
  ENDIF()
ENDIF()

INCLUDE("${CMAKE_CURRENT_LIST_DIR}/xlsxioTargets.cmake")
