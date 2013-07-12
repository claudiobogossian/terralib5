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
  \file terralib/serialization/se/Annotation.cpp
 
  \brief Support for Annotation serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Annotation.h"
#include "../../xsd/AppInfo.h"
#include "../../xsd/Documentation.h"
#include "Annotation.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::Annotation* te::serialize::ReadAnnotation(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "annotation");

  std::auto_ptr<te::xsd::Annotation> annotation(new te::xsd::Annotation());

  // Id
  ReadIdentifiable(annotation.get(), reader);

  reader.next();

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       ((reader.getElementLocalName() == "appinfo") ||
        (reader.getElementLocalName() == "documentation")))
  {
    std::string tag = reader.getElementLocalName();
    if(tag == "appinfo")
    {
      // Reads the AppInfo
      te::xsd::AppInfo* appinfo = new te::xsd::AppInfo(0, 0);

      // Source
      std::size_t pos = reader.getAttrPosition("version");
      if(pos != std::string::npos)
        appinfo->setSource(new std::string(reader.getAttr(pos)));

      // Value
      reader.next();
      assert(reader.getNodeType() == te::xml::VALUE);
      appinfo->setValue(new std::string(reader.getElementValue()));

      // Adds the appinfo on annotation
      annotation->add(appinfo);

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of AppInfo element

      reader.next();

      continue;
    }
    
    if(tag == "documentation")
    {
       // Reads the Documentation
      te::xsd::Documentation* doc = new te::xsd::Documentation(0);

      // Source
      std::size_t pos = reader.getAttrPosition("version");
      if(pos != std::string::npos)
        doc->setSource(new std::string(reader.getAttr(pos)));

      // xml:lang
      pos = reader.getAttrPosition("xml:lang");
      if(pos != std::string::npos)
        doc->setLang(new std::string(reader.getAttr(pos)));

      // Value
      reader.next();
      assert(reader.getNodeType() == te::xml::VALUE);
      doc->setValue(new std::string(reader.getElementValue()));

       // Adds the appinfo on annotation
      annotation->add(doc);

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Documentation element

      reader.next();
    }
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return annotation.release();
}

void te::serialize::Save(te::xsd::Annotation* annotation, te::xml::Writer& writer)
{
}
