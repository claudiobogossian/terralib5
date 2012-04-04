; ---------------------------------------------------------------------------
; TeSectionsUtils
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib installer sections manipulation. This sections appear
; in the components page of the installers and contains the installations
; options where the user can unselect a specific module or documentation
; sections for example.
;
; ---------------------------------------------------------------------------

!ifndef TE_SECTIONS
!define TE_SECTIONS

!include "TeComponents.nsh"
!include "TeDependencies.nsh"
!include "TeDocumentation.nsh"
!include "TeExamples.nsh"
!include "TeHeaderFiles.nsh"
!include "TeUnittest.nsh"
!include "TeTools.nsh"

!macro TE_INSERT_COMPONENTS_SUBSECTION
  SubSection "Components" SUBSEC_COMPONENTS
  !ifdef MSVC_ON
    !insertmacro TE_INSERT_MSVC_SECTION
  !endif

  !ifdef MINGW_ON
    !insertmacro TE_INSERT_MINGW_SECTION
  !endif
  SubSectionEnd
!macroend

!macro TE_INSERT_PLUGINS_SECTION
  Section "Plugins" SEC_PLUGINS
    SectionIn 1 2 3 4 5 6 7
  SectionEnd
!macroend

!macro TE_INSERT_DOCUMENTATION_SECTION
  Section "Documentation" SEC_DOCUMENTATION
    SectionIn 1 2 3 4 5 6 7
    !insertmacro TE_INCLUDE_DOCUMENTATION_FILES
    !insertmacro TE_CREATE_DOCUMENTATION_SHORTCUTS
  SectionEnd
!macroend

!macro TE_INSERT_EXAMPLES_SECTION
  Section "Examples" SEC_EXAMPLES
    SectionIn 1 2 3 4 5 6 7
    !insertmacro TE_INCLUDE_EXAMPLES_FILES
  SectionEnd
!macroend

!macro TE_INSERT_UNITTEST_SECTION
  Section "Unit Tests" SEC_UNITTEXT
    SectionIn 1 2 3 4 5 6 7
    !insertmacro TE_INCLUDE_UNITTEST_FILES
  SectionEnd
!macroend

!macro TE_INSERT_DEPENDENCIES_SUBSECTION
  SubSection "Dependencies" SUBSEC_DEPENDENCIES
    !insertmacro TE_INSERT_DEP_DEBUG_SECTION
    !insertmacro TE_INSERT_DEP_RELEASE_SECTION
  SubSectionEnd
!macroend

!macro TE_INSERT_TOOLS_SECTION
  Section "Tools" SEC_TOOLS
    SectionIn 1 2 3 4 5 6 7
    !insertmacro TE_INSERT_TOOLS
  SectionEnd
!macroend

;-----------------------------------------------------------------------------

; Components SubSection

;--------------
; Msvc

!macro TE_INSERT_MSVC_SECTION
  !ifndef HEADER_FILES_INSTALLED
    !insertmacro TE_INCLUDE_HEADER_FILES
    !define HEADER_FILES_INSTALLED
  !endif

  !insertmacro TE_MSVC_CONFIG
  SubSection "${GENERATOR}"
    !insertmacro TE_INSERT_MSVC_DEBUG_SECTION
    !insertmacro TE_INSERT_MSVC_RELEASE_SECTION
  SubSectionEnd
!macroend

