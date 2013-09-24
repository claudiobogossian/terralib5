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
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../datatype/Property.h"
#include "../geometry/Geometry.h"
#include "../maptools/AbstractLayer.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
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
    \brief It receives the required parameters to the buffer process.

    \param inputLayer             The input layer witch is used in buffer operation.
    \param distance               The buffer distance for each geometry.
    \param bufferPolygonRule      The buffer rule for all geometries (enum RuleForBuffer)
    \param bufferBoundariesRule   The buffer boundaries rule for all geometries (enum BoundariesBetweenBuffers)
    \param copyInputColumns       If boudaries between geometries was not dissolved, the user has the option to Copy de columns from the InputLayer.
    \param levels                 
    \param outputLayerName        The name of output layer.
    \param dsInfo                 Information of datasource persistence.

    \return                       The layer result.
    */
    te::map::AbstractLayerPtr Buffer(const te::map::AbstractLayerPtr& inputLayer,
                                    const std::map<te::gm::Geometry*, double>& distance,
                                    const int& bufferPolygonRule,
                                    const int& bufferBoundariesRule,
                                    const bool& copyInputColumns,
                                    const int& levels,
                                    const std::string& outputLayerName,
                                    const te::da::DataSourceInfoPtr& dsInfo);

    te::da::DataSetType* GetDataSetType(const te::map::AbstractLayerPtr& inputLayer,
                                        const std::string& outputLayerName,
                                        const int& bufferBoundariesRule,
                                        const bool& copyInputColumns);
    
    void SetBuffer(te::mem::DataSet* dataSet,
                  const std::map<te::gm::Geometry*, double>& distance,
                  const int& bufferPolygonRule,
                  const int& levels);

    te::mem::DataSet* SetDissolvedBoundaries(te::da::DataSetType* dataSetType, 
                                            te::mem::DataSet* dataset, 
                                            const int& levels);

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

