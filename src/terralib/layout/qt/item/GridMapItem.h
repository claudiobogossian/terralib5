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
#include "ObjectItem.h"
#include "../../core/Config.h"
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Envelope.h"

// STL
#include <map>
#include <string>

// Qt
#include <QColor>
#include <QList>
#include <QLineF>
#include <QPointF>
#include <QString>
#include <QPainter>
#include <QFont>

namespace te
{
  namespace layout
  {
    class Observable;

    /*!
    \brief Class that represents a graphic GridMap. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 
	  
	    \ingroup layout

	    \sa te::layout::ObjectItem
	  */
    class TELAYOUTEXPORT GridMapItem : public ObjectItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        GridMapItem( ItemController* controller, Observable* o, bool invertedMatrix = false );

        /*!
          \brief Destructor
         */
        virtual ~GridMapItem();
        
        virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from ParentItem
         */
        virtual QRectF boundingRect() const;
                        
      protected:

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual QVariant	itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );
                        
        virtual void drawGrid(QPainter* painter);

        virtual void drawDefaultGrid(QPainter* painter);

        virtual void drawContinuousLines(QPainter* painter);

        virtual void drawCrossLines(QPainter* painter);

        virtual void drawTopTexts(QPainter* painter);

        virtual void drawBottomTexts(QPainter* painter);

        virtual void drawLeftTexts(QPainter* painter);

        virtual void drawRightTexts(QPainter* painter);

        virtual void drawVerticalLines(QPainter* painter);

        virtual void drawHorizontalLines(QPainter* painter);

        virtual void calculateVertical(te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale);

        virtual void calculateHorizontal(te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale);

        virtual void drawTexts(QPainter* painter);
                
        virtual void drawText( QPointF point, QPainter* painter, std::string text, bool displacementLeft = false, bool displacementRight = false);

        virtual void recalculateBoundingRect();

        virtual bool hasLayer();

        virtual void configPainter(QPainter* painter);

        virtual void configTextPainter(QPainter* painter);

        virtual void clear();

        /*!
          \brief Check if is necessary change map displacement.

          \param width text width in mm
          \param height text height in mm
         */
        virtual void checkMaxMapDisplacement(QFont ft, std::string text, double& width, double& height);

        virtual void changeMapDisplacement(double width, double height);

        double                    m_maxWidthTextMM;
        double                    m_maxHeigthTextMM;
        double                    m_onePointMM;
        bool                      m_changeSize;

        QList<QLineF>             m_verticalLines;
        QList<QLineF>             m_horizontalLines;

        std::map<std::string, QPointF>    m_topTexts;
        std::map<std::string, QPointF>    m_bottomTexts;
        std::map<std::string, QPointF>    m_rightTexts;
        std::map<std::string, QPointF>    m_leftTexts;
    };
  }
}

#endif

