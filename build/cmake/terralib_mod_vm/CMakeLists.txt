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
#  Description: Build configuration for the Virtual Machine Module.
#
#  Author: Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

include_directories(${Boost_INCLUDE_DIR})

if(WIN32)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DTEVMDLL)
endif()

set(TERRALIB_LIBRARIES_DEPENDENCIES terralib_mod_core terralib_mod_common)

set(TERRALIB_VM_DIR ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/vm)

file(GLOB TERRALIB_VM_FILES ${TERRALIB_VM_DIR}/*.h)

file(GLOB TERRALIB_VM_HDR_FILES ${TERRALIB_VM_DIR}/core/*.h)
file(GLOB TERRALIB_VM_SRC_FILES ${TERRALIB_VM_DIR}/core/*.cpp)

source_group("Header Files"                  FILES ${TERRALIB_VM_FILES})
source_group("Header Files\\core"            FILES ${TERRALIB_VM_HDR_FILES})
source_group("Source Files\\core"            FILES ${TERRALIB_VM_SRC_FILES})

add_library(terralib_mod_vm SHARED ${TERRALIB_VM_FILES} ${TERRALIB_VM_HDR_FILES} ${TERRALIB_VM_SRC_FILES})

target_link_libraries(terralib_mod_vm ${TERRALIB_LIBRARIES_DEPENDENCIES})

set_target_properties(terralib_mod_vm
                      PROPERTIES VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}
                                 SOVERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR})

install(TARGETS terralib_mod_vm
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

