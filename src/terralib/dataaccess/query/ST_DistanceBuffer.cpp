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
  \file terralib/dataaccess/query/ST_DistanceBuffer.cpp

  \brief Spatial distance buffer operator.
*/

// TerraLib
#include "../../common/Distance.h"
#include "../../geometry/Geometry.h"
#include "ST_DistanceBuffer.h"

te::da::ST_DistanceBuffer::ST_DistanceBuffer(const std::string& name,
                                             te::da::Expression* e, 
                                             te::gm::Geometry* g,
                                             te::common::Distance* d)
  : Function(name),
    m_geom(g),
    m_d(d)
{
  m_args.push_back(e);
}

te::da::ST_DistanceBuffer::ST_DistanceBuffer(const std::string& name,
                                             const te::da::Expression& e, 
                                             const te::gm::Geometry& g,
                                             const te::common::Distance& d)
  : Function(name),
    m_geom(0),
    m_d(0)
{
  m_args.push_back(e.clone());
  m_geom.reset(static_cast<te::gm::Geometry*>(g.clone()));
  m_d.reset(new te::common::Distance(d));
}

te::da::ST_DistanceBuffer::ST_DistanceBuffer(const ST_DistanceBuffer& rhs)
  : Function(rhs),
    m_geom(0),
    m_d(0)
{
  m_geom.reset(rhs.m_geom.get() ? static_cast<te::gm::Geometry*>(rhs.m_geom->clone()) : 0);
  m_d.reset(rhs.m_d.get() ? new te::common::Distance(*rhs.m_d) : 0);
}

te::da::ST_DistanceBuffer::~ST_DistanceBuffer()
{
}

te::da::ST_DistanceBuffer& te::da::ST_DistanceBuffer::operator=(const ST_DistanceBuffer& rhs)
{
  if(this != &rhs)
  {
    Function::operator=(rhs);

    m_geom.reset(rhs.m_geom.get() ? static_cast<te::gm::Geometry*>(rhs.m_geom->clone()) : 0);
    m_d.reset(rhs.m_d.get() ? new te::common::Distance(*rhs.m_d) : 0);
  }

  return *this;
}

te::da::Expression* te::da::ST_DistanceBuffer::clone() const
{
  return new ST_DistanceBuffer(*this);
}

void te::da::ST_DistanceBuffer::setGeometry(te::gm::Geometry* g)
{
  m_geom.reset(g);
}

te::gm::Geometry* te::da::ST_DistanceBuffer::getGeometry() const
{
  return m_geom.get();
}

void te::da::ST_DistanceBuffer::setDistance(te::common::Distance* d)
{
  m_d.reset(d);
}

te::common::Distance* te::da::ST_DistanceBuffer::getDistance() const
{
  return m_d.get();
}
