// MODIS
#include "modis_dataset.h"
#include "modis_utils.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/plugin.h>
#include <terralib/raster.h>

// STL
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>

// Boost
#include <boost/timer/timer.hpp>

#define TE_USE_GDAL 1

int main(int argc, char *argv[])
{
  try
  {
// Inicializa os modulos da TerraLib
    TerraLib::getInstance().initialize();

// Informa a necessidade de carga dinamica do driver GDAL 
#if TE_USE_GDAL
    std::auto_ptr<te::plugin::PluginInfo> pinfo(te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.gdal.teplg")));
    te::plugin::PluginManager::getInstance().add(pinfo.release());
#endif

// Inicializa todos os modulos dinamicos
    te::plugin::PluginManager::getInstance().loadAll(); 


// Prepara os conjuntos de dados do banco de imagens
    std::string product("MOD13Q1");
    boost::filesystem::path mpath("C:\\spatial-data\\modis\\MOD13");
    ProdHVDate modis_files;

    msearch(mpath, product, "", modis_files);

    modis_dataset d(modis_files[product],
                    "HDF4_EOS:EOS_GRID:\"",
                    "\":MODIS_Grid_16DAY_250m_500m_VI:250m 16 days NDVI");

    {
      

      double longitude = d.grid()->getExtent()->getCenter().x;
      double latitude = d.grid()->getExtent()->getCenter().y;

      const unsigned char* values = 0;
      const std::vector<boost::gregorian::date>* times = 0;

      boost::timer::cpu_timer timer;

      d.query(longitude, latitude, &values, &times);
      
      std::string wallt = timer.format();

      std::cout << "\ttimeline generated in: " << wallt << std::endl;

      modis_print(values, times);
    }

    {
      double longitude = d.grid()->getExtent()->getCenter().x;
      double latitude = d.grid()->getExtent()->getCenter().y + d.grid()->getResolutionY() * 5;

      const unsigned char* values = 0;
      const std::vector<boost::gregorian::date>* times = 0;

      boost::timer::cpu_timer timer;

      d.query(longitude, latitude, &values, &times);

      std::string wallt = timer.format();

      std::cout << "\ttimeline generated in: " << wallt << std::endl;

      modis_print(values, times);

      modis_print(values, times);
    }

    {
      double longitude = d.grid()->getExtent()->getCenter().x;
      double latitude = d.grid()->getExtent()->getCenter().y + d.grid()->getResolutionY() * 3;

      const unsigned char* values = 0;
      const std::vector<boost::gregorian::date>* times = 0;

      boost::timer::cpu_timer timer;

      d.query(longitude, latitude, &values, &times);

      std::string wallt = timer.format();

      std::cout << "\ttimeline generated in: " << wallt << std::endl;

      modis_print(values, times);

      modis_print(values, times);
    }


    te::plugin::PluginManager::getInstance().unloadAll();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

    std::cout << "tecle algo: " << std::endl;

    int ii = 0;

    std::cin >> ii;

    return EXIT_FAILURE;
  }


  std::cout << "tecle algo: " << std::endl;

  int ii = 0;

  std::cin >> ii;

  return EXIT_SUCCESS;
}

