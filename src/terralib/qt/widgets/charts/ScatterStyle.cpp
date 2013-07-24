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
  \file terralib/qt/widgets/charts/scatterStyle.h

  \brief A class used to define the style of a scatter's chart
*/

//TerraLib
#include "../../../se/Fill.h"
#include "../../../se/Graphic.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include"MarkSymbol.h"
#include "ScatterStyle.h"
#include "Utils.h"

te::qt::widgets::ScatterStyle::ScatterStyle ()
{
  m_graphic  = new te::se::Graphic();
  m_mark = new te::se::Mark();
  m_mark->setFill(new te::se::Fill());
  m_mark->setStroke(new te::se::Stroke());
  m_graphic->add(m_mark);
}

te::qt::widgets::ScatterStyle::~ScatterStyle()
{
  delete m_graphic;
}

te::qt::widgets::ScatterStyle* te::qt::widgets::ScatterStyle::clone()
{
  te::qt::widgets::ScatterStyle* style = new te::qt::widgets::ScatterStyle();
  style->setGraphic(this->getGraphic()->clone());
  return style;
}

te::se::Graphic* te::qt::widgets::ScatterStyle::getGraphic()
{
  return m_graphic;
}

void te::qt::widgets::ScatterStyle::setGraphic(te::se::Graphic* newGraphic)
{
  m_graphic = newGraphic;
  m_mark = m_graphic->getMarks()[0];
}

QwtSymbol* te::qt::widgets::ScatterStyle::getSymbol()
{
  return te::qt::widgets::Terralib2Qwt(m_mark); 
}
