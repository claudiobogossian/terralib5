/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file CppPluginProxyFactory.h

  \brief This is the concrete factory for C++ Plugins.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXYFACTORY_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXYFACTORY_H

// TerraLib
#include "PluginFactory.h"

namespace te
{
  namespace plugin
  {
    /*!
      \class CppPluginProxyFactory

      \brief This is the concrete factory for C++ Plugins.

      \sa Plugin, ParameterizedAbstractFactory
     */
    class TEPLUGINEXPORT CppPluginProxyFactory : public PluginFactory
    {
      public:

        /** @name Public Initializer
         *  Methods for the control of the factory singleton.
         */
        //@{

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory DataSourceFactory. */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory DataSourceFactory. */
        static void finalize();

        //@}

        /** @name Public Destructor
         *  Destructor.
         */
        //@{

        /*!
          \brief Destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        ~CppPluginProxyFactory();

        //@} 

      protected:

        /** @name Re-implementation of Pure Virtual Method
         *  Concrete factory methods.
         */
        //@{

        /*!
          \brief It creates a new PostGIS DataSource.

          \param p The parameter to be passed to the object being created.

          \return It returns an object created by the concrete factory.
         */
        Plugin* build(PluginInfo p);

        //@}

        /** @name Protected Initializers
         *  Initializers.
         */
        //@{

        /*!
          \brief It creates the factory.

          The key of a CppPluginProxyFactory is a string.

          \param factoryKey The key that identifies the factory.
         */
        CppPluginProxyFactory();        

        //@}        

      private:

        static CppPluginProxyFactory* sm_factory;
    };

  } // end namespace plugin
}   // end namespace te


#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINPROXYFACTORY_H


