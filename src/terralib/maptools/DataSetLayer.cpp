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
  \file terralib/maptools/DataSetLayer.cpp

  \brief A layer with reference to a dataset.
*/

// TerraLib
#include "../se/Style.h"
#include "DataSetLayer.h"

const std::string te::map::DataSetLayer::sm_type("DATASET_LAYER");

te::map::DataSetLayer::DataSetLayer(AbstractLayer* parent)
  : AbstractLayer(parent),
    m_style(0)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent),
    m_style(0)
{
}

te::map::DataSetLayer::DataSetLayer(const std::string& id,
                                    const std::string& title,
                                    AbstractLayer* parent)
  : AbstractLayer(id, title, parent),
    m_style(0)
{
}

te::map::DataSetLayer::~DataSetLayer()
{
  delete m_style;
}

bool te::map::DataSetLayer::isValid() const
{
  return true;
}

void te::map::DataSetLayer::draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
}

const std::string& te::map::DataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::map::DataSetLayer::getDataSetName() const
{
  return m_datasetName;
}

void te::map::DataSetLayer::setDataSetName(const std::string& name)
{
  m_datasetName = name;
}

const std::string& te::map::DataSetLayer::getDataSourceId() const
{
  return m_datasourceId;
}

void te::map::DataSetLayer::setDataSourceId(const std::string& id)
{
  m_datasourceId = id;
}

const std::string& te::map::DataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::map::DataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

te::se::Style* te::map::DataSetLayer::getStyle() const
{
  return m_style;
}

void te::map::DataSetLayer::setStyle(te::se::Style* style)
{
  delete m_style;

  m_style = style;
}

