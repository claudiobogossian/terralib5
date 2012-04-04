#include <terralib/common.h>
#include <terralib/plugin.h>

#include <iostream>

void LoadModules()
{
  try
  {
    {
      te::plugin::PluginInfo info;
      
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      info.m_type = "dll";
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      info.m_type = "s.o.";
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
      info.m_type = "dylib";      
#else
  #error "Platform not supported yet"
#endif

      info.m_name = "Xerces XML Reader Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_xerces.dll";
#else
      info.m_mainFile = "terralib_xerces_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_xerces.so";
#else
      info.m_mainFile = "libterralib_xerces_d.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_xerces.dylib";
#else
      info.m_mainFile = "/Users/lubia/Documents/terralib5svn/lib32/Debug/libterralib_xerces_d.dylib";
#endif
#endif
      
      te::plugin::PluginManager::getInstance().loadPlugin(info); 
    }
  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to load data source drivers: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknow exception!" << std::endl;
  }
}

