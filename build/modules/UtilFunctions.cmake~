# ---------------------------------------------------------------------------------------
# Utils functions
#
# To use these functions make SURE that you have include this file on your CMake project.
# ---------------------------------------------------------------------------------------

# Macro MakeFilter
#
# brief Create filter for a set of files, to use in visual studio ide.
#
# This macro creates a filter in Visual Studio IDE. It's used for organization pourposes. To use the macro call it from somewhere in your cmake code, 
# like presented below:
#
# Example 1
# ... 
# set (h_files include/test1.h include/test2.h ...)
# set (src_files src/test1.cpp src/test2.cpp ...)
#
# MakeFilter ( test ${h_files} ) -> will create filter "test" in Header Files default filter.
# MakeFilter ( test ${src_files} ) -> will create filter "test" in Source Files default filter.
# ...
#
# param[input] filter_path Filter to be used. (Can use "/" to add subfilters.)
# param[input] files_lst List of files contained in the generated filter.
# param[input] h_flag Header flag. If ON the files are headers otherwise sources. We can then update the correct default filter.
# 
MACRO ( makeFilter filter_path files_lst h_flag)

	set ( f_list ${files_lst} )
	set ( n_files 0 )
  set (_fPath "")
	
	list ( LENGTH f_list n_files )
	
	if ( ${n_files} )
    if(NOT "${filter_path}" STREQUAL "" OR "${filter_path}" STREQUAL "." )
      set (_fPath "${filter_path}/" )
    endif()  
      
		if ( ${h_flag} )
			set ( _fPath "${_fPath}Header Files" )
		else ()
			set ( _fPath "${_fPath}Source Files" )
		endif ()
		STRING ( REPLACE "/" "\\" _fPath ${_fPath} )
		source_group( ${_fPath} FILES ${files_lst} )
	endif()
	
ENDMACRO(makeFilter)

# Macro getFiles
#
# brief Return the files in a folder with the required extension.
#
# param[input] folder Folder.
# param[input] extension Extension of the files to be appended.
# param[output] sources A list with all files in the folder.
# 
MACRO ( getFiles folder extension sources )
	set ( ${sources} "" )
	FILE ( GLOB new_src ${folder}/${extension} )
	list ( APPEND ${sources} ${new_src} )
ENDMACRO ()

# Macro getFfiles
#
# brief Return the (header or source) files into a folder and creating customized filters (based on folder name).
#
# param[input] fprefix Prefix to be append.
# param[input] folders List of folders.
# param[output] sfiles list of files in the folder.
# param[input] f_base Base filter. (Use in visual studio)
# 
MACRO ( getFfiles fprefix folders sfiles f_base )
	set ( ${sfiles} "" )

	foreach ( folder ${folders} )
		
		set ( pfolder "${fprefix}/${folder}" )
    
    #Header files
		set ( h_files "")
		getFiles ( "${pfolder}" "*.h*" h_files )
    
    #Source files
		set ( src_files "")
		getFiles ( "${pfolder}" "*.c*" src_files )
    
    list ( APPEND ${sfiles} "${h_files}" "${src_files}")

		if ( NOT ${f_base} EQUAL "" )
			set ( folder "${f_base}/${folder}" )
		endif()
		
		if( ("${folder}" STREQUAL "${f_base}/.") OR ("${folder}" STREQUAL ".") )
      set ( _filterPath "${f_base}" )
		else ()
      set ( _filterPath "${folder}" )
		endif ()

		makeFilter ( "${_filterPath}" "${h_files}" TRUE )
		makeFilter ( "${_filterPath}" "${src_files}" FALSE )
		
	endforeach ()
ENDMACRO ()

# Macro appPrefix
#
# brief Appends a prefix to a set of directories.
#
# param[input] prefix Prefix to be append.
# param[input] folders List of folders to use the prefix
# param[output] result Result union "${prefix}/${folder}" for each folder in the set.
# 
MACRO ( appPrefix prefix folders result )
	set ( ${result} "" )
	
	foreach(folder ${folders})
		if( ${folder} STREQUAL "." )
			list ( APPEND ${result} "${prefix}" )
		else ()
			list ( APPEND ${result} "${prefix}/${folder}" )
		endif ()
	endforeach()
ENDMACRO ()

# Macro checkLibNames
#
# Brief compose library names based on its configuration Debug/Release
#
# param[input] base_name The base name used to compose the variable name, e.g LOG4CXX_LIBRARY the base_name value is "LOG4CXX".

