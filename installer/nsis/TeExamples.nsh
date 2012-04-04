; ---------------------------------------------------------------------------
; TeExamples
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib examples manipulation. Contains macros that make the
; insertion of all TerraLib examples source code in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_EXAMPLES
!define TE_EXAMPLES

!macro TE_INCLUDE_EXAMPLES_FILES
  
    SetOutPath "$INSTDIR\examples\"
    File /nonfatal /r /x *.svn "${TERRALIB_DIR}\examples\"

!macroend

!endif ; TE_EXAMPLES