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
  \file terralib/dataaccess/query/LiteralEnvelope.cpp

  \brief A class that models a literal for Envelope values.
*/

// TerraLib
#include "../../geometry/Envelope.h"
#include "LiteralEnvelope.h"

te::da::LiteralEnvelope::LiteralEnvelope(te::gm::Envelope* e, int srid)
  : m_eval(e),
    m_srid(srid)
{
}

te::da::LiteralEnvelope::LiteralEnvelope(const te::gm::Envelope& e, int srid)
  : m_eval(new te::gm::Envelope(e)),
    m_srid(srid)
{
}

te::da::LiteralEnvelope::LiteralEnvelope(const LiteralEnvelope& rhs)
  : m_eval(0),
    m_srid(rhs.m_srid)
{
  m_eval.reset(rhs.m_eval.get() ? new te::gm::Envelope(*rhs.m_eval) : 0);
}

te::da::LiteralEnvelope::~LiteralEnvelope()
{
}

te::da::LiteralEnvelope& te::da::LiteralEnvelope::operator=(const LiteralEnvelope& rhs)
{
  if(this != &rhs)
  {
    m_eval.reset(rhs.m_eval.get() ? new te::gm::Envelope(*rhs.m_eval) : 0);

    m_srid = rhs.m_srid;
  }

  return *this;
}

te::da::Expression* te::da::LiteralEnvelope::clone() const
{
  return new LiteralEnvelope(*this);
}

te::gm::Envelope* te::da::LiteralEnvelope::getValue() const
{
  return m_eval.get();
}

void te::da::LiteralEnvelope::setValue(te::gm::Envelope* e)
{
  m_eval.reset(e);
}

