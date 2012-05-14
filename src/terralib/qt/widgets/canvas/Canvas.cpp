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
    m_bgColor(Qt::transparent),
    m_pointColor(Qt::black),
    m_pointPattern(0),
    m_pointVOffset(0),
    m_pointHOffset(0),
    m_linePen(Qt::black),
    m_polygonPen(Qt::black),
    m_polygonBrush(Qt::darkGray),
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
  delete m_pointPattern;

  if(!m_isDeviceOwner)
    return;

  QPaintDevice* device = m_painter.device();
  m_painter.end();

  delete device;
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
  m_painter.setWorldMatrixEnabled(false);
  m_painter.fillRect(m_painter.viewport(), m_bgColor);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::resize(int w, int h)
{
  if(m_painter.device()->devType() != QInternal::Pixmap)
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
  }
}

void te::qt::widgets::Canvas::draw(const te::gm::Point* point)
{
  m_point.setX(point->getX());
  m_point.setY(point->getY());

  if(m_pointPattern == 0)
  {
     m_painter.drawPoint(m_point);
     return;
  }

  QPointF center = m_matrix.map(m_point);
  center.setX(center.x() + m_pointHOffset);
  center.setY(center.y() + m_pointVOffset);

  QRectF rect(0.0, 0.0, m_pointPattern->width(), m_pointPattern->height());
  rect.moveCenter(center);

  m_painter.setWorldMatrixEnabled(false);
  m_painter.drawImage(rect, *m_pointPattern);
  m_painter.setWorldMatrixEnabled(true);
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiPoint* mpoint)
{
  const std::size_t size = mpoint->getNumGeometries();
  for(std::size_t i = 0; i < size; ++i)
    draw(static_cast<const te::gm::Point*>(mpoint->getGeometryN(i)));
}

void te::qt::widgets::Canvas::draw(const te::gm::LineString* line)
{
  QPainterPath path;

  te::gm::Coord2D* coords = line->getCoordinates();
  path.moveTo(coords[0].x, coords[0].y);
  
  const std::size_t size = line->getNPoints();
  for(register std::size_t i = 1; i != size; ++i)
    path.lineTo(coords[i].x, coords[i].y);

  m_painter.setPen(m_linePen);
  m_painter.drawPath(path);
}

void te::qt::widgets::Canvas::draw(const te::gm::MultiLineString* mline)
{
  const std::size_t size = mline->getNumGeometries();
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

  m_painter.setPen(m_polygonPen);
  m_painter.setBrush(m_polygonBrush);
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
  const std::size_t size = g->getNumGeometries();
  for(size_t i = 0; i < size; ++i)
    draw(g->getGeometryN(i));
}

void te::qt::widgets::Canvas::save(const char* fileName, te::map::ImageType t, int quality, int /*fg*/) const
{
  if(m_painter.device()->devType() != QInternal::Pixmap)
    return;

  static_cast<QPixmap*>(m_painter.device())->save(fileName, GetFormat(t), quality);
}

char* te::qt::widgets::Canvas::getImage(te::map::ImageType t, std::size_t& size, int quality, int /*fg*/) const
{
  if(m_painter.device()->devType() != QInternal::Pixmap)
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
  if(m_painter.device()->devType() != QInternal::Pixmap)
    return 0;

  te::color::RGBAColor** colors = 0;

  QPixmap* pixmap = static_cast<QPixmap*>(m_painter.device());

  if(x == 0 && y == 0 && w == 0 && y == 0)
  {
    QImage img = pixmap->toImage();

    int width = pixmap->width();
    int height = pixmap->height();

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
    QPixmap pix = pixmap->copy(x, y, w, h);

    QImage img = pix.toImage();

    for(int i = 0; i < h; ++i)
    {
      colors[i] = new te::color::RGBAColor[h];

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
  // Defines QImage size
  int iw = std::min(sw, w);
  int ih = std::min(sh, h);

  QImage img(iw, ih, QImage::Format_RGB32);
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
  m_pointColor = QColor(color.getRgba());
  m_pointColor.setAlpha(qAlpha(color.getRgba()));
// Clear point pattern
  delete m_pointPattern;
  m_pointPattern = 0;
}

void te::qt::widgets::Canvas::setPointPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  delete m_pointPattern;
  m_pointPattern = GetImage(pattern, ncols, nrows);
  if(m_pointPattern->isNull())
  {
    delete m_pointPattern;
    m_pointPattern = 0;
  }
}

void te::qt::widgets::Canvas::setPointPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  delete m_pointPattern;
  m_pointPattern = new QImage;

  const char* format = te::qt::widgets::GetFormat(t);
  if(!m_pointPattern->loadFromData((const uchar*)pattern, size, format) || m_pointPattern->isNull())
  {
    delete m_pointPattern;
    m_pointPattern = 0;
  }
}

