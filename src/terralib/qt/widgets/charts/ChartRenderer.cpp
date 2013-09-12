/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/ChartRenderer.cpp

  \brief A concrete chart renderer based on Qt4.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../maptools/Chart.h"
#include "../Utils.h"
#include "ChartRenderer.h"

// Boost
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>

te::qt::widgets::ChartRenderer::ChartRenderer()
  : te::map::AbstractChartRenderer()
{
  m_pen.setStyle(Qt::SolidLine);
  m_brush.setStyle(Qt::SolidPattern);
}

te::qt::widgets::ChartRenderer::~ChartRenderer()
{
}

te::color::RGBAColor** te::qt::widgets::ChartRenderer::render(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width)
{
  assert(chart);
  assert(dataset);

  QImage* result = 0;

  switch(chart->getType())
  {
    case te::map::Pie:
      result = drawPies(chart, dataset, width);
    break;

    case te::map::Bar:
      result = drawBars(chart, dataset, width);
    break;
  }

  // Converts QImage to te::color::RGBAColor**
  te::color::RGBAColor** rgba = te::qt::widgets::GetImage(result);

  delete result;

  return rgba;
}

void te::qt::widgets::ChartRenderer::setup(QImage* img)
{
  m_painter.begin(img);
  m_painter.setRenderHints(QPainter::Antialiasing);
}

void te::qt::widgets::ChartRenderer::end()
{
  m_painter.end();
}

QImage* te::qt::widgets::ChartRenderer::drawPies(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width)
{
  width = chart->getHeight();

  // Creates the image that will represent the chart
  QImage* img = new QImage(width, chart->getHeight(), QImage::Format_ARGB32_Premultiplied);
  img->fill(Qt::transparent);

  setup(img);

  m_pen.setColor(Convert2Qt(chart->getContourColor()));
  m_pen.setWidth(static_cast<int>(chart->getContourWidth()));
  m_painter.setPen(m_pen);

  std::vector<double> values;
  getValues(chart, dataset, values);

  double sum = computeSum(values);

  int lastAngle = 0;

  // Draw each pie slice
  for(std::size_t i = 0; i < values.size(); ++i)
  {
    // Configs a new color for this slice
    m_painter.setBrush(Convert2Qt(chart->getColor(i)));

    int currentAngle = static_cast<int>((values[i] * 5760) / sum);

    m_painter.drawPie(img->rect(), lastAngle, currentAngle);

    lastAngle += currentAngle;
  }

  end();

  return img;
}

QImage* te::qt::widgets::ChartRenderer::drawBars(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width)
{
  // TODO: For while, add it on te::map::Chart
  std::size_t barWidth = 16;

  std::vector<double> values;
  getValues(chart, dataset, values);

  width = barWidth * values.size();

  // Creates the image that will represent the chart
  QImage* img = new QImage(width, chart->getHeight(), QImage::Format_ARGB32_Premultiplied);
  img->fill(Qt::transparent);

  setup(img);

  m_pen.setColor(Convert2Qt(chart->getContourColor()));
  m_pen.setWidth(static_cast<int>(chart->getContourWidth()));
  m_painter.setPen(m_pen);

  double maxValue = getMaxValue(values);

  int lastx = 0;

  // Draw each bar
  for(std::size_t i = 0; i < values.size(); ++i)
  {
    // Configs a new color for this slice
    m_painter.setBrush(Convert2Qt(chart->getColor(i)));

    int barHeight = static_cast<int>((values[i] * chart->getHeight()) / maxValue);

    QRect bar(lastx, 0, barWidth, barHeight);

    m_painter.drawRect(bar);

    lastx += barWidth;
  }

  end();

  return img;
}

void te::qt::widgets::ChartRenderer::getValues(const te::map::Chart* chart, const te::da::DataSet* dataset, std::vector<double>& values)
{
  std::size_t precision = 5;

  const std::vector<std::string>& properties = chart->getProperties();
  
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    std::string value = dataset->getAsString(properties[i], precision);
    values.push_back(boost::lexical_cast<double>(value));
  }
}

double te::qt::widgets::ChartRenderer::computeSum(const std::vector<double>& values)
{
  double sum = 0.0;

  for(std::size_t i = 0; i < values.size(); ++i)
    sum += values[i];

  return sum;
}

double te::qt::widgets::ChartRenderer::getMaxValue(const std::vector<double>& values)
{
  double max = 0.0;

  for(std::size_t i = 0; i < values.size(); ++i)
    if(values[i] > max)
      max = values[i];

  return max;
}