MACRO(checkLibNames base_name)
  if(${base_name}_LIBRARY_DEBUG AND ${base_name}_LIBRARY_RELEASE)
    set (${base_name}_LIBRARY optimized ${${base_name}_LIBRARY_RELEASE} debug ${${base_name}_LIBRARY_DEBUG})
  else()
    if(${base_name}_LIBRARY_DEBUG AND NOT ${base_name}_LIBRARY_RELEASE)
      set (${base_name}_LIBRARY ${${base_name}_LIBRARY_DEBUG})
    endif()
    
    if(${base_name}_LIBRARY_RELEASE AND NOT ${base_name}_LIBRARY_DEBUG)
      set (${base_name}_LIBRARY ${${base_name}_LIBRARY_RELEASE})
    endif()
  endif()
ENDMACRO(checkLibNames)

# Macro configureProject
#
# brief Configure some variables that will be used by EVERY project.
#
# param[input] proj_name Project name.
# param[input] h_dir  Folder that contains header files. 
# param[input] src_dir Folder that contains source files.

MACRO (configureProject proj_name h_dir src_dir )
  cmake_minimum_required(VERSION 2.8)

  # IMPORTANT - Controls for the source and build tree locations
  set(SRCDIR ${src_dir})               # directory where the source files of the terralib color module are located
  set(INCLDIR ${h_dir})                               # directory where the header files of the terralib color module are located

  # Set the project name
  set(PROJ_NAME ${proj_name})
  project(${proj_name})             # the project name is terralib_color
  set(LIB_NAME ${proj_name})        # the library name is terralib_color
  
#  set (DEP_INCLUDES "${ROOT}/src" "${CMAKE_BINARY_DIR}")
#  list (APPEND DEP_INCLUDES "${h_dir}")
ENDMACRO (configureProject)

# Macro configureLibraryOutput
#
# brief Configure the output of the projects AS LIBRARIES. If you WON'T generate a SHARED LIBRARY, DON'T use this function.
#
# param[input] proj_name Name of the project.
# param[input] hdrs Header files.
# param[input] srcs Source files.
# param[input] inc_path List os folders to search headers.
# param[input] links Libraries to link against.

MACRO (configureLibraryOutput proj_name hdrs srcs inc_path links)
  include_directories (${inc_path} "${ROOT}/src" "${CMAKE_BINARY_DIR}")
  add_library(${proj_name} SHARED ${hdrs} ${srcs})
  target_link_libraries(${proj_name} ${links})
  set_target_properties(${proj_name} PROPERTIES LINK_INTERFACE_LIBRARIES "" DEBUG_POSTFIX _d)
ENDMACRO (configureLibraryOutput)

# Macro makeProject
#
# brief High-level macro that creates a project.
#
# param[input] proj_name Project name without preffix or suffix.
# param[input] root_h_dir Root folder of the header files.
# param[input] root_src_dir Root folder of the sources files.

MACRO(makeProject proj_name root_h_dir root_src_dir)
  configureProject("terralib_${proj_name}" "${root_h_dir}" "${root_src_dir}")
  include ("${proj_name}.cmake")
  
  configureLibraryOutput(${PROJ_NAME} "${HDRS}" "${SRCS}" "${TE_DEP_INCLUDES}" "${TE_DEP_LIBS}")
  
  # installing libraries
  installTarget(${PROJ_NAME} "lib" "LIBRARIES" "bin" "BINARIES")
  
  # installing header files
  installFiles(${root_h_dir} "terralib" "HEADERS" "*.h*")
ENDMACRO(makeProject)

# Macro makePluginProject
#
# brief High-level macro that creates a project for plug-in and the instalation target.
#
# param[input] proj_name Project name without preffix or suffix.
# param[input] root_h_dir Root folder of the header files.
# param[input] root_src_dir Root folder of the sources files.

