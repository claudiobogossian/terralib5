/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/widgets/canvas/Canvas.cpp

  \brief A canvas built on top of Qt.
*/

// TerraLib
#include "../../../annotationtext/Attributes.h"
#include "../../../annotationtext/Element.h"
#include "../../../annotationtext/Text.h"
#include "../../../color/RGBAColor.h"
#include "../../../common/StringUtils.h"
#include "../../../geometry.h"
#include "../../../maptools/PtMarker.h"
#include "../../../raster.h"
#include "../Utils.h"
#include "Canvas.h"

// Qt
#include <QBitmap>
#include <QBuffer>
#include <QImage>
#include <QPaintEngine>
#include <QPixmap>
#include <QPrinter>
#include <QResizeEvent>

// STL
#include <cassert>
#include <algorithm>

te::qt::widgets::Canvas::Canvas(int w, int h, int devType)
  : m_isDeviceOwner(true),
    m_bgColor(Qt::transparent),
    m_erase(false),
    m_ptWidth(1),
    m_ptColor(Qt::black),
    m_ptColorFrom(m_ptColor),
    m_ptImg(0),
    m_ptImgRotated(0),
    m_ptSelectionPatternImg(0),
    m_ptClearPatternImg(0),
    m_ptRotation(0),
    m_ptVOffset(0),
    m_ptHOffset(0),
    m_lnColor(Qt::transparent),
    m_lnPen(Qt::blue),
    m_polyContourColor(Qt::transparent),
    m_polyContourPen(Qt::gray),
    m_polyColor(Qt::red),
    m_polyBrush(Qt::magenta),
    m_polyImage(0),
    m_polyRotatedImage(0),
    m_polyPatternWidth(16),
    m_txtContourPen(Qt::black),
    m_txtContourEnabled(false),
    m_txtBrush(Qt::black),
    m_txtLetterSpacing(1),
    m_txtWordSpacing(1),
    m_txtLineSpacing(1)
{
  if(devType == QInternal::Pixmap)
  {
    QPixmap* p = new QPixmap(w, h);
    p->fill(m_bgColor);
    m_painter.begin(p);
  }
  else
  {
    //QImage* i = new QImage(w, h, QImage::Format_ARGB32);
    QImage* i = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    i->fill(m_bgColor.rgba());
    m_painter.begin(i);
  }

  m_ptPen.setColor(m_ptColor);
}

te::qt::widgets::Canvas::Canvas(QPaintDevice* device)
  : m_isDeviceOwner(false),
    m_bgColor(Qt::transparent),
    m_erase(false),
    m_ptWidth(1),
    m_ptColor(Qt::black),
    m_ptColorFrom(m_ptColor),
    m_ptImg(0),
    m_ptImgRotated(0),
    m_ptSelectionPatternImg(0),
    m_ptClearPatternImg(0),
    m_ptRotation(0),
    m_ptVOffset(0),
    m_ptHOffset(0),
    m_lnColor(Qt::transparent),
    m_lnPen(Qt::blue),
    m_polyContourColor(Qt::transparent),
    m_polyContourPen(Qt::gray),
    m_polyColor(Qt::red),
    m_polyBrush(Qt::magenta),
    m_polyImage(0),
    m_polyRotatedImage(0),
    m_polyPatternWidth(16),
    m_txtContourPen(Qt::black),
    m_txtContourEnabled(false),
    m_txtBrush(Qt::black),
    m_txtLetterSpacing(1),
    m_txtWordSpacing(1),
    m_txtLineSpacing(1)
{
  m_painter.begin(device);
}

te::qt::widgets::Canvas::~Canvas()
{
  if(m_isDeviceOwner)
  {
    QPaintDevice* device = m_painter.device();
    m_painter.end();
    delete device;
  }

  delete m_ptImg;
  delete m_ptImgRotated;
  delete m_ptClearPatternImg;
  delete m_ptSelectionPatternImg;

}

void te::qt::widgets::Canvas::setWindow(const double& llx, const double& lly,
                                        const double& urx, const double& ury)
{
  double xScale = static_cast<double>(getWidth()) / (urx - llx);
  double yScale = static_cast<double>(getHeight()) / (ury - lly);

  m_matrix.reset();
  m_matrix.scale(xScale, -yScale);
  m_matrix.translate(-llx, -ury);

  m_painter.setMatrix(m_matrix);
}

void te::qt::widgets::Canvas::calcAspectRatio(double& llx, double& lly, double& urx, double& ury, 
                                              const te::map::AlignType hAlign, const te::map::AlignType vAlign)
{
  double ww = urx - llx;
  double wh = ury - lly;

  double widthByHeight = static_cast<double>(getWidth()) / static_cast<double>(getHeight());

  if(widthByHeight > ww / wh)
  {
    double v = ww;

    ww = wh * widthByHeight;

    if(hAlign == te::map::Left)
      urx = llx + ww;
    else if(hAlign == te::map::Right)
      llx = urx - ww;
    else // it is center
    {
      llx = llx - (ww - v) / 2.0;
      urx = llx + ww;
    }
  }
  else
  {
    double v = wh;

    wh = ww / widthByHeight;

    if(vAlign == te::map::Top)
      lly = ury - wh;
    else if(vAlign == te::map::Bottom)
      ury = lly + wh;
    else // it is center
    {
      lly = lly - (wh - v) / 2.0;
      ury = lly + wh;
    }
  }
}

void  te::qt::widgets::Canvas::calcAspectRatio(te::gm::Envelope* envelope, const te::map::AlignType hAlign, const te::map::AlignType vAlign)
{
  calcAspectRatio(envelope->m_llx, envelope->m_lly, envelope->m_urx, envelope->m_ury, hAlign, vAlign);
}

void te::qt::widgets::Canvas::setBackgroundColor(const te::color::RGBAColor& color)
{
  m_bgColor = QColor(color.getRgba());
  m_bgColor.setAlpha(qAlpha(color.getRgba()));

  m_painter.setWorldMatrixEnabled(false);
  m_painter.fillRect(m_painter.viewport(), m_bgColor);
  m_painter.setWorldMatrixEnabled(true);
}

te::color::RGBAColor te::qt::widgets::Canvas::getBackgroundColor() const
{
  return te::color::RGBAColor(m_bgColor.red(), m_bgColor.green(), m_bgColor.blue(), m_bgColor.alpha());
}

void te::qt::widgets::Canvas::clear()
{
  int devType = m_painter.device()->devType();

  if(devType != QInternal::Image && devType != QInternal::Pixmap)
    return;

  QPaintDevice* device = m_painter.device();

  if(device)
  {
    m_painter.end();
    if(devType == QInternal::Image)
      static_cast<QImage*>(device)->fill(m_bgColor.rgba());
    else
      static_cast<QPixmap*>(device)->fill(m_bgColor);
    m_painter.begin(device);
  }
  //QPixmap* pix = getPixmap();
  //if(pix)
  //{
  //  m_painter.end();
  //  pix->fill(m_bgColor);
  //  m_painter.begin(pix);
  //}
  m_painter.setMatrix(m_matrix);

  m_ptPen.setColor(m_ptColor);
  QRectF rec(QPoint(0., 0.), QPoint(m_ptWidth, m_ptWidth));
  rec = m_matrix.inverted().mapRect(rec);
  m_ptPen.setWidthF(rec.width());
}

void te::qt::widgets::Canvas::resize(int w, int h)
{
  int devType = m_painter.device()->devType();

  if(devType != QInternal::Image && devType != QInternal::Pixmap)
    return;

  if(!m_isDeviceOwner)
    return;

  if(devType == QInternal :: Pixmap)
  {
    QPixmap* pixmap = static_cast<QPixmap*>(m_painter.device());
    m_painter.end();
    delete pixmap;
    pixmap = new QPixmap(w, h);
    pixmap->fill(m_bgColor);
    m_painter.begin(pixmap);
  }
  else
  {
    QImage* image = static_cast<QImage*>(m_painter.device());
    m_painter.end();
    delete image;
    //image = new QImage(w, h, QImage::Format_ARGB32);
    image = new QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    image->fill(m_bgColor.rgba());
    m_painter.begin(image);
  }
}

