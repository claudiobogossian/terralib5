/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/CharMapWidget.cpp

  \brief A widget used to show the set of characters of a specified font.
*/

// TerraLib
#include "CharMapWidget.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

te::qt::widgets::CharMapWidget::CharMapWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_currentChar(0),
    m_columns(11),
    m_squareSize(24)
{
  m_font.setPointSize(12);
  setMouseTracking(true);
}

te::qt::widgets::CharMapWidget::~CharMapWidget()
{
}

QSize te::qt::widgets::CharMapWidget::sizeHint() const
{
  return QSize(m_columns * m_squareSize, (65536 / m_columns) * m_squareSize);
}

int te::qt::widgets::CharMapWidget::getCurrentChar() const
{
  return m_currentChar;
}

void te::qt::widgets::CharMapWidget::updateFont(const QFont& font)
{
  m_font.setFamily(font.family());
  update();
}

void te::qt::widgets::CharMapWidget::mouseMoveEvent(QMouseEvent* e)
{
  QPoint widgetPosition = mapFromGlobal(e->globalPos());
  uint key = (widgetPosition.y() / m_squareSize) * m_columns + widgetPosition.x() / m_squareSize;

  QString text = QString::fromLatin1("<p>Char: <span style=\"font-size: 28pt; font-family: %1\">").arg(m_font.family())
                + QChar(key)
                + QString::fromLatin1("</span><p>Value: 0x")
                + QString::number(key, 16);

  QToolTip::showText(e->globalPos(), text, this);
}

void te::qt::widgets::CharMapWidget::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
  {
    QWidget::mousePressEvent(e);
    return;
  }

  m_currentChar = (e->y() / m_squareSize) * m_columns + e->x() / m_squareSize;
#if (QT_VERSION >= 0x050000)
  if(QChar(m_currentChar).category() != QChar::Other_NotAssigned)
#else
  if(QChar(m_currentChar).category() != QChar::NoCategory)
#endif
    emit charSelected(m_currentChar);

  update();
}

void te::qt::widgets::CharMapWidget::paintEvent(QPaintEvent* e)
{
  QPainter painter(this);
  painter.fillRect(e->rect(), Qt::white);
  painter.setFont(m_font);

  QRect redrawRect = e->rect();
  int beginRow = redrawRect.top() / m_squareSize;
  int endRow = redrawRect.bottom() / m_squareSize;
  int beginColumn = redrawRect.left() / m_squareSize;
  int endColumn = redrawRect.right() / m_squareSize;

  painter.setPen(Qt::gray);
  for(int row = beginRow; row <= endRow; ++row)
    for(int column = beginColumn; column <= endColumn; ++column)
      painter.drawRect(column * m_squareSize, row * m_squareSize, m_squareSize, m_squareSize);

  painter.setPen(Qt::black);
  QFontMetrics fontMetrics(m_font);
  for(int row = beginRow; row <= endRow; ++row)
  {
    for(int column = beginColumn; column <= endColumn; ++column)
    {
      int key = row * m_columns + column;
      painter.setClipRect(column * m_squareSize, row * m_squareSize, m_squareSize, m_squareSize);

      if(key == m_currentChar)
        painter.fillRect(column * m_squareSize + 1, row * m_squareSize + 1, m_squareSize, m_squareSize, Qt::lightGray);

      painter.drawText(column * m_squareSize + (m_squareSize / 2) - fontMetrics.width(QChar(key))/2,
                       row * m_squareSize + 4 + fontMetrics.ascent(),
                       QString(QChar(key)));
    }
  }
}
