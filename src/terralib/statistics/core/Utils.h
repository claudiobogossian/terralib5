/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
      \brief Get the StatisticalSummary short name filtered by enum;

      \param e   The enumerator.

      \return The short name of enum.
    */
    TESTATEXPORT std::string GetStatSummaryShortName(const int& e);

    /*!
      \brief Get the StatisticalSummary full name filtered by enum;

      \param e   The enumerator.

      \return The full name of enum.
    */
    TESTATEXPORT std::string GetStatSummaryFullName(const int& e);


    TESTATEXPORT int GetPropertyIndex(te::da::DataSet* dataSet, const std::string propName);

    TESTATEXPORT std::vector<std::string> GetStringData(te::da::DataSet* dataSet, const std::string propName);

    TESTATEXPORT std::vector<double> GetNumericData(te::da::DataSet* dataSet, const std::string propName);

  } // end namespace vp
}   // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_UTILS_H