int te::qt::widgets::Canvas::getWidth() const
{
  return m_painter.device()->width();
}

int te::qt::widgets::Canvas::getHeight() const
{
  return m_painter.device()->height();
}

void te::qt::widgets::Canvas::draw(const te::gm::Geometry* geom)
{
  int geomType = geom->getGeomTypeId();
  switch (geomType)
  {
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
      draw(static_cast<const te::gm::MultiPolygon*>(geom));
    break;

    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
      draw(static_cast<const te::gm::Point*>(geom));
    break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
      draw(static_cast<const te::gm::LineString*>(geom));
    break;

    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
      draw(static_cast<const te::gm::Polygon*>(geom));
    break;

    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
      draw(static_cast<const te::gm::MultiLineString*>(geom));
    break;

    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
      draw(static_cast<const te::gm::MultiPoint*>(geom));
    break;

    case te::gm::GeometryCollectionType:
    case te::gm::GeometryCollectionZType:
    case te::gm::GeometryCollectionMType:
    case te::gm::GeometryCollectionZMType:
      draw(static_cast<const te::gm::GeometryCollection*>(geom));
    break;

    case te::gm::MultiSurfaceType:
    case te::gm::MultiSurfaceZType:
    case te::gm::MultiSurfaceMType:
    case te::gm::MultiSurfaceZMType:
      draw(static_cast<const te::gm::MultiSurface*>(geom));
    break;

    default:
      break;
  }
}

