/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSourceGraphMetadata.cpp

  \brief Class used to define the graph metadata informations
         over a SGBD source
*/

// Terralib Includes
#include "../../../common/StringUtils.h"
#include "../../../common/Translator.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ForeignKey.h"
#include "../../../dataaccess/dataset/Index.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/query_h.h"
#include "../../../dataaccess/Enums.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../datatype/StringProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../core/EdgeProperty.h"
#include "../../core/VertexProperty.h"
#include "../../Config.h"
#include "../../Exception.h"
#include "../../Globals.h"
#include "DataSourceGraphMetadata.h"


te::graph::DataSourceGraphMetadata::DataSourceGraphMetadata(te::da::DataSource* ds) : te::graph::GraphMetadata(ds)
{
  m_mode = te::graph::Edge_List;
}

te::graph::DataSourceGraphMetadata::~DataSourceGraphMetadata()
{
}

void te::graph::DataSourceGraphMetadata::load(int id)
{
  if(m_ds == 0)
  {
    throw Exception(TR_GRAPH("Data Source not defined."));
  }

  //load graph metada information
  loadGraphInfo(id);

  //load vertex and edges attributes information
  loadGraphAttrInfo(id);
}

void te::graph::DataSourceGraphMetadata::save()
{
  if(m_ds == 0)
  {
    throw Exception(TR_GRAPH("Data Source not defined."));
  }

  if(this->getName().empty())
  {
    throw Exception(TR_GRAPH("Graph name not defined."));
  }

  if(!isValidGraphName(this->getName()))
  {
    throw Exception(TR_GRAPH("Invalid graph name."));
  }

  //add graph table entry
  addGraphTableNewEntry();

  //add graph attr table entries
  addGraphAttrTableNewEntry();

  //create graph table model
  if(this->getStorageMode() == te::graph::Vertex_List)
  {
    createGraphTableVertexModel();

    if(getEdgePropertySize() != 0)
    {
      createEdgeAttrTable();
    }
  }
  else if(this->getStorageMode() == te::graph::Edge_List)
  {
    createGraphTableEdgeModel();

    if(getVertexPropertySize() != 0)
    {
      createVertexAttrTable();
    }
  }
}

void te::graph::DataSourceGraphMetadata::update()
{
  if(m_ds == 0)
  {
    throw Exception(TR_GRAPH(""));
  }
}

void te::graph::DataSourceGraphMetadata::addVertexProperty(te::dt::Property* p)
{
  if(getVertexPropertySize() == 0)
  {
    createVertexAttrTable();
  }

  te::graph::GraphMetadata::addVertexProperty(p);

  //get data set
  std::string tableName = this->getVertexTableName();

  // add attribute entry in graph attr table
  saveGraphAttrTableNewEntry(this->getId(), tableName, p->getName(), Globals::sm_tableVertexModelAttrId, te::graph::Vertex_Attr);

  // add property in vertex table
  saveProperty(tableName, p);
}

void te::graph::DataSourceGraphMetadata::removeVertexProperty(int idx)
{
  te::dt::Property* p = te::graph::GraphMetadata::getVertexProperty(idx);

  std::string propertyName = p->getName();

  te::graph::GraphMetadata::removeVertexProperty(idx);

  //get data set
  std::string tableName = this->getVertexTableName();

  //remove property
  removeProperty(tableName, propertyName);
}

void te::graph::DataSourceGraphMetadata::addEdgeProperty(te::dt::Property* p)
{
  if(this->getStorageMode() == te::graph::Vertex_List)
  {
      throw Exception(TR_GRAPH("TO DO."));
  }

  te::graph::GraphMetadata::addEdgeProperty(p);

  //get data set
  std::string tableName = this->getEdgeTableName();

  // add attribute entry in graph attr table
  saveGraphAttrTableNewEntry(this->getId(), tableName, p->getName(), Globals::sm_tableEdgeModelAttrId, te::graph::Edge_Attr);

  // add property in edge table
  saveProperty(tableName, p);
}

