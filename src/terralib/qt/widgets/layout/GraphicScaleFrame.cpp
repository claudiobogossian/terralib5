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
  \file terralib/qt/widgets/canvas/ScaleFrame.h

  \brief The frame data....
*/

// TerraLib
#include "GraphicScaleFrame.h"
#include "LayoutEditor.h"
#include "DataFrame.h"
#include "../canvas/MultiThreadMapDisplay.h"

#include <terralib/geometry.h>
#include <terralib/common.h>
#include <terralib/srs.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// Qt
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

#include <QPainter>
#include <QColor>
#include <QRect>
#include <QWidget>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QToolTip>
#include <QResizeEvent>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QtCore/qmath.h>

te::qt::widgets::GraphicScaleFrame::GraphicScaleFrame(te::qt::widgets::DataFrame* df, Qt::WindowFlags f) :
  te::qt::widgets::Frame(df->getLayoutEditor(), f),
  m_dataFrame(df),
  m_type(4),
  m_font(QFont("Linux Libertine G")),
  m_pointSize(12),
  m_labelColor(QColor(Qt::blue)),
  m_barColor(QColor(Qt::red)),
  m_step(0),
  m_divisions(4),
  m_subDivisions(4),
  m_labelAbove(true),
  m_subTickSize(3),
  m_tickCentralized(true),
  m_space(1),
  m_unit("Kilometers"),
  m_unitAbove(false),
  m_offsetOrigin(false)
{
  setWindowTitle("GraphicScaleFrame");
  setMouseTracking(true);

  QRectF r = m_dataFrame->getFrameRect();
  m_frameRect = QRectF(0, 0, r.width() / 2, 30);
  m_frameRect.moveCenter(QPointF(r.center().x(), r.center().y() - r.height() / 2 - 30));
  m_barRect = QRectF(m_frameRect.width() / 4 , 0, m_frameRect.width() / 2, 3);

  // calcular o tamanho do widget e coloque sobre o map editor
  adjust();

  draw();
  show();
  createMenu();
}

te::qt::widgets::GraphicScaleFrame::GraphicScaleFrame(const GraphicScaleFrame& rhs) :
  te::qt::widgets::Frame(rhs.m_layoutEditor, rhs.windowFlags())
{
  te::qt::widgets::Frame::operator=(rhs);
  setWindowTitle("GraphicScaleFrame");
  setMouseTracking(true);

  m_dataFrame = rhs.m_dataFrame;
  m_type = rhs.m_type;
  m_font = rhs.m_font;
  m_pointSize = rhs.m_pointSize;
  m_labelColor = rhs.m_labelColor;
  m_labels =  rhs.m_labels;
  m_labelLengths =  rhs.m_labelLengths;
  m_labelHeight =  rhs.m_labelHeight;
  m_barColor = rhs.m_barColor;
  m_barRect = rhs.m_barRect;
  m_step = rhs.m_step;
  m_divisions = rhs.m_divisions;
  m_subDivisions = rhs.m_subDivisions;
  m_subDivisionString = rhs.m_subDivisionString;
  m_subDivisionLabelLength = rhs.m_subDivisionLabelLength;
  m_labelAbove = rhs.m_labelAbove;
  m_subTickSize = rhs.m_subTickSize;
  m_tickCentralized = rhs.m_tickCentralized;
  m_space = rhs.m_space;
  m_unit = rhs.m_unit;
  m_unitAbove = rhs.m_unitAbove;
  m_offsetOrigin = rhs.m_offsetOrigin;

  // calcular o tamanho do widget e coloque sobre o map editor
  adjust();

  draw();
  show();
  createMenu();
}

te::qt::widgets::GraphicScaleFrame::~GraphicScaleFrame()
{
  hide();
}

te::qt::widgets::GraphicScaleFrame& te::qt::widgets::GraphicScaleFrame::operator=(const GraphicScaleFrame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::Frame::operator=(rhs);
    setWindowTitle("GraphicScaleFrame");

    m_dataFrame = rhs.m_dataFrame;
    m_type = rhs.m_type;
    m_font = rhs.m_font;
    m_pointSize = rhs.m_pointSize;
    m_labelColor = rhs.m_labelColor;
    m_labels =  rhs.m_labels;
    m_labelLengths =  rhs.m_labelLengths;
    m_labelHeight =  rhs.m_labelHeight;
    m_barColor = rhs.m_barColor;
    m_barRect = rhs.m_barRect;
    m_step = rhs.m_step;
    m_divisions = rhs.m_divisions;
    m_subDivisions = rhs.m_subDivisions;
    m_subDivisionString = rhs.m_subDivisionString;
    m_subDivisionLabelLength = rhs.m_subDivisionLabelLength;
    m_labelAbove = rhs.m_labelAbove;
    m_subTickSize = rhs.m_subTickSize;
    m_tickCentralized = rhs.m_tickCentralized;
    m_space = rhs.m_space;
    m_unit = rhs.m_unit;
    m_unitAbove = rhs.m_unitAbove;
    m_offsetOrigin = rhs.m_offsetOrigin;

    // calcular o tamanho do widget e coloque sobre o map editor
    adjust();

    draw();
    show();
    createMenu();
  }

  return *this;
}

