#include "QHorizontalRulerLayoutItem.h"
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

te::layout::QHorizontalRulerLayoutItem::QHorizontalRulerLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QRectLayoutItem(controller, o)
{
}

te::layout::QHorizontalRulerLayoutItem::~QHorizontalRulerLayoutItem()
{

}

void te::layout::QHorizontalRulerLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();

  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();

  te::gm::Envelope box = utils->viewportBox(canvas, model->getBox());

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap.fill(Qt::transparent);
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::QHorizontalRulerLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QHorizontalRulerLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{

  if (change == ItemTransformHasChanged || change == ItemTransformChange || change == ItemScaleHasChanged)
  {
    int a = 0;
    a = a + 1;
  }

  return QGraphicsItem::itemChange(change, value);
}

