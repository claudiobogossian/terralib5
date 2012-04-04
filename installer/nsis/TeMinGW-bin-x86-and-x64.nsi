; ---------------------------------------------------------------------------
; TeMinGW-bin-x86-and-x64
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Contains all specific configurations to generate a TerraLib 32 and 64 bit
; installer for MinGW.
;
; ---------------------------------------------------------------------------

; Terralib configurations
!include "TeConfig.nsh"
!insertmacro APPLY_TERRALIB_CONFIGURARIONS

; Terralib Msvc Config
!include "TeMinGWConfig.nsh"
!insertmacro TE_MINGW_CONFIG

; Terralib MUI Configurarion
!include "TeMuiConfig.nsh"
!insertmacro APPLY_TERRALIB_MUI_CONFIGURARIONS

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "TerraLib5-bin-mingw-x86-and-x64-Setup-${MyTIMESTAMP}.exe"
InstallDir "$PROGRAMFILES${PLATFORM}\TerraLib5"
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"
ShowInstDetails hide
ShowUnInstDetails hide

!include "TeInstallerUtils.nsh"
!insertmacro TE_INSERT_INIT_FUNCTIONS
!insertmacro TE_INSERT_INFO

!define x86-and-x64

!include "TeSections.nsh"
!insertmacro TE_INSERT_COMPONENTS_SUBSECTION

!include "TeComponents.nsh"
!insertmacro TE_INCLUDE_CONFIG_FILES

!insertmacro TE_INSERT_PLUGINS_SECTION
!insertmacro TE_INSERT_DOCUMENTATION_SECTION
!insertmacro TE_INSERT_EXAMPLES_SECTION
!insertmacro TE_INSERT_UNITTEST_SECTION
!insertmacro TE_INSERT_DEPENDENCIES_SUBSECTION

; TeInstallerUtils
!insertmacro TE_INSERT_DESCRIPTIONS

; TeInstallerUtils.nsh
!insertmacro TE_INSERT_ADDITIONALICONS
!insertmacro TE_INSERT_POST

!include "TeUninstallerUtils.nsh"
!insertmacro TE_INSERT_UNINSTALLER_FUNCTIONS
!insertmacro TE_INSERT_UNINSTALLER
