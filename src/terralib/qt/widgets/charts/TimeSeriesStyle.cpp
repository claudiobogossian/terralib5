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
  \file terralib/qt/widgets/charts/TimeSeriesStyle.cpp

  \brief A class used to define the style of a timeSeries' chart
*/

//Terralib
#include "../../../se/Stroke.h"
#include "TimeSeriesStyle.h"

te::qt::widgets::TimeSeriesStyle::TimeSeriesStyle()
{
  m_stroke = new te::se::Stroke();
}

te::qt::widgets::TimeSeriesStyle::TimeSeriesStyle( te::se::Stroke* stroke)
: m_stroke(stroke)
{
}

te::qt::widgets::TimeSeriesStyle::~TimeSeriesStyle()
{
  delete m_stroke;
}

te::qt::widgets::TimeSeriesStyle* te::qt::widgets::TimeSeriesStyle::clone()
{
  te::qt::widgets::TimeSeriesStyle* style = new te::qt::widgets::TimeSeriesStyle();
  style->setStroke(this->getStroke()->clone());
  return style;
}

te::se::Stroke* te::qt::widgets::TimeSeriesStyle::getStroke()
{
  return m_stroke;
}

void te::qt::widgets::TimeSeriesStyle::setStroke(te::se::Stroke* newStroke)
{
  delete m_stroke;
  m_stroke = newStroke;
}

