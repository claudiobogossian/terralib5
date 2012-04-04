/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file FolderLayer.cpp

  \brief A FolderLayer can be used as a container for other kind of layers.
 */

// TerraLib
#include "../geometry/Envelope.h"
#include "FolderLayer.h"

const std::string te::map::FolderLayer::sm_type("FOLDERLAYER");

te::map::FolderLayer::FolderLayer(const std::string& id,
                                  const std::string& title,
                                  AbstractLayer* parent)
  : te::map::AbstractLayer(id, title, parent),
    m_mbr(0),
    m_srid(-1)
{}

te::map::FolderLayer::~FolderLayer()
{
  delete m_mbr;
}

const std::string& te::map::FolderLayer::getType() const
{
  return sm_type;
}

bool te::map::FolderLayer::isValid() const
{
  return true;
}

bool te::map::FolderLayer::isQueryable() const
{
  return false;
}

void te::map::FolderLayer::draw(Canvas* /*canvas*/, const te::gm::Envelope& /*bbox*/, int /*srid*/)
{}

te::se::Style* te::map::FolderLayer::getStyle() const
{
  return 0;
}

void te::map::FolderLayer::setStyle(te::se::Style* /*style*/)
{}

const te::gm::Envelope* te::map::FolderLayer::getExtent() const
{
  return m_mbr;
}

void te::map::FolderLayer::setExtent(te::gm::Envelope* mbr)
{
  delete m_mbr;
  m_mbr = mbr;
}

int te::map::FolderLayer::getSRID() const
{
  return m_srid;
}

void te::map::FolderLayer::setSRID(int srid)
{
  m_srid = srid;
}