void te::qt::widgets::Canvas::draw(const te::gm::Point* point)
{
  m_pt.setX(point->getX());
  m_pt.setY(point->getY());

  if(m_ptImg != 0)
  {
    QRectF rectf(0, 0, m_ptImg->width(), m_ptImg->height());
    QPointF pc = m_matrix.map(m_pt);
    pc.setX(pc.x() + m_ptHOffset);
    pc.setY(pc.y() + m_ptVOffset);
    rectf.moveCenter(pc);

    m_painter.setWorldMatrixEnabled(false);
    if(m_erase && m_ptClearPatternImg)
        m_painter.drawImage(rectf, *m_ptClearPatternImg);
    else if(!m_erase && m_ptSelectionPatternImg)
      m_painter.drawImage(rectf, *m_ptSelectionPatternImg);
    m_painter.setWorldMatrixEnabled(true);
  }
  else
  {
    if(m_erase)
    {
      QPen p(m_ptPen);
      p.setColor(Qt::black);
      m_painter.setPen(p);
    }
    else
      m_painter.setPen(m_ptPen);
    m_painter.drawPoint(m_pt);
  }
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiPoint* mpoint)
{
  std::size_t size = mpoint->getNumGeometries();

  for(std::size_t i = 0; i < size; ++i)
    draw(static_cast<const te::gm::Point*>(mpoint->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::LineString* line)
{
  if(m_lnPen.brush().style() != Qt::TexturePattern)
  {
    const std::size_t nPoints = line->getNPoints();
    QPolygonF qpol(nPoints);
    memcpy(&(qpol[0]), (double*)(line->getCoordinates()), 16 * nPoints);
    QPen pen(m_lnPen);
    pen.setColor(m_lnColor);
    m_painter.setPen(pen);
    m_painter.setBrush(Qt::NoBrush);
    m_painter.drawPolyline(qpol);
    return;
  }

  QPointF p1;
  QPointF p2;
  bool drawed = false;

  te::gm::Coord2D* coords = line->getCoordinates();
  std::size_t size = line->getNPoints();

  p1.setX(coords[0].x);
  p1.setY(coords[0].y);

  for(register std::size_t i = 1; i != size; ++i)
  {    
    p2.setX(coords[i].x);
    p2.setY(coords[i].y);

	  if(m_erase)
	  {
	    QPen pen(m_lnPen);
	    pen.setColor(Qt::red);
	    if(m_lnPen.brush().style() == Qt::TexturePattern)
		  pen.setCapStyle(Qt::FlatCap); // Qt::RoundCap Qt::FlatCap Qt::SquareCap
	    m_painter.setPen(pen);
	    m_painter.setBrush(Qt::NoBrush);
        m_painter.drawLine(p1, p2);
        drawed = true;
	  }
    //else if(m_lnPen.brush().style() != Qt::TexturePattern)
    //{
    //  QPen pen(m_lnPen);
    //  pen.setColor(m_lnColor);
    //  m_painter.setPen(pen);
    //  m_painter.drawLine(p1, p2);
    //  drawed = true;
    //}
    //else
    if(m_lnPen.brush().style() == Qt::TexturePattern)
    {
	    int minPixels = 20; // dx and dy is greater than minPixels, then, draw the segment using pattern
      double alfa;
      QPoint dp1 = m_matrix.map(p1).toPoint();
      QPoint dp2 = m_matrix.map(p2).toPoint();
      int ddx = dp2.x() - dp1.x();
      int ddy = dp2.y() - dp1.y();
      if(abs(ddx) <= minPixels && abs(ddy) <= minPixels)
      {
        drawed = false;
        continue;
      }

      drawed = true;
      double dx = p2.x() - p1.x();
      double dy = p2.y() - p1.y();
      double distance = sqrt((dx * dx) + (dy * dy));
      double scale = m_lnPen.widthF() / m_lnPen.brush().textureImage().height();

      if(ddx == 0.)
      {
        if(p2.y() >= p1.y())
          alfa = 90.;
        else
          alfa = 270.;
      }
      else if(ddy == 0.)
      {
        if(p2.x() >= p1.x())
          alfa = 0.;
        else
          alfa = 180.;
      }
      else
      {
        alfa = atan(dy/dx) * 180. / 3.14159265;
        if(dx < 0.)
          alfa += 180.;
        else if(alfa < 0 && dy < 0.)
          alfa += 360.;
        if(alfa == 360.)
          alfa = 0.;
      }

      QBrush brush(m_lnPen.brush());
      QPen pen(m_lnPen);
//      pen.setJoinStyle(Qt::RoundJoin); // Qt::BevelJoin Qt::MiterJoin Qt::RoundJoin
      pen.setCapStyle(Qt::FlatCap); // Qt::RoundCap Qt::FlatCap Qt::SquareCap
      QMatrix matrix;
      matrix.scale(scale, scale);
      matrix.translate(m_lnPen.brush().textureImage().width()/2, -m_lnPen.brush().textureImage().height()/2.);
      brush.setMatrix(matrix);
      pen.setBrush(brush);
      m_painter.setPen(pen);

      m_painter.save();
      m_painter.translate(p1);
      m_painter.rotate(alfa);
      m_painter.drawLine(0., 0., distance, 0.);
      if(m_lnColor.alpha() != 0)
      {
        pen.setBrush(Qt::NoBrush);
        pen.setColor(m_lnColor);
        m_painter.setPen(pen);
        m_painter.drawLine(0., 0., distance, 0.);
      }
      m_painter.restore();
    }
    p1 = p2;
  }
  if(drawed == false)
  {
    double alfa;
    QPoint dp1 = m_matrix.map(p1).toPoint();
    QPoint dp2 = m_matrix.map(p2).toPoint();
    int ddx = dp2.x() - dp1.x();
    int ddy = dp2.y() - dp1.y();
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double distance = sqrt((dx * dx) + (dy * dy));
    double scale = m_lnPen.widthF() / m_lnPen.brush().textureImage().height();

    if(ddx == 0.)
    {
      if(p2.y() >= p1.y())
        alfa = 90.;
      else
        alfa = 270.;
    }
    else if(ddy == 0.)
    {
      if(p2.x() >= p1.x())
        alfa = 0.;
      else
        alfa = 180.;
    }
    else
    {
      alfa = atan(dy/dx) * 180. / 3.14159265;
      if(dx < 0.)
        alfa += 180.;
      else if(alfa < 0 && dy < 0.)
        alfa += 360.;
      if(alfa == 360.)
        alfa = 0.;
    }

    QBrush brush(m_lnPen.brush());
    QPen pen(m_lnPen);
//      pen.setJoinStyle(Qt::RoundJoin); // Qt::BevelJoin Qt::MiterJoin Qt::RoundJoin
    pen.setCapStyle(Qt::FlatCap); // Qt::RoundCap Qt::FlatCap Qt::SquareCap
    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.translate(m_lnPen.brush().textureImage().width()/2, -m_lnPen.brush().textureImage().height()/2.);
    brush.setMatrix(matrix);
    pen.setBrush(brush);
    m_painter.setPen(pen);

    m_painter.save();
    m_painter.translate(p1);
    m_painter.rotate(alfa);
    m_painter.drawLine(0., 0., distance, 0.);
    if(m_lnColor.alpha() != 0)
    {
      pen.setBrush(Qt::NoBrush);
      pen.setColor(m_lnColor);
      m_painter.setPen(pen);
      m_painter.drawLine(0., 0., distance, 0.);
    }
    m_painter.restore();
  }
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiLineString* mline)
{
  std::size_t size = mline->getNumGeometries();

  for(size_t i = 0; i < size; ++i)
    draw(static_cast<const te::gm::LineString*>(mline->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::Polygon* poly)
{
  const std::size_t nRings = poly->getNumRings();

  assert(nRings > 0);

  QPainterPath path;

  std::vector<te::gm::LinearRing*> rings;

  for(register std::size_t i = 0; i != nRings; ++i)
  { 
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));
    rings.push_back(ring);

    const std::size_t nPoints = ring->getNPoints();

    assert(nPoints > 3);

    QPolygonF qpol(nPoints);

    memcpy(&(qpol[0]), (double*)(ring->getCoordinates()), 16 * nPoints);    
    path.addPolygon(qpol);
    //te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));
    //const std::size_t nPoints = ring->getNPoints();
    //assert(nPoints > 3);
    //rings.push_back(ring);

    //te::gm::Coord2D* coords = ring->getCoordinates();
    //std::size_t size = ring->getNPoints();
    //if(size <= 1)
    //  continue;

    //QPolygonF qpol;
    //QPointF p1(coords[0].x, coords[0].y);
    //qpol.append(p1);

    //double xMax = p1.x();
    //double yMax = p1.y();
    //double xMin = p1.x();
    //double yMin = p1.y();
    //QPointF p2;
    //for(register std::size_t i = 1; i != size; ++i)
    //{    
    //  p2.setX(coords[i].x);
    //  p2.setY(coords[i].y);

    //  xMax = qMax(xMax, p2.x());
    //  yMax = qMax(yMax, p2.y());
    //  xMin = qMin(xMin, p2.x());
    //  yMin = qMin(yMin, p2.y());

    //  if(m_matrix.map(p1).toPoint() == m_matrix.map(p2).toPoint())
    //    continue;
    //  qpol.append(p2);
    //  p1 = p2;
    //}

    //if(qpol.size() < 3)
    //{
    //  qpol.clear();
    //  qpol.append(QPointF(xMin, yMin));
    //  qpol.append(QPointF(xMax, yMin));
    //  qpol.append(QPointF(xMax, yMax));
    //  qpol.append(QPointF(xMin, yMax));
    //  qpol.append(QPointF(xMin, yMin));
    //}
    //path.addPolygon(qpol);
  }

  if(m_erase)
  {
    m_painter.setPen(Qt::NoPen);
    m_painter.setBrush(Qt::red);
    m_painter.drawPath(path);
  }
  else
  {
    // fill polygon
    m_painter.setPen(Qt::NoPen);
    if(m_polyImage && m_polyColor.alpha() != 255)
    {
      const te::gm::Envelope* envelope = poly->getMBR();
      QRectF recf(envelope->m_llx, envelope->m_lly, envelope->getWidth(), envelope->getHeight());
      QPointF pc = m_matrix.map(recf.center());
      int transx = pc.toPoint().x();
      int transy = pc.toPoint().y();

      QMatrix matrix = m_matrix.inverted();
      double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(m_polyImage->width());
      matrix.scale(scale, scale);
      if(m_polyRotatedImage)
      {
        transx = transx % m_polyRotatedImage->width();
        transy = transy % m_polyRotatedImage->height();
        matrix.translate(transx, transy);
        m_polyBrush.setMatrix(matrix);
        m_polyBrush.setTextureImage(*m_polyRotatedImage);
      }
      else
      {
        transx = transx % m_polyImage->width();
        transy = transy % m_polyImage->height();
        matrix.translate(transx, transy);
        m_polyBrush.setMatrix(matrix);
        m_polyBrush.setTextureImage(*m_polyImage);
      }

      m_painter.setBrush(m_polyBrush);

      if(m_polyContourPen.brush().style() == Qt::TexturePattern)
        m_painter.setPen(Qt::NoPen);
      else
        m_painter.setPen(m_polyContourPen);

      m_painter.drawPath(path);
    }

    m_polyDefaultBrush.setMatrix(this->getMatrix().inverted());
    m_painter.setBrush(m_polyDefaultBrush);

    if(m_polyContourPen.brush().style() == Qt::TexturePattern)
      m_painter.setPen(Qt::NoPen);
    else
      m_painter.setPen(m_polyContourPen);

    m_painter.drawPath(path);

    // draw contour
    if(m_polyContourPen.brush().style() == Qt::TexturePattern)
    {
      std::vector<te::gm::LinearRing*>::iterator it;
      for(it = rings.begin(); it != rings.end(); ++it)
        drawContour(*it);
    }
  }
}

void te::qt::widgets::Canvas::drawContour(const te::gm::LineString* line)
{
  te::gm::Coord2D* coords = line->getCoordinates();
  std::size_t size = line->getNPoints();
  if(size <= 1)
    return;

  bool drawed = false;

  m_polyContourPen.setColor(m_polyContourColor);
  m_painter.setPen(m_polyContourPen);
  m_painter.setBrush(Qt::NoBrush);

  QPointF p1(coords[0].x, coords[0].y);
  QPointF p2;

  for(register std::size_t i = 1; i != size; ++i)
  {    
    p2.setX(coords[i].x);
    p2.setY(coords[i].y);

    if(m_polyContourPen.brush().style() != Qt::TexturePattern)
    {
      drawed = true;
      m_painter.drawLine(p1, p2);   
    }
    else
    {
      double alfa, beta;
      QPointF pf1, pf2;
      QPoint dp1 = m_matrix.map(p1).toPoint();
      QPoint dp2 = m_matrix.map(p2).toPoint();
      int ddx = dp2.x() - dp1.x();
      int ddy = dp2.y() - dp1.y();
      if(ddx == 0 && ddy == 0)
      {
        drawed = false;
        continue;
      }

      drawed = true;
      double dx = p2.x() - p1.x();
      double dy = p2.y() - p1.y();
      double distance = sqrt((dx * dx) + (dy * dy));
      double scale = m_polyContourPen.widthF() / m_polyContourPen.brush().textureImage().height();

      if(ddx == 0.)
      {
        if(p2.y() >= p1.y())
          alfa = 90.;
        else
          alfa = 270.;
      }
      else if(ddy == 0.)
      {
        if(p2.x() >= p1.x())
          alfa = 0.;
        else
          alfa = 180.;
      }
      else
      {
        alfa = atan(dy/dx) * 180. / 3.14159265;
        if(dx < 0.)
          alfa += 180.;
        else if(alfa < 0 && dy < 0.)
          alfa += 360.;
        if(alfa == 360.)
          alfa = 0.;
      }

      if(alfa > 90. && alfa <= 270.)
      {
        pf1 = p2;
        pf2 = p1;
        beta = alfa + 180.;
        if(beta > 360.)
          beta -= 360.;
        if(beta == 360.)
          beta = 0.;
      }
      else
      {
        pf1 = p1;
        pf2 = p2;
        beta = alfa;
      }

      QBrush brush(m_polyContourPen.brush());
      QPen pen(m_polyContourPen);
//      pen.setJoinStyle(Qt::RoundJoin); // Qt::BevelJoin Qt::MiterJoin Qt::RoundJoin
      pen.setCapStyle(Qt::FlatCap); // Qt::RoundCap Qt::FlatCap Qt::SquareCap
      QMatrix matrix;
      matrix.scale(scale, scale);
      matrix.translate(m_polyContourPen.brush().textureImage().width()/2, -m_polyContourPen.brush().textureImage().height()/2.);
      brush.setMatrix(matrix);
      pen.setBrush(brush);
      m_painter.setPen(pen);

      m_painter.save();
      m_painter.translate(pf1);
      m_painter.rotate(beta);
      m_painter.drawLine(0., 0., distance, 0.);
      if(m_polyContourColor.alpha() != 0)
      {
        pen.setBrush(Qt::NoBrush);
        pen.setColor(m_polyContourColor);
        m_painter.setPen(pen);
        m_painter.drawLine(0., 0., distance, 0.);
      }
      m_painter.restore();
    }
    p1 = p2;
  }
  if(drawed == false)
  {
    double alfa, beta;
    QPointF pf1, pf2;
    QPoint dp1 = m_matrix.map(p1).toPoint();
    QPoint dp2 = m_matrix.map(p2).toPoint();
    int ddx = dp2.x() - dp1.x();
    int ddy = dp2.y() - dp1.y();
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double distance = sqrt((dx * dx) + (dy * dy));
    double scale = m_polyContourPen.widthF() / m_polyContourPen.brush().textureImage().height();

    if(ddx == 0.)
    {
      if(p2.y() >= p1.y())
        alfa = 90.;
      else
        alfa = 270.;
    }
    else if(ddy == 0.)
    {
      if(p2.x() >= p1.x())
        alfa = 0.;
      else
        alfa = 180.;
    }
    else
    {
      alfa = atan(dy/dx) * 180. / 3.14159265;
      if(dx < 0.)
        alfa += 180.;
      else if(alfa < 0 && dy < 0.)
        alfa += 360.;
      if(alfa == 360.)
        alfa = 0.;
    }

    if(alfa > 90. && alfa <= 270.)
    {
      pf1 = p2;
      pf2 = p1;
      beta = alfa + 180.;
      if(beta > 360.)
        beta -= 360.;
      if(beta == 360.)
        beta = 0.;
    }
    else
    {
      pf1 = p1;
      pf2 = p2;
      beta = alfa;
    }

    QBrush brush(m_polyContourPen.brush());
    QPen pen(m_polyContourPen);
//      pen.setJoinStyle(Qt::RoundJoin); // Qt::BevelJoin Qt::MiterJoin Qt::RoundJoin
    pen.setCapStyle(Qt::FlatCap); // Qt::RoundCap Qt::FlatCap Qt::SquareCap
    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.translate(m_polyContourPen.brush().textureImage().width()/2, -m_polyContourPen.brush().textureImage().height()/2.);
    brush.setMatrix(matrix);
    pen.setBrush(brush);
    m_painter.setPen(pen);

    m_painter.save();
    m_painter.translate(pf1);
    m_painter.rotate(beta);
    m_painter.drawLine(0., 0., distance, 0.);
    if(m_polyContourColor.alpha() != 0)
    {
      pen.setBrush(Qt::NoBrush);
      pen.setColor(m_polyContourColor);
      m_painter.setPen(pen);
      m_painter.drawLine(0., 0., distance, 0.);
    }
    m_painter.restore();
  }
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiPolygon* mpoly)
{
  const std::size_t size = mpoly->getNumGeometries();
  for(std::size_t i = 0; i < size; ++i)
    draw(static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::GeometryCollection* g)
{
  const std::size_t size = g->getNumGeometries();
  for(size_t i = 0; i < size; ++i)
    draw(g->getGeometryN(i));
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiSurface* g)
{
  const std::size_t size = g->getNumGeometries();
  for (size_t i = 0; i < size; ++i)
    draw(g->getGeometryN(i));
}

void te::qt::widgets::Canvas::save(const char* fileName, te::map::ImageType t, int quality, int /*fg*/) const
{
  int devType = m_painter.device()->devType();

  if(devType != QInternal::Image && devType != QInternal::Pixmap)
    return;

  static_cast<QPixmap*>(m_painter.device())->save(fileName, GetFormat(t), quality);
}

char* te::qt::widgets::Canvas::getImage(te::map::ImageType t, std::size_t& size, int quality, int /*fg*/) const
{
  int devType = m_painter.device()->devType();

  if(devType != QInternal::Image && devType != QInternal::Pixmap)
    return 0;

  QByteArray bytes;
  QBuffer buffer(&bytes);
  buffer.open(QIODevice::WriteOnly);

  switch(t)
  {
    case te::map::PNG:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_pngFmt);
    break;

    case te::map::JPEG:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_jpegFmt, quality);
    break;

    case te::map::GIF:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_gifFmt);
    break;

    case te::map::BMP:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_bmpFmt);
    break;

    case te::map::XPM:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_xpmFmt);
    break;

    case te::map::XBM:
      static_cast<QPixmap*>(m_painter.device())->save(&buffer, Globals::sm_xbmFmt);
    break;

    default:
      return 0;
  }

  int nbytes = bytes.size();

  char* result = new char[nbytes];

  memcpy(result, bytes.data(), nbytes);

  size = nbytes;

  return result;
}

