; ---------------------------------------------------------------------------
; TeConfig
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains the macro that applies the general settings, such as
; the project name, version, installation types, TerraLib path, platform and
; other settings.
;
; ---------------------------------------------------------------------------

!ifndef TE_CONFIG
!define TE_CONFIG

!macro APPLY_TERRALIB_CONFIGURATIONS

  SetCompressor lzma

  !define /date MyTIMESTAMP "%Y%m%d" ;%H%M%S"

  !define PRODUCT_NAME "TerraLib"
  !define PRODUCT_VERSION "5.0"
  !define PRODUCT_PUBLISHER "Instituto Nacional de Pesquisas Espaciais - INPE"
  !define PRODUCT_WEB_SITE "www.terralib.org"
  !define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
  !define PRODUCT_UNINST_ROOT_KEY "HKLM"

  InstType "Full"           ; 1| Numbers used to define which types of installation the sections 
  InstType "Debug"          ; 2| are checked in the components tree through the function "SectionIn"
  InstType "Release"        ; 3| called in the section. 
  InstType "Msvc-Debug"     ; 4|
  InstType "Msvc-Release"   ; 5| Example: A debug section: SectionIn 1 2. A release: SectionIn 1 3.
  InstType "MinGW-Debug"    ; 6|
  InstType "MinGW-Release"  ; 7|

  !define TERRALIB_DIR "..\.."    ;Define the TerraLib path in the local machine.
  !define PLATFORM "32"           ;Define the platform that TerraLib was compiled ("32"|"64" bit).
  
!macroend

!endif ; TE_CONFIG