#include "ConfigStyle.h"

// TerraLib
//#include <terralib/common.h>
//#include <terralib/dataaccess.h>
//#include <terralib/maptools.h>
//#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include "terralib/geometry/Coord2D.h"
#include "terralib/geometry/Geometry.h"
#include "terralib/geometry/GeometryCollection.h"
#include "terralib/geometry/LineString.h"
#include "terralib/geometry/LinearRing.h"
#include "terralib/geometry/MultiLineString.h"
#include "terralib/geometry/MultiPoint.h"
#include "terralib/geometry/MultiPolygon.h"
#include "terralib/geometry/Point.h"
#include "terralib/geometry/PointM.h"
#include "terralib/geometry/PointZ.h"
#include "terralib/geometry/PointZM.h"
#include "terralib/geometry/Polygon.h"
#include "terralib/postgis/EWKBReader.h"
#include "terralib/postgis/Utils.h"

//Qt
#include <QPainter>

//STL
#include <string>

ConfigStyle::ConfigStyle(QWidget* parent) : QDialog(parent)
{
  m_polygonColor.setColor(255, 255, 0, 150);
  m_polygonContourColor.setColor(0, 0, 255, 255);
  m_polygonContourWidth = 0;

  m_lineColor.setColor(0, 255, 255, 150);
  m_lineWidth = 0;

  m_pointColor.setColor(255, 0, 0, 150);
  m_pointWidth = 30;
  m_pointType = te::map::MarkerFourRays;

  m_splitter = new QSplitter(this);

  m_polygonWidget = new QLabel(this);
  m_polygonWidget->resize(100,100);
  m_polygonWidget->setMinimumHeight(100);
  m_polygonWidget->setMinimumWidth(100);

  m_lineWidget = new QLabel(this);
  m_lineWidget->resize(100,50);
  m_lineWidget->setMinimumHeight(50);
  m_lineWidget->setMinimumWidth(100);

  m_pointWidget = new QLabel(this);
  m_pointWidget->resize(60,60);
  m_pointWidget->setMinimumHeight(60);
  m_pointWidget->setMinimumWidth(60);

  m_groupBox = new QGroupBox(this);

  // POLYGON
  m_polygonColorButton = new QPushButton("Fill Color...", m_groupBox);
  m_polygonStyleButton = new QPushButton("Fill Style...", m_groupBox);
  m_polygonContourColorButton = new QPushButton("Contour Color...", m_groupBox);
  m_polygonContourStyleButton = new QPushButton("Contour Style...", m_groupBox);
  m_polygonContourWidthButton = new QPushButton("Contour Width...", m_groupBox);

  // LINE
  m_lineColorButton = new QPushButton("Line Color...", m_groupBox);
  m_lineStyleButton = new QPushButton("Line Style...", m_groupBox);
  m_lineWidthButton = new QPushButton("Line Width...", m_groupBox);

  // POINT
  m_pointColorButton = new QPushButton("Point Color...", m_groupBox);
  m_pointStyleButton = new QPushButton("Point Style...", m_groupBox);
  m_pointWidthButton = new QPushButton("Point Width...", m_groupBox);

  // FILTER
  m_filterButton = new QPushButton("Change Filter...", m_groupBox);
  m_filterTextEdit = new QTextEdit(m_groupBox);

  m_vBoxLayout = new QVBoxLayout(m_groupBox);

  // POLYGON
  m_polygonGroupBox = new QGroupBox(m_groupBox);
  m_polygonHBoxLayout = new QHBoxLayout(m_polygonGroupBox);
  m_polygonHBoxLayout->addWidget(m_polygonWidget);
  m_polygonHBoxLayout->addSpacing(10);
  m_polygonHBoxLayout->addWidget(m_polygonColorButton);
  m_polygonHBoxLayout->addWidget(m_polygonStyleButton);
  m_polygonHBoxLayout->addWidget(m_polygonContourColorButton);
  m_polygonHBoxLayout->addWidget(m_polygonContourStyleButton);
  m_polygonHBoxLayout->addWidget(m_polygonContourWidthButton);
  m_polygonGroupBox->setLayout(m_polygonHBoxLayout);
  m_vBoxLayout->addWidget(m_polygonGroupBox);
  m_vBoxLayout->addSpacing(10);

  // LINE
  m_lineGroupBox = new QGroupBox(m_groupBox);
  m_lineHBoxLayout = new QHBoxLayout(m_lineGroupBox);
  m_lineHBoxLayout->addWidget(m_lineWidget);
  m_lineHBoxLayout->addSpacing(10);
  m_lineHBoxLayout->addWidget(m_lineColorButton);
  m_lineHBoxLayout->addWidget(m_lineStyleButton);
  m_lineHBoxLayout->addWidget(m_lineWidthButton);
  m_lineGroupBox->setLayout(m_lineHBoxLayout);
  m_vBoxLayout->addWidget(m_lineGroupBox);
  m_vBoxLayout->addSpacing(10);

  // POINT
  m_pointGroupBox = new QGroupBox(m_groupBox);
  m_pointHBoxLayout = new QHBoxLayout(m_pointGroupBox);
  m_pointHBoxLayout->addWidget(m_pointWidget);
  m_pointHBoxLayout->addSpacing(10);
  m_pointHBoxLayout->addWidget(m_pointColorButton);
  m_pointHBoxLayout->addWidget(m_pointStyleButton);
  m_pointHBoxLayout->addWidget(m_pointWidthButton);
  m_pointGroupBox->setLayout(m_pointHBoxLayout);
  m_vBoxLayout->addWidget(m_pointGroupBox);
  m_vBoxLayout->addSpacing(10);

  // FILTER
  m_filterGroupBox = new QGroupBox(m_groupBox);
  m_filterHBoxLayout = new QHBoxLayout(m_filterGroupBox);
  m_filterHBoxLayout->addWidget(m_filterTextEdit);
  m_filterHBoxLayout->addWidget(m_filterButton);
  m_vBoxLayout->addWidget(m_filterGroupBox);


  m_groupBox->setLayout(m_vBoxLayout);

  m_hBoxLayout = new QHBoxLayout(this);
  m_hBoxLayout->addWidget(m_groupBox);
  setLayout(m_hBoxLayout);
}

