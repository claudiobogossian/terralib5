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
  \file GraphFactory.h

  \brief This is the concrete factory for the commun Graph type.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GRAPHFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_GRAPHFACTORY_H

// TerraLib
#include "../core/AbstractGraphFactory.h"
#include "../Config.h"

namespace te
{
  namespace graph
  {
    /*!
      \class GraphFactory

      \brief This is the concrete factory for the commung Graph type.

      \sa te::graph::AbstractGraphFactory
    */
    class TEGRAPHEXPORT GraphFactory : public te::graph::AbstractGraphFactory
    {
      public:

        /*! \brief Destructor. */
        ~GraphFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        const std::string& getType() const;

        /*! \brief It returns the list of parameters accepted as graph info. */
        void getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const;

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory ... */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory ... */
        static void finalize();

      protected:

        /*!
          \brief Constructor.
        */
        GraphFactory();

        /*!
          \brief This method must be re-implemented by subclasses in order to have a finner control for the graph object instantiation. 

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.

          \return A graph.

          \note The caller will take the ownership of the returned pointer.
        */
        te::graph::AbstractGraph* iOpen(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief This method must be implemented by subclasses (graph types).

          \param dsInfo    The necessary information to access the data source.
          \param gInfo    The necessary information to create the graph.
         
          \return The new graph.

          \note The caller will take the ownership of the returned pointer.
        */
        te::graph::AbstractGraph* create(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief Builder Function used to create the class object.
        */
        te::graph::AbstractGraph* build();

      private:

        static GraphFactory* sm_factory;    //!< Static instance used to register the factory

    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_GRAPHFACTORY_H

