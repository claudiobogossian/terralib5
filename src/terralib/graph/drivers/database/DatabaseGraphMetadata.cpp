#include "DatabaseGraphMetadata.h"

#include "EdgeProperty.h"
#include "VertexProperty.h"

// Terralib Includes
#include "Config.h"
#include "Exception.h"
#include "Globals.h"
#include "../common.h"
#include "../dataaccess.h"
#include "../datatype.h"
#include "../memory.h"
#include "../geometry.h"


te::graph::DatabaseGraphMetadata::DatabaseGraphMetadata(te::da::DataSource* ds) : te::graph::GraphMetadata(ds)
{
  m_mode = te::graph::Edge_List;
}

te::graph::DatabaseGraphMetadata::~DatabaseGraphMetadata()
{
}

void te::graph::DatabaseGraphMetadata::load(int id)
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

void te::graph::DatabaseGraphMetadata::save()
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

void te::graph::DatabaseGraphMetadata::update()
{
  if(m_ds == 0)
  {
    throw Exception(TR_GRAPH(""));
  }
}

void te::graph::DatabaseGraphMetadata::addVertexProperty(te::dt::Property* p)
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

void te::graph::DatabaseGraphMetadata::removeVertexProperty(int idx)
{
  te::dt::Property* p = te::graph::GraphMetadata::getVertexProperty(idx);

  std::string propertyName = p->getName();

  te::graph::GraphMetadata::removeVertexProperty(idx);

  //get data set
  std::string tableName = this->getVertexTableName();

  //remove property
  removeProperty(tableName, propertyName);
}

void te::graph::DatabaseGraphMetadata::addEdgeProperty(te::dt::Property* p)
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

void te::graph::DatabaseGraphMetadata::removeEdgeProperty(int idx)
{
  te::dt::Property* p = te::graph::GraphMetadata::getEdgeProperty(idx);

  std::string propertyName = p->getName();

  te::graph::GraphMetadata::removeEdgeProperty(idx);

  //get data set
  std::string tableName = this->getEdgeTableName();

  //remove property
  removeProperty(tableName, propertyName);
}

void te::graph::DatabaseGraphMetadata::createGraphMetadataTable()
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

void te::graph::DatabaseGraphMetadata::createGraphAttributesTable()
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

void te::graph::DatabaseGraphMetadata::createGraphTableVertexModel()
{
  throw Exception(TR_GRAPH("TO DO."));
}

void te::graph::DatabaseGraphMetadata::createGraphTableEdgeModel()
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

        dt->setDefaultGeomProperty(geomProp);
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

void te::graph::DatabaseGraphMetadata::createVertexAttrTable()
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

        dt->setDefaultGeomProperty(geomProp);
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

void te::graph::DatabaseGraphMetadata::createEdgeAttrTable()
{
  throw Exception(TR_GRAPH("TO DO."));
}

void te::graph::DatabaseGraphMetadata::createTable(std::string tableName, te::da::DataSetType* dt)
{
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(transactor)
  {
    te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

    if(catalog == 0)
    {
      throw Exception(TR_GRAPH("Error getting Catalog Loader."));
    }

    if(catalog->datasetExists(tableName))
    {
      delete dt;
      delete catalog;
      delete transactor;
      return;
    }

    delete catalog;
  }
  else
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  //create a dataset type
  te::da::DataSetTypePersistence* persistence = transactor->getDataSetTypePersistence();

  persistence->create(dt);

  delete dt;
  delete persistence;
  delete transactor;
}

bool te::graph::DatabaseGraphMetadata::isValidGraphName(std::string graphName)
{
  return true;
}

