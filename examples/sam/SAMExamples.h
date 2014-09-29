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
  \file SAMExamples.h

  \brief Several examples on how to use Spatial Access Methods in TerraLib.
 */

#ifndef __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H
#define __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H

/*! \brief This example shows how to index a set of points using the R-tree spatial access method. */
void IndexPointUsingRTree();

/*! \brief This example shows how to index a set of points using the K-d tree spatial access method. */
void IndexPointUsingKdTree();

#endif  // __TERRALIB_EXAMPLES_SAM_INTERNAL_SAMEXAMPLES_H

