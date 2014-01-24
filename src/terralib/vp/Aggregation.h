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

  \ingroup vp
*/

#ifndef __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H
#define __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

//Terralib
#include "../datatype/Property.h"
#include "../geometry/Geometry.h"
#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"

// STL
#include <map>
#include <vector>

namespace te
{
  namespace da { class DataSource; }
  
  namespace vp
  {
    /*!
     \brief Executes the Aggregation Geographical Operation and persists the result as a dataset in a given output datasource.
     
     \param inDatasetName          The name of the  dataset to be used in aggregation operation. It must exists in the input datasource.
     \param inDatasource           Pointer to the datasource that contains the input dataset. Do not pass null.
     \param groupingProperties     Selected properties based on selected layer.
     \param statisticalSummary     Map of selected functions based on input dataset properties.
     \param outDataset             The name for the resulting datasource. It should not exists in the output datasource.
     \param outDatasource          Pointer to the datasource to persist the resulting dataset. Do not pass null.
     \param inDataset              The dataset with only the objects that were selected in the input layer.
     
     \return                       True if the it succeeds and false otherwise.
     
     This algorithm assums that the input dataset exists in the datasource.
     It generates a new dataset in the ouput datasource, do it can not exists a dataset with the same name prior to the calling of this algorithm.
     */
    TEVPEXPORT bool Aggregation(const std::string& inDatasetName,
                                te::da::DataSource* inDatasource,
                                const std::vector<te::dt::Property*>& groupingProperties,
                                const std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >& statisticalSummary,
                                const std::string& outDataset,
                                te::da::DataSource* outDatasource,
                                te::da::DataSet* inDataset = 0);

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_VP_CORE_INTERNAL_AGGREGATION_H

