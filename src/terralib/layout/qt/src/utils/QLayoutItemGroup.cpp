#include <QLayoutItemGroup.h>
#include "LayoutItemModelObservable.h"
#include "LayoutItemController.h"

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"
#include "LayoutUtils.h"
#include "LayoutContext.h"

#include <QGraphicsSceneMouseEvent>

te::layout::QLayoutItemGroup::QLayoutItemGroup( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QGraphicsItemGroup(0, 0),
  LayoutItemObserver(controller, o)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
}

te::layout::QLayoutItemGroup::~QLayoutItemGroup()
{

}

void te::layout::QLayoutItemGroup::updateObserver( ContextLayoutItem context )
{

  te::color::RGBAColor** rgba = context.getPixmap();  
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  te::gm::Envelope box = model->getBox();

  QPixmap pixmp;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmp.fill(Qt::transparent);
    pixmp = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmp);
  update();
}

void te::layout::QLayoutItemGroup::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

te::gm::Coord2D te::layout::QLayoutItemGroup::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QLayoutItemGroup::addToGroup( QGraphicsItem * item )
{
  QGraphicsItemGroup::addToGroup(item);

  QRectF chRect = childrenBoundingRect();

  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  model->setBox(te::gm::Envelope(chRect.x(), chRect.y(), chRect.x() + childrenBoundingRect().width(), 
    chRect.y() + childrenBoundingRect().height()));
}

void te::layout::QLayoutItemGroup::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent(event);
}

void te::layout::QLayoutItemGroup::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mousePressEvent(event);

  int indexItem = -1;

  QList<QGraphicsItem*> graphicsItems = childItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      if(item->contains(event->scenePos()))
      {
        int a = 0;
      }
    }
  }
}

void te::layout::QLayoutItemGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseReleaseEvent(event);
}

void te::layout::QLayoutItemGroup::setPixmap( const QPixmap& pixmap )
{
  _pixmap = pixmap;
}

void te::layout::QLayoutItemGroup::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  painter->translate( model->getBox().getWidth() / 2.0, model->getBox().getHeight() / 2.0 );
  painter->translate( -boundRect.width() / 2.0, -boundRect.height() / 2.0 );
  painter->drawPixmap(boundRect, _pixmap, QRectF( 0, 0, _pixmap.width(), _pixmap.height() ));
  painter->restore();

}

void te::layout::QLayoutItemGroup::drawBackground( QPainter * painter )
{
  if (painter)
  {
    LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
    //painter->setBrush( brush() );//this causes a problem in atlas generation
    painter->setPen( Qt::NoPen );
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect( QRectF( 0, 0, model->getBox().getWidth(), model->getBox().getHeight() ) );
  }
}

bool te::layout::QLayoutItemGroup::contains( const QPointF &point ) const
{
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  
  return model->contains(te::gm::Coord2D(point.x(), point.y()));
}
