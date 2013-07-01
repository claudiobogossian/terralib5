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
  \file terralib/serialization/dataaccess/DataSetType.cpp
 
  \brief Support for DataSetType serialization.
*/

// TerraLib
#include "../../../datatype/serialization/xml/Serializer.h"
#include "../../../datatype/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../../xml/Writer.h"
#include "../../dataset/DataSetType.h"
#include "../../datasource/DataSourceCapabilities.h"
#include "../../datasource/DataSourceCatalog.h"
#include "../../datasource/DataSourceInfo.h"
#include "../../datasource/DataSourceInfoManager.h"
#include "../../query/BinaryOpEncoder.h"
#include "../../query/Distinct.h"
#include "../../query/Expression.h"
#include "../../query/Field.h"
#include "../../query/FromItem.h"
#include "../../query/Function.h"
#include "../../query/FunctionEncoder.h"
#include "../../query/GroupByItem.h"
#include "../../query/Having.h"
#include "../../query/Literal.h"
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
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

void te::serialize::xml::ReadDataSourceInfo(const std::string& datasourcesFileName)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(datasourcesFileName);

  if(!xmlReader->next())
    throw te::da::Exception((boost::format(TR_DATAACCESS("Could not read data source information in file: %1%.")) % datasourcesFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TR_DATAACCESS("Error reading the document %1%, the start element wasn't found.")) % datasourcesFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceList")
    throw te::da::Exception((boost::format(TR_DATAACCESS("The first tag in the document %1% is not 'DataSourceList'.")) % datasourcesFileName).str());

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

void te::serialize::xml::Save(const std::string& fileName)
{
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(ostr);

  ostr.close();
}

void te::serialize::xml::Save(std::ostream& ostr)
{
  te::xml::Writer w(ostr);

  Save(w);
}

void te::serialize::xml::Save(te::xml::Writer& writer)
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
  std::fstream ostr(fileName.c_str(), std::ios_base::out);

  Save(catalog, ostr);

  ostr.close();
}

void te::serialize::xml::Save(const te::da::DataSourceCatalog* catalog, std::ostream& ostr)
{
  te::xml::Writer w(ostr);

  Save(catalog, w);
}

void te::serialize::xml::Save(const te::da::DataSourceCatalog* catalog, te::xml::Writer& writer)
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

te::da::DataSetType* te::serialize::xml::ReadDataSetType(te::xml::Reader& reader)
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
    std::auto_ptr<te::dt::Property> p(ReadProperty(reader));
    dt->add(p.release());
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of DataSetType Element
  reader.next();

  return dt.release();
}

