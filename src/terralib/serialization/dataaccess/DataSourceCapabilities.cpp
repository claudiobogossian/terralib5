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
  \file DataSourceCapabilities.cpp
   
  \brief Auxiliary classes and functions to read DataSource Capabilities information from a XML document.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/ReaderFactory.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/query/BinaryOpEncoder.h"
#include "../../dataaccess/query/FunctionEncoder.h"
#include "../../dataaccess/query/SQLDialect.h"
#include "../../dataaccess/query/SQLFunctionEncoder.h"
#include "../../dataaccess/query/TemplateEncoder.h"
#include "../../dataaccess/query/UnaryOpEncoder.h"
#include "../../datatype/Utils.h"
#include "../../geometry/Geometry.h"
#include "../Exception.h"
#include "DataSourceCapabilities.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>

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
}

void te::serialize::Read(const std::string& dialectFileName, te::da::DataSourceCapabilities* capabilities, te::da::SQLDialect* dialect)
{
  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(dialectFileName);

  if(!xmlReader->next())
    throw Exception((boost::format(TR_DATAACCESS("Could not read DataSourceCapabilities information in file: %1%.")) % dialectFileName).str());

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TR_DATAACCESS("Error reading the document %1%, the start element wasn't found.")) % dialectFileName).str());

  if(xmlReader->getElementLocalName() != "DataSourceCapabilities")
    throw Exception((boost::format(TR_DATAACCESS("The first tag in the document %1% is not 'DataSourceCapabilities'.")) % dialectFileName).str());

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

  capabilities->setDataTypeCapabilities(dtc);

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // DataTypeCapabilities

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "QueryCapabilities");

  te::da::QueryCapabilities qc;
  
  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SQLDialect");

  qc.setSupportSQLDialect(xmlReader->getElementValueAsBoolean());

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

  te::da::SQLFunctionEncoder* sfe = 0;

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    
    qc.addSpatialOperator(xmlReader->getAttr("name"));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "FunctionEncoder");
    
    std::string alias = xmlReader->getAttr("alias");
    sfe = new te::da::FunctionEncoder(alias);

    dialect->insert(xmlReader->getAttr("name"), sfe);
   
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpatialOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "LogicalOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {    
    
    qc.addLogicalOperator(xmlReader->getAttr("name"));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "BinaryOpEncoder");
    
    std::string alias = xmlReader->getAttr("alias");
    sfe = new te::da::BinaryOpEncoder(alias);

    dialect->insert(xmlReader->getAttr("name"), sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // LogicalOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "ComparsionOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    
    qc.addComparsionOperator(xmlReader->getAttr("name"));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "BinaryOpEncoder");
    
    std::string alias = xmlReader->getAttr("alias");
    sfe = new te::da::BinaryOpEncoder(alias);

    dialect->insert(xmlReader->getAttr("name"), sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // ComparsionOperators
  
  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "ArithmeticOperators");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {    
    
    qc.addArithmeticOperator(xmlReader->getAttr("name"));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "BinaryOpEncoder");
    
    std::string alias = xmlReader->getAttr("alias");
    sfe = new te::da::BinaryOpEncoder(alias);

    dialect->insert(xmlReader->getAttr("name"), sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT);
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // ArithmeticOperators

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "Functions");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {    
    
    qc.addFunction(xmlReader->getAttr("name"));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
    assert(xmlReader->getElementLocalName() == "FunctionEncoder");
    
    std::string alias = xmlReader->getAttr("alias");
    sfe = new te::da::FunctionEncoder(alias);

    dialect->insert(xmlReader->getAttr("name"), sfe);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); 
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Functions

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "GeomOperands");

  while(xmlReader->next() &&
        (xmlReader->getNodeType() == te::xml::START_ELEMENT) &&
        (xmlReader->getElementLocalName() == "Function"))
  {
    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::VALUE);
    
    qc.addGeometryOperand(te::gm::Geometry::getGeomTypeId(boost::to_upper_copy(xmlReader->getAttr("name"))));

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Function
  }

  xmlReader->next();
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

  capabilities->setDataSetTypeCapabilities(dstc);

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

  capabilities->setDataSetCapabilities(dsetc);

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
    
    capabilities->addSpecificCapability(paramName, paramValue);

    xmlReader->next();
    assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // Parameter
  }

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SpecificCapabilities

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "AccessPolicyName");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setAccessPolicy(Convert2Terralib(xmlReader->getElementValue()));

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // AccessPolicyName

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportTransactions");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setSupportTransactions(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportTransactions

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportDataSetPesistenceAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setSupportDataSetPesistenceAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportDataSetPesistenceAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportDataSetTypePesistenceAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setSupportDataSetTypePesistenceAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportDataSetTypePesistenceAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportPreparedQueryAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setSupportPreparedQueryAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportPreparedQueryAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader->getElementLocalName() == "SupportBatchExecutorAPI");

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::VALUE);

  capabilities->setSupportBatchExecutorAPI(xmlReader->getElementValueAsBoolean());

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // SupportBatchExecutorAPI

  xmlReader->next();
  assert(xmlReader->getNodeType() == te::xml::END_ELEMENT); // DataSourceCapabilities
}
