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
  \file ChartDisplay.cpp

  \brief A class to represent a chart display.
*/

// TerraLib
#include "ChartDisplay.h"
#include "ChartStyle.h"
#include "Utils.h"
#include "../../../color/RGBAColor.h"
#include "../../../se.h"

//Qwt
#include <qwt_plot_grid.h>
#include <qwt_text.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
//Qt
#include <QtGui/QPen>

te::qt::widgets::ChartDisplay::ChartDisplay(QWidget* parent, QString title) : 
  QwtPlot(parent)
{
  m_chartStyle = new te::qt::widgets::ChartStyle();

  m_grid = new QwtPlotGrid;
  m_grid->enableX(true);
  m_grid->enableY(true);
  m_grid->setMajorPen(QPen(Qt::black, 0, Qt::SolidLine));
  m_grid->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));

  setTitle(title);
  setAutoFillBackground( true );
  setAutoReplot( true );

  // panning with the left mouse button
  ( void ) new QwtPlotPanner( this->canvas() );

  // zoom in/out with a bounding rectangle
//  ( void ) new QwtPlotZoomer( this->canvas() );

  // zoom in/out with the wheel
  ( void ) new QwtPlotMagnifier( this->canvas() );
}

te::qt::widgets::ChartDisplay::~ChartDisplay()
{
  delete m_chartStyle;
  delete m_grid;
}

te::qt::widgets::ChartStyle* te::qt::widgets::ChartDisplay::getStyle()
{
  return m_chartStyle;
}

void te::qt::widgets::ChartDisplay::setStyle(te::qt::widgets::ChartStyle* newStyle)
{
  m_chartStyle = newStyle;
  adjustDisplay();
}

void  te::qt::widgets::ChartDisplay::adjustDisplay()
{
  if(m_chartStyle)
  {
    setTitle(m_chartStyle->getTitle());
    setAxisTitle( QwtPlot::yLeft, m_chartStyle->getAxisY() );
    setAxisTitle( QwtPlot::xBottom,  m_chartStyle->getAxisX() );

    if(m_chartStyle->getGridChecked())
      m_grid->attach(this);
    else
      m_grid->detach();

    canvas()->setPalette(m_chartStyle->getColor());
  }
}