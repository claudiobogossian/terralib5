#include <terralib/common.h>
#include <terralib/plugin.h>

#include <iostream>

void LoadModules()
{
  try
  {
    {
      te::plugin::PluginInfo info;
      
      info.m_name = "te.xerces";
      info.m_displayName = "Xerces XML Reader Driver";
      info.m_description = "Xerces XML Reader Driver";
      info.m_engine = "C++";
      info.m_folder = PLUGINS_PATH;
      
      std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_xerces");
      
      info.m_resources.push_back(rsc);
      
      te::plugin::PluginManager::getInstance().load(info);
    }
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }
}

