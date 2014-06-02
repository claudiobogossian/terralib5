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
#include <qapplication.h>
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
  m_magnifier = new QwtPlotMagnifier(this->canvas());
  m_magnifier->setMouseButton(Qt::MiddleButton);

  // Pan on the plotted chart
  m_panner =  new QwtPlotPanner(this->canvas());
  m_panner->setMouseButton(Qt::RightButton);

  // Selection based on a rectangle - also works as a point based selection if the rectangle's width and height are both equal 0
  m_leftPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::RectRubberBand, QwtPicker::AlwaysOff, this->canvas());
  m_leftPicker->setStateMachine(new QwtPickerDragRectMachine );

  m_ctrlPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::RectRubberBand, QwtPicker::AlwaysOff, this->canvas());
  m_ctrlPicker->setStateMachine(new QwtPickerDragRectMachine );
  m_ctrlPicker->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier);

  m_shiftPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::RectRubberBand, QwtPicker::AlwaysOff, this->canvas());
  m_shiftPicker->setStateMachine(new QwtPickerDragRectMachine );
  m_shiftPicker->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ShiftModifier);

  connect(m_leftPicker, SIGNAL(selected(const QRectF&)), SLOT(onRectPicked(const QRectF&)));
  connect(m_ctrlPicker, SIGNAL(selected(const QRectF&)), SLOT(onRectPicked(const QRectF&)));
  connect(m_shiftPicker, SIGNAL(selected(const QRectF&)), SLOT(onRectPicked(const QRectF&)));

  canvas()->setCursor(Qt::CrossCursor);
}

te::qt::widgets::ChartDisplay::~ChartDisplay()
{
  delete m_chartStyle;
  delete m_ctrlPicker;
  delete m_grid;
  delete m_leftPicker;
  delete m_magnifier;
  delete m_panner;
  delete m_shiftPicker;
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
  if(oids)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
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
    QApplication::restoreOverrideCursor();
  }
}

void te::qt::widgets::ChartDisplay::setSelectionColor(QColor selColor)
{
  const QwtPlotItemList& itmList = itemList(); 

  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
    if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
      {
        static_cast<te::qt::widgets::ScatterChart*>(*it)->setSelectionColor(selColor);
        break;
      }
     else if( ( *it )->rtti() == te::qt::widgets::HISTOGRAM_CHART )
      {
        static_cast<te::qt::widgets::HistogramChart*>(*it)->setSelectionColor(selColor);
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

void te::qt::widgets::ChartDisplay::onRectPicked(const QRectF &rect)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  const QwtPlotItemList& itmList = itemList();
  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
  if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
    {
      if (QObject::sender() == m_ctrlPicker || QObject::sender() == m_shiftPicker)
        emit selected(static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight( rect), true);
      else
        emit selected(static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight( rect), false);

      break;
    }
    else if( ( *it )->rtti() == te::qt::widgets::HISTOGRAM_CHART )
    {
      if (QObject::sender() == m_ctrlPicker || QObject::sender() == m_shiftPicker)
        emit selected(static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight( rect), true);
      else
        emit selected(static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight( rect), false);

      break;
    }
  }
  QApplication::restoreOverrideCursor();
}