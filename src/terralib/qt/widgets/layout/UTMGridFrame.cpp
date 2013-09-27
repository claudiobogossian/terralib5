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
#include "UTMGridFrame.h"
#include "DataFrame.h"
#include "LayoutEditor.h"
#include "../canvas/MultiThreadMapDisplay.h"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtCore/QRect>

te::qt::widgets::UTMGridFrame::UTMGridFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f) :
  te::qt::widgets::GridFrame(df, f)
{
  setMouseTracking(true);
  show();
  installEventFilter(this);
}

te::qt::widgets::UTMGridFrame::UTMGridFrame(const UTMGridFrame& rhs) :
  GridFrame(rhs.m_dataFrame, rhs.windowFlags())
{
  setMouseTracking(true);
  show();
  installEventFilter(this);
}

te::qt::widgets::UTMGridFrame::~UTMGridFrame()
{
}

te::qt::widgets::UTMGridFrame& te::qt::widgets::UTMGridFrame::operator=(const UTMGridFrame& rhs)
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

void te::qt::widgets::UTMGridFrame::adjust()
{
  te::qt::widgets::GridFrame::adjust();
//  draw();
}

void te::qt::widgets::UTMGridFrame::draw()
{
  int i, n = 10;
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
  painter.setPen(Qt::blue);

  for(i = 0; i <= n; ++i) // linhas verticais
    painter.drawLine(QPoint(i * dw, 0), QPoint(i * dw, h-1));
  for(i = 0; i <= n; ++i) // linhas horizontais
    painter.drawLine(QPoint(0, i * dh), QPoint(w-1, i * dh));

  painter.end();
  painter.begin(&m_pixmap);
  painter.setPen(Qt::blue);

  QRect r = m_pixmap.rect();
  QPoint c = r.center();
  rr.moveCenter(c);
  painter.drawPixmap(rr, p, p.rect());

  for(i = 0; i <= n; ++i) // labels do x
  {
    painter.drawLine(QPoint(rr.left() + i * dw - 5, rr.top() - 3), QPoint(rr.left() + (i * dw) + 5, rr.top() - 3));
    painter.drawLine(QPoint(rr.left() + i * dw - 5, rr.bottom() + 3), QPoint(rr.left() + (i * dw) + 5, rr.bottom() + 3));
  }
  for(i = 0; i <= n; ++i) // linhas do y
  {
    painter.drawLine(QPoint(rr.left() - 3, rr.top() + i * dh), QPoint(rr.left() - 13, rr.top() + i * dh));
    painter.drawLine(QPoint(rr.right() + 3, rr.top() + i * dh), QPoint(rr.right() + 13, rr.top() + i * dh));
  }
  painter.end();

  copyToDraftMapDisplay();

  update();
//  raise();
}

void te::qt::widgets::UTMGridFrame::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, m_pixmap);
}

bool te::qt::widgets::UTMGridFrame::eventFilter(QObject* obj, QEvent* e)
{
  // return true to stop the event; otherwise return false.

  //if(obj == this) 
  //{
  //  if(e->type() == QEvent::MouseMove)
  //  {
  //    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
  //    if(mouseEvent->buttons() == Qt::NoButton)
  //    {
  //      lower();
  //      m_layoutEditor->setFrameSelected(0);
  //      return true;
  //    }
  //  }
  //  else if(e->type() == QEvent::Enter)
  //  {
  //    lower();
  //    m_layoutEditor->setFrameSelected(0);
  //    return true;
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
