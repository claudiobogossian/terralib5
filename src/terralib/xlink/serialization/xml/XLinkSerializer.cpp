/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/xlink/serialization/xml/XLinkSerializer.cpp

  \brief Data serialization for the XLink module.
*/

// TerraLib
#include "../../../xml/Reader.h"
#include "../../../xml/Writer.h"
#include "../../SimpleLink.h"
#include "XLinkSerializer.h"


// STL
#include <cassert>
#include <memory>

#ifdef TERRALIB_MOD_XML_ENABLED

te::xl::SimpleLink* te::xl::serialize::ReadSimpleLink(te::xml::Reader& reader)
{
  std::auto_ptr<te::xl::SimpleLink> link(new te::xl::SimpleLink);

  std::size_t n = reader.getNumberOfAttrs();
  for(std::size_t i = 0; i < n; ++i)
  {
    std::string attributeName = reader.getAttrLocalName(i);
    std::string attributeValue = reader.getAttr(i);

    if(attributeName == "href")
      link->setHref(attributeValue);

    else if(attributeName == "title")
      link->setTitle(attributeValue);

    else if(attributeName == "role")
      link->setRole(attributeValue);

    else if(attributeName == "arcrole")
      link->setArcRole(attributeValue);

    else if(attributeName == "show")
    {
      if(attributeValue == "embed")
        link->setShow(te::xl::SHOW_EMBED);

      else if(attributeValue == "new")
        link->setShow(te::xl::SHOW_NEW);

      else if(attributeValue == "other")
        link->setShow(te::xl::SHOW_OTHER);
    }

    else if(attributeName == "actuate")
    {
       if(attributeValue == "onLoad")
         link->setActuate(te::xl::ACTUATE_ONLOAD);

      else if(attributeValue == "onRequest")
        link->setActuate(te::xl::ACTUATE_ONREQUEST);

      else if(attributeValue == "other")
        link->setActuate(te::xl::ACTUATE_OTHER);
    }
  }

  return link.release();
}

void te::xl::serialize::Save(const SimpleLink* link, te::xml::Writer& writer)
{
  assert(link);

  if(!link->getHref().empty())
    writer.writeAttribute("xlink:href", link->getHref());

  if(!link->getTitle().empty())
    writer.writeAttribute("xlink:title", link->getTitle());

  if(!link->getRole().empty())
    writer.writeAttribute("xlink:role", link->getRole());

  if(!link->getArcRole().empty())
    writer.writeAttribute("xlink:arcrole", link->getArcRole());

  switch(link->getShow())
  {
    case te::xl::SHOW_NONE:
    break;

    case te::xl::SHOW_EMBED:
      writer.writeAttribute("xlink:show", "embed");
    break;

    case te::xl::SHOW_NEW:
      writer.writeAttribute("xlink:show", "new");
    break;
      
    case te::xl::SHOW_REPLACE:
      writer.writeAttribute("xlink:show", "replace");
      break;     
      
    case te::xl::SHOW_OTHER:
      writer.writeAttribute("xlink:show", "other");
  }

  switch(link->getActuate())
  {
    case te::xl::ACTUATE_NONE:
    break;

    case te::xl::ACTUATE_ONLOAD:
      writer.writeAttribute("xlink:actuate", "onLoad");
    break;

    case te::xl::ACTUATE_ONREQUEST:
      writer.writeAttribute("xlink:actuate", "onRequest");
    break;

    case te::xl::ACTUATE_OTHER:
      writer.writeAttribute("xlink:actuate", "other");
  }
}

#endif // TERRALIB_MOD_XML_ENABLED