!macro TE_INSERT_MSVC_DEBUG_SECTION
  !insertmacro TE_MSVC_CONFIG
  SubSection "Debug" SEC_MSVC_DEBUG

    !insertmacro TE_MSVC_DEBUG_ANNOTATIONTEXT_SECTION
    !insertmacro TE_MSVC_DEBUG_COLOR_SECTION
    !insertmacro TE_MSVC_DEBUG_COMMON_SECTION
    !insertmacro TE_MSVC_DEBUG_DATAACCESS_SECTION
    !insertmacro TE_MSVC_DEBUG_DATATYPE_SECTION
    !insertmacro TE_MSVC_DEBUG_DYGEO_SECTION
    !insertmacro TE_MSVC_DEBUG_FILTER_SECTION
    !insertmacro TE_MSVC_DEBUG_GDAL_SECTION
    !insertmacro TE_MSVC_DEBUG_GEOMETRY_SECTION
    !insertmacro TE_MSVC_DEBUG_GML_SECTION
    !insertmacro TE_MSVC_DEBUG_MAPTOOLS_SECTION
    !insertmacro TE_MSVC_DEBUG_MEMORY_SECTION
    !insertmacro TE_MSVC_DEBUG_OGR_SECTION
    !insertmacro TE_MSVC_DEBUG_PLUGIN_SECTION
    !insertmacro TE_MSVC_DEBUG_POSTGIS_SECTION
    !insertmacro TE_MSVC_DEBUG_QTWIDGETS_SECTION
    !insertmacro TE_MSVC_DEBUG_RASTER_SECTION
    !insertmacro TE_MSVC_DEBUG_SAM_SECTION
    !insertmacro TE_MSVC_DEBUG_SRS_SECTION
    !insertmacro TE_MSVC_DEBUG_SYMBOLOGY_SECTION
    !insertmacro TE_MSVC_DEBUG_XLINK_SECTION
    !insertmacro TE_MSVC_DEBUG_XML_SECTION
  SubSectionEnd
!macroend

!macro TE_INSERT_MSVC_RELEASE_SECTION
  !insertmacro TE_MSVC_CONFIG
  SubSection "Release" SEC_MSVC_RELEASE

    !insertmacro TE_MSVC_RELEASE_ANNOTATIONTEXT_SECTION
    !insertmacro TE_MSVC_RELEASE_COLOR_SECTION
    !insertmacro TE_MSVC_RELEASE_COMMON_SECTION
    !insertmacro TE_MSVC_RELEASE_DATAACCESS_SECTION
    !insertmacro TE_MSVC_RELEASE_DATATYPE_SECTION
	!insertmacro TE_MSVC_RELEASE_DYGEO_SECTION
    !insertmacro TE_MSVC_RELEASE_FILTER_SECTION
    !insertmacro TE_MSVC_RELEASE_GDAL_SECTION
    !insertmacro TE_MSVC_RELEASE_GEOMETRY_SECTION
    !insertmacro TE_MSVC_RELEASE_GML_SECTION
    !insertmacro TE_MSVC_RELEASE_MAPTOOLS_SECTION
    !insertmacro TE_MSVC_RELEASE_MEMORY_SECTION
    !insertmacro TE_MSVC_RELEASE_OGR_SECTION
    !insertmacro TE_MSVC_RELEASE_PLUGIN_SECTION
    !insertmacro TE_MSVC_RELEASE_POSTGIS_SECTION
    !insertmacro TE_MSVC_RELEASE_QTWIDGETS_SECTION
    !insertmacro TE_MSVC_RELEASE_RASTER_SECTION
    !insertmacro TE_MSVC_RELEASE_SAM_SECTION
    !insertmacro TE_MSVC_RELEASE_SRS_SECTION
    !insertmacro TE_MSVC_RELEASE_SYMBOLOGY_SECTION
    !insertmacro TE_MSVC_RELEASE_XLINK_SECTION
	!insertmacro TE_MSVC_RELEASE_XML_SECTION
  SubSectionEnd
!macroend

;--------------
; MinGW

!macro TE_INSERT_MINGW_SECTION
  !ifndef HEADER_FILES_INSTALLED
    !insertmacro TE_INCLUDE_HEADER_FILES
    !define HEADER_FILES_INSTALLED
  !endif

  !insertmacro TE_MINGW_CONFIG
  SubSection "${GENERATOR}"
    !insertmacro TE_INSERT_MINGW_DEBUG_SECTION
    !insertmacro TE_INSERT_MINGW_RELEASE_SECTION
  SubsectionEnd
!macroend

