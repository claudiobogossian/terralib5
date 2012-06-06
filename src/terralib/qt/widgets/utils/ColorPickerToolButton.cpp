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
  \file terralib/qt/widgets/utils/ColorPickerToolButton.cpp

  \brief Custom widget used to pick a color.
*/

// TerraLib
#include "ColorPickerToolButton.h"

te::qt::widgets::ColorPickerToolButton::ColorPickerToolButton(QWidget* parent) : QToolButton(parent)
{
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setPopupMode(QToolButton::MenuButtonPopup);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  setText(tr("Color"));

  m_popup = new ColorPickerPopup;
  m_popup->setObjectName("popup");
  m_popup->setFrameShape(QFrame::StyledPanel);
  m_popup->setFrameShadow(QFrame::Plain);

  setIconSize(QSize(16, 16));

  connect(m_popup, SIGNAL(selected(QColor)), this, SLOT(onPopupSelected(QColor)));

  m_selectedColor = Qt::black;
}

void te::qt::widgets::ColorPickerToolButton::setColor(const QColor& color)
{
  m_selectedColor = color;
  updateIcon();
}

void te::qt::widgets::ColorPickerToolButton::mouseReleaseEvent(QMouseEvent* /*e*/)
{
  m_popup->move(mapToGlobal(QPoint(0, height())));
  m_popup->show();
}

void te::qt::widgets::ColorPickerToolButton::onPopupSelected(const QColor& color)
{
  setColor(color);

  emit colorChanged();
}

void te::qt::widgets::ColorPickerToolButton::resizeEvent(QResizeEvent* e)
{
  QToolButton::resizeEvent(e);
  //setIconSize(QSize(e->size().width() - 20, e->size().height() - 6));
  updateIcon();
}

void te::qt::widgets::ColorPickerToolButton::updateIcon()
{
  QPixmap pix(iconSize());
  pix.fill(m_selectedColor);
  setIcon(pix);
}
