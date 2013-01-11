// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/dataset/DataSetAdapter.h>
#include <terralib/datatype.h>

// STL
#include <iostream>
#include <map>
#include <string>
#include <vector>

te::dt::StringProperty* CreateStringProperty(const std::string& name)
{
  return new te::dt::StringProperty(name, te::dt::STRING);
}

void DataSetAdapter()
{
  /* Accessing a shapefile */
  std::string ogrConnInfo("connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp/munic_2001.shp");

  te::da::DataSource* ogrDataSource = te::da::DataSourceFactory::make("OGR");
  ogrDataSource->setConnectionStr(ogrConnInfo);
  ogrDataSource->open();

  te::da::DataSourceTransactor* ogrTransactor = ogrDataSource->getTransactor();

  te::da::DataSet* ogrDataSet = ogrTransactor->getDataSet("munic_2001");

  /* PostGIS database */
  te::da::DataSource* pgisDataSource = te::da::DataSourceFactory::make("POSTGIS");

  bool needAdapter = te::da::DataSetAdapter::needAdapter(ogrDataSet, pgisDataSource->getCapabilities());
  std::cout << "Need  adapter? ";
  needAdapter ? std::cout << "Yes" << std::endl : std::cout << "No" << std::endl; // Here, result is "No".

  // Creates the DataSet Adapter
  //te::da::DataSetAdapter* adaptedDataSet = te::da::DataSetAdapter::adapt(ogrDataSet);
  te::da::DataSetAdapter* adaptedDataSet = te::da::DataSetAdapter::adapt(ogrDataSet, pgisDataSource->getCapabilities());

  // Adapts all properties to String
  /*te::da::DataSetType* ogrDataSetType = ogrDataSet->getType();
  const std::vector<te::dt::Property*> properties = ogrDataSetType->getProperties();
  for(std::size_t i = 0; i < properties.size() - 1; ++i)
    adaptedDataSet->adapt(i, CreateStringProperty("Property" + te::common::Convert2String(i)));*/

  // Adapts the "geom" property to String
  adaptedDataSet->adapt("geom", CreateStringProperty("GEOMETRY_ADAPTED_PROPERTY"));

  bool isValid = adaptedDataSet->isValid();
  std::cout << "DataSet Adapter is valid? "; 
  isValid ? std::cout << "Yes" << std::endl : std::cout << "No" << std::endl;
  if(!isValid)
  {
    std::vector<std::string> nonAdaptedProperties;
    adaptedDataSet->getNonAdaptedProperties(nonAdaptedProperties);
    std::cout << ":: No Adapted Properties ::" << std::endl;
    for(std::size_t i = 0; i < nonAdaptedProperties.size(); ++i)
      std::cout << "Property: " << nonAdaptedProperties[i] << std::endl;
  }
  else
    PrintDataSet(adaptedDataSet);

  // Cleaning all!
  delete adaptedDataSet;
  delete pgisDataSource;
  delete ogrDataSet;
  delete ogrTransactor;
  delete ogrDataSource;
}
