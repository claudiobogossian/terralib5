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
  \file terralib/sa/core/SkaterPartition.h

  \brief This file contains a class that represents the skater partition operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SKATERPARTITION_H
#define __TERRALIB_SA_INTERNAL_SKATERPARTITION_H

// Terralib Includes
#include "SkaterParams.h"
#include "../Config.h"

// STL Includes
#include <memory>

namespace te
{
  //forward declarations
  namespace graph 
  { 
    class AbstractGraph;
    class Edge;
    class Vertex; 
  }

  namespace sa
  {
    /*!
      \struct EdgeRemovalInfo
      
      \brief A struct that represents the skater partition values for each edge.
    */
    struct EdgeRemovalInfo
    {
      /*! \brief Default constructor. */
      EdgeRemovalInfo(){};

       /*! \brief Destructor. */
      ~EdgeRemovalInfo(){};

      std::size_t m_edgeId; //!< Edge identification

      double m_SSDT;        //!< Sum of Square Difference for Tree
      double m_SSDTa;       //!< Sum of Square Difference for Tree A
      double m_SSDTb;       //!< Sum of Square Difference for Tree B
      double m_SSDi;        //!< Difference between m_SSDa and m_SSDb
      std::size_t m_popa;   //!< Sum of population for Tree A
      std::size_t m_popb;   //!< Sum of population for Tree B
    };

    /*!
      \struct RootComparer
      
      \brief A struct to order the roots.
    */
    typedef std::pair<double, std::size_t> Root;

    struct RootComparer
    {
      bool operator()(Root a, Root b) const
      {
        return a.first < b.first;
      }
    };

    typedef std::set<Root, RootComparer> RootSet;
    
    /*!
      \class SkaterPartition

      \brief Class that represents the skater partition operation.

    */
    class TESAEXPORT SkaterPartition
    {
      public:

        /*! 
          \brief Default constructor. 

          \param graph Pointer to a graph that represents a minimum spanning tree.
          \param attrs Vector with attributes names used to calculate the skater operation
        */
        SkaterPartition(te::graph::AbstractGraph* graph, std::vector<std::string> attrs, std::string popAttr = "", std::size_t minPop = 0);

        /*! \brief Virtual destructor. */
        ~SkaterPartition();

      public:

        /*! 
          \brief Function to execute the skater partition.

          \param nGroups Number of cluster to be created.

          \return Return a vector with the root vertex id for each cluster created.
        */
        std::vector<std::size_t> execute(std::size_t nGroups);

      protected:

        bool edgeToRemove(int startVertex, double& diffA, double& diffB, std::size_t& edgeToRemoveId);

        double calculateEdgeDifference(int vertexFrom, int vertexTo, double& diffA, double& diffB, std::size_t& popA, std::size_t& popB);

        std::vector<double> calculateRootMean(int startVertex, int vertexToIgnore, std::size_t& pop);

        double calculateRootDeviation(int startVertex, int vertexToIgnore, std::vector<double>& meanVec);

        double calculateDistance(te::graph::Vertex* vertex, std::vector<double>& meanVec);

      protected:

        te::graph::AbstractGraph* m_graph;    //!< Pointer to a graph that represents a minimum spanning tree.

        std::vector<std::string> m_attrs;     //!< Vector with attributes names used to calculate the skater operation.

        std::string m_popAttr;                //!< The population attribute name.

        std::size_t m_popMin;                 //!< The minimum population value allowed in a cluster.

        std::vector<double> m_SSDiValues;      //!< Vector with the sum square differences for each edge removed.

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_SKATERPARTITION_H
