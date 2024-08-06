if(NOT TARGET cppzmq)

    # Module subdirectory
    add_subdirectory("cppzmq-4.10.0")

    # Module folder
    set_target_properties(cppzmq PROPERTIES FOLDER "modules/cppzmq-4.10.0")

endif()
