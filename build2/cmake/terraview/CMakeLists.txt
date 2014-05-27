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

# Macro installQtPlugins
#
# Installs the required Qt plugins.
#
# param plugs List of the names of plugins to be installed.
MACRO(installQtPlugins plgs)

  if ("${QT_PLUGINS_DIR}" STREQUAL "")
    set (QT_PLUGINS_DIR "${Qt5_DIR}/../../../plugins")
  endif()

  set (_regex_exp "")

  set( _first TRUE)

  foreach(plg ${plgs})
    if(NOT _first)
      set (_regex_exp ${_regex_exp}|${plg})
    else()
      set (_regex_exp ${plg})
      set (_first FALSE)
    endif()
  endforeach()

  set (_regex_exp "(${_regex_exp})(${CMAKE_SHARED_LIBRARY_SUFFIX})$")

  set (_dest qtplugins)

  install (DIRECTORY ${QT_PLUGINS_DIR}/imageformats  
    DESTINATION ${_dest} 
    COMPONENT runtime
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

  install (DIRECTORY ${QT_PLUGINS_DIR}/iconengines  
    DESTINATION ${_dest} 
    COMPONENT runtime
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

  install (DIRECTORY ${QT_PLUGINS_DIR}/sqldrivers  
    DESTINATION ${_dest} 
    COMPONENT runtime
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

ENDMACRO(installQtPlugins)


configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/Config.h.in
               ${CMAKE_BINARY_DIR}/terraview_config.h @ONLY)

include_directories(${Boost_INCLUDE_DIR})

if(Qt5_FOUND)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

else() # QT4_FOUND

  include_directories(${CMAKE_CURRENT_BINARY_DIR})

  include(${QT_USE_FILE})

  include_directories(${QT_INCLUDE_DIR})

  add_definitions(${QT_DEFINITIONS})

endif()

file(GLOB TERRALIB_SRC_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/*.cpp)
file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/*.h)
file(GLOB TERRALIB_UI_FILES  ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview/ui/*.ui)

if(Qt5_FOUND)

# uic'ing
  QT5_WRAP_UI(TERRALIB_GEN_HDR_FILES ${TERRALIB_UI_FILES})

  set(TERRALIB_FILES ${TERRALIB_SRC_FILES} ${TERRALIB_HDR_FILES} ${TERRALIB_GEN_HDR_FILES})

  add_executable(terraview ${TERRALIB_FILES})

  target_link_libraries(terraview terralib_mod_qt_apf)

  qt5_use_modules(terraview Widgets)

else()

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

installQtPlugins("qgif;qico;qjpeg;qmng;qtiff;qsvg;qsvgicon;qsqlite")

file (
  WRITE
  ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
  "[Paths]
  Binaries = .
  Plugins = ../qtplugins"
)

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
		
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
        DESTINATION ${TERRALIB_DESTINATION_RUNTIME}
		COMPONENT runtime)

export(TARGETS terraview APPEND FILE ${CMAKE_BINARY_DIR}/terralib-exports.cmake)
