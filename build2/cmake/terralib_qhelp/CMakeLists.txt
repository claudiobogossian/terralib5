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
#  Description: Script for generating the application help in qt format.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

file(WRITE
     ${CMAKE_BINARY_DIR}/config_qhelp.cmake
     "configure_file (${TERRALIB_ABSOLUTE_ROOT_DIR}/doc/qhelp/help.qhcp.in ${CMAKE_BINARY_DIR}/help/help.qhcp)"
)

add_custom_command(
  OUTPUT del_dir
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/help
  COMMENT "Removing QHelp directory..."
)

add_custom_command(
  OUTPUT copy_dir
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TERRALIB_ABSOLUTE_ROOT_DIR}/doc/qhelp ${CMAKE_BINARY_DIR}/help
  DEPENDS del_dir
  COMMENT "Copying QHelp directory..."
)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhcp
  COMMAND  ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/config_qhelp.cmake
  DEPENDS copy_dir
  COMMENT "Configuring QHelp collection file..."
)

add_custom_command(
  OUTPUT copy_icon
  COMMAND  ${CMAKE_COMMAND} -E copy ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/images/png/terralib-globe.png ${CMAKE_BINARY_DIR}/help
  DEPENDS copy_dir
  COMMENT "Copying icon file..."
)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/help/help.qhc
  COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${CMAKE_BINARY_DIR}/help/help.qhcp -o ${CMAKE_BINARY_DIR}/help/help.qhc
  DEPENDS copy_icon ${CMAKE_BINARY_DIR}/help/help.qhcp
  COMMENT "Building QHelp files ..."
)

add_custom_command(
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
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/help/terralib-globe.png
  DEPENDS ${CMAKE_BINARY_DIR}/help/help.qhc
  COMMENT "Removing copied files"
)

add_custom_target(
  "Generate QHelp"
  DEPENDS remove_files
  COMMENT "Generate QHelp project"
  VERBATIM
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/help
        DESTINATION share/terraview/help COMPONENT runtime)
