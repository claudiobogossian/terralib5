// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

te::da::UniqueKey* AddUniqueKey(te::da::DataSetType* datasetType,
                                           te::da::DataSourceTransactor* transactor)
{
  te::da::UniqueKey* uk = new te::da::UniqueKey("country_name_city_name_uk");
  uk->add(datasetType->getProperty("country_name"));
  uk->add(datasetType->getProperty("city_name"));

  te::da::DataSetTypePersistence* dp = transactor->getDataSetTypePersistence();
  
  dp->add(datasetType, uk);
  
  delete dp;

  return uk;
}

