# Scripts for generating Doxygen documentation of the Terralib 5.0 sources

find_package (Qt4)
if(QT_FOUND)

  file(
    WRITE
    ${CMAKE_BINARY_DIR}/config_qhelp.cmake
    "configure_file (${CMAKE_SOURCE_DIR}/help.qhcp.in ${CMAKE_BINARY_DIR}/help/help.qhcp)"
  )

  add_custom_command (
    OUTPUT del_dir
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help
    COMMENT "Removing help directory..."
  )
  
  add_custom_command (
    OUTPUT copy_dir
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ROOT}/resources/help ${CMAKE_BINARY_DIR}/help
    DEPENDS del_dir
    COMMENT "Copying help directory..."
  )
  
  add_custom_command (
    OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhcp
    COMMAND  ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/config_qhelp.cmake
    DEPENDS copy_dir
    COMMENT "Configuring help collection file..."
  )
  
  add_custom_command (
    OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhc
    COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${CMAKE_BINARY_DIR}/help/help.qhcp -o ${CMAKE_BINARY_DIR}/help/help.qhc
    DEPENDS ${CMAKE_BINARY_DIR}/help/help.qhcp
    COMMENT "Building QHelp files ..."
  )

  add_custom_command (
    OUTPUT remove_files
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/apf
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/terraview
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/widgets
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/about.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/sobre.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/widgets.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/terraview.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/help.qhcp
    DEPENDS ${CMAKE_BINARY_DIR}/help/help.qhc
    COMMENT "Removing copied files"
  )  
    
  add_custom_target (
    "Generate QHelp"
    DEPENDS remove_files
    COMMENT "Generate qhelp project"
    VERBATIM
  )
  
  install(
    DIRECTORY ${CMAKE_BINARY_DIR}/help
    DESTINATION . 
    COMPONENT BINARIES
  )
  
endif()