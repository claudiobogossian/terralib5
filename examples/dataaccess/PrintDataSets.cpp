// Examples
#include "DataAccessExamples.h"

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
  te::da::DataSourceTransactor* transactor = (ds->getTransactor()).release();
  //assert(transactor.get());
// from transactor, take a catalog loader to find out the datasets stored in the data source
  //te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

// now retrieve the name of the datasets
  std::vector<std::string> datasets = ds->getDataSetNames();

// and then iterate over dataset names to retrieve its information
  std::cout << "Printing all the data in datasets... number of datasets: " << datasets.size() << std::endl;

  for(unsigned int i=0; i<datasets.size(); ++i)
  //for(unsigned int i=0; i<1; ++i)
  {

// retrieve the dataset by its name
    te::da::DataSet* dataset = (ds->getDataSet(datasets[i])).release();

// print its date to standard output
    PrintDataSet(datasets[i], dataset);

// release the dataset: you are the owner
    delete dataset;
  }

// release the catalog loader: you are the owner
  //delete cloader;

// release the transactor: you are the owner
  delete transactor;
}

