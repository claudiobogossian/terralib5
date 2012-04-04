/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Canvas.cpp

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
#include <QtCore/QBuffer>
#include <QtGui/QBitmap>
#include <QtGui/QImage>
#include <QtGui/QPaintEngine>
#include <QtGui/QPixmap>
#include <QtGui/QPrinter>
#include <QtGui/QResizeEvent>

// STL
#include <cassert>
#include <algorithm>

te::qt::widgets::Canvas::Canvas(int w, int h)
  : m_isDeviceOwner(true),
    m_bgColor(QColor(255, 255, 255, 0)),
    m_ptColor(QColor(0, 0, 0, 255)),
    m_ptImg(0),
    m_ptMarkerType(te::map::MarkerDot),
    m_ptMarkerColor(QColor(255, 0, 0, 255)),
    m_ptVOffset(0),
    m_ptHOffset(0),
    m_ptWidth(1),
    m_lnPen(Qt::blue),
    m_polyPen(Qt::gray),
    m_polyBrush(Qt::magenta),
    m_polyPatternWidth(16),
    m_txtContourPen(Qt::black),
    m_txtContourEnabled(false),
    m_txtBrush(Qt::black),
    m_txtLetterSpacing(1),
    m_txtWordSpacing(1),
    m_txtLineSpacing(1)
{
  QPixmap* pixmap = new QPixmap(w, h);
  pixmap->fill(m_bgColor);
  m_painter.begin(pixmap);
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
    else  // it is center
    {
      llx = llx - (ww - v) / 2.;
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
    else  // it is center
    {
      lly = lly - (wh - v) / 2.;
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

  if(m_painter.device()->devType() != 2)  // is the device different of a pixmap?
    return;

  static_cast<QPixmap*>(m_painter.device())->fill(m_bgColor);
}

te::color::RGBAColor te::qt::widgets::Canvas::getBackgroundColor() const
{
  return te::color::RGBAColor(m_bgColor.red(), m_bgColor.green(), m_bgColor.blue(), m_bgColor.alpha());
}

void te::qt::widgets::Canvas::clear()
{
  if(m_painter.device()->devType() != 2)  // is the device different of a pixmap?
    return;

  static_cast<QPixmap*>(m_painter.device())->fill(m_bgColor);
}

void te::qt::widgets::Canvas::resize(int w, int h)
{
  if(m_painter.device()->devType() != 2)  // is the device different of a pixmap?
    return;

  if(!m_isDeviceOwner)
    return;

  QPixmap* pixmap = static_cast<QPixmap*>(m_painter.device());
  m_painter.end();

  delete pixmap;

  pixmap = new QPixmap(w, h);
  pixmap->fill(m_bgColor);

  m_painter.begin(pixmap);
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
  switch(geom->getGeomTypeId())
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
    m_painter.drawImage(rectf, *m_ptImg);
    m_painter.setWorldMatrixEnabled(true);
    return;
  }

  m_painter.drawPoint(m_pt);
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiPoint* mpoint)
{
  std::size_t size = mpoint->getNumGeometries();

  for(std::size_t i = 0; i < size; ++i)
    draw(static_cast<const te::gm::Point*>(mpoint->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::LineString* line)
{
  QPointF p1;
  QPointF p2;
  double radians, degrees;

  te::gm::Coord2D* coords = line->getCoordinates();
  std::size_t size = line->getNPoints();
  if(size <= 1)
    return;

  p1.setX(coords[0].x);
  p1.setY(coords[0].y);

  for(register std::size_t i = 1; i != size; ++i)
  {    
    p2.setX(coords[i].x);
    p2.setY(coords[i].y);

    QBrush brush = m_lnPen.brush();
    QImage img = brush.textureImage();
    if(img.isNull() == false)
    {
      QPointF dp1 = m_matrix.map(p1);
      QPointF dp2 = m_matrix.map(p2);
      if(dp2.x() == dp1.x())
        radians = 3.1415926536/2.;
      else
        radians = atan((dp1.y() - dp2.y())/(dp2.x() - dp1.x()));
      degrees = radians * 180. / 3.1415926536;
 
      QMatrix matrix = m_matrix.inverted();
      matrix.rotate(-degrees);

      QRectF rec(0, 0, m_lnPen.widthF(), m_lnPen.widthF()*(double)(img.width())/(double)(img.height()));
      int iw = (int)(m_matrix.mapRect(rec).width() + .5);
      //int ih = (int)(m_matrix.mapRect(rec).height() + .5);
      int v = (int)(sin(radians) * dp1.x() + cos(radians)*dp1.y());
      int offy = (v%iw)+iw/2;
      //int vx = (int)(dp1.x() / sin(radians) + dp1.y() / cos(radians));
      //int offx = vx%ih;

      //matrix.translate(-offx, offy);
      matrix.translate(0, offy);

      double scale = (double)iw / (double)img.height();
      matrix.scale(scale, scale);
      brush.setMatrix(matrix);
      m_lnPen.setBrush(brush);
    }
    m_painter.setPen(m_lnPen);  
    m_painter.drawLine(p1, p2);

    p1 = p2;
  }

/////////////////////////////////////////////////////////////////////////////////////////////////
  //QPointF p1;
  //QPointF p2;

  //m_painter.setPen(m_lnPen);

  //te::gm::Coord2D* coords = line->getCoordinates();
  //std::size_t size = line->getNPoints();

  //p1.setX(coords[0].x);
  //p1.setY(coords[0].y);

  //for(register std::size_t i = 1; i != size; ++i)
  //{    
  //  p2.setX(coords[i].x);
  //  p2.setY(coords[i].y);
  //  
  //  m_painter.drawLine(p1, p2);

  //  p1 = p2;
  //}
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

  for(register std::size_t i = 0; i != nRings; ++i)
  { 
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    const std::size_t nPoints = ring->getNPoints();

    assert(nPoints > 3);

    QPolygonF qpol(nPoints);

    memcpy(&(qpol[0]), (double*)(ring->getCoordinates()), 16 * nPoints);
    
    path.addPolygon(qpol);
  }

  m_painter.setPen(m_polyPen);
  m_painter.setBrush(m_polyBrush);
  m_painter.drawPath(path);
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiPolygon* mpoly)
{
  const std::size_t size = mpoly->getNumGeometries();

  for(std::size_t i = 0; i < size; ++i)
    draw(static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::GeometryCollection* g)
{
  size_t size = g->getNumGeometries();

  for(size_t i = 0; i < size; ++i)
    draw(g->getGeometryN(i));
}

void te::qt::widgets::Canvas::save(const char* fileName,
                                   te::map::ImageType t,
                                   int quality,
                                   int /*fg*/) const
{
  if(m_painter.device()->devType() == 2)  // is the device a pixmap?
    static_cast<QPixmap*>(m_painter.device())->save(fileName, GetFormat(t), quality);
}

char* te::qt::widgets::Canvas::getImage(te::map::ImageType t, std::size_t& size, int quality, int /*fg*/) const
{
  if(m_painter.device()->devType() != 2)  // if it is not a pixmap device... return!
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
  };

  int nbytes = bytes.size();

  char* result = new char[nbytes];

  memcpy(result, bytes.data(), nbytes);

  size = nbytes;

  return result;
}

te::color::RGBAColor** te::qt::widgets::Canvas::getImage(const int x, const int y, const int w, const int h) const
{
  if(m_painter.device()->devType() != 2)  // if it is not a pixmap device... return!
    return 0;

  te::color::RGBAColor** colors = 0;

  QPixmap* pixmap = static_cast<QPixmap*>(m_painter.device());

  int width = pixmap->width();
  int height = pixmap->width();

  if(x == 0 && y == 0 && w == 0 && y == 0)
  {
    QImage ima = pixmap->toImage();

    for(int i = 0; i < height; ++i)
    {
      colors[i] = new te::color::RGBAColor[width];

      unsigned char* u = ima.scanLine(i);

      for(int j = 0; j < width; ++j)
      {
        QRgb* v = (QRgb*)(u + (j << 2));
        QRgb val = *v;
        te::color::RGBAColor cor(qRed(val), qGreen(val), qBlue(val), qAlpha(val));
        colors[i][j] = cor;
      }
    }
  }
  else
  {
    QPixmap pix = pixmap->copy(x, y, w, h);

    QImage ima = pix.toImage();

    for(int i = 0; i < h; ++i)
    {
      colors[i] = new te::color::RGBAColor[h];

      unsigned char* u = ima.scanLine(i);

      for(int j = 0; j < w; ++j)
      {
        QRgb* v = (QRgb*)(u+(j<<2));
        QRgb val = *v;
        te::color::RGBAColor cor(qRed(val), qGreen(val), qBlue(val), qAlpha(val));
        colors[i][j] = cor;
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
  QImage img(w, h, QImage::Format_RGB32);

  for(int l = 0; l < h; ++l)
    for(int c = 0; c < w; ++c)
      img.setPixel(c, l, src[l][c].getRgba());

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

void te::qt::widgets::Canvas::drawImage(int x, int y, te::rst::Raster* src)
{
  int sw = src->getNumberOfColumns();
  int sh = src->getNumberOfRows();

  drawImage(x, y, sw, sh, src, 0, 0, sw, sh);
}

void te::qt::widgets::Canvas::drawImage(int x, int y, int w, int h, te::rst::Raster* src, int sx, int sy, int sw, int sh)
{
  // Define qimage size. 
  int iw = std::min(sw, w);
  int ih = std::min(sh, h);

  QImage img(iw, ih, QImage::Format_RGB32);
  double pr, pg, pb;
  te::color::RGBAColor* pixel;

  // Check if undersampling is needed. 
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
      pixel = new te::color::RGBAColor((int)pr, (int)pg, (int)pb, 255);
      img.setPixel(ri, li, pixel->getRgba());
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
  m_painter.setPen(QColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));

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

  te::at::HorizontalAlignment hAlign;
  te::at::VerticalAlignment vAlign;

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
      else if(style == te::at::Underline)
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
      double hline;
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

  if(m_painter.device()->devType() == 4)
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
  m_ptColor = QColor(color.getRgba());
  m_ptColor.setAlpha(qAlpha(color.getRgba()));

// clear marker and pattern
  delete m_ptImg;
  m_ptImg = 0;
}

void te::qt::widgets::Canvas::setPointWidth(int w)
{
  if(w == m_ptWidth)
    return;

  m_ptWidth = w;

  if(m_ptMarkerType != te::map::MarkerNone)
    setPointMarker(m_ptMarkerType, m_ptWidth);
}

void te::qt::widgets::Canvas::setPointMarker(te::map::PtMarkerType type, int w)
{
  if(w!= 0)
    m_ptWidth = w;

  m_ptMarkerType = type;

  te::color::RGBAColor c(m_ptMarkerColor.rgba());
  
  te::color::RGBAColor** marker = te::map::CreateMarker(type, m_ptWidth, c);

  delete m_ptImg;

  m_ptImg = GetImage(marker, m_ptWidth, m_ptWidth);

  for(int i = 0; i < m_ptWidth; ++i)
    delete []marker[i];

  delete []marker;
}

void  te::qt::widgets::Canvas::setPointMarkerColor(const te::color::RGBAColor& color)
{
  m_ptMarkerColor = QColor(color.getRgba());
  m_ptMarkerColor.setAlpha(qAlpha(color.getRgba()));

  setPointMarker(m_ptMarkerType, m_ptWidth);
}

void te::qt::widgets::Canvas::setPointPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  delete m_ptImg;

  m_ptImg = GetImage(pattern, ncols, nrows);
}

void te::qt::widgets::Canvas::setPointPattern(char* pattern, std::size_t size, te::map::ImageType t)
{  
  delete m_ptImg;

  m_ptImg = new QImage();

  const char* format = te::qt::widgets::GetFormat(t);

  m_ptImg->loadFromData((const uchar*)pattern, size, format);
}

void te::qt::widgets::Canvas::setLineColor(const te::color::RGBAColor& color)
{
  QColor qcolor(color.getRgba());
  qcolor.setAlpha(qAlpha(color.getRgba()));
  m_lnPen.setColor(qcolor);
  m_lnPen.setCapStyle(Qt::RoundCap);
}

void te::qt::widgets::Canvas::setLinePattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);

  if(img->isNull() == false)
  {
    QRectF rec(0, 0, m_lnPen.widthF(), m_lnPen.widthF());
    double w = m_matrix.mapRect(rec).width();
    QMatrix matrix = m_matrix.inverted();
    QBrush brush;
    brush.setTextureImage(*img);
    double scale = w / (double)img->height();
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_lnPen.setBrush(brush);
    m_lnPen.setCapStyle(Qt::RoundCap);
    //m_lnPen.setCapStyle(Qt::FlatCap);
  }
  delete img;
}

void te::qt::widgets::Canvas::setLinePattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  QImage img;

  const char* format = te::qt::widgets::GetFormat(t);

  bool result = img.loadFromData((const uchar*)pattern, size, format);

  if(result && img.isNull() == false)
  {
    QRectF rec(0, 0, m_lnPen.widthF(), m_lnPen.widthF());
    double w = m_matrix.mapRect(rec).width();
    QMatrix matrix = m_matrix.inverted();
    QBrush brush;
    brush.setTextureImage(img);
    double scale = w / (double)img.height();
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_lnPen.setBrush(brush);
    m_lnPen.setCapStyle(Qt::RoundCap);
    //m_lnPen.setCapStyle(Qt::FlatCap);
  }
}

void te::qt::widgets::Canvas::setLineWidth(int w)
{
  QRectF rec(QPoint(0., 0.), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_lnPen.setWidthF(rec.width());

  QBrush brush = m_lnPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull() == false)
  {
    QMatrix matrix = m_matrix.inverted();
    double scale = static_cast<double>(w) / static_cast<double>(img.width());
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_lnPen.setBrush(brush);
  }
}

void te::qt::widgets::Canvas::setPolygonFillColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));
  m_polyBrush.setColor(cor);
  //m_polyBrush.setColor(QColor(color.getRgba()));
  m_polyBrush.setStyle(Qt::SolidPattern);
}

