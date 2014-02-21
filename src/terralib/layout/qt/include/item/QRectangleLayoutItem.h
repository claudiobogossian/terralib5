#ifndef QRECTANGLELAYOUTITEM_H
#define QRECTANGLELAYOUTITEM_H

#include <QGraphicsItem>
#include <QtGui/QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItemObserver.h"
#include <QPainter>
#include "LayoutAbstractType.h"

#include "../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;

    class QRectangleLayoutItem : public QGraphicsPixmapItem, public LayoutItemObserver
    {
      public:

        QRectangleLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o );
        virtual ~QRectangleLayoutItem();
        
        virtual void updateObserver(ContextLayoutItem context);
        virtual te::gm::Coord2D getPosition();

        /*World coordinates*/
        virtual bool contains(const QPointF &point) const;
        
    protected:

      virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
      virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
      virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
      virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
      virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
            
      virtual void setPosition(const double& x, const double& y);

      virtual bool checkTouchesCorner(const double& x, const double& y);
      virtual QPixmap calculateNewPixmap(const double& x, const double& y);

      virtual te::gm::Envelope createNewBoxInCoordScene(const double& x, const double& y);

    protected:

      QPixmap _clonePixmap;
      bool    _mousePressedCtrl;
      QPointF _initialCoord;
      QPointF _finalCoord;
      bool    _hoverAboveItem;
      bool    _toResizeItem;
      LayoutSides _enumSides;
    };
  }
}

#endif