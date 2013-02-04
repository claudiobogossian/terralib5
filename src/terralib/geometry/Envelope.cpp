/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/Envelope.cpp

  \brief An Envelope defines a 2D rectangular region.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../srs/Converter.h"
#include "Coord2D.h"
#include "Envelope.h"

// STL
#include <cassert>
#include <cmath>
#include <memory>

te::gm::Coord2D te::gm::Envelope::getLowerLeft() const
{
  return Coord2D(m_llx, m_lly);
}

te::gm::Coord2D te::gm::Envelope::getUpperRight() const
{
  return Coord2D(m_urx, m_ury);
}

te::gm::Coord2D te::gm::Envelope::getCenter() const
{
  return Coord2D(m_llx + (m_urx - m_llx) / 2.0, m_lly + (m_ury - m_lly) / 2.0);
}

double te::gm::Envelope::distance(const Envelope& rhs) const
{
  if(intersects(rhs))
    return 0.0;

  double dx = 0.0;

// is this envelope to the left?
  if(m_urx < rhs.m_llx)
    dx = rhs.m_llx - m_urx;

// is this envelope to the right?
  if(m_llx > rhs.m_urx)
    dx = m_llx - rhs.m_urx;

  double dy = 0.0;

// is this envelope below?
  if(m_ury < rhs.m_lly)
    dy = rhs.m_lly - m_ury;

// is this envelope above?
  if(m_lly > rhs.m_ury)
    dy = m_lly - rhs.m_ury;

// are the envelopes aligned horizontally?
  if(dx == 0.0)
    return dy;
  
// are the envelopes aligned vertically?
  if(dy == 0.0)
    return dx;

  return sqrt((dx * dx) + (dy * dy));
}

void te::gm::Envelope::transform(int oldsrid, int newsrid)
{
  if(oldsrid == newsrid)
    return;

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

  converter->setSourceSRID(oldsrid);
  converter->setTargetSRID(newsrid);

  converter->convert(m_llx, m_lly);
  converter->convert(m_urx, m_ury);
}

