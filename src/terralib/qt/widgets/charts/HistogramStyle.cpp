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
  \file terralib/qt/widgets/charts/HistogramStyle.cpp

  \brief A class used to define the style of a histogram's chart
*/

//Terralib
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "HistogramStyle.h"

te::qt::widgets::HistogramStyle::HistogramStyle()
{
  m_fill = new te::se::Fill();
  m_stroke = new te::se::Stroke();
}

te::qt::widgets::HistogramStyle::HistogramStyle(te::se::Fill* fill, te::se::Stroke* stroke)
: m_fill(fill),
  m_stroke(stroke)
{
}

te::qt::widgets::HistogramStyle::~HistogramStyle()
{
  delete m_fill;
  delete m_stroke;
}

te::qt::widgets::HistogramStyle* te::qt::widgets::HistogramStyle::clone()
{
  te::qt::widgets::HistogramStyle* style = new te::qt::widgets::HistogramStyle();
  style->setFill(this->getFill()->clone());
  style->setStroke(this->getStroke()->clone());
  return style;
}

te::se::Fill* te::qt::widgets::HistogramStyle::getFill()
{
  return m_fill;
}

void te::qt::widgets::HistogramStyle::setFill(te::se::Fill* newFill)
{
  delete m_fill;
  m_fill = newFill;
}

te::se::Stroke* te::qt::widgets::HistogramStyle::getStroke()
{
  return m_stroke;
}

void te::qt::widgets::HistogramStyle::setStroke(te::se::Stroke* newStroke)
{
  delete m_stroke;
  m_stroke = newStroke;
}

