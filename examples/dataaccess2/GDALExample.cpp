#include "DataAccessExamples.h"

#include <terralib/gdal2/DataSource.h>
#include <terralib/dataaccess2/dataset/DataSet.h>
#include <terralib/dataaccess2/datasource/DataSourceFactory.h>
#include <terralib/dataaccess2/datasource/DataSourceTransactor.h>
#include <terralib/dataaccess2/utils/Utils.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

#include <boost/ptr_container/ptr_vector.hpp>

#include <iostream>
#include <memory>

void DataSourceStatic()
{
  std::string dsType = "GDAL";
  
  std::map<std::string, std::string> info;
  info["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif" ;
  if (te::da::DataSource::exists(dsType, info))
    std::cout << "Data source cbers2b_rgb342_crop.tifd exists.\n";
  else
    std::cout << "Data source cbers2b_rgb342_crop.tif does not exist.\n";
  
  std::vector<std::string> dataSourceNames = te::da::DataSource::getDataSourceNames(dsType, info);
  std::cout << "=== Data source names (URI): \n";
  for(std::size_t i = 0; i < dataSourceNames.size(); ++i)
    std::cout << dataSourceNames[i] << std::endl;
  std::cout << std::endl;
  
  std::vector<std::string> enc = te::da::DataSource::getEncodings(dsType, info);
  std::cout << "=== Data source encodings (URI): \n";
  for(std::size_t i = 0; i < enc.size(); ++i)
    std::cout << enc[i] << std::endl;
  std::cout << std::endl;  
  
  info.clear();
  info["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters" ;
  dataSourceNames = te::da::DataSource::getDataSourceNames(dsType, info);
  std::cout << "=== Data source names(SOURCE): \n";
  for(std::size_t i = 0; i < dataSourceNames.size(); ++i)
    std::cout << dataSourceNames[i] << std::endl;
  std::cout << std::endl;
  
  info.clear();
  info["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters_new" ;
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSource::create(dsType, info);
  if (te::da::DataSource::exists(dsType, info))
    std::cout << "Data source rasters_new was created and exists.\n";
  else
    std::cout << "Data source rasters_new does not exist.\n";
  ds.reset();
  
  te::da::DataSource::drop(dsType, info);
  if (te::da::DataSource::exists(dsType, info))
    std::cout << "Data source rasters_new still exists.\n";
  else
    std::cout << "Data source rasters_new was removed and does not exist.\n";
  ds.reset();  

  
  info.clear();
  info["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/testetif.tif" ;
  try 
  {
    ds = te::da::DataSource::create(dsType, info);
  }
  catch(te::common::Exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }
}

// This is an example, not a test! We expect everything runs ok and 
// no exception is thrown or any other fatal fail happens
void DataSourceFile()
{
  std::string dsType = "GDAL";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
  
  std::map<std::string, std::string> info;
  info["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif" ;
  ds->setConnectionInfo(info);
  ds->open();
  
  std::cout << std::endl << "Data source type: " << ds->getType() << std::endl;

  std::cout << std::endl << "Data source is opened: ";
  if (ds->isOpened())
    std::cout << " true." << std::endl;
  else 
    std::cout << " false." << std::endl;
  
  std::cout << std::endl << "Data source is valid: ";
  if (ds->isValid())
    std::cout << " true." << std::endl;
  else 
    std::cout << " false." << std::endl;  
  
  std::cout << std::endl << "Data source has datasets: ";
  if (ds->hasDataSets())
    std::cout << " yes." << std::endl;
  else 
    std::cout << " no." << std::endl;
    
  const std::map<std::string, std::string>& info2 = ds->getConnectionInfo();
  std::cout << std::endl << "Data source connection info: " << std::endl;
  for (std::map<std::string, std::string>::const_iterator it=info2.begin();
       it!=info2.end();
       ++it) 
    std::cout << "\t" << it->first << ": " << it->second << std::endl;
  
  std::vector<std::string> dsnames = ds->getDataSetNames();
  std::cout << std::endl <<  "Data source has " << dsnames.size() << " dataset(s):" << std::endl;
  for (size_t i=0; i< dsnames.size(); ++i)
    std::cout << "\t" << dsnames[i];
  std::cout << std::endl;
  
  if (ds->getNumberOfDataSets() != dsnames.size())
    std::cout << std::endl << "Number of datasets is not consistent with the number of dataset names." << std::endl;
  else     
    std::cout << std::endl << "Number of datasets is consistent with the number of dataset names." << std::endl;
  
  //boost::shared_ptr<DataSet>
  std::auto_ptr<te::da::DataSetType> dsptr = ds->getDataSetType("cbers2b_rgb342_crop.tif");
  if (!dsptr.get())
    std::cout << std::endl << "Coudn't retrieve the dataset type for dataset cbers2b_rgb342_crop.tif." << std::endl;
  else 
  {
    std::cout << std::endl << "Dataset type: " << dsptr->getTitle() << ", ";
    std::cout << dsptr->getType() << ", ";
    std::cout << dsptr->getId()   << std::endl;
  }
  
  std::cout << std::endl << "Dataset properties: " << std::endl;
  boost::ptr_vector<te::dt::Property> vprop = ds->getProperties("cbers2b_rgb342_crop.tif");
  for (size_t i=0; i< vprop.size(); ++i)
    std::cout << "\tProp: " << vprop[i].getName();
  std::cout << std::endl;  
    
  if (ds->getNumberOfProperties("cbers2b_rgb342_crop.tif") != vprop.size())
    std::cout << std::endl << "Number of properties in dataset is not consistent." << std::endl;
  else     
    std::cout << std::endl << "Number of properties in dataset is consistent." << std::endl;
  
  if (!ds->propertyExists("cbers2b_rgb342_crop.tif",vprop[0].getName()))
    std::cout << std::endl << "Property is not consistent." << std::endl;
  else     
    std::cout << std::endl << "Property is consistent." << std::endl;
  
  std::auto_ptr<te::dt::Property> ppt = ds->getProperty("cbers2b_rgb342_crop.tif", vprop[0].getName());
  if (ppt->getName() != vprop[0].getName())
    std::cout << std::endl << "Get property is not consistent." << std::endl;
  else     
    std::cout << std::endl << "Get property is consistent." << std::endl;  

  
  double val;
  std::auto_ptr<te::da::DataSet> ds1 = ds->getDataSet("cbers2b_rgb342_crop.tif");
  std::auto_ptr<te::rst::Raster> rst(ds1->getRaster(0));
  std::cout <<  "1) Rows x Columns: " << rst->getGrid()->getNumberOfRows() << " X " << rst->getGrid()->getNumberOfColumns() << std::endl;
  rst->getValue(10,10, val);
  std::cout << val << std::endl;
  
  std::auto_ptr<te::da::DataSet> ds2 = ds->getDataSet("cbers2b_rgb342_crop.tif");
  rst.release();
  
  std::auto_ptr<te::rst::Raster> rst2(ds2->getRaster(0));
  std::cout <<  "2) Rows x Columns: " << rst2->getGrid()->getNumberOfRows() << " X " << rst2->getGrid()->getNumberOfColumns() << std::endl;
  rst2->getValue(50,50, val);
  std::cout << val << std::endl;
  rst2.release();
  
  ds->close();
}


void DataSourceDirectory()
{
  std::string dsType = "GDAL";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
  
  std::map<std::string, std::string> info;
  info["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters" ;
  ds->setConnectionInfo(info);
  ds->open();
  
  std::vector<std::string> dsnames = ds->getDataSetNames();
  std::cout << std::endl << "Datasets in the datasource: ";
  for (size_t i=0; i<dsnames.size(); ++i) 
  {
    std::cout << "\t" << dsnames[i] << std::endl;
    std::vector<std::string> pn = ds->getPropertyNames(dsnames[i]);
    std::cout << "\tProperties:"  << std::endl;
    for (size_t j=0; j<pn.size(); ++j)
      std::cout << "\t\t" << j << ": "<< pn[j] << std::endl;
    std::cout << std::endl;
  }

  te::rst::RasterProperty* rp = new te::rst::RasterProperty("raster");
  rp->set(new te::rst::Grid(static_cast<unsigned int>(100),static_cast<unsigned int>(100),new te::gm::Envelope(0,0,100,100)));  
  rp->add(new te::rst::BandProperty(0,te::dt::UCHAR_TYPE, "banda1"));
  
  te::da::DataSetType* dt = new te::da::DataSetType("teste2.tif");
  dt->add(rp);
  
  std::map<std::string, std::string> nopt;
  
  ds->createDataSet(dt,nopt);
  
  size_t nds = ds->getNumberOfDataSets();
  if (nds > dsnames.size())
    std::cout << std::endl << "createDataSet is consistent." << std::endl;
  else 
    std::cout << std::endl << "createDataSet is not consistent." << std::endl;
  
  ds->renameDataSet("teste2.tif", "teste3.tif");
  
  ds->cloneDataSet("teste.tif","teste4.tif",nopt);
  
  dsnames.clear();

  dsnames = ds->getDataSetNames();
  std::cout << std::endl << "Datasets in the datasource post modifications: ";
  for (size_t i=0; i<dsnames.size(); ++i) 
  {
    std::cout << "\t" << dsnames[i] << std::endl;
    std::vector<std::string> pn = ds->getPropertyNames(dsnames[i]);
    std::cout << "\tProperties:"  << std::endl;
    for (size_t j=0; j<pn.size(); ++j)
      std::cout << "\t\t" << j << ": "<< pn[j] << std::endl;
    std::cout << std::endl;
  }
  
  ds->dropDataSet("teste4.tif");
  nds = ds->getNumberOfDataSets();
  if (nds < dsnames.size())
    std::cout << std::endl << "dropDataSet is consistent." << std::endl;
  else 
    std::cout << std::endl << "dropDataSet is not consistent." << std::endl;  
  
  ds->close();
}

void DataSourceTransactor()
{
  std::string dsType = "GDAL";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
  
  std::map<std::string, std::string> info;
  info["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters" ;
  ds->setConnectionInfo(info);
  ds->open();
  
  std::auto_ptr<te::da::DataSourceTransactor> t = ds->getTransactor();
  std::vector<std::string> dsnames = t->getDataSetNames();
  std::cout << std::endl << "Datasets in the datasource: ";
  for (size_t i=0; i<dsnames.size(); ++i) 
  {
    std::cout << "\t" << dsnames[i] << std::endl;
    std::vector<std::string> pn = t->getPropertyNames(dsnames[i]);
    std::cout << "\tProperties:"  << std::endl;
    for (size_t j=0; j<pn.size(); ++j)
      std::cout << "\t\t" << j << ": "<< pn[j] << std::endl;
    std::cout << std::endl;
  }
  
  ds->close();
}

void DataSourceSubDataSets()
{
  std::string dsType = "GDAL";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
  
  std::map<std::string, std::string> info;
  info["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/modis/MOD09A1.A2013001.h09v07.005.2013017013159.hdf" ;
  ds->setConnectionInfo(info);
  ds->open();
  
  std::vector<std::string> dsnames = ds->getDataSetNames();
  std::cout << std::endl << "Datasets in the datasource: ";
  for (size_t i=0; i<dsnames.size(); ++i) 
  {
    std::cout << "\t" << dsnames[i] << std::endl;
    std::vector<std::string> pn = ds->getPropertyNames(dsnames[i]);
    std::cout << "\tProperties:"  << std::endl;
    for (size_t j=0; j<pn.size(); ++j)
      std::cout << "\t\t" << j << ": "<< pn[j] << std::endl;
    std::cout << std::endl;
  }

  std::auto_ptr<te::da::DataSet> ds1 = ds->getDataSet(dsnames[0]);
  std::auto_ptr<te::rst::Raster> rst(ds1->getRaster(0));
  std::cout << std::endl << "Rows x Columns: " << rst->getGrid()->getNumberOfRows() << " X " << rst->getGrid()->getNumberOfColumns() << std::endl;
  std::cout << std::endl << "Number of bands: " << rst->getNumberOfBands() << std::endl;
  double val;
  for (size_t i=0; i<rst->getNumberOfBands(); ++i)
  {
    rst->getValue(10,10,val,i);
    std::cout << val << std::endl;
  }
  ds->close();
}


void DataSourceQuery()
{
  std::string dsType = "GDAL";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(dsType));
  
  std::map<std::string, std::string> info;
  info["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif" ;
  ds->setConnectionInfo(info);
  ds->open();
  
  std::auto_ptr<te::da::Select> sel = te::da::BuildSelect("cbers2b_rgb342_crop.tif");
  std::auto_ptr<te::da::DataSet> dataset = ds->query(*sel);
  
  std::auto_ptr<te::rst::Raster> rst(dataset->getRaster(0));
  std::cout << std::endl << "Rows x Columns: " << rst->getGrid()->getNumberOfRows() << " X " << rst->getGrid()->getNumberOfColumns() << std::endl;
  std::cout << std::endl << "Number of bands: " << rst->getNumberOfBands() << std::endl;

  ds->close();
}

void GDALExample()
{

//  DataSourceStatic();
//  DataSourceFile();
//  DataSourceDirectory();
//  DataSourceTransactor();
//  DataSourceSubDataSets();
  DataSourceQuery();
}