MACRO(makePluginProject proj_name root_h_dir root_src_dir)
  configureProject("terralib_${proj_name}" "${root_h_dir}" "${root_src_dir}")
  include ("${proj_name}.cmake")
  
  configureLibraryOutput(${PROJ_NAME} "${HDRS}" "${SRCS}" "${TE_DEP_INCLUDES}" "${TE_DEP_LIBS}")

  if(WIN32)  
    install(
      TARGETS ${PROJ_NAME}
      RUNTIME DESTINATION "bin" COMPONENT PLUGINS
    )
  elseif (APPLE)

    set_target_properties(${PROJ_NAME} 
       PROPERTIES 
       LINK_FLAGS "-current_version ${TE_VERSION} -compatibility_version ${TE_VERSION}"
       VERSION ${TE_VERSION} 
       SOVERSION ${TERRALIB_MAJOR_VERSION} 
       INSTALL_NAME_DIR "@executable_path/../Plugins"
    )

    install(
      TARGETS ${PROJ_NAME}
      LIBRARY DESTINATION ${TE_BUNDLE_APP}/Plugins COMPONENT PLUGINS
    )
  else ()
    install(
      TARGETS ${PROJ_NAME}
      LIBRARY DESTINATION "lib" COMPONENT PLUGINS
      ARCHIVE DESTINATION "lib" COMPONENT PLUGINS
    )
  endif()
  
  # installing header files
  installFiles("${root_h_dir}" "terralib" "HEADERS" "*.h*")
  
ENDMACRO(makePluginProject)

# Macro installTarget
#
# brief This macro is used to generate installation rules for the terralib libraries.
#
# param[input] proj_name Target name used on project
# param[input] lib_dst_dir Name of the directory where the libraries will be installed. (Windows) 
# param[input] lib_cmp_name Name of the component to be used with binary installer.
# param[input] bin_dst_dir Name of the directory where the binaries will be installed.
# param[input] bin_cmp_name Name of the component to be used with binary installer.
#
# note1: The destination dirs are relative to CMAKE_INSTALL_PREFIX value, or a sub-folder in CMAKE_INSTALL_PREFIX folder. 
# It means that the user do not need to give a complete location. E.g use "lib", "bin", "exec"
# note2: The component name is used in binaries installers such as NSIS (Win). This is used only for classify the components
# you can install. This way, we can create binary installers with optional installation rules.
MACRO(installTarget proj_name lib_dst_dir lib_cmp_name bin_dst_dir bin_cmp_name)
  if(APPLE AND WITH_BUNDLE)
    set_target_properties ( ${proj_name} 
      PROPERTIES
      LINK_FLAGS "-current_version ${TE_VERSION} -compatibility_version ${TE_VERSION}"
      VERSION ${TE_VERSION} 
      SOVERSION ${TERRALIB_MAJOR_VERSION} 
      INSTALL_NAME_DIR @executable_path/../Lib 
    )    

    install(
      TARGETS ${proj_name}
      EXPORT teDepends
      LIBRARY DESTINATION ${TE_BUNDLE_APP}/Lib COMPONENT ${bin_cmp_name}
      ARCHIVE DESTINATION ${TE_BUNDLE_APP}/Lib COMPONENT ${bin_cmp_name}
    )
  else()
    install(
      TARGETS ${proj_name}
      EXPORT teDepends
      RUNTIME DESTINATION "${bin_dst_dir}" COMPONENT ${bin_cmp_name}
      LIBRARY DESTINATION "${lib_dst_dir}" COMPONENT ${lib_cmp_name}
      ARCHIVE DESTINATION "${lib_dst_dir}" COMPONENT ${lib_cmp_name}
    )
  endif()
ENDMACRO(installTarget)

# Macro installFiles
#
# brief
#
# param[input] dir
# param[input] dest
# param[input] cmp_name
# param[input] ext
MACRO (installFiles dir dest cmp_name ext)
  install ( 
    DIRECTORY "${dir}"
    DESTINATION "${dest}"
    COMPONENT ${cmp_name}
    FILES_MATCHING PATTERN ${ext}
  )
ENDMACRO (installFiles)

# Macro exportModuleInformation
#
# brief Append the values to variables that will be used in terralib configuration files.
#
# param[input] mod_name Name of the module. Note that this variable must be the library name without the prefix "terralib_".
# param[input] dirs Directories at the build tree.
# param[input] inst_dirs Directories at the installation tree.
MACRO(exportModuleInformation mod_name dirs inst_dirs)
  set (TE_MODULES ${TE_MODULES} "${mod_name}" PARENT_SCOPE)
  set (TE_INCLUDE_DIRS "${TE_INCLUDE_DIRS}" "${dirs}" PARENT_SCOPE)
  set (TE_INST_INCLUDE_DIRS ${TE_INST_INCLUDE_DIRS} "${inst_dirs}" PARENT_SCOPE)
  set (TE_LIBRARIES ${TE_LIBRARIES} "terralib_${mod_name}" PARENT_SCOPE)
  set (TE_3DS ${TE_3DS} "${TE_DEP_INCLUDES}" PARENT_SCOPE)
