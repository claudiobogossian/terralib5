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
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "Enums.h"
#include "HistogramChart.h"
#include "Utils.h"
#include "../../../color/RGBAColor.h"
#include "../../../se.h"
#include "ScatterChart.h"

//Qwt
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_picker_machine.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
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

  // zoom in/out with the wheel
  ( void ) new QwtPlotMagnifier( this->canvas() );

  m_picker = new QwtPlotPicker(this->canvas());
  m_picker->setStateMachine(new QwtPickerClickPointMachine );

  connect(m_picker, SIGNAL(selected(const QPointF&)), SLOT(onPointPicked(const QPointF&)));
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

void te::qt::widgets::ChartDisplay::highlightOIds(const te::da::ObjectIdSet* oids)
{
  const QwtPlotItemList& itmList = itemList();
  for ( QwtPlotItemIterator it = itmList.begin();
    it != itmList.end(); ++it )
  {
    int type = ( *it )->rtti();
    switch (type)
    {
      case(te::qt::widgets::HISTOGRAM_CHART):
        static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight(oids);
        break;
      case(te::qt::widgets::SCATTER_CHART):
        static_cast<te::qt::widgets::ScatterChart*>(*it)->highlight(oids);
        break;
    }
  }
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

void te::qt::widgets::ChartDisplay::onPointPicked(const QPointF &pos)
{
  const QwtPlotItemList& itmList = itemList();
  for ( QwtPlotItemIterator it = itmList.begin();
      it != itmList.end(); ++it )
  {
    if ( ( *it )->rtti() == te::qt::widgets::SCATTER_CHART)
      {
        std::auto_ptr<QwtPlotCurve> c (static_cast<QwtPlotCurve *>( *it ));
        double dist = 10e10;
        int index = -1;
        double d;
            int idx = c->closestPoint( pos.toPoint(), &d );
          if ( d < dist )
          {
              index = idx;
              dist = d;
          }
          if ( c.get() && dist < 10 ) // 10 pixels tolerance
          {
              QwtSymbol *symbol = const_cast<QwtSymbol *>( c.get()->symbol() );
              symbol->setBrush( symbol->brush().color().dark( 180 ) );
          }
        break;
      }
     else if( ( *it )->rtti() == te::qt::widgets::HISTOGRAM_CHART )
      {
        static_cast<te::qt::widgets::HistogramChart*>(*it)->highlight( pos);
        break;
      }
  }
}