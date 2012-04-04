// Terralib Data Source Info tool
#include <Utils.h>

bool Utils::getDataSource(std::string dsType, std::string connStr, te::da::DataSource* & dataSource, std::string & errorMessage)
{  
  if(dsType == "OGR")
  {
      dataSource = te::da::DataSourceFactory::make("OGR");
  }
  else if(dsType == "POSTGIS")
  {
    dataSource = te::da::DataSourceFactory::make("POSTGIS");
  }
  else if(dsType == "GDAL")
  {
    dataSource = te::da::DataSourceFactory::make("GDAL");
  }
  else
    return 0;

  dataSource->open(connStr);

  return true;

}

std::string Utils::getIdxTypeName(int id)
{
  if(id == 0)
    return "Btree";
  else if(id == 1)
    return "RTree";
  else if(id == 2)
    return "QuadTree";
  else if(id == 3)
    return "Hash";
  else
    return "unknown index";
}

std::vector<std::string> Utils::getPKPropertiesNames(te::da::PrimaryKey* pk)
{
  std::vector<std::string> propertiesNames;

  std::vector<te::dt::Property*> properties = pk->getProperties();

  for(unsigned int i = 0; i < properties.size(); i++)
  {
    propertiesNames.push_back(properties[i]->getName());
  }

  return propertiesNames;
}

std::vector<std::string> Utils::getFKPropertiesNames(te::da::ForeignKey* fk)
{
  std::vector<std::string> propertiesNames;

  std::vector<te::dt::Property*> properties = fk->getProperties();

  for(unsigned int i = 0; i < properties.size(); i++)
  {
    propertiesNames.push_back(properties[i]->getName());
  }

  return propertiesNames;
}

std::vector<std::string> Utils::getUKPropertiesNames(te::da::UniqueKey* uk)
{
  std::vector<std::string> propertiesNames;

  std::vector<te::dt::Property*> properties = uk->getProperties();

  for(unsigned int i = 0; i < properties.size(); i++)
  {
    propertiesNames.push_back(properties[i]->getName());
  }

  return propertiesNames;
}

std::vector<std::string> Utils::getIdxPropertiesNames(te::da::Index* idx)
{
  std::vector<std::string> propertiesNames;

  std::vector<te::dt::Property*> properties = idx->getProperties();

  for(unsigned int i = 0; i < properties.size(); i++)
  {
    propertiesNames.push_back(properties[i]->getName());
  }

  return propertiesNames;
}

te::da::DataSet*  Utils::getDataSet(te::da::DataSource* dataSource, std::string dataSetName)
{
  std::map<std::string, te::da::DataSet*>  dataSets;

  te::da::DataSourceTransactor* transactor = dataSource->getTransactor();

  te::da::DataSet* dataset = transactor->getDataSet(dataSetName);

  delete transactor;

  return dataset;
}

bool Utils::loadModules(std::string & errorMessage)
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

    info.m_name = "OGR DataSource Driver";
    info.m_description = "This data source driver supports...";

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      #ifdef NDEBUG
        info.m_mainFile = "terralib_ogr.dll";
      #else
        info.m_mainFile = "terralib_ogr_d.dll";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
      #ifdef NDEBUG
        info.m_mainFile = "libterralib_ogr.so";
      #else
        info.m_mainFile = "libterralib_ogr_d.so";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      #ifdef NDEBUG
        info.m_mainFile = "terralib_ogr.dylib"; // should it be different????
      #else
        info.m_mainFile = "terralib_ogr.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }

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

    info.m_name = "GDAL DataSource Driver";
    info.m_description = "This data source driver supports...";

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      #ifdef NDEBUG
        info.m_mainFile = "terralib_gdal.dll";
      #else
        info.m_mainFile = "terralib_gdal_d.dll";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
      #ifdef NDEBUG
        info.m_mainFile = "libterralib_gdal.so";
      #else
        info.m_mainFile = "libterralib_gdal_d.so";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      #ifdef NDEBUG
        info.m_mainFile = "terralib_gdal.dylib";
      #else
        info.m_mainFile = "terralib_gdal.dylib";
      #endif
    #endif
      
    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }

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

    info.m_name = "PostGIS DataSource Driver";
    info.m_description = "This data source driver supports...";

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      #ifdef NDEBUG
        info.m_mainFile = "terralib_postgis.dll";
      #else
        info.m_mainFile = "terralib_postgis_d.dll";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
      #ifdef NDEBUG
        info.m_mainFile = "libterralib_postgis.so";
      #else
        info.m_mainFile = "libterralib_postgis_d.so";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      #ifdef NDEBUG
        info.m_mainFile = "terralib_postgis.dylib";
      #else
        info.m_mainFile = "terralib_postgis.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);

  } 
  
  return true;
}