#ifndef QGROUPRULER_LAYOUTITEM_H
#define QGROUPRULER_LAYOUTITEM_H

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
    class QGroupRulerLayoutItem : public QGraphicsRectItem, public LayoutItem
    {
      public:

        QGroupRulerLayoutItem(LayoutItemController* controller = 0, LayoutObservable* o = 0);
        virtual ~QGroupRulerLayoutItem();

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

#endif //QGROUPRULER_LAYOUTITEM_H