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
  \file terralib/classification/Utils.h

  \brief Utility functions for Classification.
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_UTILS_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_UTILS_H

// TerraLib
#include "../maptools/DataSetLayer.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <list>
#include <string>
#include <vector>

namespace te
{
  namespace cl
  {
    /*!
      \brief Computes euclidean distance between two double vectors.

      \param v1    The first vector.
      \param v2    The second vector.

      \return The euclidean distance between v1 and v2.
    */
    TECLEXPORT double GetEuclideanDistance(std::vector<double> v1, std::vector<double> v2);

  }  // end namespace cl
}    // end namespace te

#endif  // __TERRALIB_CLASSIFICATION_INTERNAL_UTILS_H
