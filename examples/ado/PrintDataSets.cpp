/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file examples/ado/PrintDataSets.cpp

  \brief It displays the values of all the datasets of a data source.
*/

// Example
#include "ADOExamples.h"

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

  // Get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

  // From the transactor, take a catalog loader to find out the datasets stored in the data source catalog
  te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

  // Retrieve the name of the datasets
  boost::ptr_vector<std::string> datasets;

  cloader->getDataSets(datasets);

  // Iterate over the dataset names to retrieve its information
  std::cout << "Printing all the data in datasets... number of datasets: " << datasets.size() << std::endl;

  for(unsigned int i=0; i<datasets.size(); ++i)
  {
    // Retrieve the dataset by its name
    te::da::DataSet* dataset = transactor->getDataSet(datasets[i]);

    // Print its data to the standard output
    PrintDataSetValues(dataset);

    // Release the dataset: you are the owner
    delete dataset;
  }

  // Release the catalog loader: you are the owner
  delete cloader;

  // Release the transactor: you are the owner
  delete transactor;
}
