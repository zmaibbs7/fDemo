if(NOT TARGET cppcommon)

  # Module flag
  set(CPPCOMMON_MODULE Y)

  # Module subdirectory
  add_subdirectory("CppCommon-1.0.4.1")

  # Module folder
  set_target_properties(cppcommon PROPERTIES FOLDER "modules/CppCommon-1.0.4.1")

endif()
