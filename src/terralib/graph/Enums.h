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
  \file terralib/graph/Enums.h

  \brief General enumerations.
 */

#ifndef __TERRALIB_GRAPH_INTERNAL_ENUMS_H
#define __TERRALIB_GRAPH_INTERNAL_ENUMS_H

namespace te
{
  namespace graph
  {

    enum GraphType
    {
      Undirect = 0,   // A graph in which edges have no orientation.
      Direct = 1,     // A directed graph or digraph is an ordered pair D = (V, A)
      Mixed = 2,      // A mixed graph G is a graph in which some edges may be directed and some may be undirected.
      Multi = 3,      // The term "multigraph" is generally understood to mean that multiple edges (and sometimes loops) are allowed.
      Simple = 4,     // As opposed to a multigraph, a simple graph is an undirected graph that has no loops and no more than one edge between any two different vertices.
      Weighted = 5,   // A graph is a weighted graph if a number (weight) is assigned to each edge.
      HalfEdges = 6   // In exceptional situations it is even necessary to have edges with only one end.
    };

    enum GraphClass
    {
      Regular = 0,      // A regular graph is a graph where each vertex has the same number of neighbors.
      Complete = 1,     // Complete graphs have the feature that each pair of vertices has an edge connecting them.
      Finite = 2,       // A finite graph is a graph G = (V, E) such that V and E are finite sets.
      Infinite = 3,     // An infinite graph is one with an infinite set of vertices or edges or both.
      Connected = 4,    // A graph is called connected if every pair of distinct vertices in the graph is connected.
      Disconnected = 5  // A graph is called disconnected if at least one pair of distinct vertices in the graph is not connected.
    };

    enum GraphStorageMode
    {
      Edge_List = 0,  // Using this mode a table with edge list information will be used to store this graph >> edge_id | vertex_from | vertex_to
      Vertex_List = 1  // Using this mode a table with vertex list information will be used to store this graph >> vertex_ix | edges_in | edges_out
    };

    enum GraphAttrType
    {
      Vertex_Attr = 0,  // Using this attr type will associate this attribute to vertex object
      Edge_Attr = 1     // Using this attr type will associate this attribute to edge object
    };

  } // end namespace graph
} // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_ENUMS_H

