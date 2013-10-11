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
  \file STExamples.h

  \brief Examples on how to load/manipulate st types.
 */

#ifndef __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H
#define __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H

// TerraLib
#include <terralib/common_fw.h>
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>
#include <terralib/st_fw.h>

// STL
#include <vector>
#include <string>

//ST examples
#include "Config.h"

/*! 
  \brief It contains examples about coverage
*/
void CoverageSeriesExamples();

/*! 
  \brief It prints a time series
*/
void PrintTimeSeries(te::st::TimeSeries* ts);

/*! \brief It loads the TerraLib modules. */
void LoadModules();

#endif  // __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H

