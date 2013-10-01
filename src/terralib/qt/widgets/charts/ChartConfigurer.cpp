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
  \file terralib/qt/widgets/charts/ChartConfigurer.cpp
  
  \brief A Symbology Enconding visitor that configures a given chart based on symbolizers elements.
*/

// TerraLib

#include "../../../common/STLUtils.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Utils.h"
#include "../../../maptools/ExternalGraphicRendererManager.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../se/ExternalGraphic.h"
#include "../../../se/Fill.h"
#include "../../../se/Graphic.h"
#include "../../../se/GraphicStroke.h"
#include "../../../se/LineSymbolizer.h"
#include "../../../se/Mark.h"
#include "../../../se/PointSymbolizer.h"
#include "../../../se/PolygonSymbolizer.h"
#include "../../../se/Stroke.h"
#include "../../../se/Symbolizer.h"
#include "../../../se/Style.h"
#include "../../../se/Rule.h"
#include "../../../se/Utils.h"
#include "ChartConfigurer.h"
#include "HistogramChart.h"
#include "HistogramStyle.h"
#include "ScatterChart.h"
#include "ScatterStyle.h"

// STL
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <vector>

//QWT
#include <qwt_plot_seriesitem.h>

te::qt::widgets::ChartConfigurer::ChartConfigurer(ChartDisplayWidget* chart)
{
  m_chart = chart;
}

te::qt::widgets::ChartConfigurer::~ChartConfigurer()
{
}

void te::qt::widgets::ChartConfigurer::config(const te::se::Symbolizer* symbolizer)
{
  symbolizer->accept(*this);
}

void te::qt::widgets::ChartConfigurer::config(te::map::AbstractLayer* layer)
{
  config(layer->getStyle()->getRule(0)->getSymbolizer(0));
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::Style& visited)
{
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::FeatureTypeStyle& visited)
{
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::CoverageStyle& visited)
{
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::Symbolizer& visited)
{
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::PolygonSymbolizer& visited)
{
  const te::se::Stroke* stroke = visited.getStroke();
  if(stroke)
    config(stroke);

  const te::se::Fill* fill = visited.getFill();
  if(fill)
    config(fill);
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::LineSymbolizer& visited)
{
  const te::se::Stroke* stroke = visited.getStroke();
  if(!stroke)
  {
    return;
  }

  config(stroke);
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::PointSymbolizer& visited)
{
  const te::se::Graphic* graphic = visited.getGraphic();
  if(graphic)
    config(graphic);
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::TextSymbolizer& visited)
{
}

void te::qt::widgets::ChartConfigurer::visit(const te::se::RasterSymbolizer& visited)
{
}

void te::qt::widgets::ChartConfigurer::config(const te::se::Stroke* stroke)
{
  int chartType = m_chart->getChart()->rtti();

  switch(chartType)
  {
    case(te::qt::widgets::SCATTER_CHART):
      static_cast<te::qt::widgets::ScatterChart*>(m_chart->getChart())->getScatterStyle()->setStroke(stroke->clone());
      break;
    case(te::qt::widgets::HISTOGRAM_CHART):
      static_cast<te::qt::widgets::HistogramChart*>(m_chart->getChart())->getHistogramStyle()->setStroke(stroke->clone());
      break;
  }
}

void te::qt::widgets::ChartConfigurer::config(const te::se::Fill* fill)
{
  int chartType = m_chart->getChart()->rtti();


  switch(chartType)
  {
    case(te::qt::widgets::SCATTER_CHART):
      static_cast<te::qt::widgets::ScatterChart*>(m_chart->getChart())->getScatterStyle()->setFill(fill->clone());
      break;
    case(te::qt::widgets::HISTOGRAM_CHART):
      static_cast<te::qt::widgets::HistogramChart*>(m_chart->getChart())->getHistogramStyle()->setFill(fill->clone());
      break;
  }
}

void te::qt::widgets::ChartConfigurer::config(const te::se::Graphic* graphic)
{
  int chartType = m_chart->getChart()->rtti();
  if(chartType == te::qt::widgets::SCATTER_CHART)
    static_cast<te::qt::widgets::ScatterChart*>(m_chart->getChart())->getScatterStyle()->setGraphic(graphic->clone());
}


