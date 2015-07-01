/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file maptools_fw.h
   
  \brief Forward declarations for the MapTools module.
 */

#ifndef __TERRALIB_FW_INTERNAL_MAPTOOLS_FW_H
#define __TERRALIB_FW_INTERNAL_MAPTOOLS_FW_H

// Forward declarations
namespace te
{
  namespace map
  {
    class AbstractLayer;
	class AbstractMapDisplay;
	class AbstractRenderer;
    class Canvas;
    class CoordTransformer;
	class DataGrid;
	class FolderLayer;
	class Layer;
	class LayerRenderer;
	class MapDisplay;
	class QueryEncoder;
	class QueryLayer;
	class QueryLayerRenderer;
  }
}

#endif // __TERRALIB_FW_INTERNAL_MAPTOOLS_FW_H

