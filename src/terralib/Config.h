/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/Config.h

  \brief Global configuration flags for the TerraLib.
*/

#ifndef __TERRALIB_INTERNAL_CONFIG_H
#define __TERRALIB_INTERNAL_CONFIG_H

/** @name OS Platform Detection Macros
 *  Macros for detecting operational system platform.
 */
//@{

/*!
  \def TE_PLATFORMCODE_MSWINDOWS

  \brief Microsoft Windows platform code.
 */
#define TE_PLATFORMCODE_MSWINDOWS 1

/*!
  \def TE_PLATFORMCODE_LINUX

  \brief Linux platform code.
 */
#define TE_PLATFORMCODE_LINUX 2

/*!
  \def TE_PLATFORMCODE_SOLARIS

  \brief Solaris platform code.
 */
#define TE_PLATFORMCODE_SOLARIS 3

/*!
  \def TE_PLATFORMCODE_SUNOS

  \brief Sun OS platform code.
 */
#define TE_PLATFORMCODE_SUNOS 4

/*!
  \def TE_PLATFORMCODE_APPLE

  \brief Apple platform code.
 */
#define TE_PLATFORMCODE_APPLE 5

/*!
  \def TE_PLATFORMCODE_AIX

  \brief IBM AIX platform code.
 */
#define TE_PLATFORMCODE_AIX 6

/*!
  \def TE_PLATFORMCODE_FREEBSD

  \brief FreeBSD platform code.
 */
#define TE_PLATFORMCODE_FREEBSD 7

/*!
  \def TE_PLATFORMCODE_OPENBSD

  \brief OpenBSD platform code.
 */
#define TE_PLATFORMCODE_OPENBSD 8

/*!
  \def TE_PLATFORM

  \brief It keeps the code of the Operational System platform detected by the build system.
 */
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(_WIN32_WCE)
  #define TE_PLATFORM TE_PLATFORMCODE_MSWINDOWS
#elif defined(__LINUX__) || defined(linux) || defined(__linux__) || defined(__linux)
  #define TE_PLATFORM TE_PLATFORMCODE_LINUX
#elif defined(sparc) || defined(sun) || defined(__sun) 
  #if defined(__SVR4) || defined(__svr4__)
    #define TE_PLATFORM TE_PLATFORMCODE_SOLARIS
  #else
    #define TE_PLATFORM TE_PLATFORMCODE_SUNOS
  #endif
#elif (defined(__APPLE__) && defined(__MACH__)) || defined(macintosh) || defined(Macintosh)
  #define TE_PLATFORM TE_PLATFORMCODE_APPLE
#elif defined(_AIX) || defined(__TOS_AIX__)
  #define TE_PLATFORM TE_PLATFORMCODE_AIX
#elif defined(__FreeBSD__)
  #define TE_PLATFORM TE_PLATFORMCODE_FREEBSD
#elif defined(__OpenBSD__)
  #define TE_PLATFORM TE_PLATFORMCODE_OPENBSD
#else
  #error "ERROR: Unknown platform"
#endif

//@}

/** @name Compiler Platform Detection Macros
 *  Macros for detecting compiler platform.
 */
//@{

/*!
  \def TE_COMPILERCODE_VCPLUSPLUS

  \brief MS Visual Studio C++.
 */
#define TE_COMPILERCODE_VCPLUSPLUS 1

/*!
  \def TE_COMPILERCODE_BCPLUSPLUS

  \brief Borland C++.
 */
#define TE_COMPILERCODE_BCPLUSPLUS 2

/*!
  \def TE_COMPILERCODE_GNUC

  \brief GNU Compiler.
 */
#define TE_COMPILERCODE_GNUC 3

/*!
  \def TE_COMPILERCODE_CODEWARRIOR

  \brief CodeWarrior C++.
 */
#define TE_COMPILERCODE_CODEWARRIOR 4

/*!
  \def TE_COMPILERCODE_SUNFORTE7

  \brief Sun Forte C++.
 */
#define TE_COMPILERCODE_SUNFORTE7 5

/*!
  \def TE_COMPILERCODE_XLC

  \brief AIX xlC C++.
 */