void te::qt::widgets::GraphicScaleFrame::createMenu()
{
    m_menu = new QMenu(this);
    m_setTypeAction = m_menu->addAction("Set Type...");
    m_setFamilyAction = m_menu->addAction("Set Family...");
    m_setLabelColorAction = m_menu->addAction("Set Label Color...");
    m_setBarColorAction = m_menu->addAction("Set Bar Color...");
    m_setBarHeightAction = m_menu->addAction("Set Bar Height...");
    m_offsetOriginAction = m_menu->addAction("Offset Origin");
    m_offsetOriginAction->setCheckable(true);
    m_offsetOriginAction->setChecked(m_offsetOrigin);
    m_setLabelAboveAction = m_menu->addAction("Label Above");
    m_setLabelAboveAction->setCheckable(true);
    m_setLabelAboveAction->setChecked(m_labelAbove);
    m_setUnitAboveAction = m_menu->addAction("Unit Above");
    m_setUnitAboveAction->setCheckable(true);
    m_setUnitAboveAction->setChecked(m_unitAbove);
    m_setUnitAction = m_menu->addAction("Set Unit...");
    m_setDivisionsAction = m_menu->addAction("Set Number Of Divisions...");
    m_setSubDivisionsAction = m_menu->addAction("Set Number Of Subdivisions...");
    m_setStepAction = m_menu->addAction("Set Step...");
    m_showTooTipAction = m_menu->addAction("Show Tooltip");
    m_showTooTipAction->setCheckable(true);
    m_showTooTipAction->setChecked(m_showToolTip);
}

void te::qt::widgets::GraphicScaleFrame::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, m_pixmap);
}

QPixmap* te::qt::widgets::GraphicScaleFrame::getPixmap()
{
  return &m_pixmap;
}

te::qt::widgets::DataFrame* te::qt::widgets::GraphicScaleFrame::getDataFrame()
{
  return m_dataFrame;
}

void te::qt::widgets::GraphicScaleFrame::setDataFrame(te::qt::widgets::DataFrame* df)
{
  m_dataFrame = df;
}

void te::qt::widgets::GraphicScaleFrame::draw()
{
  m_pixmap.fill(Qt::transparent);

  drawLabels();

  if(m_type == 1)
    drawBar1();
  else if(m_type == 2)
    drawBar2();
  else if(m_type == 3)
    drawBar3();
  else if(m_type == 4)
    drawBar4();
  update();
}

void te::qt::widgets::GraphicScaleFrame::drawLabels()
{
  if(m_labelLengths.empty())
    return;

  QMatrix m = m_layoutEditor->getMatrixPaperViewToVp();
  QMatrix matrix(m.m11(), m.m12(), m.m21(), m.m22(), -m_frameRect.left()* m.m11(), -m_frameRect.bottom() * m.m22());
  QPainter painter(&m_pixmap);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(m_labelColor);
  painter.setFont(m_font);

  double conv = m_dataFrame->getDataUnitToMilimeter();
  double toKm = conv / 1000000;

  // put values
  double textHeight = m_labelHeight * .9;
  QPoint p;
  for(int i = 0; i <= m_divisions; ++i)
  {
    QString s = m_labels[i];

    if(m_labelAbove)
      p = matrix.map(QPointF(m_barRect.left() - m_labelLengths[i] / 2. + m_stepmm * (double)i, m_barRect.bottom() + m_space)).toPoint();
    else
      p = matrix.map(QPointF(m_barRect.left() - m_labelLengths[i] / 2. + m_stepmm * (double)i, m_barRect.top() - m_space - textHeight)).toPoint();

    painter.drawText(p, s);
  }

  // colocar o label da metade da primeira divisao
  if(m_labelAbove)
    p = matrix.map(QPointF(m_barRect.left() - m_subDivisionLabelLength / 2. + m_stepmm/2., m_barRect.bottom() + m_space)).toPoint();
  else
    p = matrix.map(QPointF(m_barRect.left() - m_subDivisionLabelLength / 2. + m_stepmm/2., m_barRect.top() - m_space - textHeight)).toPoint();

  painter.drawText(p, m_subDivisionString);

  // put unit
  double y;
  if(m_labelAbove && m_unitAbove)
    y = m_barRect.bottom() + m_space + textHeight + m_space;
  else if(m_labelAbove && m_unitAbove == false)
    y = m_barRect.top() - m_space - textHeight + 1; // somando 1 para nao cortar o unit
  else if(m_labelAbove == false && m_unitAbove == false)
    y = m_barRect.top() - m_space - textHeight - m_space - textHeight + 1; // somando 1 para nao cortar o unit
  else if(m_labelAbove == false && m_unitAbove)
    y = m_barRect.bottom() + m_space + 1; // somando 1 para o unit não ficar muito proximo aa barra

  QRectF rt = painter.boundingRect(QRect(0, 0, 100000, 100000), Qt::AlignLeft, m_unit);
  rt = matrix.inverted().mapRect(rt);
  double x = m_barRect.center().x() - rt.width() / 2;

  p = matrix.map(QPointF(x, y)).toPoint();
  painter.setPen(m_labelColor);
  painter.drawText(p, m_unit);
}

