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
   
   \brief Class that represents a "Model" part of MapGrid MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of MapModel, so it can become observable and visitable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_GRID_MODEL_H

// TerraLib
#include "MapModel.h"
#include "../core/Config.h"

namespace te
{
  namespace layout
  {
    class GridPlanarModel;
    class GridGeodesicModel;
    class Utils;
    class GeodesicGridSettingsConfigProperties;
    class PlanarGridSettingsConfigProperties;

    /*!
    \brief Class that represents a "Model" part of MapGrid MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of MapModel, so it can become observable and visitable.
      	  
	    \ingroup layout

      \sa te::layout::MapModel
	  */
    class TELAYOUTEXPORT MapGridModel : public MapModel
    {
      public:

        /*!
          \brief Constructor
        */
        MapGridModel();

        /*!
          \brief Destructor
        */ 
        virtual ~MapGridModel();

        virtual void draw( ContextItem context );

        virtual te::layout::Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties);

        virtual void generateSystematic(te::gm::Coord2D coord);

        virtual GridPlanarModel* getGridPlanar();

        virtual GridGeodesicModel* getGridGeodesic();
        
      protected:

        virtual void drawGrid(te::map::Canvas* canvas, Utils* utils);

        virtual void drawBackground(ContextItem context);

        GridPlanarModel*    m_gridPlanar;
        GridGeodesicModel*  m_gridGeodesic;
        GeodesicGridSettingsConfigProperties* m_geodesicGridProperties;
        PlanarGridSettingsConfigProperties*   m_planarGridProperties;
        bool                                  m_mapBorder;
    };
  }
}
#endif 
