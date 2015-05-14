/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file examples/ado/PrintDatasetValues.cpp

  \brief It displays the values of a dataset
*/

// Example
#include "ADOExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>

// STL
#include <iostream>


void PrintDataSetValues(const std::string& datasetName, te::da::DataSet* dataset)
{

  dataset->isConnected(); //teste
  if(dataset == 0)
  {
    std::cout << "The informed dataset is NULL!" << std::endl;
    return;
  }

  std::size_t numProperties = dataset->getNumProperties();

  // This will be used just to count the items in the dataset
  int item = 0;

  std::cout << std::endl <<  "===========================================================" << std::endl;
  std::cout << std::endl <<  "===========================================================" << std::endl;
  std::cout << "Printing information about the dataset: " << datasetName << std::endl;

  // Traverse the dataset and print each dataset item
  while(dataset->moveNext())
  {
    std::cout << std::endl << "Row Number: " << item++ << " =======================" << std::endl;
  
    for(std::size_t i = 0; i < numProperties; ++i)
    {
      std::cout << dataset->getPropertyName(i) << ": " ; 

      // Check if the value is not null
      if(dataset->isNull(i))
      {
        std::cout << std::endl;
        continue;
      }
  
      std::string value;
      // Get the data value if it is not binary
      int colType = dataset->getPropertyDataType(i);
      if(colType == te::dt::BYTE_ARRAY_TYPE)
        value = "Binary Type";
      else if(colType == te::dt::UNKNOWN_TYPE)
        value = "Unknown column type";
      else if(colType == te::dt::DATETIME_TYPE)
        value = "Not implemented yet";
      else
        value = dataset->getAsString(i);

      std::cout << value << std::endl;
    }
  }
}
