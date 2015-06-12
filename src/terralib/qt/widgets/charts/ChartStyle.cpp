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
  \file terralib/qt/widgets/charts/ChartStyle.cpp

  \brief A class used to define a chart's style
*/

//Terralib
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "ChartStyle.h"

//QT
#include "qcolor.h"

te::qt::widgets::ChartStyle::ChartStyle()
{
  m_title = "Chart";
  m_titleFont = QFont("Arial", 16, QFont::Bold);
  m_axisX = "Property X";
  m_axisY = "Property Y";
  m_axisFont = QFont("Arial", 14);
  m_fill = new te::se::Fill();
  m_stroke = new te::se::Stroke();
  m_gridChecked = false;
  m_backColor = QColor( Qt::white);
}

te::qt::widgets::ChartStyle::ChartStyle(QString title, QFont titleFont, QString axisX, QString axisY, QFont axisFont, te::se::Fill* fill, te::se::Stroke* stroke, bool gridChecked,  QColor color)
:m_title(title),
  m_titleFont(titleFont),
  m_axisX(axisX),
  m_axisY(axisY),
  m_axisFont(axisFont),
  m_gridChecked(gridChecked),
  m_fill(fill),
  m_stroke(stroke),
  m_backColor(color)
{
}

te::qt::widgets::ChartStyle::~ChartStyle()
{
  delete m_fill;
  delete m_stroke;
}

QString& te::qt::widgets::ChartStyle::getTitle()
{
  return m_title;
}

void te::qt::widgets::ChartStyle::setTitle(QString newTitle)
{
  m_title = newTitle;
}

QFont& te::qt::widgets::ChartStyle::getTitleFont()
{
  return m_titleFont;
}

void te::qt::widgets::ChartStyle::setTitleFont(QFont newTitleFont)
{
  m_titleFont = newTitleFont;
}

QString& te::qt::widgets::ChartStyle::getAxisX()
{
  return m_axisX;
}

void te::qt::widgets::ChartStyle::setAxisX(QString newAxisX)
{
  m_axisX = newAxisX;
}

QString& te::qt::widgets::ChartStyle::getAxisY()
{
  return m_axisY;
}

void te::qt::widgets::ChartStyle::setAxisY(QString newAxisY)
{
  m_axisY = newAxisY;
}

QFont& te::qt::widgets::ChartStyle::getAxisFont()
{
  return m_axisFont;
}

void te::qt::widgets::ChartStyle::setAxisFont(QFont newAxisFont)
{
  m_axisFont = newAxisFont;
}


te::se::Fill* te::qt::widgets::ChartStyle::getFill()
{
  return m_fill;
}


void te::qt::widgets::ChartStyle::setFill(te::se::Fill* newFill)
{
  delete m_fill;
  m_fill = newFill;
}

te::se::Stroke* te::qt::widgets::ChartStyle::getStroke()
{
  return m_stroke;
}

void te::qt::widgets::ChartStyle::setStroke(te::se::Stroke* newStroke)
{
  delete m_stroke;
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
