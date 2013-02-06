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
  \file terralib/serialization/dataaccess/DataSourceCatalog.cpp
 
  \brief Support for DataSourceCatalog serialization.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "DataSetType.h"
#include "DataSourceCatalog.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

te::da::DataSourceCatalog* te::serialize::ReadDataSourceCatalog(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSourceCatalog");

  //reader.next();
  //assert(reader.getNodeType() == te::xml::START_ELEMENT);
  //assert(reader.getElementLocalName() == "Name");

  //reader.next();
  //assert(reader.getNodeType() == te::xml::VALUE);

  reader.next();
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetTypes");

  reader.next();

  std::auto_ptr<te::da::DataSourceCatalog> catalog(new te::da::DataSourceCatalog);

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "DataSetType")
  {
    te::da::DataSetTypePtr dt(ReadDataSetType(reader));
    catalog->add(dt);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSetTypes Element
  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSourceCatalog Element
  reader.next();

  return catalog.release();
}

void te::serialize::Save(const te::da::DataSourceCatalog* catalog, const std::string& fileName)
{
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(catalog, ostr);

  ostr.close();
}

void te::serialize::Save(const te::da::DataSourceCatalog* catalog, std::ostream& ostr)
{
  te::xml::Writer w(ostr);

  Save(catalog, w);
}

void te::serialize::Save(const te::da::DataSourceCatalog* catalog, te::xml::Writer& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("DataSourceCatalog");

  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/da");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/da C:/Users/gribeiro/Documents/terralib5/trunk/myschemas/terralib/da/catalog.xsd");
  writer.writeAttribute("version", "5.0.0");
  writer.writeAttribute("release", "2011-01-01");

  //writer.writeElement(ostr, "Name", "");

  writer.writeStartElement("DataSetTypes");

  const std::size_t ndatasets = catalog->getNumberOfDataSets();

  for(std::size_t i = 0; i < ndatasets; ++i)
    Save(catalog->getDataSetType(i).get(), writer);

  writer.writeEndElement("DataSetTypes");

  writer.writeEndElement("DataSourceCatalog");
}

