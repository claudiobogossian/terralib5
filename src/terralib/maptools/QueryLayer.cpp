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
  \file QueryLayer.cpp

  \brief A QueryLayer is a reference to a set of features with geometric attributes.
 */

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/query/Select.h"
#include "../geometry/Envelope.h"
#include "../se/FeatureTypeStyle.h"
#include "../se/Style.h"
#include "QueryLayer.h"
#include "QueryLayerRenderer.h"

const std::string te::map::QueryLayer::sm_type("QUERYLAYER");

te::map::QueryLayer::QueryLayer(const std::string& id,
                                const std::string& title,
                                AbstractLayer* parent)
  : te::map::AbstractLayer(id, title, parent),
    m_srid(-1),
    m_query(0),
    m_ds(0),
    m_mbr(0),
    m_style(0),
    m_renderer(0)
{}

te::map::QueryLayer::~QueryLayer()
{  
  delete m_query;  
  delete m_mbr;
  delete m_style;
  delete m_renderer;
}

const std::string& te::map::QueryLayer::getType() const
{
  return sm_type;
}

bool te::map::QueryLayer::isValid() const
{
  return false;
}

bool te::map::QueryLayer::isQueryable() const
{
  return false;
}

void te::map::QueryLayer::draw(Canvas* /*canvas*/, const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

te::se::Style* te::map::QueryLayer::getStyle() const
{
  return m_style;
}

void te::map::QueryLayer::setStyle(te::se::Style* style)
{
  delete m_style;
  m_style = style;
}

const te::gm::Envelope* te::map::QueryLayer::getExtent() const
{
  return m_mbr;
}

void te::map::QueryLayer::setExtent(te::gm::Envelope* mbr)
{
  delete m_mbr;
  m_mbr = mbr;
}

int te::map::QueryLayer::getSRID() const
{
  return m_srid;
}

void te::map::QueryLayer::setSRID(int srid)
{
  m_srid = srid;
}

te::da::Select* te::map::QueryLayer::getQuery() const
{
  return m_query;
}

void te::map::QueryLayer::setQuery(te::da::Select* query)
{
  delete m_query;
  m_query = query;
}

te::da::DataSource* te::map::QueryLayer::getDataSource() const
{
  return m_ds;
}

void te::map::QueryLayer::setDataSource(te::da::DataSource* ds)
{
  m_ds = ds;
}

