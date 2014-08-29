/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H

// Qt
#include <QGraphicsProxyWidget>
#include <QVariant>

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "../../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../../../qt/widgets/tools/AbstractTool.h"
#include "../../core/enum/EnumMode.h"

class QGraphicsSceneMouseEvent;
class QMimeData;
class QVariant;

namespace te
{
  namespace layout
  {
    class Observable;
    class ItemController;

    class MapItem : public QGraphicsProxyWidget, public ItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        MapItem( ItemController* controller, Observable* o );
        virtual ~MapItem();
        
        virtual void updateObserver(ContextItem context);

        void setPixmap( const QPixmap& pixmap );

        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem* option, QWidget * widget = 0 );

        //Mandatory implementation methods
        virtual void setRect(QRectF rect);

        virtual void setPos(const QPointF &pos);

        void	setZValue ( qreal z );

        virtual void changeCurrentTool(LayoutMode mode);

        void clearCurrentTool();
                        
      protected slots:

          void onDrawLayersFinished(const QMap<QString, QString>& errors);

    protected:
      
      virtual void	dropEvent ( QGraphicsSceneDragDropEvent * event );

      //Override method for using moves, selected, etc., 
      //which are implemented by default in QGraphicsItem
      virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
      virtual void	dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
      virtual void	dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
      virtual void resizeEvent ( QGraphicsSceneResizeEvent * event );

      virtual QVariant	itemChange ( GraphicsItemChange change, const QVariant & value );
      
      virtual te::gm::Coord2D getPosition();
            
      virtual void drawSelection( QPainter* painter );

      virtual void getMimeData(const QMimeData* mime);

      std::list<te::map::AbstractLayerPtr>  getVisibleLayers();

      te::map::AbstractLayerPtr getLayer();

      virtual int getZValueItem();

      void setCurrentTool(te::qt::widgets::AbstractTool* tool);

      virtual void applyRotation();

    protected:

      QPixmap m_pixmap;
      QRectF  m_rect;//In local coordinate
      QMimeData* m_mime;
      
      te::qt::widgets::MultiThreadMapDisplay* m_mapDisplay;
      bool m_grabbedByWidget;
      te::qt::widgets::AbstractTreeItem* m_treeItem;
      te::qt::widgets::AbstractTool* m_tool;
    };
  }
}

#endif