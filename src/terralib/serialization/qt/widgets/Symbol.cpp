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
  \file terralib/serialization/qt/Symbol.h
   
  \brief Support for SymbolLibrary serialization.
*/

// TerraLib
#include "../../../qt/widgets/se/Symbol.h"
#include "../../../qt/widgets/se/SymbolLibrary.h"
#include "../../../qt/widgets/se/SymbolLibraryManager.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../Exception.h"
#include "../../se/Symbolizer.h"
#include "Symbol.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>

void te::serialize::ReadSymbolLibrary(const std::string& symbolLibraryFileName)
{
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->read(symbolLibraryFileName);

  if(!reader->next())
    throw Exception((boost::format(TR_SERIALIZATION("Could not read symbol library information in file: %1%.")) % symbolLibraryFileName).str());

  if(reader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_SERIALIZATION("Error reading the document %1%, the start element wasn't found.")) % symbolLibraryFileName).str());

  if(reader->getElementLocalName() != "SymbolLibrary")
    throw Exception((boost::format(TR_SERIALIZATION("The first tag in the document %1% is not 'SymbolLibrary'.")) % symbolLibraryFileName).str());

  std::string name = reader->getAttr("name");

  std::auto_ptr<te::qt::widgets::SymbolLibrary> library(new te::qt::widgets::SymbolLibrary(name));

  reader->next();

  while((reader->getNodeType() == te::xml::START_ELEMENT) &&
        (reader->getElementLocalName() == "Symbol"))
  {
    te::qt::widgets::Symbol* symbol = ReadSymbol(*reader);
    library->insert(symbol);
  }

  te::qt::widgets::SymbolLibraryManager::getInstance().insert(library.release());

  assert(reader->getNodeType() == te::xml::END_DOCUMENT);
}

te::qt::widgets::Symbol* te::serialize::ReadSymbol(te::xml::Reader& reader)
{
  std::auto_ptr<te::qt::widgets::Symbol> symbol(new te::qt::widgets::Symbol);

  te::qt::widgets::SymbolInfo info;

  // Symbol Id
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Id");
  reader.next();
  info.m_id = reader.getElementValue();
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  // Symbol Name
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Name");
  reader.next();
  info.m_name = reader.getElementValue();
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  // Symbol Author
  if(reader.getElementLocalName() == "Author")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_author = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Symbol Tags
  if(reader.getElementLocalName() == "Tags")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_tags = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  // Symbol Description
  if(reader.getElementLocalName() == "Description")
  {
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    reader.next();
    info.m_description = reader.getElementValue();
    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }

  symbol->setInfo(info);

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName().find("Symbolizer") != std::string::npos);

  while((reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName().find("Symbolizer") != std::string::npos))
    symbol->addSymbolizer(te::serialize::Symbolizer::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return symbol.release();
}