void te::graph::DatabaseGraphMetadata::addGraphTableNewEntry()
{
  //create the graph metadata table model
  createGraphMetadataTable();

  //get transactor
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  //get data set 
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(Globals::sm_tableGraphName, true);

  //create new item
  te::da::DataSetItem* dsItem = new te::mem::DataSetItem((te::da::DataSetType*)ds->clone());

  dsItem->setString(0, this->getName());
  dsItem->setInt32(1, (int)this->getStorageMode());

  if(this->getStorageMode() == te::graph::Edge_List)
  {
    dsItem->setString(2, this->getEdgeTableName());
  }
  else if(this->getStorageMode() == te::graph::Vertex_List)
  {
    dsItem->setString(2, this->getVertexTableName());
  }

  dsItem->setString(3, this->getDescription());

  //get persistence and add new item
  te::da::DataSetPersistence* dsP = transactor->getDataSetPersistence();

  te::da::DataSetType* dsType = (te::da::DataSetType*)ds->clone();

  dsType->remove(dsType->getProperty(0));

  dsP->add(dsType, dsItem);

  delete dsItem;
  delete dsType;
  delete dsP;
  delete ds;
  delete transactor;

  //update graph id
  updateGraphId();
}

void te::graph::DatabaseGraphMetadata::addGraphAttrTableNewEntry()
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

void te::graph::DatabaseGraphMetadata::saveGraphAttrTableNewEntry(int graphId, std::string tableName, std::string attrName, std::string linkColumn, te::graph::GraphAttrType type)
{
//get transactor
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  //get data set
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(Globals::sm_tableAttributeName, true);

  //get persistence and add new item
  te::da::DataSetPersistence* dsP = transactor->getDataSetPersistence();

  te::da::DataSetType* dsType = (te::da::DataSetType*)ds->clone();

  dsType->remove(dsType->getProperty(0));

  te::da::DataSetItem* dsItem = new te::mem::DataSetItem(ds);
    
  dsItem->setInt32(0, graphId);
  dsItem->setString(1, tableName);
  dsItem->setString(2, te::common::Convert2LCase(attrName));
  dsItem->setString(3, te::common::Convert2LCase(linkColumn));
  dsItem->setInt32(4, type);

  dsP->add(dsType, dsItem);

  delete dsItem;
  delete dsType;
  delete dsP;
  delete catalog;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::saveProperty(std::string tableName, te::dt::Property* p)
{
 // get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }
  
  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName,true);

  te::da::DataSetTypePersistence* dsP = transactor->getDataSetTypePersistence();

  te::dt::Property* pClone = p->clone();
  pClone->setParent(0);

  dsP->add(ds, pClone);

  if(pClone->getType() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::GeometryProperty* geomProp = (te::gm::GeometryProperty*)pClone;

    ds->setDefaultGeomProperty(geomProp);

    std::string idxSpatial = tableName;
                idxSpatial += "_spatial_idx";
    te::da::Index* idx = new te::da::Index(idxSpatial, te::da::R_TREE_TYPE, ds); 
    idx->add((te::dt::Property*)geomProp);

    dsP->add(ds, idx);
  }

  delete dsP;
  delete catalog;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::removeProperty(std::string tableName, std::string propertyName)
{
// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName,true);

  te::dt::Property* pds = ds->getProperty(propertyName);

  te::da::DataSetTypePersistence* dsP = transactor->getDataSetTypePersistence();

  dsP->drop(pds);

  delete dsP;
  delete catalog;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::loadGraphInfo(int id)
{
  //get transactor
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

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

  te::da::DataSet* dataset = transactor->query(select);

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

  delete dataset;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::loadGraphAttrInfo(int id)
{
  //get transactor
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

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

  te::da::DataSet* dataset = transactor->query(select);

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

  delete dataset;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::loadVertexAttr(std::string tableName, std::string columnName)
{
// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

  te::dt::Property* p = ds->getProperty(columnName)->clone();

  te::graph::GraphMetadata::addVertexProperty(p);

  delete catalog;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::loadEdgeAttr(std::string tableName, std::string columnName)
{
// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

  te::dt::Property* p = ds->getProperty(columnName)->clone();

  te::graph::GraphMetadata::addEdgeProperty(p);

  delete catalog;
  delete transactor;
}

void te::graph::DatabaseGraphMetadata::updateGraphId()
{
  //get transactor
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

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

  te::da::DataSet* dataset = transactor->query(select);

  if(dataset->moveNext())
  {
    int id = dataset->getInt32(Globals::sm_tableGraphAttrId);
  
    m_id = id;
  }

  delete dataset;
  delete transactor;
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