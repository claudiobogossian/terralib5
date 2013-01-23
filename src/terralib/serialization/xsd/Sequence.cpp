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
  \file terralib/serialization/se/Sequence.cpp
 
  \brief Support for Sequence serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Choice.h"
#include "../../xsd/Group.h"
#include "../../xsd/Sequence.h"
#include "Any.h"
#include "Choice.h"
#include "Element.h"
#include "Group.h"
#include "Sequence.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Sequence* te::serialize::ReadSequence(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "sequence");

  std::auto_ptr<te::xsd::Sequence> sequence(new te::xsd::Sequence);

  // Id
  ReadIdentifiable(sequence.get(), reader);

  // MinOccurs and MaxOccurs
  ReadOccurs(sequence.get(), reader);

  reader.next();

  // Grammar: (annotation?,(element|group|choice|sequence|any)*)

  // Annotation
  ReadAnnotated(sequence.get(), reader);

  std::set<std::string> children;
  children.insert("element");
  children.insert("group");
  children.insert("choice");
  children.insert("sequence");
  children.insert("any");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "element")
    {
      sequence->addElement(ReadElement(reader));
      continue;
    }

    if(tag == "group")
    {
      sequence->addContent(ReadGroup(reader));
      continue;
    }

    if(tag == "choice")
    {
      sequence->addContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      sequence->addContent(ReadSequence(reader));
      continue;
    }

    if(tag == "any")
      sequence->addAny(ReadAny(reader));
  }

  return sequence.release();
}

void te::serialize::Save(te::xsd::Sequence* sequence, te::xml::Writer& writer)
{
}
