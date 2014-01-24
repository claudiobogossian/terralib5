#include "QGroupRulerLayoutItem.h"
#include "LayoutItemController.h"
#include "Coordinate.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include <QtGui/QGraphicsRectItem>
#include <QPainter>
#include "LayoutItemModel.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "LayoutItemModel.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

te::layout::QGroupRulerLayoutItem::QGroupRulerLayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  QGraphicsRectItem(0, 0),
  LayoutItem(controller, o)
{
  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItem*)item);

  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);
}

te::layout::QGroupRulerLayoutItem::~QGroupRulerLayoutItem()
{

}

void te::layout::QGroupRulerLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();

  LayoutItemModel* model = (LayoutItemModel*)_controller->getModel();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();

  te::gm::Envelope* box = utils->viewportBox(canvas, model->getBox());

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box->getWidth(), box->getHeight());
    pixmap.fill(Qt::transparent);
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box->getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::QGroupRulerLayoutItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

void te::layout::QGroupRulerLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QGroupRulerLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{

  if (change == ItemTransformHasChanged || change == ItemTransformChange || change == ItemScaleHasChanged)
  {
    int a = 0;
    a = a + 1;
  }

  return QGraphicsItem::itemChange(change, value);
}

te::layout::Coordinate te::layout::QGroupRulerLayoutItem::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  Coordinate coordinate;
  coordinate.setX(valuex);
  coordinate.setY(valuey);

  return coordinate;
}

void te::layout::QGroupRulerLayoutItem::setPixmap( const QPixmap& pixmap )
{
  _pixmap = pixmap;

  int devDpi = _ppi;
  double wmm = (_pixmap.width() * 25.4) / (double)devDpi;
  double hmm = (_pixmap.height() * 25.4) / (double)devDpi;
  QPointF point = pos();
  setRect(point.x(), point.y(), wmm, hmm);
}

void te::layout::QGroupRulerLayoutItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
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
  painter->translate( rect().width() / 2.0, rect().height() / 2.0 );
  //painter->rotate( mRotation );
  painter->translate( -boundRect.width() / 2.0, -boundRect.height() / 2.0 );
		
  painter->drawPixmap(QRectF( 0, 0, boundRect.width(),  boundRect.height() ), _pixmap, QRectF( 0, 0, _pixmap.width(), _pixmap.height() ));

  painter->restore();
	
}

void te::layout::QGroupRulerLayoutItem::drawBackground( QPainter * painter )
{
  if (painter)
  {
    painter->setBrush( brush() );//this causes a problem in atlas generation
    painter->setPen( Qt::NoPen );
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect( QRectF( 0, 0, rect().width(), rect().height() ) );
  }
}