#define TE_COMPILERCODE_XLC 6

/*!
  \def TE_COMPILER

  \brief It keeps the code of the compiler detected by the build system.
 */
#if defined( _MSC_VER )
  #define TE_COMPILER TE_COMPILERCODE_VCPLUSPLUS
#elif defined( __BORLANDC__ ) || defined( __BCPLUSPLUS__ )
  #define TE_COMPILER TE_COMPILERCODE_BCPLUSPLUS
#elif defined( __GNUC__ )
  #define TE_COMPILER TE_COMPILERCODE_GNUC
#elif defined( __MWERKS__ )
  #define TE_COMPILER TE_COMPILERCODE_CODEWARRIOR
#elif defined( __SUNPRO_CC)
  #define TE_COMPILER TE_COMPILERCODE_SUNFORTE7
#elif defined( __xlC__ )
  #define TE_COMPILER TE_COMPILERCODE_XLC
#else
  #error "ERROR: Unknown compiler"
#endif

//@}

/** @name Architecture Detection Macros
 *  Macros for detecting machine architecture.
 */
//@{

//#define TE_32BIT_ARCH 32

//#define TE_64BIT_ARCH 64

//#if defined(_M_IA64) || defined(__ia64) || defined(_IA64) || defined(__amd64) || defined(_M_X64) || defined(__x86_64__)
//  #define TE_XBIT_ARCH TE_64BIT_ARCH
//#else
//  #define TE_XBIT_PLATFORM TE_32BIT_ARCH
//#endif

//@}

/** @name TerraLib Compatibility Macros
 *  Macros for probing TerraLib version.
 */
//@{

/*!
  \def TERRALIB_VERSION_STR

  \brief This flag is used for versioning the TerraLib code. If you have plugins and other tools
         that must check the TerraLib version, you can test against this string.
 */
#define TERRALIB_STR_VERSION "5.0.0 - alpha 1"

/*!
  \def TERRALIB_INT_VERSION

  \brief This flag is used for versioning the TerraLib code. If you have plugins and other tools
         that must check the TerraLib version, you can test against this number.
 */
#define TERRALIB_INT_VERSION 20110425

/*!
  \def TERRALIB_INT_MAJOR_VERSION

  \brief Major version of TerraLib (or version).
 */
#define TERRALIB_INT_MAJOR_VERSION 5

/*!
  \def TERRALIB_INT_MINOR_VERSION

  \brief Minor version of TerraLib (or revision). 
 */
#define TERRALIB_INT_MINOR_VERSION 0

/*!
  \def TERRALIB_INT_PATCH_VERSION

  \brief Patched version of TerraLib (or patch).
 */
#define TERRALIB_INT_PATCH_VERSION 0

//@}

/** @name Platform Compatibility Declarations
 *  Macros for keeping type compatibility across platforms and compilers.
 */
//@{

/*!
  \def TE_XDR

  \brief Macro for big endian (machine byte order).
 */
#define TE_XDR 0

/*!
  \def TE_NDR

  \brief Macro for little endian (machine byte order).
 */
#define TE_NDR 1

/*!
  \def TE_MACHINE_BYTE_ORDER

  \brief This macro defines the machine byte order at build time.

  \note This macro must be set during the build system execution.
 */
#ifndef TE_MACHINE_BYTE_ORDER
#define TE_MACHINE_BYTE_ORDER TE_NDR
#endif

/*!
  \def TE_DOUBLE_NOT_A_NUMBER

  \brief Macro for defining not a number for float values.

  \todo Ver como tornar isso portavel!
 */
#define TE_DOUBLE_NOT_A_NUMBER 1.7e-308

//@}

/*!
  \def TERRALIB_DIR_ENVIRONMENT_VARIABLE

  \brief This macro defines an environment variable that indicates where TerraLib is installed.
 */
#define TERRALIB_DIR_ENVIRONMENT_VARIABLE "TERRALIB_BIN_DIR"

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include "windowsplatformdefines.h"
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX 
  #include "linuxplatformdefines.h"
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  #include "appleplatformdefines.h"
#else
  #error "Platform not supported yet"
#endif

#endif  // __TERRALIB_INTERNAL_CONFIG_H

