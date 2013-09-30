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
#include "GeographicGridFrame.h"
#include "DataFrame.h"
#include "../canvas/MultiThreadMapDisplay.h"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtCore/QRect>

te::qt::widgets::GeographicGridFrame::GeographicGridFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f) :
  te::qt::widgets::GridFrame(df, f)
{
  setMouseTracking(true);
  show();
  installEventFilter(this);
}

te::qt::widgets::GeographicGridFrame::GeographicGridFrame(const GeographicGridFrame& rhs) :
  GridFrame(rhs.m_dataFrame, rhs.windowFlags())
{
  setMouseTracking(true);
  show();
  installEventFilter(this);
}

te::qt::widgets::GeographicGridFrame::~GeographicGridFrame()
{
  hide();
}

te::qt::widgets::GeographicGridFrame& te::qt::widgets::GeographicGridFrame::operator=(const GeographicGridFrame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::GridFrame::operator=(rhs);
    setMouseTracking(true);
    show();
    installEventFilter(this);
  }

  return *this;
}

void te::qt::widgets::GeographicGridFrame::adjust()
{
  te::qt::widgets::GridFrame::adjust();
  draw();
}

void te::qt::widgets::GeographicGridFrame::draw()
{
  int i, n = 4;
  QRect rr = m_dataFrame->getMapDisplay()->getDisplayPixmap()->rect();
  double w = rr.width();
  int dw = w / n;
  double h = rr.height();
  int dh = h / n;

  QPixmap p(w, h);
  p.fill(Qt::transparent);
  m_pixmap.fill(Qt::transparent);

  QPainter painter(&p);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);

  for(i = 0; i <= n; ++i) // linhas verticais
    painter.drawLine(QPoint(i * dw, 0), QPoint(i * dw, h-1));
  for(i = 0; i <= n; ++i) // linhas horizontais
    painter.drawLine(QPoint(0, i * dh), QPoint(w-1, i * dh));

  painter.end();
  painter.begin(&m_pixmap);
  painter.setPen(Qt::red);

  QRect r = m_pixmap.rect();
  QPoint c = r.center();
  rr.moveCenter(c);
  painter.drawPixmap(rr, p, p.rect());

  for(i = 0; i <= n; ++i) // labels do x
  {
    painter.drawLine(QPoint(rr.left() + i * dw - 5, rr.top() - 6), QPoint(rr.left() + (i * dw) + 5, rr.top() - 6));
    painter.drawLine(QPoint(rr.left() + i * dw - 5, rr.bottom() + 6), QPoint(rr.left() + (i * dw) + 5, rr.bottom() + 6));
  }
  for(i = 0; i <= n; ++i) // linhas do y
  {
    painter.drawLine(QPoint(rr.left() - 6, rr.top() + i * dh), QPoint(rr.left() - 16, rr.top() + i * dh));
    painter.drawLine(QPoint(rr.right() + 6, rr.top() + i * dh), QPoint(rr.right() + 16, rr.top() + i * dh));
  }
  painter.end();

  copyToDraftMapDisplay();

  update();
  //raise();
}

void te::qt::widgets::GeographicGridFrame::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, m_pixmap);
}

bool te::qt::widgets::GeographicGridFrame::eventFilter(QObject* obj, QEvent* e)
{
  // return true to stop the event; otherwise return false.

  //if(obj == this) 
  //{
  //  if(e->type() == QEvent::MouseMove)
  //  {
  //    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
  //    if(mouseEvent->buttons() == Qt::NoButton)
  //    {
  //      QRect r = m_dataFrame->getMapDisplay()->getDisplayPixmap()->rect();
  //      r.moveCenter(m_pixmap.rect().center());
  //      QPoint p = mouseEvent->pos();
  //      if(r.contains(p))
  //        lower();
  //    }
  //  }
  //}
  if(obj == this) 
  {
    if(e->type() == QEvent::MouseMove)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
      if(mouseEvent->buttons() == Qt::NoButton)
        lower();
    }
  }

  // pass the event on to the parent class
  return QWidget::eventFilter(obj, e);
}
