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
  \file CanvasExamples.h

  \brief Several examples declarations.
 */

#ifndef __TERRALIB_EXAMPLES_QT_CANVAS_INTERNAL_CANVAS_H
#define __TERRALIB_EXAMPLES_QT_CANVAS_INTERNAL_CANVAS_H

#include "../../Config.h"

#define TE_EXAMPLE_USE_OGR 1

#define TE_EXAMPLE_USE_GDAL 1

/*! \brief It loads the data source drivers. */
void LoadModules();

/*! \brief It retrieves data from a Shape-file and prints it to a png graphics file. */
void DrawingGeometricObjects();

/*! \brief It retrieves data from a Shape-file and prints it to a png graphics file. */
void DrawPNG();

/*! \brief It retrieves data from a tiff file and prints it to a png graphics file. */
void DrawingRasterObject();

#endif  // __TERRALIB_EXAMPLES_QT_CANVAS_INTERNAL_CANVAS_H

