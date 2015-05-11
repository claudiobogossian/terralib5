/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

/*!
  \defgroup dataaccess Data Access

  \brief The data access module provides a basic framework for accessing data.

  It is designed towards data interoperability. You can
  write your own driver for accessing a different data source.
  Of course, we provide some basic data sources drivers.

  The Data Access module provides the fundamental layer for applications
  that handle spatial data from different sources, ranging from traditional DBMSs to OGC Web Services.

  This module is composed by some base abstract classes that must be extended to allow the creation
  of Data Access Drivers which actually implement all the details needed to access
  data in a specific format or system.

  This module provides the base foundation for an application discover what is stored 
  in a data source and how the data is organized in it. Keep in mind that this organization is the
  low-level organization of the data. For instance, in a DBMS, you can find out the tables stored 
  in a database and the relationships between them or detailed informations about 
  their columns (data type, name and constraints).

  It is not the role of this module to provide higher-level metadata about the data stored in a data source.

  The main classes/concepts in this module are listed here. The namespace associated to the Data Access module is te::da.
  To know more about it, see the te::da namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the DataAccess API of TerraLib.
  */
  namespace da
  {
  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_DATAACCESS_H
