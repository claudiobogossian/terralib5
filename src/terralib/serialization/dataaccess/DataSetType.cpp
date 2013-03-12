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
  \file terralib/serialization/dataaccess/DataSetType.cpp
 
  \brief Support for DataSetType serialization.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../datatype/Property.h"
#include "DataSetType.h"

// STL
#include <cassert>
#include <memory>

te::da::DataSetType* te::serialize::ReadDataSetType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetType");

  unsigned int id = reader.getAttrAsUInt32(0);
  std::string name = reader.getAttr(1);
  std::string title = reader.getAttr(2);

  reader.next();

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(name, id));

  dt->setTitle(title);

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "Property")
  {
    te::dt::Property* p = ReadProperty(reader);
    dt->add(p);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSetType Element
  reader.next();

  return dt.release();
}

void te::serialize::Save(const te::da::DataSetType* dt, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:DataSetType");

  writer.writeAttribute("id", dt->getId());
  writer.writeAttribute("name", dt->getName());
  writer.writeAttribute("title", dt->getTitle());

  const std::size_t nprops = dt->size();

  for(std::size_t i = 0; i < nprops; ++i)
    Save(dt->getProperty(i), writer);

  writer.writeEndElement("te_da:DataSetType");
}

