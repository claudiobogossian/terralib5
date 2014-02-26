#ifndef QOBJECT_LAYOUTITEM_H
#define QOBJECT_LAYOUTITEM_H

#include <QGraphicsObject>
#include <QtGui/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItemObserver.h"
#include <QPainter>
#include <QPixmap>

namespace te
{
  namespace layout
  {
    class QObjectLayoutItem : public QGraphicsObject, public LayoutItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        QObjectLayoutItem(LayoutItemController* controller = 0, LayoutItemModelObservable* o = 0);
        virtual ~QObjectLayoutItem();

        virtual void updateObserver(ContextLayoutItem context) = 0;
        virtual te::gm::Coord2D getPosition();

        void setPixmap( const QPixmap& pixmap );

        //Mandatory implementation methods
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        virtual QRectF boundingRect() const;
        virtual void setRect(QRectF rect);

      protected:

        virtual void setPosition(const double& x, const double& y);
        void drawBackground( QPainter * painter );

      protected:

        QPixmap _pixmap;
        QRectF  _rect;//In local coordinate
    };
  }
}

#endif //QRECT_LAYOUTITEM_H