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
#include "Utils.h"

te::qt::widgets::ScatterChart::ScatterChart(Scatter* data) :
  QwtPlotCurve(),
  m_scatter(data)
{
  //Set style
  setStyle(QwtPlotCurve::NoCurve);

  //Build a default Mark
   m_symbol = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    setSymbol( m_symbol );

  //Set Values
  setData();
}


te::qt::widgets::ScatterChart::ScatterChart(Scatter* data, QwtSymbol* symbol, size_t size) :
  QwtPlotCurve(),
  m_scatter(data),
  m_symbol(symbol),
  m_size(size)
{

  //Set Symbol
  setSymbol(m_symbol);

  //Set Values
  setData();
}

void te::qt::widgets::ScatterChart::setData()
{
  QPolygonF samples;

  for (int i = 0; i < m_scatter->size(); i++)
  {
    samples += QPointF( m_scatter->getX(i), m_scatter->getY(i));
  
  }

  setSamples( samples );
}

te::qt::widgets::ScatterChart::~ScatterChart()
{  
  delete m_scatter;  
}
