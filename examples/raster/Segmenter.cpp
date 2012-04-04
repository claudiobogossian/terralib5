#include "RasterExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/dataset/DataSetPersistence.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>
#include <terralib/rp/Segmenter.h>

// STL
#include <string>
#include <map>

void Segmenter()
{
  try
  {
    std::cout << "Segmentation example using Raster Processing module." << std::endl << std::endl;

// open input raster
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif";

    te::rst::Raster* rin = te::rst::RasterFactory::open(rinfo);

// create output raster
    std::map<std::string, std::string> orinfo;
    orinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_segmented.tif";

    te::rst::Grid* ogrid = new te::rst::Grid(*rin->getGrid());

    std::vector<te::rst::BandProperty*> obands;
    obands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "segmented band"));

    te::rst::Raster* rout = te::rst::RasterFactory::make(ogrid, obands, orinfo);

// define segmentation parameters
// strategy specific parameters (m_minSegmentSize: size of the smallest segment to be created; m_segmentsSimilarityThreshold: similarity between neighboring segments to merge them or not)
    te::rp::SegmenterRegionGrowingStrategy::Parameters segparameters;
    segparameters.m_minSegmentSize = 50;
    segparameters.m_segmentsSimilarityThreshold = 30;

// inputs/outputs
    te::rp::Segmenter::Parameters algoparameters;
    algoparameters.m_inRasterPtr = rin;
    algoparameters.m_inRasterBands.push_back( 0 );
    algoparameters.m_inRasterBands.push_back( 1 );
    algoparameters.m_inRasterBands.push_back( 2 );
// the output can be a previously created raster (in this case, rout)
    algoparameters.m_outRasterPtr = rout;
    algoparameters.m_outRasterBand = 0;
// or a previously opened data source (in this case, 0)
    algoparameters.m_outDataSourcePtr = 0;
    algoparameters.m_outDataSetName = "";

// parameters to use threaded processing
    algoparameters.m_enableThreadedProcessing = false;
    algoparameters.m_maxSegThreads = 0;
// divide the image into blocks for segmentation
    algoparameters.m_enableBlockProcessing = false;
    algoparameters.m_enableBlockMerging = false;
    algoparameters.m_maxBlockSize = 0;

// link specific parameters with chosen implementation
    algoparameters.m_strategyName = "RegionGrowing";
    algoparameters.setSegStrategyParams(segparameters);

// execute the algorithm
    te::rp::Segmenter seginstance;

    seginstance.initialize(algoparameters);
    seginstance.execute();

// export the segmentation into shapefile
    std::vector<te::gm::Geometry*> geometries;
    te::gdal::Vectorize(((te::gdal::Raster*) rout)->getGDALDataset()->GetRasterBand(1), geometries);

    te::da::DataSetType* dst = new te::da::DataSetType("objects from segmentation");

    te::dt::SimpleProperty* id_prop = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE, true);
    id_prop->setAutoNumber(true);

    te::gm::GeometryProperty* obj_prop = new te::gm::GeometryProperty("objects", 0, te::gm::PolygonType, true);
    obj_prop->setSRID(rin->getSRID());

    dst->add(id_prop);
    dst->add(obj_prop);
    dst->setDefaultGeomProperty(obj_prop);

    //define a primary key
    te::da::PrimaryKey* pkey = new te::da::PrimaryKey("primary key", dst);
    pkey->add(id_prop);

    te::mem::DataSet* dset = new te::mem::DataSet(dst);
    for (unsigned i = 0; i < geometries.size(); i++)
    {
      te::da::DataSetItem* dsitem = new te::mem::DataSetItem(dst);
      dsitem->setGeometry("objects", *geometries[i]);
      dset->add(dsitem);
    }

    std::map<std::string, std::string> osinfo;
    osinfo["connection_string"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/cbers2b_rgb342_crop_segmented.shp";
    te::da::DataSource* ds = te::da::DataSource::create("OGR", osinfo);
    te::da::DataSourceTransactor* trans = ds->getTransactor();
    te::da::DataSetPersistence* pers = trans->getDataSetPersistence();
    pers->create(dst, dset);

    delete pers;
    delete trans;
    delete ds;

// clean up
    delete rin;
    delete rout;

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Segmenter(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Segmenter()!" << std::endl;
  }
}

