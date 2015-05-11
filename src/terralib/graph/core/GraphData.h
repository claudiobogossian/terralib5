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
  \file GraphData.h

  \brief This class define a important struct used to group
         a map of vertex and edges. A flag is used to indicate
         if any element of this group was changed.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GRAPHDATA_H
#define __TERRALIB_GRAPH_INTERNAL_GRAPHDATA_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <map>

namespace te
{
  namespace graph
  {
    //forward declarations
    class Vertex;
    class Edge;
    
    /*!
      \class GraphData

      \brief This class define a important struct used to group
         a map of vertex and edges. A flag is used to indicate
         if any element of this group was changed.

      \sa GraphCache
    */

    class TEGRAPHEXPORT GraphData
    {
      public:

        /*! \brief Default constructor. */
        GraphData(int id);

        /*! \brief Default destructor. */
        ~GraphData();

        
        typedef std::map<int, Vertex*> VertexMap; //!< typedef for vertex map

        typedef std::map<int, Edge*> EdgeMap;     //!< typedef for edge map

        /*! \brief Get data identifier. */
        int getId();

        /** @name Graph Data Access Methods
        *  Method used to access the graph data, vertex and edge, and properties
        */
        //@{


        void addVertex(Vertex* v);

        bool removeVertex(int id);

        Vertex* getVertex(int id);


        /*!
        \brief It returns the the vertex map

        \return Map with all vertex from this graph data
        */
        VertexMap& getVertexMap();

        /*!
        \brief Used to set a map of vertex elements 

        \param map Vertex map

        */
        void setVertexMap(const VertexMap& map);


        void addEdge(Edge* e);

        bool removeEdge(int id);

        Edge* getEdge(int id);


        /*!
        \brief It returns the the edge map

        \return Map with all edges from this graph data
        */
        EdgeMap& getEdgeMap();

        /*!
        \brief Used to set a map of edge elements 

        \param map Edge map

        */
        void setEdgeMap(const EdgeMap& map);

        //@}


        /*!
        \brief Flag used to define the graph data state

        \param status Boolean value to define the state

        */
        void setDirty(bool status);

        /*!
        \brief Used to check the graph data state

        \return Boolean value that defines the graph data state

        */
        bool isDirty();

      private:

        int             m_id;      //!< Data identifier
        bool            m_dirty;        //!< Flag used to indicate that a element was changed

        VertexMap       m_vertexMap;    //!< This map contains all vertexs from this graph.
        EdgeMap         m_edgeMap;      //!< This map contains all edges from this graph.

    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_GRAPHDATA_H