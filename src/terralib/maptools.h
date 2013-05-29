/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file maptools.h

  \brief This file contains include headers for the Map Tools module of TerraLib.

  TerraLib provides a basic set of tools for dealing with maps.
  You can extend this basic framework in order to have your own way
  of rendering and representing the concept of a map.
 */

#ifndef __TERRALIB_INTERNAL_MAPTOOLS_H
#define __TERRALIB_INTERNAL_MAPTOOLS_H

// TerraLib
#include "maptools/AbstractLayer.h"
#include "maptools/AbstractMapDisplay.h"
#include "maptools/AbstractMarkRenderer.h"
#include "maptools/AbstractRenderer.h"
#include "maptools/Canvas.h"
#include "maptools/Config.h"
#include "maptools/DataSetLayer.h"
#include "maptools/DataSetLayerRenderer.h"
#include "maptools/FolderLayer.h"
#include "maptools/MapDisplay.h"
#include "maptools/MarkRendererManager.h"
#include "maptools/QueryEncoder.h"
#include "maptools/QueryLayer.h"
#include "maptools/QueryLayerRenderer.h"
#include "maptools/RendererFactory.h"
#include "maptools/Utils.h"

namespace te
{
  /*!
    \brief Namespace for the Map Tools Model of TerraLib.

    TerraLib provides a basic set of tools for dealing with maps.
  You can extend this basic framework in order to have your own way
  of rendering and representing the concept of a map.
   */
  namespace map
  {
  }	// end namespace map
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_MAPTOOLS_H

