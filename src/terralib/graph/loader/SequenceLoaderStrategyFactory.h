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
  \file SequenceLoaderStrategyFactory.h

  \brief This is the concrete factory for the database sequency loader strategy.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_SEQUENCELOADERSTRATEGYFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_SEQUENCELOADERSTRATEGYFACTORY_H

// TerraLib
#include "AbstractGraphLoaderStrategyFactory.h"
#include "../Config.h"

namespace te
{
  namespace graph
  {
    //forward declarations
    class GraphMetadata;

    /*!
      \class SequenceLoaderStrategyFactory

      \brief This is the concrete factory for the database sequency loader strategy.

      \sa te::graph::AbstractLoaderStrategyFactory
    */
    class TEGRAPHEXPORT SequenceLoaderStrategyFactory : public te::graph::AbstractGraphLoaderStrategyFactory
    {
      public:

        /*! \brief Destructor. */
        ~SequenceLoaderStrategyFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        const std::string& getType() const;

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory ... */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory ... */
        static void finalize();

      protected:

        /*! \brief Constructor. */
        SequenceLoaderStrategyFactory();

        /*!
          \brief This method must be implemented by subclasses (load strategy types).

          \param gm    The necessary graph information
         
          \return The new loader strategy.

          \note The caller will take the ownership of the returned pointer.
        */
        te::graph::AbstractGraphLoaderStrategy* create(GraphMetadata* gm);

        /*!
          \brief Builder Function used to create the class object.
        */
        te::graph::AbstractGraphLoaderStrategy* build();

      private:

        static SequenceLoaderStrategyFactory* sm_factory; //!< Static instance used to register the factory
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_SEQUENCELOADERSTRATEGYFACTORY_H

