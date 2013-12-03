// Examples
#include "DataAccessExamples.h"

te::dt::SimpleProperty* AddProperty(const std::string& datasetname,
                                    te::da::DataSourceTransactor* transactor)
{
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("population", te::dt::INT32_TYPE, true);

  transactor->addProperty(datasetname, p);

  return p;
}

