; ---------------------------------------------------------------------------
; TeMuiConfig
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains configurations about the modern user interface, that
; defines the icons path, the installer pages (welcome, licence, components,
; directory), installer languages and other configurations.
;
; ---------------------------------------------------------------------------

!ifndef TE_MUICONFIG
!define TE_MUICONFIG

; MUI 1.67 compatible ------
!include "MUI.nsh"

!macro APPLY_TERRALIB_MUI_CONFIGURARIONS

   ; MUI Settings
  !define MUI_ABORTWARNING
  !define MUI_ICON "${TERRALIB_DIR}\images\icons\64x64\terralib_install_icon.ico"
  !define MUI_UNICON "${TERRALIB_DIR}\images\icons\64x64\terralib_uninstall_icon.ico"

  ; Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
  !define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "${TERRALIB_DIR}\images\bmp\logo\terralib_installer_logo_150x57.bmp"

  ; Welcome page
  !insertmacro MUI_PAGE_WELCOME
  ; License page
  !insertmacro MUI_PAGE_LICENSE "${TERRALIB_DIR}\COPYING"
  ; Components page
  !insertmacro MUI_PAGE_COMPONENTS
  ; Directory page
  !insertmacro MUI_PAGE_DIRECTORY
  ; Instfiles page
  !insertmacro MUI_PAGE_INSTFILES
  ; Finish page
    !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
    !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
  !insertmacro MUI_PAGE_FINISH

  ; Uninstaller pages
  !insertmacro MUI_UNPAGE_INSTFILES

  ; Language files
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "PortugueseBR"

  ; Reserve files
  !insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
  
!macroend

!endif ; TE_MUICONFIG