# Find the ADO installation.
# (c) Frederico A. Bedê, 2011
# ----------------------------------------------------------------------------
# Usage:
# In your CMakeLists.txt file do something like this:
# ...
# # ADO
# FIND_PACKAGE(ADO)
# ...
# if( ADO_FOUND )
#   include_directories(${ADO_INCLUDE_DIRS})
#   link_directories(${ADO_LIBRARY_DIRS})
# endif( ADO_FOUND )
# ...
# Remember to include ${ADO_LIBRARIES} in the target_link_libraries() statement.
#
# ----------------------------------------------------------------------------
# IMPORTANT - You may need to manually set:
#  PATHS in line 54  - path to where the ADO include files are.
#  PATHS in line 62  - path to where the ADO library files are.
#  in case FindADO.cmake cannot find the include files or the library files.
#
# ----------------------------------------------------------------------------
# The following variables are set if ADO is found:
#  ADO_FOUND         - Set to true when ADO is found.
#  ADO_INCLUDE_DIR  - Include directories for ADO
#  ADO_LIBRARIES     - The ADO libraries.
#

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH(ADO_INCLUDE_DIR 
   NAMES msado26.tlb
   PATHS $ENV{T4_DEPS_DIR}/bin/win32-vs2010 "c:/windows/system" "C:/Program Files/Common Files/System/ado" "C:/Arquivos de Programas/Arquivos Comuns/System/ado"
)

FIND_PATH(OLE_INCLUDE_DIR 
   NAMES oledb32.dll
   PATHS $ENV{T4_DEPS_DIR}/bin/win32-vs2010 "c:/windows/system" "C:/Program Files/Common Files/System/Ole DB" "C:/Arquivos de Programas/Arquivos Comuns/System/Ole DB"
)

# Find library - - tries to find *.a,*.so,*.dylib in paths hard-coded by the script

FIND_FILE ( 
  ADO_LIBRARY
  NAMES msadox.dll
  PATHS $ENV{T4_DEPS_DIR}/bin/win32-vs2010 "c:/windows/system" "C:/Program Files/Common Files/System/ado" "C:/Arquivos de Programas/Arquivos Comuns/System/ado"
)

# Export include and library path for linking with other libraries

list (APPEND ADO_INC_DIRS ${ADO_INCLUDE_DIR} ${OLE_INCLUDE_DIR} )

IF ( ADO_INCLUDE_DIR  AND ADO_LIBRARY AND OLE_INCLUDE_DIR )
	SET(ADO_FOUND TRUE)
ELSE ( ADO_INCLUDE_DIR AND ADO_LIBRARY AND OLE_INCLUDE_DIR )
	MESSAGE("Could NOT find ADO library")
	SET( ADO_FOUND FALSE )
ENDIF ( ADO_INCLUDE_DIR AND ADO_LIBRARY AND OLE_INCLUDE_DIR)

MARK_AS_ADVANCED( ADO_LIBRARY ADO_INCLUDE_DIR OLE_INCLUDE_DIR )