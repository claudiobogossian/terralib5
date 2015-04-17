/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/serialization/xml/Serializer.cpp
 
  \brief Support for Property serialization.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../Enums.h"
#include "../../NumericProperty.h"
#include "../../Property.h"
#include "../../SimpleProperty.h"
#include "../../StringProperty.h"
#include "../../Exception.h"
#include "Serializer.h"

// STL
#include <cassert>

#ifdef TERRALIB_MOD_XML_ENABLED
std::auto_ptr<te::dt::Property> te::serialize::xml::ReadProperty(te::xml::Reader& /*reader*/)
{
  //assert(reader.getNodeType() == te::xml::START_ELEMENT);
  //assert(reader.getElementLocalName() == "Property");

  //unsigned int id = reader.getAttrAsUInt32(0);
  //std::string name = reader.getAttr(1);
  //int datatype = reader.getAttrAsInt32(2);

  //reader.next();

  //std::auto_ptr<te::dt::Property> p(0);

  //switch(datatype)
  //{
  //  case te::dt::CHAR_TYPE:
  //  case te::dt::UCHAR_TYPE:
  //  case te::dt::INT16_TYPE:
  //  case te::dt::INT32_TYPE:
  //  case te::dt::INT64_TYPE:
  //  case te::dt::BOOLEAN_TYPE:
  //  case te::dt::FLOAT_TYPE:
  //  case te::dt::DOUBLE_TYPE:
  //    p.reset(new te::dt::SimpleProperty(name, datatype, false, 0, id, 0));
  //  break;

  //  case te::dt::NUMERIC_TYPE:
  //    p.reset(new te::dt::NumericProperty(name, 0, 0, false, 0, id, 0));
  //  break;

  //  case te::dt::STRING_TYPE:
  //    p.reset(new te::dt::StringProperty(name, te::dt::STRING, 0, false, 0, id, 0));
  //  break;

  //  case te::dt::GEOMETRY_TYPE:
  //    p = new te::gm::GeometryProperty(name, 0, te::gm::GeometryType);
  //  break;

  //  default:
  //    throw te::dt::Exception(TE_TR("Data type not supported!"));
  //}

  //assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Property Element
  //reader.next();

  //return p;

  return std::auto_ptr<te::dt::Property>(0);
}

void te::serialize::xml::Save(const te::dt::Property& /*p*/, te::xml::AbstractWriter& /*writer*/)
{
  //writer.writeStartElement("te_dt:Property");

  //writer.writeAttribute("id", p.getId());
  //writer.writeAttribute("name", p.getName());
  //writer.writeAttribute("datatype", p.getType());

  //writer.writeEndElement("te_dt:Property");
}
#endif // TERRALIB_MOD_XML_ENABLED