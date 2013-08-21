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
#include "../../../color/RGBAColor.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../se.h"
#include "ChartDisplay.h"
#include "ChartStyle.h"
#include "Enums.h"
#include "HistogramChart.h"
#include "ScatterChart.h"
#include "Utils.h"

//Qwt
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_picker_machine.h>
#include <qwt_text.h>
#include <qwt_symbol.h>

//Qt
#include <QtGui/QPen>

//STL
#include <memory>

te::qt::widgets::ChartDisplay::ChartDisplay(QWidget* parent, QString title, ChartStyle* style) : 
  QwtPlot(parent),
  m_chartStyle(style)
{
  if(!m_chartStyle)
    m_chartStyle = new te::qt::widgets::ChartStyle();

  m_grid = new QwtPlotGrid;
  m_grid->enableX(true);
  m_grid->enableY(true);
  m_grid->setMajorPen(QPen(Qt::black, 0, Qt::SolidLine));
  m_grid->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));

  setTitle(title);
  setAutoFillBackground( true );
  setAutoReplot( true );

  // zoom in/out with the wheel
  ( void ) new QwtPlotMagnifier( this->canvas() );

  // Selection based on a point
  m_picker = new QwtPlotPicker(this->canvas());
  m_picker->setStateMachine(new QwtPickerClickPointMachine );

  connect(m_picker, SIGNAL(selected(const QPointF&)), SLOT(onPointPicked(const QPointF&)));
}

te::qt::widgets::ChartDisplay::~ChartDisplay()
{
  delete m_chartStyle;
  delete m_grid;
}

void te::qt::widgets::ChartDisplay::setPickerStyle(int chartType)
{
  switch(chartType)
  {
    case(te::qt::widgets::SCATTER_CHART):
      delete m_picker;
      m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::RectRubberBand, QwtPicker::AlwaysOff, this->canvas());
      m_picker->setStateMachine(new QwtPickerDragRectMachine );
      connect(m_picker, SIGNAL(selected(const QRectF&)), SLOT(onRectPicked(const QRectF&)));
      break;
    default:
      delete m_picker;
      m_picker = new QwtPlotPicker(this->canvas());
      m_picker->setStateMachine(new QwtPickerClickPointMachine );
      connect(m_picker, SIGNAL(selected(const QPointF&)), SLOT(onPointPicked(const QPointF&)));
      break;
  }
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

void te::qt::widgets::ChartDisplay::highlightOIds(const te::da::ObjectIdSet* oids)
{
  const QwtPlotItemList& itmList = itemList(); 

  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
    if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
      {
        static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight( oids);
        break;
      }
     else if( ( *it )->rtti() == te::qt::widgets::HISTOGRAM_CHART )
      {
        static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight( oids);
        break;
      }
  }
}

void  te::qt::widgets::ChartDisplay::adjustDisplay()
{
  if(m_chartStyle)
  {

    QwtText title( m_chartStyle->getTitle());
    QwtText axisX(m_chartStyle->getAxisX());
    QwtText axisY(m_chartStyle->getAxisY());

    title.setFont(m_chartStyle->getTitleFont());
    axisX.setFont(m_chartStyle->getAxisFont());
    axisY.setFont(m_chartStyle->getAxisFont());

    setTitle(title);
    setAxisTitle( QwtPlot::yLeft, axisY);
    setAxisTitle( QwtPlot::xBottom, axisX);

    if(m_chartStyle->getGridChecked())
      m_grid->attach(this);
    else
      m_grid->detach();

    canvas()->setPalette(m_chartStyle->getColor());
  }
}

void te::qt::widgets::ChartDisplay::onPointPicked(const QPointF &pos)
{
  const QwtPlotItemList& itmList = itemList();
  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
    if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
    {
      emit selected(static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight( pos), false);
      break;
    }
    else if( ( *it )->rtti() == te::qt::widgets::HISTOGRAM_CHART )
    {
      emit selected(static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight( pos), false);
      break;
    }
  }
}

void te::qt::widgets::ChartDisplay::onRectPicked(const QRectF &rect)
{
  const QwtPlotItemList& itmList = itemList();
  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
  if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
    {
      emit selected(static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight( rect), false);
    }
    break;
  }
}