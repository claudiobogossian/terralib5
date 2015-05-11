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
  \file SequenceIterator.cpp

  \brief 
*/

// Terralib Includes
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/query_h.h"
#include "../core/GraphMetadata.h"
#include "../Config.h"
#include "../Exception.h"
#include "../Globals.h"
#include "SequenceIterator.h"

te::graph::SequenceIterator::SequenceIterator(te::graph::AbstractGraph* g) : te::graph::AbstractIterator(g)
{
}

te::graph::SequenceIterator::~SequenceIterator()
{
}

te::graph::Vertex* te::graph::SequenceIterator::getFirstVertex()
{
  if(m_graph == 0 || m_graph->getMetadata() == 0)
  {
    throw Exception(TE_TR("Invalid graph pointer."));
  }

  if(m_vertexQuery.get())
  {
    if(m_vertexQuery->moveFirst())
    {
      int id = m_vertexQuery->getInt32(Globals::sm_tableVertexModelAttrId);

      return m_graph->getVertex(id);
    }
  }

  if(m_listIsolatedVertex)
  {
    //create a query to get eache vertex id
    std::string tableName = m_graph->getMetadata()->getVertexTableName();

    te::da::Field* f = new te::da::Field(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
    te::da::Fields* fields = new te::da::Fields;
    fields->push_back(f);

    te::da::FromItem* t = new te::da::DataSetName(tableName);
    te::da::From* from = new te::da::From;
    from->push_back(t);

    te::da::OrderByItem* obItem = new te::da::OrderByItem(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
    te::da::OrderBy* ob = new te::da::OrderBy();
    ob->push_back(obItem);

    te::da::Select select(fields, from, ob);

    m_vertexQuery.reset(0);
    m_vertexQuery = m_graph->getMetadata()->getDataSource()->query(select);
  }
  else
  {
    //create a query to get eache vertex id
    std::string tableName = m_graph->getMetadata()->getVertexTableName();
    std::string edgeAttrTable = m_graph->getMetadata()->getEdgeTableName();

    te::da::Field* f = new te::da::Field(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
    te::da::Fields* fields = new te::da::Fields;
    fields->push_back(f);

    te::da::From* from = new te::da::From;

    te::da::FromItem* t = new te::da::DataSetName(tableName, "vertex");
    from->push_back(t);

    te::da::FromItem* fiEdge = new te::da::DataSetName(edgeAttrTable, "edge");
    from->push_back(fiEdge);

    std::string vertexFrom = "edge.";
                vertexFrom += Globals::sm_tableEdgeModelAttrVFrom;

    std::string vertexTo = "edge.";
                vertexTo += Globals::sm_tableEdgeModelAttrVTo;

    std::string vId = "vertex.";
                vId += Globals::sm_tableVertexModelAttrId;

    te::da::Field* fvf = new te::da::Field(vertexFrom);
    te::da::Field* fv1id = new te::da::Field(vId);
    te::da::Expression* exp1 = new te::da::EqualTo(fv1id->getExpression(), fvf->getExpression());

    te::da::Field* fvt = new te::da::Field(vertexTo);
    te::da::Field* fv2id = new te::da::Field(vId);
    te::da::Expression* exp2 = new te::da::EqualTo(fv2id->getExpression(), fvt->getExpression());

    te::da::Or* ora = new te::da::Or(exp1, exp2);

    te::da::Where* wh = new te::da::Where(ora);

    te::da::GroupByItem* gbItem = new te::da::GroupByItem(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
    te::da::GroupBy* gb = new te::da::GroupBy();
    gb->push_back(gbItem);

    te::da::OrderByItem* obItem = new te::da::OrderByItem(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId));
    te::da::OrderBy* ob = new te::da::OrderBy();
    ob->push_back(obItem);

    te::da::Select select(fields, from, wh, gb, ob);

    m_vertexQuery.reset(0);
    m_vertexQuery = m_graph->getMetadata()->getDataSource()->query(select);
  }

  if(m_vertexQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_vertexQuery->moveNext())
  {
    id = m_vertexQuery->getInt32(Globals::sm_tableVertexModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getVertex(id);
}

te::graph::Edge* te::graph::SequenceIterator::getFirstEdge()
{
  if(m_graph == 0 || m_graph->getMetadata() == 0)
  {
    throw Exception(TE_TR("Invalid graph pointer."));
  }

  if(m_edgeQuery.get())
  {
    if(m_edgeQuery->moveFirst())
    {
      int id = m_edgeQuery->getInt32(Globals::sm_tableEdgeModelAttrId);

      return m_graph->getEdge(id);
    }
  }

  //create a query to get eache vertex id
  std::string tableName = m_graph->getMetadata()->getEdgeTableName();

  te::da::Field* f = new te::da::Field(new te::da::PropertyName(Globals::sm_tableEdgeModelAttrId));
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(tableName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::OrderByItem* obItem = new te::da::OrderByItem(new te::da::PropertyName(Globals::sm_tableEdgeModelAttrId));
  te::da::OrderBy* ob = new te::da::OrderBy();
  ob->push_back(obItem);

  te::da::Select select(fields, from, ob);

  m_edgeQuery.reset(0);
  m_edgeQuery = m_graph->getMetadata()->getDataSource()->query(select);

  if(m_edgeQuery.get() == 0)
  {
    throw Exception(TE_TR("Iterator not initialized."));
  }

  int id;

  if(m_edgeQuery->moveNext())
  {
    id = m_edgeQuery->getInt32(Globals::sm_tableEdgeModelAttrId);
  }
  else
  {
    return 0;
  }

  return m_graph->getEdge(id);
}
