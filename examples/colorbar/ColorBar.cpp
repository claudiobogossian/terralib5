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
  \file ColorBar.cpp

  \brief A widget to control the display of color bar.
 */

// The following code uses the ColorTransform class to change the color components of the bar.
// TerraLib        
#include "ColorBar.h"
#include "../../src/terralib/color/ColorTransform.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPixmap>
#include <QBrush>
//#include <QWSKeyboardHandler>
#include <QRect>

ColorBar::ColorBar(QWidget* parent)
  : QDialog(parent),
    m_colorBar(0),
    m_cursorShape(Qt::ArrowCursor),
    m_slices(5),
    m_hoffset(20),
    m_voffset(120),
    m_key(Qt::Key_H),
    m_capsLock(false),
    m_pos(-1.),
    m_editPos(-1.),
    m_copypos(-1.)
{
  m_pol = new QPolygon(4);
  m_pol->setPoint(0, 0, 0);
  m_pol->setPoint(1, 3, -6);
  m_pol->setPoint(2, -3, -6);
  m_pol->setPoint(3, 0, 0);

  setMinimumWidth(200);
  setMinimumHeight(200);

  setWindowTitle("Color bar example");
  setMouseTracking(true);
}

ColorBar::~ColorBar()
{
  delete m_pol;
}

void ColorBar::mouseMoveEvent(QMouseEvent* e)
{
  int xi = m_hoffset;
  int xf = m_width - m_hoffset;
  int yi = m_voffset;
  int yf = m_height - m_voffset/12;
  int y = m_voffset / 2 + yf / 2 - 2;
  int yy = m_voffset / 2 + yf / 2;

  QRect rec(xi, yi, xf-xi+1, y-yi+1);
  QRect rec2(xi, yy, xf - xi + 1, yf - yy + 1);
  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
  std::map<double, te::color::RGBAColor>::const_iterator mit;
  m_colorMsg.clear();
  m_colorMsg2.clear();
  m_colorMsg3.clear();
  m_colorMsg4.clear();

  if(rec.contains(e->pos()))
  {
    if(m_pos == -1)
    {
      int size = (int)colorMap.size();
      int i;
      for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
      {
        if(i == 0 || i == size-1)
          continue;

        int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
        int dx = abs(e->pos().x() - x);
        if(dx < 2)
        {
          if(cursor().shape() != Qt::SplitHCursor)
          {
            QCursor cursor(Qt::SplitHCursor);
            setCursor(cursor);
            m_color = mit->second;
            m_editPos = mit->first;
            m_copypos = mit->first;
          }
          setColorMsg(mit->second, mit->first);
          break;
        }
      }
      if(mit == colorMap.end())
      {
        m_colorMsg = "Press mouse left button to insert a color";
        if(cursor().shape() != Qt::PointingHandCursor)
        {
          QCursor cursor(Qt::PointingHandCursor);
          setCursor(cursor);
        }
      }
    }
  }
  else if(rec2.contains(e->pos()))
  {
    QString nSlices;
    nSlices.setNum(m_slices);
    m_colorMsg = "Use mouse whell to change number of color slices. slices=" + nSlices;
    if(cursor().shape() != Qt::SizeHorCursor)
    {
      QCursor cursor(Qt::SizeHorCursor);
      setCursor(cursor);
    }
  }
  else
  {
    for(mit = colorMap.begin(); mit != colorMap.end(); ++mit)
    {
      int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
      QPolygon pol = m_pol->translated(x, yi);
      if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
      {
        if(cursor().shape() != Qt::SizeVerCursor)
        {
          QCursor cursor(Qt::SizeVerCursor);
          setCursor(cursor);
          m_color = mit->second;
          m_editPos = mit->first;
        }
        setColorMsg(mit->second, mit->first);
        break;
      }
    }

    if(mit == colorMap.end() && cursor().shape() != Qt::ArrowCursor)
    {
      if(cursor().shape() != Qt::ArrowCursor)
      {
        QCursor cursor(Qt::ArrowCursor);
        setCursor(cursor);
      }
    }
  }

  int button = e->buttons();
  if(button == Qt::LeftButton)
  {
    if(cursor().shape() == Qt::SplitHCursor)
    {
      if(m_pos != -1.)
      {
        std::map<double, te::color::RGBAColor>::const_iterator cit = m_colorBar->getColorMap().find(m_pos);
        if(cit != m_colorBar->getColorMap().end())
          setColorMsg(cit->second, cit->first);
        int px = e->pos().x() - xi;
        double newpos = (double)(px) / (double)(m_colorBar->getBarSize()-1);
        m_pos = m_colorBar->move(m_pos, newpos);
        m_editPos = m_pos;
      }
    }
  }
  repaint();
}

