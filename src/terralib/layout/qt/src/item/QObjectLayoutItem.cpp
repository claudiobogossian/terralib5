#include "QObjectLayoutItem.h"
#include "LayoutItemController.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include <QtGui/QGraphicsRectItem>
#include <QPainter>
#include "LayoutItemModelObservable.h"

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"
#include "QLayoutScene.h"

te::layout::QObjectLayoutItem::QObjectLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QGraphicsObject(0),
  LayoutItemObserver(controller, o)
{
  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItemObserver*)item);
}

te::layout::QObjectLayoutItem::~QObjectLayoutItem()
{

}

void te::layout::QObjectLayoutItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

te::gm::Coord2D te::layout::QObjectLayoutItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QObjectLayoutItem::setPixmap( const QPixmap& pixmap )
{
  _pixmap = pixmap;

  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  QPointF point = pos();

  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  te::gm::Envelope box = model->getBox();

  //If you modify the boundingRect value, you need to inform Graphics View about it by calling QGraphicsItem::prepareGeometryChange();
  QGraphicsObject::prepareGeometryChange();
  //The rect is in coordinate location of the item, because the bounding rect is 
  //updated with this value, and will be used by the scene for drawing.
  //Rect values ​​do not influence the actual position of the object.
  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

void te::layout::QObjectLayoutItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );

  QRectF boundRect;
  boundRect = boundingRect();
  	
  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
  painter->drawPixmap(boundRect, _pixmap, QRectF( 0, 0, _pixmap.width(), _pixmap.height() ));
  painter->restore();  
}

void te::layout::QObjectLayoutItem::drawBackground( QPainter * painter )
{
  if (painter)
  {
    //painter->setBrush(brush());
    painter->setPen(Qt::NoPen);
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect(QRectF( 0, 0, boundingRect().width(), boundingRect().height()));
  }
}

QRectF te::layout::QObjectLayoutItem::boundingRect() const
{
  return _rect;
}

void te::layout::QObjectLayoutItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  _rect = rect;
  update(rect);
}
