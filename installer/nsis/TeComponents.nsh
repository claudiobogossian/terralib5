; ---------------------------------------------------------------------------
; TeComponents
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Contains macros that make the insertion of all TerraLib modules in the
; installer separately, besides the configuration files and modules
; description that appears in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_COMPONENTS
!define TE_COMPONENTS

!macro TE_INCLUDE_CONFIG_FILES
  Section
    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}\conf"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\conf\*.*"
  SectionEnd
!macroend

!macro TE_GET_COMPONENTS_DESCRIPTION

  !define TE_ANNOTATIONTEXT_DESCRIPTION "TerraLib Annotation Text Module"
  !define TE_COLOR_DESCRIPTION "TerraLib Color Module"
  !define TE_COMMON_DESCRIPTION "TerraLib Common Runtime Module"
  !define TE_DATAACCESS_DESCRIPTION "TerraLib Data Access Module"
  !define TE_DATATYPE_DESCRIPTION "TerraLib Data Type Module"
  !define TE_DYGEO_DESCRIPTION "TerraLib DyGeo (Dynamic Geospatial) Module"
  !define TE_FILTER_DESCRIPTION "TerraLib OGC Filter Encoding Module"
  !define TE_GDAL_DESCRIPTION "TerraLib GDAL Data Access Driver"
  !define TE_GEOMETRY_DESCRIPTION "TerraLib Vector Geometry Module"
  !define TE_GML_DESCRIPTION "TerraLib OGC Geography Markup Language Module"
  !define TE_MAPTOOLS_DESCRIPTION "TerraLib Map Tools Module"
  !define TE_MEMORY_DESCRIPTION "TerraLib Memory Data Access Driver"
  !define TE_OGR_DESCRIPTION "TerraLib OGR Data Access Driver"
  !define TE_PLUGIN_DESCRIPTION "TerraLib Plugin Module"
  !define TE_POSTGIS_DESCRIPTION "TerraLib PostGIS Data Access Driver"
  !define TE_QTWIDGETS_DESCRIPTION "TerraLib Qt Widgets Module"
  !define TE_RASTER_DESCRIPTION "TerraLib Raster Module"
  !define TE_SAM_DESCRIPTION "TerraLib Spatial Access Methods Module"
  !define TE_SRS_DESCRIPTION "TerraLib Spatial Reference System Module"
  !define TE_SYMBOLOGY_DESCRIPTION "TerraLib OGC Symbology Encoding Module"
  !define TE_XLINK_DESCRIPTION "TerraLib W3C XLink Module"
  !define TE_XML_DESCRIPTION "TerraLib XML Module"

!macroend

;------------------------------------------------------------------------------------------
;---------------------------------------DEBUG----------------------------------------------
;------------------------------------------------------------------------------------------
!macro TE_INSERT_ANNOTATIONTEXT_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_annotationtext_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_annotationtext_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_COLOR_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_color_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_color_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_COMMON_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_common_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_common_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DATAACCESS_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_dataaccess_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_dataaccess_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DATATYPE_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_datatype_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_datatype_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DYGEO_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_dygeo_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_dygeo_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_FILTER_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_filter_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_filter_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GDAL_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_gdal_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_gdal_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GEOMETRY_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_geometry_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_geometry_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GML_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_gml_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_gml_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_MAPTOOLS_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_maptools_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_maptools_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_MEMORY_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_memory_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_memory_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_OGR_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_ogr_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_ogr_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_PLUGIN_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_plugin_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_plugin_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_POSTGIS_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_postgis_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_postgis_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_QTWIDGETS_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_qtwidgets_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_qtwidgets_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_RASTER_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_raster_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_raster_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SAM_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_srs_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_srs_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SRS_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_srs_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_srs_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SYMBOLOGY_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_se_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_se_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_XLINK_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_xlink_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_xlink_d.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_XML_DEBUG

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_xml_d.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_xml_d.${LIB_EXTENSION}"

!macroend
;------------------------------------------------------------------------------------------
;------------------------------------END DEBUG---------------------------------------------
;------------------------------------------------------------------------------------------



;------------------------------------------------------------------------------------------
;------------------------------------RELEASE-----------------------------------------------
;------------------------------------------------------------------------------------------
!macro TE_INSERT_ANNOTATIONTEXT_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_annotationtext.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_annotationtext.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_COLOR_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_color.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_color.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_COMMON_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_common.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_common.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DATAACCESS_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_dataaccess.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_dataaccess.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DATATYPE_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_datatype.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_datatype.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_DYGEO_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_dygeo.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_dygeo.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_FILTER_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_filter.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_filter.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GDAL_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_gdal.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_gdal.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GEOMETRY_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_geometry.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_geometry.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_GML_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_gml.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_gml.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_MAPTOOLS_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_maptools.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_maptools.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_MEMORY_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_memory.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_memory.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_OGR_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_ogr.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_ogr.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_PLUGIN_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_plugin.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_plugin.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_POSTGIS_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_postgis.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_postgis.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_QTWIDGETS_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_qtwidgets.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_qtwidgets.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_RASTER_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_raster.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_raster.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SAM_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_srs.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_srs.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SRS_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_srs.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_srs.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_SYMBOLOGY_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_se.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_se.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_XLINK_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_xlink.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_xlink.${LIB_EXTENSION}"

!macroend

!macro TE_INSERT_XML_RELEASE

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\terralib_xml.dll"
    SetOutPath "$INSTDIR\lib${PLATFORM}\${GENERATOR}"
    File /nonfatal "${TERRALIB_DIR}\lib${PLATFORM}\${GENERATOR}\terralib_xml.${LIB_EXTENSION}"

!macroend
;------------------------------------------------------------------------------------------
;-----------------------------------END RELEASE--------------------------------------------
;------------------------------------------------------------------------------------------

!endif ; TE_COMPONENTS