void te::graph::DataSourceGraphMetadata::removeEdgeProperty(int idx)
{
  te::dt::Property* p = te::graph::GraphMetadata::getEdgeProperty(idx);

  std::string propertyName = p->getName();

  te::graph::GraphMetadata::removeEdgeProperty(idx);

  //get data set
  std::string tableName = this->getEdgeTableName();

  //remove property
  removeProperty(tableName, propertyName);
}

void te::graph::DataSourceGraphMetadata::createGraphMetadataTable()
{
  te::da::DataSetType* dt = new te::da::DataSetType(Globals::sm_tableGraphName);

  //first property: id
  te::dt::SimpleProperty* prop_id = new te::dt::SimpleProperty(Globals::sm_tableGraphAttrId, te::dt::INT32_TYPE, true);
  prop_id->setAutoNumber(true);
  dt->add(prop_id);

  //second property: name
  te::dt::StringProperty* prop_name = new te::dt::StringProperty(Globals::sm_tableGraphAttrName, te::dt::VAR_STRING);
  prop_name->setSize(32);
  dt->add(prop_name);

  //third property: type
  te::dt::SimpleProperty* prop_type = new te::dt::SimpleProperty(Globals::sm_tableGraphAttrType, te::dt::INT32_TYPE, false);
  dt->add(prop_type);

  //fourth property: table name
  te::dt::StringProperty* prop_tableName = new te::dt::StringProperty(Globals::sm_tableGraphAttrTableName, te::dt::VAR_STRING);
  prop_tableName->setSize(32);
  dt->add(prop_tableName);

  //fifth property: description
  te::dt::StringProperty* prop_desc = new te::dt::StringProperty(Globals::sm_tableGraphAttrDesc, te::dt::VAR_STRING);
  prop_desc->setSize(128);
  dt->add(prop_desc);

  //define a primary key
  std::string pkName = Globals::sm_tableGraphName;
              pkName += "_pk";

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dt);
  pk->add(prop_id);

  //create index
  std::string idxName = Globals::sm_tableGraphAttrId;
              idxName += "_idx";
  te::da::Index* index = new te::da::Index(idxName, te::da::B_TREE_TYPE, dt);
  index->add(prop_id);

  createTable(Globals::sm_tableGraphName, dt);
}

void te::graph::DataSourceGraphMetadata::createGraphAttributesTable()
{
  te::da::DataSetType* dt = new te::da::DataSetType(Globals::sm_tableAttributeName);

  //first property: id
  te::dt::SimpleProperty* prop_id = new te::dt::SimpleProperty(Globals::sm_tableAttributeAttrId, te::dt::INT32_TYPE, true);
  prop_id->setAutoNumber(true);
  dt->add(prop_id);

  //second property: id
  te::dt::SimpleProperty* prop_graphId = new te::dt::SimpleProperty(Globals::sm_tableAttributeAttrGraphId, te::dt::INT32_TYPE, true);
  dt->add(prop_graphId);

  //third property: table
  te::dt::StringProperty* prop_table = new te::dt::StringProperty(Globals::sm_tableAttributeAttrTable, te::dt::VAR_STRING);
  prop_table->setSize(32);
  dt->add(prop_table);

  //fourth property: column
  te::dt::StringProperty* prop_column = new te::dt::StringProperty(Globals::sm_tableAttributeAttrColumn, te::dt::VAR_STRING);
  prop_column->setSize(32);
  dt->add(prop_column);

  //fifth property: link name
  te::dt::StringProperty* prop_link = new te::dt::StringProperty(Globals::sm_tableAttributeAttrLink, te::dt::VAR_STRING);
  prop_link->setSize(32);
  dt->add(prop_link);

  //sixth property: type
  te::dt::SimpleProperty* prop_type = new te::dt::SimpleProperty(Globals::sm_tableAttributeAttrType, te::dt::INT32_TYPE, false);
  dt->add(prop_type);

  //define a primary key
  std::string pkName = Globals::sm_tableAttributeName;
              pkName += "_pk";

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dt);
  pk->add(prop_id);

  //create index
  std::string idxName = Globals::sm_tableAttributeAttrId;
              idxName += "_attr_idx";
  te::da::Index* index = new te::da::Index(idxName, te::da::B_TREE_TYPE, dt);
  index->add(prop_id);

  createTable(Globals::sm_tableAttributeName, dt);
}