void te::qt::widgets::GraphicScaleFrame::drawBar1()
{
  QMatrix m = m_layoutEditor->getMatrixPaperViewToVp();
  QMatrix matrix(m.m11(), m.m12(), m.m21(), m.m22(), -m_frameRect.left()* m.m11(), -m_frameRect.bottom() * m.m22());
  QPainter painter(&m_pixmap);
  painter.setBrush(Qt::NoBrush);

  double subStep = m_stepmm / (double)m_subDivisions;

  // draw bar line
  painter.setPen(m_barColor);
  QPoint p, p1, p2;
  double y1, y2;
  if(m_tickCentralized)
  {
    p1 = matrix.map(QPointF(m_barRect.left(), m_barRect.center().y())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.right(), m_barRect.center().y())).toPoint();
    y1 = m_barRect.center().y() - m_subTickSize / 2.;
    y2 = m_barRect.center().y() + m_subTickSize / 2.;
  }
  else
  {
    if(m_labelAbove)
    {
      p1 = matrix.map(QPointF(m_barRect.left(), m_barRect.top())).toPoint();
      p2 = matrix.map(QPointF(m_barRect.right(), m_barRect.top())).toPoint();
      y1 = m_barRect.top();
      y2 = m_barRect.top() + m_subTickSize;
    }
    else
    {
      p1 = matrix.map(QPointF(m_barRect.left(), m_barRect.bottom())).toPoint();
      p2 = matrix.map(QPointF(m_barRect.right(), m_barRect.bottom())).toPoint();
      y1 = m_barRect.bottom() - m_subTickSize;
      y2 = m_barRect.bottom() + m_subTickSize;
    }
  }
  painter.drawLine(p1, p2);

  // put subdivision ticks
  for(int i = 0; i < m_subDivisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, y1)).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, y2)).toPoint();
    painter.drawLine(p1, p2);
  }

  // put division ticks
  painter.setPen(Qt::black);
  for(int i = 1; i <= m_divisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }
}

void te::qt::widgets::GraphicScaleFrame::drawBar2()
{
  QPoint p1, p2;
  QMatrix m = m_layoutEditor->getMatrixPaperViewToVp();
  QMatrix matrix(m.m11(), m.m12(), m.m21(), m.m22(), -m_frameRect.left()* m.m11(), -m_frameRect.bottom() * m.m22());
  QPainter painter(&m_pixmap);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(m_barColor);

  double subStep = m_stepmm / (double)m_subDivisions;

  // put subdivision vertical lines
  for(int i = 0; i < m_subDivisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put division vertical lines
  for(int i = 1; i <= m_divisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put subdivision horizontal lines
  for(int i = 0; i < m_subDivisions; ++i)
  {
    int ii = i + 1;
    if((i%2) == 0)
    {
      p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.bottom())).toPoint();
      p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)ii, m_barRect.bottom())).toPoint();
    }
    else
    {
      p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.top())).toPoint();
      p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)ii, m_barRect.top())).toPoint();
    }
    painter.drawLine(p1, p2);
  }

  // put division horizontal lines
  for(int i = 1; i < m_divisions; ++i)
  {
    int ii = i + 1;
    if((m_subDivisions % 2) == 0)
    {
      if((i%2))
      {
        p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
        p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)ii, m_barRect.bottom())).toPoint();
      }
      else
      {
        p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
        p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)ii, m_barRect.top())).toPoint();
      }
    }
    else
    {
      if((i%2) == 0)
      {
        p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
        p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)ii, m_barRect.bottom())).toPoint();
      }
      else
      {
        p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
        p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)ii, m_barRect.top())).toPoint();
      }
    }
    painter.drawLine(p1, p2);
  }
}

void te::qt::widgets::GraphicScaleFrame::drawBar3()
{
  QPoint p1, p2;
  QMatrix m = m_layoutEditor->getMatrixPaperViewToVp();
  QMatrix matrix(m.m11(), m.m12(), m.m21(), m.m22(), -m_frameRect.left()* m.m11(), -m_frameRect.bottom() * m.m22());
  QPainter painter(&m_pixmap);

  double subStep = m_stepmm / (double)m_subDivisions;

  // put sub bars
  painter.setBrush(m_barColor);
  painter.setPen(Qt::NoPen);
  QRectF raux = m_barRect;
  QRect r;

  for(int i = 0; i < m_subDivisions; i+=2)
  {
    raux.setLeft(m_barRect.left() + subStep * (double)i);
    raux.setWidth(subStep);
    r = matrix.mapRect(raux).toRect();
    painter.drawRect(r);
  }

  // put bars
  int ii = 1;
  if((m_subDivisions) % 2)
    ii = 2;
  for(int i = ii; i < m_divisions; i+=2)
  {
    raux.setLeft(m_barRect.left() + m_stepmm * (double)i);
    raux.setWidth(m_stepmm);
    r = matrix.mapRect(raux).toRect();
    painter.drawRect(r);
  }

  // put subdivision vertical lines
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::black);
  for(int i = 1; i < m_subDivisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put division vertical lines
  for(int i = 1; i < m_divisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put horizontall line
  p1 = matrix.map(QPointF(m_barRect.left(), m_barRect.center().y())).toPoint();
  p2 = matrix.map(QPointF(m_barRect.right(), m_barRect.center().y())).toPoint();
  painter.drawLine(p1, p2);

  // put bar rect
  r = matrix.mapRect(m_barRect).toRect();
  painter.drawRect(r);
}

void te::qt::widgets::GraphicScaleFrame::drawBar4()
{
  QPoint p1, p2;
  QMatrix m = m_layoutEditor->getMatrixPaperViewToVp();
  QMatrix matrix(m.m11(), m.m12(), m.m21(), m.m22(), -m_frameRect.left()* m.m11(), -m_frameRect.bottom() * m.m22());
  QPainter painter(&m_pixmap);

  double subStep = m_stepmm / (double)m_subDivisions;

  // put sub bars
  painter.setBrush(m_barColor);
  painter.setPen(Qt::NoPen);
  QRectF raux = m_barRect;
  QRect r;

  for(int i = 0; i < m_subDivisions; i++)
  {
    raux.setLeft(m_barRect.left() + subStep * (double)i);
    raux.setWidth(subStep);
    if((i % 2) == 0)
      raux.setTop(m_barRect.center().y());
    else
      raux.setTop(m_barRect.top());
    raux.setHeight(m_barRect.height() / 2.);

    r = matrix.mapRect(raux).toRect();
    painter.drawRect(r);
  }

  // put bars
  if((m_subDivisions % 2) == 0)
  {
    for(int i = 1; i < m_divisions; i++)
    {
      raux.setLeft(m_barRect.left() + m_stepmm * (double)i);
      raux.setWidth(m_stepmm);
      if((i % 2))
        raux.setTop(m_barRect.center().y());
      else
        raux.setTop(m_barRect.top());
      raux.setHeight(m_barRect.height() / 2.);

      r = matrix.mapRect(raux).toRect();
      painter.drawRect(r);
    }
  }
  else
  {
    for(int i = 1; i < m_divisions; i++)
    {
      raux.setLeft(m_barRect.left() + m_stepmm * (double)i);
      raux.setWidth(m_stepmm);
      if((i % 2) == 0)
        raux.setTop(m_barRect.center().y());
      else
        raux.setTop(m_barRect.top());
      raux.setHeight(m_barRect.height() / 2.);

      r = matrix.mapRect(raux).toRect();
      painter.drawRect(r);
    }
  }

  // put subdivision vertical lines
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::black);
  for(int i = 1; i < m_subDivisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + subStep * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put division vertical lines
  for(int i = 1; i < m_divisions; ++i)
  {
    p1 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.top())).toPoint();
    p2 = matrix.map(QPointF(m_barRect.left() + m_stepmm * (double)i, m_barRect.bottom())).toPoint();
    painter.drawLine(p1, p2);
  }

  // put horizontall line
  p1 = matrix.map(QPointF(m_barRect.left(), m_barRect.center().y())).toPoint();
  p2 = matrix.map(QPointF(m_barRect.right(), m_barRect.center().y())).toPoint();
  painter.drawLine(p1, p2);

  // put bar rect
  r = matrix.mapRect(m_barRect).toRect();
  painter.drawRect(r);
}

