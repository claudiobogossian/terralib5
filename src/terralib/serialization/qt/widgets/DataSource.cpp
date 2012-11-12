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
  \file DataSource.cpp
   
  \brief Auxiliary classes and functions to read SQL dialect information from a XML document.
*/

// TerraLib
#include "../../../qt/widgets/datasource/core/DataSource.h"
#include "../../../qt/widgets/datasource/core/DataSourceManager.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../Exception.h"
#include "DataSource.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>

void te::serialize::ReadDataSource(const std::string& datasourcesFileName)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(datasourcesFileName);

  if(!xmlReader->next())
    throw Exception((boost::format(TR_SERIALIZATION("Could not read data source information in file: %1%.")) % datasourcesFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_SERIALIZATION("Error reading the document %1%, the start element wasn't found.")) % datasourcesFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceList")
    throw Exception((boost::format(TR_SERIALIZATION("The first tag in the document %1% is not 'DataSourceList'.")) % datasourcesFileName).str());

  if(!xmlReader->next())
    return;

  while((xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "DataSource"))
  {
    te::qt::widgets::DataSourcePtr ds(ReadDataSource(*xmlReader));

    te::qt::widgets::DataSourceManager::getInstance().add(ds);
  }

  return;
}

te::qt::widgets::DataSource* te::serialize::ReadDataSource(te::xml::Reader& reader)
{
  std::auto_ptr<te::qt::widgets::DataSource> ds(new te::qt::widgets::DataSource);

  ds->setId(reader.getAttr(0));
  ds->setType(reader.getAttr(1));  
  ds->setAccessDriver(reader.getAttr(2));

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  ds->setTitle(reader.getElementValue());

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Description");

  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  ds->setDescription(reader.getElementValue());

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ConnectionInfo");

  std::map<std::string, std::string> conninfo;

  while(reader.next() &&
        (reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName() == "Parameter"))
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Name");

    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramName = reader.getElementValue();

    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Value");

    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramValue = reader.getElementValue();

    conninfo[paramName] = paramValue;
  }

  ds->setConnInfo(conninfo);
  
  return ds.release();
}

