#
#  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.
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
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#

include_directories(${LUA_INCLUDE_DIR})

file(GLOB TERRALIB_VM_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/vm/lua/*.h)
file(GLOB TERRALIB_VM_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/vm/lua/*.cpp)

source_group("Header Files"                  FILES ${TERRALIB_VM_FILES})
source_group("Header Files\\core"            FILES ${TERRALIB_VM_HDR_FILES})
source_group("Source Files\\core"            FILES ${TERRALIB_VM_SRC_FILES})

add_library(terralib_mod_vm_lua SHARED ${TERRALIB_VM_HDR_FILES} ${TERRALIB_VM_SRC_FILES})

target_link_libraries(terralib_mod_vm_lua terralib_mod_vm
                                          terralib_mod_core
                                          ${LUA_LIBRARIES})

set_target_properties(terralib_mod_vm_lua
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_vm_lua
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

