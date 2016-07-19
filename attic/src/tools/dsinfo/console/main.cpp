// Terralib Data Source Info tool
#include <PrintManager.h>

// Terralib
#include <terralib/common.h>

// Boost
#include <boost/program_options.hpp>

boost::program_options::variables_map g_vm;                               //!< Map o options after parse

boost::program_options::options_description g_help("Help");               //!< Group of help options
boost::program_options::options_description g_functions("Functions");     //!< Group of functions options
boost::program_options::options_description g_parameters("Parameters");   //!< Group of parameters options

boost::program_options::options_description g_all("All options");         //!< All options
boost::program_options::options_description g_hidden("Hidden options");   //!< Which will not be shown to the user
boost::program_options::options_description g_visible("Allowed options"); //!< Which will be shown to the user

int main(int argc, char* argv[])
{

  try
  {
    std::string dstype = "";
    std::string connstr = "";
    std::vector<std::string> dataset;
    std::vector<std::string> datasets;
    bool dsParams = false;
    bool listdatasets = false;
    bool showAll = false;
    bool pkDetails = false;
    bool fkDetails = false;
    bool ukDetails = false;
    bool idxDetails = false;
    bool ccDetails = false;

    std::cout << std::endl << "TerraLib Data Source Info" << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;

    g_functions.add_options()
    ("list-datasets", "Show a DataSet list of a DataSource")
    ("show-all", "Show all Data Source informations")
    ("show-ds-params", "Show all Data Source parameters required")
    ("help", "help")
    ;

    g_parameters.add_options()
    ("ds-type",      boost::program_options::value<std::string>(), "Data Source Type: POSTGIS, MYSQL, GDAL, ...")
    ("conn-str",    boost::program_options::value<std::string>(), "Connection Infomation")
    ("dataset", boost::program_options::value<std::vector<std::string>>()->multitoken(), "DataSet to be described")
    ("datasets", boost::program_options::value<std::vector<std::string>>()->multitoken(), "List of one or more DataSets to be described")
    ("pk-details", "Show Primary Key Details")
    ("fk-details", "Show Foreign Key Details")
    ("uk-details", "Show Unique Key Details")
    ("idx-details", "Show Indexes Details")
    ("cc-details", "Show Check Contraints Details")
    ;

    g_hidden.add(g_help);
    g_visible.add(g_functions).add(g_parameters);
    g_all.add(g_visible).add(g_hidden);

  
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, g_all), g_vm);

    boost::program_options::notify(g_vm);

    if(g_vm.count("help") || g_vm.empty())
    {
      std::cout << g_visible << std::endl;
      std::cout << "---------------------------" << std::endl;
      return EXIT_SUCCESS;
    }

    dstype = g_vm.count("ds-type") != 0 ? g_vm["ds-type"].as<std::string>() : std::string();
    connstr = g_vm.count("conn-str") != 0 ? g_vm["conn-str"].as<std::string>() : std::string();
    dataset = g_vm.count("dataset") != 0 ? g_vm["dataset"].as<std::vector<std::string>>() : std::vector<std::string>();
    datasets = g_vm.count("datasets") != 0 ? g_vm["datasets"].as<std::vector<std::string>>() : std::vector<std::string>();
    datasets.insert(datasets.end(), dataset.begin(), dataset.end());
    dsParams = g_vm.count("show-ds-params")?true:false;
    listdatasets = g_vm.count("list-datasets") ? true : false;
    showAll = g_vm.count("show-all") ? true : false;
        
    pkDetails = g_vm.count("pk-details")?true:false;
    fkDetails = g_vm.count("fk-details")?true:false;
    ukDetails = g_vm.count("uk-details")?true:false;
    idxDetails = g_vm.count("idx-details")?true:false;
    ccDetails = g_vm.count("cc-details")?true:false;

    TerraLib::getInstance().initialize();  

    PrintManager pm;

    if(dsParams)
    {
      pm.printDataSourceParameters(dstype);
    }
    else if(!dstype.empty() && !connstr.empty())
    {      
      if(!pm.init(dstype, connstr))
      {
        std::cout << g_visible << std::endl;
        std::cout << "---------------------------" << std::endl;
        return EXIT_SUCCESS;
      }
      
      if(showAll)
      {
        pm.printAllInfo(pkDetails, fkDetails, ukDetails, idxDetails, ccDetails);
      }
      else if(!datasets.empty())
      {
        pm.printDataSetInfo(datasets, pkDetails, fkDetails, ukDetails, idxDetails, ccDetails);
      }
      else
        pm.printDataSetNameList();
    }
    else
    {
      if(dstype.empty())
      {
        std::cout << "Command Line Error: ds-type missing!" << std::endl;
        std::cout << "---------------------------" << std::endl;
        return EXIT_SUCCESS;
      }
      if(connstr.empty())
      {
        std::cout << "Command Line Error: conn-str missing!" << std::endl;
        std::cout << "---------------------------" << std::endl;
        return EXIT_SUCCESS;
      }
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