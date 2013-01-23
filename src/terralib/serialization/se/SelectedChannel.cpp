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
  \file terralib/serialization/se/SelectedChannel.cpp
 
  \brief Support for SelectedChannel serialization.
*/

// TerraLib
#include "../../se/SelectedChannel.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "ContrastEnhancement.h"
#include "SelectedChannel.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::SelectedChannel* te::serialize::ReadSelectedChannel(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GrayChannel" || reader.getElementLocalName() == "RedChannel" ||
         reader.getElementLocalName() == "GreenChannel" || reader.getElementLocalName() == "BlueChannel");

  reader.next();

  std::auto_ptr<te::se::SelectedChannel> sc(new te::se::SelectedChannel);

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SourceChannelName");
  
  // SourceChannelName
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  sc->setSourceChannelName(reader.getElementValue());
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  // ContrastEnhancement
  if(reader.getElementLocalName() == "ContrastEnhancement")
    sc->setContrastEnhancement(ReadContrastEnhancement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sc.release();
}

void te::serialize::Save(const te::se::SelectedChannel* sc, te::xml::Writer& writer)
{
  if(sc == 0)
    return;

  assert(!sc->getSourceChannelName().empty());
  writer.writeElement("SourceChannelName", sc->getSourceChannelName());
  
  Save(sc->getContrastEnhancement(), writer);
}