te::color::RGBAColor** te::qt::widgets::Canvas::getImage(const int x, const int y, const int w, const int h) const
{
  int devType = m_painter.device()->devType();

  if(devType != QInternal::Image && devType != QInternal::Pixmap)
    return 0;

  te::color::RGBAColor** colors = 0;
  QImage img;
  if(devType == QInternal::Pixmap)
    img = static_cast<QPixmap*>(m_painter.device())->toImage();
  else
    img = *(static_cast<QImage*>(m_painter.device()));

  if(x == 0 && y == 0 && w == 0 && y == 0)
  {
    int width = img.width();
    int height = img.height();

    colors = new te::color::RGBAColor*[height];
    for(int i = 0; i < height; ++i)
    {
      colors[i] = new te::color::RGBAColor[width];

      unsigned char* u = img.scanLine(i);

      for(int j = 0; j < width; ++j)
      {
        QRgb* qrgb = (QRgb*)(u + (j << 2));
        QRgb value = *qrgb;
        te::color::RGBAColor rgba(qRed(value), qGreen(value), qBlue(value), qAlpha(value));
        colors[i][j] = rgba;
      }
    }
  }
  else
  {
    img = img.copy(x, y, w, h);

    colors = new te::color::RGBAColor*[h];
    for(int i = 0; i < h; ++i)
    {
      colors[i] = new te::color::RGBAColor[w];

      unsigned char* u = img.scanLine(i);

      for(int j = 0; j < w; ++j)
      {
        QRgb* qrgba = (QRgb*)(u+(j<<2));
        QRgb value = *qrgba;
        te::color::RGBAColor rgba(qRed(value), qGreen(value), qBlue(value), qAlpha(value));
        colors[i][j] = rgba;
      }
    }
  }

  return colors;
}

void te::qt::widgets::Canvas::freeImage(char* img) const
{
  delete [] img;
}

void te::qt::widgets::Canvas::drawImage(char* src, std::size_t size, te::map::ImageType t)
{
  drawImage(0, 0, src, size, t);
}

