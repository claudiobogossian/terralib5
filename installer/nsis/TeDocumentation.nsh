; ---------------------------------------------------------------------------
; TeDocumentation
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib documentation manipulation. Contains macros that make
; the insertion of all TerraLib API documentation in the installer, besides
; the creation of some shortcuts to facilitate the user's access to the
; documentation after installed.
;
; ---------------------------------------------------------------------------

!ifndef TE_DOCUMENTATION
!define TE_DOCUMENTATION

!macro TE_INCLUDE_DOCUMENTATION_FILES
  
  SetOutPath "$INSTDIR\documentation"
  File /nonfatal /r /x *.svn "${TERRALIB_DIR}\doc\*.*"
  
!macroend

!macro TE_CREATE_DOCUMENTATION_SHORTCUTS

  CreateShortCut "$INSTDIR\documentation\api\API-Documentation.lnk" \
                 "$INSTDIR\documentation\api\html\index.html"

  CreateShortCut "$SMPROGRAMS\TerraLib5\API-Documentation.lnk" "$INSTDIR\documentation\api\html\index.html"

!macroend

!endif ; TE_DOCUMENTATION