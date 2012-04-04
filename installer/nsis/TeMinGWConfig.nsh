; ---------------------------------------------------------------------------
; TeMinGWConfig
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains specific configurations for TerraLib installers for
; MinGW.
;
; ---------------------------------------------------------------------------

!ifndef TE_MINGWCONFIG
!define TE_MINGWCONFIG

!macro TE_MINGW_CONFIG
  !ifndef MINGW_ON
    !define MINGW_ON
  !endif

  !ifdef GENERATOR
    !undef GENERATOR
  !endif

  !ifdef LIB_EXTENSION
    !undef LIB_EXTENSION
  !endif

  !define GENERATOR "mingw"
  !define LIB_EXTENSION "a"
!macroend


!endif ; TE_MINGWCONFIG