void te::graph::DataSourceGraphMetadata::createGraphTableVertexModel()
{
  throw Exception(TR_GRAPH("TO DO."));
}

void te::graph::DataSourceGraphMetadata::createGraphTableEdgeModel()
{
  std::string tableName = this->getName() + Globals::sm_tableEdgeModelSufixName;

  te::da::DataSetType* dt = new te::da::DataSetType(tableName);

  //first property: id
  te::dt::SimpleProperty* prop_edgeId = new te::dt::SimpleProperty(Globals::sm_tableEdgeModelAttrId, te::dt::INT32_TYPE, true);
  prop_edgeId->setAutoNumber(true);
  dt->add(prop_edgeId);

  //second property: id
  te::dt::SimpleProperty* prop_vFrom = new te::dt::SimpleProperty(Globals::sm_tableEdgeModelAttrVFrom, te::dt::INT32_TYPE, true);
  dt->add(prop_vFrom);

  //third property: id
  te::dt::SimpleProperty* prop_vTo = new te::dt::SimpleProperty(Globals::sm_tableEdgeModelAttrVTo, te::dt::INT32_TYPE, true);
  dt->add(prop_vTo);

  //add edge properties
  te::gm::GeometryProperty* geomProp = 0;

  if(getEdgePropertySize() != 0)
  {
    for(int i = 0; i < getEdgePropertySize(); ++i)
    {
      te::dt::Property* p = getEdgeProperty(i)->clone();
      p->setParent(0);

      dt->add(p);
        
      if(p->getType() == te::dt::GEOMETRY_TYPE)
      {
        geomProp = (te::gm::GeometryProperty*)p;
      }
    }
  }

  //define a primary key
  std::string pkName = tableName;
              pkName += "_pk";

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dt);
  pk->add(prop_edgeId);

  //create index
  std::string idxEdge = tableName;
              idxEdge += "_edge_idx";
  te::da::Index* indexEdge = new te::da::Index(idxEdge, te::da::B_TREE_TYPE, dt);
  indexEdge->add(prop_edgeId);

  std::string idxVFrom = tableName;
              idxVFrom += "_vfrom_idx";
  te::da::Index* indexVFrom = new te::da::Index(idxVFrom, te::da::B_TREE_TYPE, dt);
  indexVFrom->add(prop_vFrom);

  std::string idxVTo = tableName;
              idxVTo += "_vto_idx";
  te::da::Index* indexVTo = new te::da::Index(idxVTo, te::da::B_TREE_TYPE, dt);
  indexVTo->add(prop_vTo);

  //create spatial index
  if(geomProp)
  {
    std::string idxSpatial = tableName;
                idxSpatial += "_spatial_idx";
    te::da::Index* idx = new te::da::Index(idxSpatial, te::da::R_TREE_TYPE, dt); 
    idx->add((te::dt::Property*)geomProp);   
  }

  createTable(tableName, dt);
}

