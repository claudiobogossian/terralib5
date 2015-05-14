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

  \brief Global configuration flags for the TerraLib QT Query Builder Examples.
 */

#ifndef __TERRALIB_EXAMPLES_QT_QUERY_INTERNAL_CONFIG_H
#define __TERRALIB_EXAMPLES_QT_QUERY_INTERNAL_CONFIG_H

#include "../../Config.h"

#if TE_USE_GDAL
#define TE_EXAMPLE_USE_GDAL 0
#endif

#if TE_USE_OGR
#define TE_EXAMPLE_USE_OGR 1
#endif

#if TE_USE_GRIB
#define TE_EXAMPLE_USE_GRIB 0
#endif

#if TE_USE_PGIS
#define TE_EXAMPLE_USE_PGIS 1
#endif

#define TE_EXAMPLE_USE_MEMORY 0

#endif //__TERRALIB_EXAMPLES_QT_QUERY_INTERNAL_CONFIG_H
