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
  \file terralib/statistics/core/Utils.h

  \brief Utility functions for Statistics.
*/

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_UTILS_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_UTILS_H

// Terralib
#include "../../maptools/DataSetLayer.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <list>
#include <string>
#include <vector>

namespace te
{
  namespace stat
  {
    /*!
      \brief Get the statistical parameter short name from its enumerator.

      \param e   The enumerator.

      \return The short name for the statistical parameter.
    */
    TESTATEXPORT std::string GetStatSummaryShortName(const int& e);

    /*!
      \brief Get the statistical parameter full name ffrom its enumerator.

      \param e   The enumerator.

      \return The statistical parameter full name of enum.
    */
    TESTATEXPORT std::string GetStatSummaryFullName(const int& e);

    /*!
     \brief Returns the values of a string type property in a vector of values.
     
     It returns only the non-empty values.
     
     \param dataSet   A pointer to the dataset. Do not pass null.
     \param propName  The propoerty name. Do not pass empty.
     
     \return The vector of string values.
     */
    TESTATEXPORT std::vector<std::string> GetStringData(te::da::DataSet* dataSet, const std::string propName);
    
    /*!
     \brief Returns the values of a numeric type property in a vector of values.
     
     It returns only the non-empty values.
     
     \param dataSet   A pointer to the dataset. Do not pass null.
     \param propName  The propoerty name. Do not pass empty.
     
     \return The vector of numeric values (double).
     */
    TESTATEXPORT std::vector<double> GetNumericData(te::da::DataSet* dataSet, const std::string propName);

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_UTILS_H
