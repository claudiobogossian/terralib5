/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file BoxLoaderStrategy.cpp

  \brief This class implements the main functions necessary to
        save and load the graph data and metadata information
        using as strategy a bounding box to create a region
        that defines a group of elements.
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/query_h.h"
#include "../../datatype/AbstractData.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Point.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/EdgeProperty.h"
#include "../core/GraphCache.h"
#include "../core/GraphData.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../Config.h"
#include "../Globals.h"
#include "../Exception.h"
#include "BoxLoaderStrategy.h"


te::graph::BoxLoaderStrategy::BoxLoaderStrategy(te::graph::GraphMetadata* metadata) : AbstractGraphLoaderStrategy(metadata)
{
}

te::graph::BoxLoaderStrategy::~BoxLoaderStrategy()
{
}


void te::graph::BoxLoaderStrategy::loadDataByVertexId(int vertexId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  Vertex* vAux = 0;

  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    vAux =  loadVertexAttrs(vertexId);
  }
  else if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    vAux =  loadVertex(vertexId); 
  }

  if(vAux == 0)
  {
    throw Exception(TE_TR("Vertex Id not found."));
  }

  //ONLY WORKS FOR te::graph::Edge_List
  if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    throw Exception(TE_TR("TO DO"));
  }

  //get the tables names
  std::string vertexAttrTable = m_graphMetadata->getVertexTableName();
  std::string edgeAttrTable = m_graphMetadata->getEdgeTableName();

  //get the vertex coord
  te::gm::Point* point = 0;
  std::string geometryAttrName = "";

  for(int i = 0; i < m_graphMetadata->getVertexPropertySize(); ++i)
  {
    if(m_graphMetadata->getVertexProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      geometryAttrName = m_graphMetadata->getVertexProperty(i)->getName();

      point = dynamic_cast<te::gm::Point*>(vAux->getAttributes()[i]);

      break;
    }
  }

  assert(point);

  //calculate box
  te::gm::Envelope* envelope = calculateBox(point, vertexAttrTable);

  //get all id's from vertex and edges that is inside that box
  
  //filds
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));

  //from
  te::da::From* from = new te::da::From;

  te::da::FromItem* fi = new te::da::DataSetName(vertexAttrTable, "vertex");
  from->push_back(fi);

  te::da::FromItem* fiEdge = new te::da::DataSetName(edgeAttrTable, "edge");
  from->push_back(fiEdge);

  std::string vertexFrom = "edge.";
              vertexFrom += Globals::sm_tableEdgeModelAttrVFrom;

  std::string vertexTo = "edge.";
              vertexTo += Globals::sm_tableEdgeModelAttrVTo;

  std::string vId = "vertex.";
              vId += Globals::sm_tableVertexModelAttrId;

  //where
  te::da::Field* fvf = new te::da::Field(vertexFrom);
  te::da::Field* fv1id = new te::da::Field(vId);
  te::da::Expression* exp1 = new te::da::EqualTo(fv1id->getExpression(), fvf->getExpression());

  te::da::Field* fvt = new te::da::Field(vertexTo);
  te::da::Field* fv2id = new te::da::Field(vId);
  te::da::Expression* exp2 = new te::da::EqualTo(fv2id->getExpression(), fvt->getExpression());

  te::da::Or* ora = new te::da::Or(exp1, exp2);

  
  std::string vEttr = "vertex." + geometryAttrName;

  int srid = point->getSRID();
  te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(*envelope, srid);
  te::da::Field* fvattr = new te::da::Field(vEttr);
  te::da::ST_Intersects* intersects = new te::da::ST_Intersects(fvattr->getExpression(), lenv);

  te::da::And* andd = new te::da::And(intersects, ora);

  te::da::Where* wh = new te::da::Where(andd);
  
  //select
  te::da::Select select(all, from, wh);

  //query
  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  std::auto_ptr<te::da::DataSetType> vertexDsType = m_graphMetadata->getDataSource()->getDataSetType(vertexAttrTable);

  int vertexProperties = vertexDsType->getProperties().size();

  std::string graphType = g->getMetadata()->getType();

  te::graph::Vertex* v = 0;

  int currentId = -1;

  //list of all attributes: edge table + vertex table + vertex table
  while(dataset->moveNext())
  {
    int vId   = dataset->getInt32(0);                       //first item is the vertex id
    int eId   = dataset->getInt32(vertexProperties);        //first after vertexProperties item is the edge id
    int vFrom = dataset->getInt32(vertexProperties + 1);    //second after vertexPropertie item is the vertex from id
//    int vTo   = dataset->getInt32(vertexProperties + 2);    //third after vertexPropertie item is the vertex to id

    if(currentId != vId)
    {
      //verify if its already in cache
      if(gc->checkCacheByVertexId(vId) == nullptr)
      {
        v = new te::graph::Vertex(vId, false);

        v->setAttributeVecSize(vertexProperties - 1);

        for(int i = 1; i < vertexProperties; ++i)
        {
          v->addAttribute(i - 1, dataset->getValue(i).release());
        }

        g->add(v);
      }

      currentId = vId;
    }

    if(v)
    {
      if(graphType == TE_GRAPH_FACTORY_GRAPH_TYPE_BIDIRECTIONALGRAPH)
      {
        if(vId == vFrom)
          v->getSuccessors().insert(eId);
        else
          v->getPredecessors().insert(eId);
      }

      //TODO for other graph types
    }
  }

  delete vAux;
  delete envelope;
}