void te::graph::DataSourceGraphMetadata::createVertexAttrTable()
{
  std::string tableName = this->getName() + Globals::sm_tableVertexAttributeModelSufixName;

  te::da::DataSetType* dt = new te::da::DataSetType(tableName);

  //first property: id
  te::dt::SimpleProperty* prop_Id = new te::dt::SimpleProperty(Globals::sm_tableVertexModelAttrId, te::dt::INT32_TYPE, true);
  prop_Id->setAutoNumber(true);
  dt->add(prop_Id);

  te::gm::GeometryProperty* geomProp = 0;

  if(getVertexPropertySize() != 0)
  {
    for(int i = 0; i < getVertexPropertySize(); ++i)
    {
      te::dt::Property* p = getVertexProperty(i)->clone();
      p->setParent(0);

      dt->add(p);
        
      if(p->getType() == te::dt::GEOMETRY_TYPE)
      {
        geomProp = (te::gm::GeometryProperty*)p;
      }
    }
  }

  //define a primary key
  std::string pkName = tableName;
              pkName += "_pk";
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dt);
  pk->add(prop_Id);

  //create index
  std::string idxName = tableName;
              idxName += "_idx";
  te::da::Index* index = new te::da::Index(idxName, te::da::B_TREE_TYPE, dt);
  index->add(prop_Id);

  //create spatial index
  if(geomProp)
  {
    std::string idxSpatial = tableName;
                idxSpatial += "_spatial_idx";
    te::da::Index* idx = new te::da::Index(idxSpatial, te::da::R_TREE_TYPE, dt); 
    idx->add((te::dt::Property*)geomProp);   
  }

  createTable(tableName, dt);
}

void te::graph::DataSourceGraphMetadata::createEdgeAttrTable()
{
  throw Exception(TR_GRAPH("TO DO."));
}

void te::graph::DataSourceGraphMetadata::createTable(std::string tableName, te::da::DataSetType* dt)
{
  if(m_ds->dataSetExists(tableName))
  {
    delete dt;
    return;
  }

  std::map<std::string, std::string> options;

  m_ds->createDataSet(dt, options);
}

bool te::graph::DataSourceGraphMetadata::isValidGraphName(std::string graphName)
{
  return true;
}

void te::graph::DataSourceGraphMetadata::addGraphTableNewEntry()
{
  //create the graph metadata table model
  createGraphMetadataTable();

  //get dataset type
  std::auto_ptr<te::da::DataSetType> dsType(m_ds->getDataSetType(Globals::sm_tableGraphName));

  std::auto_ptr<te::mem::DataSet> dsMem(new te::mem::DataSet(dsType.get()));

  //create new item
  te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem.get());

  dsItem->setString(1, this->getName());
  dsItem->setInt32(2, (int)this->getStorageMode());

  if(this->getStorageMode() == te::graph::Edge_List)
  {
    dsItem->setString(3, this->getEdgeTableName());
  }
  else if(this->getStorageMode() == te::graph::Vertex_List)
  {
    dsItem->setString(3, this->getVertexTableName());
  }

  dsItem->setString(4, this->getDescription());

  dsMem->add(dsItem);

  //add item
  std::map<std::string, std::string> options;

  dsMem->moveBeforeFirst();

  m_ds->add(Globals::sm_tableGraphName, dsMem.get(), options);

  //clear
  dsMem->clear();

  //update graph id
  updateGraphId();
}

void te::graph::DataSourceGraphMetadata::addGraphAttrTableNewEntry()
{
  createGraphAttributesTable();

  //add edge attributes
  if(getEdgePropertySize() != 0)
  {
    std::string tableName = this->getEdgeTableName();

    for(int i = 0; i < getEdgePropertySize(); ++i)
    {
      saveGraphAttrTableNewEntry(this->getId(), tableName, getEdgeProperty(i)->getName(), Globals::sm_tableEdgeModelAttrId, te::graph::Edge_Attr);
    }
  }

  //add vertex attributes
  if(getVertexPropertySize() != 0)
  {
    std::string tableName = this->getVertexTableName();

    for(int i = 0; i < getVertexPropertySize(); ++i)
    {
      saveGraphAttrTableNewEntry(this->getId(), tableName, getVertexProperty(i)->getName(), Globals::sm_tableVertexModelAttrId, te::graph::Vertex_Attr);
    }
  }
}

