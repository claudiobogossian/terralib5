// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>

te::da::DataSetType* CreateDataSetType(te::da::DataSourceTransactor* transactor)
{
  if(transactor == 0)
    throw te::common::Exception("You informed a NULL transactor!");

// check if dataset with the same name already exists
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

  if(cloader->datasetExists("public.our_country"))
  {
    delete cloader;
    throw te::common::Exception("The table public.our_country already exists!");
  }

  delete cloader;

// mount one in memory...
  te::da::DataSetType* datasetType = new te::da::DataSetType("public.our_country");

  datasetType->add(new te::dt::SimpleProperty("gid", te::dt::INT32_TYPE, true));
  datasetType->add(new te::dt::StringProperty("country_name", te::dt::STRING));
  datasetType->add(new te::dt::StringProperty("city_name", te::dt::VAR_STRING, 50, true));
  datasetType->add(new te::gm::GeometryProperty("spatial_data", 4326, te::gm::GeometryType, true));

// ... and then call persistence API
  te::da::DataSetTypePersistence* dp = transactor->getDataSetTypePersistence();
  
  dp->create(datasetType);

// remember to release the persistence
  delete dp;

  return datasetType;
}

