// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void PrintCatalog(te::da::DataSource* ds)
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
  std::cout << "Printing information about datasets...\n\n Number of datasets: " << datasets.size() << std::endl;

  for(std::vector<std::string*>::const_iterator it = datasets.begin(); it < datasets.end(); ++it)
  {
    const std::string* datasetName = *it;

// get dataset information: the parameter true indicates that we want all information about the dataset
//                          you can set it to false and see the difference (no primary key and other stuffs are retrieved)
    te::da::DataSetType* dt = cloader->getDataSetType(*datasetName, true);

    std::cout << "DataSet: " << dt->getName() << std::endl;
    std::cout << "\t" << "Number of attributes:" << dt->size() << std::endl;
    std::cout << "\t" << "Has PK:" << (dt->getPrimaryKey() != 0) << std::endl;
    std::cout << "\t" << "Number of UKs:" << dt->getNumberOfUniqueKeys() << std::endl;
    std::cout << "\t" << "Number of Indexes:" << dt->getNumberOfIndexes() << std::endl;
    std::cout << "\t" << "Number of Check-Constraints:" << dt->getNumberOfCheckConstraints() << std::endl;
    std::cout << std::endl;

    for(unsigned int i=0; i<dt->size(); ++i)
    {
      te::dt::Property* prop = dt->getProperty(i);
      std::cout << "Name: " << prop->getName() << std::endl;
      std::cout << "Type: " << prop->getType() << std::endl;      
    }

// release the dataset type: you are the owner!
    delete dt;
  }

// don't forget to release the vector of dataset names... they are pointers to strings
  te::common::FreeContents(datasets);

// release the catalog loader: you are the owner
  delete cloader;

// release the transactor: you are the owner
  delete transactor;
}

