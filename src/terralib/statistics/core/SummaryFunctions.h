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
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace stat
  {
    /*!
      \brief It returns the mode of the values.

      \param values   Vector of values.

      \output The mode of values.
    */
    double Mode(const std::vector<double>& values);

  } // end namespace stat
}   // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_SUMMARYFUNCTIONS_H

