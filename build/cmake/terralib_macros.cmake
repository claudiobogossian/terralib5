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

IF(XGETTEXT_FOUND)
  macro(TERRALIB_REGISTER_TRANSLATION proj_name locale)
    set(_po_file "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj_name}_${locale}.po")
    set(_mo_file "${CMAKE_BINARY_DIR}/share/terralib/translations/${locale}/LC_MESSAGES/${proj_name}.mo")
    if(EXISTS ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj_name}_${locale}.po)
      add_custom_command(
        TARGET ${proj_name}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/share/terralib/translations/${locale}/LC_MESSAGES/
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_mo_file} ${_po_file}
        COMMENT "Generating translation binary file for ${proj_name}...")
    endif()
  endmacro(TERRALIB_REGISTER_TRANSLATION)

  macro(TERRALIB_CREATE_TRANSLATION proj_name keyword_s keyword_p locale directory)
    get_filename_component(_absDir ${directory} ABSOLUTE)
    set(_pot_file ${CMAKE_CURRENT_BINARY_DIR}/${proj_name}.pot)
    set(_po_file "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj_name}_${locale}.po")
    file(GLOB_RECURSE _srcs RELATIVE ${_absDir} ${_absDir}/*.cpp)
    set(encoding "UTF-8")
    if(NOT EXISTS "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj_name}_${locale}.po")
      configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/template_translation.po.in
                     ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj_name}_${locale}.po @ONLY)
    endif()
    add_custom_command(
      TARGET ${proj_name}
      PRE_BUILD
      COMMAND ${XGETTEXT_EXECUTABLE} --from-code=${encoding} --no-wrap --c++ --keyword=${keyword_s} --keyword=${keyword_p}:1,2 -o ${_pot_file} ${_srcs}
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} -U --backup=none -q --lang=${locale} ${_po_file} ${_pot_file}
      COMMENT "Generating translation source file for ${proj_name}...")
  endmacro(TERRALIB_CREATE_TRANSLATION)
ENDIF(XGETTEXT_FOUND)

