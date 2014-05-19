/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file QLayoutView.h
   
  \brief 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_VIEW_H

// Qt
#include <QGraphicsView>
#include <QTabwidget>
#include <QDockWidget>
#include <QPointF>
#include <QList>

// TerraLib
#include "AbstractView.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;
class QGraphicsItemGroup;

namespace te
{
  namespace layout
  {
    class OutsideArea;
    class VisualizationArea;
    class BuildGraphicsItem;

    class View : public QGraphicsView, public AbstractView
    {
      Q_OBJECT //for slots/signals

      public:
        View(QWidget* widget = (QWidget*)0);
        ~View();

        virtual void config();        
        
        void setOutsideArea(OutsideArea* outsideArea);
        
      public slots:

        virtual void onToolbarChangeContext(bool change);
        virtual void onSelectionChanged();
        virtual void onAddItemFinalized();

      protected:
        void mousePressEvent(QMouseEvent * event);
        void wheelEvent(QWheelEvent *event);
        void scaleView(qreal scaleFactor);
        virtual void keyPressEvent(QKeyEvent* keyEvent);
        virtual void resizeEvent(QResizeEvent * event);
        void hideEvent ( QHideEvent * event );
        void closeEvent ( QCloseEvent * event );
        virtual void	showEvent ( QShowEvent * event );

        virtual void	paintEvent ( QPaintEvent * event );

        //PaintDevice
        virtual int	metric ( PaintDeviceMetric metric ) const;

        virtual void createItem(const te::gm::Coord2D& coord );

        virtual void createItemGroup();

        virtual void destroyItemGroup();

        virtual void deleteItems();

        virtual void buildTemplate();
        
      protected:
        OutsideArea*  m_outsideArea;
        VisualizationArea* m_visualizationArea;
        BuildGraphicsItem* m_buildItems;
    };
  }
}
#endif