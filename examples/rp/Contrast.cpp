#include "RPExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/dataset/DataSetPersistence.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterSummaryManager.h>
#include <terralib/rp/Contrast.h>

// STL
#include <string>
#include <map>

void Contrast()
{
  try
  {
    std::cout << "Contrast example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

    bool executeok = false;
    bool initok = false;
    {
      std::cout << "Using Linear Contrast" << std::endl;

// create output raster for linear contrast
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_linear_contrast.tif";

      te::rst::Grid* ogrid = new te::rst::Grid(*rin->getGrid());

      std::vector<te::rst::BandProperty*> obands;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
        obands.push_back(new te::rst::BandProperty(b, rin->getBandDataType(b), "band with linear contrast"));

      te::rst::Raster* rout = te::rst::RasterFactory::make(ogrid, obands, orinfo);

// create contrast algorithm parameters
      te::rp::Contrast::Parameters contparameters;

      contparameters.m_type = te::rp::Contrast::Parameters::LinearContrastT;
      contparameters.m_minInput = 100;
      contparameters.m_maxInput = 255;
      contparameters.m_inRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
      {
        contparameters.m_inRasterBands.push_back(b);
        contparameters.m_outRasterBands.push_back(b);
      }
      contparameters.m_outRasterPtr = rout;
      contparameters.m_outDataSourcePtr = 0;
      contparameters.m_outDataSetName = "";

// execute the algorithm
      te::rp::Contrast continstance;

      initok = continstance.initialize(contparameters);

      if (initok)
        executeok = continstance.execute();

      if (!executeok)
        std::cout << "Problems in linear contrast." << std::endl;

      delete rout;
    }

    {
      std::cout << "Using Histogram Equalization Contrast" << std::endl;

// create output raster for linear contrast
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_histogram_equalization_contrast.tif";

      te::rst::Grid* ogrid = new te::rst::Grid(*rin->getGrid());

      std::vector<te::rst::BandProperty*> obands;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
        obands.push_back(new te::rst::BandProperty(b, te::dt::DOUBLE_TYPE, "band with histogram equalization contrast"));

      te::rst::Raster* rout = te::rst::RasterFactory::make(ogrid, obands, orinfo);

// create contrast algorithm parameters
      te::rp::Contrast::Parameters contparameters;

      contparameters.m_type = te::rp::Contrast::Parameters::HistogramEqualizationContrastT;
      contparameters.m_inRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
      {
        contparameters.m_inRasterBands.push_back(b);
        contparameters.m_outRasterBands.push_back(b);
      }
      contparameters.m_outRasterPtr = rout;
      contparameters.m_outDataSourcePtr = 0;
      contparameters.m_outDataSetName = "";

// execute the algorithm
      te::rp::Contrast continstance;

      initok = continstance.initialize(contparameters);

      if (initok)
        executeok = continstance.execute();

      if (!executeok)
        std::cout << "Problems in histogram equalization contrast." << std::endl;

      delete rout;
    }

    {
      std::cout << "Using Contrast with predefined Mean and Std" << std::endl;

// create output raster for linear contrast
      std::map<std::string, std::string> orinfo;
      orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_meanstd_contrast.tif";

      te::rst::Grid* ogrid = new te::rst::Grid(*rin->getGrid());

      std::vector<te::rst::BandProperty*> obands;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
        obands.push_back(new te::rst::BandProperty(b, te::dt::DOUBLE_TYPE, "band with predefined mean and std contrast"));

      te::rst::Raster* rout = te::rst::RasterFactory::make(ogrid, obands, orinfo);

// create contrast algorithm parameters
      te::rp::Contrast::Parameters contparameters;

      contparameters.m_type = te::rp::Contrast::Parameters::SetMeanAndStdContrastT;
      contparameters.m_meanInput = 200;
      contparameters.m_stdInput = 50;
      contparameters.m_inRasterPtr = rin;
      for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
      {
        contparameters.m_inRasterBands.push_back(b);
        contparameters.m_outRasterBands.push_back(b);
      }
      contparameters.m_outRasterPtr = rout;
      contparameters.m_outDataSourcePtr = 0;
      contparameters.m_outDataSetName = "";

// execute the algorithm
      te::rp::Contrast continstance;

      initok = continstance.initialize(contparameters);

      if (initok)
        executeok = continstance.execute();

      if (!executeok)
        std::cout << "Problems in contrast with predefined Mean and Std." << std::endl;

      const te::rst::RasterSummary* rinsummary = te::rst::RasterSummaryManager::getInstance().get(rin, te::rst::SUMMARY_ALL);
      const te::rst::RasterSummary* routsummary = te::rst::RasterSummaryManager::getInstance().get(rout, te::rst::SUMMARY_ALL);

      for( unsigned int b = 0 ; b < rin->getNumberOfBands(); b++ )
      {
        std::cout << "================" << std::endl;
        std::cout << "rin mean at band " << b << ": " << rinsummary->at(b).m_meanVal->real() << std::endl;
        std::cout << "rin std at band " << b << ": " << rinsummary->at(b).m_stdVal->real() << std::endl << std::endl;

        std::cout << "rout mean at band " << b << ": " << routsummary->at(b).m_meanVal->real() << std::endl;
        std::cout << "rout std at band " << b << ": " << routsummary->at(b).m_stdVal->real() << std::endl << std::endl;
      }

      delete rout;
    }

    // clean up
    delete rin;

    if (executeok)
      std::cout << "Done!" << std::endl << std::endl;
    else
      std::cout << "Problems in contrast." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Contrast(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Contrast()!" << std::endl;
  }
}