void te::graph::BoxLoaderStrategy::loadDataByEdgeId(int edgeId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  Edge* e = 0;

  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    e =  loadEdge(edgeId);
  }
  else if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    e =  loadEdgeAttrs(edgeId);
  }

  if(e == 0)
  {
    throw Exception(TE_TR("Edge Id not found."));
  }

  Vertex* vAux = 0;

  int vAuxId = e->getIdFrom();

  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    vAux =  loadVertexAttrs(vAuxId);
  }
  else if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    vAux =  loadVertex(vAuxId); 
  }

  if(vAux == 0)
  {
    throw Exception(TE_TR("Vertex Id not found."));
  }

  delete e;

  //ONLY WORKS FOR te::graph::Edge_List
  if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    throw Exception(TE_TR("TO DO"));
  }

  //get the tables names
  std::string vertexAttrTable = m_graphMetadata->getVertexTableName();
  std::string edgeAttrTable = m_graphMetadata->getEdgeTableName();

  //get the vertex coord
  te::gm::Point* point = 0;
  std::string geometryAttrName = "";

  for(int i = 0; i < m_graphMetadata->getVertexPropertySize(); ++i)
  {
    if(m_graphMetadata->getVertexProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      geometryAttrName = m_graphMetadata->getVertexProperty(i)->getName();

      point = dynamic_cast<te::gm::Point*>(vAux->getAttributes()[i]);

      break;
    }
  }

  assert(point);

  //calculate box
  te::gm::Envelope* envelope = calculateBox(point, vertexAttrTable);

  //get all id's from vertex and edges that is inside that box
  
  //filds
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));

  //from
  te::da::From* from = new te::da::From;

  te::da::FromItem* fi = new te::da::DataSetName(vertexAttrTable, "vertex");
  from->push_back(fi);

  te::da::FromItem* fiEdge = new te::da::DataSetName(edgeAttrTable, "edge");
  from->push_back(fiEdge);

  std::string vertexFrom = "edge.";
              vertexFrom += Globals::sm_tableEdgeModelAttrVFrom;

  std::string vertexTo = "edge.";
              vertexTo += Globals::sm_tableEdgeModelAttrVTo;

  std::string vId = "vertex.";
              vId += Globals::sm_tableVertexModelAttrId;

  //where
  te::da::Field* fvf = new te::da::Field(vertexFrom);
  te::da::Field* fv1id = new te::da::Field(vId);
  te::da::Expression* exp1 = new te::da::EqualTo(fv1id->getExpression(), fvf->getExpression());

  te::da::Field* fvt = new te::da::Field(vertexTo);
  te::da::Field* fv2id = new te::da::Field(vId);
  te::da::Expression* exp2 = new te::da::EqualTo(fv2id->getExpression(), fvt->getExpression());

  te::da::Or* ora = new te::da::Or(exp1, exp2);

  
  std::string vEttr = "vertex." + geometryAttrName;

  int srid = point->getSRID();
  te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(*envelope, srid);
  te::da::Field* fvattr = new te::da::Field(vEttr);
  te::da::ST_Intersects* intersects = new te::da::ST_Intersects(fvattr->getExpression(), lenv);

  te::da::And* andd = new te::da::And(intersects, ora);

  te::da::Where* wh = new te::da::Where(andd);
  
  //select
  te::da::Select select(all, from, wh);

  //query
  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  std::auto_ptr<te::da::DataSetType> vertexDsType = m_graphMetadata->getDataSource()->getDataSetType(vertexAttrTable);
  std::auto_ptr<te::da::DataSetType> edgeDsType = m_graphMetadata->getDataSource()->getDataSetType(edgeAttrTable);

  int vertexProperties = vertexDsType->getProperties().size();
  int edgeProperties = edgeDsType->getProperties().size();

  std::string graphType = g->getMetadata()->getType();

  int currentId = -1;

  //list of all attributes: edge table + vertex table + vertex table
  while(dataset->moveNext())
  {
    int eId   = dataset->getInt32(vertexProperties);        //first after vertexProperties item is the edge id
    int vFrom = dataset->getInt32(vertexProperties + 1);    //second after vertexPropertie item is the vertex from id
    int vTo   = dataset->getInt32(vertexProperties + 2);    //third after vertexPropertie item is the vertex to id

    if(currentId != eId)
    {
      //verify if its already in cache
      if(gc->checkCacheByEdgeId(eId) == 0)
      {
        te::graph::Edge* e = new te::graph::Edge(eId, vFrom, vTo, false);

        e->setAttributeVecSize(edgeProperties - 3);

        for(int i = 3; i < edgeProperties; ++i)
        {
          e->addAttribute(i - 3, dataset->getValue(i + vertexProperties).release());
        };

        g->add(e);
      }

      currentId = eId;
    }
  }

  delete vAux;
  delete envelope;
}


