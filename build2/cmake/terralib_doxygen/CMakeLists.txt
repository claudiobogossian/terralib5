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
#  Description: Script for generating the documentation with Doxygen.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/doc/api/Doxyfile.in
               ${CMAKE_CURRENT_BINARY_DIR}/doxygen.txt @ONLY)

add_custom_target(terralib_doxygen
                  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.txt
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating API documentation with Doxygen" VERBATIM)

install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} terralib_doxygen WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
        DESTINATION share/terralib COMPONENT devel)
