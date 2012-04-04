; ---------------------------------------------------------------------------
; TeUnittest
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib unit tests manipulation. Contains macros that make the
; insertion of all TerraLib unit tests source code in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_UNITTEST
!define TE_UNITTEST

!macro TE_INCLUDE_UNITTEST_FILES

    SetOutPath "$INSTDIR\unittest\"
    File /nonfatal /r /x *.svn "${TERRALIB_DIR}\unittest\"

!macroend

!endif ; TE_UNITTEST