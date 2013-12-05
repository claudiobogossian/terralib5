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
#  ADOTL4_FOUND         - Set to true when ADO is found.
#  ADOTL4_INCLUDE_DIR  - Include directories for ADO
#

cmake_minimum_required(VERSION 2.8)

# Find path - tries to find *.h in paths hard-coded by the script
FIND_PATH (ADOTL4_MSADO26_INCLUDE 
		   NAMES msado26.tlb
		   PATHS "C:/Program Files (x86)/Common Files/System/ado" "C:/Program Files/Common Files/System/ado" "C:/Arquivos de Programas (x86)/Arquivos Comuns/System/ado" "C:/Arquivos de Programas/Arquivos Comuns/System/ado"
)

FIND_PATH (ADOTL4_OLE_INCLUDE 
           NAMES oledb32.dll
           PATHS "C:/Program Files (x86)/Common Files/System/Ole DB" "C:/Program Files/Common Files/System/Ole DB" "C:/Arquivos de Programas (x86)/Arquivos Comuns/System/Ole DB" "C:/Arquivos de Programas/Arquivos Comuns/System/Ole DB"
)

FIND_PATH (ADOTL4_MSADOX_INCLUDE
		   NAMES msadox.dll
           PATHS "C:/Program Files (x86)/Common Files/System/ado" "C:/Program Files/Common Files/System/ado" "C:/Arquivos de Programas (x86)/Arquivos Comuns/System/ado" "C:/Arquivos de Programas/Arquivos Comuns/System/ado"
)

# Export include and library path for linking with other libraries

list (APPEND ADOTL4_INCLUDE_DIR ${ADOTL4_MSADO26_INCLUDE} ${ADOTL4_OLE_INCLUDE} ${ADOTL4_MSADOX_INCLUDE})

IF ( ADOTL4_INCLUDE_DIR  AND ADOTL4_MSADO26_INCLUDE AND ADOTL4_OLE_INCLUDE AND ADOTL4_MSADOX_INCLUDE )
	SET(ADOTL4_FOUND TRUE)
ELSE ( ADOTL4_INCLUDE_DIR  AND ADOTL4_MSADO26_INCLUDE AND ADOTL4_OLE_INCLUDE AND ADOTL4_MSADOX_INCLUDE )
	MESSAGE("Could NOT find ADOTL4 library")
	SET( ADOTL4_FOUND FALSE )
ENDIF ( ADOTL4_INCLUDE_DIR  AND ADOTL4_MSADO26_INCLUDE AND ADOTL4_OLE_INCLUDE AND ADOTL4_MSADOX_INCLUDE )

MARK_AS_ADVANCED( ADOTL4_MSADO26_INCLUDE ADOTL4_OLE_INCLUDE ADOTL4_MSADOX_INCLUDE )