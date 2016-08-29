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
  \file terralib/qt/widgets/se/AbstractGraphicWidget.cpp

  \brief Abstract class that represents a widget that can be used to build a graphic element.
*/

// TerraLib
#include "../../../se/AnchorPoint.h"
#include "../../../se/Displacement.h"
#include "../../../se/ParameterValue.h"
#include "../../../se/Graphic.h"
#include "AbstractGraphicWidget.h"

te::qt::widgets::AbstractGraphicWidget::AbstractGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_graphic(new te::se::Graphic)
{
}

te::qt::widgets::AbstractGraphicWidget::~AbstractGraphicWidget()
{
  delete m_graphic;
}

te::se::Graphic* te::qt::widgets::AbstractGraphicWidget::getGraphic() const
{
  return m_graphic->clone();
}

void te::qt::widgets::AbstractGraphicWidget::setGraphicSize(const QString& size)
{
  m_graphic->setSize(new te::se::ParameterValue(size.toUtf8().data()));
  emit graphicChanged();
}

void te::qt::widgets::AbstractGraphicWidget::setGraphicAngle(const QString& angle)
{
  m_graphic->setRotation(new te::se::ParameterValue(angle.toUtf8().data()));
  emit graphicChanged();
}

void te::qt::widgets::AbstractGraphicWidget::setGraphicOpacity(int value)
{
  double opacity = value / 100.0;
  m_graphic->setOpacity(new te::se::ParameterValue(QString::number(opacity, 'g', 2).toUtf8().data()));
  emit graphicChanged();
}

void te::qt::widgets::AbstractGraphicWidget::setGraphicDisplacement(const QString& dx, const QString& dy)
{
  te::se::Displacement* disp = new te::se::Displacement;
  disp->setDisplacementX(new te::se::ParameterValue(dx.toUtf8().data()));
  disp->setDisplacementY(new te::se::ParameterValue(dy.toUtf8().data()));
  m_graphic->setDisplacement(disp);
  emit graphicChanged();
}

void te::qt::widgets::AbstractGraphicWidget::setGraphicAnchorPoint(const QString& ax, const QString& ay)
{
  te::se::AnchorPoint* ac = new te::se::AnchorPoint;
  ac->setAnchorPointX(new te::se::ParameterValue(ax.toUtf8().data()));
  ac->setAnchorPointY(new te::se::ParameterValue(ay.toUtf8().data()));
  m_graphic->setAnchorPoint(ac);
  emit graphicChanged();
}
