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
  \file SequenceLoaderStrategy.cpp

  \brief This class implements the main functions necessary to
        save and load the graph data and metadata information
        using as strategy a "order by" to create a sequence
        of objects.
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/query_h.h"
#include "../../datatype/AbstractData.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/GraphCache.h"
#include "../core/EdgeProperty.h"
#include "../core/GraphData.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../Config.h"
#include "../Globals.h"
#include "../Exception.h"
#include "SequenceLoaderStrategy.h"


te::graph::SequenceLoaderStrategy::SequenceLoaderStrategy(te::graph::GraphMetadata* metadata) : AbstractGraphLoaderStrategy(metadata)
{
}

te::graph::SequenceLoaderStrategy::~SequenceLoaderStrategy()
{
}


void te::graph::SequenceLoaderStrategy::loadDataByVertexId(int vertexId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  //ONLY WORKS FOR te::graph::Edge_List
  if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    throw Exception(TE_TR("TO DO"));
  }

  //get the table names
  std::string vertexAttrTable = m_graphMetadata->getVertexTableName();
  std::string edgeAttrTable = m_graphMetadata->getEdgeTableName();

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
  te::da::Field* fvtBegin = new te::da::Field(Globals::sm_tableVertexModelAttrId);
  te::da::GreaterThanOrEqualTo* gtetvt = new te::da::GreaterThanOrEqualTo(fvtBegin->getExpression(), new te::da::LiteralInt32(vertexId));

  te::da::Field* fvtEnd = new te::da::Field(Globals::sm_tableVertexModelAttrId);
  te::da::LessThanOrEqualTo* ltetvt = new te::da::LessThanOrEqualTo(fvtEnd->getExpression(), new te::da::LiteralInt32(vertexId + m_graphMetadata->m_maxCacheSize));

  te::da::And* andop = new te::da::And(gtetvt, ltetvt);

  te::da::Field* fvf = new te::da::Field(vertexFrom);
  te::da::Field* fv1id = new te::da::Field(vId);
  te::da::Expression* exp1 = new te::da::EqualTo(fvf->getExpression(), fv1id->getExpression());

  te::da::Field* fvt = new te::da::Field(vertexTo);
  te::da::Field* fv2id = new te::da::Field(vId);
  te::da::Expression* exp2 = new te::da::EqualTo(fvt->getExpression(), fv2id->getExpression());

  te::da::Or* orop = new te::da::Or(exp1, exp2);

  te::da::And* andd = new te::da::And(andop, orop);

  te::da::Where* wh = new te::da::Where(andd);

  // order by
  te::da::OrderByItem* obItem = new te::da::OrderByItem(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
  te::da::OrderBy* ob = new te::da::OrderBy();
  ob->push_back(obItem);
  
  //select
  te::da::Select select(all, from, wh, ob);

  //query
  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  std::auto_ptr<te::da::DataSetType> vertexDsType = m_graphMetadata->getDataSource()->getDataSetType(vertexAttrTable);

  int vertexProperties = vertexDsType->getProperties().size();

  std::string graphType = g->getMetadata()->getType();

  te::graph::Vertex* v = 0;

  int currentId = -1;

  //list of all attributes: vertex table + edge table
  while(dataset->moveNext())
  {
    int vId   = dataset->getInt32(0);                       //first item is the vertex id
    int eId   = dataset->getInt32(vertexProperties);        //first after vertexProperties item is the edge id
    int vFrom = dataset->getInt32(vertexProperties + 1);    //second after vertexPropertie item is the vertex from id
    //int vTo   = dataset->getInt32(vertexProperties + 2);    //third after vertexPropertie item is the vertex to id

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
}


void te::graph::SequenceLoaderStrategy::loadDataByEdgeId(int edgeId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0 || g == 0)
  {
    throw Exception(TE_TR(""));
  }

  //ONLY WORKS FOR te::graph::Edge_List
  if(m_graphMetadata->getStorageMode() == te::graph::Vertex_List)
  {
    throw Exception(TE_TR("TO DO"));
  }

  //get the tables names
  std::string edgeTable = m_graphMetadata->getEdgeTableName();

  //get all id's from vertex and edges that is inside that box
  
  //filds
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));

  //from
  te::da::From* from = new te::da::From;

  te::da::FromItem* fi1 = new te::da::DataSetName(edgeTable);
  from->push_back(fi1);

  //where
  te::da::Field* feiBegin = new te::da::Field(Globals::sm_tableEdgeModelAttrId);
  te::da::GreaterThanOrEqualTo* gtet = new te::da::GreaterThanOrEqualTo(feiBegin->getExpression(), new te::da::LiteralInt32(edgeId));

  te::da::Field* feiEnd = new te::da::Field(Globals::sm_tableEdgeModelAttrId);
  te::da::LessThanOrEqualTo* ltet = new te::da::LessThanOrEqualTo(feiEnd->getExpression(), new te::da::LiteralInt32(edgeId + m_graphMetadata->m_maxCacheSize));

  te::da::And* andop = new te::da::And(gtet, ltet);

  te::da::Where* wh = new te::da::Where(andop);

  // order by
  te::da::OrderByItem* obItem = new te::da::OrderByItem(new te::da::PropertyName(Globals::sm_tableEdgeModelAttrId));
  te::da::OrderBy* ob = new te::da::OrderBy();
  ob->push_back(obItem);
  
  //select
  te::da::Select select(all, from, wh, ob);

  //query
  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  std::auto_ptr<te::da::DataSetType> edgeDsType = m_graphMetadata->getDataSource()->getDataSetType(edgeTable);

  int edgeProperties = edgeDsType->getProperties().size();

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

      e->setAttributeVecSize(edgeProperties - 3);

      for(int i = 3; i < edgeProperties; ++i)
      {
        e->addAttribute(i - 3, dataset->getValue(i).release());
      };

      g->add(e);
    }
  }
}
