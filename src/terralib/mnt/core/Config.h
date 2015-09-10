
#ifndef __TERRALIB_MNT_CORE_INTERNAL_CONFIG_H
#define __TERRALIB_MNT_CORE_INTERNAL_CONFIG_H

#define TE_MNT_MODULE_NAME "te.mnt"


/*! Defines the attribute name Bayes used in Bayes operation*/
#define TE_MNT_CREATEISOLINES_ATTR_NAME "CreateIsolines"

/** @name DLL/LIB Module
*  Flags for building TerraLib as a DLL or as a Static Library
*/
//@{

/*!
\def TEMNTEXPORT

\brief You can use this macro in order to export/import classes and functions from this module.

\note If you want to compile TerraLib as DLL in Windows, remember to insert TEMNTDLL into the project's list of defines.

\note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEMNTSTATIC flag into the project list of defines.
*/
#ifdef WIN32

#ifdef _MSC_VER
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )
#endif


#ifdef TEMNTSTATIC
#define TEMNTEXPORT                          // Don't need to export/import  . it is a static library
#elif TEMNTDLL
#define TEMNTEXPORT  __declspec(dllexport)   // export DLL information
#else
#define TEMNTEXPORT  __declspec(dllimport)   // import DLL information
#endif 
#else
#define TEMNTEXPORT
#endif

//@}

#endif  // __TERRALIB_MNT_CORE_INTERNAL_CONFIG_H
