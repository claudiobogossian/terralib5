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


#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "Enums.h"
#include "ScatterChart.h"
#include "Scatter.h"
#include "ScatterStyle.h"
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

  //Adjusting the symbol
  m_scatterStyle =  new te::qt::widgets::ScatterStyle();
  if(m_scatter->sizeX() > 100 || m_scatter->sizeY() > 100)
  {
    QwtSymbol* newSymbol =  m_scatterStyle->getSymbol(); 
    newSymbol->setSize(QSize( 1, 1 ));
    setSymbol(newSymbol);
  }
  else
  {
    setSymbol(new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::gray ), QPen( Qt::black, 1 ), QSize( 8, 8 )));
  }

  //The default selection color is green
  m_selColor = ("#00FF00");
  m_selection->setSymbol(new QwtSymbol( symbol()->style(), m_selColor, m_selColor, symbol()->size()));
}

te::qt::widgets::ScatterChart::ScatterChart(Scatter* data, ScatterStyle* style, size_t size) :
  QwtPlotCurve(),
  m_scatter(data),
  m_scatterStyle(style)
{

  m_selection = new QwtPlotCurve();
  m_selection->setStyle(QwtPlotCurve::NoCurve);
  m_selection->setPaintAttribute(QwtPlotCurve::FilterPoints);
  m_selection->attach(plot());

  //Set style
  setStyle(QwtPlotCurve::NoCurve);
  setPaintAttribute(QwtPlotCurve::FilterPoints);

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
  delete m_selection;
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
  delete m_scatter;
  m_scatter = newScatter;
}

te::qt::widgets::ScatterStyle* te::qt::widgets::ScatterChart::getScatterStyle()
{
 return m_scatterStyle->clone();
}

void te::qt::widgets::ScatterChart::setScatterStyle(te::qt::widgets::ScatterStyle* newSymbol)
{
  delete m_scatterStyle;
  m_scatterStyle = newSymbol;

  //Updating the Chart's symbol based on the current style
  setSymbol(m_scatterStyle->getSymbol());

  //Updating the selection symbol based on the current style;
  QwtSymbol* selSymbol = new QwtSymbol( symbol()->style(), symbol()->brush().color().darker (180 ), symbol()->pen().color().darker( 180), symbol()->size());
  QPixmap selPixmap = symbol()->pixmap();
  selPixmap.fill(m_selColor);
  selSymbol->setPixmap(selPixmap);
  m_selection->setSymbol(selSymbol);
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
      if(this->boundingRect().contains(point.first, point.second))
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

  int index = closestPoint( mappedPoint);

  double x = sample(index).x();
  double y = sample(index).y();
  return m_scatter->find(x, y);
}

te::da::ObjectIdSet* te::qt::widgets::ScatterChart::highlight(QRectF rect)
{
  QwtSeriesData<QPointF>* values = data();
  std::vector<QPointF> selected;

  if(rect.width() == 0 && rect.height() == 0)
    return highlight(QPoint(rect.x(), rect.y()));

  //Acquiring all the selected points:
  for(size_t i = 0; i < values->size(); ++i)
  {
    if( rect.contains(values->sample(i)))
      selected.push_back(values->sample(i));
  }
  return m_scatter->find(selected);
}

void te::qt::widgets::ScatterChart::setSelectionColor(QColor selColor)
{
  m_selColor = selColor;

  //The highlighted color will always have an alpha aplied to it
  m_selColor.setAlpha(100);
}