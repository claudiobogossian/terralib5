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
  \file terralib/dataaccess/serialization/xml/DataSetType.cpp
 
  \brief Support for DataSetType serialization.
*/

// TerraLib
#include "../../../Version.h"
#include "../../../common/BoostUtils.h"
#include "../../../common/PlatformUtils.h"
#include "../../../datatype/AbstractData.h"
#include "../../../datatype/Enums.h"
#include "../../../datatype/serialization/xml/Serializer.h"
#include "../../../datatype/StringProperty.h"
#include "../../../datatype/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/WKTReader.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/AbstractWriterFactory.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../dataset/DataSetType.h"
#include "../../datasource/DataSourceCapabilities.h"
#include "../../datasource/DataSourceCatalog.h"
#include "../../datasource/DataSourceInfo.h"
#include "../../datasource/DataSourceInfoManager.h"
#include "../../query/BinaryFunction.h"
#include "../../query/BinaryOpEncoder.h"
#include "../../query/DataSetName.h"
#include "../../query/Distinct.h"
#include "../../query/Expression.h"
#include "../../query/Field.h"
#include "../../query/FromItem.h"
#include "../../query/Function.h"
#include "../../query/FunctionEncoder.h"
#include "../../query/GroupByItem.h"
#include "../../query/Having.h"
#include "../../query/Join.h"
#include "../../query/JoinConditionOn.h"
#include "../../query/JoinConditionUsing.h"
#include "../../query/Literal.h"
#include "../../query/LiteralDouble.h"
#include "../../query/LiteralGeom.h"
#include "../../query/LiteralInt16.h"
#include "../../query/LiteralInt32.h"
#include "../../query/LiteralInt64.h"
#include "../../query/LiteralString.h"
#include "../../query/OrderByItem.h"
#include "../../query/PropertyName.h"
#include "../../query/Select.h"
#include "../../query/SQLDialect.h"
#include "../../query/SQLFunctionEncoder.h"
#include "../../query/TemplateEncoder.h"
#include "../../query/UnaryOpEncoder.h"
#include "../../query/Where.h"
#include "../../Exception.h"
#include "Serializer.h"

// STL
#include <cassert>
#include <fstream>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

