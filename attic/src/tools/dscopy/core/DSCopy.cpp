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
  \file terralib/tools/dscopy/core/DSCopy.cpp

  \brief An utility class to provide methods to copy a Data Source
 */

// Terralib Data Source Copy Tool
#include "DSCopy.h"
#include "Utils.h"

// TerraLib
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/dataaccess/datasource/DataSourceCatalogLoader.h>
#include <terralib/dataaccess/dataset/DataSetPersistence.h>

bool te::tools::dscopy::DSCopy::init(std::string & errorMessage)
{
  return Utils::loadModules(errorMessage);
}

bool te::tools::dscopy::DSCopy::copy(std::string dsTypeOrigin, std::string dsConnStrOrigin, std::string dsTypeDestination, 
  std::string dsConnStrDestination, std::vector<std::string*> datasets, std::string& errorMessage)
{

  te::da::DataSource* dsOrigin = te::da::DataSourceFactory::make(dsTypeOrigin);
  dsOrigin->open(dsConnStrOrigin);

  if(dsOrigin == 0)
  {
    errorMessage = "Could not get a connection with the origin DataSource.";
    return false;
  }

  te::da::DataSource* dsDestination = te::da::DataSourceFactory::make(dsTypeDestination);
  dsDestination->open(dsConnStrDestination);

  if(dsDestination == 0)
  {
    errorMessage = "Could not get a connection with the destination DataSource.";
    return false;;
  }

  if(!copyDataSets(dsOrigin, dsDestination, datasets, errorMessage))
  {
    return false;
  }

  return true;

}

bool te::tools::dscopy::DSCopy::copy(te::da::DataSource* dsOrigin, te::da::DataSource* dsDestination, std::vector<std::string*> datasets, std::string& errorMessage)
{
  return copyDataSets(dsOrigin, dsDestination, datasets, errorMessage);
}

bool te::tools::dscopy::DSCopy::copyDataSets(te::da::DataSource* dsOrigin, te::da::DataSource* dsDestination, std::vector<std::string*> datasets, std::string& errorMessage)
{ 
 
  te::da::DataSourceTransactor* tOrigin = dsOrigin->getTransactor();
  te::da::DataSourceCatalogLoader* cOrigin = tOrigin->getCatalogLoader();
  cOrigin->loadCatalog();

  if(datasets.empty())
  {
    std::vector<std::string*> datasetsNames;
    cOrigin->getDataSets(datasetsNames);
    datasets = datasetsNames;
  }

  te::da::DataSourceTransactor* tDestination = dsDestination->getTransactor();
  te::da::DataSetPersistence* pDestination = tDestination->getDataSetPersistence();

  for(unsigned int i = 0; i < datasets.size(); i++)
  {
    te::da::DataSetType* datasettype = tOrigin->getCatalogLoader()->getDataSetType(*datasets[i]);      

    te::da::DataSet* dataSet = tOrigin->getDataSet(*datasets[i]);

    pDestination->create(datasettype, dataSet);
  }

  std::cout << "Copy executed successfully!" << std::endl << std::endl;
  std::cout << "-------------------------" << std::endl;

  if(dsDestination->getType() == "MEM")
  {
    std::vector<std::string*> datasets;
    dsDestination->getTransactor()->getCatalogLoader()->getDataSets(datasets);

    std::cout << std::endl << "In-memory data sets:" << std::endl << std::endl; 
    for(std::size_t i = 0; i < datasets.size(); i++)
      std::cout << "  " << *datasets[i] << std::endl;
  }

  delete tOrigin;
  delete tDestination;

  return true;
}