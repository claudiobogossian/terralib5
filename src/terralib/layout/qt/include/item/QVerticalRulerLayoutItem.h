#ifndef QVERTICALRULER_LAYOUTITEM_H
#define QVERTICALRULER_LAYOUTITEM_H

#include "QRectLayoutItem.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>

namespace te
{
  namespace layout
  {
    class QVerticalRulerLayoutItem : public QRectLayoutItem
    {
      public:

        QVerticalRulerLayoutItem(LayoutItemController* controller = 0, LayoutItemModelObservable* o = 0);
        virtual ~QVerticalRulerLayoutItem();

        virtual void updateObserver(ContextLayoutItem context);

      protected:

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    };
  }
}

#endif