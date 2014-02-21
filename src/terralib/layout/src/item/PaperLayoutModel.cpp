#include "PaperLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"

#include "../../../maptools/Canvas.h"

te::layout::PaperLayoutModel::PaperLayoutModel()
{
  _backgroundColor = te::color::RGBAColor(255, 255, 255, 255);
  _shadowColor = te::color::RGBAColor(0, 0, 0, 255);

  _boxPaper = te::gm::Envelope(0., 0., 210., 297.);
  _boxShadow = te::gm::Envelope(10., 10., 220., 307.);
  _box = te::gm::Envelope(0., 0., 220., 307.);
}

te::layout::PaperLayoutModel::~PaperLayoutModel()
{

}

void te::layout::PaperLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(canvas, _box);
  
  canvas->setPolygonContourColor(_shadowColor);
  canvas->setPolygonFillColor(_shadowColor);

  utils->drawRectW(canvas, _boxShadow);

  canvas->setPolygonContourColor(_borderColor);
  canvas->setPolygonFillColor(_backgroundColor);

  utils->drawRectW(canvas, _boxPaper);
    
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(canvas, _box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

te::color::RGBAColor te::layout::PaperLayoutModel::getShadowColor()
{
  return _shadowColor;
}

void te::layout::PaperLayoutModel::setShadowColor( te::color::RGBAColor color )
{
  _shadowColor = color;
}

