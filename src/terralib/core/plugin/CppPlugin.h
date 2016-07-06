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
  \file terralib/core/plugin/CppPlugin.h

  \brief A base class for C++ plugins in TerraLib.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_CPPPLUGIN_H__
#define __TERRALIB_CORE_PLUGIN_CPPPLUGIN_H__

// TerraLib
#include "AbstractPlugin.h"

namespace te
{
  namespace core
  {

    /*!
      \class CppPlugin

      \brief The base class for C++ plugins.
     */
    class TECOREEXPORT CppPlugin : public AbstractPlugin
    {
      public:

        /*! \brief Default constructor. */
        CppPlugin(const PluginInfo& pinfo);

        /*! \brief Virtual destructor. */
        virtual ~CppPlugin();

        const PluginInfo& info() const;

        bool initialized() const;

      protected:

        PluginInfo m_pinfo;
        bool m_initialized;
    };

    //! The type of function for plugin's entry point.
    typedef CppPlugin* (*te_get_plugin_fnct_t)(const PluginInfo& pinfo);

  }  // end namespace plugin
}    // end namespace te

#define TERRALIB_CPP_PLUGIN_BEGIN(plugin_class_name) \
class plugin_class_name : public te::core::CppPlugin \
{ \
  public: \
\
    plugin_class_name(const te::core::PluginInfo& pinfo) \
      : CppPlugin(pinfo) \
    { \
    }

#define TERRALIB_CPP_PLUGIN_STARTUP \
    void startup()

#define TERRALIB_CPP_PLUGIN_SHUTDOWN \
    void shutdown()

#define TERRALIB_CPP_PLUGIN_END \
};

#ifdef WIN32
  #define TERRALIB_PLUGIN_EXPORT_MACRO __declspec(dllexport)
#else
  #define TERRALIB_PLUGIN_EXPORT_MACRO
#endif

//! This macro should be used by C++ plugins in order to declare the exportable/callable DLL function.
#define TERRALIB_PLUGIN_CALL_BACK_IMPL(PLUGIN_CLASS_NAME) \
extern "C" TERRALIB_PLUGIN_EXPORT_MACRO te::core::CppPlugin* te_cpp_plugin_get_instance(const te::core::PluginInfo& pinfo); \
\
te::core::CppPlugin* te_cpp_plugin_get_instance(const te::core::PluginInfo& pinfo) \
{ \
  return new PLUGIN_CLASS_NAME(pinfo); \
}

#endif  // __TERRALIB_CORE_PLUGIN_CPPPLUGIN_H__