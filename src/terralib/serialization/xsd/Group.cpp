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
  \file terralib/serialization/se/Group.cpp
 
  \brief Support for Group serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/All.h"
#include "../../xsd/Choice.h"
#include "../../xsd/Group.h"
#include "../../xsd/Sequence.h"
#include "All.h"
#include "Choice.h"
#include "Group.h"
#include "Sequence.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::xsd::Group* te::serialize::ReadGroup(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "group");

  std::auto_ptr<te::xsd::Group> group(new te::xsd::Group);

  // Id
  ReadIdentifiable(group.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    group->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    group->setRef(CreateQName(reader.getAttr(pos)));

  // MinOccurs and MaxOccurs
  ReadOccurs(group.get(), reader);

  reader.next();

  // Grammar: (annotation?,(all|choice|sequence)?)

  // Annotation
  ReadAnnotated(group.get(), reader);

  if(reader.getElementLocalName() == "all")
    group->setContent(ReadAll(reader));
  else if(reader.getElementLocalName() == "choice")
    group->setContent(ReadChoice(reader));
  else if(reader.getElementLocalName() == "sequence")
    group->setContent(ReadSequence(reader));
  else throw; // TODO: Add an exception here...

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return group.release();
}

void te::serialize::Save(te::xsd::Group* group, te::xml::Writer& writer)
{
}
