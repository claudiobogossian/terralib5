/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/ModuleUtils.h

  \brief Utility functions for module definition.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_MODULEUTILS_H
#define __TERRALIB_COMMON_INTERNAL_MODULEUTILS_H

//#if !TE_TERRALIB_STATIC_BUILD
//  #if TE_COMPILER == TE_COMPILERCODE_VCPLUSPLUS
//    #include <windows.h>
//
//    #define TE_REGISTER_MODULE(moduleName, startupFunction, shutdownFunction) \
//              BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/,  DWORD fdwReason, LPVOID /*lpReserved*/)  \
//              {                                   \
//                switch(fdwReason)                 \
//                {                                 \
//                  case DLL_PROCESS_ATTACH:        \
//                    startupFunction();            \
//                  break;                          \
//                                                  \
//                  case DLL_THREAD_ATTACH:         \
//                  break;                          \
//                                                  \
//                  case DLL_THREAD_DETACH:         \
//                  break;                          \
//                                                  \
//                  case DLL_PROCESS_DETACH:        \
//                    shutdownFunction();           \
//                  break;                          \
//                }                                 \
//                                                  \
//                return TRUE;                      \
//              }
//  #elif TE_COMPILER == TE_COMPILERCODE_GNUC
//      #define TE_REGISTER_MODULE(moduleName, startupFunction, shutdownFunction) \
//        void __attribute__ ((constructor)) TeInitialize ## moduleName(void) \
//        {                     \
//          startupFunction();  \
//        }                     \
//                              \
//        void __attribute__ ((destructor)) TeFinalize ## moduleName(void) \
//        {                     \
//          shutdownFunction(); \
//        }
//  #else
//    #define TE_REGISTER_MODULE(moduleName, startupFunction, shutdownFunction)
//  #endif
//
//#else
//  #define TE_REGISTER_MODULE(moduleName, startupFunction, shutdownFunction)
//#endif  // TE_TERRALIB_STATIC_BUILD

#endif  // __TERRALIB_COMMON_INTERNAL_MODULEUTILS_H

