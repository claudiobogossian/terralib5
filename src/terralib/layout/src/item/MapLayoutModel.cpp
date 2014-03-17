#include "MapLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"

#include "../../../maptools/Canvas.h"

te::layout::MapLayoutModel::MapLayoutModel()
{
  _backgroundColor = te::color::RGBAColor(0, 0, 255, 255);
  _box = te::gm::Envelope(0., 0., 100., 70.);
}

te::layout::MapLayoutModel::~MapLayoutModel()
{

}

void te::layout::MapLayoutModel::draw( ContextLayoutItem context )
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

void te::layout::MapLayoutModel::setPosition( const double& x, const double& y )
{
  double x1 = x; 
  double y1 = y - _box.getHeight();

  double x2 = x + _box.getWidth();
  double y2 = y;

  _box = te::gm::Envelope(x1, y1, x2, y2);
}

