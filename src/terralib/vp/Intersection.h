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
 \file IntersectionOp.h
 
 \brief Intersection operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_INTERSECTION_H
#define __TERRALIB_VP_INTERNAL_INTERSECTION_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../geometry/Enums.h"
#include "../memory/DataSet.h"

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
    class TEVPEXPORT Intersection : public Algorithm
    {
    public:

      Intersection();

      virtual ~Intersection() {}

      //bool executeMemory(te::vp::AlgorithmParams* mainParams);
      bool executeMemory(te::vp::AlgorithmParams* mainParams);

      bool executeQuery(te::vp::AlgorithmParams* mainParams);

    protected:

      te::da::DataSet* updateGeomType(te::da::DataSetType* dsType, te::da::DataSet* ds);

      te::da::DataSetType* getOutputDataSetType(te::vp::AlgorithmParams* mainParams);

      te::gm::GeomType getGeomResultType(const te::gm::GeomType& firstGeom, const te::gm::GeomType& secondGeom, const bool& isCollection);

      std::vector<te::dt::Property*> getFirstSelectedProperties(const te::da::DataSetType* dataSetType, const std::map<std::string, te::dt::AbstractData*>& specificParams);

      std::vector<te::dt::Property*> getSecondSelectedProperties(const te::da::DataSetType* dataSetType, const std::map<std::string, te::dt::AbstractData*>& specificParams);

      std::vector<std::pair<std::string, std::string> > getSelectedProperties(const std::map<std::string, te::dt::AbstractData*>& specificParams);

      bool isCollection(const std::map<std::string, te::dt::AbstractData*>& specificParams);

    private:

      std::map<std::string, std::string> m_firstAttrNameMap;

      std::map<std::string, std::string> m_secondAttrNameMap;

    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_INTERSECTION_H