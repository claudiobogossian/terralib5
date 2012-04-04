; ---------------------------------------------------------------------------
; TeTools
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib tools manipulation. Contains macros that make
; the insertion of all TerraLib tools in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_TOOLS
!define TE_TOOLS

!macro TE_INSERT_TOOLS
	!insertmacro TE_INSERT_TOOLS_DSINFO
!macroend
  


!macro TE_INSERT_TOOLS_DSINFO
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\te_dsinfo.exe"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_program_options-vc100-mt-1_47.dll"
!macroend


!endif ; TE_TOOLS