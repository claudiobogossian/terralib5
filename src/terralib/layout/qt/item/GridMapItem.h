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
  \file GridMapItem.h
   
   \brief Class that represents a graphic GridMap. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_ITEM_H

// TerraLib
#include "AbstractItem.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    class Observable;
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic GridMap. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and AbstractItem, so it can become observer of a model (Observable). 
    
      \ingroup layout

      \sa te::layout::AbstractItem
    */
    class TELAYOUTEXPORT GridMapItem : public AbstractItem<QGraphicsItem>
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        GridMapItem(AbstractItemController* controller, bool invertedMatrix = false);

        /*!
          \brief Destructor
         */
        virtual ~GridMapItem();

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from ParentItem
         */
        virtual QRectF boundingRect() const;

        virtual void calculateGrid() = 0;

      protected:

        /*!
          \brief Draws the grid
        */
        virtual void drawGrid(QPainter* painter);
        
        virtual void configPainter(QPainter* painter);

        virtual void configTextPainter(QPainter* painter);

        virtual void clear();

        virtual void drawContinuousLines(QPainter* painter);

        virtual void drawVerticalLines(QPainter* painter);

        virtual void drawHorizontalLines(QPainter* painter);

        virtual void drawTexts(QPainter* painter);

        virtual void drawTopTexts(QPainter* painter);

        virtual void drawBottomTexts(QPainter* painter);

        virtual void drawLeftTexts(QPainter* painter);

        virtual void drawRightTexts(QPainter* painter);

        virtual void drawCrossLines(QPainter* painter);

        virtual bool drawCrossIntersectMapBorder(QLineF vrt, QLineF hrz, QPainter* painter);

        virtual void debugDrawLineEdges(QPainter* painter, const QLineF& line);

        virtual void debugDrawTextRect(QPainter* painter, const QPointF& point, const std::string& text, int rotate = 0);

      protected:

        double                          m_maxWidthTextMM;
        double                          m_maxHeigthTextMM;
        double                          m_onePointMM;
        bool                            m_changeSize;

        QList<QLineF>                   m_verticalLines;
        QList<QLineF>                   m_horizontalLines;

        std::map<std::string, QPointF>  m_topTexts;
        std::map<std::string, QPointF>  m_bottomTexts;
        std::map<std::string, QPointF>  m_rightTexts;
        std::map<std::string, QPointF>  m_leftTexts;

        te::gm::Envelope                m_boundingBox;
        int                             m_defaultRotate;

        bool                            m_showDebugDrawings;
    };
  }
}

#endif
