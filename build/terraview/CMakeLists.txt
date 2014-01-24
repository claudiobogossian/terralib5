# This script builds the datagrid example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terraview)             # directory where the source files of the datagrid example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the datagrid example are located

# Set the project name
set(PROJ_NAME "terraview")
project(${PROJ_NAME})         # the project name is terralib_example_datagrid

find_package(Boost ${_Boost_VERSION} REQUIRED COMPONENTS system)

if(Boost_FOUND)
  list (APPEND _dep_incs ${Boost_INCLUDE_DIRS})
  list (APPEND _dep_libs ${Boost_LIBRARIES})
endif()

find_package(Qt4 ${_Qt4_VERSION} REQUIRED COMPONENTS QtCore QtGui)

if(QT4_FOUND)
  include (${QT_USE_FILE})
  add_definitions (${QT_DEFINITIONS})

  list (APPEND _dep_incs ${QT_INCLUDE_DIR})
  list (APPEND _dep_libs ${QT_LIBRARIES})
  
endif()

list (APPEND 
  _dep_incs 
  "${CMAKE_BINARY_DIR}" 
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${ROOT}/src"
  "${INCLDIR}" 
)

include_directories (${_dep_incs})

list (APPEND 
  _dep_libs 
  terralib_common
  terralib_qt_af
  terralib_qt_widgets
)

file (GLOB SRCS ${SRCDIR}/*.cpp)
file (GLOB HDRS ${INCLDIR}/*.h)
  
  # Select the FORMS widgets     
file (GLOB FORMS ${SRCDIR}/ui/*.ui)
  
  # Select the header files for moc'ing
file (
  GLOB
  HDRS_TO_MOC 
  ${INCLDIR}/TerraView.h
)
te_moc2("${HDRS_TO_MOC}" "terraview" MOC)
  
qt4_wrap_ui (UI ${FORMS})

source_group("Form Files" FILES ${FORMS})
source_group("Generated Files" FILES ${UI} ${MOC})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}") 

get_filename_component(TVIEW_OUT_DIR "${TVIEW_CONFIG_FILE}" PATH)

set(TVIEW_TE_LOGO "${ROOT}/resources/images/png/terralib.png")
set(TVIEW_LOGO "${ROOT}/resources/images/png/terraview.png")
set(TVIEW_ICO "${ROOT}/resources/images/png/terralib-globe-small.png")
set(TVIEW_LARGE_LOGO "${ROOT}/resources/images/png/terraview-textual.png")
set(TVIEW_SPLASH_SCREEN_PIXMAP "${ROOT}/resources/images/png/terraview-splashscreen.png")

if(TE_WIN32_GUI)
  set (GUI_TYPE WIN32)
  list (APPEND _dep_libs ${QT_QTMAIN_LIBRARY})
else()
  set (GUI_TYPE)
endif()

add_definitions (-DBOOST_ALL_NO_LIB)

add_executable(${PROJ_NAME} ${GUI_TYPE} ${SRCS} ${HDRS} ${UI} ${MOC})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${_dep_libs})

message ("Generating configurations files in directory \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\".")
  
set (TE_DEFAULT_PROJECT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default_project.xml")  
set (TE_DEFAULT_DATASOURCES_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/datasources.xml") 
 
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/user_settings.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/user_settings.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/application_plugins.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/application_plugins.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/project.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/default_project.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/datasources.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/datasources.xml)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/TerraViewConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/TerraViewConfig.h)

install(
  TARGETS ${PROJ_NAME}
  RUNTIME DESTINATION "bin" COMPONENT BINARIES
  LIBRARY DESTINATION "bin" COMPONENT BINARIES
  ARCHIVE DESTINATION "bin" COMPONENT BINARIES
)

# install tree for config files
include (${CMAKE_CURRENT_SOURCE_DIR}/installConfig.cmake)
set (TE_DEFAULT_PROJECT "./default_project.xml")  
set (TE_DEFAULT_DATASOURCES_FILE "./datasources.xml") 
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.xml.in ${CMAKE_BINARY_DIR}/toInstall/config.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/user_settings.xml.in ${CMAKE_BINARY_DIR}/toInstall/user_settings.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/application_plugins.xml.in ${CMAKE_BINARY_DIR}/toInstall/application_plugins.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/project.xml.in ${CMAKE_BINARY_DIR}/toInstall/default_project.xml)
#configure_file (${CMAKE_CURRENT_SOURCE_DIR}/datasources.xml.in ${CMAKE_BINARY_DIR}/toInstall/datasources.xml)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/TerraViewConfig.h.in ${CMAKE_BINARY_DIR}/toInstall/TerraViewConfig.h)
  
file (
  WRITE
  ${CMAKE_BINARY_DIR}/qt.conf
  "[Paths]
Binaries = .
Plugins = plugins"
)

install (
  FILES ${CMAKE_BINARY_DIR}/qt.conf
  DESTINATION bin
  COMPONENT BINARIES
)