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
  \file postgis.h

  \brief This file contains include headers for the TerraLib PostGIS driver.
 */

#ifndef __TERRALIB_INTERNAL_POSTGIS_H
#define __TERRALIB_INTERNAL_POSTGIS_H

// TerraLib
#include "postgis/Config.h"
#include "postgis/ConnectionPool.h"
#include "postgis/DataSet.h"
#include "postgis/DataSource.h"
#include "postgis/DataSourceFactory.h"
#include "postgis/DataTypes.h"
#include "postgis/EWKBReader.h"
#include "postgis/EWKBWriter.h"
#include "postgis/Module.h"
#include "postgis/SQLVisitor.h"
#include "postgis/Transactor.h"
#include "postgis/Utils.h"

namespace te
{
  /*! \brief Namespace for the TerraLib PostGIS driver implementation. */
  namespace pgis
  {
  }	// end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_POSTGIS_H