!macro TE_INSERT_MINGW_DEBUG_SECTION
  !insertmacro TE_MINGW_CONFIG
  SubSection "Debug" SEC_MINGW_DEBUG

    !insertmacro TE_MINGW_DEBUG_ANNOTATIONTEXT_SECTION
    !insertmacro TE_MINGW_DEBUG_COLOR_SECTION
    !insertmacro TE_MINGW_DEBUG_COMMON_SECTION
    !insertmacro TE_MINGW_DEBUG_DATAACCESS_SECTION
    !insertmacro TE_MINGW_DEBUG_DATATYPE_SECTION
	!insertmacro TE_MINGW_DEBUG_DYGEO_SECTION
    !insertmacro TE_MINGW_DEBUG_FILTER_SECTION
    !insertmacro TE_MINGW_DEBUG_GDAL_SECTION
    !insertmacro TE_MINGW_DEBUG_GEOMETRY_SECTION
    !insertmacro TE_MINGW_DEBUG_GML_SECTION
    !insertmacro TE_MINGW_DEBUG_MAPTOOLS_SECTION
    !insertmacro TE_MINGW_DEBUG_MEMORY_SECTION
    !insertmacro TE_MINGW_DEBUG_OGR_SECTION
    !insertmacro TE_MINGW_DEBUG_PLUGIN_SECTION
    !insertmacro TE_MINGW_DEBUG_POSTGIS_SECTION
    !insertmacro TE_MINGW_DEBUG_QTWIDGETS_SECTION
    !insertmacro TE_MINGW_DEBUG_RASTER_SECTION
    !insertmacro TE_MINGW_DEBUG_SAM_SECTION
    !insertmacro TE_MINGW_DEBUG_SRS_SECTION
    !insertmacro TE_MINGW_DEBUG_SYMBOLOGY_SECTION
    !insertmacro TE_MINGW_DEBUG_XLINK_SECTION
	!insertmacro TE_MINGW_DEBUG_XML_SECTION
  SubSectionEnd
!macroend

!macro TE_INSERT_MINGW_RELEASE_SECTION
  !insertmacro TE_MINGW_CONFIG
  SubSection "Release" SEC_MINGW_RELEASE

    !insertmacro TE_MINGW_RELEASE_ANNOTATIONTEXT_SECTION
    !insertmacro TE_MINGW_RELEASE_COLOR_SECTION
    !insertmacro TE_MINGW_RELEASE_COMMON_SECTION
    !insertmacro TE_MINGW_RELEASE_DATAACCESS_SECTION
    !insertmacro TE_MINGW_RELEASE_DATATYPE_SECTION
	!insertmacro TE_MINGW_RELEASE_DYGEO_SECTION
    !insertmacro TE_MINGW_RELEASE_FILTER_SECTION
    !insertmacro TE_MINGW_RELEASE_GDAL_SECTION
    !insertmacro TE_MINGW_RELEASE_GEOMETRY_SECTION
    !insertmacro TE_MINGW_RELEASE_GML_SECTION
    !insertmacro TE_MINGW_RELEASE_MAPTOOLS_SECTION
    !insertmacro TE_MINGW_RELEASE_MEMORY_SECTION
    !insertmacro TE_MINGW_RELEASE_OGR_SECTION
    !insertmacro TE_MINGW_RELEASE_PLUGIN_SECTION
    !insertmacro TE_MINGW_RELEASE_POSTGIS_SECTION
    !insertmacro TE_MINGW_RELEASE_QTWIDGETS_SECTION
    !insertmacro TE_MINGW_RELEASE_RASTER_SECTION
    !insertmacro TE_MINGW_RELEASE_SAM_SECTION
    !insertmacro TE_MINGW_RELEASE_SRS_SECTION
    !insertmacro TE_MINGW_RELEASE_SYMBOLOGY_SECTION
    !insertmacro TE_MINGW_RELEASE_XLINK_SECTION
	!insertmacro TE_MINGW_RELEASE_XML_SECTION
  SubSectionEnd
!macroend

;-----------------------------------------------------------------------------

; Dependencies SubSection

