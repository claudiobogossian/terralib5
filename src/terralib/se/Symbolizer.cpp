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
  \file terralib/se/Symbolizer.cpp
  
  \brief A Symbolizer describes how a Feature is to appear on a map.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../common/UnitOfMeasure.h"
#include "../common/UnitsOfMeasureManager.h"
#include "../xlink/SimpleLink.h"
#include "Description.h"
#include "Symbolizer.h"

te::se::Symbolizer::Symbolizer()
  : m_description(0),
    m_baseSymbolizer(0),
    m_version(TE_SE_DEFAULT_VERSION),
    m_uom(0)
{
}

te::se::Symbolizer::Symbolizer(const te::se::Symbolizer& rhs)
  : m_name(rhs.m_name),
    m_description(0),
    m_baseSymbolizer(0),
    m_version(rhs.m_version),
    m_uom(0)
{
  if(rhs.m_description)
    m_description = rhs.m_description->clone();

  if(rhs.m_baseSymbolizer)
    m_baseSymbolizer = new te::xl::SimpleLink(*rhs.m_baseSymbolizer);

  // TODO: Review BaseUnitOfMeasure
  //if(rhs.m_uom) 
    //m_uom = new te::common::BaseUnitOfMeasure(*m_uom);
}

te::se::Symbolizer::~Symbolizer()
{
  delete m_description;
  delete m_baseSymbolizer;
}

void te::se::Symbolizer::setName(const std::string& name)
{
  m_name = name;
}

const std::string& te::se::Symbolizer::getName() const
{
  return m_name;
}

void te::se::Symbolizer::setDescription(Description* d)
{
  delete m_description;
  m_description = d;
}

const te::se::Description* te::se::Symbolizer::getDescription() const
{
  return m_description;
}

void te::se::Symbolizer::setBaseSymbolizer(te::xl::SimpleLink* baseSymbolizer)
{
  delete m_baseSymbolizer;
  m_baseSymbolizer = baseSymbolizer;
}

const te::xl::SimpleLink* te::se::Symbolizer::getBaseSymbolizer() const
{
  return m_baseSymbolizer;
}

void te::se::Symbolizer::setVersion(const std::string& version)
{
  m_version = version;
}

const std::string& te::se::Symbolizer::getVersion() const
{
  return m_version;
}

void te::se::Symbolizer::setUom(const te::common::BaseUnitOfMeasure* uom)
{
  m_uom = uom;
}

const te::common::BaseUnitOfMeasure* te::se::Symbolizer::getUom() const
{
  return m_uom;
}