void te::qt::widgets::GraphicScaleFrame::adjust()
{
  // calcule o tamanho do widget
  QPointF center = m_frameRect.center();

  labelLengthCalculation();
  if(m_labelLengths.empty())
    return;

  double mmFrameHeight = m_labelHeight + m_labelHeight + 2 * m_space + m_barRect.height();
  double mmFrameWidth = m_labelLengths[0] / 2. + m_stepmm * (double)m_divisions + m_labelLengths[m_divisions-1] / 2.;

  double x = m_labelLengths[0] / 2.;
  double bw = mmFrameWidth - m_labelLengths[0] / 2. - m_labelLengths[m_divisions-1] / 2.;
  double bh = m_barRect.height();

  double y;
  if(m_labelAbove == false && m_unitAbove == false)
    y = m_labelHeight + m_space + m_labelHeight + m_space;
  else if(m_labelAbove == true && m_unitAbove == false)
    y = m_labelHeight + m_space;
  else if(m_labelAbove == false  && m_unitAbove == true)
    y = m_labelHeight + m_space;
  else if(m_labelAbove == true && m_unitAbove == true)
   y = 1; // nao coloque zero porque pode falhar na hora de desenhar (devido a erros de transformacao)

  m_frameRect = QRectF(0, 0, mmFrameWidth + 2, mmFrameHeight); // some 2mm no width para evitar o corte do ultimo label.
  m_frameRect.moveCenter(center);

  m_barRect = QRectF(x + m_frameRect.left(), y + m_frameRect.top(), bw, bh);

  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  QRect widgetRect = matrix.mapRect(m_frameRect).toRect();

  //colocar o widget sobre o map editor
  resize(widgetRect.size());
  move(widgetRect.left(), widgetRect.top());

  m_pixmap = QPixmap(widgetRect.width(), widgetRect.height());

  calculateSelectionPoints();
  draw();
}

void te::qt::widgets::GraphicScaleFrame::calculateSelectionPoints()
{
  QRect rec = m_pixmap.rect();

  double pw = rec.width();
  double ph = rec.height();
  int ipw = qRound(pw);
  int iph = qRound(ph);
  int ipw2 = qRound(pw/2.);
  int iph2 = qRound(ph/2.);
  double w = 9;
  double h = 9;

  if(pw < w*5)
    w = pw / 5.;
  if(ph < h*5)
    h = ph / 5.;

  int iw = qRound(w);
  int ih = qRound(h);
  int iw2 = qRound(w/2.);
  int ih2 = qRound(h/2.);
  m_recSel1 = QRect(0, 0, iw, ih);
  m_recSel2 = QRect(ipw2 - iw2, 0, iw, ih);
  m_recSel3 = QRect(ipw - iw, 0, iw, ih);
  m_recSel4 = QRect(ipw - iw, iph2 - ih2, iw, ih);
  m_recSel5 = QRect(ipw - iw, iph - ih, iw, ih);
  m_recSel6 = QRect(ipw2 - iw2, iph - ih, iw, ih);
  m_recSel7 = QRect(0, iph - ih, iw, ih);
  m_recSel8 = QRect(0, iph2 - ih2, iw, ih);
  //m_recSel9 = QRect(ipw2 - iw2, iph2 - ih2, iw, ih);
  m_recSel10 = QRect(0, 0, ipw, iph);
}
void te::qt::widgets::GraphicScaleFrame::labelLengthCalculation()
{
  setPointSize(m_pointSize);

  QPixmap pix(10, 10);
  QPainter painter(&pix);
  painter.setFont(m_font);

  QString s("0");
  QRectF rt = painter.boundingRect(QRect(0, 0, 100000, 100000), Qt::AlignLeft, s);
  m_barRect.setX(rt.width() / 2.);

  getLabels();

  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  m_labelLengths.clear();
  std::vector<QString>::iterator it;
  for(it = m_labels.begin(); it != m_labels.end(); ++it)
  {
    QString s = *it;
    rt = painter.boundingRect(QRect(0, 0, 100000, 100000), Qt::AlignLeft, s);
    rt = matrix.inverted().mapRect(rt);
    m_labelLengths.push_back(rt.width());
  }

  rt = painter.boundingRect(QRect(0, 0, 100000, 100000), Qt::AlignLeft, m_subDivisionString);
  rt = matrix.inverted().mapRect(rt);
  m_subDivisionLabelLength = rt.width();
  m_labelHeight = rt.height();
}

