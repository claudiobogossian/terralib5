include (BundleUtilities)
    
function(gp_item_default_embedded_path item default_embedded_path_var)
  set (is_system FALSE)
  set (is_QImageFormat FALSE)
  set (is_QIconEngine FALSE)
  set (is_QSQLDrivers FALSE)
  set (is_Plugin FALSE)
  set (is_Lib FALSE)

  get_filename_component (resolved_file ${item} PATH)

  if(resolved_file MATCHES "^(/System/Library/|/usr/lib/|/usr/local|@loader_path)" OR "${resolved_file}" STREQUAL "")
     set(is_system TRUE)
  elseif(resolved_file MATCHES "(imageformats)$")
    set (is_QImageFormat TRUE)
  elseif(resolved_file MATCHES "(iconengines)$")
    set (is_QIconEngine TRUE)
  elseif(resolved_file MATCHES "(sqldrivers)$")
    set (is_QSQLDrivers TRUE)
  elseif(resolved_file MATCHES "(Plugins)$")
    set (is_Plugin TRUE)
  elseif(resolved_file MATCHES "(Lib)$")
    set (is_Lib TRUE)
  endif()
    
  if(item MATCHES "\\.dylib$")
     if(is_system)
       set(${default_embedded_path_var} "@executable_path/../MacOS" PARENT_SCOPE)
     elseif(is_QImageFormat)
       set(${default_embedded_path_var} "@executable_path/../QtPlugins/imageformats" PARENT_SCOPE)
     elseif(is_QIconEngine)
       set(${default_embedded_path_var} "@executable_path/../QtPlugins/iconengines" PARENT_SCOPE)
     elseif(is_QSQLDrivers)
       set(${default_embedded_path_var} "@executable_path/../QtPlugins/sqldrivers" PARENT_SCOPE)
     elseif(is_Plugin)
       set(${default_embedded_path_var} "@executable_path/../Plugins" PARENT_SCOPE)
     elseif(is_Lib)
       set(${default_embedded_path_var} "@executable_path/../Lib" PARENT_SCOPE)
     else()
       set(${default_embedded_path_var} "${resolved_file}" PARENT_SCOPE)
     endif()
  elseif(item MATCHES "[^/]+\\.framework/")
    set(${default_embedded_path_var} "@executable_path/../Frameworks" PARENT_SCOPE)
  endif()
endfunction()


file (GLOB_RECURSE TE_ALL_SHARED "${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/Plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}")

file (GLOB_RECURSE QT_PLUGINS "${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/QtPlugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}")

set (TE_DEPS_ALLDIRS "/usr/local/lib;/usr/lib")

fixup_bundle("${CMAKE_INSTALL_PREFIX}/terraview.app" "${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/Lib/libQtCLucene.4.8.5.dylib;${TE_ALL_SHARED};${QT_PLUGINS}" "${TE_DEPS_ALLDIRS}")

execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/terraview ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/tview
)

execute_process (
  COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/terraview.sh ${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/terraview
)

execute_process (
  COMMAND /bin/chmod +x "${CMAKE_INSTALL_PREFIX}/terraview.app/Contents/MacOS/terraview"
)