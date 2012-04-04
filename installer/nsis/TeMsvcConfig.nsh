; ---------------------------------------------------------------------------
; TeMsvcConfig
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains specific configurations for TerraLib installers for
; Visual Studio 2010.
;
; ---------------------------------------------------------------------------

!ifndef TE_MSVCCONFIG
!define TE_MSVCCONFIG

!macro TE_MSVC_CONFIG
  !ifndef MSVC_ON
    !define MSVC_ON
  !endif

  !ifdef GENERATOR
    !undef GENERATOR
  !endif

  !ifdef LIB_EXTENSION
    !undef LIB_EXTENSION
  !endif

  !define GENERATOR "msvc2010"
  !define LIB_EXTENSION "lib"
!macroend

!endif ; TE_MSVCCONFIG