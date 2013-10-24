/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/se/ChannelSelection.cpp
 
  \brief Support for ChannelSelection serialization.
*/

// TerraLib
#include "../../se/ChannelSelection.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "ChannelSelection.h"
#include "SelectedChannel.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::ChannelSelection* te::serialize::ReadChannelSelection(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ChannelSelection");

  reader.next();

  std::auto_ptr<te::se::ChannelSelection> cs(new te::se::ChannelSelection);
  cs->setColorCompositionType(te::se::UNKNOWN_COMPOSITION);

  // GrayChannel
  if(reader.getElementLocalName() == "GrayChannel")
  {
    cs->setGrayChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::GRAY_COMPOSITION);

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();

    return cs.release();
  }

  std::size_t nChannels = 0; // To count the number of channels

  // RedChannel
  if(reader.getElementLocalName() == "RedChannel")
  {
    cs->setRedChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::RED_COMPOSITION);
    nChannels++;
  }

  // GreenChannel
  if(reader.getElementLocalName() == "GreenChannel")
  {
    cs->setGreenChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::GREEN_COMPOSITION);
    nChannels++;
  }

  // BlueChannel
  if(reader.getElementLocalName() == "BlueChannel")
  {
    cs->setBlueChannel(ReadSelectedChannel(reader));
    cs->setColorCompositionType(te::se::BLUE_COMPOSITION);
    nChannels++;
  }

  assert(nChannels > 0);

  // Adjusting...
  if(nChannels > 1)
    nChannels == 3 ? cs->setColorCompositionType(te::se::RGB_COMPOSITION) : cs->setColorCompositionType(te::se::UNKNOWN_COMPOSITION);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cs.release();
}

void te::serialize::Save(const te::se::ChannelSelection* cs, te::xml::Writer& writer)
{
  if(cs == 0)
    return;

  writer.writeStartElement("se:ChannelSelection");

  if(cs->getColorCompositionType() == te::se::GRAY_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:GrayChannel", cs->getGrayChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::RED_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:RedChannel", cs->getRedChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::GREEN_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:GreenChannel", cs->getGreenChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::BLUE_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:BlueChannel", cs->getBlueChannel(), writer);
  }
  else if(cs->getColorCompositionType() == te::se::RGB_COMPOSITION)
  {
    WriteSelectedChannelHelper("se:RedChannel", cs->getRedChannel(), writer);
    WriteSelectedChannelHelper("se:GreenChannel", cs->getGreenChannel(), writer);
    WriteSelectedChannelHelper("se:BlueChannel", cs->getBlueChannel(), writer);
  }

  writer.writeEndElement("se:ChannelSelection");
}
