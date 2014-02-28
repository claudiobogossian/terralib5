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
  \file terralib/graph/Globals.h
   
  \brief An static class with global definitions for the TerraLib Graph Module.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GLOBAL_H
#define __TERRALIB_GRAPH_INTERNAL_GLOBAL_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace graph
  {
    /*!
      \class Globals

      \brief An static class with global definitions for the TerraLib Graph Module.
    */
    class TEGRAPHEXPORT Globals : public te::common::Static
    {
      public:

        static const std::string sm_moduleIdentifier;                       //!< The GRAPH module identifier.


        static const std::string sm_factoryGraphTypeGraph;                  //!< Graph Factory Name.
        static const std::string sm_factoryGraphTypeBidirectionalGraph;     //!< Bidirectional Graph Factory Name.
        static const std::string sm_factoryGraphTypeDirectedGraph;          //!< Directed Graph Factory Name.
        static const std::string sm_factoryGraphTypeUndirectedGraph;        //!< Undirected Graph Factory Name.

        static const std::string sm_factoryGraphTypeMemoryGraph;            //!< Memory Graph Factory Name.

        static const std::string sm_factoryCachePolicyTypeFIFO;             //!< FIFO Cache Policy Factory Name.
        static const std::string sm_factoryCachePolicyTypeLFU;              //!< LFU Cache Policy Factory Name.

        static const std::string sm_factoryLoaderStrategyTypeBox;           //!< Box Loader Strategy Factory Name.
        static const std::string sm_factoryLoaderStrategyTypeSequence;      //!< Sequence Loader Strategy Factory Name.


        static const std::string sm_graphFactoryDefaultObject;              //!< This definition is used to set the default graph type.
        static const std::string sm_cachePolicyFactoryDefaultObject;        //!< This definition is used to set the default cache policy.
        static const std::string sm_loaderStrategyFactoryDefaultObject;     //!< This definition is used to set the default graph loader strategy.


        static const int sm_graphCacheDefaultMaxSize;                       //!< This definition is used to set the max graph cache size.
        static const int sm_graphVecCacheDefaultMaxSize;                    //!< This definition is used to set the max graph cache vector size.
        static const int sm_boxLoaderStrategyDefaultSize;                   //!< This definition is used to set the default box strategy loader box size.


        static const std::string sm_vertexStorageMode;                      //!< This definition is used to set the vertex storage mode.
        static const std::string sm_edgeStorageMode;                        //!< This definition is used to set the edge storage mode.


        static const std::string sm_tableGraphName;                         //!< Database Model Table Graph Name
        static const std::string sm_tableGraphAttrId;                       //!< Attribute Id
        static const std::string sm_tableGraphAttrName;                     //!< Attribute Name
        static const std::string sm_tableGraphAttrType;                     //!< Attribute Type
        static const std::string sm_tableGraphAttrTableName;                //!< Attribute Table Name
        static const std::string sm_tableGraphAttrDesc;                     //!< Attribute Description

        static const std::string sm_tableAttributeName;                     //!< Database Model Table Attribute Name
        static const std::string sm_tableAttributeAttrId;                   //!< Attribute Id
        static const std::string sm_tableAttributeAttrGraphId;              //!< Attribute Graph Id
        static const std::string sm_tableAttributeAttrTable;                //!< Attribute Table
        static const std::string sm_tableAttributeAttrColumn;               //!< Attribute Column
        static const std::string sm_tableAttributeAttrLink;                 //!< Attribute Link
        static const std::string sm_tableAttributeAttrType;                 //!< Attribute Type

        static const std::string sm_tableEdgeModelSufixName;                //!< Database Model Edge Model Table Name
        static const std::string sm_tableEdgeAttributeModelSufixName;       //!< Database Model Edge Attribute Model Table Name
        static const std::string sm_tableEdgeModelAttrId;                   //!< Attribute Id
        static const std::string sm_tableEdgeModelAttrVFrom;                //!< Attribute Vertex From
        static const std::string sm_tableEdgeModelAttrVTo;                  //!< Attribute Vertex To

        static const std::string sm_tableVertexModelSufixName;              //!< Database Model Vertex Model Table Name
        static const std::string sm_tableVertexAttributeModelSufixName;     //!< Database Model Vertex Attribute Model Table Name
        static const std::string sm_tableVertexModelAttrId;                 //!< Attribute id

    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_GLOBAL_H
