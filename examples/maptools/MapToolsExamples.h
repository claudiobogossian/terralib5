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
  \file MapToolsExample.h

  \brief Several examples declarations.
 */

#ifndef __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H
#define __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H

// TerraLib
#include <terralib/dataaccess_fw.h>

/*!
  \brief Flags for TerraLib MapTools Examples.
         The set of flags below controls which data source driver will be loaded.
         If you want to disable a specific data source driver just just set the flag to 0.
*/
#define TE_EXAMPLE_USE_OGR  1
#define TE_EXAMPLE_USE_GDAL 0
#define TE_EXAMPLE_USE_PGIS 0

/*! \brief It loads the data source drivers. */
void LoadModules();

/*! \brief It retrieves data from a data source, create a set of layer and show map display. */
void MapDisplay();

/*! \brief It converts a OGC Filter Expression to TerraLib Query. */
void Filter2Query();

#endif  // __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H

