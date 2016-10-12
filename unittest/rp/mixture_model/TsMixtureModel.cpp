/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/unittest/rp/mixture_model/TsMixtureModel.cpp

  \brief A test suit for the Mixture Model interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (mixtureModel_tests)

BOOST_AUTO_TEST_CASE(linear_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster for linear mixture model */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop_linearMixtureModel.tif";

  /* Create algorithm parameters */

  te::rp::MixtureModel::InputParameters mmInputParameters;
  te::rp::MixtureModel::OutputParameters mmOutputParameters;

  /* Defining input parameters */

  mmInputParameters.m_inputRasterPtr = rin;
  for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
    mmInputParameters.m_inputRasterBands.push_back(b);
  mmInputParameters.m_strategyName = "linear";

  /* Link specific parameters with chosen implementation */

  te::rp::MixtureModelLinearStrategy::Parameters lmmParameters;
  mmInputParameters.setMixtureModelStrategyParams(lmmParameters);

  /* Defining endmembers */

  mmInputParameters.m_components["clouds"].push_back(255.0);
  mmInputParameters.m_components["clouds"].push_back(184.875);
  mmInputParameters.m_components["clouds"].push_back(255.0);

  mmInputParameters.m_components["shadow"].push_back(42.075);
  mmInputParameters.m_components["shadow"].push_back(24.99);
  mmInputParameters.m_components["shadow"].push_back(49.98);

  mmInputParameters.m_components["vegetation"].push_back(99.96);
  mmInputParameters.m_components["vegetation"].push_back(64.005);
  mmInputParameters.m_components["vegetation"].push_back(154.02);

  /* Defining output parameters */

  mmOutputParameters.m_rInfo = orinfo;
  mmOutputParameters.m_rType = "GDAL";
  mmOutputParameters.m_createErrorRaster = true;

  /* Execute the algorithm */

  te::rp::MixtureModel algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(mmInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(mmOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_CASE(pca_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster for PCA mixture model */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop_PCAMixtureModel.tif";

  /* Create algorithm parameters */

  te::rp::MixtureModel::InputParameters mmInputParameters;
  te::rp::MixtureModel::OutputParameters mmOutputParameters;

  /* Defining input parameters */

  mmInputParameters.m_inputRasterPtr = rin;
  for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
    mmInputParameters.m_inputRasterBands.push_back(b);
  mmInputParameters.m_strategyName = "pca";

  /* Link specific parameters with chosen implementation */

  te::rp::MixtureModelPCAStrategy::Parameters pcammParameters;
  mmInputParameters.setMixtureModelStrategyParams(pcammParameters);

  /* Defining endmembers */

  mmInputParameters.m_components["clouds"].push_back(255.0);
  mmInputParameters.m_components["clouds"].push_back(184.875);
  mmInputParameters.m_components["clouds"].push_back(255.0);

  mmInputParameters.m_components["shadow"].push_back(42.075);
  mmInputParameters.m_components["shadow"].push_back(24.99);
  mmInputParameters.m_components["shadow"].push_back(49.98);

  mmInputParameters.m_components["vegetation"].push_back(99.96);
  mmInputParameters.m_components["vegetation"].push_back(64.005);
  mmInputParameters.m_components["vegetation"].push_back(154.02);

  /* Defining output parameters */

  mmOutputParameters.m_rInfo = orinfo;
  mmOutputParameters.m_rType = "GDAL";
  mmOutputParameters.m_createErrorRaster = true;

  /* Execute the algorithm */

  te::rp::MixtureModel algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(mmInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(mmOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_SUITE_END()