void te::qt::widgets::Canvas::setPolygonContourColor(const te::color::RGBAColor& color)
{
  QColor cor(color.getRgba());
  cor.setAlpha(qAlpha(color.getRgba()));
  m_polyPen.setColor(cor);
}

void te::qt::widgets::Canvas::setPolygonFillPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);

  if(img->isNull() == false)
  {
    QMatrix matrix = m_matrix.inverted();
    double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img->width());
    matrix.scale(scale, scale);
    m_polyBrush.setMatrix(matrix);
    m_polyBrush.setTextureImage(*img);
  }
  delete img;
}

void te::qt::widgets::Canvas::setPolygonFillPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  QImage img;

  const char* format = te::qt::widgets::GetFormat(t);

  bool result = img.loadFromData((const uchar*)pattern, size, format);

  if(result && img.isNull() == false)
  {
    QMatrix matrix = m_matrix.inverted();
    double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img.width());
    matrix.scale(scale, scale);
    m_polyBrush.setMatrix(matrix);
    m_polyBrush.setTextureImage(img);
  }
}

void te::qt::widgets::Canvas::setPolygonPatternWidth(int w)
{
  m_polyPatternWidth = w;

  if(m_polyPatternWidth == 0.)
    m_polyPatternWidth = 1.;

  QImage img = m_polyBrush.textureImage();

  if(img.isNull() == false)
  {
    QMatrix matrix = m_matrix.inverted();
    double scale = static_cast<double>(m_polyPatternWidth) / static_cast<double>(img.width());
    matrix.scale(scale, scale);
    m_polyBrush.setMatrix(matrix);
  }
}

