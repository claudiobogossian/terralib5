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
  \file terralib/plugin/CppPluginEngineFactory.h

  \brief This is the concrete factory for C++ plugin engines.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINEFACTORY_H
#define __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINEFACTORY_H

// TerraLib
#include "PluginEngineFactory.h"

namespace te
{
  namespace plugin
  {
    /*!
      \class CppPluginEngineFactory

      \brief This is the concrete factory for C++ plugin engines.

      \sa PluginEngineFactory, AbstractFactory, PluginEngine, Plugin
    */
    class TEPLUGINEXPORT CppPluginEngineFactory : public PluginEngineFactory
    {
      protected:

        /*!
          \brief It creates a new plugin engine for C++.

          \return It returns plugin engine for C++.
        */
        AbstractPluginEngine* build();

        /*! It register the factory in the abstract dictionary. */
        CppPluginEngineFactory();

        /*! \brief Destructor. */
        ~CppPluginEngineFactory();

      private:

        static CppPluginEngineFactory sm_factory;  //!< The C++ plugin engine factory.
    };

  } // end namespace plugin
}   // end namespace te


#endif  // __TERRALIB_PLUGIN_INTERNAL_CPPPLUGINENGINEFACTORY_H

