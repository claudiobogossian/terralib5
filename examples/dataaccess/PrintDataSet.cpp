// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>

void PrintDataSet(std::string datasetName, te::da::DataSet* dataset)
{
  if(dataset == 0)
  {
    std::cout << "The informed dataset is NULL!" << std::endl;
    return;
  }

// this will be used just to count the items in the dataset
  int item = 0;

// traverse the dataset and print each dataset item
  dataset->moveBeforeFirst();
  std::cout << "DATASET: " << datasetName;
  while(dataset->moveNext())
  {
    std::cout << std::endl << "ITEM NUMBER: " << item++ << " =======================" << std::endl;
  
    for(std::size_t i = 0; i < dataset->getNumProperties(); ++i)
    {
      std::cout << dataset->getPropertyName(i) << ": " ;
      if(dataset->isNull(i))
      {
        std::cout <<  std::endl;
        continue;
      }
      std::cout << dataset->getAsString(i) << std::endl;
    }
  }
  dataset->moveBeforeFirst();
}

