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
  \file terralib/se/Graphic.cpp
  
  \brief A Graphic is a graphic symbol with an inherent shape, color(s), and possibly size.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "AnchorPoint.h"
#include "Displacement.h"
#include "ExternalGraphic.h"
#include "Graphic.h"
#include "Mark.h"
#include "ParameterValue.h"

// STL
#include <cassert>

te::se::Graphic::Graphic()
  : m_opacity(0),
    m_size(0),
    m_rotation(0),
    m_anchorPoint(0),
    m_displacement(0)
{
}

te::se::Graphic::~Graphic()
{
  te::common::FreeContents(m_externalGraphics);
  te::common::FreeContents(m_marks);

  delete m_opacity;
  delete m_size;
  delete m_rotation;
  delete m_anchorPoint;
  delete m_displacement;
}

void te::se::Graphic::add(ExternalGraphic* g)
{
  assert(g);
  m_externalGraphics.push_back(g);
}

void te::se::Graphic::setExternalGraphic(std::size_t index, ExternalGraphic* g)
{
  assert(index < m_externalGraphics.size());

  assert(g);

  delete m_externalGraphics[index];

  m_externalGraphics[index] = g;
}

const std::vector<te::se::ExternalGraphic*> te::se::Graphic::getExternalGraphics() const
{
  return m_externalGraphics;
}

void te::se::Graphic::add(Mark* m)
{
  assert(m);
  m_marks.push_back(m);
}

void te::se::Graphic::setMark(std::size_t index, Mark* m)
{
  assert(index < m_marks.size());

  assert(m);

  delete m_marks[index];

  m_marks[index] = m;
}

const std::vector<te::se::Mark*> te::se::Graphic::getMarks() const
{
  return m_marks;
}

void te::se::Graphic::setOpacity(ParameterValue* value)
{
  delete m_opacity;
  m_opacity = value;
}

const te::se::ParameterValue* te::se::Graphic::getOpacity() const
{
  return m_opacity;
}

void te::se::Graphic::setSize(ParameterValue* value)
{
  delete m_size;
  m_size = value;
}

const te::se::ParameterValue* te::se::Graphic::getSize() const
{
  return m_size;
}

void te::se::Graphic::setRotation(ParameterValue* value)
{
  delete m_rotation;
  m_rotation = value;
}

const te::se::ParameterValue* te::se::Graphic::getRotation() const
{
  return m_rotation;
}

void te::se::Graphic::setAnchorPoint(AnchorPoint* value)
{
  delete m_anchorPoint;
  m_anchorPoint = value;
}

const te::se::AnchorPoint* te::se::Graphic::getAnchorPoint() const
{
  return m_anchorPoint;
}

void te::se::Graphic::setDisplacement(Displacement* value)
{
  delete m_displacement;
  m_displacement = value;
}

const te::se::Displacement* te::se::Graphic::getDisplacement() const
{
  return m_displacement;
}

void te::se::Graphic::clear()
{
  te::common::FreeContents(m_externalGraphics);
  m_externalGraphics.clear();

  te::common::FreeContents(m_marks);
  m_marks.clear();
}

te::se::Graphic* te::se::Graphic::clone() const
{
  Graphic* graphic = new Graphic;

  for(std::size_t i = 0; i < m_externalGraphics.size(); ++i)
  {
    const ExternalGraphic* eg = m_externalGraphics[i];
    if(eg)
      graphic->add(eg->clone());
  }

  for(std::size_t i = 0; i < m_marks.size(); ++i)
  {
    const Mark* m = m_marks[i];
    if(m)
      graphic->add(m->clone());
  }

  if(m_opacity)
    graphic->setOpacity(new ParameterValue(*m_opacity));
  
  if(m_size)
    graphic->setSize(new ParameterValue(*m_size));

  if(m_rotation)
    graphic->setRotation(new ParameterValue(*m_rotation));

  if(m_anchorPoint)
    graphic->setAnchorPoint(m_anchorPoint->clone());

  if(m_displacement)
    graphic->setDisplacement(m_displacement->clone());

  return graphic;
}