void te::serialize::xml::ReadDataSourceInfo(const std::string& datasourcesFileName)
{
  if(!boost::filesystem::is_regular_file(datasourcesFileName))
    return;

  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(datasourcesFileName);

  if(!xmlReader->next())
    throw te::da::Exception((boost::format(TE_TR("Could not read data source information in file: %1%.")) % datasourcesFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TE_TR("Error reading the document %1%, the start element wasn't found.")) % datasourcesFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceList")
    throw te::da::Exception((boost::format(TE_TR("The first tag in the document %1% is not 'DataSourceList'.")) % datasourcesFileName).str());

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

te::da::DataSourceInfo* te::serialize::xml::ReadDataSourceInfo(te::xml::Reader& reader)
{
  std::auto_ptr<te::da::DataSourceInfo> ds(new te::da::DataSourceInfo);

  ds->setId(reader.getAttr("id"));
  ds->setType(reader.getAttr("type"));
  ds->setAccessDriver(reader.getAttr("access_driver"));

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

void te::serialize::xml::Save(const std::string& fileName)
{
  std::auto_ptr<te::xml::AbstractWriter> w(te::xml::AbstractWriterFactory::make());

  w->setURI(fileName);

  Save(*w.get());
}

void te::serialize::xml::Save(te::xml::AbstractWriter& writer)
{
  std::string schema_loc = te::common::FindInTerraLibPath("share/terralib/schemas/terralib/dataaccess/dataaccess.xsd");

  boost::replace_all(schema_loc, " ", "%20");

  schema_loc = "file:///" + schema_loc;

  writer.writeStartDocument("UTF-8", "no");

  writer.setRootNamespaceURI("http://www.terralib.org/schemas/dataaccess");

  writer.writeStartElement("te_da:DataSourceList");

  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns:te_common", "http://www.terralib.org/schemas/common");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/dataaccess");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/dataaccess " + schema_loc);
  writer.writeAttribute("version", TERRALIB_VERSION_STRING);
  writer.writeAttribute("release", "2013-01-01");

  te::da::DataSourceInfoManager::iterator itBegin = te::da::DataSourceInfoManager::getInstance().begin();
  te::da::DataSourceInfoManager::iterator itEnd = te::da::DataSourceInfoManager::getInstance().end();
  te::da::DataSourceInfoManager::iterator it;

  for(it=itBegin; it!=itEnd; ++it)
  {
    bool ogrDsrc = it->second->getAccessDriver() == "OGR";

    writer.writeStartElement("te_da:DataSource");

    writer.writeAttribute("id", it->second->getId());
    writer.writeAttribute("type", it->second->getType());
    writer.writeAttribute("access_driver", it->second->getAccessDriver());

    writer.writeStartElement("te_da:Title");
    writer.writeValue((!ogrDsrc) ? it->second->getTitle() : te::common::ConvertLatin1UTFString(it->second->getTitle()));
    writer.writeEndElement("te_da:Title");

    writer.writeStartElement("te_da:Description");
    writer.writeValue((!ogrDsrc) ? it->second->getDescription() : te::common::ConvertLatin1UTFString(it->second->getDescription()));
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
      writer.writeValue((ogrDsrc && (conIt->first == "URI" || conIt->first == "SOURCE")) ? te::common::ConvertLatin1UTFString(conIt->second) : conIt->second);
      writer.writeEndElement("te_common:Value");

      writer.writeEndElement("te_common:Parameter");
    }
    writer.writeEndElement("te_da:ConnectionInfo");

    writer.writeEndElement("te_da:DataSource");
  }

  writer.writeEndElement("te_da:DataSourceList");

  writer.writeToFile();
}

te::da::DataSourceCatalog* te::serialize::xml::ReadDataSourceCatalog(te::xml::Reader& reader)
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

void te::serialize::xml::Save(const te::da::DataSourceCatalog* catalog, const std::string& fileName)
{
  std::auto_ptr<te::xml::AbstractWriter> w(te::xml::AbstractWriterFactory::make());
  w->setURI(fileName);
  Save(catalog, *w.get());
}

void te::serialize::xml::Save(const te::da::DataSourceCatalog* catalog, te::xml::AbstractWriter& writer)
{
  writer.writeStartDocument("UTF-8", "no");

  writer.writeStartElement("DataSourceCatalog");

  writer.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  writer.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance");
  writer.writeAttribute("xmlns", "http://www.terralib.org/schemas/da");
  writer.writeAttribute("xsd:schemaLocation", "http://www.terralib.org/schemas/da C:/Users/gribeiro/Documents/terralib5/trunk/myschemas/terralib/da/catalog.xsd");
  writer.writeAttribute("version", "5.0.0");
  writer.writeAttribute("release", "2011-01-01");

  writer.writeStartElement("DataSetTypes");

  const std::size_t ndatasets = catalog->getNumberOfDataSets();

  for(std::size_t i = 0; i < ndatasets; ++i)
    Save(catalog->getDataSetType(i).get(), writer);

  writer.writeEndElement("DataSetTypes");

  writer.writeEndElement("DataSourceCatalog");
}

te::da::DataSetType* te::serialize::xml::ReadDataSetType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "DataSetType");

  unsigned int id = reader.getAttrAsUInt32("id");
  std::string name = reader.getAttr("name");
  std::string title = reader.getAttr("title");

  reader.next();

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(name, id));

  dt->setTitle(title);

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "Property")
  {
    std::auto_ptr<te::dt::Property> p(ReadProperty(reader));
    dt->add(p.release());
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSetType Element
  reader.next();

  return dt.release();
}

void te::serialize::xml::Save(const te::da::DataSetType* dt, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_da:DataSetType");

  writer.writeAttribute("id", dt->getId());
  writer.writeAttribute("name", dt->getName());
  writer.writeAttribute("title", dt->getTitle());

  const std::size_t nprops = dt->size();

  for(std::size_t i = 0; i < nprops; ++i)
    Save(*(dt->getProperty(i)), writer);

  writer.writeEndElement("te_da:DataSetType");
}

te::common::AccessPolicy Convert2Terralib(std::string accessPolicy)
{
  if(accessPolicy == "NO_ACCESS")
    return te::common::NoAccess;
  else if (accessPolicy == "R_ACCESS")
    return te::common::RAccess;
  else if(accessPolicy == "W_ACCESS")
    return te::common::WAccess;
  else if(accessPolicy == "RW_ACCESS")
    return te::common::RWAccess;
  else
    return te::common::NoAccess;
}

