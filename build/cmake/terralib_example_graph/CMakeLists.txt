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
#  Description: Example for the TerraLib Graph module.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#


include_directories(${Boost_INCLUDE_DIR})

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/graph/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/graph/*.h)

if(Qt5_FOUND)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  add_executable(terralib_example_graph ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES})

  target_link_libraries(terralib_example_graph terralib_mod_graph
                                               terralib_mod_plugin
                                               terralib_mod_qt_widgets
											   terralib_mod_sa_core
                                               ${BOOST_SYSTEM_LIBRARY})

  qt5_use_modules(terralib_example_graph Widgets)

else()

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/examples/graph/DisplayWindow.h)
  
# moc'ing
  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES})

  add_executable(terralib_example_graph ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_SRC_FILES})

  target_link_libraries(terralib_example_graph terralib_mod_graph
                                               terralib_mod_plugin
                                               terralib_mod_qt_widgets
											   terralib_mod_sa_core
                                               ${BOOST_SYSTEM_LIBRARY}
                                               ${QT_LIBRARIES})

endif()

install(FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_EXAMPLES}/graph COMPONENT devel)
