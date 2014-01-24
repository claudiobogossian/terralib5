#include "QRectangleLayoutItem.h"
#include "LayoutItemController.h"
#include "Coordinate.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include <QtGui/QGraphicsPixmapItem>
#include "LayoutObservable.h"
#include "Coordinate.h"

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "LayoutItemModel.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

te::layout::QRectangleLayoutItem::QRectangleLayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  QGraphicsPixmapItem(0, 0),
  LayoutItem(controller, o)
{
  QGraphicsItem* item = this;
  te::layout::LayoutContext::getInstance()->getScene()->insertItem((LayoutItem*)item);

  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);
}

te::layout::QRectangleLayoutItem::~QRectangleLayoutItem()
{

}

void te::layout::QRectangleLayoutItem::updateObserver( ContextLayoutItem context )
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
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box->getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::QRectangleLayoutItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

void te::layout::QRectangleLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QRectangleLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{

  if (change == ItemTransformHasChanged || change == ItemTransformChange || change == ItemScaleHasChanged)
  {
    int a = 0;
    a = a + 1;
  }

  return QGraphicsItem::itemChange(change, value);
}

te::layout::Coordinate te::layout::QRectangleLayoutItem::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  Coordinate coordinate;
  coordinate.setX(valuex);
  coordinate.setY(valuey);

  return coordinate;
}
