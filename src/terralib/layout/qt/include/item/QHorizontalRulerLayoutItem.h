#ifndef QHORIZONTALRULER_LAYOUTITEM_H
#define QHORIZONTALRULER_LAYOUTITEM_H

#include "QObjectLayoutItem.h"
#include <QGraphicsItem>
#include <QtGui/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>

namespace te
{
  namespace layout
  {
    class QHorizontalRulerLayoutItem : public QObjectLayoutItem
    {
      public:

        QHorizontalRulerLayoutItem(LayoutItemController* controller = 0, LayoutItemModelObservable* o = 0);
        virtual ~QHorizontalRulerLayoutItem();

        virtual void updateObserver(ContextLayoutItem context);
        
      protected:

        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    };
  }
}

#endif