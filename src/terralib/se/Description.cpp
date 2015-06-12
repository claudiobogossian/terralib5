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
  \file terralib/se/Description.cpp
  
  \brief A Description gives human-readable descriptive information for the object it is included within.
*/

// TerraLib
#include "Description.h"

te::se::Description::Description()
{
}

te::se::Description::~Description()
{
}

void te::se::Description::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& te::se::Description::getTitle() const
{
  return m_title;
}

void te::se::Description::setAbstract(const std::string& a)
{
  m_abstract = a;
}

const std::string& te::se::Description::getAbstract() const
{
  return m_abstract;
}

te::se::Description* te::se::Description::clone() const
{
  Description* d = new Description;
  d->setTitle(m_title);
  d->setAbstract(m_abstract);

  return d;
}
