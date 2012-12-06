# This script builds the datagrid example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terraview)             # directory where the source files of the datagrid example are located
set(INCLDIR ${SRCDIR})                            # directory where the header files of the datagrid example are located

# Set the project name
set(PROJ_NAME "terraView")
project(${PROJ_NAME})         # the project name is terralib_example_datagrid

find_package (terralib 5 COMPONENTS color common dataaccess datatype filter geometry
    maptools memory plugin qtwidgets raster rp srs st stloader symbology af serialization REQUIRED)

set (ICON_THEME "terralib" CACHE STRING "Defines the default theme to be used.")
set (ICON_THEME_PATH "${ROOT}/resources/themes" CACHE STRING "Lists of directories that contains icon themes.")

add_definitions(-DICON_THEME=\"${ICON_THEME}\" -DICON_THEME_PATH=\"${ICON_THEME_PATH}\" -DTERRALIB_SCHEMA_LOCATION=\"${TERRALIB_SCHEMA_LOCATION}\")
    
if(terralib_FOUND)

  include (${T5_MODULEPATH}/GeneralConfig.cmake)

  find_package(Boost REQUIRED COMPONENTS system)
  
  include_directories (${TE_INCLUDE_DIRS} ${INCLDIR} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
  list (APPEND _dep_libs ${TE_LIBRARIES} ${QT_LIBRARIES} ${Boost_LIBRARIES}) 
  
  file (GLOB SRCS ${SRCDIR}/*.cpp)
  file (GLOB HDRS ${INCLDIR}/*.h)
  
  # Select the FORMS widgets     
  file (GLOB FORMS ${SRCDIR}/ui/*.ui)
  
  # Select the header files for moc'ing
  file (
    GLOB
    HDRS_TO_MOC 
    ${INCLDIR}/MainWindow.h
    ${INCLDIR}/NewGDALLayer.h
    ${INCLDIR}/NewOGRLayer.h
  )
  te_moc2("${HDRS_TO_MOC}" "terraview" MOC)
  
  qt4_wrap_ui (UI ${FORMS})

  source_group("Form Files" FILES ${FORMS})
  source_group("Generated Files" FILES ${UI} ${MOC})
  
  add_definitions (-DBOOST_ALL_NO_LIB -DTERRALIB_BIN_DIR=\"${TERRALIB_BIN_DIR}\" -DTERRALIB_LOGO=\"${TERRALIB_LOGO}\")
  add_executable(${PROJ_NAME} ${SRCS} ${HDRS} ${UI} ${MOC})
  set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
  target_link_libraries(${PROJ_NAME} ${_dep_libs})

  if(WIN32)
    message (STATUS "-- Generating run batch file.")
    if(_NOTFOUND)
      message (WARNING "Batch file may not contain paths to all searched dlls.")
    endif()
    
    # Release settings
    set (GDAL_BIN ${TE_DEPENDENCIES_DIR}/gdal/win32/release)
    generateRunningBatch(${T5_MODULEPATH}/runme.bat.in runme.bat)
  endif()  
endif()

