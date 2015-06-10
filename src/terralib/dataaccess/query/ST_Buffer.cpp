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
  \file terralib/dataaccess/query/ST_Buffer.cpp

  \brief Spatial buffer operator.
*/

// TerraLib
#include "../../geometry/Geometry.h"
#include "LiteralDouble.h"
#include "LiteralGeom.h"
#include "LiteralString.h"
#include "PropertyName.h"
#include "ST_Buffer.h"

te::da::ST_Buffer::ST_Buffer(const te::gm::Geometry& g, const double& d)
  : Function("st_buffer")
{
  LiteralGeom* geom = new LiteralGeom(g);
  LiteralDouble* dist = new LiteralDouble(d);

  m_args.push_back(geom);
  m_args.push_back(dist);
}

te::da::ST_Buffer::ST_Buffer(const PropertyName& prop, const double& d)
  : Function("st_buffer")
{
  LiteralDouble* dist = new LiteralDouble(d);

  m_args.push_back(prop.clone());
  m_args.push_back(dist);
}

te::da::ST_Buffer::ST_Buffer(const te::gm::Geometry& g, const PropertyName& propDist)
  : Function("st_buffer")
{
  LiteralGeom* geom = new LiteralGeom(g);

  m_args.push_back(geom);
  m_args.push_back(propDist.clone());
}

te::da::ST_Buffer::ST_Buffer(const PropertyName& propGeom, const PropertyName& propDist)
  : Function("st_buffer")
{
  m_args.push_back(propGeom.clone());
  m_args.push_back(propDist.clone());
}

te::da::ST_Buffer::ST_Buffer(const Expression& e)
  : Function("st_buffer")
{
  m_args.push_back(e.clone());
}

te::da::ST_Buffer::~ST_Buffer()
{
}

te::da::ST_Buffer& te::da::ST_Buffer::operator=(const ST_Buffer& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_Buffer::clone() const
{
  return new ST_Buffer(*this);
}
