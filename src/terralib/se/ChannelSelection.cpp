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
  \file terralib/se/ChannelSelection.cpp
  
  \brief ChannelSelection specifies the false-color channel selection for a multi-spectral raster source (such as a multi-band satellite-imagery source).
*/

// TerraLib
#include "ChannelSelection.h"
#include "SelectedChannel.h"

te::se::ChannelSelection::ChannelSelection()
  : m_redChannel(0),
    m_greenChannel(0),
    m_blueChannel(0),
    m_grayChannel(0),
    m_colorCompType(UNKNOWN_COMPOSITION)
{
}

te::se::ChannelSelection::~ChannelSelection()
{
  delete m_redChannel;
  delete m_greenChannel;
  delete m_blueChannel;
  delete m_grayChannel;
}

void te::se::ChannelSelection::setRedChannel(SelectedChannel* c)
{
  delete m_redChannel;
  m_redChannel = c;
}

void te::se::ChannelSelection::setGreenChannel(SelectedChannel* c)
{
  delete m_greenChannel;
  m_greenChannel = c;
}

void te::se::ChannelSelection::setBlueChannel(SelectedChannel* c)
{
  delete m_blueChannel;
  m_blueChannel = c;
}

void te::se::ChannelSelection::setGrayChannel(SelectedChannel* c)
{
  delete m_grayChannel;
  m_grayChannel = c;
}

void te::se::ChannelSelection::setColorCompositionType(ColorCompositionType cct)
{
  m_colorCompType = cct;
}

te::se::ChannelSelection* te::se::ChannelSelection::clone() const
{
  ChannelSelection* cs = new ChannelSelection;

  cs->setColorCompositionType(m_colorCompType);

  if(m_redChannel)
    cs->setRedChannel(m_redChannel->clone());

  if(m_greenChannel)
    cs->setGreenChannel(m_greenChannel->clone());

  if(m_blueChannel)
    cs->setBlueChannel(m_blueChannel->clone());

  if(m_grayChannel)
    cs->setGrayChannel(m_grayChannel->clone());

  return cs;
}
