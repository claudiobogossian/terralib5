// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

te::da::PrimaryKey* AddPrimaryKey(te::da::DataSetType* datasetType,
                                             te::da::DataSourceTransactor* transactor)
{
  te::da::PrimaryKey* pk = new te::da::PrimaryKey("attribute6_int_pk");
  
  pk->add(datasetType->getProperty("gid"));

  te::da::DataSetTypePersistence* dp = transactor->getDataSetTypePersistence();
  
  dp->add(datasetType, pk);
  
  delete dp;

  return pk;
}


