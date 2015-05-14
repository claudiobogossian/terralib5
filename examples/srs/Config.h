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
  \file Config.h

  \brief Global configuration flags for the TerraLib SRS Examples.
 */

#ifndef __TERRALIB_EXAMPLES_SRS_INTERNAL_CONFIG_H
#define __TERRALIB_EXAMPLES_SRS_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/*!
  \def TE_EXAMPLE_USE_GDAL, TE_EXAMPLE_USE_OGR, TE_EXAMPLE_USE_PGIS and eventually other defines to specify another supported driver.

  \brief Flags for TerraLib SRS Examples.
         The set of flags below controls which data source driver examples will be executed.
         If you want to disable the examples for a specific data source driver
         just set the flag to 0 (this will turn of the test).
*/

#if TE_USE_OGR
#define TE_EXAMPLE_USE_OGR 1
#endif

#if TE_USE_PROJ4
#define TE_EXAMPLE_USE_PROJ4 1
#endif

#endif  // __TERRALIB_EXAMPLES_SRS_INTERNAL_CONFIG_H
