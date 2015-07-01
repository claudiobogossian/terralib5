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
  \file SRSExamples.h

  \brief Several examples on how to use Spatial Reference Systens handling in TerraLib.
 */

#ifndef __TERRALIB_EXAMPLES_SRS_INTERNAL_SRSEXAMPLES_H
#define __TERRALIB_EXAMPLES_SRS_INTERNAL_SRSEXAMPLES_H

#include "Config.h"

/*! \brief This example shows how to use the Spatial Reference Systems Manager. */
void SpatialReferenceSystemManager();

/*! \brief This example shows how Coordinate Systems from identifiers are recognized by the converter. */
void RecognizeSRIDs();

/*! \brief This example shows how to transform a geometry from an SRS to another. */
void ConvertCoordinates();

#endif  // __TERRALIB_EXAMPLES_SRS_INTERNAL_SRSEXAMPLES_H

