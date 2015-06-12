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
  \file SpatialCapabilities.cpp
  
  \brief Spatial capabilities include the ability to filter spatial data.
 */

// TerraLib
#include "GeometryOperands.h"
#include "SpatialCapabilities.h"
#include "SpatialOperators.h"

te::fe::SpatialCapabilities::SpatialCapabilities()
  : m_geometryOperands(0),
    m_spatialOperators(0)
{
}

te::fe::SpatialCapabilities::~SpatialCapabilities()
{
  delete m_geometryOperands;
  delete m_spatialOperators;
}

void te::fe::SpatialCapabilities::setGeometryOperands(GeometryOperands* gOps)
{
  delete m_geometryOperands;
  m_geometryOperands = gOps;
}

const te::fe::GeometryOperands* te::fe::SpatialCapabilities::getGeometryOperands() const
{
  return m_geometryOperands;
}

void te::fe::SpatialCapabilities::setSpatialOperators(SpatialOperators* sOps)
{
  delete m_spatialOperators;
  m_spatialOperators = sOps;
}

const te::fe::SpatialOperators* te::fe::SpatialCapabilities::getSpatialOperators() const
{
  return m_spatialOperators;
}