!macro TE_INSERT_DEP_DEBUG_SECTION
  Section "Debug" SEC_DEP_DEBUG
    SectionIn 1 2 4 6
    !insertmacro TE_INSERT_DEPENDENCIES_DEBUG_${PLATFORM}
      
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DEPENDENCIES_DEBUG_${PLATFORM}
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_INSERT_DEP_RELEASE_SECTION
  Section "Release" SEC_DEP_RELEASE
    SectionIn 1 3 5 7
    !insertmacro TE_INSERT_DEPENDENCIES_RELEASE_${PLATFORM}
      
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DEPENDENCIES_RELEASE_${PLATFORM}
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

;-----------------------------------------------------------------------------

; Components Sections

;-----------------------------------------
; Msvc

;-------------
; Debug

!macro TE_MSVC_DEBUG_ANNOTATIONTEXT_SECTION
  Section "AnnotationText" SEC_MSVC_DEBUG_ANNOTATIONTEXT
    SectionIn 1 2 4
    !insertmacro TE_INSERT_ANNOTATIONTEXT_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_ANNOTATIONTEXT_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_COLOR_SECTION
  Section "Color" SEC_MSVC_DEBUG_COLOR
    SectionIn 1 2 4
    !insertmacro TE_INSERT_COLOR_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COLOR_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_COMMON_SECTION
  Section "Common" SEC_MSVC_DEBUG_COMMON
    SectionIn 1 2 4
    !insertmacro TE_INSERT_COMMON_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COMMON_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_DATAACCESS_SECTION
  Section "DataAccess" SEC_MSVC_DEBUG_DATAACCESS
    SectionIn 1 2 4
    !insertmacro TE_INSERT_DATAACCESS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATAACCESS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_DATATYPE_SECTION
  Section "DataType" SEC_MSVC_DEBUG_DATATYPE
    SectionIn 1 2 4
    !insertmacro TE_INSERT_DATATYPE_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATATYPE_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_DYGEO_SECTION
  Section "DyGeo" SEC_MSVC_DEBUG_DYGEO
    SectionIn 1 2 4
    !insertmacro TE_INSERT_DYGEO_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DYGEO_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_FILTER_SECTION
  Section "Filter" SEC_MSVC_DEBUG_FILTER
    SectionIn 1 2 4
    !insertmacro TE_INSERT_FILTER_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_FILTER_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_GDAL_SECTION
  Section "GDAL" SEC_MSVC_DEBUG_GDAL
    SectionIn 1 2 4
    !insertmacro TE_INSERT_GDAL_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GDAL_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_GEOMETRY_SECTION
  Section "Geometry" SEC_MSVC_DEBUG_GEOMETRY
    SectionIn 1 2 4
    !insertmacro TE_INSERT_GEOMETRY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GEOMETRY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_GML_SECTION
  Section "GML" SEC_MSVC_DEBUG_GML
    SectionIn 1 2 4
    !insertmacro TE_INSERT_GML_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GML_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_MAPTOOLS_SECTION
  Section "MapTools" SEC_MSVC_DEBUG_MAPTOOLS
    SectionIn 1 2 4
    !insertmacro TE_INSERT_MAPTOOLS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MAPTOOLS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_MEMORY_SECTION
  Section "Memory" SEC_MSVC_DEBUG_MEMORY
    SectionIn 1 2 4
    !insertmacro TE_INSERT_MEMORY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MEMORY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_OGR_SECTION
  Section "OGR" SEC_MSVC_DEBUG_OGR
    SectionIn 1 2 4
    !insertmacro TE_INSERT_OGR_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_OGR_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_PLUGIN_SECTION
  Section "Plugin" SEC_MSVC_DEBUG_PLUGIN
    SectionIn 1 2 4
    !insertmacro TE_INSERT_PLUGIN_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_PLUGIN_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_POSTGIS_SECTION
  Section "PostGIS" SEC_MSVC_DEBUG_POSTGIS
    SectionIn 1 2 4
    !insertmacro TE_INSERT_POSTGIS_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_POSTGIS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_QTWIDGETS_SECTION
  Section "QtWidgets" SEC_MSVC_DEBUG_QTWIDGETS
    SectionIn 1 2 4
    !insertmacro TE_INSERT_QTWIDGETS_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_QTWIDGETS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_RASTER_SECTION
  Section "Raster" SEC_MSVC_DEBUG_RASTER
    SectionIn 1 2 4
    !insertmacro TE_INSERT_RASTER_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_RASTER_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_SAM_SECTION
  Section "Sam" SEC_MSVC_DEBUG_SAM
    SectionIn 1 2 4
    !insertmacro TE_INSERT_SAM_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SAM_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_SRS_SECTION
  Section "SRS" SEC_MSVC_DEBUG_SRS
    SectionIn 1 2 4
    !insertmacro TE_INSERT_SRS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SRS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_SYMBOLOGY_SECTION
  Section "Symbology" SEC_MSVC_DEBUG_SYMBOLOGY
    SectionIn 1 2 4
    !insertmacro TE_INSERT_SYMBOLOGY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SYMBOLOGY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_XLINK_SECTION
  Section "XLink" SEC_MSVC_DEBUG_XLINK
    SectionIn 1 2 4
    !insertmacro TE_INSERT_XLINK_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XLINK_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_DEBUG_XML_SECTION
  Section "XML" SEC_MSVC_DEBUG_XML
    SectionIn 1 2 4
    !insertmacro TE_INSERT_XML_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XML_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