void ColorBar::mousePressEvent(QMouseEvent* e)
{
  int xi = m_hoffset;
  int xf = m_width - m_hoffset;
  int yi = m_voffset;
  int yf = m_height - m_voffset/12;
  int y = m_voffset / 2 + yf / 2 - 2;

  QRect rec(xi, yi, xf-xi+1, y-yi+1);
  if(rec.contains(e->pos()))
  {
    // add color
    int shape = cursor().shape();
    int button = e->button();
    if(button == Qt::LeftButton && shape == Qt::PointingHandCursor)
    {
      te::color::RGBAColor c(0, 255, 128, 255);
      int x = e->pos().x();
      int p = x - xi;
      int w = xf - xi + 1;
      double dp = (double)p / (double)w;
      if(dp > 1.)
        dp = 1.;
      m_colorBar->addColor(c, dp);
      m_pos = dp;
      if(cursor().shape() != Qt::SplitHCursor)
      {
        m_editPos = m_pos;
        QCursor cursor(Qt::SplitHCursor);
        setCursor(cursor);
      }
      setColorMsg(c, m_pos);
      m_color = c;
    }

    // move color
    else if(e->button() == Qt::LeftButton && cursor().shape() == Qt::SplitHCursor)
    {
      const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();

      if((e->pos().x() - xi) > 2)
      {
        std::map<double, te::color::RGBAColor>::const_iterator mit;
        int size = (int)colorMap.size();
        int i;
        for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
        {
          if(i == 0 || i == size-1)
            continue;
          int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
          int dx = abs(e->pos().x() - x);
          te::color::RGBAColor c = mit->second;
          setColorMsg(c, mit->first);
          if(dx < 2)
          {
            m_pos = mit->first;
            m_editPos = m_pos;
            m_color = c;
            break;
          }
        }
        if(mit == colorMap.end())
        {
          m_pos = -1.;
          m_editPos = m_pos;
        }

      }
      else
      {
        std::map<double, te::color::RGBAColor>::const_reverse_iterator mit;
        int size = (int)colorMap.size();
        int i;
        for(mit = colorMap.rbegin(), i=size-1; mit != colorMap.rend(); ++mit, --i)
        {
          if(i == 0 || i == size-1)
            continue;
          int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
          int dx = abs(e->pos().x() - x);
          te::color::RGBAColor c = mit->second;
          setColorMsg(c, mit->first);
          if(dx < 2)
          {
            m_pos = mit->first;
            m_editPos = m_pos;
            m_color = c;
            break;
          }
        }
        if(mit == colorMap.rend())
        {
          m_pos = -1.;
          m_editPos = m_pos;
        }
      }
    }
  }

  //remove color
  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
  std::map<double, te::color::RGBAColor>::const_iterator mit;
  int size = (int)colorMap.size();
  int i;
  for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
  {
    if(i == 0 || i == size-1)
      continue;
    int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
    te::color::RGBAColor c = mit->second;
    QPolygon pol = m_pol->translated(x, yi);
    if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
    {
      if(e->button() == Qt::LeftButton)
      {
        m_colorBar->remove(mit->first);   
        m_pos = -1.;
        m_editPos = m_pos;
        m_colorMsg.clear();
        m_colorMsg2.clear();
        m_colorMsg3.clear();
        m_colorMsg4.clear();
        break;
      }
    }
  }
  repaint();
}

void ColorBar::mouseReleaseEvent(QMouseEvent*)
{
  m_copypos = m_pos;
  m_pos = -1.;
}

void ColorBar::keyPressEvent(QKeyEvent* e)
{
  if(e->key() == Qt::Key_M ||
    e->key() == Qt::Key_H ||
    e->key() == Qt::Key_S ||
    e->key() == Qt::Key_L ||
    e->key() == Qt::Key_R ||
    e->key() == Qt::Key_G ||
    e->key() == Qt::Key_B)
  {
    int key = m_key;
    m_key = e->key();

    if(key != m_key && m_colorMsg.isEmpty() == false)
    {
      if(!m_colorMsg.contains("Wheel: "))
        return;

      QChar abre('(');
      QChar fecha(')');
      int i, ini, fim;
      i = 0;
      while(m_colorMsg[i] != abre)
        ++i;
      ini = i + 1;
      while(m_colorMsg[i] != fecha)
        ++i;
      fim = i - ini;

      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
        m_colorMsg.replace(ini, fim, "Hue");
      else if(m_key == Qt::Key_R)
        m_colorMsg.replace(ini, fim, "Red");
      else if(m_key == Qt::Key_G)
        m_colorMsg.replace(ini, fim, "Green");
      else if(m_key == Qt::Key_B)
        m_colorMsg.replace(ini, fim, "Blue");
      else if(m_key == Qt::Key_S)
        m_colorMsg.replace(ini, fim, "Saturation");
      else if(m_key == Qt::Key_L)
        m_colorMsg.replace(ini, fim, "Lightness");
    
      repaint();

      if(cursor().shape() == Qt::SplitHCursor || cursor().shape() == Qt::SizeVerCursor)
      {
        const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
        std::map<double, te::color::RGBAColor>::const_iterator mit;
        mit = colorMap.find(m_editPos);
        if(mit != colorMap.end())
          m_color = mit->second;
      }
    }
  }
}

void ColorBar::keyReleaseEvent(QKeyEvent* e)
{
  if(e->key() == Qt::Key_CapsLock)
  {
    if(e->nativeModifiers() == 768)
      m_capsLock = true;
    else
      m_capsLock = false;
  }
}

