/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file dataaccess.h

  \brief This file contains include headers for the Data Access module of TerraLib.
 */

#ifndef __TERRALIB_INTERNAL_DATAACCESS_H
#define __TERRALIB_INTERNAL_DATAACCESS_H

// TerraLib

// dataaccess
#include "dataaccess/Config.h"
#include "dataaccess/Exception.h"
#include "dataaccess/dataset_h.h"
#include "dataaccess/datasource_h.h"
#include "dataaccess/query_h.h"
#include "dataaccess/utils/Utils.h"

namespace te
{
  /*!
    \brief Namespace for the DataAccess API of TerraLib.

    TerraLib provides a basic framework for accessing data.
    It is designed towards data interoperability. You can
    write your own driver for accessing a different data source.
    Of course, we provide some basic data sources: PostGIS, Oracle Spatial,
    Shapefiles, WFS, WMS and WCS.
   */
  namespace da
  {
  }	// end namespace da
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_DATAACCESS_H

