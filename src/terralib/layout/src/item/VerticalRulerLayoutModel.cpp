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
_invertedLines(false),
_backEndSpacing(10),
_backEndMargin(2.)
{
  _box = te::gm::Envelope(0., 0., 150., 150.);
  _paperBox = te::gm::Envelope(0., 0., 210., 297.); // A4 default
  _backEndBox = te::gm::Envelope(0., 0., 150., 150.);

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
    utils->configCanvas(_box);  
  
  drawRuler(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(_box);
  
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

    envPaper = te::gm::Envelope(_backEndBox.getLowerLeftX(), _paperBox.getLowerLeftY(),
      _backEndBox.getUpperRightX(), _paperBox.getUpperRightY());

    te::color::RGBAColor colorp2(255,255,255, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp2);
    canvas->setPolygonContourColor(colorp2);
    //Cor 255
    utils->drawRectW(envPaper);

    drawVerticalRuler(canvas, utils);

    te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp3);
    //Cor 0
    if(_invertedLines)
      envMargin = te::gm::Envelope(_backEndBox.getUpperRightX() - _lineMargin, _backEndBox.getLowerLeftY(), 
      _backEndBox.getUpperRightX() - _lineMargin, _backEndBox.getHeight());
    else
    envMargin = te::gm::Envelope(_lineMargin, _backEndBox.getLowerLeftY(), _lineMargin, 
      _backEndBox.getHeight());

    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
    line = utils->createSimpleLine(envMargin);
    utils->drawLineW(line);
    if(line) delete line;
  } 
}

void te::layout::VerticalRulerLayoutModel::drawVerticalRuler(te::map::Canvas* canvas, LayoutUtils* utils)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  te::gm::Envelope box;
  _posCount = _backEndBox.getLowerLeftY();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);
  for(int i = 0 ; i < _verticalBlockMarks ; ++i )
  {
    //TypeRulerVertical
    if(_invertedLines)
      box = te::gm::Envelope(_backEndBox.getUpperRightX(), _posCount, _backEndBox.getUpperRightX() - _longLine, _posCount);
    else
      box = te::gm::Envelope(_backEndBox.getLowerLeftX(), _posCount, _backEndBox.getLowerLeftX() + _longLine, _posCount);
    line = utils->createSimpleLine(box);
    utils->drawLineW(line);
    if(line) delete line;
    if(_invertedLines)
      canvas->drawText(_backEndBox.getUpperRightX() - (_longLine + 1.), _posCount, _verticalTexts[i], -90);
    else
      canvas->drawText(_backEndBox.getLowerLeftX() + (_longLine + 1.), _posCount, _verticalTexts[i], -90);
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
        box = te::gm::Envelope(_backEndBox.getUpperRightX(), _posCount, _backEndBox.getUpperRightX() - _mediumLine, _posCount);
      else
        box = te::gm::Envelope(_backEndBox.getLowerLeftX(), _posCount, _backEndBox.getLowerLeftX()+ _mediumLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);    
      if(line) delete line;
  }
  else
  { 
      //TypeRulerVertical
      if(_invertedLines)
        box = te::gm::Envelope(_backEndBox.getUpperRightX(), _posCount, _backEndBox.getUpperRightX() - _smallLine, _posCount);
      else
        box = te::gm::Envelope(_backEndBox.getLowerLeftX(), _posCount, _backEndBox.getLowerLeftX() + _smallLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
      drawMarks(canvas, utils, marks - 1);
      _posCount += 1;
      //TypeRulerVertical
      if(_invertedLines)
        box = te::gm::Envelope(_backEndBox.getUpperRightX(), _posCount, _backEndBox.getUpperRightX() - _smallLine, _posCount);
      else
        box = te::gm::Envelope(_backEndBox.getLowerLeftX(), _posCount, _backEndBox.getLowerLeftX() + _smallLine, _posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
}

void te::layout::VerticalRulerLayoutModel::updateVerticalListText()
{
  _verticalTexts.clear();
  int y = (int)_backEndBox.getLowerLeftY();
  int totaly = (int)_backEndBox.getUpperRightY();

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

  _box = box;
  _backEndBox = te::gm::Envelope(_box.getLowerLeftX() + _backEndMargin, _box.getLowerLeftY(),
    _box.getUpperRightX() - _backEndMargin , _box.getUpperRightY() - _backEndSpacing);
  _verticalBlockMarks = (int)std::abs(std::ceil(_backEndBox.getHeight() / _blockSize));
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
