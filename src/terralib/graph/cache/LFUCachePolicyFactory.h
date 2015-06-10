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
  \file LFUCachePolicyFactory.h

  \brief This is the concrete factory for the LFU cache policy
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_LFUCACHEPOLICYFACTORY_H
#define __TERRALIB_GRAPH_INTERNAL_LFUCACHEPOLICYFACTORY_H

// TerraLib
#include "AbstractCachePolicyFactory.h"
#include "../Config.h"

namespace te
{
  namespace graph
  {
    /*!
      \class LFUCachePolicyFactory

      \brief This is the concrete factory for the LFU cache policy

      \sa te::graph::AbstractCachePolicyFactory
    */
    class TEGRAPHEXPORT LFUCachePolicyFactory : public te::graph::AbstractCachePolicyFactory
    {
      public:

        /*! \brief Virtual destructor. */
        ~LFUCachePolicyFactory() {}

        const std::string& getType() const;

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory ... */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory ... */
        static void finalize();

      protected:

        /*! \brief Default constructor. */
        LFUCachePolicyFactory();

        /*! \brief Builds a new  cache policy object. */
        te::graph::AbstractCachePolicy* build();

      private:

        static LFUCachePolicyFactory* sm_factory;   //!< Static attribute used to register this factory
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_LFUCACHEPOLICYFACTORY_H

