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
  \file SAMExamples.h

  \brief Several examples on how to use Spatial Access Methods in TerraLib.
 */

#ifndef __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H
#define __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H

//#define TE_EXAMPLE_USE_GDAL
#define TE_EXAMPLE_USE_OGR
//#define TE_EXAMPLE_USE_PGIS
//#define TE_EXAMPLE_USE_T3
//#define TE_EXAMPLE_USE_SQLITE
//#define TE_EXAMPLE_USE_QTSQL
//#define TE_EXAMPLE_USE_SPRING

#define TE_DATA_LOCALE "."

/*! \brief This example shows how to index a set of points using the R-tree spatial access method. */
void IndexPointUsingRTree();

/*! \brief It loads the data source drivers. */
void LoadModules();

#endif  // __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H

