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
  \file FilterCapabilities.cpp
  
  \brief This class can be used to create capabilities document of services that use filter encoding.
 */

// TerraLib
#include "FilterCapabilities.h"
#include "IdCapabilities.h"
#include "ScalarCapabilities.h"
#include "SpatialCapabilities.h"

// STL
#include <cassert>

te::fe::FilterCapabilities::FilterCapabilities()
  : m_spatialCapabilities(0),
    m_scalarCapabilities(0),
    m_idCapabilities(0)
{
}

te::fe::FilterCapabilities::~FilterCapabilities()
{
  delete m_spatialCapabilities;
  delete m_scalarCapabilities;
  delete m_idCapabilities;
}

void te::fe::FilterCapabilities::setSpatialCapabilities(SpatialCapabilities* spCapabilities)
{
  delete m_spatialCapabilities;
  m_spatialCapabilities = spCapabilities;
}

const te::fe::SpatialCapabilities* te::fe::FilterCapabilities::getSpatialCapabilities() const
{
  return m_spatialCapabilities;
}

void te::fe::FilterCapabilities::setScalarCapabilities(ScalarCapabilities* scCapabilities)
{
  delete m_scalarCapabilities;
  m_scalarCapabilities = scCapabilities;
}

const te::fe::ScalarCapabilities* te::fe::FilterCapabilities::getScalarCapabilities() const
{
  return m_scalarCapabilities;
}

void te::fe::FilterCapabilities::setIdCapabilities(IdCapabilities* idCapabilities)
{
  delete m_idCapabilities;
  m_idCapabilities = idCapabilities;
}

const te::fe::IdCapabilities* te::fe::FilterCapabilities::getIdCapabilities() const
{
  return m_idCapabilities;
}

