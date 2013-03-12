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
  \file terralib/serialization/dataaccess/DataSource.cpp

  \brief Auxiliary classes and functions to read SQL dialect information from a XML document.
*/

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../xml/Writer.h"
#include "../Exception.h"
#include "DataSourceInfo.h"

// STL
#include <cassert>
#include <memory>
#include <fstream>

// Boost
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

void te::serialize::ReadDataSourceInfo(const std::string& datasourcesFileName)
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
    te::da::DataSourceInfoPtr ds(ReadDataSourceInfo(*xmlReader));
    te::da::DataSourceInfoManager::getInstance().add(ds);
  }

  return;
}

te::da::DataSourceInfo* te::serialize::ReadDataSourceInfo(te::xml::Reader& reader)
{
  std::auto_ptr<te::da::DataSourceInfo> ds(new te::da::DataSourceInfo);

  ds->setId(reader.getAttr(0));
  ds->setType(reader.getAttr(1));
  ds->setAccessDriver(reader.getAttr(2));

  /* Title Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Title");
  reader.next();
  assert(reader.getNodeType() == te::xml::VALUE);
  ds->setTitle(reader.getElementValue());
  reader.next();
  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* Description Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Description");
  reader.next();

  if(reader.getNodeType() == te::xml::VALUE)
  {
    ds->setDescription(reader.getElementValue());
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  /* ConnectionInfo Element */
  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "ConnectionInfo");

  std::map<std::string, std::string> conninfo;

  while(reader.next() &&
        (reader.getNodeType() == te::xml::START_ELEMENT) &&
        (reader.getElementLocalName() == "Parameter"))
  {
    // Parameter Name
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Name");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramName = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    // Parameter Value
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);
    assert(reader.getElementLocalName() == "Value");
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    std::string paramValue = reader.getElementValue();
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    conninfo[paramName] = paramValue;

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of ConnectionInfo Element
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSource Element
  reader.next();

  ds->setConnInfo(conninfo);

  return ds.release();
}

void te::serialize::Save(const std::string& fileName)
{
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(ostr);

  ostr.close();
}

void te::serialize::Save(std::ostream& ostr)
{
  te::xml::Writer w(ostr);

  Save(w);
}

void te::serialize::Save(te::xml::Writer& writer)
{
  boost::filesystem::path p(TE_SCHEMA_LOCATION);


  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("te_da:DataSourceList");

  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns:te_common", "http://www.terralib.org/schemas/common");
  writer.writeAttribute("xmlns:te_da", "http://www.terralib.org/schemas/dataaccess");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/dataaccess");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/dataaccess " + p.generic_string() + "/dataaccess/dataaccess.xsd");
  writer.writeAttribute("version", TERRALIB_STRING_VERSION);
  writer.writeAttribute("release", "2013-01-01");

  te::da::DataSourceInfoManager::iterator itBegin = te::da::DataSourceInfoManager::getInstance().begin();
  te::da::DataSourceInfoManager::iterator itEnd = te::da::DataSourceInfoManager::getInstance().end();
  te::da::DataSourceInfoManager::iterator it;

  for(it=itBegin; it!=itEnd; ++it)
  {
    writer.writeStartElement("te_da:DataSource");

    writer.writeAttribute("id", it->second->getId());
    writer.writeAttribute("type", it->second->getType());
    writer.writeAttribute("access_driver", it->second->getAccessDriver());

    writer.writeStartElement("te_da:Title");
    writer.writeValue(it->second->getTitle());
    writer.writeEndElement("te_da:Title");

    writer.writeStartElement("te_da:Description");
    writer.writeValue(it->second->getDescription());
    writer.writeEndElement("te_da:Description");

    writer.writeStartElement("te_da:ConnectionInfo");
    std::map<std::string, std::string> info = it->second->getConnInfo();
    std::map<std::string, std::string>::iterator conIt;

    for(conIt=info.begin(); conIt!=info.end(); ++conIt)
    {
      writer.writeStartElement("te_common:Parameter");

      writer.writeStartElement("te_common:Name");
      writer.writeValue(conIt->first);
      writer.writeEndElement("te_common:Name");

      writer.writeStartElement("te_common:Value");
      writer.writeValue(conIt->second);
      writer.writeEndElement("te_common:Value");

      writer.writeEndElement("te_common:Parameter");
    }
    writer.writeEndElement("te_da:ConnectionInfo");

    writer.writeEndElement("te_da:DataSource");
  }

  writer.writeEndElement("te_da:DataSourceList");
}

