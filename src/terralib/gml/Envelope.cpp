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
  \file Envelope.cpp

  \brief GML Envelope class implementation.
 */

// TerraLib
#include "../geometry/Envelope.h"
#include "Envelope.h"

te::gml::Envelope::Envelope()
  : m_envelope(0),
    m_srid(-1)
{
}

te::gml::Envelope::Envelope(te::gm::Envelope* e, int srid)
  : m_envelope(e),
    m_srid(srid)
{
}

te::gml::Envelope::~Envelope()
{
  delete m_envelope;
}

te::gm::Envelope* te::gml::Envelope::getCoordinates() const
{
  return m_envelope;
}

void te::gml::Envelope::setCoordinates(const double& llx, const double& lly, const double& urx, const double& ury)
{
  if(m_envelope == 0)
    m_envelope = new te::gm::Envelope(llx, lly, urx, ury);
  else
    m_envelope->init(llx, lly, urx, ury);
}

int te::gml::Envelope::getSRID() const
{
  return m_srid;
}

void te::gml::Envelope::setSRID(int srid)
{
  m_srid = srid;
}

