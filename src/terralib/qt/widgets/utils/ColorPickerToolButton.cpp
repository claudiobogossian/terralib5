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
  \file terralib/qt/widgets/utils/ColorPickerToolButton.cpp

  \brief Custom widget used to pick a color.
*/

// TerraLib
#include "ColorPickerToolButton.h"

te::qt::widgets::ColorPickerToolButton::ColorPickerToolButton(QWidget* parent)
  : QToolButton(parent),
    m_popup(new ColorPickerPopup),
    m_isOpen(false)
{
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setPopupMode(QToolButton::MenuButtonPopup);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  setText(tr("Color"));
  setIconSize(QSize(16, 16));
  setColor(Qt::black);
  
  // Signals & slots
  connect(m_popup, SIGNAL(selected(const QColor&)), this, SLOT(onPopupSelected(const QColor&)));
}

te::qt::widgets::ColorPickerToolButton::~ColorPickerToolButton()
{
}

void te::qt::widgets::ColorPickerToolButton::setColor(const QColor& color)
{
  m_popup->setSelected(color);
  m_selectedColor = color;
  updateIcon();
}

QColor te::qt::widgets::ColorPickerToolButton::getColor() const
{
  return m_selectedColor;
}

void te::qt::widgets::ColorPickerToolButton::resizeEvent(QResizeEvent* e)
{
  QToolButton::resizeEvent(e);
  updateIcon();
}

void te::qt::widgets::ColorPickerToolButton::mousePressEvent(QMouseEvent* /*e*/)
{
  if (!m_isOpen)
  {
    m_popup->move(mapToGlobal(QPoint(0, height())));
    m_popup->show();
    m_isOpen = true;
  }
  else
  {
    m_popup->close();
    m_isOpen = false;
  }
}

void te::qt::widgets::ColorPickerToolButton::updateIcon()
{
  QPixmap pix(iconSize());
  pix.fill(m_selectedColor);
  QPainter p(&pix);
  p.setPen(QPen(Qt::darkGray, 2));
  p.drawRect(pix.rect());
  setIcon(pix);
}

void te::qt::widgets::ColorPickerToolButton::onPopupSelected(const QColor& color)
{
  setColor(color);
  emit colorChanged(color);
  m_isOpen = false;
}
