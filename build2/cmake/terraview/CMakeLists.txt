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
#  Description: Build configuration for TerraView.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/Config.h.in
               ${CMAKE_BINARY_DIR}/terraview_config.h @ONLY)

include_directories(${Boost_INCLUDE_DIR})

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/*.h)
file(GLOB TERRALIB_UI_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/ui/*.ui)

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

# uic'ing
  QT5_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES})

  add_executable(terraview ${TERRALIB_FILES})

  target_link_libraries(terraview terralib_mod_qt_apf)

  qt5_use_modules(terraview Widgets)

else()

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

# uic'ing
  QT4_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

# moc'ing
  set(TERRALIB_HDRS_TO_MOC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/TerraView.h)

  QT4_WRAP_CPP(TERRALIB_GEN_SRC_FILES ${TERRALIB_HDRS_TO_MOC_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES} ${TERRALIB_GEN_SRC_FILES})

  add_executable(terraview ${TERRALIB_FILES})

  target_link_libraries(terraview terralib_mod_qt_apf
                                  ${QT_LIBRARIES})

endif()

install(TARGETS terraview
        EXPORT terralib-targets
        RUNTIME DESTINATION ${TERRALIB_DESTINATION_RUNTIME} COMPONENT runtime
        LIBRARY DESTINATION ${TERRALIB_DESTINATION_LIBRARY} COMPONENT runtime
        ARCHIVE DESTINATION ${TERRALIB_DESTINATION_ARCHIVE} COMPONENT runtime)

install(FILES ${TERRALIB_HDR_FILES}
              ${TERRALIB_GEN_HDR_FILES}
              ${CMAKE_BINARY_DIR}/terraview_config.h
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terraview COMPONENT devel)

install(DIRECTORY ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terraview
        DESTINATION share/terraview COMPONENT runtime)

if(TERRALIB_TRACK_3RDPARTY_DEPENDENCIES)
  TeInstallQtPlugins("qgif;qico;qjpeg;qmng;qtiff;qsvg;qsvgicon;qsqlite")

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
       "[Paths]\nBinaries = .\nPlugins = ../qtplugins")

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
          DESTINATION ${TERRALIB_DESTINATION_RUNTIME}
          COMPONENT runtime)
endif()

export(TARGETS terraview APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
