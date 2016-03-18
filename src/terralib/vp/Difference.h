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
 \file Difference.h
 
 \brief Difference operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_DIFFERENCE_H
#define __TERRALIB_VP_INTERNAL_DIFFERENCE_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../geometry/Enums.h"
#include "../memory/DataSet.h"
#include "../sam.h"

#include "Algorithm.h"
#include "AlgorithmParams.h"
#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace vp
  {
    class TEVPEXPORT Difference : public Algorithm
    {
    public:

      Difference();

      virtual ~Difference() {}

      bool getSpecificParams();

      void setSpecificParams(bool copyInputColumns);

      bool executeMemory(te::vp::AlgorithmParams* mainParams);

      bool executeQuery(te::vp::AlgorithmParams* mainParams);

      bool persistsDataSet();

      bool validMemoryParams(te::vp::AlgorithmParams* mainParams);

      bool validQueryParams(te::vp::AlgorithmParams* mainParams);

    protected:
      
      te::da::DataSet* updateGeomType(te::da::DataSetType* dsType, te::da::DataSet* ds);

      std::vector<std::string> getPropNames(const std::map<std::string, te::dt::AbstractData*>& specificParams);

      te::da::DataSetType* getOutputDataSetType(te::vp::AlgorithmParams* mainParams);

      te::gm::GeomType setGeomResultType(te::gm::GeomType firstGeom);

      te::gm::Geometry* setGeometryType(te::gm::Geometry* geom);
      
      // it defines the type of the result considering the input geometries being aggregated
      //te::gm::GeomType getGeomResultType(te::gm::GeomType geom);

      bool m_copyInputColumns;
      std::size_t m_SRID;

    private:
        
      typedef te::sam::rtree::Index<size_t, 8>* DataSetRTree;

    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_DIFFERENCE_H