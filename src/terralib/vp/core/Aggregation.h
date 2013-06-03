/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Aggregation.h
   
  \brief Aggregation Vector Processing functions.
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H
#define __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

//Terralib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../geometry/Geometry.h"
#include "../../maptools/AbstractLayer.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"

// STL
#include <map>
#include <vector>

namespace te
{
  namespace vp
  {

    void Aggregation(const te::map::AbstractLayerPtr& inputLayer,
                     const std::vector<te::dt::Property*>& groupingProperties,
                     const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType,
                     const te::vp::MemoryUse memoryUse,
                     const std::string& outputLayerName,
                     const te::da::DataSourceInfoPtr& dsInfo);

    te::da::DataSetType* GetDataSetType(const std::string& outputLayerName, 
                                        const std::vector<te::dt::Property*>& properties, 
                                        const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType);

    std::map<std::string, std::vector<te::mem::DataSetItem*> > GetGroups( te::mem::DataSet* inputDataSet,
                                                                          const std::vector<te::dt::Property*>& groupingProperties);

    std::size_t GetPropertyIndex(const te::mem::DataSetItem* item, const std::string propertyName);

    std::string GetGroupingFunctionsTypeMap(const int& type);

    te::gm::Geometry* GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items);

    std::map<std::string, std::string> CalculateStringGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType, 
                                                                        const std::vector<te::mem::DataSetItem*>& items);

    std::map<std::string, double> CalculateDoubleGroupingFunctions(const std::map<te::dt::Property*, std::vector<te::vp::GroupingFunctionsType> >& groupingFunctionsType,
                                                                   const std::vector<te::mem::DataSetItem*>& items);

    bool PropertyExists(const std::string& propertyName, const te::mem::DataSet* dataSet);

    double Sum(const std::vector<double>& values);

    double Mode(const std::vector<double>& values);

    void Persistence( te::da::DataSetType* dataSetType,
                      te::mem::DataSet* dataSet,
                      const te::da::DataSourceInfoPtr& dsInfo,
                      const std::map<std::string, std::string> options = std::map<std::string, std::string>());

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

