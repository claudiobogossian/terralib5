// DSInfo
#include "../core/DSCopy.h"

// Terralib
#include <terralib/common.h>

// Boost
#include <boost/program_options.hpp>

boost::program_options::variables_map g_vm;                               //!< Map o options after parse

boost::program_options::options_description g_parameters("Parameters");   //!< Group of parameters options

boost::program_options::options_description g_all("All options");         //!< All options
boost::program_options::options_description g_visible("Allowed options"); //!< Which will be shown to the user

void showError(std::string errorMessage)
{
  std::cout << ">>>> " << errorMessage << " <<<<" << std::endl << std::endl;
  std::cout << g_visible << std::endl;
  std::cout << "---------------------------" << std::endl;
}

int main(int argc, char* argv[])
{  
  std::string dsTypeOrigin = "";
  std::string connStrOrigin = "";
  std::string dsTypeDestination = "";
  std::string connStrDestination = "";
  std::vector<std::string> dataset = std::vector<std::string>();
  std::vector<std::string> datasets = std::vector<std::string>();

  try
  {

    std::cout << std::endl << "TerraLib Data Source Copy" << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;
    
    g_parameters.add_options()
    ("ds-type-origin,o",      boost::program_options::value<std::string>(), "Origin Data Source Type: POSTGIS, MYSQL, GDAL, ...")
    ("conn-str-origin,s",    boost::program_options::value<std::string>(), "Origin Data Source Connection Infomation")
    ("ds-type-destination,d",      boost::program_options::value<std::string>(), "Destination Data Source Type: POSTGIS, MYSQL, GDAL, ...")
    ("conn-str-destination,t",    boost::program_options::value<std::string>(), "Destination Data Source Connection Infomation")
    ("dataset", boost::program_options::value<std::vector<std::string>>()->multitoken(), "DataSet to be described")
    ("datasets", boost::program_options::value<std::vector<std::string>>()->multitoken(), "List of one or more DataSets to be described")
    ("help", "help");

    g_visible.add(g_parameters);
    g_all.add(g_visible);
    
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, g_all), g_vm);
  
    boost::program_options::notify(g_vm);

    if(g_vm.count("help") || g_vm.empty())
    {
      std::cout << g_visible << std::endl;
      std::cout << "---------------------------" << std::endl;
      return EXIT_SUCCESS;
    }


    dsTypeOrigin = (g_vm.count("ds-type-origin") != 0 ? g_vm["ds-type-origin"].as<std::string>() : std::string());
    connStrOrigin = (g_vm.count("conn-str-origin") != 0 ? g_vm["conn-str-origin"].as<std::string>() : std::string());

    dsTypeDestination = (g_vm.count("ds-type-destination") != 0 ? g_vm["ds-type-destination"].as<std::string>() : std::string());
    connStrDestination = (g_vm.count("conn-str-destination") != 0 ? g_vm["conn-str-destination"].as<std::string>() : std::string());
 
    dataset = g_vm.count("dataset") != 0 ? g_vm["dataset"].as<std::vector<std::string>>() : std::vector<std::string>();
    datasets = g_vm.count("datasets") != 0 ? g_vm["datasets"].as<std::vector<std::string>>() : std::vector<std::string>();
    datasets.insert(datasets.end(), dataset.begin(), dataset.end());


    if(dsTypeOrigin.empty())
    {
      showError("Comannd line error: ds-type-origin missing!");
      return EXIT_SUCCESS;
    }
    if(dsTypeDestination.empty())
    {
      showError("Comannd line error: ds-type-destination missing!");
      return EXIT_SUCCESS;
    }
    if(connStrOrigin.empty())
    {
      showError("Comannd line error: conn-str-origin missing!");
      return EXIT_SUCCESS;
    }
    if(connStrDestination.empty())
    {
      showError("Comannd line error: conn-str-destination missing!");
      return EXIT_SUCCESS;
    }


    TerraLib::getInstance().initialize();

    std::vector<std::string*> datasetsP;
    for(size_t i = 0; i < datasets.size(); i++)
      datasetsP.push_back(&datasets[i]);
    
    std::string errorMessage;

    te::tools::dscopy::DSCopy ds;

    if(!ds.init(errorMessage))
    {
      showError(errorMessage);
      return EXIT_SUCCESS;
    }

    if(!ds.copy(dsTypeOrigin, connStrOrigin, dsTypeDestination, connStrDestination, datasetsP, errorMessage))
    {
      showError(errorMessage);
      return EXIT_SUCCESS;
    }

    TerraLib::getInstance().finalize();

  }
  catch(std::exception& e)
  {
    std::cout << "The application concluded erroneously: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << "The application concluded unexpectedly!" << std::endl;
  }

  return EXIT_SUCCESS;
}