void te::qt::widgets::GraphicScaleFrame::getLabels()
{
  double conv = m_dataFrame->getDataUnitToMilimeter();
  m_toKm = conv / 1000000;
  if(conv == 0)
  {
    QMessageBox::warning (this, "Projection Error", "Parece que tem algo errado na projeção do dado");
    return;
  }

  // Meters, Kilometers, Miles, Leagues, Inches, Feet, Hand, Fingers, Nautic Miles, Nautic Fathom, Decimal Degrees, Yards, Fourlongs, Chains, Marathons, Unknow
  if(m_unit == "Kilometers")
    m_kmToUnit = 1;
  else if(m_unit == "Meters")
    m_kmToUnit = 1000;
  else if(m_unit == "Miles")
    m_kmToUnit = .6214;
  else if(m_unit == "Leagues")
    m_kmToUnit = .2071;
  else if(m_unit == "Inches")
    m_kmToUnit = 39370.0787;
  else if(m_unit == "Feet")
    m_kmToUnit = 3280.8399;
  else if(m_unit == "Hand")
    m_kmToUnit = 9842.5197;
  else if(m_unit == "Fingers")
    m_kmToUnit = 44994.3757;
  else if(m_unit == "Nautic Miles")
    m_kmToUnit = .5398;
  else if(m_unit == "Nautic Fathom")
    m_kmToUnit = 546.8066;
  else if(m_unit == "Decimal Degrees")
    m_kmToUnit = .009178;
  else if(m_unit == "Yards")
    m_kmToUnit = 1093.6133;
  else if(m_unit == "Fourlongs")
    m_kmToUnit = 4.971;
  else if(m_unit == "Chains")
    m_kmToUnit = 49.7097;
  else if(m_unit == "Marathons")
    m_kmToUnit = 0.0237;
  else // "Unknow"
    m_kmToUnit = 1000000/conv;

  double ww = m_dataFrame->getMapDisplay()->getExtent().getWidth();
  double widthUnit = ww * m_toKm * m_kmToUnit;

  if(m_step == 0)
    findNiceStep(.75 * widthUnit / m_divisions);

  // calcule step em mm
  double w = m_dataFrame->getFrameRect().width();
  m_stepmm = m_step * w / (ww * m_toKm * m_kmToUnit);

  std::string s;
  if(m_precision == 0)
  {
    if(((int)m_step)%2 == 1)
      s = te::common::Convert2String(m_step/2, 1);
    else
      s = te::common::Convert2String(m_step/2, 0);
  }
  else
  {
    s = te::common::Convert2String(m_step, m_precision);
    double v = atof(s.c_str());

    QString qs(s.c_str());
    QChar c = qs[qs.length()-1];
    if(c == '0' || c == '2' || c == '4' || c == '6' || c == '8')
      s = te::common::Convert2String(v/2, m_precision);
    else
      s = te::common::Convert2String(v/2, m_precision+1);
  }

  m_subDivisionString = s.c_str();

  m_labels.clear();
  if(m_offsetOrigin)
  {
    s = te::common::Convert2String(m_step, m_precision);
    QString qs = s.c_str();
    m_labels.push_back(qs);
    m_labels.push_back("0");
  }
  else
    m_labels.push_back("0");

  for(int i = 1; i <= m_divisions; ++i)
  {
    double v = m_step * i;
    s = te::common::Convert2String(v, m_precision);
    QString qs = s.c_str();

    m_labels.push_back(qs);
  }
}

void te::qt::widgets::GraphicScaleFrame::findNiceStep(double newStep)
{
  // se m_step == 0 use fineRound false e nao faca increase ou decrease
  bool fineRound = false;
  int increase = 0; // 0 nao faz nada; 1 increase; 2 decrease
  m_precision = 0;
  double fator;

  if(m_step != 0)
  {
    double fat = newStep / m_step;
    if(fat > .95 && fat < 1.05)
      fineRound = true;
    if(fat > 1)
      increase = 1;
    else
      increase = 2;
  }

  double vstep = newStep;

  // ache o primeiro algarismo entre 1 e 9
  double mv = vstep;
  double mult = 1;
  while(mv <= 1)
  {
    m_precision++;
    mult *= 10;
    mv = vstep * mult;
  }

  double dv = vstep;
  double div = 1;
  while(dv >= 10)
  {
    div *= 10;
    dv = vstep / div;
  }

  // pegue o número e o fator que depende se o step é maior ou menor que 1
  double v;
  if(mult == 1 && dv >= 1)
  {
    v = dv;
    fator = div;
  }
  else
  {
    v = mv;
    fator = 1 / mult;
  }

  if(fineRound == false) // faça ajuste grosseiro (gera número mais redondo)
  {
    vstep = qRound(v) * fator;
    if(increase == 1)
    {
      if(vstep <= m_step)
        vstep = qRound(v+1) * fator;
    }
    else if(increase == 2)
    {
      if(vstep >= m_step)
        vstep = qRound(v-1) * fator;
    }
  }
  else // faça ajuste refinado (gera número menos redondo)
  {
    vstep = qRound(v * 10) * fator / 10;
    double delta = fator / 10;
    if(increase == 1)
    {
      if(vstep <= m_step)
        vstep += delta;
    }
    else if(increase == 2)
    {
      if(vstep >= m_step)
        vstep -= delta;
    }

    if(div == 1 && mult == 1)
      m_precision++;

    if(mult > 1)
      m_precision++;
  }

  std::string st = te::common::Convert2String(vstep, m_precision);
  m_step = atof(st.c_str());
 }

