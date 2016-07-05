/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin/PluginInfoSerializerManager.h

  \brief A singleton for managing PluginInfo serializers.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_PLUGININFOSERIALIZERMANAGER_H__
#define __TERRALIB_CORE_PLUGIN_PLUGININFOSERIALIZERMANAGER_H__

// TerraLib
#include "../Config.h"
#include "PluginInfo.h"

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace core
  {
    //! Serializers allow applications to extend how to parse plugin data.
    typedef boost::function1<PluginInfo, const std::string&> PluginInfoSerializerFnct;

    //! A singleton that can be used to register plugin serializers.
    class TECOREEXPORT PluginInfoSerializerManager 
    {
      public:

        //! Return the list of registered serializers.
        std::vector<std::string> getSerializers() const;

        //! Return a specific serializer.
        /*!
          \exception OutOfRangeException If a finder with the given name is not found.
         */
        PluginInfoSerializerFnct& get(const std::string& serializer_name);

        //! Registers a new serializer.
        /*!
          \exception InvalidArgumentException Throw an exception if a finder with the same name already exists.
         */
        void insert(const std::string& serializer_name, PluginInfoSerializerFnct serializer);

        //! Remove a serializer from the manager.
        /*!
          \exception OutOfRangeException If a serializer with the given name is not found.
         */
        void remove(const std::string& serializer_name);

        //! Return true if a serializer with the given name is registered.
        bool exists(const std::string& serializer_name) const;

        //! Access the singleton.
        static PluginInfoSerializerManager& instance();

      protected:

        /*! \brief Consructor. */
        PluginInfoSerializerManager();

        /*! \brief Destructor. */
        ~PluginInfoSerializerManager();

      private:

// no copy allowed
        PluginInfoSerializerManager(const PluginInfoSerializerManager&);
        PluginInfoSerializerManager& operator=(const PluginInfoSerializerManager&);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_PLUGININFOSERIALIZERMANAGER_H__
