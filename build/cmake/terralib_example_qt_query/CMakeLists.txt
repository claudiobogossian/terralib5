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
#  Description: The Qt Query example.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#


include_directories(${Boost_INCLUDE_DIR})

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/qt/query/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/qt/query/*.h)

add_executable(terralib_example_qt_query ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

if(Qt5_FOUND)

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  target_link_libraries(terralib_example_qt_query terralib_mod_dataaccess
                                                  terralib_mod_plugin
                                                  terralib_mod_qt_widgets)

  qt5_use_modules(terralib_example_qt_query Widgets)

else()

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

  target_link_libraries(terralib_example_qt_query terralib_mod_dataaccess
                                                  terralib_mod_plugin
                                                  terralib_mod_qt_widgets
                                                  ${QT_LIBRARIES})

endif()

install(FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/qt/query COMPONENT devel)