void te::serialize::xml::Read(const std::string& dialectFileName, te::da::DataSourceCapabilities& capabilities, te::da::SQLDialect& dialect)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(dialectFileName);

  if(!xmlReader->next())
    throw te::da::Exception((boost::format(TE_TR("Could not read DataSourceCapabilities information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TE_TR("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceCapabilities")
    throw te::da::Exception((boost::format(TE_TR("The first tag in the document %1% is not 'DataSourceCapabilities'.")) % dialectFileName).str());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DataTypeCapabilities");

  te::da::DataTypeCapabilities dtc; 

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "DataType"))
  {
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::VALUE);

    dtc.setSupport(te::dt::Convert2Terralib(xmlReader->getElementValue()), true);
    
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  }

  capabilities.setDataTypeCapabilities(dtc);

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // DataTypeCapabilities

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "QueryCapabilities");

  te::da::QueryCapabilities qc;

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SQLDialect");
  
  xmlReader->next();
  qc.setSupportSQLDialect(xmlReader->getElementValueAsBoolean());  

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SQLDialect

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpatialSQLDialect");

  xmlReader->next();
  qc.setSupportSpatialSQLDialect(xmlReader->getElementValueAsBoolean());  

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialSQLDialect

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Insert");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportInsert(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Insert

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Update");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportUpdate(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Update

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Delete");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportDelete(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Delete

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Create");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportCreate(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Create

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Drop");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportDrop(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Drop

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Alter");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportAlter(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Alter

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Select");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportSelect(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Select

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SelectInto");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  qc.setSupportSelectInto(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SelectInto

  //----------------------------

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpatialTopologicOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addSpatialTopologicOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);
    
    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialTopologicOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpatialMetricOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addSpatialMetricOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);
    
    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialMetricOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpatialNewGeomOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addSpatialNewGeomOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);
    
    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialNewGeomOperators

  //----------------------------

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpatialOperators");   

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addSpatialOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);   
    
    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();    
  }  

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "LogicalOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {    
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addLogicalOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);

    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();    
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // LogicalOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "ComparsionOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    te::da::SQLFunctionEncoder* sfe = 0;

    std::string fname = xmlReader->getAttr("name");

    qc.addComparsionOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);

    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();    
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // ComparsionOperators
  
  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "ArithmeticOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {    
    te::da::SQLFunctionEncoder* sfe = 0;
    
    std::string fname = xmlReader->getAttr("name");

    qc.addArithmeticOperator(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);

    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();    
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // ArithmeticOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Functions");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {   
    te::da::SQLFunctionEncoder* sfe = 0;
    
    std::string fname = xmlReader->getAttr("name");

    qc.addFunction(fname);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else if(encoderType == "TemplateEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      std::string temp = xmlReader->getAttr(1);
      sfe = new te::da::TemplateEncoder(alias, temp);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    dialect.insert(fname, sfe);

    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
   
    xmlReader->next();    
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Functions

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "GeomOperands");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "GeomType"))
  {
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::VALUE);
    
    std::string geom = boost::to_upper_copy(xmlReader->getElementValue());

    qc.addGeometryOperand(te::gm::Geometry::getGeomTypeId(geom));

    xmlReader->next();
    
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);        
  }

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // GeomOperands

  xmlReader->next();

  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // QueryCapabilities

  capabilities.setQueryCapabilities(qc);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DataSetTypeCapabilities");

  te::da::DataSetTypeCapabilities dstc;

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportPrimaryKey");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportPrimaryKey(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportPrimaryKey

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportUniqueKey");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportUniqueKey(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportUniqueKey

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportForeignKey");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportForeingKey(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportForeignKey

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportSequence");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportSequence(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportSequence

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportCheckConstraints");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportCheckConstraints(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportCheckConstraints

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportIndex");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportIndex(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportIndex

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportRTreeIndex");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportRTreeIndex(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportRTreeIndex

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportBTreeIndex");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportBTreeIndex(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportBTreeIndex

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportHashIndex");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportHashIndex(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportHashIndex

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportQuadTreeIndex");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dstc.setSupportQuadTreeIndex(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportQuadTreeIndex

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // DataSetTypeCapabilities

  capabilities.setDataSetTypeCapabilities(dstc);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DataSetCapabilities");

  te::da::DataSetCapabilities dsetc;

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Bidirectional");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportBidirectionalTraversing(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Bidirectional

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Random");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportRandomTraversing(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Random

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Indexed");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportIndexedTraversing(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Indexed

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientMovePrevious");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientMovePrevious(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientMovePrevious

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientMoveBeforeFirst");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientMoveBeforeFirst(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientMoveBeforeFirst

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientMoveLast");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientMoveLast(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientMoveLast

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientMoveAfterLast");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientMoveAfterLast(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientMoveAfterLast

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientMove");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientMove(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientMove

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "EfficientSize");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportEfficientDataSetSize(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // EfficientSize

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // DataSetCapabilities

  capabilities.setDataSetCapabilities(dsetc);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SpecificCapabilities");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Parameter"))
  {
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "Name");
    
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::VALUE);

    std::string paramName = xmlReader->getElementValue();

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Name

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "Value");
    
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::VALUE);

    std::string paramValue = xmlReader->getElementValue();

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Value
    
    capabilities.addSpecificCapability(paramName, paramValue);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Parameter
  }
    
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpecificCapabilities

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "AccessPolicyName");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setAccessPolicy(Convert2Terralib(xmlReader->getElementValue()));

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // AccessPolicyName

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportTransactions");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setSupportTransactions(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportTransactions

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportDataSetPesistenceAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setSupportDataSetPesistenceAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportDataSetPesistenceAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportDataSetTypePesistenceAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setSupportDataSetTypePesistenceAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportDataSetTypePesistenceAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportPreparedQueryAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setSupportPreparedQueryAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportPreparedQueryAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportBatchExecutorAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities.setSupportBatchExecutorAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportBatchExecutorAPI

  xmlReader->next();
  
  assert(xmlReader->getNodeType() == te::xml::END_DOCUMENT); // DataSourceCapabilities
}

