#ifndef QPAPERLAYOUTITEM_H
#define QPAPERLAYOUTITEM_H

#include "QObjectLayoutItem.h"
#include <QtGui/QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include "LayoutItemObserver.h"

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;

    class QPaperLayoutItem : public QObjectLayoutItem
    {
      public:

        QPaperLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o );
        virtual ~QPaperLayoutItem();
        
        virtual void updateObserver(ContextLayoutItem context);
              
    protected:

      virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
      virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    };
  }
}

#endif