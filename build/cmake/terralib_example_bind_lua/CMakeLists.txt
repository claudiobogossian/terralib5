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
#  Description: Build the example for the bind lua module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories( ${LUA_INCLUDE_DIR} )

#if(WIN32)
#  add_definitions(-DLUA_BUILD_AS_DLL)
#endif()

file(GLOB TERRALIB_SRC_FILES "${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/binding/lua/*.cpp" "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/examples/lua/geometry.lua")

add_executable(terralib_example_bind_lua ${TERRALIB_SRC_FILES})

target_link_libraries(terralib_example_bind_lua  terralib_mod_common ${LUA_LIBRARIES})

install(FILES ${TERRALIB_SRC_FILES} 
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/example/binding/lua COMPONENT devel)
