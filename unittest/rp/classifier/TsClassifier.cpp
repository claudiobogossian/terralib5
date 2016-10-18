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
  \file terralib/unittest/rp/classifier/TsClassifier.cpp

  \brief Main file of test suit for Classifier interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/dataaccess.h>
#include <terralib/rp.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/memory.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (classifier_tests)

/* This code is for image segmentation to be used with ISOSegClassifier function */
void SegmentImage(te::rst::Raster* rin, std::vector<te::gm::Polygon*>& polygons, 
  std::vector< double >& polsValues )
{
 /* Define segmentation parameters */

 /* Create output raster info */
 std::map<std::string, std::string> orinfo;
 orinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop_segmented.tif";

 /* Input parameters */

 te::rp::Segmenter::InputParameters algoInputParameters;
 algoInputParameters.m_inputRasterPtr = rin;
 algoInputParameters.m_inputRasterBands.push_back(0);
 algoInputParameters.m_inputRasterBands.push_back(1);
 algoInputParameters.m_inputRasterBands.push_back(2);
 algoInputParameters.m_enableBlockProcessing = false;

 /* Link specific parameters with chosen implementation */
 te::rp::SegmenterRegionGrowingMeanStrategy::Parameters segparameters;
 segparameters.m_minSegmentSize = 50;
 segparameters.m_segmentsSimilarityThreshold = 0.10;

 algoInputParameters.m_strategyName = "RegionGrowingMean";
 algoInputParameters.setSegStrategyParams(segparameters);

 /* Output parameters */

 te::rp::Segmenter::OutputParameters algoOutputParameters;
 algoOutputParameters.m_rInfo = orinfo;
 algoOutputParameters.m_rType = "GDAL";

 /* Execute the algorithm */

 te::rp::Segmenter seginstance;

 if(!seginstance.initialize(algoInputParameters)) throw;
 if(!seginstance.execute(algoOutputParameters )) throw;

 /* Export the segmentation into shapefile */
 std::vector<te::gm::Geometry*> geometries;
 algoOutputParameters.m_outputRasterPtr->vectorize( geometries, 0, 0, &polsValues );

 for (unsigned i = 0; i < geometries.size(); i++)
 {
   polygons.push_back(static_cast<te::gm::Polygon*> (geometries[i]));
 }
}

void saveToShp( std::vector<te::gm::Polygon*> polygons, std::vector< double >& polygonsValues,
  const std::string& shpBaseFileName )
{
  std::auto_ptr<te::da::DataSetType> dataSetTypePtr1(new te::da::DataSetType(shpBaseFileName));
  
  dataSetTypePtr1->add( new te::dt::SimpleProperty("value", te::dt::DOUBLE_TYPE, true) );  
  dataSetTypePtr1->add( new te::dt::SimpleProperty("id", te::dt::DOUBLE_TYPE, true) );
  dataSetTypePtr1->add( new te::gm::GeometryProperty("polygon", polygons[ 0 ]->getSRID(), 
    te::gm::PolygonType, true) );  
  
  std::auto_ptr<te::da::DataSetType> dataSetTypePtr2( new te::da::DataSetType( *dataSetTypePtr1 ) );
  
  std::auto_ptr< te::mem::DataSet > memDataSetPtr( new te::mem::DataSet( dataSetTypePtr1.get()) );
  
  for( unsigned int polygonsIdx = 0 ; polygonsIdx < polygons.size() ; ++polygonsIdx )
  {
    te::mem::DataSetItem* dsItemPtr = new te::mem::DataSetItem(memDataSetPtr.get());
    dsItemPtr->setDouble( 0, polygonsValues[ polygonsIdx ] );
    dsItemPtr->setDouble( 1, polygonsIdx );
    dsItemPtr->setGeometry( 2, (te::gm::Geometry*)polygons[ polygonsIdx ]->clone() );
    
    memDataSetPtr->add( dsItemPtr );
  }

  remove( ( shpBaseFileName + ".shx" ).c_str() );
  remove( ( shpBaseFileName + ".shp" ).c_str() );
  remove( ( shpBaseFileName + ".prj" ).c_str() );
  remove( ( shpBaseFileName + ".dbf" ).c_str() );
  
  std::string connInfo("File://" + shpBaseFileName + ".shp");
  
  std::unique_ptr<te::da::DataSource> dsOGR( te::da::DataSourceFactory::make("OGR", connInfo) );
  dsOGR->open();
  
  memDataSetPtr->moveBeforeFirst();
  
  te::da::Create(dsOGR.get(), dataSetTypePtr2.get(), memDataSetPtr.get());

  dsOGR->close();
}  

