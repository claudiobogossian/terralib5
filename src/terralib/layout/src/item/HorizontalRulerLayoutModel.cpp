#include "HorizontalRulerLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModelObservable.h"
#include "LayoutContext.h"
#include "LayoutMode.h"
#include "LayoutUtils.h"

#include <sstream>
#include <string>     // std::string, std::to_string

#include "../../../maptools/Canvas.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../color/RGBAColor.h"

te::layout::HorizontalRulerLayoutModel::HorizontalRulerLayoutModel() :
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
_invertedLines(false),
_backEndSpacing(10),
_backEndMargin(2.)
{
  _box = te::gm::Envelope(0., 0., 150., 150.);
  _paperBox = te::gm::Envelope(0., 0., 210., 297.); // A4 default
  _backEndBox = te::gm::Envelope(0., 0., 150., 150.);

  updateHorizontalListText();
}

te::layout::HorizontalRulerLayoutModel::~HorizontalRulerLayoutModel()
{

}

void te::layout::HorizontalRulerLayoutModel::updateObserver( ContextLayoutItem context )
{

}

void te::layout::HorizontalRulerLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  
  if(context.isResizeCanvas())
    utils->configCanvas(_box);  
  
  drawRuler(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(_box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::HorizontalRulerLayoutModel::drawRuler( te::map::Canvas* canvas, LayoutUtils* utils )
{
  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;
  
  if(_visibleHorizontalRuler)
  {
    te::color::RGBAColor colorp4(180,180,180, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp4);
    canvas->setPolygonContourColor(colorp4);
    //Cor 80
    utils->drawRectW(_box);

    te::color::RGBAColor colorp85(145,145,145, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp85);
    canvas->setPolygonContourColor(colorp85);
    //Cor 80
    utils->drawRectW(_backEndBox);
                
    envPaper = te::gm::Envelope(_paperBox.getLowerLeftX(), _backEndBox.getLowerLeftY(),
      _paperBox.getUpperRightX(), _backEndBox.getUpperRightY());

    te::color::RGBAColor colorp5(255,255,255, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp5);
    canvas->setPolygonContourColor(colorp5);
    utils->drawRectW(envPaper);

    drawHorizontalRuler(canvas, utils);

    te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp6);

    if(_invertedLines)
      envMargin = te::gm::Envelope(_backEndBox.getLowerLeftX(), _lineMargin, _backEndBox.getWidth(), 
      _lineMargin);
    else
      envMargin = te::gm::Envelope(_backEndBox.getLowerLeftX(), _backEndBox.getUpperRightY() - _lineMargin, _backEndBox.getWidth(), 
      _backEndBox.getUpperRightY() - _lineMargin);

    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
    line = utils->createSimpleLine(envMargin);
    utils->drawLineW(line);
    if(line) delete line;
  }
}

void te::layout::HorizontalRulerLayoutModel::drawHorizontalRuler(te::map::Canvas* canvas, LayoutUtils* utils)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  te::gm::Envelope box;
  _posCount = _backEndBox.getLowerLeftX();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  for(int i = 0 ; i < _horizontalBlockMarks ; ++i )
  {
    //TypeRulerHorizontal
    if(_invertedLines)
      box = te::gm::Envelope(_posCount, _backEndBox.getLowerLeftY(), _posCount, _backEndBox.getLowerLeftY() + _longLine);
    else
     box = te::gm::Envelope(_posCount, _backEndBox.getUpperRightY(), _posCount, _backEndBox.getUpperRightY() - _longLine);
    line = utils->createSimpleLine(box);
    utils->drawLineW(line);
    if(line) delete line;
    if(_invertedLines)
      canvas->drawText(_posCount, _longLine + 1., _horizontalTexts[i], 0);
    else      
      canvas->drawText(_posCount, _backEndBox.getUpperRightY() - (_longLine + 1.), _horizontalTexts[i], 0);
    
    drawMarks(canvas, utils, _blockSize - 1);
    _posCount += 1;
  }
  _posCount = 0;
}

void te::layout::HorizontalRulerLayoutModel::drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks)
{
  te::gm::Envelope box;
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  _posCount += 1;
  if(marks == _middleBlockSize)
  {
      //TypeRulerHorizontal
      if(_invertedLines)
        box = te::gm::Envelope(_posCount, _backEndBox.getLowerLeftY(), _posCount, _backEndBox.getLowerLeftY() + _mediumLine);
      else
        box = te::gm::Envelope(_posCount, _backEndBox.getUpperRightY(), _posCount, _backEndBox.getUpperRightY() - _mediumLine);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
  else
  { 
      //TypeRulerHorizontal
      if(_invertedLines)
        box = te::gm::Envelope(_posCount, _backEndBox.getLowerLeftY(), _posCount, _backEndBox.getLowerLeftY() + _smallLine);
      else
        box = te::gm::Envelope(_posCount, _backEndBox.getUpperRightY(), _posCount, _backEndBox.getUpperRightY() - _smallLine);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;

      drawMarks(canvas, utils, marks - 1);
      _posCount += 1;
      //TypeRulerHorizontal
      if(_invertedLines)
        box = te::gm::Envelope(_posCount, _backEndBox.getLowerLeftY(), _posCount, _backEndBox.getLowerLeftY() + _smallLine);  
      else
        box = te::gm::Envelope(_posCount, _backEndBox.getUpperRightY(), _posCount, _backEndBox.getUpperRightY() - _smallLine);  
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
}

void te::layout::HorizontalRulerLayoutModel::updateHorizontalListText()
{
  _horizontalTexts.clear();
  int x = (int)_backEndBox.getLowerLeftX();
  int totalx = (int)_backEndBox.getUpperRightX();
  
  int lastDigit = std::abs(x % 10);
  int is_dozen = _blockSize - lastDigit;

  int count = x;
  bool is_negative = count < 0;

  //Round number
  if(is_dozen > 0 && is_dozen < 10)
  {
    if(is_negative)
      count = count - is_dozen; 
    else
      count = count + is_dozen; 
  }

  while(count <= totalx)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream
    _horizontalTexts.push_back(ss.str());
    count += _blockSize;
  }
}

void te::layout::HorizontalRulerLayoutModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 15. || box.getHeight() < 3.)
    return;

  _box = box;
  _backEndBox = te::gm::Envelope(_box.getLowerLeftX() + _backEndSpacing, _box.getLowerLeftY() + _backEndMargin,
    _box.getUpperRightX(), _box.getUpperRightY() - _backEndMargin);
  _horizontalBlockMarks = (int)std::abs(std::ceil(_backEndBox.getWidth() / _blockSize));
  updateHorizontalListText();
}

void te::layout::HorizontalRulerLayoutModel::setVisibleHorizontalRuler( bool visible )
{
  _visibleHorizontalRuler = visible;
}

bool te::layout::HorizontalRulerLayoutModel::isVisibleHorizontalRuler()
{
  return _visibleHorizontalRuler;
}

void te::layout::HorizontalRulerLayoutModel::setPaperBox( te::gm::Envelope box )
{
  _paperBox = box;
}

te::gm::Envelope te::layout::HorizontalRulerLayoutModel::getPaperBox()
{
  return _paperBox;
}

void te::layout::HorizontalRulerLayoutModel::invertedLines( bool invert )
{
  _invertedLines = invert;
}
