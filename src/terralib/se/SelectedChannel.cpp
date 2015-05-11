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
  \file terralib/se/SelectedChannel.cpp
  
  \brief A selected channel to be display.
*/

// TerraLib
#include "ContrastEnhancement.h"
#include "SelectedChannel.h"

te::se::SelectedChannel::SelectedChannel()
  : m_contrastEnhancement(0)
{
}

te::se::SelectedChannel::~SelectedChannel()
{
  delete m_contrastEnhancement;
}

void te::se::SelectedChannel::setSourceChannelName(const std::string& name)
{
  m_sourceChannelName = name;
}

void te::se::SelectedChannel::setContrastEnhancement(ContrastEnhancement* c)
{
  delete m_contrastEnhancement;
  m_contrastEnhancement = c;
}

te::se::SelectedChannel* te::se::SelectedChannel::clone() const
{
  SelectedChannel* sc = new SelectedChannel;
  sc->setSourceChannelName(m_sourceChannelName);

  if(m_contrastEnhancement)
    sc->setContrastEnhancement(m_contrastEnhancement->clone());

  return sc;
}
