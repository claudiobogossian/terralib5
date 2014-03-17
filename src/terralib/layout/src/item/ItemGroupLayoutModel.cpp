#include "ItemGroupLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"

#include "../../../geometry/Envelope.h"

#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"

te::layout::ItemGroupLayoutModel::ItemGroupLayoutModel()
{
  _box = te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::ItemGroupLayoutModel::~ItemGroupLayoutModel()
{

}

void te::layout::ItemGroupLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(_box);
  
  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 255, 255));
  
  utils->drawRectW(_box);
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(_box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

