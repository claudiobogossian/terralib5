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
  \file terralib/geometry/AbstractPoint.cpp

  \brief A base abstract class for 0-dimensional geometric objects that represents a single location in coordinate space.
*/

// TerraLib
#include "../Defines.h"
#include "../common/Translator.h"
#include "../srs/Converter.h"
#include "AbstractPoint.h"
#include "Envelope.h"
#include "Exception.h"

// STL
#include <memory>

const std::string te::gm::AbstractPoint::sm_typeName("Point");

const double te::gm::AbstractPoint::sm_notNumber(TE_DOUBLE_NOT_A_NUMBER);

te::gm::AbstractPoint::AbstractPoint(GeomType t, int srid, Envelope* mbr)
  : Geometry(t, srid, mbr)
{
}

te::gm::AbstractPoint::AbstractPoint(const AbstractPoint& rhs)
  : Geometry(rhs)
{
}

te::gm::AbstractPoint& te::gm::AbstractPoint::operator=(const AbstractPoint& rhs)
{
  Geometry::operator=(rhs);

  return *this;
}

te::gm::Dimensionality te::gm::AbstractPoint::getDimension() const throw()
{
  return te::gm::P;
}

const std::string& te::gm::AbstractPoint::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::AbstractPoint::setSRID(int srid) throw()
{
  m_srid = srid;
}

void te::gm::AbstractPoint::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

  converter->setSourceSRID(m_srid);
  converter->setTargetSRID(srid);

  double x = getX();
  double y = getY();

  converter->convert(x,y);

  setX(x);
  setY(y);

  m_srid = srid;

  if(m_mbr)
    computeMBR(false);
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::AbstractPoint::computeMBR(bool /*cascade*/) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope(getX(), getY(), getX(), getY());
  else
    m_mbr->init(getX(), getY(), getX(), getY());
}




