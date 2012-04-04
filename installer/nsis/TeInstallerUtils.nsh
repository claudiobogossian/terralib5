; ---------------------------------------------------------------------------
; TeInstallerUtils
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains some installer configurations, such as functions that
; execute when the intaller starts (.onInit), star menu shortcuts, system
; registry end other configurations.
;
; ---------------------------------------------------------------------------

!ifndef TE_INSTALLERTUTILS
!define TE_INSTALLERTUTILS

!macro TE_INSERT_INIT_FUNCTIONS
  Function .onInit
    !insertmacro MUI_LANGDLL_DISPLAY
    SetDetailsPrint both
  FunctionEnd
!macroend

!macro TE_INSERT_INFO
  Section -InfoSection

    SetOutPath "$INSTDIR"
    File /nonfatal "${TERRALIB_DIR}\AUTHORS"
    File "${TERRALIB_DIR}\COPYING"
    File /nonfatal "${TERRALIB_DIR}\NEWS"
    File /oname=README.txt "${TERRALIB_DIR}\README"

  SectionEnd ;-InfoSection
!macroend

!macro TE_INSERT_ADDITIONALICONS
Section -AdditionalIcons
  
  ;Define that SMPROGRAMS will be the all users SMPROGRAMS
  SetShellVarContext all
  
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateDirectory "$SMPROGRAMS\TerraLib5"
  CreateShortCut "$SMPROGRAMS\TerraLib5\${PRODUCT_NAME}.org.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\TerraLib5\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd ; -AdditionalIcons
!macroend

!macro TE_INSERT_POST
Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd ; -Post
!macroend

!macro TE_INSERT_DESCRIPTIONS
  ; Sections descriptions
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN

    !insertmacro MUI_DESCRIPTION_TEXT ${SUBSEC_COMPONENTS} "Components of TerraLib 5"
    !insertmacro MUI_DESCRIPTION_TEXT ${SEC_PLUGINS} "Plugins for TerraLib 5"
    !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DOCUMENTATION} "TerraLib 5 Documentation"
    !insertmacro MUI_DESCRIPTION_TEXT ${SEC_EXAMPLES} "TerraLib 5 Examples"
    !insertmacro MUI_DESCRIPTION_TEXT ${SEC_UNITTEXT} "TerraLib 5 Init Test"
    !insertmacro MUI_DESCRIPTION_TEXT ${SUBSEC_DEPENDENCIES} "TerraLib 5 Dependencies"

    !include "TeComponents.nsh"
    !insertmacro TE_GET_COMPONENTS_DESCRIPTION
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_ANNOTATIONTEXT} "${ANNOTATIONTEXT_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_COLOR} "${COLOR_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_COMMON} "${COMMON_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_DATAACCESS} "${DATAACCESS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_DATATYPE} "${DATATYPE_DESCRIPTION}"
	  !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_DYGEO} "${DYGEO_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_FILTER} "${FILTER_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_GDAL} "${GDAL_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_GEOMETRY} "${GEOMETRY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_GML} "${GML_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_MAPTOOLS} "${MAPTOOLS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_MEMORY} "${MEMORY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_OGR} "${OGR_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_PLUGIN} "${PLUGIN_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_POSTGIS} "${POSTGIS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_QTWIDGETS} "${QTWIDGETS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_RASTER} "${RASTER_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_SAM} "${SAM_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_SRS} "${SRS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_SYMBOLOGY} "${SYMBOLOGY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_XLINK} "${XLINK_DESCRIPTION}"
	  !insertmacro MUI_DESCRIPTION_TEXT ${SEC_DEBUG_XML} "${XML_DESCRIPTION}"

      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_ANNOTATIONTEXT} "${ANNOTATIONTEXT_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_COLOR} "${COLOR_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_COMMON} "${COMMON_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_DATAACCESS} "${DATAACCESS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_DATATYPE} "${DATATYPE_DESCRIPTION}"
	  !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_DYGEO} "${DYGEO_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_FILTER} "${FILTER_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_GDAL} "${GDAL_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_GEOMETRY} "${GEOMETRY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_GML} "${GML_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_MAPTOOLS} "${MAPTOOLS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_MEMORY} "${MEMORY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_OGR} "${OGR_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_PLUGIN} "${PLUGIN_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_POSTGIS} "${POSTGIS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_QTWIDGETS} "${QTWIDGETS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_RASTER} "${RASTER_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_SAM} "${SAM_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_SRS} "${SRS_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_SYMBOLOGY} "${SYMBOLOGY_DESCRIPTION}"
      !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_XLINK} "${XLINK_DESCRIPTION}"
	  !insertmacro MUI_DESCRIPTION_TEXT ${SEC_RELEASE_XML} "${XML_DESCRIPTION}"

  !insertmacro MUI_FUNCTION_DESCRIPTION_END
!macroend

!endif ; TE_INSTALLERTUTILS