void te::graph::DataSourceGraphMetadata::saveGraphAttrTableNewEntry(int graphId, std::string tableName, std::string attrName, std::string linkColumn, te::graph::GraphAttrType type)
{
  //get dataset type
  std::auto_ptr<te::da::DataSetType> dsType(m_ds->getDataSetType(Globals::sm_tableAttributeName));

  std::auto_ptr<te::mem::DataSet> dsMem(new te::mem::DataSet(dsType.get()));

  //create datase item
  te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem.get());

  dsItem->setInt32(0, graphId);
  dsItem->setString(1, tableName);
  dsItem->setString(2, te::common::Convert2LCase(attrName));
  dsItem->setString(3, te::common::Convert2LCase(linkColumn));
  dsItem->setInt32(4, type);

  //add item
  std::map<std::string, std::string> options;

  m_ds->add(Globals::sm_tableAttributeName, dsMem.get(), options);

  //clear
  dsMem->clear();
}

void te::graph::DataSourceGraphMetadata::saveProperty(std::string tableName, te::dt::Property* p)
{
  //get dataset type
  std::auto_ptr<te::da::DataSetType> dsType(m_ds->getDataSetType(tableName));

  //add property
  m_ds->addProperty(tableName, p);

  //if property is a geometry property
  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    //create index
    std::string idxSpatial = tableName;
                idxSpatial += "_spatial_idx";
    te::da::Index* idx = new te::da::Index(idxSpatial, te::da::R_TREE_TYPE, dsType.get()); 
    idx->add(p);

    //add index
    std::map<std::string, std::string> options;

    m_ds->addIndex(tableName, idx, options);
  }
}

void te::graph::DataSourceGraphMetadata::removeProperty(std::string tableName, std::string propertyName)
{
  m_ds->dropProperty(tableName, propertyName);
}