te::da::SQLDialect* te::serialize::xml::ReadDialect(const std::string& dialectFileName)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(dialectFileName);

  if(!xmlReader->next())
    throw te::da::Exception((boost::format(TE_TR("Could not read query dialect information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TE_TR("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "QueryDialect")
    throw te::da::Exception((boost::format(TE_TR("The first tag in the document %1% is not 'QueryDialect'.")) % dialectFileName).str());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "DriverName");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);
  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Functions");

  std::auto_ptr<te::da::SQLDialect> mydialect(new te::da::SQLDialect);

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    std::string fname = xmlReader->getAttr(0);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);

    std::string encoderType = xmlReader->getElementLocalName();

    te::da::SQLFunctionEncoder* sfe = 0;

    if(encoderType == "FunctionEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::FunctionEncoder(alias);
    }
    else if(encoderType == "BinaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::BinaryOpEncoder(alias);
    }
    else if(encoderType == "UnaryOpEncoder")
    {
      std::string alias = xmlReader->getAttr(0);
      sfe = new te::da::UnaryOpEncoder(alias);
    }
    else
    {
      throw te::da::Exception(TE_TR("Unsupported encoder type!"));
    }

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // End of Encoder

    mydialect->insert(fname, sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // End of Function
  }

  assert(xmlReader->getNodeType() == te::xml::END_DOCUMENT); // End of QueryDialect

  return mydialect.release();
}

te::da::Distinct* te::serialize::xml::ReadDistinct(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Distinct");

  reader.next();

  te::da::Distinct* dist = new te::da::Distinct;

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Expression* exp = 0;

    if(reader.getElementLocalName() == "PropertyName")
    {
      exp = ReadPropertyName(reader);
    }
    else if(reader.getElementLocalName() == "Literal")
    {
      exp = ReadLiteral(reader);
    }
    else if(reader.getElementLocalName() == "Function")
    {
      exp = ReadFunction(reader);
    }

    dist->push_back(exp);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Distinct

  reader.next();

  return dist;
}

te::da::Fields* te::serialize::xml::ReadFields(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Fields");

  reader.next();

  te::da::Fields* fields = new te::da::Fields;

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    assert(reader.getElementLocalName() == "Field");
    reader.next();
    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    if(reader.getElementLocalName() == "PropertyName")
    {
      te::da::Field* f = new te::da::Field(ReadPropertyName(reader));
      fields->push_back(f);
    }
    else if(reader.getElementLocalName() == "Function")
    {
      te::da::Field* f = new te::da::Field(ReadFunction(reader));
      fields->push_back(f);
    }
    else if(reader.getElementLocalName() == "Literal")
    {
      te::da::Field* f = new te::da::Field(ReadLiteral(reader));
      fields->push_back(f);
    }

    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Field

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Fields

  reader.next();

  return fields;
}

