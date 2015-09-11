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
  \file TsClassifier.cpp

  \brief A test suit for the Classifier interface.
 */

// TerraLib
#include "../Config.h"
#include <terralib/gdal.h>
#include <terralib/gdal/Utils.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

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
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_segmented.tif";

// input parameters
  te::rp::Segmenter::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);
  algoInputParameters.m_enableBlockProcessing = false;

// link specific parameters with chosen implementation
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters segparameters;
  segparameters.m_minSegmentSize = 50;
  segparameters.m_segmentsSimilarityThreshold = 0.10;

  algoInputParameters.m_strategyName = "RegionGrowingMean";
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
  {
    polygons.push_back(static_cast<te::gm::Polygon*> (geometries[i]));
    polygons[ i ]->setSRID( rin->getGrid()->getSRID() );
  }

  return polygons;
}

void TsClassifier::ISOSeg()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/terralib_unittest_rp_Classifier_ISOSeg_Test.tif";

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
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/terralib_unittest_rp_Classifier_MAP_Test.tif";

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

  te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCTPtr allClassesSamples(new te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT());
  allClassesSamples->insert(te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT::value_type(1, class1Samples));
  allClassesSamples->insert(te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCT::value_type(2, class2Samples));

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::ClassifierMAPStrategy::Parameters classifierparameters;
  classifierparameters.m_trainSamplesPtr = allClassesSamples;

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

void TsClassifier::EM()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/terralib_unittest_rp_Classifier_EM_Test.tif";

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::ClassifierEMStrategy::Parameters classifierparameters;
  classifierparameters.m_numberOfClusters = 4;
  classifierparameters.m_maxIterations = 100;
  classifierparameters.m_maxInputPoints = 1000;
  classifierparameters.m_epsilon = 15.0;
  classifierparameters.m_clustersMeans = std::vector<std::vector<double> >();

  algoInputParameters.m_strategyName = "em";
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

void TsClassifier::SAM()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/terralib_unittest_rp_Classifier_SAM_Test.tif";

// Defining the classes samples

  te::rp::ClassifierSAMStrategy::SampleT sampleC1_1;
  sampleC1_1.push_back( 166 );
  sampleC1_1.push_back( 255 );
  sampleC1_1.push_back( 255 );
  te::rp::ClassifierSAMStrategy::SamplesT class1Samples;
  class1Samples.push_back( sampleC1_1 );

  te::rp::ClassifierSAMStrategy::SampleT sampleC2_1;
  sampleC2_1.push_back( 36 );
  sampleC2_1.push_back( 255 );
  sampleC2_1.push_back( 76 );
  te::rp::ClassifierSAMStrategy::SamplesT class2Samples;
  class2Samples.push_back( sampleC2_1 );
  
  te::rp::ClassifierSAMStrategy::SampleT sampleC3_1;
  sampleC3_1.push_back( 36 );
  sampleC3_1.push_back( 36 );
  sampleC3_1.push_back( 76 );
  te::rp::ClassifierSAMStrategy::SamplesT class3Samples;
  class3Samples.push_back( sampleC3_1 );  

  te::rp::ClassifierSAMStrategy::ClassesSamplesTPtr allClassesSamples(new te::rp::ClassifierSAMStrategy::ClassesSamplesT());
  allClassesSamples->insert(te::rp::ClassifierSAMStrategy::ClassesSamplesT::value_type(1, class1Samples));
  allClassesSamples->insert(te::rp::ClassifierSAMStrategy::ClassesSamplesT::value_type(2, class2Samples));
  allClassesSamples->insert(te::rp::ClassifierSAMStrategy::ClassesSamplesT::value_type(3, class3Samples));

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::ClassifierSAMStrategy::Parameters classifierparameters;
  classifierparameters.m_trainSamplesPtr = allClassesSamples;
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  algoInputParameters.m_strategyName = "sam";
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

void TsClassifier::KMeans()
{
// first open the input image
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = TERRALIB_DATA_DIR"/rasters/terralib_unittest_rp_Classifier_KMeans_Test.tif";

// define classification parameters

// input parameters
  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
  te::rp::ClassifierKMeansStrategy::Parameters classifierparameters;
  classifierparameters.m_K = 4;
  classifierparameters.m_maxIterations = 100;
  classifierparameters.m_maxInputPoints = 1000;
  classifierparameters.m_epsilon = 15.0;

  algoInputParameters.m_strategyName = "kmeans";
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
