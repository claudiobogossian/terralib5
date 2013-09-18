/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/Utils.h

  \brief Utility functions for Vector Processing.  
*/

#ifndef __TERRALIB_VP_QT_INTERNAL_UTILS_H
#define __TERRALIB_VP_QT_INTERNAL_UTILS_H

// Terralib
#include "../geometry/Geometry.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

//STL
#include <map>
#include <string>

namespace te
{
  namespace vp
  {
    /*!
      \brief It returns the union of a geometry vector.

      \param items  Vector of itens that represents a group.

      \return Union of the geometry.
    */
    te::gm::Geometry* GetGeometryUnion(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx);

    std::string GetSimpleTableName(std::string fullName);

    te::gm::GeometryProperty* SetOutputGeometryType(const te::gm::GeometryProperty* firstGeom, const te::gm::GeometryProperty* secondGeom);

    /*!
      \brief It persists the buffer result.

      \param dataSetType  The output DataSetType.
      \param dataSet      The output DataSet.
      \param dsInfo       Information of datasource persistence.
      \param options      A list of optional modifiers. It is driver specific.

      \return The buffer result persistence.
    */
    void Persistence( te::da::DataSetType* dataSetType,
                            te::mem::DataSet* dataSet,
                            const te::da::DataSourceInfoPtr& dsInfo,
                            const std::map<std::string, std::string> options = std::map<std::string, std::string>());

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_QT_INTERNAL_UTILS_H
