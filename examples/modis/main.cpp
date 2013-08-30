// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <cstdlib>
#include <memory>

using namespace std;

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
// ... ?????

// Exemplo de consulta
        // const CoverageSeries* cvs = modis_db.get("nome-produto?");

        //TimeSeries ts = cvs->getTimeSeries(lat, lon);

        //...

        te::plugin::PluginManager::getInstance().unloadAll();

        TerraLib::getInstance().finalize();
    }
    catch(const std::exception& e)
    {
        std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

