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
  \file GraphCache.h

  \brief Class used to manager the graph data elements.
        This class uses a cache policy to control the elements in memory.
        If a element was requested and not found  in cache, the 
        GraphDataManager is used to loaded a new GraphData.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GRAPHCACHE_H
#define __TERRALIB_GRAPH_INTERNAL_GRAPHCACHE_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <map>

namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractCachePolicy;
    class AbstractGraphLoaderStrategy;
    class GraphData;
    class GraphDataManager;
    class GraphMetadata;
    
    /*!
      \class GraphCache

      \brief Class used to manager the graph data elements.
            This class uses a cache policy to control the elements in memory.
            If a element was requested and not found  in cache, the 
            GraphDataManager is used to loaded a new GraphData.

      \sa GraphDataManager, GraphData
    */

    class TEGRAPHEXPORT GraphCache
    {
      public:

        /*! 
          \brief Default constructor. 

          \param cp Implementation of a cache policy

          \param dm Data manager pointer 

        */
        GraphCache(AbstractCachePolicy* cp, GraphDataManager* dm);

        /*! \brief Default destructor. */
        ~GraphCache();

         /** @name Graph Cache Manager Methods
        *  Method used to manager a graph data
        */
        //@{

        /*!
          \brief Get a graph data from vector using a vertex id information.
                 if not found a new graph data has to be loaded using AbstractDataManager

          \param id The vertex identifier of the desired element.

          \return A Graph Data that contains the request element.

        */
        GraphData* getGraphDataByVertexId(int id);

        /*!
          \brief Get a graph data from vector using a edge id information.
                 if not found a new graph data has to be loaded using AbstractDataManager

          \param id The edge identifier of the desired element.

          \return A Graph Data that contains the request element.

        */
        GraphData* getGraphDataByEdgeId(int id);

        /*!
          \brief Get a graph data 

          \note It's always try to return the graph data with the max number of elements that
                is not already full. If its not possible a new graph data will be created.

          \return A pointer to a graph data.

        */
        GraphData* getGraphData();

        /*!
          \brief Creates a new graph data structure 

          \return  A pointer to a new graph data

        */
        GraphData* createGraphData();

        /*!
          \brief Used to remove a graph data from cache

          \param idx The graph data identifier

        */
        void removeGraphData(int idx);

        /*!
          \brief Save the graph data structure inside a data source

          \param data The graph data to be saved
        */
        void saveGraphData(GraphData* data);

        /*!
          \brief Used to remove from memory all elements loaded.

          \note The function Flush from AbstractGraph calls this function.

        */
        void clearCache();

         /*!
          \brief This functions check in cache if the vertex element with a given id was alredy in memory

          \param id The vertex Id

          \return If the element was found its returns a pointer to a graph data that contains this element
        */
        GraphData* checkCacheByVertexId(int id);

         /*!
          \brief This functions check in cache if the edge element with a given id was alredy in memory

          \param id The edge Id

          \return If the element was found its returns a pointer to a graph data that contains this element
        */
        GraphData* checkCacheByEdgeId(int id);

        //@}

      protected:

        /*!
          \brief Protected function used to define a new value of Id to a graph data.

          \return Integer value with the id information
        */
        int getGraphDataId();

      private:

        std::map<int, GraphData*> m_graphDataMap;   //!< This map represents all data loaded in cache

        AbstractCachePolicy* m_policy;              //!< Cache policy to control the cache in memory

        GraphDataManager* m_dataManager;            //!< Used to load and save GraphData information from a DataSource

        GraphMetadata* m_metadata;                  //!< Graph metadata information.

        int m_graphDataCounter;                     //!< Graph data identifier counter
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_GRAPHCACHE_H