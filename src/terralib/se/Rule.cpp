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
  \file terralib/se/Rule.cpp
  
  \brief A Rule is used to attach property/scale conditions to and group the individual symbols used for rendering.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../fe/Filter.h"
#include "Description.h"
#include "Graphic.h"
#include "Rule.h"
#include "Symbolizer.h"

// STL
#include <cassert>
#include <limits>

te::se::Rule::Rule()
  : m_name(0),
    m_description(0),
    m_legendGraphic(0),
    m_filter(0),
    m_elseFilter(false),
    m_minScaleDenominator(0.0),
    m_maxScaleDenominator(std::numeric_limits<double>::infinity())
{
}

te::se::Rule::~Rule()
{
  delete m_name;
  delete m_description;
  delete m_legendGraphic;
  delete m_filter;
  te::common::FreeContents(m_symbolizers);
}

void te::se::Rule::setName(std::string* name)
{
  delete m_name;
  m_name = name;
}

const std::string* te::se::Rule::getName() const
{
  return m_name;
}

void te::se::Rule::setDescription(Description* d)
{
  delete m_description;
  m_description = d;
}

const te::se::Description* te::se::Rule::getDescription() const
{
  return m_description;
}

void te::se::Rule::setLegendGraphic(Graphic* legendGraphic)
{
  delete m_legendGraphic;
  m_legendGraphic = legendGraphic;
}

const te::se::Graphic* te::se::Rule::getLegendGraphic() const
{
  return m_legendGraphic;
}

void te::se::Rule::setFilter(te::fe::Filter* f)
{
  delete m_filter;
  m_filter = f;
}

const te::fe::Filter* te::se::Rule::getFilter() const
{
  return m_filter;
}

void te::se::Rule::enableElseFilter()
{
  assert(m_filter == 0);
  m_elseFilter = true;
}

void te::se::Rule::disableElseFilter()
{
  m_elseFilter = false;
}

bool te::se::Rule::hasElseFilter() const
{
  return m_elseFilter;
}

void te::se::Rule::setMinScaleDenominator(const double& minScaleDenominator)
{
  m_minScaleDenominator = minScaleDenominator;
}

const double& te::se::Rule::getMinScaleDenominator() const
{
  return m_minScaleDenominator;
}

void te::se::Rule::setMaxScaleDenominator(const double& maxScaleDenominator)
{
  m_maxScaleDenominator = maxScaleDenominator;
}

const double& te::se::Rule::getMaxScaleDenominator() const
{
  return m_maxScaleDenominator;
}

void te::se::Rule::push_back(Symbolizer* s)
{
  assert(s);
  m_symbolizers.push_back(s);
}

void te::se::Rule::setSymbolizer(std::size_t i, te::se::Symbolizer* s)
{
  assert(s);
  assert(i < m_symbolizers.size());
  delete m_symbolizers[i];
  m_symbolizers[i] = s;
}

void te::se::Rule::setSymbolizers(const std::vector<Symbolizer*>& symbs)
{
  clearSymbolizers();
  m_symbolizers = symbs;
}

const std::vector<te::se::Symbolizer*>& te::se::Rule::getSymbolizers() const
{
  return m_symbolizers;
}

const te::se::Symbolizer* te::se::Rule::getSymbolizer(std::size_t i) const
{
  assert(i < m_symbolizers.size());
  return m_symbolizers[i];
}

void te::se::Rule::removeSymbolizer(std::size_t i)
{
  assert(i < m_symbolizers.size());

  delete m_symbolizers[i];

  m_symbolizers.erase(m_symbolizers.begin() + i);
}

void te::se::Rule::clearSymbolizers()
{
  te::common::FreeContents(m_symbolizers);
  m_symbolizers.clear();
}

te::se::Rule* te::se::Rule::clone() const
{
  Rule* rule = new Rule;

  if(m_name)
    rule->setName(new std::string(*m_name));

  if(m_description)
    rule->setDescription(m_description->clone());

  if(m_legendGraphic)
    rule->setLegendGraphic(m_legendGraphic->clone());

  rule->setFilter(0); // TODO: Filter clone

  m_elseFilter?rule->enableElseFilter():rule->disableElseFilter();

  rule->setMinScaleDenominator(getMinScaleDenominator());

  rule->setMaxScaleDenominator(getMaxScaleDenominator());

  for(std::size_t i = 0; i < m_symbolizers.size(); ++i)
  {
    if(m_symbolizers[i])
      rule->m_symbolizers.push_back(m_symbolizers[i]->clone());
  }

  return rule;
}