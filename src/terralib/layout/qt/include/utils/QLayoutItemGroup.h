#ifndef	QLAYOUT_ITEMGROUP_H
#define QLAYOUT_ITEMGROUP_H

#include <QGraphicsItemGroup>
#include "LayoutItemObserver.h"
#include "LayoutAbstractType.h"
#include "../../../../geometry/Envelope.h"
#include <QPainter>
#include <QPixmap>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;

namespace te
{
  namespace layout
  {
    class QLayoutItemGroup : public QGraphicsItemGroup, public LayoutItemObserver
    {
      public:
        QLayoutItemGroup( LayoutItemController* controller, LayoutItemModelObservable* o );
        ~QLayoutItemGroup();

        virtual void updateObserver(ContextLayoutItem context);
        virtual te::gm::Coord2D getPosition();

        void setPixmap( const QPixmap& pixmap );
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void	addToGroup ( QGraphicsItem * item );
        virtual bool contains(const QPointF &point) const;

      protected:

        virtual void setPosition(const double& x, const double& y);
        void drawBackground( QPainter * painter );

        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );

      protected:

        QPixmap _pixmap;

    };
  }
}
#endif