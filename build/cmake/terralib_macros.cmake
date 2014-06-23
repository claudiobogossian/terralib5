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
#  Description: Auxiliary macros and functions.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

#
# Macro installQtPlugins
#
# Description: Installs the required Qt plugins.
#
# param plugs List of the names of plugins to be installed.
#
MACRO(TeInstallQtPlugins plgs)

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

  if(APPLE)
    set (_dest terraview.app/Contents/qtplugins)
  else()
    set (_dest qtplugins)
  endif()

  set (_plugin_dirs "imageformats;iconengines;sqldrivers;platforms")

  foreach(_plugin_dir ${_plugin_dirs})

    install (DIRECTORY ${QT_PLUGINS_DIR}/${_plugin_dir}
             DESTINATION ${_dest}  COMPONENT runtime
             FILES_MATCHING REGEX "${_regex_exp}")
  endforeach()

ENDMACRO(TeInstallQtPlugins)