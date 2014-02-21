#ifndef QRECT_LAYOUTITEM_H
#define QRECT_LAYOUTITEM_H

#include <QGraphicsItem>
#include <QtGui/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItemObserver.h"
#include <QPainter>

namespace te
{
  namespace layout
  {
    class QRectLayoutItem : public QGraphicsRectItem, public LayoutItemObserver
    {
      public:

        QRectLayoutItem(LayoutItemController* controller = 0, LayoutItemModelObservable* o = 0);
        virtual ~QRectLayoutItem();

        virtual void updateObserver(ContextLayoutItem context) = 0;
        virtual te::gm::Coord2D getPosition();

        void setPixmap( const QPixmap& pixmap );
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

      protected:

        virtual void setPosition(const double& x, const double& y);
        void drawBackground( QPainter * painter );

      protected:

        QPixmap _pixmap;
    };
  }
}

#endif //QRECT_LAYOUTITEM_H