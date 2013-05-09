/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/ChartStyle.cpp

  \brief A class used to define a chart's style
*/

//Terralib
#include "ChartStyle.h"
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"

//QT
#include "qcolor.h"

te::qt::widgets::ChartStyle::ChartStyle()
{
  m_title = "";
  m_fill = new te::se::Fill();
  m_stroke = new te::se::Stroke();
  m_gridChecked = false;
}

te::qt::widgets::ChartStyle::ChartStyle(QString title, te::se::Fill* fill, te::se::Stroke* stroke, bool gridChecked,  QColor color)
:m_title(title),
  m_gridChecked(gridChecked),
  m_fill(fill),
  m_stroke(stroke),
  m_backColor(color)
{
}

te::qt::widgets::ChartStyle::~ChartStyle()
{
}


QString& te::qt::widgets::ChartStyle::getTitle()
{
  return m_title;
}

void te::qt::widgets::ChartStyle::setTitle(QString newTitle)
{
  m_title = newTitle;
}

te::se::Fill* te::qt::widgets::ChartStyle::getFill()
{
  return m_fill;
}

void te::qt::widgets::ChartStyle::setFill(te::se::Fill* newFill)
{
  m_fill = newFill;
}

te::se::Stroke* te::qt::widgets::ChartStyle::getStroke()
{
  return m_stroke;
}

void te::qt::widgets::ChartStyle::setStroke(te::se::Stroke* newStroke)
{
  m_stroke = newStroke;
}

QColor& te::qt::widgets::ChartStyle::getColor()
{
  return m_backColor;
}

void te::qt::widgets::ChartStyle::setColor(QColor newColor)
{
  m_backColor = newColor;
}

bool te::qt::widgets::ChartStyle::getGridChecked()
{
  return m_gridChecked;
}

void te::qt::widgets::ChartStyle::setGridChecked(bool newGridChecked)
{
  m_gridChecked = newGridChecked;
}
