// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/datatype.h>

te::da::DataSetType* CreateDataSetType(te::da::DataSourceTransactor* transactor)
{
  if(transactor == 0)
    throw te::common::Exception("You informed a NULL transactor!");

// check if dataset with the same name already exists
  
  if(transactor->dataSetExists("public.our_country1"))
  {
    throw te::common::Exception("The table public.our_country1 already exists!");
  }

// mount one in memory...
  te::da::DataSetType* datasetType = new te::da::DataSetType("public.our_country1");

  datasetType->add(new te::dt::SimpleProperty("gid", te::dt::INT32_TYPE, true));
  datasetType->add(new te::dt::StringProperty("country_name", te::dt::STRING));
  datasetType->add(new te::dt::StringProperty("city_name", te::dt::VAR_STRING, 50, true));
  datasetType->add(new te::gm::GeometryProperty("spatial_data", 4326, te::gm::GeometryType, true));

// ... and then call persistence API
  std::map<std::string, std::string> options; //API exije esse param
  transactor->createDataSet(datasetType,options);

  return datasetType;
}
