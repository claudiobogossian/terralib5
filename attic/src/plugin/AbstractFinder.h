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
  \file terralib/plugin/AbstractFinder.h
  
  \brief The AbstractFinder class allows applications to extend how PluginManager can search for plugins.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_ABSTRACTFINDER_H
#define __TERRALIB_PLUGIN_INTERNAL_ABSTRACTFINDER_H

// TerraLib
#include "Config.h"
#include "Exception.h"

// Boost
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace plugin
  { 
// Forwar declaration
    struct PluginInfo;

    /*!
      \class AbstractFinder
      
      \brief The AbstractFinder class allows applications to extend how PluginManager can search for plugins.

      Subclasses can search for plugins installed in any place.
      There is a default finder named DefaultFinder that
      seraches for plugins in some common places.

      \ingroup plugin

      \sa PluginManager, PluginInfo, DefaultFinder
    */
    class TEPLUGINEXPORT AbstractFinder : public boost::noncopyable
    {
      public:        

        /*! \brief Constructor. */
        AbstractFinder();

        /*! \brief Virtual destructor. */
        virtual ~AbstractFinder();

        /*!
          \brief This method searches for installed plugins and output the plugins information in the PluginInfo vector.

          \param plugins A vector to output all information about found plugins.

          \exception Exception This method may throw an exception with something goes wrong during the search.
        */
        virtual void getPlugins(boost::ptr_vector<PluginInfo>& plugins) = 0;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_ABSTRACTFINDER_H