BOOST_AUTO_TEST_CASE(ISOSeg_test)
{
 /* First open the input image */

 std::map<std::string, std::string> rinfo;
 rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

 std::unique_ptr< te::rst::Raster > rin( te::rst::RasterFactory::open(rinfo) );

 /* To apply ISOSeg the image must be segmented */
 std::vector<te::gm::Polygon*> pin;
 std::vector< double > polsValues;
 SegmentImage(rin.get(), pin, polsValues );
 saveToShp( pin, polsValues, "terralib_unittest_rp_Classifier_ISOSeg_Test" );

 /* Define classification parameters */

 /* Input parameters */
 te::rp::Classifier::InputParameters algoInputParameters;
 algoInputParameters.m_inputRasterPtr = rin.get();
 algoInputParameters.m_inputRasterBands.push_back(0);
 algoInputParameters.m_inputRasterBands.push_back(1);
 algoInputParameters.m_inputRasterBands.push_back(2);
 algoInputParameters.m_inputPolygons = pin;

 /* Link specific parameters with chosen implementation */

 te::rp::ClassifierISOSegStrategy::Parameters classifierparameters;
 classifierparameters.m_acceptanceThreshold = 99.0;
 algoInputParameters.m_strategyName = "isoseg";
 
 // Mahalanobis distance test

 {
   classifierparameters.m_distanceType = 
     te::rp::ClassifierISOSegStrategy::Parameters::MahalanobisDistanceType;
   algoInputParameters.setClassifierStrategyParams(classifierparameters);   
   
   /* Output parameters */
 
   std::map<std::string, std::string> orinfo;
   orinfo["URI"] = 
     "terralib_unittest_rp_Classifier_ISOSeg_Mahalanobis_Test.tif";

   te::rp::Classifier::OutputParameters algoOutputParameters;
   algoOutputParameters.m_rInfo = orinfo;
   algoOutputParameters.m_rType = "GDAL";

   /* Execute the algorithm */

   te::rp::Classifier algorithmInstance;

   BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
   BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );
 }
 
 // Bhattacharyya distance test

 {
   classifierparameters.m_distanceType = 
     te::rp::ClassifierISOSegStrategy::Parameters::BhattacharyyaDistanceType;
   algoInputParameters.setClassifierStrategyParams(classifierparameters);  
   
   /* Output parameters */
 
   std::map<std::string, std::string> orinfo;
   orinfo["URI"] = 
     "terralib_unittest_rp_Classifier_ISOSeg_Bhattacharyya_Test.tif";

   te::rp::Classifier::OutputParameters algoOutputParameters;
   algoOutputParameters.m_rInfo = orinfo;
   algoOutputParameters.m_rType = "GDAL";

   /* Execute the algorithm */

   te::rp::Classifier algorithmInstance;

   BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
   BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );
 } 
}

BOOST_AUTO_TEST_CASE(MAP_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_MAP_Test.tif";

  /* Defining the classes samples */

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

  /* Define classification parameters */

  /* Input parameters */

  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

  /* Link specific parameters with chosen implementation */

  te::rp::ClassifierMAPStrategy::Parameters classifierparameters;
  classifierparameters.m_trainSamplesPtr = allClassesSamples;

  algoInputParameters.m_strategyName = "map";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

  /* Output parameters */

  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

  /* Execute the algorithm */

  te::rp::Classifier algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_CASE(EM_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_EM_Test.tif";

  /* Define classification parameters */

  /* Input parameters */

  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

  /* Link specific parameters with chosen implementation */

  te::rp::ClassifierEMStrategy::Parameters classifierparameters;
  classifierparameters.m_numberOfClusters = 4;
  classifierparameters.m_maxIterations = 100;
  classifierparameters.m_maxInputPoints = 1000;
  classifierparameters.m_epsilon = 15.0;
  classifierparameters.m_clustersMeans = std::vector<std::vector<double> >();

  algoInputParameters.m_strategyName = "em";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

  /* Output parameters */

  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

  /* Execute the algorithm */

  te::rp::Classifier algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_CASE(SAM_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_SAM_Test.tif";

  /* Defining the classes samples */

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

  /* Define classification parameters */

  /* Input parameters */

  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

  /* Link specific parameters with chosen implementation */

  te::rp::ClassifierSAMStrategy::Parameters classifierparameters;
  classifierparameters.m_trainSamplesPtr = allClassesSamples;
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  classifierparameters.m_maxAngularDistances.push_back( 0.2 );
  algoInputParameters.m_strategyName = "sam";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

  /* Output parameters */

  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

  /* Execute the algorithm */

  te::rp::Classifier algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_CASE(KMeans_test)
{
  /* First open the input image */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif";

  te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_rp_Classifier_KMeans_Test.tif";

  /* Define classification parameters */

  /* Input parameters */

  te::rp::Classifier::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

  /* Link specific parameters with chosen implementation */

  te::rp::ClassifierKMeansStrategy::Parameters classifierparameters;
  classifierparameters.m_K = 4;
  classifierparameters.m_maxIterations = 100;
  classifierparameters.m_maxInputPoints = 1000;
  classifierparameters.m_epsilon = 15.0;

  algoInputParameters.m_strategyName = "kmeans";
  algoInputParameters.setClassifierStrategyParams(classifierparameters);

  /* Output parameters */

  te::rp::Classifier::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = orinfo;
  algoOutputParameters.m_rType = "GDAL";

  /* Execute the algorithm */

  te::rp::Classifier algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize(algoInputParameters) );
  BOOST_CHECK( algorithmInstance.execute(algoOutputParameters) );

  /* Clean up */
  delete rin;
}

BOOST_AUTO_TEST_SUITE_END()
