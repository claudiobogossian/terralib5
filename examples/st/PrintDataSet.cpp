// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void PrintDataSet(te::da::DataSet* dataset, const std::string& datasetName)
{
  if(dataset == 0)
  {
    std::cout << "The informed dataset is NULL!" << std::endl;
    return;
  }

  std::size_t nproperties = dataset->getNumProperties();

// this will be used just to count the items in the dataset
  int item = 0;

  std::cout << "Printing information about the dataset: " << datasetName << std::endl;

// traverse the dataset and print each dataset item
  while(dataset->moveNext())
  {
    std::cout << std::endl << "ITEM NUMBER: " << item++ << " =======================" << std::endl;
  
    for(std::size_t i = 0; i < nproperties; ++i)
    {
      std::cout << dataset->getPropertyName(i) << ": " ; 

// check if value is not null
      if(dataset->isNull(i))
      {
        std::cout << std::endl;
        continue;
      }

// get data value
      std::string value = dataset->getAsString(i);

      std::cout << value << std::endl;
    }
  }
}
