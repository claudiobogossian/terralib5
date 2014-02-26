#ifndef QMAPLAYOUTITEM_H
#define QMAPLAYOUTITEM_H

#include "QObjectLayoutItem.h"
#include "../../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include <QtGui/QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>

class QGraphicsProxyWidget;

namespace te
{
  namespace layout
  {
    class LayoutItemModelObservable;
    class LayoutItemController;

    class QMapLayoutItem : public QObjectLayoutItem
    {
      public:

        QMapLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o );
        virtual ~QMapLayoutItem();
        
        virtual void updateObserver(ContextLayoutItem context);
        
    protected slots:
      void onDrawLayersFinished(const QMap<QString, QString>& errors);

    protected:
      
      virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
      virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
      void drawLayerSelection();

      te::qt::widgets::MultiThreadMapDisplay* m_mapDisplay;
      QPixmap m_lastDisplayContent;
      QColor m_selectionColor;
      QGraphicsProxyWidget* _proxyWidgetDisplay;
    };
  }
}

#endif