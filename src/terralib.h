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
  \file terralib.h

  \brief This file contains include headers for all TerraLib modules.
 */

#ifndef __TERRALIB_INTERNAL_TERRALIB_H
#define __TERRALIB_INTERNAL_TERRALIB_H

// TerraLib
#ifdef TE_COMPILE_COLOR_MODULE
#include "terralib/color.h"
#endif

#ifdef TE_COMPILE_COMMON_MODULE
#include "terralib/common.h"
#endif

#ifdef TE_COMPILE_DATAACCESS_MODULE
#include "terralib/dataaccess.h"
#endif

#ifdef TE_COMPILE_FILTER_ENCODING_MODULE
#include "terralib/fe.h"
#endif

#ifdef TE_COMPILE_GEOMETRY_MODULE
#include "terralib/geometry.h"
#endif

#ifdef TE_COMPILE_RASTER_MODULE
#include "terralib/raster.h"
#endif

#ifdef TE_COMPILE_GML_MODULE
#include "terralib/gml.h"
#endif

#ifdef TE_COMPILE_MAPTOOLS_MODULE
#include "terralib/maptools.h"
#endif

#ifdef TE_COMPILE_PLUGIN_MODULE
#include "terralib/plugin.h"
#endif

#ifdef TE_COMPILE_POSTGIS_MODULE
#include "terralib/postgis.h"
#endif

#ifdef TE_COMPILE_PROJ4_MODULE
#include "terralib/proj4.h"
#endif

#ifdef TE_COMPILE_QT_MODULE
#include "terralib/qt.h"
#endif

#ifdef TE_COMPILE_SYMBOLOGY_ENCODING_MODULE
#include "terralib/se.h"
#endif

#ifdef TE_COMPILE_SQLITE_MODULE
#include "terralib/sqlite.h"
#endif

#ifdef TE_COMPILE_SRS_MODULE
#include "terralib/srs.h"
#endif

#ifdef TE_COMPILE_XLINK_MODULE
#include "terralib/xlink.h"
#endif

#ifdef TE_COMPILE_GDAL_MODULE
#include "terralib/gdal.h"
#endif

/*!
  \brief All the TerraLib code is inside this namespace, and each modulo (or package) will add its own namespace.

  The use of namespace has several benefits, and we try to use it carefully.
  The rule used in TerraLib is very simple:
  <ul>
  <li>Each module has its own namespace;</li>
  <li>And all namespaces belong to the main namespace called <b>te</b>.</li>
  </ul>
  We recommend to avoid the C++ <i>using</i> keyword. For example, if you want to use
  a class called Exception located in the Common module, you can write a code like this:
  <br>
  \code
  te::common::Exception e("my exception text goes here...");
  \endcode

  and if you want to call a function, like Convert2String:
  <br>
  \code
  std::string mystring = te::common::Convert2String(67);
  \endcode
 */
namespace te
{
}	// end namespace te

#endif  // __TERRALIB_INTERNAL_TERRALIB_H