te::da::From* te::serialize::xml::ReadFrom(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "From");

  reader.next();

  te::da::From* from = new te::da::From;

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    if(reader.getElementLocalName() == "FromItem")
    {
      from->push_back(ReadFromItem(reader));
    }
    else if(reader.getElementLocalName() == "JoinItem")
    {
      reader.next();

      te::da::FromItem* first = ReadFromItem(reader);
      te::da::FromItem* second = ReadFromItem(reader);

      assert(reader.getNodeType() == te::xml::START_ELEMENT);
      assert(reader.getElementLocalName() == "Type");

      reader.next();

      assert(reader.getNodeType() == te::xml::VALUE);

      std::string type = reader.getElementValue();

      reader.next();

      te::da::JoinType joinType = te::da::JOIN;

      if(type ==  "JOIN")
        joinType = te::da::JOIN;
      else if (type == "INNER_JOIN")
        joinType = te::da::INNER_JOIN;
      else if (type == "LEFT_JOIN")
        joinType = te::da::LEFT_JOIN;
      else if (type == "RIGHT_JOIN")
        joinType = te::da::RIGHT_JOIN;
      else if (type == "FULL_OUTER_JOIN")
        joinType = te::da::FULL_OUTER_JOIN;
      else if ( type == "CROSS_JOIN")
        joinType = te::da::CROSS_JOIN;
      else
        joinType = te::da::NATURAL_JOIN;

      assert(reader.getNodeType() == te::xml::END_ELEMENT); // Type

      reader.next();

      assert(reader.getNodeType() == te::xml::START_ELEMENT);
      assert(reader.getElementLocalName() == "Condition");

      reader.next();
      te::da::JoinCondition* JoinCondition = 0;
      assert(reader.getNodeType() == te::xml::START_ELEMENT);

      if(reader.getElementLocalName() == "JoinConditionOn")
      {
        reader.next();
        JoinCondition = new te::da::JoinConditionOn(ReadFunction(reader));
        assert(reader.getNodeType() == te::xml::END_ELEMENT); // JoinConditionOn
      }
      else if(reader.getElementLocalName() == "JoinConditionUsing")
      {
        reader.next();
        while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "PropertyName")
        {
          JoinCondition = new te::da::JoinConditionUsing();
          dynamic_cast<te::da::JoinConditionUsing*>(JoinCondition)->push_back(ReadPropertyName(reader));
        }
        assert(reader.getNodeType() == te::xml::END_ELEMENT); // JoinConditionUsing
      }

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // Condition

      te::da::Join* join = new te::da::Join(first, second, joinType, JoinCondition);
      from->push_back(join);

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // JoinItem
      reader.next();
    }
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // From
  reader.next();

  return from;
}

te::da::FromItem* te::serialize::xml::ReadFromItem(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "FromItem");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Value");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string name = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Alias");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string alias = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // FromItem

  reader.next();

  return new te::da::DataSetName(name, alias);
}

te::da::Function* te::serialize::xml::ReadFunction(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Function");

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Name");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string name = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);

  reader.next();

  te::da::Expression* exp1 = 0;
  te::da::Expression* exp2 = 0;

  std::size_t countAux = 0;
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Expression* exp = 0;

    if(reader.getElementLocalName() == "PropertyName")
    {
      exp = ReadPropertyName(reader);
    }
    else if(reader.getElementLocalName() == "Literal")
    {
      exp = ReadLiteral(reader);
    }
    else if(reader.getElementLocalName() == "Function")
    {
      exp = ReadFunction(reader);
    }

    if(countAux == 0)
      exp1 = exp;
    else
      exp2 = exp;

    ++countAux;
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Function

  te::da::BinaryFunction* func = new te::da::BinaryFunction(name, exp1, exp2);

  reader.next();

  return func;
}

te::da::GroupBy* te::serialize::xml::ReadGroupBy(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GroupBy");

  reader.next();

  te::da::GroupBy* groupBy = new te::da::GroupBy;

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    assert(reader.getElementLocalName() == "GroupByItem");

    reader.next();

    te::da::Expression* exp = 0;

    if(reader.getElementLocalName() == "PropertyName")
    {
      exp = ReadPropertyName(reader);
    }
    else if(reader.getElementLocalName() == "Literal")
    {
      exp = ReadLiteral(reader);
    }
    else if(reader.getElementLocalName() == "Function")
    {
      exp = ReadFunction(reader);
    }

    te::da::GroupByItem* groupByItem = new te::da::GroupByItem(exp);

    groupBy->push_back(groupByItem);

    assert(reader.getNodeType() == te::xml::END_ELEMENT); //GroupByItem

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // GroupBy

  reader.next();

  return groupBy;
}

te::da::Having* te::serialize::xml::ReadHaving(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Having");

  reader.next();

  te::da::Expression* exp = 0;

  if(reader.getElementLocalName() == "PropertyName")
  {
    exp = ReadPropertyName(reader);
  }
  else if(reader.getElementLocalName() == "Literal")
  {
    exp = ReadLiteral(reader);
  }
  else if(reader.getElementLocalName() == "Function")
  {
    exp = ReadFunction(reader);
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); //Having

  reader.next();

  return new te::da::Having(exp);
}

