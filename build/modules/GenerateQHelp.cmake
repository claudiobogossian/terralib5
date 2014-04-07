# Scripts for generating Doxygen documentation of the Terralib 5.0 sources

find_package (Qt4)
if(QT_FOUND)

  set (TVIEW_LOGO_ICON "./terralib-globe-small.png")

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
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TE_HELP_DIR}/help ${CMAKE_BINARY_DIR}/help
    DEPENDS del_dir
    COMMENT "Copying help directory..."
  )
  
  add_custom_command (
    OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhcp
    COMMAND  ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/config_qhelp.cmake
    DEPENDS copy_dir
    COMMENT "Copying icon..."
  )
  
  add_custom_command (
    OUTPUT copy_icon
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/../resources/images/png/terralib-globe-small.png ${CMAKE_BINARY_DIR}/help
    DEPENDS copy_dir
    COMMENT "Configuring help collection file..."
  )

  add_custom_command (
    OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhc
    COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${CMAKE_BINARY_DIR}/help/help.qhcp -o ${CMAKE_BINARY_DIR}/help/help.qhc
    DEPENDS copy_icon ${CMAKE_BINARY_DIR}/help/help.qhcp
    COMMENT "Building QHelp files ..."
  )

  add_custom_command (
    OUTPUT remove_files
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/apf
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/terraview
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/widgets
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help/plugins
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/about.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/sobre.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/widgets.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/terraview.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/plugins.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/apf.qhp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/help.qhcp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/terralib-globe-small.png
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