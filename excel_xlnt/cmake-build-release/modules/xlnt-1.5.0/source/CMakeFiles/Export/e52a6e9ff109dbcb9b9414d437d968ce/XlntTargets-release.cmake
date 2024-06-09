#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "xlnt::xlnt" for configuration "Release"
set_property(TARGET xlnt::xlnt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(xlnt::xlnt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/xlnt.lib"
  )

list(APPEND _cmake_import_check_targets xlnt::xlnt )
list(APPEND _cmake_import_check_files_for_xlnt::xlnt "${_IMPORT_PREFIX}/lib/xlnt.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
