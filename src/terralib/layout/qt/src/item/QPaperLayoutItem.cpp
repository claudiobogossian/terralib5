#include "QPaperLayoutItem.h"
#include "LayoutItemController.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutItemModelObservable.h"
#include <QCursor>
#include <QPixmap>

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

te::layout::QPaperLayoutItem::QPaperLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QRectLayoutItem(controller, o)
{
  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItemObserver*)item);
  
  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
}

te::layout::QPaperLayoutItem::~QPaperLayoutItem()
{

}

void te::layout::QPaperLayoutItem::updateObserver( ContextLayoutItem context )
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
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::QPaperLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QPaperLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{
  return QGraphicsItem::itemChange(change, value);
}
