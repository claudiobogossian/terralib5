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
  \file Buffer.h
   
  \brief Buffer Vector Processing functions.

  \ingroup vp
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_BUFFER_H
#define __TERRALIB_VP_CORE_INTERNAL_BUFFER_H

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
    \brief Executes the Buffer Geographical Operation and persists the result as a dataset in a given output datasource.

    \param inDatasetName          The name of the  dataset to be used in aggregation operation. It must exists in the input datasource.
    \param inDatasource           Pointer to the datasource that contains the input dataset. Do not pass null.
    \param bufferPolygonRule      The buffer rule for all geometries (enum RuleForBuffer)
    \param bufferBoundariesRule   The buffer boundaries rule for all geometries (enum BoundariesBetweenBuffers)
    \param copyInputColumns       If boudaries between geometries was not dissolved, the user has the option to Copy de columns from the InputLayer.
    \param levels                 The number of levels.
    \param outDatasetName         The name for the resulting datasource. It should not exists in the output datasource.
    \param outDatasource          Pointer to the datasource to persist the resulting dataset. Do not pass null.
    \param fixedDistance          The buffer distance for each geometry with the fixed value.
    \param fromAttDistance        The buffer distance for each geometry is related with a value from one property.

    \return                       True if the it succeeds and false otherwise.

    This algorithm assums that the input dataset exists in the datasource.
     It generates a new dataset in the ouput datasource, do it can not exists a dataset with the same name prior to the calling of this algorithm.
    */
    TEVPEXPORT bool Buffer(const std::string& inDataset,
                          te::da::DataSource* inDatasource,
                          const int& bufferPolygonRule,
                          const int& bufferBoundariesRule,
                          const bool& copyInputColumns,
                          const int& levels,
                          const std::string& outDataset,
                          te::da::DataSource* outDatasource,
                          const double& fixedDistance = 0,
                          const std::string& fromAttDistance = "");

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_BUFFER_H

