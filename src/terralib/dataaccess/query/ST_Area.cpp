/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/ST_Area.cpp

  \brief Spatial area operator.
*/

// TerraLib
#include "../../geometry/Geometry.h"
#include "LiteralGeom.h"
#include "LiteralBool.h"
#include "PropertyName.h"
#include "ST_Area.h"

te::da::ST_Area::ST_Area(const te::gm::Geometry& g)
  : Function("st_area")
{
  LiteralGeom* geom = new LiteralGeom(g);
  m_args.push_back(geom);
}

te::da::ST_Area::ST_Area(const PropertyName& prop)
  : Function("st_area")
{
  m_args.push_back(prop.clone());
}

te::da::ST_Area::ST_Area(const te::gm::Geometry& g, const bool& p)
  : Function("st_area")
{
  LiteralGeom* geom = new LiteralGeom(g);
  LiteralBool* precision = new LiteralBool(p);

  m_args.push_back(geom);
  m_args.push_back(precision);

}

te::da::ST_Area::ST_Area(const PropertyName& prop, const bool& p)
  : Function("st_area")
{
  LiteralBool* precision = new LiteralBool(p);

  m_args.push_back(prop.clone());
  m_args.push_back(precision);
}

te::da::ST_Area::ST_Area(const Expression& e)
  : Function("st_area")
{
  m_args.push_back(e.clone());
}

te::da::ST_Area::ST_Area(const Expression& e, const bool& p)
  : Function("st_area")
{
  LiteralBool* precision = new LiteralBool(p);

  m_args.push_back(e.clone());
  m_args.push_back(precision);
}

te::da::ST_Area::~ST_Area()
{
}

te::da::ST_Area& te::da::ST_Area::operator=(const ST_Area& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_Area::clone() const
{
  return new ST_Area(*this);
}

