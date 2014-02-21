#include "LayoutUtils.h"

#include "../../../color/RGBAColor.h"
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Enums.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"

#include "../../../qt/widgets/canvas/Canvas.h"
#include "LayoutContext.h"
#include "LayoutScene.h"

te::layout::LayoutUtils::LayoutUtils()
{

}

te::layout::LayoutUtils::~LayoutUtils()
{

}

void te::layout::LayoutUtils::drawRectW( te::map::Canvas* canvas, te::gm::Envelope box )
{
  te::gm::Polygon* rect = new te::gm::Polygon(1, te::gm::PolygonType, 0, &box);
  
  te::gm::LinearRing* outRingPtr0 = new te::gm::LinearRing(5, te::gm::LineStringType);
  outRingPtr0->setPointN( 0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  outRingPtr0->setPointN( 1, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY())); 
  outRingPtr0->setPointN( 2, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  outRingPtr0->setPointN( 3, te::gm::Point(box.getLowerLeftX(), box.getUpperRightY())); 
  outRingPtr0->setPointN( 4, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY())); 

  rect->setRingN(0, outRingPtr0);
  canvas->draw(rect);
}

void te::layout::LayoutUtils::drawLineW( te::map::Canvas* canvas, te::gm::LinearRing* line )
{
  canvas->draw(line);
}

te::gm::LinearRing* te::layout::LayoutUtils::createSimpleLine( te::gm::Envelope box )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line->setNumCoordinates(3);

  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  line->setPointN(1, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  line->setPointN(2, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  return line;
}

te::color::RGBAColor** te::layout::LayoutUtils::getImageW( te::map::Canvas* canvas, te::gm::Envelope box )
{
  te::color::RGBAColor** pixmap = 0;

  te::gm::Envelope boxViewport = viewportBox(canvas, box);

  if(boxViewport.isValid())
    pixmap = canvas->getImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight());
  return pixmap;
}

int te::layout::LayoutUtils::mm2pixel( te::map::Canvas* canvas, double mm )
{
  te::qt::widgets::Canvas* canvascopy = static_cast<te::qt::widgets::Canvas*>(canvas);

  int devDpi = canvascopy->getResolution();
  int px = (mm * devDpi) / 25.4 ;
  return px;
}

void te::layout::LayoutUtils::configCanvas( te::map::Canvas* canvas, te::gm::Envelope box )
{
  te::gm::Envelope boxViewport = viewportBox(canvas, box);
  canvas->resize(boxViewport.getWidth(), boxViewport.getHeight());
  canvas->setWindow(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY());  
}

te::gm::Envelope te::layout::LayoutUtils::viewportBox( te::map::Canvas* canvas, te::gm::Envelope box )
{
  int pxwidth = mm2pixel(canvas, box.getWidth());
  int pxheight = mm2pixel(canvas, box.getHeight());
  
  // Adjust internal renderer transformer
  m_transformer.setTransformationParameters(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY(), pxwidth, pxheight);

  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  m_transformer.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  m_transformer.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);
  
  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}

te::gm::Envelope te::layout::LayoutUtils::viewportBoxFromScene( te::map::Canvas* canvas, te::gm::Envelope box )
{
  te::gm::Envelope boxScene = LayoutContext::getInstance()->getScene()->getSceneBox();

  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;

  px1 = boxScene.getLowerLeftX();
  py1 = boxScene.getLowerLeftY();
  px2 = boxScene.getUpperRightX();
  py2 = boxScene.getUpperRightY();

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope newBoxScene(px1, py1, px2, py2);

  int pxwidth = mm2pixel(canvas, newBoxScene.getWidth());
  int pxheight = mm2pixel(canvas, newBoxScene.getHeight());

  // Adjust internal renderer transformer
  m_transformer.setTransformationParameters(newBoxScene.getLowerLeftX(), newBoxScene.getLowerLeftY(), 
    newBoxScene.getUpperRightX(), newBoxScene.getUpperRightY(), pxwidth, pxheight);

  m_transformer.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  m_transformer.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}
