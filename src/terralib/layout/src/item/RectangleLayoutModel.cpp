#include "RectangleLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"

#include "../../../geometry/Envelope.h"

#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"

te::layout::RectangleLayoutModel::RectangleLayoutModel() 
{
  _borderColor = te::color::RGBAColor(0, 0, 255, 255);
  _backgroundColor = te::color::RGBAColor(0, 255, 0, 100);

  _box = te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::RectangleLayoutModel::~RectangleLayoutModel()
{

}

void te::layout::RectangleLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(_box);
  
  canvas->setPolygonContourColor(_borderColor);
  canvas->setPolygonFillColor(_backgroundColor);
  
  utils->drawRectW(_box);
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(_box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