void te::qt::widgets::Canvas::drawImage(te::color::RGBAColor** src, int w, int h)
{
  drawImage(0, 0, src, w, h);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, char* src, std::size_t size, te::map::ImageType t)
{
  QPixmap pix;

  const char* f = GetFormat(t);
  pix.loadFromData((unsigned char*)src, size, f);

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawPixmap(x, y, pix);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, te::color::RGBAColor** src, int w, int h)
{
  QImage img(w, h, QImage::Format_ARGB32);

  for(int l = 0; l < h; ++l)
  {
    for(int c = 0; c < w; ++c)
    {
      QRgb val = qRgba(src[l][c].getRed(), src[l][c].getGreen(), src[l][c].getBlue(), src[l][c].getAlpha());

      img.setPixel(c, l, val);
    }
  }

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawImage(x, y, img);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, char* src, std::size_t size, te::map::ImageType t)
{
  QPixmap pix;

  const char* f = GetFormat(t);
  pix.loadFromData((unsigned char*)src, size, f);

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawPixmap(x, y, w, h, pix);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int srcw, int srch)
{
  QImage img(srcw, srch, QImage::Format_RGB32);

  for(int l = 0; l < srch; ++l)
    for(int c = 0; c < srcw; ++c)
      img.setPixel(c, l, src[l][c].getRgba());

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawImage(QRect(x, y, w, h), img, QRect(0, 0, srcw, srch));
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, char* src, std::size_t size, te::map::ImageType t, int sx, int sy, int sw, int sh)
{
  QPixmap pix;

  const char* f = GetFormat(t);
  pix.loadFromData((unsigned char*)src, size, f);

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawPixmap(x, y, w, h, pix, sx, sy, sw, sh);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int sx, int sy, int sw, int sh)
{
  QImage img(sw, sh, QImage::Format_RGB32);

  int ssh = sy + sh;
  int ssw = sx + sw;

  for(int l = sy, li = 0; l < ssh; ++l, ++li)
    for(int c = sx, ci = 0; c < ssw; ++c, ++ci)
      img.setPixel(ci, li, src[l][c].getRgba());

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawImage(QRect(x, y, w, h), img, QRect(0, 0, sw, sh));
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, te::rst::Raster* src, int opacity)
{
  int sw = src->getNumberOfColumns();
  int sh = src->getNumberOfRows();

  drawImage(x, y, sw, sh, src, 0, 0, sw, sh, opacity);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, te::rst::Raster* src, int sx, int sy, int sw, int sh, int opacity)
{
  // Defines QImage size
  int iw = std::min(sw, w);
  int ih = std::min(sh, h);

  QImage img(iw, ih, QImage::Format_ARGB32);
  double pr, pg, pb;
  te::color::RGBAColor* pixel;

  // Checks if undersampling is needed
  double sl = std::max(1.0, sh/(double)h);
  double sr = std::max(1.0, sw/(double)w);

  int l, r;
  double dl = sy;
  for(int li = 0; li < ih; dl+=sl, ++li)
  {
    double dr = sx;
    for(int ri = 0; ri < iw; dr+=sr, ++ri)
    {
      r = static_cast<int> (dr);
      l = static_cast<int> (dl);
      src->getValue(r, l, pr, 0);
      src->getValue(r, l, pg, 1);
      src->getValue(r, l, pb, 2);

      pixel = new te::color::RGBAColor((int)pr, (int)pg, (int)pb, opacity);
      QRgb val = qRgba(pixel->getRed(), pixel->getGreen(), pixel->getBlue(), pixel->getAlpha());

      img.setPixel(ri, li, val);

      delete pixel;
    }
  }

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawImage(QRect(x, y, w, h), img, QRect(0, 0, iw, ih));
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawPixel(int x, int y)
{
  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawPoint(x, y);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawPixel(int x, int y, const te::color::RGBAColor& color)
{
  QColor c(color.getRgba());
  c.setAlpha(qAlpha(color.getRgba()));

  m_painter.setPen(c);

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawPoint(x, y);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::drawText(int x, int y,
                                       const std::string& txt,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  QPoint p(x, y);
  drawText(p, txt, angle, hAlign, vAlign);
}

void te::qt::widgets::Canvas::drawText(const te::gm::Point* p,
                                       const std::string& txt,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  drawText(p->getX(), p->getY(), txt, angle, hAlign, vAlign);
}

void te::qt::widgets::Canvas::drawText(const double& x, const double& y,
                                       const std::string& txt,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  QPointF pf(x, y);
  pf = m_matrix.map(pf);
  QPoint p = pf.toPoint();
  drawText(p, txt, angle, hAlign, vAlign);
}

void te::qt::widgets::Canvas::draw(const te::at::Text* txt)
{
  std::size_t size = txt->getElements().size();

  if(size == 0)
    return;
 
  std::size_t i = 0;

  while(txt->getElements()[i]->getAttributes() == 0 && i < size)
    ++i;

  te::at::HorizontalAlignment hAlign = te::at::Start;
  te::at::VerticalAlignment vAlign = te::at::Baseline;

  while(i < size)
  {
    // set attributes
    te::at::Element* element = txt->getElements()[i];
    te::at::Attributes* atr = (te::at::Attributes*)element->getAttributes();
    if(atr)
    {
      // get font family
      std::string family = ((te::at::Attributes*)atr)->getFamily();
      m_font.setFamily(family.c_str());

      // get font point size
      const double& pointSize = atr->getPointSize();
      setTextPointSize(pointSize);

      // get font weight
      const te::at::FontWeight& weight = atr->getWeight();
      if(weight == te::at::Light)
        m_font.setWeight(QFont::Light);
      else if(weight == te::at::NormalWeight)
        m_font.setWeight(QFont::Normal);
      else if(weight == te::at::DemiBold)
        m_font.setWeight(QFont::DemiBold);
      else if(weight == te::at::Bold)
        m_font.setWeight(QFont::Bold);
      else
        m_font.setWeight(QFont::Black);

      // get font style
      const te::at::FontStyle& style = atr->getStyle();
      if(style == te::at::NormalStyle)
        m_font.setStyle(QFont::StyleNormal);
      else if(style == te::at::Oblique)
        m_font.setStyle(QFont::StyleOblique);
      else
        m_font.setStyle(QFont::StyleItalic);

      // get font decoration
      const te::at::TextDecoration& decoration = atr->getDecoration();
      if(decoration == te::at::None)
      {
        m_font.setOverline(false);
        m_font.setUnderline(false);
        m_font.setStrikeOut(false);
      }
      else if(decoration == te::at::Overline)
        m_font.setOverline(true);
      else if(decoration == te::at::Underline)
        m_font.setUnderline(true);
      else
        m_font.setStrikeOut(true);

      m_painter.setFont(m_font);

      // get letter spacing
      m_txtLetterSpacing = atr->getLetterSpacing();
      // nao sei o que fazer ????????????

      // get word spacing
      m_txtWordSpacing = atr->getLetterSpacing();
      // nao sei o que fazer ????????????

      // get text color and opacity
      const std::string& cor = atr->getTextColor();
      QColor qcor(cor.c_str());
      int opacity = atr->getTextOpacity() * 255.;
      qcor.setAlpha(opacity);
      m_txtBrush = QBrush(qcor);

      // get stroke color and opacity
      const std::string& sCor = atr->getTextStrokeColor();
      QColor qscor(sCor.c_str());
      int sOpacity = atr->getTextStrokeOpacity() * 255.;
      qscor.setAlpha(sOpacity);
      m_txtContourPen = QPen(qscor);

      // get stroke width
      m_txtContourPen.setWidth(10. * atr->getTextStrokeWidth() * (double)getResolution() / 72.);

      // get horizontal alignment
      hAlign = atr->getHorizontalAlignment();

      // get vertical alignment
      vAlign = atr->getVerticalAlignment();

      // get multi line justification
      //m_txtLineJustification = atr->getLineJustification();
      // parece haver confusao entre horizontal alignment e line justification ????????????
      // por enquanto estou usando somente horizontal aligment.

      // get multi line spacing
      m_txtLineSpacing = atr->getLineSpacing();
      // como usar isto (0...1) ????????????
    }

    if(element->getLeaderLine() == 0) // without leaderLine
    {
      // supondo que location e' do tipo ponto
      const te::gm::Point* pt = static_cast<const te::gm::Point*>(element->getLocation());
      std::string text = element->getValue();
      size_t p = text.find("\n");
      double hline = 0;
      te::gm::Point* pclone = 0;
      if(p != std::string::npos)
      {
        QFontMetrics fm = QFontMetrics(m_font);
        QRectF rec(fm.boundingRect("A"));
        hline = QRectF(m_matrix.inverted().mapRect(rec)).height();
        hline = hline + hline * m_txtLineSpacing; // no chute ????????
        pclone = (te::gm::Point*)pt->clone();
      }
      while(p != std::string::npos) // multiline text
      {
        std::string t = text.substr(0, p);
        drawText(pclone, t, 0., hAlign, vAlign);
        text = text.substr(p+1);
        p = text.find("\n");
        pclone->setY(pclone->getY() - hline); // next line
      }
      if(pclone) // multiline
        drawText(pclone, text, 0., hAlign, vAlign);
      else
        drawText(pt, text, 0., hAlign, vAlign);

      delete pclone;
    }
    else // with leaderLine
    {
    }

    ++i;
  }
}

te::gm::Polygon* te::qt::widgets::Canvas::getTextBoundary(int x, int y, const std::string& txt, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  QPoint p(x, y);
  return getTextBoundary(p, txt, angle, hAlign, vAlign);
}

te::gm::Polygon* te::qt::widgets::Canvas::getTextBoundary(const te::gm::Point* p, const std::string& txt, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  return getTextBoundary(p->getX(), p->getY(), txt, angle, hAlign, vAlign);
}

te::gm::Polygon* te::qt::widgets::Canvas::getTextBoundary(const double& x, const double& y, const std::string& txt, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  QPointF pf(x, y);
  pf = m_matrix.map(pf);
  QPoint p = pf.toPoint();
  return getTextBoundary(p, txt, angle, hAlign, vAlign);
}

void te::qt::widgets::Canvas::setTextColor(const te::color::RGBAColor& color)
{
  m_txtBrush.setStyle(Qt::SolidPattern);
  QColor cor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  m_txtBrush.setColor(cor);
}

void te::qt::widgets::Canvas::setTextOpacity(int opacity)
{
  QColor cor(m_txtBrush.color().red(), m_txtBrush.color().green(), m_txtBrush.color().blue(), opacity);
  m_txtBrush.setColor(cor);
}

void te::qt::widgets::Canvas::setFontFamily(const std::string& family)
{
  m_font.setFamily(family.c_str());
}

void te::qt::widgets::Canvas::setTextPointSize(double psize)
{
  double resolution = 0.0;

  if(m_painter.device()->devType() == QInternal::Printer)
    resolution = static_cast<QPrinter*>(m_painter.device())->resolution();
  else
    resolution = m_painter.device()->logicalDpiY();

  double PointSize = psize * resolution / 72.0;

  m_font.setPointSizeF(PointSize);
}

void te::qt::widgets::Canvas::setTextStyle(te::at::FontStyle style)
{
  m_font.setStyle((QFont::Style)style);
}

void te::qt::widgets::Canvas::setTextWeight(te::at::FontWeight weight)
{
  int wg = weight;

  if(wg > 99)
    wg = 99;

  m_font.setWeight((QFont::Weight)wg);
}

void te::qt::widgets::Canvas::setTextStretch(std::size_t stretch)
{
  m_font.setStretch(stretch);
}

void te::qt::widgets::Canvas::setTextUnderline(bool b)
{
  m_font.setUnderline(b);
}

void te::qt::widgets::Canvas::setTextOverline(bool b)
{
  m_font.setOverline(b);
}

void te::qt::widgets::Canvas::setTextStrikeOut(bool b)
{
  m_font.setStrikeOut(b);
}

void te::qt::widgets::Canvas::setTextDecorationColor(const te::color::RGBAColor& /*color*/)
{
}

void te::qt::widgets::Canvas::setTextDecorationWidth(int /*width*/)
{
}

void te::qt::widgets::Canvas::setTextContourColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));
  m_txtContourPen.setColor(cor);
}

