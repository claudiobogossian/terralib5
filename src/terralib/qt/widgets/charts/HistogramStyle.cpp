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
  \file terralib/qt/widgets/charts/HistogramStyle.h

  \brief A class used to define the style of a histogram's chart
*/

//Terralib
#include "HistogramStyle.h";
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"

//QT
#include "qcolor.h"
#include "qpalette.h"

//QWT
#include "qwt_column_symbol.h"

te::qt::widgets::HistogramStyle::HistogramStyle()
{
  m_fill = new te::se::Fill();
  m_stroke = new te::se::Stroke();
  m_symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
  m_color.fromRgb(100, 100, 100);
  m_symbol->setPalette(m_color);
}

te::qt::widgets::HistogramStyle::HistogramStyle(te::se::Fill* fill, te::se::Stroke* stroke, QwtColumnSymbol* symbol, QColor color)
: m_fill(fill),
  m_stroke(stroke),
  m_symbol(symbol),
  m_color(color)
{
}

te::qt::widgets::HistogramStyle::~HistogramStyle()
{
}

te::se::Fill* te::qt::widgets::HistogramStyle::getFill()
{
  return m_fill;
}

void te::qt::widgets::HistogramStyle::setFill(te::se::Fill* newFill)
{
  m_fill = newFill;
}

te::se::Stroke* te::qt::widgets::HistogramStyle::getStroke()
{
  return m_stroke;
}

void te::qt::widgets::HistogramStyle::setStroke(te::se::Stroke* newStroke)
{
  m_stroke = newStroke;
}

QwtColumnSymbol* te::qt::widgets::HistogramStyle::getSymbol()
{
  return m_symbol;
}

void te::qt::widgets::HistogramStyle::setSymbol(QwtColumnSymbol* newSymbol)
{
  m_symbol = newSymbol;
}


QColor& te::qt::widgets::HistogramStyle::getColor()
{
  return m_color;
}

void te::qt::widgets::HistogramStyle::setColor(QColor newColor)
{
  m_color = newColor;
  adjustSymbol();
}

void te::qt::widgets::HistogramStyle::adjustSymbol()
{
  m_symbol->setPalette(m_color);
}

