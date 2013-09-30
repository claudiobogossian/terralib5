include (InstallRequiredSystemLibraries)

# There is a bug in NSI that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backlasshes.

set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/../resources/images/ico\\\\terralib-install-icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/../resources/images/ico\\\\terralib-uninstall-icon.ico")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${PKG_NAME}")
set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.dpi.inpe.br")
set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.funcate.org.br")

# Creating TERRALIB_DIR enviroment variable.
list (APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  WriteRegStr HKCU 'Environment' 'TERRALIB_DIR' '$INSTDIR'  \\n  SendMessage \\\${HWND_BROADCAST} \\\${WM_WININICHANGE} 0 \\\"STR:Environment\\\" /TIMEOUT=5000")

# Removing TERRALIB_DIR from enviroment variables.
set (CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS " DeleteRegValue HKCU 'Environment' 'TERRALIB_DIR' \\n  SendMessage \\\${HWND_BROADCAST} \\\${WM_WININICHANGE} 0 \\\"STR:Environment\\\" /TIMEOUT=5000")

list (APPEND CPACK_NSIS_CREATE_ICONS_EXTRA ${CPACK_NSIS_CREATE_ICONS_EXTRA}
"\nSetOutPath \\\"$INSTDIR\\\\bin\\\""
"\nCreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\TerraView.lnk\\\" \\\"$INSTDIR\\\\bin\\\\terraview.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\resources\\\\images\\\\ico\\\\terralib-install-icon.ico\\\""
"\nCreateShortCut \\\"$DESKTOP\\\\TerraView.lnk\\\" \\\"$INSTDIR\\\\bin\\\\terraview.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\resources\\\\images\\\\ico\\\\terralib-install-icon.ico\\\""
)

list (APPEND CPACK_NSIS_DELETE_ICONS_EXTRA ${CPACK_NSIS_DELETE_ICONS_EXTRA}
"\nDelete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\TerraView.lnk\\\""
"\nDelete \\\"$DESKTOP\\\\TerraView.lnk\\\""
)