;-------------
; Release

!macro TE_MSVC_RELEASE_ANNOTATIONTEXT_SECTION
  Section "AnnotationText" SEC_MSVC_RELEASE_ANNOTATIONTEXT
    SectionIn 1 3 5
    !insertmacro TE_INSERT_ANNOTATIONTEXT_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_ANNOTATIONTEXT_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_COLOR_SECTION
  Section "Color" SEC_MSVC_RELEASE_COLOR
    SectionIn 1 3 5
    !insertmacro TE_INSERT_COLOR_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COLOR_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_COMMON_SECTION
  Section "Common" SEC_MSVC_RELEASE_COMMON
    SectionIn 1 3 5
    !insertmacro TE_INSERT_COMMON_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COMMON_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_DATAACCESS_SECTION
  Section "DataAccess" SEC_MSVC_RELEASE_DATAACCESS
    SectionIn 1 3 5
    !insertmacro TE_INSERT_DATAACCESS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATAACCESS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
 SectionEnd
!macroend

!macro TE_MSVC_RELEASE_DATATYPE_SECTION
  Section "DataType" SEC_MSVC_RELEASE_DATATYPE
    SectionIn 1 3 5
    !insertmacro TE_INSERT_DATATYPE_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATATYPE_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_DYGEO_SECTION
  Section "DyGeo" SEC_MSVC_RELEASE_DYGEO
    SectionIn 1 3 5
    !insertmacro TE_INSERT_DYGEO_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DYGEO_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_FILTER_SECTION
  Section "Filter" SEC_MSVC_RELEASE_FILTER
    SectionIn 1 3 5
    !insertmacro TE_INSERT_FILTER_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_FILTER_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_GDAL_SECTION
  Section "GDAL" SEC_MSVC_RELEASE_GDAL
    SectionIn 1 3 5
    !insertmacro TE_INSERT_GDAL_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GDAL_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_GEOMETRY_SECTION
  Section "Geometry" SEC_MSVC_RELEASE_GEOMETRY
    SectionIn 1 3 5
    !insertmacro TE_INSERT_GEOMETRY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GEOMETRY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_GML_SECTION
  Section "GML" SEC_MSVC_RELEASE_GML
    SectionIn 1 3 5
    !insertmacro TE_INSERT_GML_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GML_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_MAPTOOLS_SECTION
  Section "MapTools" SEC_MSVC_RELEASE_MAPTOOLS
    SectionIn 1 3 5
    !insertmacro TE_INSERT_MAPTOOLS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MAPTOOLS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_MEMORY_SECTION
  Section "Memory" SEC_MSVC_RELEASE_MEMORY
    SectionIn 1 3 5
    !insertmacro TE_INSERT_MEMORY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MEMORY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_OGR_SECTION
  Section "OGR" SEC_MSVC_RELEASE_OGR
    SectionIn 1 3 5
    !insertmacro TE_INSERT_OGR_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_OGR_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_PLUGIN_SECTION
  Section "Plugin" SEC_MSVC_RELEASE_PLUGIN
    SectionIn 1 3 5
    !insertmacro TE_INSERT_PLUGIN_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_PLUGIN_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_POSTGIS_SECTION
  Section "PostGIS" SEC_MSVC_RELEASE_POSTGIS
    SectionIn 1 3 5
    !insertmacro TE_INSERT_POSTGIS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_POSTGIS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_QTWIDGETS_SECTION
  Section "QtWidgets" SEC_MSVC_RELEASE_QTWIDGETS
    SectionIn 1 3 5
    !insertmacro TE_INSERT_QTWIDGETS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_QTWIDGETS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_RASTER_SECTION
  Section "Raster" SEC_MSVC_RELEASE_RASTER
    SectionIn 1 3 5
    !insertmacro TE_INSERT_RASTER_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_RASTER_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_SAM_SECTION
  Section "Sam" SEC_MSVC_RELEASE_SAM
    SectionIn 1 3 5
    !insertmacro TE_INSERT_SAM_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SAM_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_SRS_SECTION
  Section "SRS" SEC_MSVC_RELEASE_SRS
    SectionIn 1 3 5
    !insertmacro TE_INSERT_SRS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SRS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_SYMBOLOGY_SECTION
  Section "Symbology" SEC_MSVC_RELEASE_SYMBOLOGY
    SectionIn 1 3 5
    !insertmacro TE_INSERT_SYMBOLOGY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SYMBOLOGY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_XLINK_SECTION
  Section "XLink" SEC_MSVC_RELEASE_XLINK
    SectionIn 1 3 5
    !insertmacro TE_INSERT_XLINK_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XLINK_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MSVC_RELEASE_XML_SECTION
  Section "XML" SEC_MSVC_RELEASE_XML
    SectionIn 1 3 5
    !insertmacro TE_INSERT_XML_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XML_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

