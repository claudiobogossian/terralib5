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
  \file terralib/vp/qt/Utils.h

  \brief Utility functions for Vector Processing.  
*/

#ifndef __TERRALIB_VP_INTERNAL_UTILS_H
#define __TERRALIB_VP_INTERNAL_UTILS_H

// Terralib
#include "../geometry/Geometry.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../sam/rtree.h"
#include "Config.h"

//STL
#include <map>
#include <string>

namespace te
{
  namespace vp
  {
    class AlgorithmParams;

    enum Strategy
    {
      MEMORY,
      QUERY
    };

    /*!
      \brief It returns the union of a geometry vector.

      \param items  Vector of itens that represents a group.

      \return Union of the geometry.
    */
    te::gm::Geometry* GetGeometryUnion(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx, te::gm::GeomType outGeoType);

    te::gm::Geometry* GetGeometryUnion(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx);

    void SplitGeometryCollection(te::gm::GeometryCollection* geomIn, te::gm::GeometryCollection* gcOut);

    bool VerifyGeometryRepresentation(te::gm::Geometry* geom, te::gm::GeomType type);

    std::string GetSimpleTableName(std::string fullName);

    std::auto_ptr<te::da::DataSet> PrepareAdd(te::da::DataSet* ds, te::da::DataSetType* dt);

    void Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType);

    bool IsMultiType(te::gm::GeomType geomType);

    te::gm::GeomType GetSimpleType(te::gm::GeomType geomType);
    
    te::gm::GeomType GetMultiType(te::gm::GeomType geomType);

    TEVPEXPORT te::da::DataSourcePtr CreateOGRDataSource(std::string repository);

    void ValidateAlgorithmParams(AlgorithmParams* mainParams, Strategy st);

    te::sam::rtree::Index<size_t, 8>* GetRtree(te::da::DataSet* data);

    te::gm::Geometry* SetGeomAsMulti(te::gm::Geometry* geom);

    std::string GetDistinctName(const std::string& name, std::vector<std::string> names, std::size_t maxSize = 0);

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_INTERNAL_UTILS_H