void te::qt::widgets::GraphicScaleFrame::setPointSize(int pointSize)
{
  m_pointSize = pointSize;

  QMatrix matrix = m_layoutEditor->getMatrixPaperViewToVp();
  double h = m_pointSize * 25.4 / 72.;
  double vtam = h * matrix.m11();
  int ps = qRound(vtam);
  if(ps < 1)
    ps = 1;

  m_font.setPixelSize(ps);
}

QFont te::qt::widgets::GraphicScaleFrame::getFont()
{
  return m_font;
}

void te::qt::widgets::GraphicScaleFrame::setFont(const QFont& f)
{
  m_font = f;
}

QColor te::qt::widgets::GraphicScaleFrame::getLabelColor()
{
  return m_labelColor;
}

void te::qt::widgets::GraphicScaleFrame::setLabelColor(const QColor& c)
{
  m_labelColor = c;
}

QColor te::qt::widgets::GraphicScaleFrame::getBarColor()
{
  return m_barColor;
}

void te::qt::widgets::GraphicScaleFrame::setBarColor(const QColor& c)
{
  m_barColor = c;
}

QRectF te::qt::widgets::GraphicScaleFrame::getBarRect()
{
  return m_barRect;
}

void te::qt::widgets::GraphicScaleFrame::setBarRect(const QRectF& r)
{
  m_barRect = r;
}

int te::qt::widgets::GraphicScaleFrame::getType()
{
  return m_type;
}

void te::qt::widgets::GraphicScaleFrame::setType(int t)
{
  m_type = t;
}

bool te::qt::widgets::GraphicScaleFrame::isLabelAbove()
{
  return m_labelAbove;
}

void te::qt::widgets::GraphicScaleFrame::setLabelAbove(bool b)
{
  m_labelAbove = b;
}

bool te::qt::widgets::GraphicScaleFrame::isUnitAbove()
{
  return m_unitAbove;
}

void te::qt::widgets::GraphicScaleFrame::setUnitAbove(bool b)
{
  m_unitAbove = b;
}

double te::qt::widgets::GraphicScaleFrame::getSubTickSize()
{
  return m_subTickSize;
}

void te::qt::widgets::GraphicScaleFrame::setSubTickSize(double s)
{
  m_subTickSize = s;
}

bool te::qt::widgets::GraphicScaleFrame::isTickCentralized()
{
  return m_tickCentralized;
}

void te::qt::widgets::GraphicScaleFrame::setTickCentralized(bool b)
{
  m_tickCentralized = b;
}

double te::qt::widgets::GraphicScaleFrame::getStep()
{
  return m_step;
}

void te::qt::widgets::GraphicScaleFrame::setStep(double s)
{
  m_step = s;
}

int te::qt::widgets::GraphicScaleFrame::getDivisions()
{
  return m_divisions;
}

void te::qt::widgets::GraphicScaleFrame::setDivisions(int n)
{
  m_divisions = n;
}

int te::qt::widgets::GraphicScaleFrame::getSubDivisions()
{
  return m_subDivisions;
}

void te::qt::widgets::GraphicScaleFrame::setSubDivisions(int n)
{
  m_subDivisions = n;
}

double te::qt::widgets::GraphicScaleFrame::getSpace()
{
  return m_space;
}

void te::qt::widgets::GraphicScaleFrame::setSpace(double s)
{
  m_space = s;
}

QString te::qt::widgets::GraphicScaleFrame::getUnit()
{
  return m_unit;
}

void te::qt::widgets::GraphicScaleFrame::setUnit(const QString& s)
{
  m_unit = s;
}

void te::qt::widgets::GraphicScaleFrame::mousePressEvent(QMouseEvent* mouseEvent)
{
  if(mouseEvent->buttons() == Qt::LeftButton)
  {
    QPoint p = mouseEvent->pos();
    m_pressPoint = p;
    m_undo = false;
    m_copyAuxFrameRect = QRect();
    m_layoutEditor->raiseDraftLayoutEditor();
  }
  else if(mouseEvent->buttons() == Qt::RightButton)
  {
    QPoint gp = mouseEvent->globalPos();
    if(m_selected == 10)
    {
      QAction* action = m_menu->exec(gp);
      if(action == m_setTypeAction)
        setTypeAction();
      else if(action == m_setFamilyAction)
        setFamilyAction();
      else if(action == m_setLabelColorAction)
        setLabelColorAction();
      else if(action == m_setBarColorAction)
        setBarColorAction();
      else if(action == m_setBarHeightAction)
        setBarHeightAction();
      else if(action == m_offsetOriginAction)
        setOffsetOriginAction();
      else if(action == m_setLabelAboveAction)
        setLabelAboveAction();
      else if(action == m_setUnitAboveAction)
        setUnitAboveAction();
      else if(action == m_setUnitAction)
        setUnitAction();
      else if(action == m_setDivisionsAction)
        setDivisionsAction();
      else if(action == m_setSubDivisionsAction)
        setSubDivisionsAction();
      else if(action == m_setStepAction)
        setStepAction();
      else if(action == m_showTooTipAction)
        setShowTooTipAction();
    }
  }
}

