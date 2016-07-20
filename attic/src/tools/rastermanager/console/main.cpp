// Terralib Raster Manager Tool
#include <RasterManager.h>

// Terralib
#include <terralib/common.h>

// Boost
#include <boost/program_options.hpp>

// STL
#include <iostream>

te::tools::rastermanager::RasterManager g_rm;                             //!< Raster Manager Object

boost::program_options::variables_map g_vm;                               //!< Map o options after parse

boost::program_options::options_description g_help("Help");               //!< Group of help options
boost::program_options::options_description g_functions("Functions");     //!< Group of functions options
boost::program_options::options_description g_parameters("Parameters");   //!< Group of parameters options

boost::program_options::options_description g_all("All options");         //!< All options
boost::program_options::options_description g_hidden("Hidden options");   //!< Which will not be shown to the user
boost::program_options::options_description g_visible("Allowed options"); //!< Which will be shown to the user



void showError(std::string errorMessage)
{
  std::cout << ">>>> " << errorMessage << " <<<<" << std::endl << std::endl;
  std::cout << g_visible << std::endl;
  std::cout << "---------------------------" << std::endl;
}

bool showSpecificHelp()
{
  bool shown = false;
  std::ostringstream helpText;

  if(g_vm.count("help-input"))
  {
    helpText << "input" << std::endl;
    helpText << "-----" << std::endl << std::endl;
    helpText << "The \"input\" is a required parameter for all functions. It's the full path of the raster, including the file name and extension." << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-info"))
  {
    helpText << "info" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"info\" function show raster informations, like:" << std::endl;
    helpText << " - Raster Name" << std::endl;
    helpText << " - Number of Columns" << std::endl;
    helpText << " - Number of Rows" << std::endl;
    helpText << " - Number of Bands" << std::endl;
    helpText << " - SRID" << std::endl;
    helpText << " - Resolution in X" << std::endl;
    helpText << " - Resolution in Y" << std::endl;
    helpText << " - Extent UR" << std::endl;
    helpText << " - Extent LL" << std::endl;
    helpText << "And informations about the raster bands, like:" << std::endl;    
    helpText << " - Min Values" << std::endl;    
    helpText << " - Max Values" << std::endl;    
    helpText << " - Mean Values" << std::endl;    
    helpText << " - Std Values" << std::endl;    
    helpText << " - Gain values" << std::endl;    
    helpText << " - Offset values" << std::endl << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-copy"))
  {
    helpText << "copy" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"copy\" is a function that make a copy of the input raster. In this function, the output parameter is required." << std::endl << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << " - output" << std::endl;
    helpText << "Optional parameters:" << std::endl;
    helpText << " - band" << std::endl;
    helpText << " - band-type" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-output"))
  {
    helpText << "output" << std::endl;
    helpText << "------" << std::endl << std::endl;
    helpText << "The \"output\" parameter is the full path of the output raster, including the file name and extension." << std::endl;
    helpText << "It's used by the functions (copy, reproject, ...). Some functions overwrite the input raster if the output doesn't be declared." << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-band"))
  {
    helpText << "band" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"band\" is a copy function parameter that receive a list of bands indexes that the user would like to copy." << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-reproject"))
  {
    helpText << "reproject" << std::endl;
    helpText << "---------" << std::endl << std::endl;
    helpText << "The \"reproject\" is a function that reproject a raster based in the srid passed." << std::endl << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << " - srid" << std::endl;
    helpText << "Optional parameters:" << std::endl;
    helpText << " - output" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-convert"))
  {
    helpText << "convert" << std::endl;
    helpText << "-------" << std::endl << std::endl;
    helpText << "The \"convert\" is a function that convert a raster based in the extension passed." << std::endl;
    helpText << "If you pass the \"output\", the image will have these output path (including name and extension)." << std::endl;
    helpText << "Else, if you pass the \"extension\", the image will have the sabe name, but the extension passed." << std::endl;
    helpText  << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << " - extension or output" << std::endl;
    helpText << "Optional parameters:" << std::endl;
    helpText << " - band" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-resolution"))
  {
    helpText << "resolution" << std::endl;
    helpText << "----------" << std::endl << std::endl;
    helpText << "The \"resolution\" is a function that change the raster resolution based in the method and scale passed." << std::endl << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << " - method" << std::endl;
    helpText << " - scale" << std::endl;
    helpText << "Optional parameters:" << std::endl;
    helpText << " - output" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-method"))
  {
    helpText << "method" << std::endl;
    helpText << "------" << std::endl << std::endl;
    helpText << "The \"method\" is a Resolution function parameter that receive a method for the function is based." << std::endl;
    helpText << "The options are:" << std::endl;
    helpText << " - \"1\" to use Nearest Neighbor method" << std::endl;
    helpText << " - \"2\" to use Bilinear method" << std::endl;
    helpText << " - \"3\" to use Bicubic method" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-scale"))
  {
    helpText << "scale" << std::endl;
    helpText << "-----" << std::endl << std::endl;
    helpText << "The \"scale\" is a Resolution function parameter that receive a scale integer for the function is based." << std::endl;
    helpText << "For example: with a scale equal \"2\", all raster grid cells dimensions will be multiplied to 2." << std::endl;
    helpText << "             but if scale equal \"-2\", all raster grid cells dimensions will be divided to 2." << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-extension"))
  {
    helpText << "extension" << std::endl;
    helpText << "---------" << std::endl << std::endl;
    helpText << "The \"extension\" is a Convert function parameter that receive a extension for the function is based." << std::endl;
    helpText << "The options are:" << std::endl;
    helpText << " - jpg" << std::endl;
    helpText << " - png" << std::endl;
    helpText << " - tif" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-srid"))
  {
    helpText << "srid" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"srid\" is a Reproject function parameter that receive a srid code for the function is based." << std::endl;    
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-trim"))
  {
    helpText << "trim" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"trim\" is a function that trim a raster based in the envelope passed." << std::endl << std::endl;
    helpText << "Required parameters:" << std::endl;
    helpText << " - input" << std::endl;
    helpText << " - envelope" << std::endl;
    helpText << "Optional parameters:" << std::endl;
    helpText << " - output" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-envelope"))
  {
    helpText << "envelope" << std::endl;
    helpText << "--------" << std::endl << std::endl;
    helpText << "--envelope <llx> <lly> <urx> <urx>" << std::endl << std::endl;
    helpText << "The \"envelope\" is a Trim function parameter that receive four doubles representind a envelope (box)." << std::endl;
    helpText << "The order of passing parameters is:" << std::endl;
    helpText << " - llx" << std::endl;
    helpText << " - lly" << std::endl;
    helpText << " - urx" << std::endl;
    helpText << " - ury" << std::endl;
    helpText << "Example: --envelope -40 -30 -20 5" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("help-band-type"))
  {
    helpText << "band type" << std::endl;
    helpText << "----" << std::endl << std::endl;
    helpText << "The \"band type\" is a copy function parameter that define witch type the output raster will have." << std::endl;
    helpText << "(Not implemented yet!)" << std::endl;
    helpText << std::endl;
    shown = true;
  }
  else if(g_vm.count("examples"))
  {
    helpText << "Use Examples" << std::endl;
    helpText << "------------" << std::endl << std::endl;
    helpText << "Info function" << std::endl << std::endl;
    helpText << "--info --input D:\\MyRasters\\raster.tif" << std::endl;
    helpText << std::endl << "Copy function" << std::endl << std::endl;
    helpText << "--copy --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_copy.tif" << std::endl;
    helpText << "--copy --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_copy.tif --band 2 3" << std::endl;
    helpText << "--copy --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_copy.tif --band-type 4" << std::endl;
    helpText << std::endl << "Reproject function" << std::endl << std::endl;
    helpText << "--reproject --input D:\\MyRasters\\raster.tif --srid 29181" << std::endl;
    helpText << "--reproject --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_reprojected.tif --srid 29181" << std::endl;
    helpText << std::endl << "Convert function" << std::endl << std::endl;
    helpText << "--convert --input D:\\MyRasters\\raster.tif --extension jpg" << std::endl;
    helpText << "--convert --input D:\\MyRasters\\raster.tif" "D:\\MyRasters\\raster_converted.jpg" << std::endl;
    helpText << std::endl << "Resolution function" << std::endl << std::endl;
    helpText << "--resolution --input D:\\MyRasters\\raster.tif --method 1 --scale 2" << std::endl;
    helpText << "--resolution --input D:\\MyRasters\\raster.tif --method 1 --scale -2" << std::endl;
    helpText << "--resolution --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_new.tif --method 1 --scale -2" << std::endl;
    helpText << std::endl << "Trim function" << std::endl << std::endl;
    helpText << "--trim --input D:\\MyRasters\\raster.tif --envelope -40 -30 -20 5" << std::endl;
    helpText << "--trim --input D:\\MyRasters\\raster.tif --output D:\\MyRasters\\raster_trimmed.tif --envelope -40 -30 -20 5" << std::endl;
    helpText << std::endl;
    shown = true;
  }

  std::cout << helpText.str() << std::endl;

  return shown;

}

int main(int argc, char* argv[])
{  
  try
  {

    // Variables that receive the attributes
    std::string input             = std::string();
    std::string output            = std::string();
    std::vector<int> bands        = std::vector<int>();
    int srid                      = -1;
    std::string extension         = std::string();
    int method                    = int();
    int scale                     = int();
    std::vector<double> envelope  = std::vector<double>();
    int bandType = -1;
   
    std::string errorMessage  = std::string();
   

    std::cout << std::endl << "TerraLib Raster Manager" << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;    
    
    // Group of functions options
    g_functions.add_options()
    ("info,f", "Show Raster Informations")
    ("copy,p", "Copy function")
    ("reproject,r", "Reproject function")
    ("convert,c", "Convert function")
    ("resolution,l", "Change Resolution function")
    ("trim,t", "Trim function")
    ("help,h", "Terralib Raster Manager. --help-<function> or --help-<parameter>")
    ("examples,x", "Show use examples")
    ("supported,u", "Supported Extensions")
    ;

    // Group of parameters options
    g_parameters.add_options()
    ("input,i", boost::program_options::value<std::string>(), "Input Raster (Required)")
    ("output,o", boost::program_options::value<std::string>(), "Output Raster")  
    ("band,b", boost::program_options::value<std::vector<int>>()->multitoken(), "Band to copy")
    ("method,m", boost::program_options::value<int>(), "Method used to change resolution (\"1\"-NearestNeighbor, \"2\"-Bilinear, \"3\"-Bicubic)")
    ("scale,s", boost::program_options::value<int>(), "Scale used to change resolution")
    ("srid,d", boost::program_options::value<int>(), "Srid used to reproject")
    ("extension,e", boost::program_options::value<std::string>(), "Extension used to convert")
    ("envelope,v", boost::program_options::value<std::vector<double>>()->multitoken(), "Envelope used to trim")
    ("band-type,n", boost::program_options::value<int>(), "Type of the band")
    ;
    
    // Group of help options
    g_help.add_options()
    ("help-input","")
    ("help-info","")
    ("help-copy","")
    ("help-output","")
    ("help-band","")
    ("help-reproject","")
    ("help-convert","")
    ("help-resolution","")
    ("help-method","")
    ("help-extension","")
    ("help-srid","")
    ("help-scale","")
    ("help-trim","")
    ("help-band-type","")    
    ;
    
    // Which will not be shown to the user
    g_hidden.add(g_help);

    // Which will be shown to the user
    g_visible.add(g_functions).add(g_parameters);

    // All options to parse
    g_all.add(g_visible).add(g_hidden);    

    // Parse the options
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, g_all), g_vm);
    boost::program_options::notify(g_vm);
    
    // Check if one of "help" was called and show
    if(showSpecificHelp())
      return EXIT_SUCCESS;

    if(g_vm.count("help") || g_vm.empty())
    {
      std::cout << g_visible << std::endl;
      std::cout << "---------------------------" << std::endl;
      return EXIT_SUCCESS;
    }
   
    // Getting parameters
    input     = (g_vm.count("input") ? g_vm["input"].as<std::string>() : std::string());
    output    = (g_vm.count("output") ? g_vm["output"].as<std::string>() : std::string());
    bands     = (g_vm.count("band") ? g_vm["band"].as<std::vector<int>>() : std::vector<int>());
    srid      = (g_vm.count("srid") ? g_vm["srid"].as<int>() : -1);
    extension = (g_vm.count("extension") ?  g_vm["extension"].as<std::string>() : std::string());
    method    = (g_vm.count("method") ? g_vm["method"].as<int>() : int());
    scale     = (g_vm.count("scale") ? g_vm["scale"].as<int>() : int());
    envelope  = (g_vm.count("envelope") ? g_vm["envelope"].as<std::vector<double>>() : std::vector<double>());
    bandType  = (g_vm.count("band-type") ? g_vm["band-type"].as<int>() : -1);
    

    // Starting Terralib
    TerraLib::getInstance().initialize();
 
    // Supported Estensions
    if(g_vm.count("supported"))
    {
      g_rm.showSupportedExtensions(errorMessage);
      return EXIT_SUCCESS;
    }
    
    // Required
    if(input.empty())
    {
      showError("Comannd line error: input raster missing!");
      return EXIT_SUCCESS;
    }

    if(!g_rm.init(input, errorMessage))
    {
      showError(errorMessage);
      return EXIT_SUCCESS;
    }
    
    // Info
    if(g_vm.count("info"))
    {
      std::ostringstream output;
      if(!g_rm.getRasterInfo(output, errorMessage))
      {
        showError(errorMessage);
      }
      else
        std::cout << output.str();
      return EXIT_SUCCESS;
    }
    
    // Copy
    else if(g_vm.count("copy"))
    {
      if(output.empty())
      {
        showError("For copy, output is required!");
        return EXIT_SUCCESS;
      }
      else if(!g_rm.copyRaster(output, bands, bandType, errorMessage))
      {
        showError(errorMessage);
      }
      else
      {
        std::cout << "Rester Copied!" << std::endl << std::endl;        
      }
    }

    // Reproject
    else if(g_vm.count("reproject"))
    { 
      if(srid == -1)
      {
        showError("For reproject, srid is required!");
        return EXIT_SUCCESS;
      }
      if(!g_rm.reproject(output, srid, errorMessage))
      {
        showError(errorMessage);
      }
      else
      {
        std::cout << "Rester Reprojected!" << std::endl << std::endl;
      }      
    }

    // Convert
    else if(g_vm.count("convert"))
    {
      if(extension.empty() && output.empty())
      {
        showError("For Convert, extension or output is required!");
        return EXIT_SUCCESS;
      }
      if(!g_rm.convert(output, extension, bands, errorMessage))
      {
        showError(errorMessage);
      }
      else
      {
        std::cout << "Rester Converted!" << std::endl << std::endl;
      }      
    }

    // Change Resolution
    else if(g_vm.count("resolution"))
    {
      if(method == 0 || scale == 0)
      {
        showError("For change Resolution, method and scale are required!");
        return EXIT_SUCCESS;
      }
      if(!g_rm.changeResolution(output, method, scale, errorMessage))
      {
        showError(errorMessage);
      }
      else
      {
        std::cout << "Resolution Changed!" << std::endl << std::endl;
      }
    }

    // Trim
    else if(g_vm.count("trim"))
    {
      if(!g_rm.trim(output, envelope, errorMessage))
      {
        showError(errorMessage);
      }
      else
      {
        std::cout << "Trim executed!" << std::endl << std::endl;
      }
    }
    
    // Show Raster Info as Default
    else
    {
      std::ostringstream output;
      if(!g_rm.getRasterInfo(output, errorMessage))
      {
        showError(errorMessage);
      }      
      else
        std::cout << output.str();
    }  

    // Finalize Terralib
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