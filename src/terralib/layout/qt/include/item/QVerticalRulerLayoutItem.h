#ifndef QVERTICALRULER_LAYOUTITEM_H
#define QVERTICALRULER_LAYOUTITEM_H

#include <QGraphicsItem>
#include <QtGui/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItem.h"
#include "Coordinate.h"

namespace te
{
  namespace layout
  {
    class QVerticalRulerLayoutItem : public QGraphicsRectItem, public LayoutItem
    {
      public:

        QVerticalRulerLayoutItem(LayoutItemController* controller = 0, LayoutObservable* o = 0);
        virtual ~QVerticalRulerLayoutItem();

        virtual void updateObserver(ContextLayoutItem context);
        virtual Coordinate getPosition();

        void setPixmap( const QPixmap& pixmap );
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

      protected:

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
        virtual void setPosition(const double& x, const double& y);
        void drawBackground( QPainter * painter );

      protected:

        QPixmap _pixmap;
    };
  }
}

#endif