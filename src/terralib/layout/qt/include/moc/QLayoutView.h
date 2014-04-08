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

#ifndef	__TERRALIB_LAYOUT_INTERNAL_QLAYOUT_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_QLAYOUT_VIEW_H

// Qt
#include <QGraphicsView>
#include <QTabwidget>
#include <QDockWidget>
#include <QPointF>

// TerraLib
#include "LayoutView.h"
#include "../../../../geometry/Envelope.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;

namespace te
{
  namespace layout
  {
    class QPropertiesWindowOutside; 
    class QObjectInspectorWindowOutside;
    class QToolbarWindowOutside;

    class QLayoutView : public QGraphicsView, public LayoutView
    {
      Q_OBJECT //for slots/signals

      public:
        QLayoutView(QWidget* widget = (QWidget*)0);
        ~QLayoutView();

        virtual void config();        
        
        virtual void setDockPropertiesParent(QWidget* dockParent);
        
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

        virtual void	paintEvent ( QPaintEvent * event );
        
        //PaintDevice
        virtual int	metric ( PaintDeviceMetric metric ) const;

        virtual bool createMap(QPointF point);
        virtual bool createText(QPointF point);
        virtual bool createRectangle(QPointF point);

      protected:
        double	   m_diagonalScreenInchSize;
        QPropertiesWindowOutside* m_dockProperties;
        QObjectInspectorWindowOutside* m_dockInspector;
        QToolbarWindowOutside* m_dockToolbar;
        QWidget*      m_dockParent;
        double        m_dpiX;
        double        m_dpiY;
    };
  }
}
#endif