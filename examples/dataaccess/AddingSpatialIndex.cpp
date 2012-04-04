// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

te::da::Index* AddSpatialIndex(te::da::DataSetType* datasetType,
                               te::da::DataSourceTransactor* transactor)
{
  te::da::Index* spidx = new te::da::Index("our_country_spatial_data_spidx", te::da::R_TREE_TYPE);

  spidx->add(datasetType->getProperty("spatial_data"));

  te::da::DataSetTypePersistence* dp = transactor->getDataSetTypePersistence();

  dp->add(datasetType, spidx);

  delete dp;

  return spidx;
}

