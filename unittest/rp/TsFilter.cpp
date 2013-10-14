/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsFilter.cpp

  \brief A test suit for the Filter interface.
 */

// TerraLib
#include "../Config.h"
#include <terralib/gdal.h>
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/rp/Filter.h>
#include "TsFilter.h"

// STL
//#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( TsFilter );

void TsFilter::Filters()
{
// open input raster
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_DIR"/data/rasters/cbers2b_rgb342_crop.tif";
  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// define a vector of filter names and types to reduce lines of code for testing
  std::vector<std::string> filterNames;
  filterNames.push_back("sobel");
  filterNames.push_back("mean");
  filterNames.push_back("mode");
  filterNames.push_back("median");
  filterNames.push_back("dilation");
  filterNames.push_back("erosion");
  filterNames.push_back("user_defined");

  std::vector<te::rp::Filter::InputParameters::FilterType> filterTypes;
  filterTypes.push_back(te::rp::Filter::InputParameters::SobelFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::MeanFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::ModeFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::MedianFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::DilationFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::ErosionFilterT);
  filterTypes.push_back(te::rp::Filter::InputParameters::UserDefinedWindowT);

  for (unsigned int f = 0; f < filterNames.size(); f++)
  {
    std::cout << "Testing filter " << filterNames[f] << std::endl;
    
// create output raster for sobel filter in band 0
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = TE_DATA_DIR"/data/rasters/cbers2b_rgb342_crop_" + filterNames[f] + "_filter.tif";

// create filter algorithm parameters
    te::rp::Filter::InputParameters filterInputParameters;
// examples of filters include MeanFilterT, ModeFilterT, MedianFilterT, DilationFilterT and ErosionFilterT
    filterInputParameters.m_type = filterTypes[f];
    filterInputParameters.m_windowH = 3;
    filterInputParameters.m_windowW = 3;
    filterInputParameters.m_enableProgress = true;
    filterInputParameters.m_inRasterPtr = rin;
    for (unsigned int b = 0; b < rin->getNumberOfBands(); b++)
      filterInputParameters.m_inRasterBands.push_back(b);
      
    if (filterNames[f] == "user_defined")
    {
// example of a user defined window (this example shows the X component of the Sobel filter
      boost::numeric::ublas::matrix<double> window(3, 3);
      window(0, 0) = 1;
      window(0, 1) = 2;
      window(0, 2) = 1;
      window(1, 0) = 0;
      window(1, 1) = 0;
      window(1, 2) = 0;
      window(2, 0) = -1;
      window(2, 1) = -2;
      window(2, 2) = -1;

      filterInputParameters.m_window = window;
    }

    te::rp::Filter::OutputParameters filterOutputParameters;
    filterOutputParameters.m_createdOutRasterInfo = orinfo;
    filterOutputParameters.m_createdOutRasterDSType = "GDAL";
    filterOutputParameters.m_normalizeOutput = false;

// execute the algorithm
    te::rp::Filter filterInstance;
    CPPUNIT_ASSERT(filterInstance.initialize(filterInputParameters));
    CPPUNIT_ASSERT(filterInstance.execute(filterOutputParameters));
  }

// clean up
  delete rin;
}
