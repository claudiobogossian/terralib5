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
  \file terralib/raster/RasterProperty.cpp

  \brief Raster property.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../datatype/Enums.h"
#include "BandProperty.h"
#include "Grid.h"
#include "RasterProperty.h"

// STL
#include <cassert>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

te::rst::RasterProperty::RasterProperty(const std::string& name,
                                        bool isRequired,
                                        unsigned int id,
                                        te::dt::Property* parent)
  : te::dt::SimpleProperty(name, te::dt::RASTER_TYPE, isRequired, 0, id, parent),
    m_grid(0)
{
}

te::rst::RasterProperty::RasterProperty(Grid* grid,
                                        const std::vector<BandProperty*>& bands,
                                        const std::map<std::string, std::string>& rinfo,
                                        bool isRequired,
                                        unsigned int id,
                                        te::dt::Property* parent)
  : te::dt::SimpleProperty("", te::dt::RASTER_TYPE, isRequired, 0, id, parent),
    m_grid(grid),
    m_bands(bands),
    m_rinfo(rinfo)
{
}

te::rst::RasterProperty::RasterProperty(const RasterProperty& rhs)
  : te::dt::SimpleProperty(rhs)
{
  m_grid = rhs.m_grid ? new te::rst::Grid(*rhs.m_grid) : 0;

  te::common::Copy(rhs.m_bands, m_bands);

  m_rinfo = rhs.m_rinfo;
}

te::rst::RasterProperty::~RasterProperty() 
{
  delete m_grid;

  te::common::FreeContents(m_bands);
}

te::rst::RasterProperty& te::rst::RasterProperty::operator=(const RasterProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);
    
    delete m_grid;

    m_grid = rhs.m_grid ? new Grid(*rhs.m_grid) : 0;

    te::common::FreeContents(m_bands);

    m_bands.clear();

    te::common::Copy(rhs.m_bands, m_bands);

    m_rinfo = rhs.m_rinfo;
  }

  return *this;
}

void te::rst::RasterProperty::set(te::rst::Grid* grid)
{
  delete m_grid;

  m_grid = grid;
}

te::rst::Grid* te::rst::RasterProperty::getGrid()
{
  return m_grid;
}

const te::rst::Grid* te::rst::RasterProperty::getGrid() const
{
  return m_grid;
}

void te::rst::RasterProperty::add(te::rst::BandProperty* b)
{
  m_bands.push_back(b);
}

void te::rst::RasterProperty::set(std::size_t i, te::rst::BandProperty* b)
{
  assert(i < m_bands.size());

  delete m_bands[i];

  m_bands[i] = b;
}

std::vector<te::rst::BandProperty*>& te::rst::RasterProperty::getBandProperties() 
{ 
  return m_bands; 
} 

const std::vector<te::rst::BandProperty*>& te::rst::RasterProperty::getBandProperties() const
{ 
  return m_bands; 
}

void te::rst::RasterProperty::setInfo(const std::map<std::string, std::string>& rinfo)
{
  m_rinfo = rinfo;
}

const std::map<std::string, std::string>& te::rst::RasterProperty::getInfo() const
{
  return m_rinfo;
}

te::dt::Property* te::rst::RasterProperty::clone() const
{
  return new RasterProperty(*this);
}

//void te::rst::RasterProperty::setName(const std::string& name)
//{
//  m_name = name;
//
//  boost::filesystem::path path(m_rinfo["URI"]);
//
//  if (path.has_filename())
//    path.remove_filename();
//
//  path /= name;
//
//  m_rinfo["URI"] = path.string();
//}
