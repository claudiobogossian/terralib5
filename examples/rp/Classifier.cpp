#include "RPExamples.h"

// TerraLib
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/rp.h>

// STL
#include <map>
#include <string>

std::vector<te::gm::Polygon*> CreatePolygons(te::rst::Raster* rin)
{
// create 4 input polygons
  std::vector<te::gm::Polygon*> pin;

  double xc = (rin->getExtent()->getUpperRightX() + rin->getExtent()->getLowerLeftX())/2;
  double yc = (rin->getExtent()->getUpperRightY() + rin->getExtent()->getLowerLeftY())/2;

// polygon 0
  {
    te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
    lr->setPoint(0, rin->getExtent()->getLowerLeftX(), rin->getExtent()->getLowerLeftY()); // lower left
    lr->setPoint(1, rin->getExtent()->getLowerLeftX(), yc); // upper left
    lr->setPoint(2, xc, yc); // upper rigth
    lr->setPoint(3, xc, rin->getExtent()->getLowerLeftY()); // lower rigth
    lr->setPoint(4, rin->getExtent()->getLowerLeftX(), rin->getExtent()->getLowerLeftY()); // closing

    te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
    pol->push_back(lr);

    pin.push_back(pol);
  }

// polygon 1
  {
    te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
    lr->setPoint(0, rin->getExtent()->getLowerLeftX(), yc); // lower left
    lr->setPoint(1, rin->getExtent()->getLowerLeftX(), rin->getExtent()->getUpperRightY()); // upper left
    lr->setPoint(2, xc, rin->getExtent()->getUpperRightY()); // upper rigth
    lr->setPoint(3, xc, yc); // lower rigth
    lr->setPoint(4, rin->getExtent()->getLowerLeftX(), yc); // closing

    te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
    pol->push_back(lr);

    pin.push_back(pol);
  }

// polygon 2
  {
    te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
    lr->setPoint(0, xc, yc); // lower left
    lr->setPoint(1, xc, rin->getExtent()->getUpperRightY()); // upper left
    lr->setPoint(2, rin->getExtent()->getUpperRightX(), rin->getExtent()->getUpperRightY()); // upper rigth
    lr->setPoint(3, rin->getExtent()->getUpperRightX(), yc); // lower rigth
    lr->setPoint(4, xc, yc); // closing

    te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
    pol->push_back(lr);

    pin.push_back(pol);
  }

// polygon 3
  {
    te::gm::LinearRing* lr = new te::gm::LinearRing(5, te::gm::LineStringType);
    lr->setPoint(0, xc, rin->getExtent()->getLowerLeftY()); // lower left
    lr->setPoint(1, xc, yc); // upper left
    lr->setPoint(2, rin->getExtent()->getUpperRightX(), yc); // upper rigth
    lr->setPoint(3, rin->getExtent()->getUpperRightX(), rin->getExtent()->getLowerLeftY()); // lower rigth
    lr->setPoint(4, xc, rin->getExtent()->getLowerLeftY()); // closing

    te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
    pol->push_back(lr);

    pin.push_back(pol);
  }

  return pin;
}

void DummyClassifier()
{
  try
  {
    std::cout << "Classification example using Dummy module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_dummy.tif";

// create 4 input polygons
    std::vector<te::gm::Polygon*> pin = CreatePolygons(rin);

// define classification parameters

// input parameters
    te::rp::Classifier::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back(0);
    algoInputParameters.m_inputRasterBands.push_back(1);
    algoInputParameters.m_inputRasterBands.push_back(2);
    algoInputParameters.m_inputPolygons = pin;

// link specific parameters with chosen implementation
    te::rp::ClassifierDummyStrategy::Parameters classifierparameters;
    classifierparameters.m_dummyParameter = 10;

    algoInputParameters.m_strategyName = "dummy";
    algoInputParameters.setClassifierStrategyParams(classifierparameters);

// output parameters
    te::rp::Classifier::OutputParameters algoOutputParameters;
    algoOutputParameters.m_rInfo = orinfo;
    algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;
    pin.clear();

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in DummyClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in DummyClassifier()!" << std::endl;
  }
}