void te::qt::widgets::Canvas::setPolygonPatternOpacity(int opacity)
{
// TESTAR SE ESTA SENDIO APLICADA A IMAGEM????
  //QColor cor = m_polyBrush.color();
  //cor.setAlpha(opacity);
  //m_polyBrush.setColor(cor);
// NAO FUNCIONA... o painter nao usa isto para plotar polygons com pattern

// O que pode fazer é mudar a opacidade do pattern, mas,
  // o melhor mesmo é setar outro pattern com transparencia desejada.
  // por enquanto vou mudar a opacidade do pattern existente no brush.

  // melhor e' fazer pattern com fundo 100% trasnparente.

  QImage img = m_polyBrush.textureImage();

  if(img.isNull())
    return;

  int op = opacity << 24;
  for(int i = 0; i < img.height(); ++i)
  {
    unsigned char* u = img.scanLine(i);

    for(int j = 0; j < img.width(); ++j)
    {
      QRgb* v = (QRgb*)(u + (j << 2));
      *v = *v & 0x00ffffff;
      *v = op | *v;
    }
  }
  m_polyBrush.setTextureImage(img);
}

void te::qt::widgets::Canvas::setPolygonContourPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);

  if(img->isNull() == false)
  {
    QRectF rec(0, 0, m_polyPen.widthF(), m_polyPen.widthF());
    double w = m_matrix.mapRect(rec).width();
    QMatrix matrix = m_matrix.inverted();
    QBrush brush;
    brush.setTextureImage(*img);
    double scale = w / (double)img->height();
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_polyPen.setBrush(brush);
  }
}

