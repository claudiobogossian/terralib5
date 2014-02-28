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
  \file terralib/graph/Globals.cpp
   
  \brief An static class with global definitions for the TerraLib Graph Module.
*/

// TerraLib
#include "Globals.h"

const std::string te::graph::Globals::sm_moduleIdentifier(TE_GRAPH_MODULE_IDENTIFIER);

const std::string te::graph::Globals::sm_factoryGraphTypeGraph(TE_GRAPH_FACTORY_GRAPH_TYPE_GRAPH);
const std::string te::graph::Globals::sm_factoryGraphTypeBidirectionalGraph(TE_GRAPH_FACTORY_GRAPH_TYPE_BIDIRECTIONALGRAPH);
const std::string te::graph::Globals::sm_factoryGraphTypeDirectedGraph(TE_GRAPH_FACTORY_GRAPH_TYPE_DIRECTEDGRAPH);
const std::string te::graph::Globals::sm_factoryGraphTypeUndirectedGraph(TE_GRAPH_FACTORY_GRAPH_TYPE_UNDIRECTEDGRAPH);

const std::string te::graph::Globals::sm_factoryGraphTypeMemoryGraph(TE_GRAPH_FACTORY_GRAPH_TYPE_MEMORYGRAPH);

const std::string te::graph::Globals::sm_factoryCachePolicyTypeFIFO(TE_GRAPH_FACTORY_CACHEPOLICY_TYPE_FIFO);
const std::string te::graph::Globals::sm_factoryCachePolicyTypeLFU(TE_GRAPH_FACTORY_CACHEPOLICY_TYPE_LFU);

const std::string te::graph::Globals::sm_factoryLoaderStrategyTypeBox(TE_GRAPH_FACTORY_LOADERSTRATEGY_TYPE_BOX);
const std::string te::graph::Globals::sm_factoryLoaderStrategyTypeSequence(TE_GRAPH_FACTORY_LOADERSTRATEGY_TYPE_SEQUENCE);


const std::string te::graph::Globals::sm_graphFactoryDefaultObject(TE_DEFAULT_GRAPH_TYPE);
const std::string te::graph::Globals::sm_cachePolicyFactoryDefaultObject(TE_DEFAULT_CACHE_POLICY_TYPE);
const std::string te::graph::Globals::sm_loaderStrategyFactoryDefaultObject(TE_DEFAULT_GRAPH_LOADER_STRATEGY_TYPE);


const int te::graph::Globals::sm_graphCacheDefaultMaxSize(TE_GRAPH_DEFAULT_MAX_CACHE_SIZE);
const int te::graph::Globals::sm_graphVecCacheDefaultMaxSize(TE_GRAPH_DEFAULT_MAX_VEC_CACHE_SIZE);
const int te::graph::Globals::sm_boxLoaderStrategyDefaultSize(TE_GRAPH_DEFAULT_BOX_STRATEGY_LOADER_SIZE);


const std::string te::graph::Globals::sm_vertexStorageMode(TE_GRAPH_STORAGE_MODE_BY_VERTEX);
const std::string te::graph::Globals::sm_edgeStorageMode(TE_GRAPH_STORAGE_MODE_BY_EDGE);


const std::string te::graph::Globals::sm_tableGraphName(TE_GRAPH_GRAPH_TABLE_NAME);
const std::string te::graph::Globals::sm_tableGraphAttrId(TE_GRAPH_GRAPH_TABLE_ATTR_ID);
const std::string te::graph::Globals::sm_tableGraphAttrName(TE_GRAPH_GRAPH_TABLE_ATTR_NAME);
const std::string te::graph::Globals::sm_tableGraphAttrType(TE_GRAPH_GRAPH_TABLE_ATTR_TYPE);
const std::string te::graph::Globals::sm_tableGraphAttrTableName(TE_GRAPH_GRAPH_TABLE_ATTR_TABLE_NAME);
const std::string te::graph::Globals::sm_tableGraphAttrDesc(TE_GRAPH_GRAPH_TABLE_ATTR_DESCRIPTION);

const std::string te::graph::Globals::sm_tableAttributeName(TE_GRAPH_GRAPH_ATTR_TABLE_NAME);
const std::string te::graph::Globals::sm_tableAttributeAttrId(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_ID);
const std::string te::graph::Globals::sm_tableAttributeAttrGraphId(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_GRAPH_ID);
const std::string te::graph::Globals::sm_tableAttributeAttrTable(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_TABLE);
const std::string te::graph::Globals::sm_tableAttributeAttrColumn(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_COLUMN);
const std::string te::graph::Globals::sm_tableAttributeAttrLink(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_LINK);
const std::string te::graph::Globals::sm_tableAttributeAttrType(TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_TYPE);

const std::string te::graph::Globals::sm_tableEdgeModelSufixName(TE_GRAPH_GRAPH_TABLE_EDGE_SUFIX);
const std::string te::graph::Globals::sm_tableEdgeAttributeModelSufixName(TE_GRAPH_GRAPH_TABLE_ATTR_EDGE_SUFIX);
const std::string te::graph::Globals::sm_tableEdgeModelAttrId(TE_GRAPH_GRAPH_EDGE_MODEL_ID);
const std::string te::graph::Globals::sm_tableEdgeModelAttrVFrom(TE_GRAPH_GRAPH_EDGE_MODEL_VFROM);
const std::string te::graph::Globals::sm_tableEdgeModelAttrVTo(TE_GRAPH_GRAPH_EDGE_MODEL_VTO);

const std::string te::graph::Globals::sm_tableVertexModelSufixName(TE_GRAPH_GRAPH_TABLE_VERTEX_SUFIX);
const std::string te::graph::Globals::sm_tableVertexAttributeModelSufixName(TE_GRAPH_GRAPH_TABLE_ATTR_VERTEX_SUFIX);
const std::string te::graph::Globals::sm_tableVertexModelAttrId(TE_GRAPH_GRAPH_VERTEX_MODEL_ID);