void te::qt::widgets::Canvas::setTextContourEnabled(bool b)
{
  m_txtContourEnabled = b;
}

void te::qt::widgets::Canvas::setTextContourOpacity(int opacity)
{
  QColor cor(m_txtContourPen.color().red(), m_txtContourPen.color().green(), m_txtContourPen.color().blue(), opacity);
  m_txtContourPen.setColor(cor);
}

void te::qt::widgets::Canvas::setTextContourWidth(int width)
{
  m_txtContourPen.setWidth(width);
}

void te::qt::widgets::Canvas::setTextJustification(te::at::LineJustification /*just*/)
{
}

void te::qt::widgets::Canvas::setTextMultiLineSpacing(int spacing)
{
  m_txtLineSpacing = spacing;
}

void te::qt::widgets::Canvas::setPointColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));
  if(m_ptColor == cor)
    return;

  m_ptColor = cor;
  m_ptPen.setColor(m_ptColor);

  if(m_ptColorFrom != m_ptColor && m_ptImg)
    createPointPatterns();
}

void te::qt::widgets::Canvas::setPointWidth(int w)
{
  if(w == m_ptWidth)
    return;
  
  m_ptWidth = w;
  QRectF rec(QPoint(0., 0.), QPoint(m_ptWidth, m_ptWidth));
  rec = m_matrix.inverted().mapRect(rec);
  m_ptPen.setWidthF(rec.width());

  if(m_ptImg)
  {
    double scale = (double)m_ptWidth / (double)m_ptImg->width();
    int h = m_ptImg->height() * scale;
    QImage* ima = new QImage(m_ptImg->scaled(m_ptWidth, h));
    delete m_ptImg;
    m_ptImg = ima;
  }
  createPointPatterns();
}

void te::qt::widgets::Canvas::setPointPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  delete m_ptImg;
  m_ptImg = 0;

  if(pattern == 0 || ncols == 0 || nrows == 0)
    return;

  m_ptImg = GetImage(pattern, ncols, nrows);
  int width = m_ptImg->width();
  m_ptWidth = width;

  createPointPatterns();
}

void te::qt::widgets::Canvas::setPointPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  delete m_ptImg;
  m_ptImg = 0;

  if(pattern == 0 || size == 0)
    return;

  m_ptImg = new QImage();

  const char* format = te::qt::widgets::GetFormat(t);

  if(m_ptImg->loadFromData((const uchar*)pattern, size, format) == false)
  {
    delete m_ptImg;
    m_ptImg = 0;
    return;
  }

  int width = m_ptImg->width();
  m_ptWidth = width;
  
  createPointPatterns();
}

void te::qt::widgets::Canvas::setPointPatternRotation(const double& angle)
{
  m_ptRotation = angle;
  createPointPatterns();
}

void te::qt::widgets::Canvas::setPointPatternOpacity(int opacity)
{
  if(m_ptImg == 0)
    return;

  updateAlpha(*m_ptImg, opacity);
  createPointPatterns();
}

void te::qt::widgets::Canvas::setLineColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));

  //if(cor.alpha() == 255)
  //{
  //  m_lnPen.setColor(cor);
  //  m_lnColor = QColor(0, 0, 0, 0);
  //}
  //else
    m_lnColor = cor;
}

void te::qt::widgets::Canvas::setLinePattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  if(pattern == 0)
  {
    m_lnPen.setBrush(Qt::NoBrush);
    return;
  }

  QImage* img = GetImage(pattern, ncols, nrows);

  if(img->isNull() == false)
  {
    QBrush brush;
    brush.setTextureImage(*img);
    m_lnPen.setBrush(brush);
  }
  delete img;
}

void te::qt::widgets::Canvas::setLinePattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  if(pattern == 0)
  {
    m_lnPen.setBrush(Qt::NoBrush);
    return;
  }

  QImage img;

  const char* format = te::qt::widgets::GetFormat(t);

  bool result = img.loadFromData((const uchar*)pattern, size, format);

  if(result && img.isNull() == false)
  {
    QBrush brush;
    brush.setTextureImage(img);
    m_lnPen.setBrush(brush);
  }
}