void ColorBar::wheelEvent(QWheelEvent* e)
{
  int d = e->delta();

  int xi = m_hoffset;
  int xf = m_width - m_hoffset;
  int yi = m_voffset;
  int yf = m_height - m_voffset/12;

  int y = m_voffset / 2 + yf / 2;
  QRect rec(xi, y, xf-xi+1, yf-y+1);
  if(rec.contains(e->pos()))
  {
    if(d > 0)
      ++m_slices;
    else
      --m_slices;

    if(m_slices > 50)
      m_slices = 50;
    if(m_slices < 1)
      m_slices = 1;

    QString nSlices;
    nSlices.setNum(m_slices);
    m_colorMsg = "Use mouse whell to change the number of color slices. Color slices = " + nSlices;
    repaint();
    return;
  }

  int ind;
  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
  std::map<double, te::color::RGBAColor>::const_iterator mit;
  for(mit = colorMap.begin(), ind = 0; mit != colorMap.end(); ++mit, ind++)
  {
    int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
    te::color::RGBAColor c = mit->second;
    te::color::ColorTransform qc(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
    QPolygon pol = m_pol->translated(x, yi);
    if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
    {
      int hh, ss, ll, a;
      int h, s, l, r, g, b, d = 1;
      te::color::ColorTransform c(m_color.getRgba());
      c.getHsl(&hh, &ss, &ll, &a);

      te::color::ColorTransform qc(mit->second.getRgba());
      if(m_capsLock == false)
        d = 8;
      if(e->delta() < 0)
        d *= -1;

      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
      {
        qc.getHsl(&h, &s, &l, &a);
        h += d;

        if(h > 359)
          h -= 360;
        if(h < 0)
          h += 360;

        qc.setHsl(h, s, l, a);
        m_color = qc.getRgba();
      }
      else if(m_key == Qt::Key_S)
      {
        qc.getHsl(&h, &s, &l, &a);
        s += d;

        if(s > 255)
          s = 255;
        else if(s < 0)
          s = 0;

        qc.setHsl(hh, s, ll, a);
      }
      else if(m_key == Qt::Key_L)
      {
        qc.getHsl(&h, &s, &l, &a);
        l += d;

        if(l > 255)
          l = 255;
        if(l < 0)
          l = 0;

        qc.setHsl(hh, ss, l, a);
      }
      else if(m_key == Qt::Key_R)
      {
        r = qc.getRed();
        r += d;

        if(r > 255)
          r = 255;
        if(r < 0)
          r = 0;

        qc.setRed(r);
      }
      else if(m_key == Qt::Key_G)
      {
        g = qc.getGreen();
        g += d;

        if(g > 255)
          g = 255;
        if(g < 0)
          g = 0;

        qc.setGreen(g);
      }
      else if(m_key == Qt::Key_B)
      {
        b = qc.getBlue();
        b += d;

        if(b > 255)
          b = 255;
        if(b < 0)
          b = 0;

        qc.setBlue(b);
      }

      te::color::RGBAColor cor(qc.getRed(), qc.getGreen(), qc.getBlue(), qc.getAlpha());
      m_colorBar->changeColor(ind, cor);
      repaint();
      setColorMsg(cor, mit->first);
      return;
    }
  }

  if(cursor().shape() == Qt::SplitHCursor)
  {
    mit = colorMap.find(m_copypos);
    if(mit != colorMap.end())
    {
      int hh, ss, ll, a;
      int h, s, l, r, g, b, d = 1;
      te::color::ColorTransform c(m_color.getRgba());
      c.getHsl(&hh, &ss, &ll, &a);

      te::color::ColorTransform qc(mit->second.getRgba());

      if(m_capsLock == false)
        d = 8;
      if(e->delta() < 0)
        d *= -1;

      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
      {
        qc.getHsl(&h, &s, &l, &a);
        h += d;

        if(h > 359)
          h -= 360;
        if(h < 0)
          h += 360;

        qc.setHsl(h, s, l, a);
        m_color = qc.getRgba();
      }
      else if(m_key == Qt::Key_S)
      {
        qc.getHsl(&h, &s, &l, &a);
        s += d;

        if(s > 255)
          s = 255;
        else if(s < 0)
          s = 0;

        qc.setHsl(hh, s, ll, a);
      }
      else if(m_key == Qt::Key_L)
      {
        qc.getHsl(&h, &s, &l, &a);
        l += d;

        if(l > 255)
          l = 255;
        if(l < 0)
          l = 0;

        qc.setHsl(hh, ss, l, a);
      }
      else if(m_key == Qt::Key_R)
      {
        r = qc.getRed();
        r += d;

        if(r > 255)
          r = 255;
        if(r < 0)
          r = 0;

        qc.setRed(r);
        m_color = qc.getRgba();
      }
      else if(m_key == Qt::Key_G)
      {
        g = qc.getGreen();
        g += d;

        if(g > 255)
          g = 255;
        if(g < 0)
          g = 0;

        qc.setGreen(g);
        m_color = qc.getRgba();
      }
      else if(m_key == Qt::Key_B)
      {
        b = qc.getBlue();
        b += d;

        if(b > 255)
          b = 255;
        if(b < 0)
          b = 0;

        qc.setBlue(b);
        m_color = qc.getRgba();
      }

      te::color::RGBAColor cor = qc.getRgba();
      m_colorBar->changeColor(m_copypos, cor);
      setColorMsg(cor, mit->first);
      repaint();
    }
  }
}

void ColorBar::resizeEvent(QResizeEvent* e)
{
  m_width = e->size().width();
  m_height = e->size().height();

  if(m_colorBar == 0)
    m_colorBar = new te::color::ColorBar(m_width - m_hoffset - m_hoffset + 1);
  else
    m_colorBar->setBarSize(m_width - m_hoffset - m_hoffset + 1);
}

void ColorBar::paintEvent(QPaintEvent*)
{
  double size;
  int x, y, isize;
  te::color::RGBAColor c;
  std::vector<te::color::RGBAColor>::const_iterator it;
  QPainter painter;
  QPixmap pix(m_width, m_height);
  pix.fill();
  painter.begin(&pix);

  int xi = m_hoffset;
  int xf = m_width - m_hoffset;
  int yi = m_voffset;
  int yf = m_height - m_voffset/12;

  const std::vector<te::color::RGBAColor>& colors = m_colorBar->getColorBar();
  y = m_voffset / 2 + yf / 2 - 2;
  x = xi;
  for(it = colors.begin(); it != colors.end(); ++it)
  {
    c = *it;
    QColor qc(c.getRed(), c.getGreen(), c.getBlue());
    painter.setPen(qc);
    painter.drawLine(QPoint(x, yi), QPoint(x, y));
    ++x;
  }

  x = xi;
  int ii = 1;
  y = m_voffset / 2 + yf / 2;
  const std::vector<te::color::RGBAColor>& scolors = m_colorBar->getSlices(m_slices);
  for(it = scolors.begin(); it != scolors.end(); ++ii, ++it)
  {
    c = *it;
    QColor qc(c.getRed(), c.getGreen(), c.getBlue());
    painter.setPen(qc);
    size = (double)(xf-xi+1)/(double)m_slices;
    isize = (int)((double)ii * size + .5) + m_hoffset;
    while(x <= isize-2 && (isize-2) < xf)
    {
      painter.drawLine(QPoint(x, y), QPoint(x, yf));
      ++x;
    }
    ++x;
  }

  painter.setPen(Qt::black);
  painter.drawLine(QPoint(xi, yi), QPoint(xi, yf));
  painter.drawLine(QPoint(xi, yf), QPoint(xf, yf));
  painter.drawLine(QPoint(xf, yf), QPoint(xf, yi));
  painter.drawLine(QPoint(xf, yi), QPoint(xi, yi));

  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
  std::map<double, te::color::RGBAColor>::const_iterator mit;
  for(mit = colorMap.begin(); mit != colorMap.end(); ++mit)
  {
    x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
    c = mit->second;
    QColor qc(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
    painter.setBrush(QBrush(qc));
    painter.save();
    painter.translate(x, yi);
    painter.drawPolygon(*m_pol);
    painter.restore();
  }

  QFont font("Times", 12);
  painter.setFont(font);
  painter.setPen(Qt::black);
  QBrush brush(Qt::black);
  painter.setBrush(brush);
  if(m_colorMsg.isEmpty() == false)
    painter.drawText(10, 20, m_colorMsg);
  if(m_colorMsg2.isEmpty() == false)
    painter.drawText(10, 40, m_colorMsg2);
  painter.drawLine(0, 45, m_width, 45);
  if(m_colorMsg3.isEmpty() == false)
    painter.drawText(10, 70, m_colorMsg3);
  if(m_colorMsg4.isEmpty() == false)
    painter.drawText(10, 90, m_colorMsg4);
  painter.drawLine(0, 95, m_width, 95);
  painter.end();
  painter.begin(this);
  painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
  painter.end();
}

void ColorBar::setColorMsg(const te::color::RGBAColor& color, const double& pos)
{
  te::color::ColorTransform qc;
  int h, s, v, l, c, m, y, k;
  QString sr, sg, sb, sh, ss, sv, sl, sc, sm, sy, sk;

  int r = color.getRed();
  int g = color.getGreen();
  int b = color.getBlue();

  qc.setRgba(r, g, b, color.getAlpha());           
  qc.getHsl(&h, &s, &l);

  m_colorMsg = "Wheel: ";
  if(m_key == Qt::Key_H || m_key == Qt::Key_M)
    m_colorMsg += "(Hue)  ";
  else if(m_key == Qt::Key_R)
    m_colorMsg += "(Red)  ";
  else if(m_key == Qt::Key_G)
    m_colorMsg += "(Green)  ";
  else if(m_key == Qt::Key_B)
    m_colorMsg += "(Blue)  ";
  else if(m_key == Qt::Key_S)
    m_colorMsg += "(Saturation)  ";
  else if(m_key == Qt::Key_L)
    m_colorMsg += "(Lightness)  ";

  sr.setNum(r);
  sg.setNum(g);
  sb.setNum(b);
  sh.setNum(h);
  ss.setNum(s);
  sl.setNum(l);

  m_colorMsg += "Red=" + sr + " Green=" + sg + " Blue=" + sb + " Hue(hsl)=" + sh + " Saturation(hsl)=" + ss + " Lightness(hsl)=" + sl;
  m_colorMsg2 = "Choice (R, G, B, H, S, L) and use mouse wheel to change color.";
  if(cursor().shape() == Qt::SplitHCursor)
    m_colorMsg2 += " Drag left button to change position.";
  if(cursor().shape() == Qt::SizeVerCursor)
    if(pos != 0. && pos != 1.)
      m_colorMsg2 += " Press left button to REMOVE the color.";

  qc.setRgba(r, g, b, color.getAlpha());           
  qc.getHsv(&h, &s, &v);
  sh.setNum(h);
  ss.setNum(s);
  sv.setNum(v);

  qc.setRgba(r, g, b, color.getAlpha());           
  qc.getCmyk(&c, &m, &y, &k);
  sc.setNum(c);
  sm.setNum(m);
  sy.setNum(y);
  sk.setNum(k);

  m_colorMsg3 = "Hue(hsv)=" + sh + "  Saturation(hsv)=" + ss + "  Value(hsv)=" + sv;
  m_colorMsg4 = "Cyan=" + sc + "  Magenta=" + sm + "  Yellow=" + sy + "  Black="+ sk + "  --- Trick: use capslock to get fine-tune";
}

// The following code uses the QT color transformations
// TerraLib        
//#include "ColorBar.h"
//#include <QtGui/qmouseevent>
//#include <QtGui/qkeyevent>
//#include <QtGui/qwheelevent>
//#include <QtGui/qresizeevent>
//#include <QtGui/qpainter>
//#include <QtGui/qpixmap>
//#include <QtGui/qbrush>
//#include <QtGui/qwskeyboardhandler>
//#include <QtCore/qrect>
//
//ColorBar::ColorBar(QWidget* parent)
//  : QDialog(parent),
//    m_colorBar(0),
//    m_cursorShape(Qt::ArrowCursor),
//    m_slices(5),
//    m_hoffset(20),
//    m_voffset(120),
//    m_key(Qt::Key_H),
//    m_capsLock(false),
//    m_pos(-1.),
//    m_editPos(-1.),
//    m_copypos(-1.)
//{
//  m_pol = new QPolygon(4);
//  m_pol->setPoint(0, 0, 0);
//  m_pol->setPoint(1, 3, -6);
//  m_pol->setPoint(2, -3, -6);
//  m_pol->setPoint(3, 0, 0);
//
//  setMinimumWidth(200);
//  setMinimumHeight(200);
//
//  setWindowTitle("Color bar example");
//  setMouseTracking(true);
//}
//
//ColorBar::~ColorBar()
//{
//  delete m_pol;
//}
//
//void ColorBar::mouseMoveEvent(QMouseEvent* e)
//{
//  int xi = m_hoffset;
//  int xf = m_width - m_hoffset;
//  int yi = m_voffset;
//  int yf = m_height - m_voffset/12;
//  int y = m_voffset / 2 + yf / 2 - 2;
//  int yy = m_voffset / 2 + yf / 2;
//
//  QRect rec(xi, yi, xf-xi+1, y-yi+1);
//  QRect rec2(xi, yy, xf - xi + 1, yf - yy + 1);
//  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//  std::map<double, te::color::RGBAColor>::const_iterator mit;
//  m_colorMsg.clear();
//  m_colorMsg2.clear();
//  m_colorMsg3.clear();
//  m_colorMsg4.clear();
//
//  if(rec.contains(e->pos()))
//  {
//    if(m_pos == -1)
//    {
//      int size = (int)colorMap.size();
//      int i;
//      for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
//      {
//        if(i == 0 || i == size-1)
//          continue;
//
//        int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//        int dx = abs(e->pos().x() - x);
//        if(dx < 2)
//        {
//          if(cursor().shape() != Qt::SplitHCursor)
//          {
//            QCursor cursor(Qt::SplitHCursor);
//            setCursor(cursor);
//            m_color = mit->second;
//            m_editPos = mit->first;
//          }
//          setColorMsg(mit->second, mit->first);
//          break;
//        }
//      }
//      if(mit == colorMap.end())
//      {
//        m_colorMsg = "Press mouse left button to insert a color";
//        if(cursor().shape() != Qt::PointingHandCursor)
//        {
//          QCursor cursor(Qt::PointingHandCursor);
//          setCursor(cursor);
//        }
//      }
//    }
//  }
//  else if(rec2.contains(e->pos()))
//  {
//    QString nSlices;
//    nSlices.setNum(m_slices);
//    m_colorMsg = "Use mouse whell to change number of color slices. slices=" + nSlices;
//    if(cursor().shape() != Qt::SizeHorCursor)
//    {
//      QCursor cursor(Qt::SizeHorCursor);
//      setCursor(cursor);
//    }
//  }
//  else
//  {
//    for(mit = colorMap.begin(); mit != colorMap.end(); ++mit)
//    {
//      int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//      QPolygon pol = m_pol->translated(x, yi);
//      if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
//      {
//        if(cursor().shape() != Qt::SizeVerCursor)
//        {
//          QCursor cursor(Qt::SizeVerCursor);
//          setCursor(cursor);
//          m_color = mit->second;
//          m_editPos = mit->first;
//          m_editPos = mit->first;
//        }
//        setColorMsg(mit->second, mit->first);
//        break;
//      }
//    }
//
//    if(mit == colorMap.end() && cursor().shape() != Qt::ArrowCursor)
//    {
//      if(cursor().shape() != Qt::ArrowCursor)
//      {
//        QCursor cursor(Qt::ArrowCursor);
//        setCursor(cursor);
//      }
//    }
//  }
//
//  int button = e->buttons();
//  if(button == Qt::LeftButton)
//  {
//    if(cursor().shape() == Qt::SplitHCursor)
//    {
//      if(m_pos != -1.)
//      {
//        std::map<double, te::color::RGBAColor>::const_iterator cit = m_colorBar->getColorMap().find(m_pos);
//        if(cit != m_colorBar->getColorMap().end())
//          setColorMsg(cit->second, cit->first);
//        int px = e->pos().x() - xi;
//        double newpos = (double)(px) / (double)(m_colorBar->getBarSize()-1);
//        m_pos = m_colorBar->move(m_pos, newpos);
//        m_editPos = m_pos;
//      }
//    }
//  }
//  repaint();
//}
//
//void ColorBar::mousePressEvent(QMouseEvent* e)
//{
//  int xi = m_hoffset;
//  int xf = m_width - m_hoffset;
//  int yi = m_voffset;
//  int yf = m_height - m_voffset/12;
//  int y = m_voffset / 2 + yf / 2 - 2;
//
//  QRect rec(xi, yi, xf-xi+1, y-yi+1);
//  if(rec.contains(e->pos()))
//  {
//    // add color
//    int shape = cursor().shape();
//    int button = e->button();
//    if(button == Qt::LeftButton && shape == Qt::PointingHandCursor)
//    {
//      te::color::RGBAColor c(0, 255, 128, 255);
//      int x = e->pos().x();
//      int p = x - xi;
//      int w = xf - xi + 1;
//      double dp = (double)p / (double)w;
//      if(dp > 1.)
//        dp = 1.;
//      m_colorBar->addColor(c, dp);
//      m_pos = dp;
//      if(cursor().shape() != Qt::SplitHCursor)
//      {
//        m_editPos = m_pos;
//        QCursor cursor(Qt::SplitHCursor);
//        setCursor(cursor);
//      }
//      setColorMsg(c, m_pos);
//      m_color = c;
//    }
//
//    // move color
//    else if(e->button() == Qt::LeftButton && cursor().shape() == Qt::SplitHCursor)
//    {
//      const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//
//      if((e->pos().x() - xi) > 2)
//      {
//        std::map<double, te::color::RGBAColor>::const_iterator mit;
//        int size = (int)colorMap.size();
//        int i;
//        for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
//        {
//          if(i == 0 || i == size-1)
//            continue;
//          int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//          int dx = abs(e->pos().x() - x);
//          te::color::RGBAColor c = mit->second;
//          setColorMsg(c, mit->first);
//          if(dx < 2)
//          {
//            m_pos = mit->first;
//            m_editPos = m_pos;
//            m_color = c;
//            break;
//          }
//        }
//        if(mit == colorMap.end())
//        {
//          m_pos = -1.;
//          m_editPos = m_pos;
//        }
//
//      }
//      else
//      {
//        std::map<double, te::color::RGBAColor>::const_reverse_iterator mit;
//        int size = (int)colorMap.size();
//        int i;
//        for(mit = colorMap.rbegin(), i=size-1; mit != colorMap.rend(); ++mit, --i)
//        {
//          if(i == 0 || i == size-1)
//            continue;
//          int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//          int dx = abs(e->pos().x() - x);
//          te::color::RGBAColor c = mit->second;
//          setColorMsg(c, mit->first);
//          if(dx < 2)
//          {
//            m_pos = mit->first;
//            m_editPos = m_pos;
//            m_color = c;
//            break;
//          }
//        }
//        if(mit == colorMap.rend())
//        {
//          m_pos = -1.;
//          m_editPos = m_pos;
//        }
//      }
//    }
//  }
//
//  //remove color
//  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//  std::map<double, te::color::RGBAColor>::const_iterator mit;
//  int size = (int)colorMap.size();
//  int i;
//  for(mit = colorMap.begin(), i=0; mit != colorMap.end(); ++mit, ++i)
//  {
//    if(i == 0 || i == size-1)
//      continue;
//    int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//    te::color::RGBAColor c = mit->second;
//    QPolygon pol = m_pol->translated(x, yi);
//    if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
//    {
//      if(e->button() == Qt::LeftButton)
//      {
//        m_colorBar->remove(mit->first);   
//        m_pos = -1.;
//        m_editPos = m_pos;
//        m_colorMsg.clear();
//        m_colorMsg2.clear();
//        m_colorMsg3.clear();
//        m_colorMsg4.clear();
//        break;
//      }
//    }
//  }
//  repaint();
//}
//
//void ColorBar::mouseReleaseEvent(QMouseEvent*)
//{
//  m_copypos = m_pos;
//  m_pos = -1.;
//}
//
//void ColorBar::keyPressEvent(QKeyEvent* e)
//{
//  if(e->key() == Qt::Key_M ||
//    e->key() == Qt::Key_H ||
//    e->key() == Qt::Key_S ||
//    e->key() == Qt::Key_L ||
//    e->key() == Qt::Key_R ||
//    e->key() == Qt::Key_G ||
//    e->key() == Qt::Key_B)
//  {
//    int key = m_key;
//    m_key = e->key();
//
//    if(key != m_key && m_colorMsg.isEmpty() == false)
//    {
//      if(!m_colorMsg.contains("Wheel: "))
//        return;
//
//      QChar abre('(');
//      QChar fecha(')');
//      int i, ini, fim;
//      i = 0;
//      while(m_colorMsg[i] != abre)
//        ++i;
//      ini = i + 1;
//      while(m_colorMsg[i] != fecha)
//        ++i;
//      fim = i - ini;
//
//      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
//        m_colorMsg.replace(ini, fim, "Hue");
//      else if(m_key == Qt::Key_R)
//        m_colorMsg.replace(ini, fim, "Red");
//      else if(m_key == Qt::Key_G)
//        m_colorMsg.replace(ini, fim, "Green");
//      else if(m_key == Qt::Key_B)
//        m_colorMsg.replace(ini, fim, "Blue");
//      else if(m_key == Qt::Key_S)
//        m_colorMsg.replace(ini, fim, "Saturation");
//      else if(m_key == Qt::Key_L)
//        m_colorMsg.replace(ini, fim, "Lightness");
//    
//      repaint();
//
//      if(cursor().shape() == Qt::SplitHCursor || cursor().shape() == Qt::SizeVerCursor)
//      {
//        const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//        std::map<double, te::color::RGBAColor>::const_iterator mit;
//        mit = colorMap.find(m_editPos);
//        if(mit != colorMap.end())
//          m_color = mit->second;
//      }
//    }
//  }
//}
//
//void ColorBar::keyReleaseEvent(QKeyEvent* e)
//{
//  if(e->key() == Qt::Key_CapsLock)
//  {
//    if(e->nativeModifiers() == 768)
//      m_capsLock = true;
//    else
//      m_capsLock = false;
//  }
//}
//
//void ColorBar::wheelEvent(QWheelEvent* e)
//{
//  int d = e->delta();
//
//  int xi = m_hoffset;
//  int xf = m_width - m_hoffset;
//  int yi = m_voffset;
//  int yf = m_height - m_voffset/12;
//
//  int y = m_voffset / 2 + yf / 2;
//  QRect rec(xi, y, xf-xi+1, yf-y+1);
//  if(rec.contains(e->pos()))
//  {
//    if(d > 0)
//      ++m_slices;
//    else
//      --m_slices;
//
//    if(m_slices > 50)
//      m_slices = 50;
//    if(m_slices < 1)
//      m_slices = 1;
//
//    QString nSlices;
//    nSlices.setNum(m_slices);
//    m_colorMsg = "Use mouse whell to change the number of color slices. Color slices = " + nSlices;
//    repaint();
//    return;
//  }
//
//  int ind;
//  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//  std::map<double, te::color::RGBAColor>::const_iterator mit;
//  for(mit = colorMap.begin(), ind = 0; mit != colorMap.end(); ++mit, ind++)
//  {
//    int x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//    te::color::RGBAColor c = mit->second;
//    QColor qc(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
//    QPolygon pol = m_pol->translated(x, yi);
//    if(pol.containsPoint(e->pos(), Qt::OddEvenFill))
//    {
//      int hh, ss, ll, a;
//      int h, s, l, r, g, b, d = 1;
//      QColor c(m_color.getRgba());
//      c.getHsl(&hh, &ss, &ll, &a);
//
//      QColor qc(mit->second.getRgba());
//      if(m_capsLock == false)
//        d = 8;
//      if(e->delta() < 0)
//        d *= -1;
//
//      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        h += d;
//
//        if(h > 359)
//          h -= 360;
//        if(h < 0)
//          h += 360;
//
//        qc.setHsl(h, s, l, a);
//        m_color = qc.rgba();
//      }
//      else if(m_key == Qt::Key_S)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        s += d;
//
//        if(s > 255)
//          s = 255;
//        else if(s < 0)
//          s = 0;
//
//        qc.setHsl(hh, s, ll, a);
//      }
//      else if(m_key == Qt::Key_L)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        l += d;
//
//        if(l > 255)
//          l = 255;
//        if(l < 0)
//          l = 0;
//
//        qc.setHsl(hh, ss, l, a);
//      }
//      else if(m_key == Qt::Key_R)
//      {
//        r = qc.red();
//        r += d;
//
//        if(r > 255)
//          r = 255;
//        if(r < 0)
//          r = 0;
//
//        qc.setRed(r);
//      }
//      else if(m_key == Qt::Key_G)
//      {
//        g = qc.green();
//        g += d;
//
//        if(g > 255)
//          g = 255;
//        if(g < 0)
//          g = 0;
//
//        qc.setGreen(g);
//      }
//      else if(m_key == Qt::Key_B)
//      {
//        b = qc.blue();
//        b += d;
//
//        if(b > 255)
//          b = 255;
//        if(b < 0)
//          b = 0;
//
//        qc.setBlue(b);
//      }
//
//      te::color::RGBAColor cor(qc.red(), qc.green(), qc.blue(), qc.alpha());
//      m_colorBar->changeColor(ind, cor);
//      repaint();
//      setColorMsg(cor, mit->first);
//      return;
//    }
//  }
//
//  if(cursor().shape() == Qt::SplitHCursor)
//  {
//    mit = colorMap.find(m_copypos);
//    if(mit != colorMap.end())
//    {
//      int hh, ss, ll, a;
//      int h, s, l, r, g, b, d = 1;
//      QColor c(m_color.getRgba());
//      c.getHsl(&hh, &ss, &ll, &a);
//
//      QColor qc(mit->second.getRgba());
//
//      if(m_capsLock == false)
//        d = 8;
//      if(e->delta() < 0)
//        d *= -1;
//
//      if(m_key == Qt::Key_H || m_key == Qt::Key_M)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        h += d;
//
//        if(h > 359)
//          h -= 360;
//        if(h < 0)
//          h += 360;
//
//        qc.setHsl(h, s, l, a);
//        m_color = qc.rgba();
//      }
//      else if(m_key == Qt::Key_S)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        s += d;
//
//        if(s > 255)
//          s = 255;
//        else if(s < 0)
//          s = 0;
//
//        qc.setHsl(hh, s, ll, a);
//      }
//      else if(m_key == Qt::Key_L)
//      {
//        qc.getHsl(&h, &s, &l, &a);
//        l += d;
//
//        if(l > 255)
//          l = 255;
//        if(l < 0)
//          l = 0;
//
//        qc.setHsl(hh, ss, l, a);
//      }
//      else if(m_key == Qt::Key_R)
//      {
//        r = qc.red();
//        r += d;
//
//        if(r > 255)
//          r = 255;
//        if(r < 0)
//          r = 0;
//
//        qc.setRed(r);
//        m_color = qc.rgba();
//      }
//      else if(m_key == Qt::Key_G)
//      {
//        g = qc.green();
//        g += d;
//
//        if(g > 255)
//          g = 255;
//        if(g < 0)
//          g = 0;
//
//        qc.setGreen(g);
//        m_color = qc.rgba();
//      }
//      else if(m_key == Qt::Key_B)
//      {
//        b = qc.blue();
//        b += d;
//
//        if(b > 255)
//          b = 255;
//        if(b < 0)
//          b = 0;
//
//        qc.setBlue(b);
//        m_color = qc.rgba();
//      }
//
//      te::color::RGBAColor cor = qc.rgba();
//      m_colorBar->changeColor(m_copypos, cor);
//      setColorMsg(cor, mit->first);
//      repaint();
//    }
//  }
//}
//
//void ColorBar::resizeEvent(QResizeEvent* e)
//{
//  m_width = e->size().width();
//  m_height = e->size().height();
//
//  if(m_colorBar == 0)
//    m_colorBar = new te::color::ColorBar(m_width - m_hoffset - m_hoffset + 1);
//  else
//    m_colorBar->setBarSize(m_width - m_hoffset - m_hoffset + 1);
//}
//
//void ColorBar::paintEvent(QPaintEvent*)
//{
//  double size;
//  int x, y, isize;
//  te::color::RGBAColor c;
//  std::vector<te::color::RGBAColor>::const_iterator it;
//  QPainter painter;
//  QPixmap pix(m_width, m_height);
//  pix.fill();
//  painter.begin(&pix);
//
//  int xi = m_hoffset;
//  int xf = m_width - m_hoffset;
//  int yi = m_voffset;
//  int yf = m_height - m_voffset/12;
//
//  const std::vector<te::color::RGBAColor>& colors = m_colorBar->getColorBar();
//  y = m_voffset / 2 + yf / 2 - 2;
//  x = xi;
//  for(it = colors.begin(); it != colors.end(); ++it)
//  {
//    c = *it;
//    QColor qc(c.getRed(), c.getGreen(), c.getBlue());
//    painter.setPen(qc);
//    painter.drawLine(QPoint(x, yi), QPoint(x, y));
//    ++x;
//  }
//
//  x = xi;
//  int ii = 1;
//  y = m_voffset / 2 + yf / 2;
//  const std::vector<te::color::RGBAColor>& scolors = m_colorBar->getSlices(m_slices);
//  for(it = scolors.begin(); it != scolors.end(); ++ii, ++it)
//  {
//    c = *it;
//    QColor qc(c.getRed(), c.getGreen(), c.getBlue());
//    painter.setPen(qc);
//    size = (double)(xf-xi+1)/(double)m_slices;
//    isize = (int)((double)ii * size + .5) + m_hoffset;
//    while(x <= isize-2 && (isize-2) < xf)
//    {
//      painter.drawLine(QPoint(x, y), QPoint(x, yf));
//      ++x;
//    }
//    ++x;
//  }
//
//  painter.setPen(Qt::black);
//  painter.drawLine(QPoint(xi, yi), QPoint(xi, yf));
//  painter.drawLine(QPoint(xi, yf), QPoint(xf, yf));
//  painter.drawLine(QPoint(xf, yf), QPoint(xf, yi));
//  painter.drawLine(QPoint(xf, yi), QPoint(xi, yi));
//
//  const std::map<double, te::color::RGBAColor>& colorMap = m_colorBar->getColorMap();
//  std::map<double, te::color::RGBAColor>::const_iterator mit;
//  for(mit = colorMap.begin(); mit != colorMap.end(); ++mit)
//  {
//    x = (int)(mit->first * (double)m_colorBar->getBarSize() + .5) + xi;
//    c = mit->second;
//    QColor qc(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
//    painter.setBrush(QBrush(qc));
//    painter.save();
//    painter.translate(x, yi);
//    painter.drawPolygon(*m_pol);
//    painter.restore();
//  }
//
//  QFont font("Times", 12);
//  painter.setFont(font);
//  painter.setPen(Qt::black);
//  QBrush brush(Qt::black);
//  painter.setBrush(brush);
//  if(m_colorMsg.isEmpty() == false)
//    painter.drawText(10, 20, m_colorMsg);
//  if(m_colorMsg2.isEmpty() == false)
//    painter.drawText(10, 40, m_colorMsg2);
//  painter.drawLine(0, 45, m_width, 45);
//  if(m_colorMsg3.isEmpty() == false)
//    painter.drawText(10, 70, m_colorMsg3);
//  if(m_colorMsg4.isEmpty() == false)
//    painter.drawText(10, 90, m_colorMsg4);
//  painter.end();
//  painter.begin(this);
//  painter.drawPixmap(0, 0, pix.width(), pix.height(), pix);
//  painter.end();
//}
//
//void ColorBar::setColorMsg(const te::color::RGBAColor& color, const double& pos)
//{
//  QColor qc;
//  int h, s, v, l, c, m, y, k;
//  QString sr, sg, sb, sh, ss, sv, sl, sc, sm, sy, sk;
//
//  int r = color.getRed();
//  int g = color.getGreen();
//  int b = color.getBlue();
//
//  qc.setRgb(r, g, b, color.getAlpha());           
//  qc.getHsl(&h, &s, &l);
//
//  m_colorMsg = "Wheel: ";
//  if(m_key == Qt::Key_H || m_key == Qt::Key_M)
//    m_colorMsg += "(Hue)  ";
//  else if(m_key == Qt::Key_R)
//    m_colorMsg += "(Red)  ";
//  else if(m_key == Qt::Key_G)
//    m_colorMsg += "(Green)  ";
//  else if(m_key == Qt::Key_B)
//    m_colorMsg += "(Blue)  ";
//  else if(m_key == Qt::Key_S)
//    m_colorMsg += "(Saturation)  ";
//  else if(m_key == Qt::Key_L)
//    m_colorMsg += "(Lightness)  ";
//
//  sr.setNum(r);
//  sg.setNum(g);
//  sb.setNum(b);
//  sh.setNum(h);
//  ss.setNum(s);
//  sl.setNum(l);
//
//  m_colorMsg += "Red=" + sr + " Green=" + sg + " Blue=" + sb + " Hue=" + sh + " Saturation=" + ss + " Lightness=" + sl;
//  m_colorMsg2 = "Choice (R, G, B, H, S, L) and use mouse wheel to change color.";
//  if(cursor().shape() == Qt::SplitHCursor)
//    m_colorMsg2 += " Drag left button to change position.";
//  if(cursor().shape() == Qt::SizeVerCursor)
//    if(pos != 0. && pos != 1.)
//      m_colorMsg2 += " Press left button to REMOVE the color.";
//
//  qc.setRgba(r, g, b, color.getAlpha());           
//  qc.getHsv(&h, &s, &v);
//  sh.setNum(h);
//  ss.setNum(s);
//  sv.setNum(v);
//
//  qc.setRgba(r, g, b, color.getAlpha());           
//  qc.getCmyk(&c, &m, &y, &k);
//  sc.setNum(c);
//  sm.setNum(m);
//  sy.setNum(y);
//  sk.setNum(k);
//
//  m_colorMsg3 = "Hue(hsv)=" + sh + "  Saturation(hsv)=" + ss + "  Value(hsv)=" + sv;
//  m_colorMsg4 = "Cyan=" + sc + "  Magenta=" + sm + "  Yellow=" + sy + "  Black="+ sk + "  --- Trick: use capslock to get fine-tune";
//}
