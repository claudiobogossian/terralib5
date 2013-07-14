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
     /*!
      \brief It groups the values defined by a range of iterators using the standard deviation algorithm.

      \param inputLayer             The input layer witch is used in aggregation operation.
      \param groupingProperties     Selected properties based on selected layer.
      \param statisticalSummary  Map of selected functions based on layer properties.
      \param memoryUse              The memory use while processing.
      \param outputLayerName        The name of output layer.
      \param dsInfo                 Information of datasource persistence.

      \output                     The aggregation result.
    */
    void Aggregation(const te::map::AbstractLayerPtr& inputLayer,
                     const std::vector<te::dt::Property*>& groupingProperties,
                     const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                     const std::string& outputLayerName,
                     const te::da::DataSourceInfoPtr& dsInfo);

    /*!
      \brief It returns the output DataSetType.

      \param outputLayerName        The name of output layer.
      \param properties             Selected properties based on selected layer.
      \param statisticalSummary     Map of selected functions based on layer properties.

      \output The output DataSetType.
    */
    te::da::DataSetType* GetDataSetType(const std::string& outputLayerName, 
                                        const std::vector<te::dt::Property*>& properties, 
                                        const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary);


    void AggregationQuery(  const te::map::AbstractLayerPtr& inputLayer,
                            const std::vector<te::dt::Property*>& groupingProperties,
                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                            te::mem::DataSet* outputDataSet);

    void SetOutputDatasetQuery( const std::vector<te::dt::Property*>& groupingProperties,
                                te::da::DataSet* dsQuery,
                                te::mem::DataSet* outputDataSet);

    void AggregationMemory( const te::map::AbstractLayerPtr& inputLayer,
                            const std::vector<te::dt::Property*>& groupingProperties,
                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                            te::mem::DataSet* outputDataSet);

    /*!
      \brief It returns a map with a key group and items related to it.

      \param inputDataSet         The input DataSet.
      \param groupingProperties   Selected properties based on selected layer.

      \output The aggregation groups.
    */
    std::map<std::string, std::vector<te::mem::DataSetItem*> > GetGroups( te::mem::DataSet* inputDataSet,
                                                                          const std::vector<te::dt::Property*>& groupingProperties);
    
    /*!
      \brief It returns the index of a property.

      \param item           The DataSetItem.
      \param propertyName   The property name.

      \output The property index.
    */
    std::size_t GetPropertyIndex(const te::mem::DataSetItem* item, const std::string propertyName);

    /*!
      \brief It returns the union of a geometry vector.

      \param items  Vector of itens that represents a group.

      \output Union of the geometry.
    */
    te::gm::Geometry* GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items);

    /*!
      \brief It returns the result of statistics for string properties.

      \param statisticalSummary  A map of Property as Key and related functions.
      \param items                  Itens that represents a group.

      \output The result of statistics.
    */
    std::map<std::string, std::string> CalculateStringGroupingFunctions(  const te::map::AbstractLayerPtr& inputLayer,
                                                                          const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary, 
                                                                          const std::vector<te::mem::DataSetItem*>& items);

    /*!
      \brief It returns the result of statistics for double properties.

      \param statisticalSummary  A map of Property as Key and related functions.
      \param items                  Itens that represents a group.

      \output The result of statistics.
    */
    std::map<std::string, double> CalculateDoubleGroupingFunctions( const te::map::AbstractLayerPtr& inputLayer,
                                                                    const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                                    const std::vector<te::mem::DataSetItem*>& items);
    
    /*!
      \brief It verify if the property exists.

      \param propertyName   The property name.
      \param dataSet        The output DataSet.

      \output A bool value.
    */
    bool PropertyExists(const std::string& propertyName, const te::mem::DataSet* dataSet);

    /*!
      \brief It persists the aggregation result.

      \param dataSetType  The output DataSetType.
      \param dataSet      The output DataSet.
      \param dsInfo       Information of datasource persistence.
      \param options      A list of optional modifiers. It is driver specific.

      \output The aggregation result persistence.
    */
    void Persistence( te::da::DataSetType* dataSetType,
                      te::mem::DataSet* dataSet,
                      const te::da::DataSourceInfoPtr& dsInfo,
                      const std::map<std::string, std::string> options = std::map<std::string, std::string>());

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

