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
  \file BBOXOp.cpp
  
  \brief A convenient and more compact way of encoding the very common bounding box constraint based on an envelope.
 */

// TerraLib
#include "../gml/Envelope.h"
#include "BBOXOp.h"
#include "Globals.h"
#include "PropertyName.h"

te::fe::BBOXOp::BBOXOp()
  : SpatialOp(Globals::sm_bbox),
    m_property(0),
    m_envelope(0)
{
}

te::fe::BBOXOp::~BBOXOp()
{
  delete m_property;
  delete m_envelope;
}

void te::fe::BBOXOp::setProperty(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

te::fe::PropertyName* te::fe::BBOXOp::getProperty() const
{
  return m_property;
}

void te::fe::BBOXOp::setEnvelope(te::gml::Envelope* e)
{
  delete m_envelope;
  m_envelope = e;
}

te::gml::Envelope* te::fe::BBOXOp::getEnvelope() const
{
  return m_envelope;
}

