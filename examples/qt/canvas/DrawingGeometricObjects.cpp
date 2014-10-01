#include "CanvasExamples.h"

// TerraLib
#include <terralib/color.h>
#include <terralib/geometry.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QApplication>

void DrawingGeometricObjects()
{
// If you want to use any Qt resource you must initialize a QApplication in some place!!!
  int argc = 0;
  QApplication app(argc, 0);

// create the canvas and adjust the world-device transformation parameters
  te::qt::widgets::Canvas canvas(800, 600);

  canvas.setWindow(-180.0, -90.0, +180.0, +90.0);

// adjust line pen
  canvas.setLineColor(te::color::RGBAColor(255, 0, 0, 255));

// draw a line
  te::gm::LineString l(2, te::gm::LineStringType);
  l.setPoint(0, -90.0, -45.0);
  l.setPoint(1, +90.0, +45.0);

  canvas.draw(&l);

// draw a polygon
  te::gm::LinearRing* shell = new te::gm::LinearRing(4, te::gm::LineStringType);
  shell->setPoint(0, -90.0, -45.0);
  shell->setPoint(1, +90.0, -45.0);
  shell->setPoint(2, 0.0, +45.0);
  shell->setPoint(3, -90.0, -45.0);

  te::gm::LinearRing* hole1 = new te::gm::LinearRing(4, te::gm::LineStringType);
  hole1->setPoint(0, -50.0, -30.0);
  hole1->setPoint(1, -40.0, -30.0);
  hole1->setPoint(2, -45.0, -10.0);
  hole1->setPoint(3, -50.0, -30.0);

  te::gm::LinearRing* hole2 = new te::gm::LinearRing(4, te::gm::LineStringType);
  hole2->setPoint(0, -30.0, -30.0);
  hole2->setPoint(1, -20.0, -30.0);
  hole2->setPoint(2, -25.0, -10.0);
  hole2->setPoint(3, -30.0, -30.0);

  te::gm::Polygon p(3, te::gm::PolygonType);
  p.setRingN(0, shell);
  p.setRingN(1, hole1);
  p.setRingN(2, hole2);

  canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 255, 255));
  canvas.setPolygonFillColor(te::color::RGBAColor(0, 255, 0, 100));

  canvas.draw(&p); 


// save a PNG to disk
  canvas.save("canvas.png", te::map::PNG);
}

