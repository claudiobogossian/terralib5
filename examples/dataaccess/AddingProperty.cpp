// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>

te::dt::SimpleProperty* AddProperty(te::da::DataSetType* datasetType,
                                    te::da::DataSourceTransactor* transactor)
{
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("population", te::dt::INT32_TYPE, true);

  te::da::DataSetTypePersistence* dp = transactor->getDataSetTypePersistence();
  
  dp->add(datasetType, p);

  return p;
}

