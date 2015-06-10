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
  \file MapToolsExample.h

  \brief Several examples declarations.
 */

#ifndef __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H
#define __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H

// TerraLib Examples
#include "../Config.h"

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/se_fw.h>

// STL
#include <string>

// Global Id
extern unsigned int G_ID;

/*! \brief It loads the data source drivers. */
void LoadModules();

/*! \brief It retrieves data from a data source, create a set of layer and show map display. */
void MapDisplay();

/*! \brief It converts a OGC Filter Expression to TerraLib Expression. */
void Filter2Query();

/*! \brief It draws a set of layers using styles encoded using OGC Symbology Enconding specification. */
void DrawStyledLayers();

/*! \brief It draws a set of raster layers using styles encoded using OGC RasterSymbology Enconding specification. */
void DrawRasterStyledLayers();

/** @name Hard-coded styles
 *  Methods to build pre-defined styles.
*/
//@{

/* Polygon Styles */
te::se::Style* SimplePolygonStyle();
te::se::Style* SimplePolygonStyleWithStroke();
te::se::Style* TransparentPolygonStyle();
te::se::Style* GraphicFillPolygonStyle(const std::string& markName);
te::se::Style* TreePolygonStyle();
te::se::Style* GraphicFillContourPolygonStyle(const std::string& markName);

/* Line Styles */
te::se::Style* SimpleLineStyle();
te::se::Style* DashedLineStyle();
te::se::Style* LineWithBorderStyle();
te::se::Style* RoadStyleOne();
te::se::Style* RoadStyleTwo();

/* Point Styles */
te::se::Style* MarkPointStyle(const std::string& markName);

//@}

#endif  // __TERRALIB_EXAMPLES_MAPTOOLS_INTERNAL_MAPTOOLSEXAMPLES_H
