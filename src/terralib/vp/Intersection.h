/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/core/IntersectionDialog.h

  \brief Group of functions to intersection operation

  \ingroup vp
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H
#define __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H

// TerraLib
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../maptools/AbstractLayer.h"
#include "../sam/rtree.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace vp
  {
    TEVPEXPORT bool Intersection(const std::string& inFirstDataSetName,
                                te::da::DataSource* inFirstDataSource,
                                const std::string& inSecondDataSetName,
                                te::da::DataSource* inSecondDataSource,
                                const bool& copyInputColumns,
                                const std::string& outDataSetName,
                                te::da::DataSource* outDataSource);
  }
}

#endif // __TERRALIB_VP_CORE_INTERNAL_INTERSECTION_H