void te::serialize::xml::Save(const te::da::DataSetType* dt, te::xml::Writer& writer)
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
    throw te::da::Exception((boost::format(TR_DATAACCESS("Could not read DataSourceCapabilities information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TR_DATAACCESS("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceCapabilities")
    throw te::da::Exception((boost::format(TR_DATAACCESS("The first tag in the document %1% is not 'DataSourceCapabilities'.")) % dialectFileName).str());

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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Insertion");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportInsertion(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Insertion

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Update");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportUpdate(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Update

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Deletion");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  dsetc.setSupportDeletion(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Deletion

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
    throw te::da::Exception((boost::format(TR_DATAACCESS("Could not read query dialect information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw te::da::Exception((boost::format(TR_DATAACCESS("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "QueryDialect")
    throw te::da::Exception((boost::format(TR_DATAACCESS("The first tag in the document %1% is not 'QueryDialect'.")) % dialectFileName).str());

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
      throw te::da::Exception(TR_DATAACCESS("Unsupported encoder type!"));
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
  throw te::da::Exception("Not implemented yet!");
}

te::da::Expression* te::serialize::xml::ReadExpression(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Field* te::serialize::xml::ReadQueryField(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Fields* te::serialize::xml::ReadFields(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::From* te::serialize::xml::ReadFrom(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Function* te::serialize::xml::ReadFunction(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::GroupBy* te::serialize::xml::ReadGroupBy(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Having* te::serialize::xml::ReadHaving(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Literal* te::serialize::xml::ReadLiteral(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::OrderBy* te::serialize::xml::ReadOrderBy(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::PropertyName* te::serialize::xml::ReadPropertyName(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Select* te::serialize::xml::ReadSelect(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

te::da::Where* te::serialize::xml::ReadWhere(te::xml::Reader& reader)
{
  throw te::da::Exception("Not implemented yet!");
}

void te::serialize::xml::Save(const te::da::Distinct* distinct, te::xml::Writer& writer)
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

void te::serialize::xml::Save(const te::da::Expression* expression, te::xml::Writer& writer)
{
  assert(expression);
  std::auto_ptr<te::da::Expression> exp(expression->clone());

  // Check Expression Type
  std::auto_ptr<te::da::PropertyName> pName(dynamic_cast<te::da::PropertyName*>(exp.get()));
  std::auto_ptr<te::da::Function> func(dynamic_cast<te::da::Function*>(exp.get()));
  std::auto_ptr<te::da::Literal> lit(dynamic_cast<te::da::Literal*>(exp.get()));

  if(pName.get())
    Save(pName.get(), writer);
  else if(func.get())
    Save(func.get(), writer);
  else if(lit.get())
    Save(lit.get(), writer);
  else
    throw te::da::Exception("Error: Expression Type Indefined!");
}

void te::serialize::xml::Save(const te::da::Field* field, te::xml::Writer& writer)
{
  assert(field);
  writer.writeStartElement("te_da:Field");

  Save(field->getExpression(), writer);

  writer.writeStartElement("te_da:Alias");

  writer.writeValue(*field->getAlias());

  writer.writeEndElement("te_da:Alias");

  writer.writeEndElement("te_da:Field");

}

void te::serialize::xml::Save(const te::da::Fields* fields, te::xml::Writer& writer)
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

void te::serialize::xml::Save(const te::da::From* from, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:From");

  for(std::size_t i = 0; i < from->size(); ++i)
  {
    const te::da::FromItem& item= from->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:From");
}

void te::serialize::xml::Save(const te::da::FromItem* fromItem, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:FromItem");

  writer.writeValue(fromItem->getAlias());

  writer.writeEndElement("te_da:FromItem");
}

void te::serialize::xml::Save(const te::da::Function* func, te::xml::Writer& writer)
{
  assert(func);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "Function");

  

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::xml::Save(const te::da::GroupByItem* groupByItem, te::xml::Writer& writer)
{
  assert(groupByItem);
  writer.writeStartElement("te_da:GroupByItem");

  Save(groupByItem->getExpression(), writer);

  writer.writeEndElement("te_da:GroupByItem");
}

void te::serialize::xml::Save(const te::da::GroupBy* groupBy, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:GroupBy");

  for(std::size_t i = 0; i < groupBy->size(); ++i)
  {
    const te::da::GroupByItem& item = groupBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:GroupBy");
}

void te::serialize::xml::Save(const te::da::Having* having, te::xml::Writer& writer)
{
  assert(having);
  writer.writeStartElement("te_da:Having");

  Save(having->getExp(), writer);

  writer.writeEndElement("te_da:Having");
}

void te::serialize::xml::Save(const te::da::Literal* lit, te::xml::Writer& writer)
{
  assert(lit);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "Literal");

  //writer.writeValue(lit->getValue());

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::xml::Save(const te::da::OrderByItem* orderByItem, te::xml::Writer& writer)
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

void te::serialize::xml::Save(const te::da::OrderBy* orderBy, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:OrderBy");

  for(std::size_t i = 0; i < orderBy->size(); ++i)
  {
    const te::da::OrderByItem& item = orderBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:OrderBy");
}

void te::serialize::xml::Save(const te::da::PropertyName* propertyName, te::xml::Writer& writer)
{
  assert(propertyName);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "PropertyName");

  writer.writeValue(propertyName->getName());

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::xml::Save(const te::da::Select* select, te::xml::Writer& writer)
{
  assert(select);
  writer.writeStartElement("te_da:Select");

  std::auto_ptr<const te::da::Fields> fields(select->getFields());
  if(fields.get())
    Save(fields.get(), writer); // Fields
  
  std::auto_ptr<const te::da::From> from(select->getFrom());
  if(from.get())
    Save(from.get(), writer); // From

  std::auto_ptr<const te::da::Where> wh(select->getWhere());
  if(wh.get())
    Save(wh.get(), writer); // Where

  std::auto_ptr<const te::da::GroupBy> groupBy(select->getGroupBy());
  if(groupBy.get())
    Save(groupBy.get(), writer); // GroupBy

  std::auto_ptr<const te::da::Having> having(select->getHaving());
  if(having.get())
    Save(having.get(), writer); // Having

  std::auto_ptr<const te::da::OrderBy> orderBy(select->getOrderBy());
  if(orderBy.get())
    Save(orderBy.get(), writer); // OrderBy

  std::auto_ptr<const te::da::Distinct> distinct(select->getDistinct());
  if(distinct.get())
    Save(distinct.get(), writer); // Distinct

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

void te::serialize::xml::Save(const te::da::Where* wh, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:OrdeWhererBy");

  Save(wh->getExp(), writer);

  writer.writeEndElement("te_da:Where");
}