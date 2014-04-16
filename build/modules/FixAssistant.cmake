set (_assist_exe_path ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/Assistant.app/Contents/MacOS)

execute_process (
  COMMAND /bin/chmod +x "${_assist_exe_path}/Assistant"
)

execute_process (COMMAND install_name_tool -change libQtCLucene.4.dylib @executable_path/../Lib/libQtCLucene.4.8.5.dylib ${CMAKE_INSTALL_PREFIX}/${TE_BUNDLE_APP}/Frameworks/QtHelp.framework/Versions/4/QtHelp)

file (
  WRITE
  ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/Assistant.app/Contents/Resources/qt.conf
  "[Paths]
Plugins = ../../../QtPlugins"
)


file(
  WRITE
  ${_assist_exe_path}/assist.sh
  "#!/bin/sh

BUNDLE=\"`echo \"$0\" | sed -e 's/\\/Contents\\/MacOS\\/.*//'`\"
EXECDIR=\"$BUNDLE/Contents/MacOS/Assistant.app/Contents/MacOS\"

export \"DYLD_LIBRARY_PATH=$BUNDLE/Contents/Lib\"

$EXECDIR/qassistant \"$@\"
"
)

execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${_assist_exe_path}/Assistant ${_assist_exe_path}/qassistant
)

execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${_assist_exe_path}/assist.sh ${_assist_exe_path}/Assistant)

execute_process (
  COMMAND /bin/chmod +x "${_assist_exe_path}/Assistant"
)


execute_process (
  COMMAND ln -s ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/Frameworks ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/Assistant.app/Contents/Frameworks
)