ENDMACRO(exportModuleInformation)

# Macro generateRunningBatch
#
# brief
#
# param[input] fileName
MACRO(generateRunningBatch templateFileName fileName)
  if(WIN32)
    get_filename_component (TE_QT_DIR ${QT_QMAKE_EXECUTABLE} PATH)
    set (TE_QT_DIR ${TE_QT_DIR} CACHE PATH "Location of installed Qt binaries.")
    set (TE_BIN_DIR "${TE_BIN_DIR}" CACHE PATH "Location of the terralib binaries.")
    configure_file (${templateFileName} ${CMAKE_CURRENT_BINARY_DIR}/${fileName})
  endif()
ENDMACRO(generateRunningBatch)

# Macro copyLogFile
#
# brief
# param[input] logDir
MACRO(copyLogFiles logDir)
  if(NOT EXISTS ${logDir}/conf/te-log.conf)
    message (STATUS "-- Copying ${TE_DEFAULT_LOGCONF_FILE}.")
    configure_file(${TE_DEFAULT_LOGCONF_FILE} "${logDir}/conf/te-log.conf" COPYONLY)
    message (STATUS "-- te-log.conf copied to \"${logDir}/conf\".")
  endif()
ENDMACRO(copyLogFiles)

# Macro makeQHelp
#
# brief
# param[input] input
# param[input] output
MACRO(makeQHelpProject input output)
  message ("Creating Qt help project file...")
  
  get_filename_component (_qtbindir ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} PATH)
  
  execute_process(
    COMMAND ${_qtbindir}/qhelpgenerator ${input} -o ${output}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE res
    ERROR_FILE error.log
  )
  
  if(res EQUAL 0)
    message ("Qt Help project file created successfully!")
  else()
    message ("Error generating Qt Help project file. See error.log for details.")
  endif()
  
ENDMACRO(makeQHelpProject)

# Macro makeQHelp
#
# brief
# param[input] input
# param[input] output
MACRO(makeQHelp input output)
  message ("Creating Qt help files...")
  
  execute_process(
    COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${input} -o ${output}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE res
    ERROR_FILE error.log
  )
  
  if(res EQUAL 0)
    message ("Qt Help file created successfully!")
  else()
    message ("Error generating Qt Help file. See error.log for details.")
  endif()
ENDMACRO(makeQHelp)

# Macro removeFileIfExists
#
# brief
# param[input] fileName
MACRO (removeFileIfExists fileName)
  if(EXISTS ${fileName})
    file (REMOVE ${fileName})
  endif()
ENDMACRO (removeFileIfExists)

MACRO (getVSVersion v)
  if(WIN32 AND MSVC)
    if(MSVC60)
      set (${v} 6)
    else() 
      if(MSVC70)
        set (${v} 7)
      else() 
        if(MSVC71)
          set (${v} 7.1)
        else() 
          if(MSVC80)
            set (${v} 8)
          else() 
            if(MSVC90)
              set (${v} 9)
            else() 
              if(MSVC10)
                set (${v} 10)
              endif()
            endif()
          endif()
        endif()
      endif()
    endif()
  else()
    set (${v} UNKNOWN)
  endif()
ENDMACRO(getVSVersion)

MACRO (getPackageName packName)
  if(${BUILD_TERRAVIEW})
    set (_pname "terraview-${TE_VERSION}") 
  else()
    set (_pname "terralib-${TE_VERSION}") 
  endif()
  
  if(WIN32)
    if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
      set(_pname "${_pname}-win32")
    else()
     set(_pname "${_pname}-win64")
     endif()
  endif()
  
 # if(WIN32)
 #   if(MSVC)
 #     set (VSv "")
 #     getVSVersion(VSv)
 #     set(_pname ${_pname}-msvc${VSv})
 #     if(CMAKE_CL_64)
 #       set(_pname ${_pname}-i686)
 #     else()
 #       set(_pname ${_pname}-i386)
 #     endif()
 #   endif()
 # else()
 #   if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
 #     set(_arc i386)
 #   else()
 #     set(_arc i686)
 #   endif()
    
#    if(UNIX AND NOT APPLE)
#      set(_pname ${_pname}-${CMAKE_SYSTEM_NAME}-${_arc})
#    endif()
#  endif()

  set (${packName} ${_pname})
ENDMACRO(getPackageName)

