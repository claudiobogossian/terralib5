/*
 * File: Plugin.i
 *
 * Plugin
 */

#define TEPLUGINEXPORT

namespace te {
  namespace common {
    %template(PluginMgrSingleton) Singleton < te::plugin::PluginManager >;
  }
}

typedef te::common::Singleton< te::plugin::PluginManager > PluginMgrSingleton;

%{
#include "terralib/plugin/Provider.h"
#include "terralib/plugin/PluginInfo.h"
#include "terralib/plugin/PluginManager.h"
%}

%nspace te::plugin::Provider;
%nspace te::plugin::PluginInfo;
%nspace te::plugin::PluginManager;

%include "terralib/plugin/Provider.h"
%include "terralib/plugin/PluginInfo.h"
%include "terralib/plugin/PluginManager.h"