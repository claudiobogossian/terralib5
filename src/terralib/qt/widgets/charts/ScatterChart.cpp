/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ScatterChart.cpp

  \brief A class to represent a scatter chart.
*/

// TerraLib
#include "../../../se/Mark.h"
#include "../../../se/Utils.h"

//QWT
#include <qwt_plot_curve.h>

#include "ScatterChart.h"
#include "Scatter.h"
#include "MarkSymbol.h"
#include "Utils.h"


te::qt::widgets::ScatterChart::ScatterChart(Scatter* data) :
  QwtPlotCurve(),
  m_scatter(data)
{
  //Set style
  setStyle(QwtPlotCurve::NoCurve);

  //Build a default Mark
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "1"); //black e espessura 1
  te::se::Fill* fill = te::se::CreateFill("#000000", "1.0");  //black 100% opaco
  m_mark = te::se::CreateMark("triangle", stroke, fill);
  m_size = 10;

  //Set Symbol
  MarkSymbol* chartSymbol = new MarkSymbol(m_mark, m_size);
  setSymbol(chartSymbol);

  //Set Values
  setValues();
}


te::qt::widgets::ScatterChart::ScatterChart(Scatter* data, te::se::Mark* style, size_t size) :
  QwtPlotCurve(),
  m_scatter(data),
  m_mark(style),
  m_size(size)
{
  //Set style
  setStyle(QwtPlotCurve::NoCurve);

  //Set Symbol
  MarkSymbol* chartSymbol = new MarkSymbol(m_mark, m_size);
  setSymbol(chartSymbol);

  //Set Values
  setValues();
}

void te::qt::widgets::ScatterChart::setValues()
{
  std::size_t size = m_scatter->size();
  if(size == 0)
  {
    setRawSamples(0, 0, size);
    return;
  }

  setRawSamples(m_scatter->getX(), m_scatter->getY(), size);
}

te::qt::widgets::ScatterChart::~ScatterChart()
{  
  delete m_mark;  
}