void te::qt::widgets::Canvas::setPointPatternWidth(int w)
{
  if(m_pointPattern == 0)
    return;

  QImage scaled = m_pointPattern->scaledToWidth(w);

  delete m_pointPattern;
  m_pointPattern = new QImage(scaled);
}

void te::qt::widgets::Canvas::setPointPatternRotation(const double& angle)
{
  if(m_pointPattern == 0)
    return;

  QMatrix m;
  m.rotate(angle);

  QImage rotated = m_pointPattern->transformed(m);

  delete m_pointPattern;
  m_pointPattern = new QImage(rotated);
}

void te::qt::widgets::Canvas::setPointPatternOpacity(int opacity)
{
  if(m_pointPattern == 0)
    return;

  updateAlpha(*m_pointPattern, opacity);
}

void te::qt::widgets::Canvas::setLineColor(const te::color::RGBAColor& color)
{
  QColor qcolor(color.getRgba());
  qcolor.setAlpha(qAlpha(color.getRgba()));
  m_linePen.setColor(qcolor);
}

void te::qt::widgets::Canvas::setLinePattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);
  if(img->isNull())
    return;

  // Initializes the transformation matrix
  QBrush brush = m_linePen.brush();
  QMatrix m = brush.matrix() * m_matrix.inverted();
  brush.setMatrix(m);

  brush.setTextureImage(*img);

  m_linePen.setBrush(brush);

  delete img;
}

void te::qt::widgets::Canvas::setLinePattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  const char* format = te::qt::widgets::GetFormat(t);

  QImage img;
  if(!img.loadFromData((const uchar*)pattern, size, format))
    return;

  if(img.isNull())
    return;

  // Initializes the transformation matrix
  QBrush brush = m_linePen.brush();
  QMatrix m = brush.matrix() * m_matrix.inverted();
  brush.setMatrix(m);

  brush.setTextureImage(img);

  m_linePen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLinePatternWidth(int w)
{
  QBrush brush = m_linePen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  double scale = static_cast<double>(w) / static_cast<double>(img.width());

  // Updates matrix
  QMatrix matrix = brush.matrix();
  matrix.scale(scale, scale);
  brush.setMatrix(matrix);

  m_linePen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLinePatternRotation(const double& angle)
{
  QBrush brush = m_linePen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates matrix
  QMatrix matrix = brush.matrix();
  matrix.rotate(angle);
  brush.setMatrix(matrix);

  m_linePen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLinePatternOpacity(int opacity)
{
  QBrush brush = m_linePen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  updateAlpha(img, opacity);

  brush.setTextureImage(img);
  m_linePen.setBrush(brush);
}

void te::qt::widgets::Canvas::setLineDashStyle(te::map::LineDashStyle style)
{
  m_linePen.setStyle((Qt::PenStyle)style);
}

void te::qt::widgets::Canvas::setLineDashStyle(const std::vector<double>& style)
{
  setLineDashStyle(m_linePen, style);
}

void te::qt::widgets::Canvas::setLineCapStyle(te::map::LineCapStyle style)
{
  m_linePen.setCapStyle((Qt::PenCapStyle)style);
}

void te::qt::widgets::Canvas::setLineJoinStyle(te::map::LineJoinStyle style)
{
  m_linePen.setJoinStyle((Qt::PenJoinStyle)style);
}

void te::qt::widgets::Canvas::setLineWidth(int w)
{
  QRectF rec(QPoint(0.0, 0.0), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_linePen.setWidthF(rec.width());
}

void te::qt::widgets::Canvas::setPolygonFillColor(const te::color::RGBAColor& color)
{
  QColor qcolor(color.getRgba());
  qcolor.setAlpha(qAlpha(color.getRgba()));
  m_polygonBrush.setColor(qcolor);
  m_polygonBrush.setStyle(Qt::SolidPattern);
}

void te::qt::widgets::Canvas::setPolygonContourColor(const te::color::RGBAColor& color)
{
  QColor qcolor(color.getRgba());
  qcolor.setAlpha(qAlpha(color.getRgba()));
  m_polygonPen.setColor(qcolor);
}

void te::qt::widgets::Canvas::setPolygonFillPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);
  if(img->isNull())
    return;

  // Initializes the transformation matrix
  QMatrix m = m_polygonBrush.matrix() * m_matrix.inverted();
  m_polygonBrush.setMatrix(m);

  m_polygonBrush.setTextureImage(*img);

  delete img;
}

void te::qt::widgets::Canvas::setPolygonFillPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  const char* format = te::qt::widgets::GetFormat(t);

  QImage img;
  if(!img.loadFromData((const uchar*)pattern, size, format))
    return;

  if(img.isNull())
    return;

  // Initializes the transformation matrix
  QMatrix m = m_polygonBrush.matrix() * m_matrix.inverted();
  m_polygonBrush.setMatrix(m);

  m_polygonBrush.setTextureImage(img);
}