// this code is for image segmentation to be used with ISOSegClassifier function
std::vector<te::gm::Polygon*> SegmentImage(te::rst::Raster* rin)
{
// define segmentation parameters

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_segmented.tif";

// input parameters
  te::rp::Segmenter::InputParameters algoInputParameters;
  algoInputParameters.m_inputRasterPtr = rin;
  algoInputParameters.m_inputRasterBands.push_back(0);
  algoInputParameters.m_inputRasterBands.push_back(1);
  algoInputParameters.m_inputRasterBands.push_back(2);

// link specific parameters with chosen implementation
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created;
// m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
  te::rp::SegmenterRegionGrowingStrategy::Parameters segparameters;
  segparameters.m_minSegmentSize = 50;
  segparameters.m_segmentsSimilarityThreshold = 0.15;
  segparameters.m_segmentFeatures = te::rp::SegmenterRegionGrowingStrategy::Parameters::MeanFeaturesType;

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
  algoOutputParameters.m_outputRasterPtr->vectorize( geometries, 0 );

  std::vector<te::gm::Polygon*> polygons;
  for (unsigned i = 0; i < geometries.size(); i++)
    polygons.push_back(static_cast<te::gm::Polygon*> (geometries[i]));

  return polygons;
}

void ISOSegClassifier()
{
  try
  {
    std::cout << "Classification example using ISOSeg algorithm." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_isoseg.tif";

// to apply ISOSeg the image must be segmented
    std::vector<te::gm::Polygon*> polygons = SegmentImage(rin);
    // std::vector<te::gm::Geometry*> geometries = CreatePolygons(rin);

    std::cout << "Segmentation created " << polygons.size() << " polygons" << std::endl;

// define classification parameters

// input parameters
    te::rp::Classifier::InputParameters algoInputParameters;
    algoInputParameters.m_inputRasterPtr = rin;
    algoInputParameters.m_inputRasterBands.push_back(0);
    algoInputParameters.m_inputRasterBands.push_back(1);
    algoInputParameters.m_inputRasterBands.push_back(2);
    algoInputParameters.m_inputPolygons = polygons;

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
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in ISOSegClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in ISOSegClassifier()!" << std::endl;
  }
}

void MAPClassifier()
{
  try
  {
    std::cout << "Classification example using MAP algorithm." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_MAP.tif";

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
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in MAPClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in MAPClassifier()!" << std::endl;
  }
}

void EMClassifier()
{
  try
  {
    std::cout << "Classification example using EM (Expectation-Maximization) algorithm." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_em.tif";

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
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in EMClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in EMClassifier()!" << std::endl;
  }
}

void SAMClassifier()
{
  try
  {
    std::cout << "Classification example using SAM algorithm." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_SAM.tif";

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
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in SAMClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in SAMClassifier()!" << std::endl;
  }
}

void KMeansClassifier()
{
  try
  {
    std::cout << "Classification example using KMeans algorithm." << std::endl << std::endl;

// first open the input image
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster info
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop_classified_kmeans.tif";

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
    classifierparameters.m_maxIterations = 10;
    classifierparameters.m_epsilon = 15.0;

    algoInputParameters.m_strategyName = "kmeans";
    algoInputParameters.setClassifierStrategyParams(classifierparameters);

// output parameters
    te::rp::Classifier::OutputParameters algoOutputParameters;
    algoOutputParameters.m_rInfo = orinfo;
    algoOutputParameters.m_rType = "GDAL";

// execute the algorithm
    te::rp::Classifier classifierinstance;

    if(!classifierinstance.initialize(algoInputParameters))
      throw;
    if(!classifierinstance.execute(algoOutputParameters))
      throw;

// clean up
    delete rin;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in KMeansClassifier(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in KMeansClassifier()!" << std::endl;
  }
}

void Classifier()
{
  SAMClassifier();
  DummyClassifier();
  ISOSegClassifier();
  MAPClassifier();
  EMClassifier();
  KMeansClassifier();
}
