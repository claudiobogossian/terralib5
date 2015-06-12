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
  \file AbstractGraphFactory.h

  \brief This is the abstract factory for Graphs.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHFACTORY_H

// TerraLib
#include "../../common/AbstractFactory.h"
#include "../graphs/Graph.h"
#include "../Config.h"


namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractCachePolicy;
    class AbstractGraph;
    class AbstractGraphLoaderStrategy;

    /*!
      \class AbstractGraphFactory

      \brief This is the abstract factory for Graphs.

      \sa AbstractGraph
    */
    class TEGRAPHEXPORT AbstractGraphFactory : public te::common::AbstractFactory<AbstractGraph, std::string>
    {
      public:

        /*!
          \brief It creates and returns an empty graph with default graph type.

          \return An empty graph.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraph* make();

        /*!
          \brief It creates an empty graph with the proper type.

          \param gType The name of the specific graph type to be used to create the graph.

          \return An empty graph.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraph* make(const std::string& gType);

        /*!
          \brief It creates a graph with the given parameters using the default graph type.

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return A new graph.

          \note The caller will take the ownership of the returned pointer.

        */
        static AbstractGraph* make(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief It creates a graph with the given parameters using the default graph type.

          \param gType    The name of the specific driver to create the raster.
          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return A new graph.

          \note The caller will take the ownership of the returned pointer.

        */
        static AbstractGraph* make(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief It opens a graph with the given parameters and default graph type.

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return The opened graph.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraph* open(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief It creates a graph with the given parameters.

          \param gType  The name of the specific graph type to create the graph.
          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return The opened graph.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraph* open(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);


        /*! \brief Destructor. */
        virtual ~AbstractGraphFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        virtual const std::string& getType() const = 0;

        /*! \brief It returns the list of parameters accepted as graph info. */
        virtual void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const = 0;

      protected:

        /*!
          \brief Constructor.

          \param factoryKey The key that identifies the factory.
        */
        AbstractGraphFactory(const std::string& factoryKey);

        /*!
          \brief This method must be re-implemented by subclasses in order to have a finner control for the graph object instantiation. 

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return A graph.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual AbstractGraph* iOpen(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo) = 0;

        /*!
          \brief This method must be implemented by subclasses (graph types).

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.
         
          \return The new graph.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual AbstractGraph* create(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo) = 0;

        /*!
          \brief This method is a auxiliar function used to get the metadata pointer

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.
         
          \return The graph metadata pointer.

          \note The caller will take the ownership of the returned pointer.
        */
        static te::graph::GraphMetadata* getMetadata(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief This method is a auxiliar function used to get the graph id

          \param gInfo    The necessary information to create the graph.
         
          \return The graph id.

        */
        static int getId(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief This method is a auxiliar function used to get the cache policy pointer

          \param gInfo    The necessary information to create the graph.
         
          \return The graph cache policy pointer.

          \note The caller will take the ownership of the returned pointer.
        */
        static te::graph::AbstractCachePolicy* getCachePolicy(const std::map<std::string, std::string>& gInfo);

        /*!
          \brief This method is a auxiliar function used to get the loader strategy pointer

          \param gInfo    The necessary information to create the graph.
          \param metadata The graph metadata pointer.
         
          \return The graph loader strategy pointer.

          \note The caller will take the ownership of the returned pointer.
        */
        static te::graph::AbstractGraphLoaderStrategy* getLoaderStrategy(const std::map<std::string, std::string>& gInfo, te::graph::GraphMetadata* metadata);

        static void setMetadataInformation(const std::map<std::string, std::string>& gInfo, te::graph::GraphMetadata* metadata);
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHFACTORY_H
