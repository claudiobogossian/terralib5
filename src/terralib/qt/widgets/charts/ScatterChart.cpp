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
  \file ScatterChart.cpp

  \brief A class to represent a scatter chart.
*/

// TerraLib

#include "Enums.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../se/Mark.h"
#include "../../../se/Graphic.h"
#include "ScatterChart.h"
#include "Scatter.h"
#include "ScatterStyle.h"
#include "../../../se/Utils.h"
#include "Utils.h"

//QWT
#include <qwt_symbol.h>
#include <qwt_plot.h>

//STL
#include <limits>

te::qt::widgets::ScatterChart::ScatterChart(Scatter* data) :
  QwtPlotCurve(),
  m_scatter(data)
{
  //Set style
  setStyle(QwtPlotCurve::NoCurve);
  setPaintAttribute(QwtPlotCurve::FilterPoints );

  m_selection = new QwtPlotCurve();
  m_selection->setStyle(QwtPlotCurve::NoCurve);
  m_selection->setPaintAttribute(QwtPlotCurve::FilterPoints);
  m_selection->attach(plot());

  //Set Values
  setData();
}

te::qt::widgets::ScatterChart::ScatterChart(Scatter* data, ScatterStyle* style, size_t size) :
  QwtPlotCurve(),
  m_scatter(data),
  m_scatterStyle(style)
{
  //Set style
  setStyle(QwtPlotCurve::NoCurve);
  setPaintAttribute(QwtPlotCurve::FilterPoints);

  m_selection = new QwtPlotCurve();
  m_selection->setStyle(QwtPlotCurve::NoCurve);
  m_selection->setPaintAttribute(QwtPlotCurve::FilterPoints);
  m_selection->attach(plot());

  //Set Values
  setData();
}

void te::qt::widgets::ScatterChart::setData()
{
  QPolygonF samples;

  if(m_scatter->sizeX() < m_scatter->sizeY())
  {
    for (size_t i = 0; i < m_scatter->sizeX(); i++)
    {
      samples += QPointF( m_scatter->getX(i), m_scatter->getY(i));
    }
 }
  else
  {
    for (size_t i = 0; i < m_scatter->sizeY(); i++)
    {
      samples += QPointF( m_scatter->getX(i), m_scatter->getY(i));
    }
  }

  setSamples( samples );
}

te::qt::widgets::ScatterChart::~ScatterChart()
{  
  delete m_scatter;
  delete m_scatterStyle;
}

int  te::qt::widgets::ScatterChart::rtti() const
{
  return te::qt::widgets::SCATTER_CHART;
}

te::qt::widgets::Scatter* te::qt::widgets::ScatterChart::getScatter()
{
 return m_scatter;
}

void te::qt::widgets::ScatterChart::setScatter(te::qt::widgets::Scatter* newScatter)
{
  m_scatter = newScatter;
}

te::qt::widgets::ScatterStyle* te::qt::widgets::ScatterChart::getScatterStyle()
{
 return m_scatterStyle;
}

void te::qt::widgets::ScatterChart::setScatterStyle(te::qt::widgets::ScatterStyle* newSymbol)
{
  m_scatterStyle = newSymbol;
  setSymbol(m_scatterStyle->getSymbol());
  m_selection->setSymbol(new QwtSymbol( symbol()->style(), symbol()->brush().color().darker (180 ), symbol()->pen().color().darker( 180), symbol()->size()));
}

void te::qt::widgets::ScatterChart::highlight(const te::da::ObjectIdSet* oids)
{
  //Removing the previous selection, if there was any.
  m_selection->detach();

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator itObjSet; 
  QVector<QPointF> highlightedPoints;

    for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
    {
      std::pair<double, double> point = m_scatter->find((*itObjSet));
      highlightedPoints.push_back(QPointF(point.first, point.second));
    }

  m_selection->setSamples(highlightedPoints);
  m_selection->attach(plot());
  plot()->replot();
}

te::da::ObjectIdSet* te::qt::widgets::ScatterChart::highlight(QPointF point)
{
  const QwtScaleMap xMap = plot()->canvasMap( xAxis() );
  const QwtScaleMap yMap = plot()->canvasMap( yAxis() );

  const double cx = xMap.transform( point.rx());
  const double cy = yMap.transform( point.ry() );

  QPoint mappedPoint = QPoint(cx, cy);

  double dist;
  int index = closestPoint( mappedPoint, &dist );

  if(dist < 10)
  {
    double x = sample(index).x();
    double y = sample(index).y();
    return m_scatter->find(x, y);
  }
  else
  {
    double x = std::numeric_limits<double>::max();
    double y = std::numeric_limits<double>::max();
    return m_scatter->find(x, y);
  }
}