void te::qt::widgets::GraphicScaleFrame::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  QPoint p = mouseEvent->pos();
  QPoint gp = mouseEvent->globalPos();

  if(mouseEvent->buttons() == Qt::NoButton)
  {
    setCursor();

    m_dragging = false;
    getSelectionPoint(p);
    if(m_selected == 0)
      return;
    else
    {
      showSelectionPoints();
      toolTip(gp, "Selection");
      if(m_selected == 10 && m_layoutEditor->getFrameSelected() != this)
        m_layoutEditor->setFrameSelected(this);
    }

    setCursor();
  }
  else if(mouseEvent->buttons() == Qt::LeftButton)
  {
    m_dragging = true;
    QPoint p = mouseEvent->pos();
    QPoint d = m_pressPoint - p;
    QMatrix matrix = m_layoutEditor->getMatrix();
    m_auxFrameRect = matrix.mapRect(m_frameRect).toRect();

    // mouse drag with left buttom
    if(m_selected != 0)
    {
      if(m_undo == false)
      {
        m_layoutEditor->insertCopy2Undo(m_dataFrame);
        m_undo = true;
      }

      if(m_selected == 4) // resize right
        m_auxFrameRect = QRect(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width() - d.x(), m_auxFrameRect.height());
      else if(m_selected == 8) // resize left
        m_auxFrameRect = QRect(m_auxFrameRect.left() - d.x(), m_auxFrameRect.top(), m_auxFrameRect.width() + d.x(), m_auxFrameRect.height());
      else // move data frame
        m_auxFrameRect.moveCenter(m_auxFrameRect.center() - d);

      rubberBand();
      setCursor();
    }
  }
}

void te::qt::widgets::GraphicScaleFrame::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
  if(mouseEvent->button() == Qt::LeftButton)
  {
    QPoint p = mouseEvent->pos();

    if(m_pressPoint != p && m_selected != 0)
    {
      setCursor();

      QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
      pixmap->fill(Qt::transparent);
      QMatrix matrix = m_layoutEditor->getMatrix();
      QRectF r(m_auxFrameRect.left(), m_auxFrameRect.top(), m_auxFrameRect.width(), m_auxFrameRect.height());
      double w = m_frameRect.width();
      m_frameRect = matrix.inverted().mapRect(r);
      double nw = m_frameRect.width();
      m_layoutEditor->lowerDraftLayoutEditor();

      if(m_selected == 4 || m_selected == 8)
      {
        bool fineRound = false;
        double fat = nw / w;
        if(fat > .95 && fat < 1.05)
          fineRound = true;

        double newStep = m_step * fat;
        findNiceStep(newStep);
      }

      adjust();
      m_layoutEditor->draw();

      if(m_selected != 10)
      {
        QPoint c = getCenterSelected();
        cursor().setPos(mapToGlobal(c));
        QMouseEvent e1(QEvent::MouseButtonPress, c, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &e1);
      }
      showSelectionPoints();
    }
  }
}

void te::qt::widgets::GraphicScaleFrame::enterEvent(QEvent*)
{
  if(QApplication::overrideCursor() || m_dragging)
    return;

  m_layoutEditor->setFrameSelected(this);
  raise();
  m_selected = 0;
}

void te::qt::widgets::GraphicScaleFrame::leaveEvent(QEvent*)
{
  if(m_dragging)
  {
    if(m_selected != 0)
      raise();
    return;
  }

  m_selected = 0;
  m_layoutEditor->setFrameSelected(0);
  lower();
  setCursor();
}

void te::qt::widgets::GraphicScaleFrame::rubberBand()
{
  // faz tudo em coordenadas do dispositivo

  QPixmap* pixmap = m_layoutEditor->getDraftPixmap();
  QPainter painter(pixmap);
  painter.translate(m_layoutEditor->getVerticalRulerWidth(), m_layoutEditor->getHorizontalRulerWidth());
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::red);

  // erase old 
  if(m_copyAuxFrameRect.isValid())
  {
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.drawRect(m_copyAuxFrameRect);
  }

  // show new
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  painter.drawRect(m_auxFrameRect);
  painter.end();

  m_copyAuxFrameRect = m_auxFrameRect;
          
  m_layoutEditor->update();
}

void te::qt::widgets::GraphicScaleFrame::showSelectionPoints()
{
  // faz tudo em coordenadas do dispositivo

  m_pixmap.fill(Qt::transparent);
  draw();

  QPainter painter(&m_pixmap);

  painter.setPen(Qt::blue);
  painter.setBrush(QColor(0, 0, 255, 24));
  painter.drawRect(QRect(m_recSel10.left(), m_recSel10.top(), m_recSel10.width()-1, m_recSel10.height()-1));  

  // draw selection points
  painter.setBrush(QColor(0, 0, 255, 150));
  painter.drawRect(m_recSel4);
  painter.drawRect(m_recSel8);
  painter.end();
  update();
}

void te::qt::widgets::GraphicScaleFrame::hideSelectionPoints()
{
  m_pixmap.fill(Qt::transparent);
  draw();
  update();
}

void te::qt::widgets::GraphicScaleFrame::getSelectionPoint(const QPoint& p)
{
  // faz tudo em coordenadas do dispositivo

  m_selected = 0;

  if(m_recSel4.contains(p, Qt::OddEvenFill))
    m_selected = 4;
  else if(m_recSel8.contains(p, Qt::OddEvenFill))
    m_selected = 8;
  else if(m_recSel10.contains(p, Qt::OddEvenFill))
    m_selected = 10;
}

