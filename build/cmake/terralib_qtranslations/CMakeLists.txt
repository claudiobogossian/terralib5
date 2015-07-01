
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
#  Description: Script for generating the application translations in qt format.
#
#  Author: Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#
MACRO(GenerateTranslation translation_dir project_name qm_files)
  set(_ts_file_name ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${project_name}_pt_br.ts)
  set(qm_file ${CMAKE_BINARY_DIR}/translations/${project_name}_pt_br.qm)

  add_custom_command(
    OUTPUT ${qm_file}
          COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/translations"
	  COMMAND ${Qt5_LUPDATE_EXECUTABLE} 
	  ARGS -I ${translation_dir} -recursive ${translation_dir} -ts "${_ts_file_name}"
	  COMMAND ${Qt5_LRELEASE_EXECUTABLE} 
	  ARGS "${_ts_file_name}" -qm "${qm_file}"
	  COMMENT "Generating ${project_name} translations"
  )
  
  list(APPEND ${qm_files} ${qm_file})
ENDMACRO(GenerateTranslation)

if(DEFINED TERRALIB_QTRANSLATION_ENABLED)

  set(_qm_files "")

  if(TERRALIB_MOD_QT_WIDGETS_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets" "terralib_mod_qt_widgets" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_APF_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af" "terralib_mod_qt_apf" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_ADO_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/ado" "terralib_mod_qt_plugins_datasource_ado" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_GDAL_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/gdal" "terralib_mod_qt_plugins_datasource_gdal" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_TERRALIB4_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/terralib4" "terralib_mod_qt_plugins_datasource_terralib4" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_OGR_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/ogr" "terralib_mod_qt_plugins_datasource_ogr" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_POSTGIS_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/pgis" "terralib_mod_qt_plugins_datasource_pgis" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WCS_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wcs" "terralib_mod_qt_plugins_datasource_wcs" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WFS_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wfs" "terralib_mod_qt_plugins_datasource_wfs" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WMS_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wms" "terralib_mod_qt_plugins_datasource_wms" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_VP_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/vp" "terralib_mod_qt_plugins_vp" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_RP_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/rp" "terralib_mod_qt_plugins_rp" _qm_files) 
  endif()

  if(TERRALIB_MOD_QT_PLUGINS_ST_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/st" "terralib_mod_qt_plugins_st" _qm_files) 
  endif()

  if(TERRALIB_MOD_VP_QT_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/vp/qt" "terralib_mod_vp_qt" _qm_files) 
  endif()

  if(TERRALIB_TERRAVIEW_ENABLED)
    GenerateTranslation("${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview" "terraview" _qm_files) 
  endif()

  add_custom_target(
    terralib_qtranslations
    COMMENT "Generating qt translation files..."
    SOURCES ${_qm_files}
  )

  install(
    FILES ${_qm_files}
    DESTINATION ${TERRALIB_DESTINATION_SHARE}/translations
    COMPONENT runtime
  )

endif()
