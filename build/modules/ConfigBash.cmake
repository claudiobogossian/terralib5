execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_INSTALL_PREFIX}/bin/terraview ${CMAKE_INSTALL_PREFIX}/bin/tview
)

execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_INSTALL_PREFIX}/bin/terraview.sh ${CMAKE_INSTALL_PREFIX}/bin/terraview
)

execute_process (
  COMMAND /bin/chmod +x "${CMAKE_INSTALL_PREFIX}/bin/terraview"
)
