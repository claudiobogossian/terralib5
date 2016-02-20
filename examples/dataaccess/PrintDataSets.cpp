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

// now retrieve the name of the datasets
  std::vector<std::string> datasets = ds->getDataSetNames();

// and then iterate over dataset names to retrieve its information
  std::cout << "There is(are) "<< datasets.size() << " dataset(s) in the datasource:" << std::endl;

  for(unsigned int i=0; i<datasets.size(); ++i)
  {
    try {
// retrieve the dataset by its name
    te::da::DataSet* dataset = (ds->getDataSet(datasets[i])).release();

// print its date to standard output
    std::cout << i << ": ";
    PrintDataSet(datasets[i], dataset);

// release the dataset: you are the owner
    delete dataset;
    }
    catch(te::common::Exception& ex)
    {
      std::cout << "\n\tCan not print dataset " << datasets[i] << ": " << ex.what() << std::endl;
    }
    catch(...)
    {
      std::cout << "\n\tCan not print dataset " << datasets[i] << std::endl;
    }
  }
// release the transactor: you are the owner
  delete transactor;
}

