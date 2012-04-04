; ---------------------------------------------------------------------------
; TeHeaderFiles
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib header files manipulation. Contains macros that make
; the insertion of all TerraLib header files in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_HEADER_FILES
!define TE_HEADER_FILES

!macro TE_INCLUDE_HEADER_FILES
  Section
    SetOutPath "$INSTDIR\include\"
    File /nonfatal /r /x *.svn "${TERRALIB_DIR}\src\*.h"
  SectionEnd
!macroend

!endif ; TE_HEADER_FILES