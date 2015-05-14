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
  \file ado.h

  \brief This file contains include headers for the TerraLib ADO driver.
 */

#ifndef __TERRALIB_INTERNAL_ADO_H
#define __TERRALIB_INTERNAL_ADO_H

// TerraLib
#include "ado/Config.h"
#include "ado/DataSet.h"
#include "ado/DataSetPersistence.h"
#include "ado/DataSetTypePersistence.h"
#include "ado/DataSource.h"
#include "ado/DataSourceCatalogLoader.h"
#include "ado/DataSourceFactory.h"
#include "ado/DataSourceTransactor.h"
#include "ado/Exception.h"
#include "ado/Globals.h"
#include "ado/Module.h"
#include "ado/SQLVisitor.h"
#include "ado/Utils.h"

namespace te
{
  /*! \brief Namespace for the TerraLib ADO driver implementation. */
  namespace ado
  {
  }	// end namespace ado
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_ADO_H

