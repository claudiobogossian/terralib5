#include "GroupRulerLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModel.h"
#include "LayoutContext.h"
#include "LayoutMode.h"
#include "LayoutUtils.h"

#include <sstream>
#include <string>     // std::string, std::to_string

#include "../../../maptools/Canvas.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../color/RGBAColor.h"
#include "AbstractRulerLayoutModel.h"

te::layout::GroupRulerLayoutModel::GroupRulerLayoutModel() :
_visibleHorizontalRuler(true),
_spacing(1.),
_horizontalBlockMarks(15),
_blockSize(10),
_middleBlockSize(5),
_longLine(6.),
_mediumLine(4.),
_smallLine(3.),
_borderLineSize(0.3),
_posCount(0),
_lineMargin(2.),
_invertedLines(false)
{
  _box = new te::gm::Envelope(0., 0., 150., 150.);
  _paperBox = new te::gm::Envelope(0., 0., 210., 297.); // A4 default
}

te::layout::GroupRulerLayoutModel::GroupRulerLayoutModel( AbstractRulerLayoutModel* verticalRuler, AbstractRulerLayoutModel* horizontalRuler )
{
  _verticalRuler = verticalRuler;
  _horizontalRuler = horizontalRuler;

  te::gm::Envelope* boxH = _horizontalRuler->getBox();
  te::gm::Envelope* boxV = _verticalRuler->getBox();
  _box = new te::gm::Envelope(boxH->getLowerLeftX(), boxV->getLowerLeftY(), 
    boxH->getUpperRightX(), boxV->getUpperRightY());
}

te::layout::GroupRulerLayoutModel::~GroupRulerLayoutModel()
{

}

void te::layout::GroupRulerLayoutModel::updateObserver( ContextLayoutItem context )
{

}

void te::layout::GroupRulerLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  
  if(context.isResizeCanvas())
    utils->configCanvas(canvas, _box);  
  
  ContextLayoutItem contextRuler;
  contextRuler.setResizeCanvas(false);

  _verticalRuler->draw(contextRuler);
  _horizontalRuler->draw(contextRuler);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(canvas, _box);

  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::GroupRulerLayoutModel::setPosition( const double& x, const double& y )
{
  double x1 = x; 
  double y1 = y;
  double x2 = x + _box->getWidth();
  double y2 = y + _box->getHeight();

  _box = new te::gm::Envelope(x1, y1, x2, y2);
}

void te::layout::GroupRulerLayoutModel::setVisibleHorizontalRuler( bool visible )
{
  _visibleHorizontalRuler = visible;
}

bool te::layout::GroupRulerLayoutModel::isVisibleHorizontalRuler()
{
  return _visibleHorizontalRuler;
}

void te::layout::GroupRulerLayoutModel::setPaperBox( te::gm::Envelope* box )
{
  _paperBox = box;
}

te::gm::Envelope* te::layout::GroupRulerLayoutModel::getPaperBox()
{
  return _paperBox;
}

void te::layout::GroupRulerLayoutModel::invertedLines( bool invert )
{
  _invertedLines = invert;
}