te::gm::Envelope* te::graph::BoxLoaderStrategy::calculateBox(te::gm::Point* p, std::string tableName)
{
  te::gm::Envelope* e = getMetadata()->getEnvelope();

  double h = e->getHeight() / (100. / getMetadata()->m_boxPercentSize);
  double w = e->getWidth() / (100. / getMetadata()->m_boxPercentSize);

  te::gm::Envelope* env = new te::gm::Envelope();

  //env->m_llx = p->getX();
  //env->m_urx = p->getX() + w;
  //env->m_lly = p->getY() - h;
  //env->m_ury = p->getY();

  env->m_llx = p->getX() - ( w / 2.);
  env->m_urx = p->getX() + ( w / 2.);
  env->m_lly = p->getY() - ( h / 2.);
  env->m_ury = p->getY() + ( h / 2.);

  return env;
}


/*
void te::graph::BoxLoaderStrategy::loadDataByVertexId(int vertexId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  Vertex* vAux = 0;

  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    vAux =  loadVertexAttrs(vertexId);
  }
  else if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    vAux =  loadVertex(vertexId); 
  }

  if(vAux == 0)
  {
    throw Exception(TE_TR("Vertex Id not found."));
  }

  //ONLY WORKS FOR te::graph::Edge_List
  if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    throw Exception(TE_TR("TO DO"));
  }

  //get the tables names
  std::string edgeTable = m_graphMetadata->getEdgeTableName();
  std::string vertexAttrTalbe = m_graphMetadata->getVertexTableName();

  //get the vertex coord
  te::gm::Point* point = 0;
  std::string geometryAttrName = "";

  for(int i = 0; i < m_graphMetadata->getVertexPropertySize(); ++i)
  {
    if(m_graphMetadata->getVertexProperty(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      geometryAttrName = m_graphMetadata->getVertexProperty(i)->getName();

      point = dynamic_cast<te::gm::Point*>(vAux->getAttributes()[i]);

      break;
    }
  }

  assert(point);

  //calculate box
  te::gm::Envelope* envelope = calculateBox(point, vertexAttrTalbe);

  //get all id's from vertex and edges that is inside that box
  
  //filds
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));

  //from
  te::da::From* from = new te::da::From;

  te::da::FromItem* fi1 = new te::da::DataSetName(edgeTable, "edges");
  from->push_back(fi1);
  te::da::FromItem* fi2 = new te::da::DataSetName(vertexAttrTalbe, "v1");
  from->push_back(fi2);
  te::da::FromItem* fi3 = new te::da::DataSetName(vertexAttrTalbe, "v2");
  from->push_back(fi3);


  std::string vertexFrom = "edges.";
              vertexFrom += Globals::sm_tableEdgeModelAttrVFrom;

  std::string vertexTo = "edges.";
              vertexTo += Globals::sm_tableEdgeModelAttrVTo;

  std::string v1Id = "v1.";
              v1Id += Globals::sm_tableVertexModelAttrId;

  std::string v2Id = "v2.";
              v2Id += Globals::sm_tableVertexModelAttrId;

  //where
  te::da::Field* fvf = new te::da::Field(vertexFrom);
  te::da::Field* fv1id = new te::da::Field(v1Id);
  te::da::Expression* exp1 = new te::da::EqualTo(fvf->getExpression(), fv1id->getExpression());

  te::da::Field* fvt = new te::da::Field(vertexTo);
  te::da::Field* fv2id = new te::da::Field(v2Id);
  te::da::Expression* exp2 = new te::da::EqualTo(fvt->getExpression(), fv2id->getExpression());

  te::da::And* and = new te::da::And(exp1, exp2);

  
  std::string v1Attr = "v1." + geometryAttrName;
  std::string v2Attr = "v2." + geometryAttrName;

  int srid = point->getSRID();
  te::da::LiteralEnvelope* lenv1 = new te::da::LiteralEnvelope(*envelope, srid);
  te::da::LiteralEnvelope* lenv2 = new te::da::LiteralEnvelope(*envelope, srid);
  te::da::Field* fv1attr = new te::da::Field(v1Attr);
  te::da::Field* fv2attr = new te::da::Field(v2Attr);
  te::da::ST_Intersects* intersects1 = new te::da::ST_Intersects(fv1attr->getExpression(), lenv1);
  te::da::ST_Intersects* intersects2 = new te::da::ST_Intersects(fv2attr->getExpression(), lenv2);
  te::da::Or* or = new te::da::Or(intersects1, intersects2);

  te::da::And* andd = new te::da::And(and, or);

  te::da::Where* wh = new te::da::Where(andd);
  
  //select
  te::da::Select select(all, from, wh);

  //query
  te::da::DataSourceTransactor* transactor = m_graphMetadata->getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TE_TR("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSet* dataset = transactor->query(select);
  te::da::DataSetType* edgeDsType = catalog->getDataSetType(edgeTable);
  te::da::DataSetType* vertexDsType = catalog->getDataSetType(vertexAttrTalbe);

  int edgeProperties = edgeDsType->getProperties().size();
  int vertexProperties = vertexDsType->getProperties().size();

  //list of all attributes: edge table + vertex table + vertex table
  while(dataset->moveNext())
  {
    int edgeId = dataset->getInt32(0);    //first item is the edge id
    int vFromId = dataset->getInt32(1);   //second item is the vertefFrom id
    int vToId = dataset->getInt32(2);     //third item is the verteTo id
    
    //verify if its already in cache
    if(gc->checkCacheByEdgeId(edgeId) == 0)
    {
      te::graph::Edge* e = new te::graph::Edge(edgeId, vFromId, vToId, false);

      e->setAttributeVecSize(edgeDsType->getProperties().size() - 3);

      for(size_t i = 3; i < edgeDsType->getProperties().size(); ++i)
      {
        e->addAttribute(i - 3, dataset->getValue(i));
      };

      g->add(e);
    }

    if(gc->checkCacheByVertexId(vFromId) == false)
    {
      te::graph::Vertex* v = new te::graph::Vertex(vFromId, false);

      v->setAttributeVecSize(vertexDsType->getProperties().size() - 1);

      for(size_t i = 1; i < vertexDsType->getProperties().size(); ++i)
      {
        v->addAttribute(i - 1, dataset->getValue(i + edgeProperties));
      }

      g->add(v);
    }

    if(gc->checkCacheByVertexId(vToId) == false)
    {
      te::graph::Vertex* v = new te::graph::Vertex(vToId, false);

      v->setAttributeVecSize(vertexDsType->getProperties().size() - 1);

      for(size_t i = 1; i < vertexDsType->getProperties().size(); ++i)
      {
        v->addAttribute(i - 1, dataset->getValue(i + edgeProperties + vertexProperties));
      }

      g->add(v);
    }
  }

  delete vAux;
  delete envelope;
  delete dataset;
  delete catalog;
  delete transactor;
}


void te::graph::BoxLoaderStrategy::loadDataByEdgeId(int edgeId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  Edge* e = 0;

  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    e =  loadEdge(edgeId);
  }
  else if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    e =  loadEdgeAttrs(edgeId);
  }

  if(e == 0)
  {
    throw Exception(TE_TR("Edge Id not found."));
  }

  int vId = e->getIdFrom();

  delete e;

  loadDataByVertexId(vId, g, gc);
}
*/