void te::qt::widgets::Canvas::setLinePatternRotation(const double& angle)
{
  QBrush brush = m_lnPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  QMatrix matrix;
  matrix.translate(img.width()/2, img.height()/2);
  matrix.rotate(angle);
  QImage imgRot = img.transformed(matrix);

  brush.setTextureImage(imgRot);
  m_lnPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLinePatternOpacity(int opacity)
{
  QBrush brush = m_lnPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  updateAlpha(img, opacity);

  brush.setTextureImage(img);
  m_lnPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLineDashStyle(te::map::LineDashStyle style)
{
  m_lnPen.setStyle((Qt::PenStyle)style);
}

void te::qt::widgets::Canvas::setLineDashStyle(const std::vector<double>& style)
{
  setLineDashStyle(m_lnPen, style);
}

void te::qt::widgets::Canvas::setLineCapStyle(te::map::LineCapStyle style)
{
  m_lnPen.setCapStyle((Qt::PenCapStyle)style);
}

void te::qt::widgets::Canvas::setLineJoinStyle(te::map::LineJoinStyle style)
{
  m_lnPen.setJoinStyle((Qt::PenJoinStyle)style);
}

void te::qt::widgets::Canvas::setLineWidth(int w)
{
  QRectF rec(QPoint(0., 0.), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_lnPen.setWidthF(rec.width());
}

void te::qt::widgets::Canvas::setPolygonFillColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));

  QBrush b(cor, Qt::SolidPattern);

  setPolygonFillColor(b);
}

void te::qt::widgets::Canvas::setPolygonFillColor(const QBrush& color)
{
  m_polyDefaultBrush = color;
  m_polyColor = color.color();
}


void te::qt::widgets::Canvas::setPolygonFillPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  if(pattern == 0)
  {
    m_polyBrush.setStyle(Qt::NoBrush);
    delete m_polyImage;
    delete m_polyRotatedImage;
    m_polyImage = 0;
    m_polyRotatedImage = 0;
    return;
  }

  if(m_polyImage)
    delete m_polyImage;

  m_polyImage = GetImage(pattern, ncols, nrows);
  if(m_polyImage->isNull())
  {
    delete m_polyImage;
    m_polyImage = 0;
  }
  m_polyPatternWidth = m_polyImage->width();

  //QImage* img = GetImage(pattern, ncols, nrows);

  //if(img->isNull() == false)
  //{
  //  //QMatrix matrix = m_matrix.inverted();
  //  //double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img->width());
  //  //matrix.scale(scale, scale);
  //  //m_polyBrush.setStyle(Qt::TexturePattern);
  //  //m_polyBrush.setMatrix(matrix);
  //  //m_polyBrush.setTextureImage(*img);
  //}
  //delete img;
}

void te::qt::widgets::Canvas::setPolygonFillPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  if(pattern == 0)
  {
    m_polyBrush.setStyle(Qt::NoBrush);
    delete m_polyImage;
    delete m_polyRotatedImage;
    m_polyImage = 0;
    m_polyRotatedImage = 0;
    return;
  }
  delete m_polyImage;

  m_polyImage = new QImage();
  const char* format = te::qt::widgets::GetFormat(t);
  bool result = m_polyImage->loadFromData((const uchar*)pattern, size, format);
  if(result == false)
  {
    delete m_polyImage;
    m_polyImage = 0;
  }

  //QImage img;

  //const char* format = te::qt::widgets::GetFormat(t);

  //bool result = img.loadFromData((const uchar*)pattern, size, format);

  //if(result && img.isNull() == false)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img.width());
  //  matrix.scale(scale, scale);
  //  m_polyBrush.setStyle(Qt::TexturePattern);
  //  m_polyBrush.setMatrix(matrix);
  //  m_polyBrush.setTextureImage(img);
  //}
}

void te::qt::widgets::Canvas::setPolygonPatternWidth(int w)
{
  m_polyPatternWidth = w;

  if(m_polyPatternWidth == 0.)
    m_polyPatternWidth = 1.;

  //QImage img = m_polyBrush.textureImage();

  //if(img.isNull() == false)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img.width());
  //  matrix.scale(scale, scale);
  //  m_polyBrush.setMatrix(matrix);
  //}
}

void te::qt::widgets::Canvas::setPolygonPatternRotation(const double& angle)
{
  if(m_polyImage)
  {
    QMatrix matrix;
    matrix.translate((double)m_polyImage->width()/2., (double)m_polyImage->height()/2.);
    matrix.rotate(angle);

    if(m_polyRotatedImage == 0)
      m_polyRotatedImage = new QImage();
    *m_polyRotatedImage = m_polyImage->transformed(matrix);
  }
}

void te::qt::widgets::Canvas::setPolygonPatternOpacity(int opacity)
{
  QImage img = m_polyBrush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  if(m_polyImage)
    updateAlpha(*m_polyImage, opacity);
  if(m_polyRotatedImage)
    updateAlpha(*m_polyRotatedImage, opacity);

  //m_polyBrush.setTextureImage(img);
}

void te::qt::widgets::Canvas::setPolygonContourColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));

  //if(cor.alpha() == 255)
  //{
  //  m_polyContourPen.setColor(cor);
  //  m_polyContourColor = QColor(0, 0, 0, 0);
  //}
  //else
  //  m_polyContourColor = cor;
  m_polyContourPen.setColor(cor);
  m_polyContourColor = cor;
}

void te::qt::widgets::Canvas::setPolygonContourPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  if(pattern == 0)
  {
    m_polyContourPen.setBrush(Qt::NoBrush);
    return;
  }

  QImage* img = GetImage(pattern, ncols, nrows);

  if(img->isNull() == false)
  {
    QBrush brush;
    brush.setTextureImage(*img);
    m_polyContourPen.setBrush(brush);
  }
  delete img;
}

void te::qt::widgets::Canvas::setPolygonContourPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  if(pattern == 0)
  {
    m_polyContourPen.setBrush(Qt::NoBrush);
    return;
  }

  QImage img;

  const char* format = te::qt::widgets::GetFormat(t);

  bool result = img.loadFromData((const uchar*)pattern, size, format);

  if(result && img.isNull() == false)
  {
    QBrush brush;
    brush.setTextureImage(img);
    m_polyContourPen.setBrush(brush);
  }
}

