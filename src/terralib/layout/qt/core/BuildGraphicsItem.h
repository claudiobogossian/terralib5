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
  \file BuildGraphicsItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H

//TerraLib
#include "../../core/AbstractBuildGraphicsItem.h"

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class BuildGraphicsItem : public AbstractBuildGraphicsItem
    {
      public:

        BuildGraphicsItem();

        virtual ~BuildGraphicsItem();
        
        QGraphicsItem* rebuildItem(te::layout::Properties* props, bool draw = true);

        QGraphicsItem* createItem(te::layout::EnumType* mode, const te::gm::Coord2D& coordinate, bool draw = true);

      protected:

        QGraphicsItem* createPaper();

        QGraphicsItem* createMap();

        QGraphicsItem* createMapGrid();

        QGraphicsItem* createText();

        QGraphicsItem* createRectangle();

	      QGraphicsItem* createLegend();

        QGraphicsItem* createScale();

        QGraphicsItem* createItemGroup();

        QGraphicsItem* createImage();  

        QGraphicsItem* createArrow();

        QGraphicsItem* createEllipse();
        
        QGraphicsItem* createPoint();

        QGraphicsItem* createTextGrid();

        QGraphicsItem* createTitle();

        QGraphicsItem* createLegendChild();
        
        std::string nameItem(std::string name, te::layout::EnumType* type);
        
      protected:

        std::string             m_paperItem;
        std::string             m_mapItem;
        std::string             m_mapGridItem;
        std::string             m_textItem;
        std::string             m_rectangleItem;
        std::string             m_legendItem;
        std::string             m_scaleItem;
        std::string             m_horizontalRuler;
        std::string             m_verticalRuler;
        std::string             m_groupItem;
        std::string             m_imageItem;
        std::string             m_arrowItem;
        std::string             m_ellipseItem;
        std::string             m_pointItem;
        std::string             m_textGridItem;
        std::string             m_titleItem;
        std::string             m_legendChildItem;
    };
  }
}

#endif
