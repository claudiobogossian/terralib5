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
      \brief It receives the required parameters to the aggregation process.

      \param inputLayer             The input layer witch is used in aggregation operation.
      \param groupingProperties     Selected properties based on selected layer.
      \param statisticalSummary     Map of selected functions based on layer properties.
      \param outputLayerName        The name of output layer.
      \param dsInfo                 Information of datasource persistence.

      \return                       The aggregation result.
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

      \return The output DataSetType.
    */
    te::da::DataSetType* GetDataSetType(const std::string& outputLayerName, 
                                        const std::vector<te::dt::Property*>& properties, 
                                        const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary);

     /*!
      \brief This function is called when it is able to process the aggregation through a database using query.

      \param inputLayer             The input layer witch is used in aggregation operation.
      \param groupingProperties     Selected properties based on selected layer.
      \param statisticalSummary     Map of selected functions based on layer properties.
      \param outputDataSet          The result of the aggregation using query.

    */
    void AggregationQuery(  const te::map::AbstractLayerPtr& inputLayer,
                            const std::vector<te::dt::Property*>& groupingProperties,
                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                            te::mem::DataSet* outputDataSet);

     /*!
      \brief It takes the result of query and set in a new dataset, already prepared to receive these data.

      \param groupingProperties     Selected properties based on selected layer.
      \param dsQuery                The result of query.
      \param outputDataSet          The output of the aggregation using query.

    */
    void SetOutputDatasetQuery( const std::vector<te::dt::Property*>& groupingProperties,
                                te::da::DataSet* dsQuery,
                                te::mem::DataSet* outputDataSet);

     /*!
      \brief This function is called when it is not able to process the aggregation through a database using query, then the aggregation is processed in memory.

      \param inputLayer             The input layer witch is used in aggregation operation.
      \param groupingProperties     Selected properties based on selected layer.
      \param statisticalSummary     Map of selected functions based on layer properties.
      \param outputDataSet          The result of the aggregation using memory.

    */
    void AggregationMemory( const te::map::AbstractLayerPtr& inputLayer,
                            const std::vector<te::dt::Property*>& groupingProperties,
                            const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                            te::mem::DataSet* outputDataSet);

    /*!
      \brief It returns a map with a key group and items related to it.

      \param inputDataSet         The input DataSet.
      \param groupingProperties   Selected properties based on selected layer.

      \return The aggregation groups.
    */
    std::map<std::string, std::vector<te::mem::DataSetItem*> > GetGroups( te::mem::DataSet* inputDataSet,
                                                                          const std::vector<te::dt::Property*>& groupingProperties);

    /*!
      \brief It returns the result of statistics for string properties.

      \param statisticalSummary     A map of Property as Key and related functions.
      \param items                  Itens that represents a group.

      \return The result of statistics.
    */
    std::map<std::string, std::string> CalculateStringGroupingFunctions(  const te::map::AbstractLayerPtr& inputLayer,
                                                                          const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary, 
                                                                          const std::vector<te::mem::DataSetItem*>& items);

    /*!
      \brief It returns the result of statistics for double properties.

      \param statisticalSummary  A map of Property as Key and related functions.
      \param items                  Itens that represents a group.

      \return The result of statistics.
    */
    std::map<std::string, double> CalculateDoubleGroupingFunctions( const te::map::AbstractLayerPtr& inputLayer,
                                                                    const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                                                    const std::vector<te::mem::DataSetItem*>& items);

    /*!
      \brief It persists the aggregation result.

      \param dataSetType  The output DataSetType.
      \param dataSet      The output DataSet.
      \param dsInfo       Information of datasource persistence.
      \param options      A list of optional modifiers. It is driver specific.

      \return The aggregation result persistence.
    */
    void Persistence( te::da::DataSetType* dataSetType,
                      te::mem::DataSet* dataSet,
                      const te::da::DataSourceInfoPtr& dsInfo,
                      const std::map<std::string, std::string> options = std::map<std::string, std::string>());

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

