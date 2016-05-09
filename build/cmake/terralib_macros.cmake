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

MACRO(TeInstallPlugins plugins location)

  set(_files)

  foreach(plugin ${plugins})
    get_target_property(_loc ${plugin} LOCATION)
    list(APPEND _files ${_loc})
  endforeach()
  
  install(FILES ${_files}
           DESTINATION "${TERRALIB_BASE_DESTINATION_DIR}qtplugins/${location}"
           CONFIGURATIONS Release
           COMPONENT runtime)
  
ENDMACRO(TeInstallPlugins)


MACRO(TeInstallQt5Plugins)
  find_package(Qt5 COMPONENTS Sql Svg PrintSupport)

# Installing image plugins
  set(_plugins Qt5::QGifPlugin Qt5::QICOPlugin Qt5::QJpegPlugin Qt5::QMngPlugin Qt5::QTiffPlugin)
  TeInstallPlugins("${_plugins}" "imageformats")
  
# Installing svg plugins
  set(_plugins Qt5::QSvgPlugin Qt5::QSvgIconPlugin)
  TeInstallPlugins("${_plugins}" "iconengines")
    
# Installing sql plugins
  set(_plugins Qt5::QSQLiteDriverPlugin)
  TeInstallPlugins("${_plugins}" "sqldrivers")

# Installing printer support plugin
  if(WIN32)
    set(_plugins Qt5::QWindowsPrinterSupportPlugin)
    TeInstallPlugins("${_plugins}" "printsupport")
  endif()
  
# Installing platform plugins
  if(WIN32)
    set(_plugins Qt5::QWindowsIntegrationPlugin Qt5::QMinimalIntegrationPlugin)
    TeInstallPlugins("${_plugins}" "platforms")
  elseif(APPLE)
    set(_plugins Qt5::QCocoaIntegrationPlugin Qt5::QMinimalIntegrationPlugin)
    TeInstallPlugins("${_plugins}" "platforms")
  endif()
  
ENDMACRO(TeInstallQt5Plugins)

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

  set (_regex_exp "(${_regex_exp})?(${CMAKE_SHARED_LIBRARY_SUFFIX})$")

  set (_dest "${TERRALIB_BASE_DESTINATION_DIR}qtplugins")

if(QT4_FOUND)
  set (_plugin_dirs "imageformats;iconengines;sqldrivers")
else()
    set (_plugin_dirs "imageformats;iconengines;sqldrivers;platforms")
endif()

  foreach(_plugin_dir ${_plugin_dirs})

    install (DIRECTORY ${QT_PLUGINS_DIR}/${_plugin_dir}
             DESTINATION ${_dest}  COMPONENT runtime
             FILES_MATCHING REGEX "${_regex_exp}")
  endforeach()

ENDMACRO(TeInstallQtPlugins)

#
# Macro TeInstallTerraLibPlugins
#
# Description: Installs the required TerraLib plugins. (Used by outter projects.)
#
# param plugins List of the names of plugins to be installed.
#
# param location Path where the plugins must be installed.
#
MACRO(TeInstallTerraLibPlugins plugins location)
  set(_files "")
  set(_filesd "")
  
  foreach(plugin ${plugins})
    get_target_property(_loc ${plugin} LOCATION_RELEASE)
    list(APPEND _files ${_loc})
    
    get_target_property(_locd ${plugin} LOCATION_DEBUG)
    list(APPEND _filesd ${_locd})
  endforeach()
  
  install (FILES ${_files}
    DESTINATION ${location}
    CONFIGURATIONS Release
    COMPONENT runtime
  )  

  install (FILES ${_filesd}
    DESTINATION ${location}
    CONFIGURATIONS Debug
    COMPONENT runtime
  )  
ENDMACRO(TeInstallTerraLibPlugins)

MACRO(GetTerraLibModules _modules)
  set(allModules "${TERRALIB_MODULES}" "${TERRALIB_PLUGINS}")

  foreach(module ${allModules})
    set (_mod_name terralib_mod_${module})
    
    string(FIND ${_mod_name} python _py_ok)
        
    if(${_py_ok} GREATER 0)
      set(_mod_name _${_mod_name})
    endif()
    
    list(APPEND ${_modules} ${_mod_name})
  endforeach()
ENDMACRO(GetTerraLibModules)