;-----------------------------------------
; MinGW

;-------------
; Debug

!macro TE_MINGW_DEBUG_ANNOTATIONTEXT_SECTION
  Section "AnnotationText" SEC_MINGW_DEBUG_ANNOTATIONTEXT
    SectionIn 1 2 6
    !insertmacro TE_INSERT_ANNOTATIONTEXT_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_ANNOTATIONTEXT_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_COLOR_SECTION
  Section "Color" SEC_MINGW_DEBUG_COLOR
    SectionIn 1 2 6
    !insertmacro TE_INSERT_COLOR_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COLOR_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_COMMON_SECTION
  Section "Common" SEC_MINGW_DEBUG_COMMON
    SectionIn 1 2 6
    !insertmacro TE_INSERT_COMMON_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COMMON_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_DATAACCESS_SECTION
  Section "DataAccess" SEC_MINGW_DEBUG_DATAACCESS
    SectionIn 1 2 6
    !insertmacro TE_INSERT_DATAACCESS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATAACCESS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_DATATYPE_SECTION
  Section "DataType" SEC_MINGW_DEBUG_DATATYPE
    SectionIn 1 2 6
    !insertmacro TE_INSERT_DATATYPE_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATATYPE_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_DYGEO_SECTION
  Section "DyGeo" SEC_MINGW_DEBUG_DYGEO
    SectionIn 1 2 6
    !insertmacro TE_INSERT_DYGEO_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DYGEO_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_FILTER_SECTION
  Section "Filter" SEC_MINGW_DEBUG_FILTER
    SectionIn 1 2 6
    !insertmacro TE_INSERT_FILTER_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_FILTER_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_GDAL_SECTION
  Section "GDAL" SEC_MINGW_DEBUG_GDAL
    SectionIn 1 2 6
    !insertmacro TE_INSERT_GDAL_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GDAL_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_GEOMETRY_SECTION
  Section "Geometry" SEC_MINGW_DEBUG_GEOMETRY
    SectionIn 1 2 6
    !insertmacro TE_INSERT_GEOMETRY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GEOMETRY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_GML_SECTION
  Section "GML" SEC_MINGW_DEBUG_GML
    SectionIn 1 2 6
    !insertmacro TE_INSERT_GML_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GML_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_MAPTOOLS_SECTION
  Section "MapTools" SEC_MINGW_DEBUG_MAPTOOLS
    SectionIn 1 2 6
    !insertmacro TE_INSERT_MAPTOOLS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MAPTOOLS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_MEMORY_SECTION
  Section "Memory" SEC_MINGW_DEBUG_MEMORY
    SectionIn 1 2 6
    !insertmacro TE_INSERT_MEMORY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MEMORY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_OGR_SECTION
  Section "OGR" SEC_MINGW_DEBUG_OGR
    SectionIn 1 2 6
    !insertmacro TE_INSERT_OGR_DEBUG
	
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_OGR_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_PLUGIN_SECTION
  Section "Plugin" SEC_MINGW_DEBUG_PLUGIN
    SectionIn 1 2 6
    !insertmacro TE_INSERT_PLUGIN_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_PLUGIN_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_POSTGIS_SECTION
  Section "PostGIS" SEC_MINGW_DEBUG_POSTGIS
    SectionIn 1 2 6
    !insertmacro TE_INSERT_POSTGIS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_POSTGIS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_QTWIDGETS_SECTION
  Section "QtWidgets" SEC_MINGW_DEBUG_QTWIDGETS
    SectionIn 1 2 6
    !insertmacro TE_INSERT_QTWIDGETS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_QTWIDGETS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_RASTER_SECTION
  Section "Raster" SEC_MINGW_DEBUG_RASTER
    SectionIn 1 2 6
    !insertmacro TE_INSERT_RASTER_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_RASTER_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_SAM_SECTION
  Section "Sam" SEC_MINGW_DEBUG_SAM
    SectionIn 1 2 6
    !insertmacro TE_INSERT_SAM_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SAM_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_SRS_SECTION
  Section "SRS" SEC_MINGW_DEBUG_SRS
    SectionIn 1 2 6
    !insertmacro TE_INSERT_SRS_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SRS_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_SYMBOLOGY_SECTION
  Section "Symbology" SEC_MINGW_DEBUG_SYMBOLOGY
    SectionIn 1 2 6
    !insertmacro TE_INSERT_SYMBOLOGY_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SYMBOLOGY_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_XLINK_SECTION
  Section "XLink" SEC_MINGW_DEBUG_XLINK
    SectionIn 1 2 6
    !insertmacro TE_INSERT_XLINK_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XLINK_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_DEBUG_XML_SECTION
  Section "XML" SEC_MINGW_DEBUG_XML
    SectionIn 1 2 6
    !insertmacro TE_INSERT_XML_DEBUG
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XML_DEBUG
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

