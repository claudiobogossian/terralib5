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

// TerraLib
#include "../../core/AbstractView.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Coord2D.h"
#include "Scene.h"
#include "../outside/PageSetupOutside.h"
#include "../outside/SystematicScaleOutside.h"
#include "MenuItem.h"

// Qt
#include <QGraphicsView>
#include <QMenu>
#include <QImage>
#include <QCursor>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;
class QGraphicsItemGroup;
class QLineF;
class QContextMenuEvent;

namespace te
{
  namespace layout
  {
    class VisualizationArea;
    class AbstractViewTool;
    class HorizontalRuler;
    class VerticalRuler;

    class TELAYOUTEXPORT View : public QGraphicsView, public AbstractView
    {
      Q_OBJECT //for slots/signals

      public:

        View(QWidget* widget = (QWidget*)0);

        virtual ~View();

        virtual void config();     

        virtual void closeOutsideWindows();

        virtual QImage createImage();

        virtual void resetView();

        virtual void pan();

        virtual void zoomArea();

        virtual void zoomOut();

        virtual void print();

        virtual void recompose();

        virtual void zoomPercentage();

        virtual void changeZoomFactor(double currentZoom);
                                
      public slots:

        virtual void onToolbarChangeContext(bool change);

        virtual void onMainMenuChangeContext(bool change);

        virtual void onChangeConfig();

        virtual void onSystematicApply(double scale, SystematicScaleType type);

        virtual void onSelectionChanged();

      signals:

        void changeSceneCoordMouse(QPointF pos);

        void showView();

        void closeView();

        void hideView();

        /* The Properties only load when selection change and mouse release */
        void reloadProperties(); 

        void changeZoom(double currentFactor);

      protected:
        
        virtual void mousePressEvent(QMouseEvent * event);

        virtual void	mouseMoveEvent ( QMouseEvent * event );

        virtual void	mouseReleaseEvent ( QMouseEvent * event );

        virtual void wheelEvent(QWheelEvent *event);

        virtual void keyPressEvent(QKeyEvent* keyEvent);

        virtual void resizeEvent(QResizeEvent * event);

        virtual void hideEvent ( QHideEvent * event );

        virtual void closeEvent ( QCloseEvent * event );

        virtual void	showEvent ( QShowEvent * event );

        virtual void	contextMenuEvent ( QContextMenuEvent * event );

        virtual void	drawForeground ( QPainter * painter, const QRectF & rect );
                
        virtual void createItemGroup();

        virtual void destroyItemGroup();
        
        virtual void resetDefaultConfig();

        virtual void outsideAreaChangeContext(bool change);
        
        virtual void showPageSetup();

        virtual void showSystematicScale();

        virtual bool intersectionSelectionItem(int x, int y);
        
        virtual QCursor createCursor(std::string pathIcon);
        
        virtual bool exportProperties(EnumType* type);

        virtual bool importTemplate(EnumType* type);

        virtual void exportItemsToImage();

        virtual bool isExceededLimit(double currentScale, double factor, double oldFactor);

      protected:

        VisualizationArea*      m_visualizationArea;
        AbstractViewTool*       m_currentTool;
        PageSetupOutside*       m_pageSetupOutside;
        SystematicScaleOutside* m_systematicOutside;
        te::gm::Coord2D         m_coordSystematic;
        bool                    m_selectionChange;
        MenuItem*               m_menuItem;
        HorizontalRuler*        m_horizontalRuler;
        VerticalRuler*          m_verticalRuler;
        double                  m_maxZoomLimit;
        double                  m_minZoomLimit;
    };
  }
}
#endif
