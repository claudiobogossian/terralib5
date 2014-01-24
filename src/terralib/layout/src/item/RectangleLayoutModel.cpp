#include "RectangleLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModel.h"
#include "LayoutContext.h"

#include "../../../geometry/Envelope.h"

#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"

te::layout::RectangleLayoutModel::RectangleLayoutModel()
{
  _box = new te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::RectangleLayoutModel::~RectangleLayoutModel()
{

}

void te::layout::RectangleLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  canvas->clear();

  utils->configCanvas(canvas, _box);
  
  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 255, 255));
  canvas->setPolygonFillColor(te::color::RGBAColor(0, 255, 0, 100));
  
  utils->drawRectW(canvas, _box);
  
  pixmap = utils->getImageW(canvas, _box);

  canvas->clear();

  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::RectangleLayoutModel::setPosition( const double& x, const double& y )
{
  double x1 = x; 
  double y1 = y;
  double x2 = x + _box->getWidth();
  double y2 = y + _box->getHeight();

  _box = new te::gm::Envelope(x1, y1, x2, y2);
}