void te::graph::DataSourceGraphMetadata::loadGraphInfo(int id)
{
  //load graph info
  te::da::Field* f = new te::da::Field("*");
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(Globals::sm_tableGraphName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::LiteralInt32* l = new te::da::LiteralInt32(id);

  te::da::EqualTo* equal = new te::da::EqualTo(new te::da::PropertyName(Globals::sm_tableGraphAttrId), l);

  te::da::Where* filter = new te::da::Where(equal);

  te::da::Select select(fields, from, filter);

  //get dataset result
  std::auto_ptr<te::da::DataSet> dataset = m_ds->query(select);

  if(dataset->moveNext())
  {
    m_id = dataset->getInt32(Globals::sm_tableGraphAttrId);
    m_name = dataset->getString(Globals::sm_tableGraphAttrName);
    m_description = dataset->getString(Globals::sm_tableGraphAttrDesc);

    int type = dataset->getInt32(Globals::sm_tableGraphAttrType);
    if(type == te::graph::Edge_List)
    {
      m_mode = te::graph::Edge_List;
    }
    else if(type == te::graph::Vertex_List)
    {
      m_mode =te::graph::Vertex_List;
    }
  }
}

void te::graph::DataSourceGraphMetadata::loadGraphAttrInfo(int id)
{
  //load attributes
  te::da::Field* f = new te::da::Field("*");
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(Globals::sm_tableAttributeName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::LiteralInt32* l = new te::da::LiteralInt32(id);

  te::da::EqualTo* equal = new te::da::EqualTo(new te::da::PropertyName(Globals::sm_tableAttributeAttrGraphId), l);

  te::da::Where* filter = new te::da::Where(equal);

  te::da::Select select(fields, from, filter);

  //get dataset result
  std::auto_ptr<te::da::DataSet> dataset = m_ds->query(select);

  while(dataset->moveNext())
  {
    int type = dataset->getInt32(Globals::sm_tableAttributeAttrType);

    std::string tableName = dataset->getString(Globals::sm_tableAttributeAttrTable);
    std::string columnName = dataset->getString(Globals::sm_tableAttributeAttrColumn);

    if(type == 0) // vertex attribute
    {
      loadVertexAttr(tableName, columnName);
    }
    else if(type == 1) // edge attribute
    {
      loadEdgeAttr(tableName, columnName);
    }
  }
}

void te::graph::DataSourceGraphMetadata::loadVertexAttr(std::string tableName, std::string columnName)
{
  //get property
  std::auto_ptr<te::dt::Property> p = m_ds->getProperty(tableName, columnName);

  te::dt::Property* myProp = p.release();

  te::graph::GraphMetadata::addVertexProperty(myProp);
}

void te::graph::DataSourceGraphMetadata::loadEdgeAttr(std::string tableName, std::string columnName)
{
  //get property
  std::auto_ptr<te::dt::Property> p = m_ds->getProperty(tableName, columnName);

  te::dt::Property* myProp = p.release();

  te::graph::GraphMetadata::addEdgeProperty(myProp);
}

void te::graph::DataSourceGraphMetadata::updateGraphId()
{
  te::da::Field* f = new te::da::Field(new te::da::PropertyName(Globals::sm_tableGraphAttrId));
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(Globals::sm_tableGraphName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::LiteralString* l = new te::da::LiteralString(this->getName());

  te::da::EqualTo* equal = new te::da::EqualTo(new te::da::PropertyName(Globals::sm_tableGraphAttrName), l);

  te::da::Where* filter = new te::da::Where(equal);

  te::da::Select select(fields, from, filter);

  std::auto_ptr<te::da::DataSet> dataset = m_ds->query(select);

  if(dataset->moveNext())
  {
    int id = dataset->getInt32(Globals::sm_tableGraphAttrId);
  
    m_id = id;
  }
}

/*
      GRAPH PACKAGE - DATABASE MODEL

-- create graph metadata table

CREATE TABLE te_graph (
  graph_id serial NOT NULL,
  graph_name VARCHAR NULL,
  graph_type INTEGER NULL,
  graph_table_name VARCHAR NULL,
  graph_description VARCHAR NULL,
  PRIMARY KEY(graph_id)
);

-- create graph attributes

CREATE TABLE te_graph_attr (
  attr_id serial NOT NULL,
  graph_id INTEGER NOT NULL,
  attr_table VARCHAR NULL,
  attr_column VARCHAR NULL,
  attr_link_column VARCHAR NULL,
  attribute_type INTEGER NULL,
  PRIMARY KEY(attr_id)
);


-- create graph table model edge list

CREATE TABLE graph_model_edge (
  edge_id serial NOT NULL,
  vertex_from INT NULL,
  vertex_to INT NULL,
  PRIMARY KEY(edge_id)
);

-- create graph table model vertex list

CREATE TABLE graph_model_vertex (
  vertex_id serial NOT NULL,
  edges_in bytea NULL,
  edges_out bytea NULL,
  PRIMARY KEY(vertex_id)
);

-- create vertex table properties

CREATE TABLE graph_vertex_properties (
  vertex_id serial NOT NULL,
  geometry bytea NULL,
  PRIMARY KEY(vertex_id)
);

-- create edge table properties

CREATE TABLE graph_edge_properties (
  edge_id serial NOT NULL,
  geometry bytea NULL,
  PRIMARY KEY(edge_id)
);

-- create graph algorithms table

CREATE TABLE te_graph_algorithms (
  algol_id serial NOT NULL,
  name VARCHAR NULL,
  description VARCHAR NULL,
  PRIMARY KEY(algol_id)
);

-- create graph algorithms params table

CREATE TABLE te_graph_algorithms_params (
  algol_id serial NOT NULL,
  parameter_number serial NOT NULL,
  parameter_name VARCHAR NULL,
  parameter_description VARCHAR NULL,
  PRIMARY KEY(algol_id, parameter_number)
);

-- create the table with relation between graphs and algorithms

CREATE TABLE graph_algorithm (
  graph_id INTEGER NOT NULL,
  algol_id INTEGER NOT NULL,
  parameter_number INTEGER NOT NULL,
  attribute_id INTEGER NOT NULL,
  attribute_value VARCHAR NULL,
  PRIMARY KEY(graph_id, algol_id, parameter_number, attribute_id)
);
*/