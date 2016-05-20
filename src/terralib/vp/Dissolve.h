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
 \file Dissolve.h
 
 \brief Dissolve operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_VP_INTERNAL_DISSOLVE_H
#define __TERRALIB_VP_INTERNAL_DISSOLVE_H

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
    std::vector<std::string> GetDissolveProps(const std::map<std::string, te::dt::AbstractData*>& specificParams);

    std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > GetSummaryProps(const std::map<std::string, te::dt::AbstractData*>& specificParams);

    bool IsCollection(const std::map<std::string, te::dt::AbstractData*>& specificParams);

    te::da::DataSetType* GetOutputDataSetType(te::vp::AlgorithmParams* mainParams);

    te::gm::GeomType SetGeomResultType(const te::gm::GeomType& geomType, const bool& isCollection);

    std::vector<te::gm::Geometry*> ExtractGeometry(te::gm::Geometry* inputGeometry, te::gm::GeomType outputGeomType);

    void PopulateItens(te::da::DataSetType* inputDataSetType, std::vector<te::mem::DataSetItem*> inputItens, std::map<std::string, te::dt::AbstractData*> specificParams, std::vector<te::mem::DataSetItem*> outputItemVec);


    class GroupThreadManager;

    class TEVPEXPORT Dissolve : public Algorithm
    {
    public:

      Dissolve();

      virtual ~Dissolve() {}

      bool executeMemory(te::vp::AlgorithmParams* mainParams);

      bool executeQuery(te::vp::AlgorithmParams* mainParams);

      static void threadUnion(GroupThreadManager* manager);

      static void threadSave(GroupThreadManager* manager);

    private:

      typedef te::sam::rtree::Index<size_t, 8>* DataSetRTree;

    };
  }
}
#endif // __TERRALIB_VP_INTERNAL_DISSOLVE_H