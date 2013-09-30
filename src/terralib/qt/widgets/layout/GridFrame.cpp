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
  \file terralib/qt/widgets/canvas/TextFrame.h

  \brief The frame data....
*/

// TerraLib
#include "GridFrame.h"
#include "DataFrame.h"
#include "LayoutEditor.h"
#include "../canvas/MultiThreadMapDisplay.h"

// Qt
#include <QtGui/QPainter>

te::qt::widgets::GridFrame::GridFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f) :
  Frame(df->getLayoutEditor(), f),
  m_dataFrame(df),
  m_hSpace(100),
  m_vSpace(100),
  m_verticalLabelSpace(2),
  m_horizontalLabelSpace(2),
  m_verticalLabelAngle(0),
  m_horizontalLabelAngle(90)
{
  adjust();

  m_font = QFont("Lucida Sans", 10);

  QRectF dr = m_dataFrame->getDataRect();
  if(dr.isValid())
  {
    m_initialCoordX = dr.left();
    m_deltaX = (dr.right() - dr.left()) / 5.;
    m_initialCoordY = dr.top();
    m_deltaY = (dr.bottom() - dr.top()) / 5.;
  }
  else // so para teste
  {
    m_initialCoordX = 10;
    m_deltaX = 5;
    m_initialCoordY = 100;
    m_deltaY = 5;
  }
}

te::qt::widgets::GridFrame::GridFrame(const GridFrame& rhs) :
  Frame(rhs.m_layoutEditor, rhs.windowFlags())
{
  m_dataFrame = rhs.m_dataFrame;
  m_hSpace = rhs.m_hSpace;
  m_vSpace = rhs.m_vSpace;
  m_verticalLabelSpace = rhs.m_verticalLabelSpace;
  m_horizontalLabelSpace = rhs.m_horizontalLabelSpace;
  m_verticalLabelAngle = rhs.m_verticalLabelAngle;
  m_horizontalLabelAngle = rhs.m_horizontalLabelAngle;

  adjust();

  m_font = rhs.m_font;
  m_initialCoordX = rhs.m_initialCoordX;
  m_deltaX = rhs.m_deltaX;
  m_initialCoordY = rhs.m_initialCoordY;
  m_deltaY = rhs.m_deltaY;
}

te::qt::widgets::GridFrame::~GridFrame()
{
  hide();
}

te::qt::widgets::GridFrame& te::qt::widgets::GridFrame::operator=(const GridFrame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::Frame::operator=(rhs);
    m_dataFrame = rhs.m_dataFrame;
    m_hSpace = rhs.m_hSpace;
    m_vSpace = rhs.m_vSpace;
    m_verticalLabelSpace = rhs.m_verticalLabelSpace;
    m_horizontalLabelSpace = rhs.m_horizontalLabelSpace;
    m_verticalLabelAngle = rhs.m_verticalLabelAngle;
    m_horizontalLabelAngle = rhs.m_horizontalLabelAngle;

    adjust();

    m_font = rhs.m_font;
    m_initialCoordX = rhs.m_initialCoordX;
    m_deltaX = rhs.m_deltaX;
    m_initialCoordY = rhs.m_initialCoordY;
    m_deltaY = rhs.m_deltaY;
  }

  return *this;
}

void te::qt::widgets::GridFrame::adjust()
{
  // centralize deixando espacos na horizontal e na vertical
  // para poder colocar os labels.

  int w = m_dataFrame->getMapDisplay()->getDisplayPixmap()->width();
  int h = m_dataFrame->getMapDisplay()->getDisplayPixmap()->height();

  double scale = (double)w / m_dataFrame->getFrameRect().width();

  int hs = (int)(m_hSpace * scale + .5);
  int vs = (int)(m_vSpace * scale + .5);
  int tw = 2 * hs + w;
  int th = 2 * vs + h;
  resize(tw, th);

  m_pixmap = QPixmap(tw, th);
  m_pixmap.fill(Qt::transparent);

  //int x = m_dataFrame->x() + m_layoutEditor->getVerticalRulerWidth();
  //int y = m_dataFrame->y() + m_layoutEditor->getHorizontalRulerWidth();
  int x = m_dataFrame->x();
  int y = m_dataFrame->y();

  move(x - hs + 9, y - vs + 9);
}

void te::qt::widgets::GridFrame::copyToDraftMapDisplay()
{
  // copie sobre o draft pixmap do map display
  QPixmap* dPixmap = m_dataFrame->getMapDisplay()->getDraftPixmap();
  QRect dr = dPixmap->rect();
  dr.moveCenter(m_pixmap.rect().center());
  QPainter painter(dPixmap);
  painter.drawPixmap(dPixmap->rect(), m_pixmap, dr);
  painter.end();
}

void te::qt::widgets::GridFrame::setDataFrame(te::qt::widgets::DataFrame* df)
{
  m_dataFrame = df;
}

QFont& te::qt::widgets::GridFrame::getFont()
{
  return m_font;
}

unsigned int te::qt::widgets::GridFrame::getVerticalLabelSpace()
{
  return m_verticalLabelSpace;
}

void te::qt::widgets::GridFrame::setVerticalLabelSpace(unsigned int s)
{
  m_verticalLabelSpace = s;
}

unsigned int te::qt::widgets::GridFrame::getHorizontalLabelSpace()
{
  return m_horizontalLabelSpace;
}

void te::qt::widgets::GridFrame::setHorizontalLabelSpace(unsigned int s)
{
  m_horizontalLabelSpace = s;
}

int te::qt::widgets::GridFrame::getVerticalLabelAngle()
{
  return m_verticalLabelAngle;
}

void te::qt::widgets::GridFrame::setVerticalLabelAngle(int a)
{
  m_verticalLabelAngle = a;
}

int te::qt::widgets::GridFrame::getHorizontalLabelAngle()
{
  return m_horizontalLabelAngle;
}

void te::qt::widgets::GridFrame::setHorizontalLabelAngle(int a)
{
  m_horizontalLabelAngle = a;
}

double te::qt::widgets::GridFrame::getInitialCoordX()
{
  return m_initialCoordX;
}

void te::qt::widgets::GridFrame::setInitialCoordX(double x)
{
  m_initialCoordX = x;
}

double te::qt::widgets::GridFrame::getDeltaX()
{
  return m_deltaX;
}

void te::qt::widgets::GridFrame::setDeltaX(double x)
{
  m_deltaX = x;
}

double te::qt::widgets::GridFrame::getInitialCoordY()
{
  return m_initialCoordY;
}

void te::qt::widgets::GridFrame::setInitialCoordY(double y)
{
  m_initialCoordY = y;
}

double te::qt::widgets::GridFrame::getDeltaY()
{
  return m_deltaY;
}

void te::qt::widgets::GridFrame::setDeltaY(double y)
{
  m_deltaY = y;
}

unsigned int te::qt::widgets::GridFrame::getHSpace()
{
  return m_hSpace;
}

void te::qt::widgets::GridFrame::setHSpace(unsigned int h)
{
  m_hSpace = h;
}

unsigned int te::qt::widgets::GridFrame::getVSpace()
{
  return m_vSpace;
}

void te::qt::widgets::GridFrame::setVSpace(unsigned int v)
{
  m_vSpace = v;
}