void te::qt::widgets::Canvas::setPolygonContourWidth(int w)
{
  QRectF rec(QPoint(0., 0.), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_polyContourPen.setWidthF(rec.width());
}

void te::qt::widgets::Canvas::setPolygonContourPatternRotation(const double& angle)
{
  QBrush brush = m_polyContourPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  QMatrix matrix;
  matrix.translate(img.width()/2, img.height()/2);
  matrix.rotate(angle);
  QImage imgRot = img.transformed(matrix);

  brush.setTextureImage(imgRot);
  m_polyContourPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourPatternOpacity(int opacity)
{
  QBrush brush = m_polyContourPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  updateAlpha(img, opacity);

  brush.setTextureImage(img);
  m_polyContourPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourDashStyle(te::map::LineDashStyle style)
{
  m_polyContourPen.setStyle((Qt::PenStyle)style);
}

void te::qt::widgets::Canvas::setPolygonContourDashStyle(const std::vector<double>& style)
{
  setLineDashStyle(m_polyContourPen, style);
}

void te::qt::widgets::Canvas::setPolygonContourCapStyle(te::map::LineCapStyle style)
{
  m_polyContourPen.setCapStyle((Qt::PenCapStyle)style);
}

void te::qt::widgets::Canvas::setPolygonContourJoinStyle(te::map::LineJoinStyle style)
{
  m_polyContourPen.setJoinStyle((Qt::PenJoinStyle)style);
}

void te::qt::widgets::Canvas::drawText(const QPoint& p,
                                       const std::string& txt,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  if(txt.empty())
    return;

  double ax, ay; // alignment position
  QString qtx(txt.c_str());
  QPainterPath path;
  QFontMetrics fm(m_font);
  QRectF rec(fm.boundingRect(qtx));

  if(hAlign == te::at::Start)
    ax = rec.left();
  else if(hAlign == te::at::CenterH)
    ax = rec.center().x();
  else
    ax = rec.right();

  if(vAlign == te::at::Bottom)
    ay = rec.bottom();
  else if(vAlign == te::at::CenterV)
    ay = rec.center().y();
  else if(vAlign == te::at::Top)
    ay = rec.top();
  else
    ay = 0.;

  path.addText(-ax, -ay, m_font, qtx);

  m_painter.resetMatrix();
  m_painter.translate(p);
  if(angle != 0.)
    m_painter.rotate(angle);

  if(m_txtContourEnabled)
  {
    m_painter.setBrush(Qt::NoBrush);
    m_painter.setPen(m_txtContourPen);
    m_painter.drawPath(path);
  }
  
  m_painter.setPen(Qt::NoPen);
  m_painter.fillPath(path, m_txtBrush);

  m_painter.drawPath(path);

  m_painter.setMatrix(m_matrix);
}

te::gm::Polygon* te::qt::widgets::Canvas::getTextBoundary(const QPoint& p, const std::string& txt, float angle,
                                                          te::at::HorizontalAlignment hAlign,
                                                          te::at::VerticalAlignment vAlign)
{
  if(txt.empty())
    return 0;

  double ax, ay; // alignment position
  QString qtx(txt.c_str());
  QPainterPath path;
  QFontMetrics fm(m_font);
  QRectF rec(fm.boundingRect(qtx));
  QRectF wrec(m_matrix.inverted().mapRect(rec));

  if(hAlign == te::at::Start)
    ax = wrec.left();
  else if(hAlign == te::at::CenterH)
    ax = wrec.center().x();
  else
    ax = wrec.right();

// reverse top-bottom
  if(vAlign == te::at::Bottom)
    ay = wrec.top();
  else if(vAlign == te::at::CenterV)
    ay = wrec.center().y();
  else if(vAlign == te::at::Top)
    ay = wrec.bottom();
  else // Baseline
    ay = m_matrix.inverted().map(QPoint(0, 0)).y();

  wrec.translate(-ax, -ay); // rotation point on text alignment
  QPolygonF polf(wrec);

  if(angle != 0.)
  {
    QMatrix m;
    m.rotate(-angle);
    polf = m.map(polf);
  }
  QPointF wp = m_matrix.inverted().map(QPointF(p));
  polf.translate(wp.x(), wp.y()); // translate to entry point

//converter QPolygon para te::gm::Polygon. Como fazer isso?
  te::gm::LinearRing* lr = new te::gm::LinearRing(4, te::gm::LineStringType);
  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType);
  poly->setRingN(0, lr);
  lr->setPoint(0, polf.at(0).x(), polf.at(0).y());
  lr->setPoint(1, polf.at(1).x(), polf.at(1).y());
  lr->setPoint(2, polf.at(2).x(), polf.at(2).y());
  lr->setPoint(3, polf.at(3).x(), polf.at(3).y());
  return poly;
}

QPixmap* te::qt::widgets::Canvas::getPixmap() const
{
  if(m_painter.device()->devType() == QInternal::Pixmap)
    return static_cast<QPixmap*>(m_painter.device());
  else
    return 0;
}

QImage* te::qt::widgets::Canvas::getImage() const
{
  if(m_painter.device()->devType() == QInternal::Image)
    return static_cast<QImage*>(m_painter.device());
  else
    return 0;
}

QPaintDevice* te::qt::widgets::Canvas::getDevice() const
{
  return m_painter.device();
}

void te::qt::widgets::Canvas::setDevice(QPaintDevice* device, bool takeOwnerShip)
{
  m_painter.end();

  if(m_isDeviceOwner)
    delete m_painter.device();

  m_isDeviceOwner = takeOwnerShip;
  m_painter.begin(device);
}

int te::qt::widgets::Canvas::getResolution()
{
  if(m_painter.device()->devType() == QInternal::Printer)
    return static_cast<QPrinter*>(m_painter.device())->resolution();
  else
    return m_painter.device()->logicalDpiX();
}

QMatrix te::qt::widgets::Canvas::getMatrix()
{
  return m_matrix;
}

void  te::qt::widgets::Canvas::setRenderHint(QPainter::RenderHint hint, bool on)
{
  m_painter.setRenderHint(hint, on);
}

QPainter* te::qt::widgets::Canvas::getPainter()
{
  return &m_painter;
}

void te::qt::widgets::Canvas::setLineDashStyle(QPen& pen, const std::vector<double>& style)
{
  assert(style.size() % 2 == 0);
  QVector<qreal> pattern;
  for(std::size_t i = 0; i < style.size(); ++i)
    pattern << style[i];
  pen.setDashPattern(pattern);
}

void te::qt::widgets::Canvas::updateAlpha(QImage& img, const int& opacity)
{
  int o = opacity << 24;
  for(int i = 0; i < img.height(); ++i)
  {
    unsigned char* u = img.scanLine(i);
    for(int j = 0; j < img.width(); ++j)
    {
      QRgb* v = (QRgb*)(u + (j << 2));
      if(qAlpha(*v) <= opacity)
        continue;
      *v = *v & 0x00ffffff;
      *v = o | *v;
    }
  }
}

void te::qt::widgets::Canvas::createPointPatterns()
{
  delete m_ptSelectionPatternImg;
  delete m_ptClearPatternImg;
  delete m_ptImgRotated;
  m_ptSelectionPatternImg = 0;
  m_ptClearPatternImg = 0;
  m_ptImgRotated = 0;

  if(m_ptImg == 0)
    return;

  QMatrix m;
  m.rotate(m_ptRotation);
  m_ptImgRotated = new QImage(m_ptImg->transformed(m));

  m_ptColorFrom = m_ptColor;

  double a = (double)m_ptColor.alpha() / (double)255;
  double b = (double)(255 - m_ptColor.alpha()) / (double)255;
  int width = m_ptImgRotated->width();
  int height = m_ptImgRotated->height();
  m_ptSelectionPatternImg = new QImage(width, height, QImage::Format_ARGB32);
  m_ptClearPatternImg = new QImage(width, height, QImage::Format_ARGB32);
  for(int i = 0; i < height; ++i)
  {
    unsigned char* u = m_ptImgRotated->scanLine(i);
    unsigned char* uu = m_ptSelectionPatternImg->scanLine(i);
    unsigned char* uuu = m_ptClearPatternImg->scanLine(i);
    for(int j = 0; j < width; ++j)
    {
      QRgb* v = (QRgb*)(u + (j << 2));
      QRgb* vv = (QRgb*)(uu + (j << 2));
      QRgb* vvv = (QRgb*)(uuu + (j << 2));
      if(qAlpha(*v) == 0)
      {
        *vv = qRgba(0, 0, 0, 0);
        *vvv = qRgba(0, 0, 0, 0);
      }
      else
      {
        int red = (int)((double)m_ptColor.red() * a + (double)qRed(*v) * b);
        int green = (int)((double)m_ptColor.green() * a + (double)qGreen(*v) * b);
        int blue = (int)((double)m_ptColor.blue() * a + (double)qBlue(*v) * b);
        *vv = qRgba(red, green, blue, qAlpha(*v));
        *vvv = qRgba(255, 255, 255, 255);
      }
    }
  }
}

void te::qt::widgets::Canvas::setEraseMode()
{
  m_painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
  m_erase = true;
}

void te::qt::widgets::Canvas::setNormalMode()
{
  m_painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  m_erase = false;
}

void te::qt::widgets::Canvas::setMatrix(const QMatrix& matrix )
{
  m_matrix = matrix;

  m_painter.setMatrix(m_matrix);
}