QPoint te::qt::widgets::GraphicScaleFrame::getCenterSelected()
{
  // faz tudo em coordenadas do dispositivo

  QPoint c;

  if(m_selected == 4)
    c = m_recSel4.center();
  else if(m_selected == 8)
    c = m_recSel8.center();
  else if(m_selected == 10)
    c = m_recSel10.center();

  return c;
}

void te::qt::widgets::GraphicScaleFrame::setCursor()
{
  QCursor cursor;

  if(m_selected == 10)
    cursor.setShape(Qt::SizeAllCursor);
  else if(m_selected == 4 || m_selected == 8)
    cursor.setShape(Qt::SizeHorCursor);

  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() != cursor.shape())
    {
      while(QApplication::overrideCursor())
        QApplication::restoreOverrideCursor();
      if(cursor.shape() != Qt::ArrowCursor)
        QApplication::setOverrideCursor(cursor);
    }
  }
  else if(cursor.shape() != Qt::ArrowCursor)
    QApplication::setOverrideCursor(cursor);
}


void te::qt::widgets::GraphicScaleFrame::toolTip(const QPoint& p, const QString& type)
{
  if(m_showToolTip == false)
  {
    QToolTip::hideText();
    return;
  }
 
  if(type == "Selection")
  {
    if(m_selected == 4 || m_selected == 8)
      QToolTip::showText(p, "Drag to Change Step", this);
    else if(m_selected == 10)
      QToolTip::showText(p, "Drag to Move", this);
    else
      QToolTip::hideText();
  }
}

void te::qt::widgets::GraphicScaleFrame::setTypeAction()
{
  bool ok;
  int i = QInputDialog::getInt(this, tr("Configure Type"),
                              tr("Graphic Scale Type:"), m_type, 1, 4, 1, &ok);
  if(ok)
  {
    m_type = i;
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setFamilyAction()
{
  bool ok;
  QFont f = m_font;
  f.setPointSize(m_pointSize);

  QFont font = QFontDialog::getFont(&ok, f, this);
  if(ok)
  {
    m_font = font;
    m_pointSize = font.pointSize();
    setPointSize(m_pointSize);
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setLabelColorAction()
{
  QColor cor = QColorDialog::getColor(m_labelColor, this, "Label Color");
  if(cor.isValid())
  {
    m_labelColor = cor;
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setBarColorAction()
{
  QColor cor = QColorDialog::getColor(m_labelColor, this, "Label Color");
  if(cor.isValid())
  {
    m_barColor = cor;
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setBarHeightAction()
{
  bool ok;
  int i = QInputDialog::getInt(this, tr("Configure Bar Height"),
                              tr("Bar Height:"), qRound(m_barRect.height()), 2, 20, 1, &ok);
  if(ok)
  {
    m_barRect.setHeight(i);
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setOffsetOriginAction()
{
  if(m_offsetOriginAction->isChecked() != m_offsetOrigin)
  {
    m_offsetOrigin = m_offsetOriginAction->isChecked();
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setLabelAboveAction()
{
  if(m_setLabelAboveAction->isChecked() != m_labelAbove)
  {
    m_labelAbove = m_setLabelAboveAction->isChecked();
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setUnitAboveAction()
{
  if(m_setUnitAboveAction->isChecked() != m_unitAbove)
  {
    m_unitAbove = m_setUnitAboveAction->isChecked();
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setUnitAction()
{
  QStringList items;
  items << tr("Kilometers") << tr("Meters") << tr("Decimal Degrees") << tr("Miles") << \
    tr("Leagues") << tr("Inches") << tr("Feet") << tr("Hand") << tr("Fingers") << \
    tr("Nautic Miles") << tr("Nautic Fathom") << tr("Yards") << tr("Fourlongs") << \
    tr("Chains") << tr("Marathons") << tr("Unknow");

  int current;
  for(current = 0; current < items.size(); current++)
  {
    if(m_unit == items[current])
      break;
  }

  bool ok;
  QString item = QInputDialog::getItem(this, tr("Set Unit"),
                                          tr("Unit:"), items, current, false, &ok);

  if(ok && item.isEmpty() == false && item != m_unit)
  {
    setUnit(item);
    m_step = 0; // calcula de novo o step
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setDivisionsAction()
{
  bool ok;
  int i = QInputDialog::getInt(this, tr("Set Number Of Divisions"),
                              tr("Number Of Divisions:"), m_divisions, 2, 10, 1, &ok);
  if(ok)
  {
    m_divisions = i;

  // na interface pode ter uma opção para recalcular o step (isto mantem o tamanho horizontal da escala)
    m_step = 0; // para recalcular o step - retirar esta linha caso cntrario
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setSubDivisionsAction()
{
  bool ok;
  int i = QInputDialog::getInt(this, tr("Set Number Of Subdivisions"),
                              tr("Number Of Subdivisions:"), m_subDivisions, 2, 10, 1, &ok);
  if(ok)
  {
    m_subDivisions = i;
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setStepAction()
{
  bool ok;
  double v = QInputDialog::getDouble(this, tr("Set Step Distance"),
                              tr("Step Distance:"), m_step, 0, 2147483647, 1, &ok);
  if(ok)
  {
    m_step = v;
    adjust();
    update();
  }
}

void te::qt::widgets::GraphicScaleFrame::setShowTooTipAction()
{
  if(m_showTooTipAction->isChecked() != m_showToolTip)
  {
    m_showToolTip = m_setUnitAboveAction->isChecked();
    showToolTip(m_showToolTip);
  }
}

