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
 \file SpatialReferenceSystem.cpp
 */

// TerraLib
#include "SpatialReferenceSystem.h"

te::srs::SpatialReferenceSystem::SpatialReferenceSystem(const std::string& name, const std::string& unitName) :
  m_name(name),
  m_unitName(unitName),
  m_srid(std::pair<unsigned int,std::string>(0,""))
{
}

te::srs::SpatialReferenceSystem::~SpatialReferenceSystem()
{
}

void te::srs::SpatialReferenceSystem::setName(const std::string& name) 
{ 
  m_name = name; 
}

const std::string& te::srs::SpatialReferenceSystem::getName() const 
{ 
  return m_name; 
}

void te::srs::SpatialReferenceSystem::setUnitName(const std::string& unitName)  
{ 
  m_unitName = unitName; 
}

const std::string& te::srs::SpatialReferenceSystem::getUnitName() const 
{ 
  return m_unitName; 
}

void te::srs::SpatialReferenceSystem::getSRID(int& id, std::string& authName) const
{
  id = m_srid.first;
  authName = m_srid.second;
}

void te::srs::SpatialReferenceSystem::setSRID(int id, const std::string& authName)
{
  m_srid.first = id;
  m_srid.second = authName;
}