te::da::Literal* te::serialize::xml::ReadLiteral(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Literal");

  std::size_t litType = reader.getAttrAsInt32("type");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string value = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Literal

  reader.next();

  te::da::Literal* lit = 0;

  // TODO: Others data type
  switch(litType)
  {
    case te::dt::STRING_TYPE:
    {
      lit = new te::da::LiteralString(value);
      break;
    }
    case te::dt::INT16_TYPE:
    {
      lit = new te::da::LiteralInt16(boost::lexical_cast<int16_t>(value));
      break;
    }
    case te::dt::INT32_TYPE:
    {
      lit = new te::da::LiteralInt32(boost::lexical_cast<int32_t>(value));
      break;
    }
    case te::dt::INT64_TYPE:
    {
      lit = new te::da::LiteralInt64(boost::lexical_cast<int64_t>(value));
      break;
    }
    case te::dt::DOUBLE_TYPE:
    {
      lit = new te::da::LiteralDouble(boost::lexical_cast<double>(value));
      break;
    }
    case te::dt::GEOMETRY_TYPE:
    {
      std::auto_ptr<te::gm::Geometry> geom(te::gm::WKTReader::read(value.c_str()));

      lit = new te::da::LiteralGeom(geom.release());
      break;
    }
    default:
      throw te::da::Exception(TE_TR("Data Type Undefined!"));

  }

  return lit;
}

te::da::OrderBy* te::serialize::xml::ReadOrderBy(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "OrderBy");

  reader.next();

  te::da::OrderBy* orderBy = new te::da::OrderBy;

  while(reader.getNodeType() == te::xml::START_ELEMENT)
  {
    assert(reader.getElementLocalName() == "OrderByItem");

    reader.next();

    te::da::Expression* exp = 0;

    if(reader.getElementLocalName() == "PropertyName")
    {
      exp = ReadPropertyName(reader);
    }
    else if(reader.getElementLocalName() == "Literal")
    {
      exp = ReadLiteral(reader);
    }
    else if(reader.getElementLocalName() == "Function")
    {
      exp = ReadFunction(reader);
    }

    assert(reader.getNodeType() == te::xml::START_ELEMENT);

    assert(reader.getElementLocalName() == "SortOrder");

    reader.next();

    assert(reader.getNodeType() == te::xml::VALUE);

    std::string sortOrder = reader.getElementValue();

    te::da::SortOrder order = sortOrder == "ASC" ? te::da::ASC : te::da::DESC;

    te::da::OrderByItem* orderByItem = new te::da::OrderByItem(exp, order);

    orderBy->push_back(orderByItem);

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT);

    reader.next();

    assert(reader.getNodeType() == te::xml::END_ELEMENT); //OrderByItem

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // OrderBy

  reader.next();

  return orderBy;
}

te::da::PropertyName* te::serialize::xml::ReadPropertyName(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  reader.next();

  assert(reader.getNodeType() == te::xml::VALUE);

  std::string name = reader.getElementValue();

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // PropertyName

  reader.next();

  return new te::da::PropertyName(name);
}

