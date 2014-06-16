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
  \file TsMixtureModel.cpp

  \brief A test suit for the Mixture Model interface.
 */

// TerraLib
#include "../Config.h"
#include "TsMixtureModel.h"
#include <terralib/gdal.h>
#include <terralib/raster.h>
#include <terralib/rp/MixtureModel.h>
#include <terralib/rp/MixtureModelLinearStrategy.h>
#include <terralib/rp/MixtureModelPCAStrategy.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMixtureModel );

void TsMixtureModel::Linear()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/data/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster for linear mixture model
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/data/rasters/cbers2b_rgb342_crop_linearMixtureModel.tif";

// create algorithm parameters
  te::rp::MixtureModel::InputParameters mmInputParameters;
  te::rp::MixtureModel::OutputParameters mmOutputParameters;

// defining input parameters
  mmInputParameters.m_inputRasterPtr = rin;
  for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
    mmInputParameters.m_inputRasterBands.push_back(b);
  mmInputParameters.m_strategyName = "linear";

// link specific parameters with chosen implementation
  te::rp::MixtureModelLinearStrategy::Parameters lmmParameters;
  mmInputParameters.setMixtureModelStrategyParams(lmmParameters);

// defining endmembers
  mmInputParameters.m_components["clouds"].push_back(255.0);
  mmInputParameters.m_components["clouds"].push_back(184.875);
  mmInputParameters.m_components["clouds"].push_back(255.0);

  mmInputParameters.m_components["shadow"].push_back(42.075);
  mmInputParameters.m_components["shadow"].push_back(24.99);
  mmInputParameters.m_components["shadow"].push_back(49.98);

  mmInputParameters.m_components["vegetation"].push_back(99.96);
  mmInputParameters.m_components["vegetation"].push_back(64.005);
  mmInputParameters.m_components["vegetation"].push_back(154.02);

// defining output parameters
  mmOutputParameters.m_rInfo = orinfo;
  mmOutputParameters.m_rType = "GDAL";
  mmOutputParameters.m_normalizeOutput = true;
  mmOutputParameters.m_createErrorRaster = true;

// execute the algorithm
  te::rp::MixtureModel algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize(mmInputParameters) );
  CPPUNIT_ASSERT( algorithmInstance.execute(mmOutputParameters) );

// clean up
  delete rin;
}

void TsMixtureModel::PCA()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/data/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster for PCA mixture model
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/data/rasters/cbers2b_rgb342_crop_PCAMixtureModel.tif";

// create algorithm parameters
  te::rp::MixtureModel::InputParameters mmInputParameters;
  te::rp::MixtureModel::OutputParameters mmOutputParameters;

// defining input parameters
  mmInputParameters.m_inputRasterPtr = rin;
  for (unsigned b = 0; b < rin->getNumberOfBands(); b++)
    mmInputParameters.m_inputRasterBands.push_back(b);
  mmInputParameters.m_strategyName = "pca";

// link specific parameters with chosen implementation
  te::rp::MixtureModelPCAStrategy::Parameters pcammParameters;
  mmInputParameters.setMixtureModelStrategyParams(pcammParameters);

// defining endmembers
  mmInputParameters.m_components["clouds"].push_back(255.0);
  mmInputParameters.m_components["clouds"].push_back(184.875);
  mmInputParameters.m_components["clouds"].push_back(255.0);

  mmInputParameters.m_components["shadow"].push_back(42.075);
  mmInputParameters.m_components["shadow"].push_back(24.99);
  mmInputParameters.m_components["shadow"].push_back(49.98);

  mmInputParameters.m_components["vegetation"].push_back(99.96);
  mmInputParameters.m_components["vegetation"].push_back(64.005);
  mmInputParameters.m_components["vegetation"].push_back(154.02);

// defining output parameters
  mmOutputParameters.m_rInfo = orinfo;
  mmOutputParameters.m_rType = "GDAL";
  mmOutputParameters.m_normalizeOutput = true;
  mmOutputParameters.m_createErrorRaster = true;

// execute the algorithm
  te::rp::MixtureModel algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize(mmInputParameters) );
  CPPUNIT_ASSERT( algorithmInstance.execute(mmOutputParameters) );

// clean up
  delete rin;
}
