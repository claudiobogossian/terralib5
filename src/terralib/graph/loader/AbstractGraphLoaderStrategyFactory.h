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
  \file AbstractGraphLoaderStrategyFactory.h

  \brief This is the abstract factory for graph loader strategy.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGYFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGYFACTORY_H

// TerraLib
#include "../../common/AbstractFactory.h"
#include "../Config.h"

namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractGraphLoaderStrategy;
    class GraphMetadata;

    /*!
      \class AbstractGraphLoaderStrategyFactory

      \brief This is the abstract factory for graph loader strategy.

      \sa AbstractGraphLoaderStrategy
    */
    class TEGRAPHEXPORT AbstractGraphLoaderStrategyFactory : public te::common::AbstractFactory<AbstractGraphLoaderStrategy, std::string>
    {
      public:

        /*!
          \brief It creates and returns default graph loader strategy.

          \return A default graph loader strategy.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraphLoaderStrategy* make();

        /*!
          \brief It creates graph loader strategy with the proper type.

          \param lsType  Type The name of the specific graph loader strategy type to be used to create.

          \return A proper graph loader strategy.

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractGraphLoaderStrategy* make(const std::string& lsType);

        /*!
          \brief It creates graph loader strategy with the proper type.

          \param lsType   Type The name of the specific graph loader strategy type to be used to create.
          \param gm       Pointer to a class that defines the graph metadata

          \return A proper graph loader strategy.

          \note The caller will take the ownership of the returned pointer.

        */
        static AbstractGraphLoaderStrategy* make(const std::string& lsType, GraphMetadata* gm);


        /*! \brief Destructor. */
        virtual ~AbstractGraphLoaderStrategyFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        virtual const std::string& getType() const = 0;

      protected:

        /*!
          \brief Constructor.

          \param factoryKey The key that identifies the factory.
        */
        AbstractGraphLoaderStrategyFactory(const std::string& factoryKey);

        /*!
          \brief This method must be implemented by subclasses (loader strategy types).

          \param gm
         
          \return A proper graph loader strategy.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual AbstractGraphLoaderStrategy* create(GraphMetadata* gm) = 0;
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGYFACTORY_H