te::da::Select* te::serialize::xml::ReadSelect(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Select");

  te::da::Select* select = new te::da::Select();

  reader.next();

  if(reader.getElementLocalName() == "Fields" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Fields* fields = ReadFields(reader);
    select->setFields(fields);
  }

  if(reader.getElementLocalName() == "From" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::From* from = ReadFrom(reader);
    select->setFrom(from);
  }

  if(reader.getElementLocalName() == "Where" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Where* wh = ReadWhere(reader);
    select->setWhere(wh);
  }

  if(reader.getElementLocalName() == "GroupBy" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::GroupBy* gBy = ReadGroupBy(reader);
    select->setGroupBy(gBy);
  }

  if(reader.getElementLocalName() == "Having" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Having* having = ReadHaving(reader);
    select->setHaving(having);
  }

  if(reader.getElementLocalName() == "OrderBy" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::OrderBy* oBy = ReadOrderBy(reader);
    select->setOrderBy(oBy);
  }

  if(reader.getElementLocalName() == "Distinct" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    te::da::Distinct* distinct = ReadDistinct(reader);
    select->setDistinct(distinct);
  }

  if(reader.getElementLocalName() == "Limit" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    select->setLimit(boost::lexical_cast<std::size_t>(reader.getElementValue()));
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Limit
    reader.next();
  }

  if(reader.getElementLocalName() == "Offset" && reader.getNodeType() == te::xml::START_ELEMENT)
  {
    reader.next();
    assert(reader.getNodeType() == te::xml::VALUE);
    select->setOffset(boost::lexical_cast<std::size_t>(reader.getElementValue()));
    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // Offset
    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Select

  reader.next();

  return select;
}

te::da::Where* te::serialize::xml::ReadWhere(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Where");

  reader.next();

  te::da::Where* wh = 0;

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  if(reader.getElementLocalName() == "PropertyName")
  {
    wh = new te::da::Where(ReadPropertyName(reader));
  }
  else if(reader.getElementLocalName() == "Function")
  {
    wh = new te::da::Where(ReadFunction(reader));
  }
  else if(reader.getElementLocalName() == "Literal")
  {
    wh = new te::da::Where(ReadLiteral(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // Where

  reader.next();

  return wh;
}

void te::serialize::xml::Save(const te::da::Distinct* distinct, te::xml::AbstractWriter& writer)
{
  assert(distinct);
  writer.writeStartElement("te_da:Distinct");

  for(std::size_t i = 0; i < distinct->size(); ++i)
  {
    const te::da::Expression& exp = distinct->at(i);

    Save(&exp, writer);
  }

  writer.writeEndElement("te_da:Distinct");
}

void te::serialize::xml::Save(const te::da::Expression* expression, te::xml::AbstractWriter& writer)
{
  assert(expression);

  std::auto_ptr<te::da::Expression> exp(expression->clone());

  // Check Expression Type
  te::da::PropertyName* pName = dynamic_cast<te::da::PropertyName*>(exp.get());
  te::da::Function* func = dynamic_cast<te::da::Function*>(exp.get());
  te::da::Literal* lit = dynamic_cast<te::da::Literal*>(exp.get());

  if(pName)
    Save(pName, writer);
  else if(func)
    Save(func, writer);
  else if(lit)
    Save(lit, writer);
  else
    throw te::da::Exception(TE_TR("Error: Expression Type Undefined!"));
}

void te::serialize::xml::Save(const te::da::Field* field, te::xml::AbstractWriter& writer)
{
  assert(field);
  writer.writeStartElement("te_da:Field");

  Save(field->getExpression(), writer);

  if(field->getAlias())
  {
    writer.writeStartElement("te_da:Alias");

    writer.writeValue(*field->getAlias());

    writer.writeEndElement("te_da:Alias");
  }

  writer.writeEndElement("te_da:Field");

}

void te::serialize::xml::Save(const te::da::Fields* fields, te::xml::AbstractWriter& writer)
{
  assert(fields);
  writer.writeStartElement("te_da:Fields");

  for(std::size_t i = 0; i < fields->size(); ++i)
  {
    const te::da::Field& item = fields->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:Fields");
}

void te::serialize::xml::Save(const te::da::From* from, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_da:From");

  for(std::size_t i = 0; i < from->size(); ++i)
  {
    const te::da::FromItem& item= from->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:From");
}

void te::serialize::xml::Save(const te::da::FromItem* fromItem, te::xml::AbstractWriter& writer)
{
  const te::da::DataSetName* dsName = dynamic_cast<const te::da::DataSetName*>(fromItem);
  if(dsName)
  {
    writer.writeStartElement("te_da:FromItem");

    writer.writeStartElement("te_da:Value");

    writer.writeValue(dsName->getName());

    writer.writeEndElement("te_da:Value");

    writer.writeStartElement("te_da:Alias");

    writer.writeValue(fromItem->getAlias());

    writer.writeEndElement("te_da:Alias");

    writer.writeEndElement("te_da:FromItem");
  }

  const te::da::Join* dsJoin = dynamic_cast<const te::da::Join*>(fromItem);
  if(dsJoin)
  {
    writer.writeStartElement("te_da:JoinItem");

    Save(dsJoin->getFirst(), writer);

    Save(dsJoin->getSecond(), writer);

    std::string joinType;
    switch(dsJoin->getType())
    {
      case te::da::JOIN:
        joinType = "JOIN";
        break;
      case te::da::INNER_JOIN:
        joinType = "INNER_JOIN";
        break;
      case te::da::LEFT_JOIN:
        joinType = "LEFT_JOIN";
        break;
      case te::da::RIGHT_JOIN:
        joinType = "RIGHT_JOIN";
        break;
      case te::da::FULL_OUTER_JOIN:
        joinType = "FULL_OUTER_JOIN";
        break;
      case te::da::CROSS_JOIN:
        joinType = "CROSS_JOIN";
        break;
      case te::da::NATURAL_JOIN:
        joinType = "NATURAL_JOIN";
        break;
      default:
        joinType = "JOIN";
    }

    writer.writeElement("te_da:Type",joinType);

    writer.writeStartElement("te_da:Condition");

    const te::da::JoinConditionOn* joinOn = dynamic_cast<const te::da::JoinConditionOn*>(dsJoin->getCondition());
    if(joinOn)
    {
      writer.writeStartElement("te_da:JoinConditionOn");
      te::da::Expression* exp = joinOn->getCondition();
      Save(exp, writer);
      writer.writeEndElement("te_da:JoinConditionOn");
    }
    const te::da::JoinConditionUsing* joinUsing = dynamic_cast<const te::da::JoinConditionUsing*>(dsJoin->getCondition());
    if(joinUsing)
    {
      writer.writeStartElement("te_da:JoinConditionUsing");
      for(std::size_t i = 0; i < joinUsing->getNumFields(); ++i)
      {
        const te::da::Expression* exp = (*joinUsing)[i];
        Save(exp, writer);
      }
      writer.writeEndElement("te_da:JoinConditionUsing");
    }

    writer.writeEndElement("te_da:Condition");

    writer.writeEndElement("te_da:JoinItem");
  }
}

void te::serialize::xml::Save(const te::da::Function* func, te::xml::AbstractWriter& writer)
{
  assert(func);
  writer.writeStartElement("te_da:Function");

  std::string funcName = func->getName();

  writer.writeElement("te_da:Name", funcName);

  for(std::size_t i = 0; i < func->getNumArgs(); i++)
  {
    Save(func->getArg(i), writer);
  }

  writer.writeEndElement("te_da:Function");
}

void te::serialize::xml::Save(const te::da::GroupByItem* groupByItem, te::xml::AbstractWriter& writer)
{
  assert(groupByItem);
  writer.writeStartElement("te_da:GroupByItem");

  Save(groupByItem->getExpression(), writer);

  writer.writeEndElement("te_da:GroupByItem");
}

void te::serialize::xml::Save(const te::da::GroupBy* groupBy, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_da:GroupBy");

  for(std::size_t i = 0; i < groupBy->size(); ++i)
  {
    const te::da::GroupByItem& item = groupBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:GroupBy");
}

void te::serialize::xml::Save(const te::da::Having* having, te::xml::AbstractWriter& writer)
{
  assert(having);
  writer.writeStartElement("te_da:Having");

  Save(having->getExp(), writer);

  writer.writeEndElement("te_da:Having");
}

void te::serialize::xml::Save(const te::da::Literal* lit, te::xml::AbstractWriter& writer)
{
  assert(lit);
  writer.writeStartElement("te_da:Literal");

  te::dt::AbstractData* d = lit->getValue();

  writer.writeAttribute("type", d->getTypeCode());

  writer.writeValue(d->toString());

  writer.writeEndElement("te_da:Literal");
}

void te::serialize::xml::Save(const te::da::OrderByItem* orderByItem, te::xml::AbstractWriter& writer)
{
  assert(orderByItem);
  writer.writeStartElement("te_da:OrderByItem");

  Save(orderByItem->getExpression(), writer);

  writer.writeStartElement("te_da:SortOrder");
  std::string so = (orderByItem->getSortOrder() == te::da::ASC ? "ASC" : "DESC");
  writer.writeValue(so);
  writer.writeEndElement("te_da:SortOrder");

  writer.writeEndElement("te_da:OrderByItem");
}

void te::serialize::xml::Save(const te::da::OrderBy* orderBy, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_da:OrderBy");

  for(std::size_t i = 0; i < orderBy->size(); ++i)
  {
    const te::da::OrderByItem& item = orderBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:OrderBy");
}

void te::serialize::xml::Save(const te::da::PropertyName* propertyName, te::xml::AbstractWriter& writer)
{
  assert(propertyName);

  writer.writeElement("te_da:PropertyName", propertyName->getName());
}

void te::serialize::xml::Save(const te::da::Select* select, te::xml::AbstractWriter& writer)
{
  assert(select);
  writer.writeStartElement("te_da:Select");

  const te::da::Fields* fields = select->getFields();
  if(fields)
    Save(fields, writer); // Fields
  
  const te::da::From* from = select->getFrom();
  if(from)
    Save(from, writer); // From

  const te::da::Where* wh = select->getWhere();
  if(wh)
    Save(wh, writer); // Where

  const te::da::GroupBy*  groupBy = select->getGroupBy();
  if(groupBy)
    Save(groupBy, writer); // GroupBy

  const te::da::Having* having = select->getHaving();
  if(having)
    Save(having, writer); // Having

  const te::da::OrderBy* orderBy = select->getOrderBy();
  if(orderBy)
    Save(orderBy, writer); // OrderBy

  const te::da::Distinct* distinct = select->getDistinct();
  if(distinct)
    Save(distinct, writer); // Distinct

  if(select->getLimit() != 0) // Limit
  {
    writer.writeStartElement("te_da:Limit");
    writer.writeValue(static_cast<unsigned int>(select->getLimit()));
    writer.writeEndElement("te_da:Limit");
  }

  if(select->getOffset() != 0) // Offset
  {
    writer.writeStartElement("te_da:Offset");
    writer.writeValue(static_cast<unsigned int>(select->getOffset()));
    writer.writeEndElement("te_da:Offset");
  }

  writer.writeEndElement("te_da:Select");
}

void te::serialize::xml::Save(const te::da::Where* wh, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("te_da:Where");

  Save(wh->getExp(), writer);

  writer.writeEndElement("te_da:Where");
}
