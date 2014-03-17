#ifndef QMAPLAYOUTITEM_H
#define QMAPLAYOUTITEM_H

//#include "QObjectLayoutItem.h"
#include <QGraphicsProxyWidget>
#include "LayoutItemObserver.h"
#include "../../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include <QtGui/QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "../../../../maptools/AbstractLayer.h"

//class QGraphicsProxyWidget;
class QGraphicsSceneMouseEvent;

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;
    class LayoutItemController;

    class QMapLayoutItem : public QGraphicsProxyWidget, public LayoutItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        QMapLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o );
        virtual ~QMapLayoutItem();
        
        virtual void updateObserver(ContextLayoutItem context);

        void setPixmap( const QPixmap& pixmap );

        //Mandatory implementation methods
        virtual void setRect(QRectF rect);
                        
    protected:
      
      virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
      virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	dropEvent ( QGraphicsSceneDragDropEvent * event );

      //Override method for using moves, selected, etc., 
      //which are implemented by default in QGraphicsItem
      virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
      virtual void	dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
      virtual void	dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
      
      //void drawBackground( QPainter * painter );

      virtual te::gm::Coord2D getPosition();

      virtual void setPosition( const double& x, const double& y );

    protected:

      QPixmap _pixmap;
      QRectF  _rect;//In local coordinate
      
      te::qt::widgets::MultiThreadMapDisplay* m_mapDisplay;
      bool grabbedByWidget;
    };
  }
}

#endif