ConfigStyle::~ConfigStyle()
{
  delete m_splitter;
}

void ConfigStyle::paintEvent(QPaintEvent*)
{
  // draw Polygon
  te::qt::widgets::Canvas canvas(100, 100);
  canvas.setWindow(0, 0, 99, 99);
  te::color::RGBAColor c(255, 255, 255, 255);
  canvas.setBackgroundColor(c);

  canvas.setPolygonFillColor(m_polygonColor);
  canvas.setPolygonContourColor(m_polygonContourColor);
  canvas.setPolygonContourWidth(m_polygonContourWidth);
  te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType);
  poly->setRingN(0, lr);
  lr->setPoint(0, 5, 5);
  lr->setPoint(1, 94, 5);
  lr->setPoint(2, 94, 94);
  lr->setPoint(3, 5, 94);
  lr->setPoint(4, 5, 5);
  canvas.draw(poly);
  delete poly;
  m_polygonWidget->setPixmap(*canvas.getPixmap());

  // draw Line
  canvas.resize(100, 50);
  canvas.clear();
  canvas.setWindow(0, 0, 99, 49);
  canvas.setLineColor(m_lineColor);
  canvas.setLineWidth(m_lineWidth);
  lr = new te::gm::LinearRing(10, te::gm::LineStringType);
  lr->setPoint(0, 3, 40);
  lr->setPoint(1, 10, 37);
  lr->setPoint(2, 18, 31);
  lr->setPoint(3, 28, 27);
  lr->setPoint(4, 38, 25);
  lr->setPoint(5, 47, 23);
  lr->setPoint(6, 54, 25);
  lr->setPoint(7, 70, 26);
  lr->setPoint(8, 80, 24);
  lr->setPoint(9, 97, 26);
  canvas.draw(lr);
  delete lr;
  lr = new te::gm::LinearRing(5, te::gm::LineStringType);
  lr->setPoint(0, 7, 18);
  lr->setPoint(1, 13, 16);
  lr->setPoint(2, 20, 18);
  lr->setPoint(3, 28, 20);
  lr->setPoint(4, 38, 25);
  canvas.draw(lr);
  delete lr;
  m_lineWidget->setPixmap(*canvas.getPixmap());

  // draw Point
  canvas.resize(60, 60);
  canvas.clear();
  canvas.setWindow(0, 0, 59, 59);
  canvas.setPointColor(m_pointColor);
  canvas.setPointWidth(m_pointWidth);
  canvas.setPointMarker(m_pointType);
  te::gm::Point *pt = new te::gm::Point();
  pt->setX(29.);
  pt->setY(29.);
  canvas.draw(pt);
  delete pt;
  m_pointWidget->setPixmap(*canvas.getPixmap());
}

void ConfigStyle::setLayer(te::map::AbstractLayer* layer)
{
  te::se::Style* style = layer->getStyle();
  update();
}


