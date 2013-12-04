// Examples
#include "DataAccessExamples.h"

te::da::Index* AddSpatialIndex(const std::string& datasetname,
                                           te::da::DataSourceTransactor* transactor)
{
  te::da::Index* spidx = new te::da::Index("our_country_spatial_data_spidx", te::da::R_TREE_TYPE);
  
  //std::auto_ptr<te::dt::Property>  sp = transactor->getProperty(datasetname,"spatial_data");
  //spidx->add(sp.get());

  spidx->add((transactor->getProperty(datasetname,"spatial_data")).release());
  std::map<std::string,std::string> opt;

  transactor->addIndex(datasetname,spidx,opt);

  return spidx;
}

