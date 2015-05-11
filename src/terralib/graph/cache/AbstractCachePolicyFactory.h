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
  \file AbstractCachePolicyFactory.h

  \brief This is the abstract factory for cache policy.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICYFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICYFACTORY_H

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
	
    /*!
      \class AbstractCachePolicyFactory

      \brief This is the abstract factory for cache policy.

      \sa AbstractGraph
    */
    class TEGRAPHEXPORT AbstractCachePolicyFactory : public te::common::AbstractFactory<AbstractCachePolicy, std::string>
    {
      public:

        /*!
          \brief It creates and returns default cache policy

          \return A default cache policy

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractCachePolicy* make();

        /*!
          \brief It creates a cache policy with the proper type.

          \param cpType The name of the specific cache policy.

          \return A specific cache policy

          \note The caller will take the ownership of the returned pointer.
        */
        static AbstractCachePolicy* make(const std::string& cpType);

       
        /*! \brief Destructor. */
        virtual ~AbstractCachePolicyFactory() {}

        /*! \brief Returns the type (name) of this factory. */
        virtual const std::string& getType() const = 0;

      protected:

        /*!
          \brief Constructor.

          \param factoryKey The key that identifies the factory.
        */
        AbstractCachePolicyFactory(const std::string& factoryKey);
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICYFACTORY_H
