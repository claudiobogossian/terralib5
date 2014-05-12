/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapGridModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_MODEL_H

// TerraLib
#include "MapModel.h"

namespace te
{
  namespace layout
  {
    class GridPlanarModel;
    class GridGeodesicModel;
    class Utils;

    class MapGridModel : public MapModel
    {
      public:

        MapGridModel();
        virtual ~MapGridModel();

        virtual void draw( ContextItem context );

        virtual te::layout::Properties* getProperties() const;
        virtual void updateProperties(te::layout::Properties* properties);

      protected:

        virtual void drawGrid(te::map::Canvas* canvas, Utils* utils);

        GridPlanarModel*    m_gridPlanar;
        GridGeodesicModel*  m_gridGeodesic;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_MAP_GRID_MODEL_H