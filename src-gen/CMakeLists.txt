if (GENDBUS)
    find_program(GDBUS_CODEGEN_EXECUTABLE NAMES gdbus-codegen DOC "gdbus-codegen executable")
    if(NOT GDBUS_CODEGEN_EXECUTABLE)
        message(FATAL_ERROR "Executable gdbus-codegen not found")
    endif(NOT GDBUS_CODEGEN_EXECUTABLE)

    execute_process(COMMAND ${GDBUS_CODEGEN_EXECUTABLE}
            --c-namespace=Skype
            --generate-c-code=${CMAKE_CURRENT_SOURCE_DIR}/DBUSAPI
            --interface-prefix=com.Skype.
            ${CMAKE_SOURCE_DIR}/assets/skype_dbus_interface.xml
            RESULT_VARIABLE codegen_failed)
    if(codegen_failed)
        message(FATAL_ERROR "Error in generating code for dbus interface using gdbus-codegen")
    endif(codegen_failed)
endif(GENDBUS)