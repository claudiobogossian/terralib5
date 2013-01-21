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

#find_package (terralib 5 COMPONENTS color common dataaccess datatype filter geometry
#    maptools memory plugin qt_widgets raster rp srs st stloader symbology qt_af serialization REQUIRED)

set (ICON_THEME "terralib" CACHE STRING "Defines the default theme to be used.")
set (ICON_THEME_PATH "${ROOT}/resources/themes" CACHE STRING "Lists of directories that contains icon themes.")

include (${CMAKE_BINARY_DIR}/terralibConfig.cmake)

add_definitions(-DICON_THEME=\"${ICON_THEME}\" -DICON_THEME_PATH=\"${ICON_THEME_PATH}\" -DTERRALIB_SCHEMA_LOCATION=\"${TERRALIB_SCHEMA_LOCATION}\")
    
#if(terralib_FOUND)

#  include (${T5_MODULEPATH}/GeneralConfig.cmake)

find_package(Boost REQUIRED COMPONENTS system)

include_directories (${TE_INCLUDE_DIRS} ${INCLDIR} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
list (APPEND _dep_libs ${TE_LIBRARIES} ${QT_LIBRARIES} ${Boost_LIBRARIES}) 

file (GLOB SRCS ${SRCDIR}/*.cpp)
file (GLOB HDRS ${INCLDIR}/*.h)
  
  # Select the FORMS widgets     
#  file (GLOB FORMS ${SRCDIR}/ui/*.ui)
  
  # Select the header files for moc'ing
#  file (
#    GLOB
#    HDRS_TO_MOC 
#    ${INCLDIR}/MainWindow.h
#    ${INCLDIR}/NewGDALLayer.h
#    ${INCLDIR}/NewOGRLayer.h
#  )
#  te_moc2("${HDRS_TO_MOC}" "terraview" MOC)
  
#  qt4_wrap_ui (UI ${FORMS})

#  source_group("Form Files" FILES ${FORMS})
#  source_group("Generated Files" FILES ${UI} ${MOC})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TE_CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TE_CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TE_CMAKE_LIBRARY_OUTPUT_DIRECTORY}")  

add_definitions (-DBOOST_ALL_NO_LIB -DTERRALIB_BIN_DIR=\"${TERRALIB_BIN_DIR}\" -DTERRALIB_LOGO=\"${TERRALIB_LOGO}\")
add_executable(${PROJ_NAME} ${SRCS} ${HDRS})
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
target_link_libraries(${PROJ_NAME} ${_dep_libs})

message ("Generating configurations files in directory \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\".")
  
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.xml)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/user_settings.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/user_settings.xml)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/application_plugins.xml.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/application_plugins.xml)
