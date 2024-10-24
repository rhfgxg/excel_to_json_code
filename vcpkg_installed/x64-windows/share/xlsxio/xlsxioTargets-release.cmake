#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xlsxio::xlsxio_read_SHARED" for configuration "Release"
set_property(TARGET xlsxio::xlsxio_read_SHARED APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xlsxio::xlsxio_read_SHARED PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/xlsxio_read.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "unofficial::minizip::minizip;expat::expat"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/xlsxio_read.dll"
  )

list(APPEND _cmake_import_check_targets xlsxio::xlsxio_read_SHARED )
list(APPEND _cmake_import_check_files_for_xlsxio::xlsxio_read_SHARED "${_IMPORT_PREFIX}/lib/xlsxio_read.lib" "${_IMPORT_PREFIX}/bin/xlsxio_read.dll" )

# Import target "xlsxio::xlsxio_write_SHARED" for configuration "Release"
set_property(TARGET xlsxio::xlsxio_write_SHARED APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xlsxio::xlsxio_write_SHARED PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/xlsxio_write.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "unofficial::minizip::minizip"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/xlsxio_write.dll"
  )

list(APPEND _cmake_import_check_targets xlsxio::xlsxio_write_SHARED )
list(APPEND _cmake_import_check_files_for_xlsxio::xlsxio_write_SHARED "${_IMPORT_PREFIX}/lib/xlsxio_write.lib" "${_IMPORT_PREFIX}/bin/xlsxio_write.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
