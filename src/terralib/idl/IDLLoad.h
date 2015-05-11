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
\file terralib/idl/IDLLoad.h

\brief  IDL module entry point.
*/ 
 
#ifndef __TERRALIB_IDL_INTERNAL_IDLLOAD_H
#define __TERRALIB_IDL_INTERNAL_IDLLOAD_H
  
#include "Config.h"

#include <stdio.h>
#include <idl_export.h>

#ifdef __cplusplus
  extern "C" {
#endif

/*!
  \brief IDL module entry point.

  \details Every loadable module sharable library must export a single symbol 
  called IDL_Load(). This function is called when IDL loads the module, and is expected to do all the work required to load real definitions for the routines supplied by the
  function and prepare the module for use.

  \return It is expected to return True (non-zero) if it was successful, and 
  False (0) if some initialization step failed.
*/
int TEIDLEXPORT IDL_Load(void);

/*!
  \brief IDL exit handler

  \details IDL maintains a list of exit handler functions that it calls as part of its shutdown
operations. These handlers perform actions such as closing files, wrapping up
graphics output, and restoring the user environment to its initial state.
*/
void IDL_ExitHandler(void);

#ifdef __cplusplus
  }; // extern "C"
#endif  

#endif // __TERRALIB_IDL_INTERNAL_IDLLOAD_H
