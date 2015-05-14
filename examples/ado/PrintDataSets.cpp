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
  \file examples/ado/PrintDataSets.cpp

  \brief It displays the values of all the datasets of a data source.
*/

// Example
#include "ADOExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>

// STL
#include <iostream>


void PrintDataSets(te::da::DataSource* ds)
{
  if(ds == 0 || !ds->isOpened())
  {
    std::cout << "The informed data source is NULL or is closed!" << std::endl;
    return;
  }

  std::vector<std::string> datasets = ds->getDataSetNames();

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    // Retrieve the dataset by its name
    std::auto_ptr<te::da::DataSet> dataset(ds->getDataSet(datasets[i]));

    // Print its data to the standard output
    PrintDataSetValues(datasets[i], dataset.get());
  }
}
