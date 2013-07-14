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
  \file SummaryFunctions.h
   
  \brief Statistical summary functions.
*/

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H

//Terralib
#include "../../dataaccess2/dataset/DataSet.h"
#include "../../datatype/Property.h"
#include "../../maptools2/AbstractLayer.h"
#include "Config.h"
#include "Enums.h"
#include "NumericStatisticalSummary.h"
#include "StringStatisticalSummary.h"

// STL
#include <map>
#include <vector>

namespace te
{
  namespace stat
  {
//memory
    TESTATEXPORT void GetStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss);

    TESTATEXPORT void GetNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss);

    TESTATEXPORT double Mode(const std::vector<double>& values);

    TESTATEXPORT std::string Mode(const std::vector<std::string>& values);

//dataaccess
    TESTATEXPORT void GetStringStatisticalSummaryQuery( const te::map::AbstractLayerPtr& layer, 
                                                        const te::dt::Property* prop,
                                                        te::stat::StringStatisticalSummary& ss);

    TESTATEXPORT void GetNumericStatisticalSummaryQuery(const te::map::AbstractLayerPtr& layer,
                                                        const te::dt::Property* prop,
                                                        te::stat::NumericStatisticalSummary& ss);

  } // end namespace stat
}   // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H

