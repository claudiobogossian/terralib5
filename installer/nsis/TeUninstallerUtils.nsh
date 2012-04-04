; ---------------------------------------------------------------------------
; TeUninstallerUtils
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script that contains some uninstaller configurations, such as the
; informations that the uninstaller must remove.
;
; ---------------------------------------------------------------------------

!ifndef TE_UNINSTALLERTUTILS
!define TE_UNINSTALLERTUTILS

!macro TE_INSERT_UNINSTALLER

  Section Uninstall

    ;Define that SMPROGRAMS will be the all users SMPROGRAMS
    SetShellVarContext all

    RMDir /r "$SMPROGRAMS\TerraLib5"

    Delete "$INSTDIR\${PRODUCT_NAME}.url"
    Delete "$INSTDIR\uninst.exe"
    Delete "$INSTDIR\AUTHORS"
    Delete "$INSTDIR\COPYING"
    Delete "$INSTDIR\NEWS"
    Delete "$INSTDIR\README.txt"

    RMDir /r "$INSTDIR\bin${PLATFORM}"
    RMDir /r "$INSTDIR\lib${PLATFORM}"
    !ifdef x86-and-x64
      RMDir /r "$INSTDIR\bin64"
      RMDir /r "$INSTDIR\lib64"
    !endif

    RMDir /r "$INSTDIR\include"
    RMDir /r "$INSTDIR\examples"
    RMDir /r "$INSTDIR\unittest"
    RMDir /r "$INSTDIR\documentation"

    RMDir "$INSTDIR"

    DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
    SetAutoClose true
  SectionEnd ; Uninstall

!macroend

!macro TE_INSERT_UNINSTALLER_FUNCTIONS
  Function un.onUninstSuccess
    HideWindow
    MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) foi removido com sucesso do seu computador."
  FunctionEnd ; un.onUninstSuccess

  Function un.onInit
    !insertmacro MUI_UNGETLANGUAGE
    MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Tem certeza que quer remover completamente $(^Name) e todos os seus componentes?" IDYES +2
    Abort
  FunctionEnd ; un.onInit
!macroend

!endif ; TE_UNINSTALLERTUTILS