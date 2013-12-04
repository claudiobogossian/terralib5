// Examples
#include "DataAccessExamples.h"

te::da::UniqueKey* AddUniqueKey(const std::string& datasetname,
                                           te::da::DataSourceTransactor* transactor)
{
  te::da::UniqueKey* uk = new te::da::UniqueKey("country_name_city_name_uk");
  std::auto_ptr<te::dt::Property>  uk1 = transactor->getProperty(datasetname,"country_name");
  std::auto_ptr<te::dt::Property>  uk2 = transactor->getProperty(datasetname,"city_name");

  uk->add(uk1.get());
  uk->add(uk2.get());

  transactor->addUniqueKey(datasetname,uk); 

  return uk;
}

