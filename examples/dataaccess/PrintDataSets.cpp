// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void PrintDataSets(te::da::DataSource* ds)
{
  if(ds == 0 || !ds->isOpened())
  {
    std::cout << "The informed data source is NULL or is closed!" << std::endl;
    return;
  }

// get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

// from transactor, take a catalog loader to find out the datasets stored in the data source
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

// now retrieve the name of the datasets
  std::vector<std::string*> datasets;

  cloader->getDataSets(datasets);

// and then iterate over dataset names to retrieve its information
  std::cout << "Printing all the data in datasets... number of datasets: " << datasets.size() << std::endl;

  for(std::vector<std::string*>::const_iterator it = datasets.begin(); it < datasets.end(); ++it)
  {
    const std::string* datasetName = *it;

// retrieve the dataset by its name
    te::da::DataSet* dataset = transactor->getDataSet(*datasetName);

// print its date to standard output
    PrintDataSet(dataset);

// release the dataset: you are the owner
    delete dataset;
  }

// don't forget to release the vector of dataset names... they are pointers to strings
  te::common::FreeContents(datasets);

// release the catalog loader: you are the owner
  delete cloader;

// release the transactor: you are the owner
  delete transactor;
}