void te::qt::widgets::Canvas::setPolygonContourPattern(char* pattern, std::size_t size, te::map::ImageType t)
{  
  QImage img;

  const char* format = te::qt::widgets::GetFormat(t);

  bool result = img.loadFromData((const uchar*)pattern, size, format);

  if(result && img.isNull() == false)
  {
    QRectF rec(0, 0, m_polyPen.widthF(), m_polyPen.widthF());
    double w = m_matrix.mapRect(rec).width();
    QMatrix matrix = m_matrix.inverted();
    QBrush brush;
    brush.setTextureImage(img);
    double scale = w / (double)img.height();
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_polyPen.setBrush(brush);
  }
}

void te::qt::widgets::Canvas::setPolygonContourWidth(int w)
{
  QRectF rec(QPoint(0., 0.), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_polyPen.setWidthF(rec.width());

  QBrush brush = m_polyPen.brush();
  QImage img = m_polyPen.brush().textureImage();

  if(img.isNull() == false)
  {
    QMatrix matrix = m_matrix.inverted();
    double scale = static_cast<double>(w) / static_cast<double>(img.width());
    matrix.scale(scale, scale);
    brush.setMatrix(matrix);
    m_polyPen.setBrush(brush);
  }
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

  m_painter.setBrush(Qt::NoBrush);
  m_painter.fillPath(path, m_txtBrush);

  if(m_txtContourEnabled)
  {
    m_painter.setPen(m_txtContourPen);
    m_painter.drawPath(path);
  }
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
  if(m_painter.device()->devType() == 2)
    return static_cast<QPixmap*>(m_painter.device());
  else
    return 0;
}

void te::qt::widgets::Canvas::setDevice(QPaintDevice* device, bool takeOwnerShip)
{
  if(m_isDeviceOwner)
    delete m_painter.device();

  m_painter.end();

  m_isDeviceOwner = takeOwnerShip;
  m_painter.begin(device);
}

int te::qt::widgets::Canvas::getResolution()
{
  if(m_painter.device()->devType() == 4)
    return static_cast<QPrinter*>(m_painter.device())->resolution();
  else
    return m_painter.device()->logicalDpiX();
}

QMatrix te::qt::widgets::Canvas::getMatrix()
{
  return m_matrix;
}


