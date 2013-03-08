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
  \file TsClassifier.cpp

  \brief A test suit for the Classifier interface.
 */

// TerraLib
#include "../Config.h"
#include <terralib/gdal.h>
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/rp/Classifier.h>
#include <terralib/rp/ClassifierISOSegStrategy.h>
#include <terralib/rp/ClassifierMAPStrategy.h>
#include <terralib/rp/Segmenter.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>
#include "TsClassifier.h"

// STL
//#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( TsClassifier );

// this code is for image segmentation to be used with ISOSegClassifier function
std::vector<te::gm::Polygon*> SegmentImage(te::rst::Raster* rin)
{
// define segmentation parameters

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TE_DATA_LOCALE"/data/rasters/cbers2b_rgb342_crop_segmented.tif";

// input parameters
  te::rp::Segmenter::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::SegmenterRegionGrowingStrategy::Parameters segparameters;
  segparameters.m_minSegmentSize = 250;
  segparameters.m_segmentsSimilarityThreshold = 30;
  segparameters.m_segmentFeatures = 
    te::rp::SegmenterRegionGrowingStrategy::Parameters::MeanFeaturesType;

  algoInputParameters.m_strategyName = "RegionGrowing";
  algoInputParameters.setSegStrategyParams(segparameters);

// output parameters
  te::rp::Segmenter::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
  te::rp::Segmenter seginstance;

  if(!seginstance.initialize(algoInputParameters)) throw;
  if(!seginstance.execute(algoOutputParameters )) throw;

// export the segmentation into shapefile
  std::vector<te::gm::Geometry*> geometries;
  te::gdal::Vectorize(((te::gdal::Raster*) algoOutputParameters.m_outputRasterPtr.get())->getGDALDataset()->GetRasterBand(1), geometries);

  std::vector<te::gm::Polygon*> polygons;
  for (unsigned i = 0; i < geometries.size(); i++)
    polygons.push_back(static_cast<te::gm::Polygon*> (geometries[i]));

  return polygons;
}

void TsClassifier::ISOSeg()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_ISOSeg_Test.tif";

// to apply ISOSeg the image must be segmented
  std::vector<te::gm::Polygon*> pin = SegmentImage(rin);

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);
  algoInputParameters.m_inputPolygons = pin;

// link specific parameters with chosen implementation
  te::rp::ClassifierISOSegStrategy::Parameters classifierparameters;
  classifierparameters.m_acceptanceThreshold = 99.0;

  algoInputParameters.m_strategyName = "isoseg";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

// output parameters
  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
  te::rp::Classifier algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize(algoInputParameters) );
  CPPUNIT_ASSERT( algorithmInstance.execute(algoOutputParameters) );

// clean up
  delete rin;
}

void TsClassifier::MAP()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TE_DATA_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_MAP_Test.tif";

// Defining the classes samples

  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC1_1;
  sampleC1_1.push_back( 20 );
  sampleC1_1.push_back( 190 );
  sampleC1_1.push_back( 50 );
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC1_2;
  sampleC1_2.push_back( 143 );
  sampleC1_2.push_back( 242 );
  sampleC1_2.push_back( 174 );  
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC1_3;
  sampleC1_3.push_back( 81 );
  sampleC1_3.push_back( 226 );
  sampleC1_3.push_back( 112 );  
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSamplesContainerT class1Samples;
  class1Samples.push_back( sampleC1_1 );
  class1Samples.push_back( sampleC1_2 );
  class1Samples.push_back( sampleC1_3 );
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC2_1;
  sampleC2_1.push_back( 255 );
  sampleC2_1.push_back( 226 );
  sampleC2_1.push_back( 245 );  

  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC2_2;
  sampleC2_2.push_back( 168 );
  sampleC2_2.push_back( 138 );
  sampleC2_2.push_back( 122 );  
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSampleT sampleC2_3;
  sampleC2_3.push_back( 179 );
  sampleC2_3.push_back( 167 );
  sampleC2_3.push_back( 153 );  
  
  te::rp::ClassifierMAPStrategy::Parameters::ClassSamplesContainerT class2Samples;
  class2Samples.push_back( sampleC2_1 );
  class2Samples.push_back( sampleC2_2 );
  class2Samples.push_back( sampleC2_3 );  
  
  te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT allClassesSamples;
  allClassesSamples[ 1 ] = class1Samples;
  allClassesSamples[ 2 ] = class2Samples;  

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::ClassifierMAPStrategy::Parameters classifierparameters;
  classifierparameters.m_trainSamplesPtr = &allClassesSamples;
  classifierparameters.m_classThresholds.push_back( 0.5 );
  classifierparameters.m_classThresholds.push_back( 0.5 );

  algoInputParameters.m_strategyName = "map";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

// output parameters
  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
  te::rp::Classifier algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize(algoInputParameters) );
  CPPUNIT_ASSERT( algorithmInstance.execute(algoOutputParameters) );

// clean up
  delete rin;
}
