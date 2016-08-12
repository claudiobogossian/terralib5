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

  \brief Configuration flags for TerraLib Unittest DataAccess drivers .
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_CONFIG_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/*!
  \def TE_TEST_GDAL, TE_TEST_OGR, TE_TEST_PGIS and other dataaccess drivers

  \brief Flags for TerraLib Unittest Dataaccess drivers.
         The set of flags below controls which data source driver Unittest will be executed.
         If you want to disable the Unittest for a specific data source driver
         just set the flag to 0 (this will turn of the test).
 */

#if TE_USE_GDAL
#define TE_TEST_GDAL 1
#endif

#if TE_USE_OGR
#define TE_TEST_OGR 0
#endif

#if TE_USE_PGIS
#define TE_TEST_PGIS 1
#endif


#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_CONFIG_H


