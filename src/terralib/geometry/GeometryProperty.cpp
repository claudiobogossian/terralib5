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
  \file terralib/geometry/GeometryProperty.cpp

  \brief Geometric property.
*/

// TerraLib
#include "../datatype/Enums.h"
#include "Envelope.h"
#include "GeometryProperty.h"

te::gm::GeometryProperty::GeometryProperty(const std::string& name,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           te::dt::Property* parent)
  : SimpleProperty(name, te::dt::GEOMETRY_TYPE, isRequired, defaultValue, id, parent),
    m_srid(0),
    m_geomType(UnknownGeometryType)
{
}

te::gm::GeometryProperty::GeometryProperty(const std::string& name,
                                           int srid,
                                           GeomType subtype,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           te::dt::Property* parent)
  : SimpleProperty(name, te::dt::GEOMETRY_TYPE, isRequired, defaultValue, id, parent),
    m_srid(srid),
    m_geomType(subtype)
{
}

te::gm::GeometryProperty::GeometryProperty(const GeometryProperty& rhs)
  : SimpleProperty(rhs),
    m_srid(rhs.m_srid),
    m_geomType(rhs.m_geomType)
{
}

te::gm::GeometryProperty::~GeometryProperty()
{
}

te::dt::Property* te::gm::GeometryProperty::clone() const
{
  return new GeometryProperty(*this);
}

