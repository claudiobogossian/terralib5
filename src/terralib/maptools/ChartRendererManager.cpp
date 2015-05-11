/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/ChartRendererManager.cpp

  \brief This is a singleton for managing chart renderer instance available in the system.
*/

// TerraLib
#include "../common/Translator.h"
#include "AbstractChartRenderer.h"
#include "ChartRendererManager.h"
#include "Exception.h"

// STL
#include <cassert>

te::map::ChartRendererManager::ChartRendererManager()
  : m_renderer(0)
{
}

te::map::ChartRendererManager::~ChartRendererManager()
{
  delete m_renderer;
}

te::color::RGBAColor** te::map::ChartRendererManager::render(const Chart* chart, const te::da::DataSet* dataset, std::size_t& width)
{
  LockWrite l;

  if(m_renderer == 0)
    throw Exception(TE_TR("There is not a chart renderer registered!"));

  return m_renderer->render(chart, dataset, width);
}

te::color::RGBAColor** te::map::ChartRendererManager::render(const Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width)
{
  LockWrite l;

  if(m_renderer == 0)
    throw Exception(TE_TR("There is not a chart renderer registered!"));

  return m_renderer->render(chart, chartValue, width);
}

void te::map::ChartRendererManager::set(AbstractChartRenderer* renderer)
{
  delete m_renderer;
  m_renderer = renderer;
}
