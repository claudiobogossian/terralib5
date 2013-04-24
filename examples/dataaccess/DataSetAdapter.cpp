// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>

// STL
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

te::dt::Property* CreateStringProperty(const std::string& name)
{
  return new te::dt::StringProperty(name, te::dt::STRING);
}

te::dt::Property* CreateSimpleProperty(const std::string& name, int type)
{
  return new te::dt::SimpleProperty(name, type);
}

te::dt::Property* CreateGeometryProperty(const std::string& name)
{
  return new te::gm::GeometryProperty(name);
}

te::dt::AbstractData* TupleToStringUpperCaseConverter(te::da::DataSet* dataset, const std::vector<int>& indexes, int /*dstType*/)
{
  assert(dataset);

  std::string result;

  std::vector<int>::const_iterator it;
  for(it = indexes.begin(); it != indexes.end(); ++it)
    result += te::common::Convert2UCase(dataset->getAsString(*it)) + ";";

  result.erase(--result.end());

  return new te::dt::String(result);
}

void DataSetAdapter()
{
  ///* Accessing a shapefile */
  //std::string ogrConnInfo("connection_string="TE_DATA_EXAMPLE_DIR"/data/shp/munic_2001.shp");

  //te::da::DataSource* ogrDataSource = te::da::DataSourceFactory::make("OGR");
  //ogrDataSource->setConnectionStr(ogrConnInfo);
  //ogrDataSource->open();

  //te::da::DataSourceTransactor* ogrTransactor = ogrDataSource->getTransactor();

  //te::da::DataSet* ogrDataSet = ogrTransactor->getDataSet("munic_2001");

  ///* PostGIS database to gets the DataSource Capabilities */
  //te::da::DataSource* pgisDataSource = te::da::DataSourceFactory::make("POSTGIS");

  //// Verifies if ogr data set -> pgis data source need an adapter
  //bool needAdapter = te::da::DataSetAdapter::needAdapter(ogrDataSet, pgisDataSource->getCapabilities());
  //std::cout << "Need  adapter? ";
  //needAdapter ? std::cout << "Yes" << std::endl : std::cout << "No" << std::endl; // Here, result is "No".

  //// No need! But, let's create an adapter

  //// Creates the DataSet Adapter
  //te::da::DataSetAdapter* adaptedDataSet = te::da::DataSetAdapter::adapt(ogrDataSet);
  ////te::da::DataSetAdapter* adaptedDataSet = te::da::DataSetAdapter::adapt(ogrDataSet, pgisDataSource->getCapabilities()); // Here, try automatically create an adapter using DataSourceCapabilities 

  //// Prints the non adapted properties
  //std::vector<std::string> nonAdaptedProperties;
  //adaptedDataSet->getNonAdaptedProperties(nonAdaptedProperties);
  //std::cout << ":: No Adapted Properties ::" << std::endl;
  //for(std::size_t i = 0; i < nonAdaptedProperties.size(); ++i)
  //  std::cout << "Property: " << nonAdaptedProperties[i] << std::endl;

  ///* Adapting... */

  //// Adapts 3 attributes to String using a function that generates a pre-defined pattern. e.g. "[ 2148 Seritinga 178 ]"
  //std::vector<std::string> idAttributes;
  //idAttributes.push_back("GEOCODIGO");
  //idAttributes.push_back("NOME");
  //idAttributes.push_back("AREA_TOT_G");
  //adaptedDataSet->adapt(idAttributes, CreateStringProperty("GEOCODIGO_NOME_AREA_ADAPTED_TO_STRING"), te::da::TupleToStringConverter);

  //// Adapts the same 3 attributes to String using other function defined in this example! "TupleToStringUpperCaseConverter"
  //// The function generates a pre-defined pattern (using ';' as delimiter)  and the attribute values are converted to String Upper Case! e.g. "2148;SERITINGA;178"
  //adaptedDataSet->adapt(idAttributes, CreateStringProperty("GEOCODIGO_NOME_AREA_ADAPTED_TO_STRING_2"), TupleToStringUpperCaseConverter);

  //// Adapts lat/lon to Geometry Point
  //std::vector<std::string> latLonAttributes;
  //latLonAttributes.push_back("LONGITUDES");
  //latLonAttributes.push_back("LATITUDESE");
  //adaptedDataSet->adapt(latLonAttributes, CreateGeometryProperty("LAT_LONG_ADAPTED_TO_POINT"), te::da::XYToPointConverter);

  //// Adapts the "geom" property to String
  //adaptedDataSet->adapt("geom", CreateStringProperty("GEOM_ADAPTED_TO_STRING"));

  //// Prints the adapted data set
  //PrintDataSet(adaptedDataSet);

  //// Here, we will adapt the previous adapted data set again!
  //te::da::DataSetAdapter* adaptedDataSetAgain = te::da::DataSetAdapter::adapt(adaptedDataSet);

  //// Adapts the LAT_LONG_ADAPTED_TO_POINT property to 2 properties called "X" and "Y". Note the use of attribute converter functions: PointToXConverter and PointToYConverter.
  //std::vector<std::string> geomAttribute;
  //adaptedDataSetAgain->adapt("LAT_LONG_ADAPTED_TO_POINT", CreateSimpleProperty("X", te::dt::DOUBLE_TYPE), te::da::PointToXConverter);
  //adaptedDataSetAgain->adapt("LAT_LONG_ADAPTED_TO_POINT", CreateSimpleProperty("Y", te::dt::DOUBLE_TYPE), te::da::PointToYConverter);

  //// Prints the adapted data set
  //PrintDataSet(adaptedDataSetAgain);

  //// Cleaning all!
  //delete adaptedDataSetAgain;
  //delete adaptedDataSet;
  //delete pgisDataSource;
  //delete ogrDataSet;
  //delete ogrTransactor;
  //delete ogrDataSource;
}
