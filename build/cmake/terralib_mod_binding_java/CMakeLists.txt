#
#  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: CMake script for the TerraLib Java binding.
#
#  Author: Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include ( ${SWIG_USE_FILE} )

file (GLOB TERRALIB_SWIG_SCRIPT_FILE "../../../src/terralib/binding/swig/TerraLibJAVA.i")

set_source_files_properties (${TERRALIB_SWIG_SCRIPT_FILE} PROPERTIES CPLUSPLUS ON CMAKE_SWIG_FLAGS "-includeall")

include_directories (
  "${TERRALIB_ABSOLUTE_ROOT_DIR}/src"
  "${CMAKE_BINARY_DIR}"
  "${Boost_INCLUDE_DIR}"
  "${JAVA_INCLUDE_PATH}"
  "${JAVA_INCLUDE_PATH2}"
)

set (TERRALIB_CHDR 
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/MultiImager.h
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/QtInitializer.h
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/Styler.h
)

set (TERRALIB_CSRC 
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/QtInitializer.cpp
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/Styler.cpp
)

set (
  TERRALIB_HDRS_TO_MOC_FILES
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/MultiImager.h
  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/binding/swig/java/Styler.h
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/br/inpe/terralib5" CACHE PATH "Directory for swig output.")

set (CMAKE_SWIG_FLAGS -package "br.inpe.terralib5")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qt.conf.in ${CMAKE_CURRENT_BINARY_DIR}/qt.conf COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tlib.qrc.in ${CMAKE_CURRENT_BINARY_DIR}/tlib.qrc COPYONLY)

if(WIN32)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/netbeans.bat.in ${CMAKE_CURRENT_BINARY_DIR}/netbeans.bat COPYONLY)

	install (
	  FILES ${CMAKE_CURRENT_BINARY_DIR}/netbeans.bat 
	  DESTINATION .
	  COMPONENT devel
	)
endif()

qt5_add_resources(_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/tlib.qrc)

SWIG_ADD_MODULE (terralib_mod_binding_java java ${TERRALIB_SWIG_SCRIPT_FILE} ${TERRALIB_CHDR} ${TERRALIB_CSRC} ${_RESOURCES})
  
SWIG_LINK_LIBRARIES (terralib_mod_binding_java
  terralib_mod_annotationtext
  terralib_mod_color
  terralib_mod_common
  terralib_mod_dataaccess
  terralib_mod_datatype
  terralib_mod_filter
  terralib_mod_geometry
  terralib_mod_maptools
  terralib_mod_memory
  terralib_mod_raster
  terralib_mod_symbology
  terralib_mod_srs
  terralib_mod_plugin
  terralib_mod_qt_widgets
  ${Boost_FILESYSTEM_LIBRARY} 
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)

qt5_use_modules(terralib_mod_binding_java Widgets)

if(WIN32)  
  install(
    TARGETS terralib_mod_binding_java
    RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
    LIBRARY DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
    ARCHIVE DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
  )
endif()

add_custom_command(
    TARGET terralib_mod_binding_java
    POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin
  COMMAND "${Java_JAVAC_EXECUTABLE}" -d ${CMAKE_CURRENT_BINARY_DIR}/bin ${CMAKE_SWIG_OUTDIR}/*.java 
    COMMAND "${Java_JAVADOC_EXECUTABLE}" -sourcepath ${CMAKE_CURRENT_BINARY_DIR} -d ${CMAKE_CURRENT_BINARY_DIR}/doc -use -splitindex -windowtitle "TerraLib 5 Java Binding API Specification" -J-Xmx180m br.inpe.terralib5
    COMMAND "${Java_JAR_EXECUTABLE}" cf ${CMAKE_CURRENT_BINARY_DIR}/terralib5.jar -C ${CMAKE_CURRENT_BINARY_DIR}/bin br
    COMMENT "Building java..." VERBATIM
)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/terralib5.jar 
  DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/java
  COMPONENT devel
)

install (
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
  DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/java
  COMPONENT devel
)
