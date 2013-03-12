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
  \file terralib/serialization/se/SvgParameter.cpp
 
  \brief Support for SvgParameter serialization.
*/

// TerraLib
#include "../../se/SvgParameter.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../fe/Expression.h"
#include "ParameterValue.h"
#include "SvgParameter.h"

// STL
#include <cassert>
#include <memory>

te::se::SvgParameter* te::serialize::ReadSvgParameter(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "SvgParameter");
  assert(reader.hasAttrs());

  std::string name = reader.getAttr("name");
  assert(!name.empty());

  std::auto_ptr<te::se::SvgParameter> svgParam(new te::se::SvgParameter(name));

  reader.next();

  // Expression TODO: (n's expressions?)
  te::se::ParameterValue::Parameter* p = new te::se::ParameterValue::Parameter;

  if(reader.getNodeType() == te::xml::VALUE)
  {
    p->m_mixedData = new std::string(reader.getElementValue());
    reader.next();
  }
  else
  {
    p->m_expression = Expression::getInstance().read(reader);
  }

  svgParam->add(p);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return svgParam.release();
}

void te::serialize::Save(const te::se::SvgParameter* p, te::xml::Writer& writer)
{
  if(p == 0)
    return;

  writer.writeStartElement("se:SvgParameter");

  writer.writeAttribute("name", p->getName());

  Save(static_cast<const te::se::ParameterValue*>(p), writer);

  writer.writeEndElement("se:SvgParameter");
}
