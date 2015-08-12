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
 \file SummaryFunctions.h
 
 \brief A set of functions to calculate the statistic summary from a set of values.
 */

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H

//Terralib
#include "Config.h"
#include "Enums.h"
#include "NumericStatisticalSummary.h"
#include "StringStatisticalSummary.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace da { class DataSource; }
  
  namespace stat
  {
    
    /*! Fills the summary statistics structure from a set of categorical values.
     
     \param values a vector of categorical values. Do not pass empty.
     \param ss     to return the statistical summary.
     */
    TESTATEXPORT void GetStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss);
    
    /*! Fills the summary statistics structure from a set of categorical values.
     
     \param values a vector of categorical values. Do not pass empty.
     \param ss     to return the statistical summary.
     \param nullVal a value to be considered as non-value or invalid value.
     */
    TESTATEXPORT void GetStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss, const std::string& nullValue);
    
    /*! Fills the summary statistics structure from a set of numerical values.
     
     \param values  a vector of numerical values. Do not pass empty.
     \param ss      to return the statistical summary.
     \param nullVal a value to be considered as non-value or invalid value.
     */
    TESTATEXPORT void GetNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss, double nullVal);
    
    /*! Fills the summary statistics structure from a set of numerical values.
     
     \param values  a vector of numerical values. Do not pass empty.
     \param ss      to return the statistical summary.
     */
    TESTATEXPORT void GetNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss);

    /*! Fills the percentage of each class by area.

    \param values       a vector of numerical values, representing pixels from a raster. Do not pass empty.
    \param resolutionX  a raster horizontal (x-axis) resolution.
    \param resolutionY  a raster vertical (y-axis) resolution.
    \param area         area of a geometry that intersects the raster.
    \param ss           to return the percentage of each class by area.
    
    */
    TESTATEXPORT void GetPercentOfEachClassByArea(std::vector<double>& values,
                                                  double& resolutionX,
                                                  double& resolutionY,
                                                  double& area,
                                                  te::stat::NumericStatisticalSummary& ss);
    
    /*! Returns the mode of set of numerical values.
     
     \param values  a reference to a set of numerical values. Do not pass empty.
     \return The mode (most frequent value).
     */
    TESTATEXPORT std::vector<double> Mode(const std::vector<double>& values);
    
    /*! Returns the mode of set of categorical values.
     
     \param values  a reference to a set of categorical values;
     \return The mode (most frequent value).
     */
    TESTATEXPORT std::string Mode(const std::vector<std::string>& values);
    
    
    /*! Fills the summary statistics structure of a given categorical property from a dataset.
     
     \param inDataset  a dataset name.
     \param inDatasource a pointer to a datasource. Do not pass null.
     \param propName the name of the property to be summarized.
     \param ss     to return the statistical summary.
     */
    TESTATEXPORT void GetStringStatisticalSummaryQuery(const std::string& inDataset,
                                                       te::da::DataSource* inDatasource,
                                                       const std::string& propName,
                                                       te::stat::StringStatisticalSummary& ss);
    
    /*! Fills the summary statistics structure of a given numerical property from a dataset.
     
     \param inDataset  a dataset name.
     \param inDatasource a pointer to a datasource. Do not pass null.
     \param propName the name of the property to be summarized.
     \param ss     to return the statistical summary.
     */
    TESTATEXPORT void GetNumericStatisticalSummaryQuery(const std::string& inDataset,
                                                        te::da::DataSource* inDatasource,
                                                        const std::string& propName,
                                                        te::stat::NumericStatisticalSummary& ss);
    
  } // end namespace stat
}   // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H

