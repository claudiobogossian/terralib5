
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

MACRO(GenerateTranslation QM_FILE proj dir)
  file(GLOB_RECURSE h2Translate "${dir}/*.h")
  file(GLOB_RECURSE cpp2Translate "${dir}/*.c*")
  file(GLOB_RECURSE ui2Translate "${dir}/*.ui")

  set(files2Translate ${h2Translate} ${cpp2Translate} ${ui2Translate})

  qt5_create_translation(${QM_FILE} ${files2Translate} "${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${proj}_pt_br.ts")	

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${proj}_pt_br.qm
    DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib/translations
    COMPONENT runtime
  )
ENDMACRO(GenerateTranslation)

if(TERRALIB_MOD_QT_WIDGETS_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_widgets" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/widgets")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_APF_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_apf" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/af")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_ADO_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_ado" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/ado")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_GDAL_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_gdal" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/gdal")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_TERRALIB4_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_terralib4" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/terralib4")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_OGR_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_ogr" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/ogr")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_POSTGIS_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_pgis" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/pgis")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WCS_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_wcs" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wcs")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WFS_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_wfs" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wfs")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_DATASOURCE_WMS_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_datasource_wms" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/datasource/wms")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_VP_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_vp" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/vp")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_RP_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_rp" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/rp")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_QT_PLUGINS_ST_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_qt_plugins_st" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/qt/plugins/st")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_MOD_VP_QT_ENABLED)
  GenerateTranslation(QM_FILE "terralib_mod_vp_qt" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/vp/qt")
  list(APPEND qm_files ${QM_FILE})
endif()

if(TERRALIB_TERRAVIEW_ENABLED)
  GenerateTranslation(QM_FILE "terraview" "${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terraview")
  list(APPEND qm_files ${QM_FILE})
endif()

add_custom_target(
  terralib_qtranslations
  COMMENT "Generating qt translation files..."
  SOURCES ${qm_files}
)

foreach (qm ${qm_files})
  add_custom_command(
    TARGET terralib_qtranslations
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/share/terralib/translations"
    COMMAND ${CMAKE_COMMAND} -E copy ${qm} "${CMAKE_BINARY_DIR}/share/terralib/translations"
  )
endforeach()