void te::qt::widgets::Canvas::setPolygonPatternWidth(int w)
{
  QImage img = m_polygonBrush.textureImage();
  if(img.isNull())
    return;

  double scale = static_cast<double>(w) / static_cast<double>(img.width());

  // Updates the transformation matrix
  QMatrix matrix = m_polygonBrush.matrix();
  matrix.scale(scale, scale);

  m_polygonBrush.setMatrix(matrix);
}

void te::qt::widgets::Canvas::setPolygonPatternRotation(const double& angle)
{
  QImage img = m_polygonBrush.textureImage();
  if(img.isNull())
    return;

  // Updates the transformation matrix
  QMatrix matrix = m_polygonBrush.matrix();
  matrix.rotate(angle);

  m_polygonBrush.setMatrix(matrix);
}

void te::qt::widgets::Canvas::setPolygonPatternOpacity(int opacity)
{
  QImage img = m_polygonBrush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  updateAlpha(img, opacity);

  m_polygonBrush.setTextureImage(img);
}

void te::qt::widgets::Canvas::setPolygonContourPattern(te::color::RGBAColor** pattern, int ncols, int nrows)
{
  QImage* img = GetImage(pattern, ncols, nrows);
  if(img->isNull())
    return;

  // Initializes the transformation matrix
  QBrush brush = m_polygonPen.brush();
  QMatrix m = brush.matrix() * m_matrix.inverted();
  brush.setMatrix(m);
  brush.setTextureImage(*img);

  m_polygonPen.setBrush(brush);

  delete img;
}

void te::qt::widgets::Canvas::setPolygonContourPattern(char* pattern, std::size_t size, te::map::ImageType t)
{
  const char* format = te::qt::widgets::GetFormat(t);

  QImage img;
  if(!img.loadFromData((const uchar*)pattern, size, format))
    return;

  if(img.isNull())
    return;

  // Initializes the transformation matrix
  QBrush brush = m_polygonPen.brush();
  QMatrix m = brush.matrix() * m_matrix.inverted();
  brush.setMatrix(m);

  brush.setTextureImage(img);

  m_polygonPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourPatternWidth(int w)
{
  QBrush brush = m_polygonPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  double scale = static_cast<double>(w) / static_cast<double>(img.width());

  // Updates matrix
  QMatrix matrix = brush.matrix();
  matrix.scale(scale, scale);
  brush.setMatrix(matrix);

  m_polygonPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourPatternRotation(const double& angle)
{
  QBrush brush = m_polygonPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates matrix
  QMatrix matrix = brush.matrix();
  matrix.rotate(angle);
  brush.setMatrix(matrix);

  m_polygonPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourPatternOpacity(int opacity)
{
  QBrush brush = m_polygonPen.brush();
  QImage img = brush.textureImage();
  if(img.isNull())
    return;

  // Updates the alpha channel
  updateAlpha(img, opacity);

  brush.setTextureImage(img);
  m_polygonPen.setBrush(brush);
}

void te::qt::widgets::Canvas::setPolygonContourDashStyle(te::map::LineDashStyle style)
{
  m_polygonPen.setStyle((Qt::PenStyle)style);
}

void te::qt::widgets::Canvas::setPolygonContourDashStyle(const std::vector<double>& style)
{
  setLineDashStyle(m_polygonPen, style);
}

void te::qt::widgets::Canvas::setPolygonContourCapStyle(te::map::LineCapStyle style)
{
  m_polygonPen.setCapStyle((Qt::PenCapStyle)style);
}

void te::qt::widgets::Canvas::setPolygonContourJoinStyle(te::map::LineJoinStyle style)
{
  m_polygonPen.setJoinStyle((Qt::PenJoinStyle)style);
}

void te::qt::widgets::Canvas::setPolygonContourWidth(int w)
{
  QRectF rec(QPoint(0.0, 0.0), QPoint(w, w));
  rec = m_matrix.inverted().mapRect(rec);
  m_polygonPen.setWidthF(rec.width());
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
  if(m_painter.device()->devType() == QInternal::Pixmap)
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
  if(m_painter.device()->devType() == QInternal::Printer)
    return static_cast<QPrinter*>(m_painter.device())->resolution();
  else
    return m_painter.device()->logicalDpiX();
}

QMatrix te::qt::widgets::Canvas::getMatrix()
{
  return m_matrix;
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