# Macro te_moc
#
# brief Sets the include of the mocs as subfolders of src. 
# In Visual Studio, depending on your folder structure, the include file may be longer,
# and generate error. 
#
# param input Files to moc.
# param output Files generated.
#
MACRO (te_moc input output)
  foreach(sfile_ ${input})
    get_filename_component(fpath_ ${sfile_} PATH)
    string (FIND ${fpath_} "terralib" te_pos_ REVERSE)
    string (SUBSTRING "${fpath_}" ${te_pos_} -1 te_path_)
    qt4_wrap_cpp(moc_ ${sfile_} OPTIONS -p ${te_path_})
    list(APPEND ${output} ${moc_})
  endforeach()
ENDMACRO(te_moc)

MACRO(te_moc2 input prefix output)
  set (moc_ "")
  qt4_wrap_cpp(moc_ ${input} OPTIONS -p ${prefix})
  list(APPEND ${output} ${moc_})
ENDMACRO(te_moc2)

# Macro getPluginInfo
#
# Get xml formatted information of a plug-in 
#
# param plugin_name[input] Name of the plug-in.
# param plugin_location[input] Location of the binary file of the plug-in.
# param plugins_info[output] Variable containing information plug-ins updated.
#
MACRO(getPluginInfo plugin_name plugin_location plugins_info)
set (${plugins_info}
  "  <Plugin>\n    <Name>${plugin_name}</Name>\n    <Path xlink:href=\"${plugin_location}\"></Path>\n  </Plugin>\n"
  )
ENDMACRO(getPluginInfo)

# Macro getPluginResources
#
# Get xml formatted information of a resource 
#
# param rsc_name[input] Resource type.
# param rsc_link[input] Resource xlink.
# param rsc_info[output] Resource information formatted as XML.
#
MACRO(getPluginResources rsc_name rsc_link rsc_info)
  set (${rsc_info}
  "\n    <Resource name=\"${rsc_name}\" xlink:href=\"${rsc_link}\"/>"
  )  
ENDMACRO(getPluginResources)

# Macro getPluginRequirements
#
# Get xml formatted information of a resource 
#
# param req_name[input] Requirement name.
# param req_info[output] Requirement information formatted as XML.
#
MACRO(getPluginRequirements req_name req_info)
  set (${req_info}
  "\n    <PluginId>${req_name}</PluginId>\n"
  )  
ENDMACRO(getPluginRequirements)

# Macro today
#
# Gets the current date as a formatted string.
#
# param RESULT[output] The formatted data.
MACRO (today RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${RESULT})
        EXECUTE_PROCESS(COMMAND "cmd" " /C time /T" OUTPUT_VARIABLE time_)
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
        string(REGEX REPLACE "(..):(..):(..)" "\\1/\\2/\\3" time_ ${time_})
		
	 set(${RESULT} "${${RESULT}}-${time_}")

    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%M/%Y" OUTPUT_VARIABLE ${RESULT})
        EXECUTE_PROCESS(COMMAND "date" "+%X" OUTPUT_VARIABLE time_)
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\1/\\2/\\3" ${RESULT} ${${RESULT}})
        string(REGEX REPLACE "(..):(..):(..)" "\\1/\\2/\\3" time_ ${time_})
		
	set(${RESULT} "${${RESULT}}-${time_}")
    ELSE ()
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (today)

# Macro installQtPlugins
#
# Installs the required Qt plugins.
#
# param plugs List of the names of plugins to be installed.
MACRO(installQtPlugins plgs)

  set (_regex_exp "")

  set( _first TRUE)

  foreach(plg ${plgs})
    if(NOT _first)
      set (_regex_exp ${_regex_exp}|${plg})
    else()
      set (_regex_exp ${plg})
      set (_first FALSE)
    endif()
  endforeach()

  set (_regex_exp "(${_regex_exp})")

  set (_dest ${TE_BUNDLE_APP}/QtPlugins)

  install (DIRECTORY ${QT_PLUGINS_DIR}/imageformats  
    DESTINATION ${_dest} 
    COMPONENT BINARIES
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

  install (DIRECTORY ${QT_PLUGINS_DIR}/iconengines  
    DESTINATION ${_dest} 
    COMPONENT BINARIES
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

  install (DIRECTORY ${QT_PLUGINS_DIR}/sqldrivers  
    DESTINATION ${_dest} 
    COMPONENT BINARIES
    FILES_MATCHING
    REGEX "${_regex_exp}"
  )

ENDMACRO(installQtPlugins)
