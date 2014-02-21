#include "VerticalRulerLayoutModel.h"
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

te::layout::VerticalRulerLayoutModel::VerticalRulerLayoutModel() :
_visibleVerticalRuler(true),
_spacing(1.),
_verticalBlockMarks(15),
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
  _box = te::gm::Envelope(0., 0., 150., 150.);
  _paperBox = te::gm::Envelope(0., 0., 210., 297.); // A4 default

  updateVerticalListText();
}

te::layout::VerticalRulerLayoutModel::~VerticalRulerLayoutModel()
{

}

void te::layout::VerticalRulerLayoutModel::updateObserver( ContextLayoutItem context )
{

}

void te::layout::VerticalRulerLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
 
  if(context.isResizeCanvas())
    utils->configCanvas(canvas, _box);  
  
  drawRuler(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(canvas, _box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::VerticalRulerLayoutModel::drawRuler( te::map::Canvas* canvas, LayoutUtils* utils )
{
  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;
  
  if(_visibleVerticalRuler)
  {
    te::color::RGBAColor colorp1(80,80,80, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp1);
    //Cor 80
    utils->drawRectW(canvas, _box);

    envPaper = te::gm::Envelope(_box.getLowerLeftX(), _paperBox.getLowerLeftY(),
      _box.getUpperRightX(), _paperBox.getUpperRightY());

    te::color::RGBAColor colorp2(0,255,0, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp2);
    //Cor 255
    utils->drawRectW(canvas, envPaper);

    drawVerticalRuler(canvas, utils);

    te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp3);
    //Cor 0
    if(_invertedLines)
      envMargin = te::gm::Envelope(_box.getUpperRightX() - _lineMargin, _box.getLowerLeftY(), 
      _box.getUpperRightX() - _lineMargin, _box.getHeight());
    else
    envMargin = te::gm::Envelope(_lineMargin, _box.getLowerLeftY(), _lineMargin, 
      _box.getHeight());

    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
    line = utils->createSimpleLine(envMargin);
    utils->drawLineW(canvas, line);
    if(line) delete line;
  } 
}

void te::layout::VerticalRulerLayoutModel::drawVerticalRuler(te::map::Canvas* canvas, LayoutUtils* utils)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  te::gm::Envelope box;
  _posCount = _box.getLowerLeftY();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);
  for(int i = 0 ; i < _verticalBlockMarks ; ++i )
  {
    //TypeRulerVertical
    if(_invertedLines)
      box = te::gm::Envelope(_box.getUpperRightX(), _posCount, _box.getUpperRightX() - _longLine, _posCount);
    else
      box = te::gm::Envelope(_box.getLowerLeftX(), _posCount, _box.getLowerLeftX() + _longLine, _posCount);
    line = utils->createSimpleLine(box);
    utils->drawLineW(canvas, line);
    if(line) delete line;
    if(_invertedLines)
      canvas->drawText(_box.getUpperRightX() - (_longLine + 1.), _posCount, _verticalTexts[i], -90);
    else
      canvas->drawText(_box.getLowerLeftX() + (_longLine + 1.), _posCount, _verticalTexts[i], -90);
    drawMarks(canvas, utils, _blockSize - 1);
    _posCount += 1;
  }	
  _posCount = 0;
}

void te::layout::VerticalRulerLayoutModel::drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks)
{
  te::gm::Envelope box;
  te::gm::LinearRing* line = 0;
  _posCount += 1;
  if(marks == _middleBlockSize)
  {      
      //TypeRulerVertical
      if(_invertedLines)
        box = te::gm::Envelope(_box.getUpperRightX(), _posCount, _box.getUpperRightX() - _mediumLine, _posCount);
      else
        box = te::gm::Envelope(_box.getLowerLeftX(), _posCount, _box.getLowerLeftX()+ _mediumLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(canvas, line);    
      if(line) delete line;
  }
  else
  { 
      //TypeRulerVertical
      if(_invertedLines)
        box = te::gm::Envelope(_box.getUpperRightX(), _posCount, _box.getUpperRightX() - _smallLine, _posCount);
      else
        box = te::gm::Envelope(_box.getLowerLeftX(), _posCount, _box.getLowerLeftX() + _smallLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(canvas, line);
      if(line) delete line;
      drawMarks(canvas, utils, marks - 1);
      _posCount += 1;
      //TypeRulerVertical
      if(_invertedLines)
        box = te::gm::Envelope(_box.getUpperRightX(), _posCount, _box.getUpperRightX() - _smallLine, _posCount);
      else
        box = te::gm::Envelope(_box.getLowerLeftX(), _posCount, _box.getLowerLeftX() + _smallLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(canvas, line);
      if(line) delete line;
  }
}

void te::layout::VerticalRulerLayoutModel::updateVerticalListText()
{
  _verticalTexts.clear();
  int y = (int)_box.getLowerLeftY();
  int totaly = (int)_box.getUpperRightY();

  int lastDigit = std::abs(y % 10);
  int is_dozen = _blockSize - lastDigit;

  int count = y;
  bool is_negative = count < 0;

  //Round number
  if(is_dozen > 0 && is_dozen < 10)
  {
    if(is_negative)
      count = count - is_dozen; 
    else
      count = count + is_dozen; 
  }

  while(count <= totaly)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream
    _verticalTexts.push_back(ss.str());
    count += _blockSize;
  }
}

void te::layout::VerticalRulerLayoutModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 3. || box.getHeight() < 10.)
    return;

  _verticalBlockMarks = (int)std::abs(std::ceil(box.getHeight() / _blockSize));
  _box = box;
  updateVerticalListText();
}

void te::layout::VerticalRulerLayoutModel::setVisibleVerticalRuler( bool visible )
{
  _visibleVerticalRuler = visible;
}

bool te::layout::VerticalRulerLayoutModel::isVisibleVerticalRuler()
{
  return _visibleVerticalRuler;
}

void te::layout::VerticalRulerLayoutModel::setPaperBox( te::gm::Envelope box )
{
  _paperBox = box;
}

te::gm::Envelope te::layout::VerticalRulerLayoutModel::getPaperBox()
{
  return _paperBox;
}

void te::layout::VerticalRulerLayoutModel::invertedLines( bool invert )
{
  _invertedLines = invert;
}
