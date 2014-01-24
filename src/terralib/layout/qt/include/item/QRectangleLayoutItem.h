#ifndef QRECTANGLELAYOUTITEM_H
#define QRECTANGLELAYOUTITEM_H

#include <QGraphicsItem>
#include <QtGui/QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItem.h"

namespace te
{
  namespace layout
  {
    class QRectangleLayoutItem : public QGraphicsPixmapItem, public LayoutItem
    {
      public:

        QRectangleLayoutItem( LayoutItemController* controller, LayoutObservable* o );
        virtual ~QRectangleLayoutItem();

        virtual void updateObserver(ContextLayoutItem context);
        virtual te::layout::Coordinate getPosition();

      protected:

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
        virtual void setPosition(const double& x, const double& y);
    };
  }
}

#endif