;-------------
; Release

!macro TE_MINGW_RELEASE_ANNOTATIONTEXT_SECTION
  Section "AnnotationText" SEC_MINGW_RELEASE_ANNOTATIONTEXT
    SectionIn 1 3 7
    !insertmacro TE_INSERT_ANNOTATIONTEXT_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_ANNOTATIONTEXT_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_COLOR_SECTION
  Section "Color" SEC_MINGW_RELEASE_COLOR
    SectionIn 1 3 7
    !insertmacro TE_INSERT_COLOR_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COLOR_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_COMMON_SECTION
  Section "Common" SEC_MINGW_RELEASE_COMMON
    SectionIn 1 3 7
    !insertmacro TE_INSERT_COMMON_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_COMMON_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_DATAACCESS_SECTION
  Section "DataAccess" SEC_MINGW_RELEASE_DATAACCESS
    SectionIn 1 3 7
    !insertmacro TE_INSERT_DATAACCESS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATAACCESS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
 SectionEnd
!macroend

!macro TE_MINGW_RELEASE_DATATYPE_SECTION
  Section "DataType" SEC_MINGW_RELEASE_DATATYPE
    SectionIn 1 3 7
    !insertmacro TE_INSERT_DATATYPE_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DATATYPE_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_DYGEO_SECTION
  Section "DyGeo" SEC_MINGW_RELEASE_DYGEO
    SectionIn 1 3 7
    !insertmacro TE_INSERT_DYGEO_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_DYGEO_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_FILTER_SECTION
  Section "Filter" SEC_MINGW_RELEASE_FILTER
    SectionIn 1 3 7
    !insertmacro TE_INSERT_FILTER_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_FILTER_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_GDAL_SECTION
  Section "GDAL" SEC_MINGW_RELEASE_GDAL
    SectionIn 1 3 7
    !insertmacro TE_INSERT_GDAL_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GDAL_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_GEOMETRY_SECTION
  Section "Geometry" SEC_MINGW_RELEASE_GEOMETRY
    SectionIn 1 3 7
    !insertmacro TE_INSERT_GEOMETRY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GEOMETRY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_GML_SECTION
  Section "GML" SEC_MINGW_RELEASE_GML
    SectionIn 1 3 7
    !insertmacro TE_INSERT_GML_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_GML_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_MAPTOOLS_SECTION
  Section "MapTools" SEC_MINGW_RELEASE_MAPTOOLS
    SectionIn 1 3 7
    !insertmacro TE_INSERT_MAPTOOLS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MAPTOOLS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_MEMORY_SECTION
  Section "Memory" SEC_MINGW_RELEASE_MEMORY
    SectionIn 1 3 7
    !insertmacro TE_INSERT_MEMORY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_MEMORY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_OGR_SECTION
  Section "OGR" SEC_MINGW_RELEASE_OGR
    SectionIn 1 3 7
    !insertmacro TE_INSERT_OGR_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_OGR_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_PLUGIN_SECTION
  Section "Plugin" SEC_MINGW_RELEASE_PLUGIN
    SectionIn 1 3 7
    !insertmacro TE_INSERT_PLUGIN_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_PLUGIN_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_POSTGIS_SECTION
  Section "PostGIS" SEC_MINGW_RELEASE_POSTGIS
    SectionIn 1 3 7
    !insertmacro TE_INSERT_POSTGIS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_POSTGIS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_QTWIDGETS_SECTION
  Section "QtWidgets" SEC_MINGW_RELEASE_QTWIDGETS
    SectionIn 1 3 7
    !insertmacro TE_INSERT_QTWIDGETS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_QTWIDGETS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_RASTER_SECTION
  Section "Raster" SEC_MINGW_RELEASE_RASTER
    SectionIn 1 3 7
    !insertmacro TE_INSERT_RASTER_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_RASTER_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_SAM_SECTION
  Section "Sam" SEC_MINGW_RELEASE_SAM
    SectionIn 1 3 7
    !insertmacro TE_INSERT_SAM_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SAM_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_SRS_SECTION
  Section "SRS" SEC_MINGW_RELEASE_SRS
    SectionIn 1 3 7
    !insertmacro TE_INSERT_SRS_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SRS_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_SYMBOLOGY_SECTION
  Section "Symbology" SEC_MINGW_RELEASE_SYMBOLOGY
    SectionIn 1 3 7
    !insertmacro TE_INSERT_SYMBOLOGY_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_SYMBOLOGY_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_XLINK_SECTION
  Section "XLink" SEC_MINGW_RELEASE_XLINK
    SectionIn 1 3 7
    !insertmacro TE_INSERT_XLINK_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XLINK_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend

!macro TE_MINGW_RELEASE_XML_SECTION
  Section "XML" SEC_MINGW_RELEASE_XML
    SectionIn 1 3 7
    !insertmacro TE_INSERT_XML_RELEASE
    
    !ifdef x86-and-x64
      !undef PLATFORM
      !define PLATFORM "64"
      !insertmacro TE_INSERT_XML_RELEASE
      !undef PLATFORM
      !define PLATFORM "32"
    !endif
  SectionEnd
!macroend


!endif ; TE_SECTIONSUTILS