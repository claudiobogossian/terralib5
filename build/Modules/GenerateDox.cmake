# Scripts for generating Doxygen files from Terralib 5.0.0 sources

find_package (Doxygen)
if(DOXYGEN_FOUND)
  configure_file (Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  set (DOX_IN ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  install (CODE "message(\"Building API Documentation...\")" COMPONENT DOCS)
  install (CODE "execute_process ( COMMAND \"${DOXYGEN_EXECUTABLE}\" \"${DOX_IN}\" WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\" )" COMPONENT DOCS)
  install (CODE "message(\"Documentation generated.\")" COMPONENT DOCS)
  
  install (
    DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/doc/html
    DESTINATION doc
    COMPONENT DOCS
  )
  
  add_custom_target (
    DOC
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOX_IN} 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building API Documentation..."
  )
endif()