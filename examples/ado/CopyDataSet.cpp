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
  \file examples/ado/CopyDataSet.cpp

  \brief It copy a DataSet
*/

// Example
#include "ADOExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>

// STL
#include <iostream>

void CreateDataSource(std::string name)
{
  // Set the minimum server connection information needed to connect to the database server
  std::string connInfo("file://");
  std::string data_dir = TERRALIB_DATA_DIR;
  std::string aux("");

  std::cout << "Inform the user to access your Microsoft Access database (ENTER if there is none): ";
  std::getline(std::cin, aux);
  connInfo += aux.empty() ? "" : aux + ":";

  std::cout << "Inform the Password to access Microsoft Access database (ENTER if there is none): ";
  std::getline(std::cin, aux);
  connInfo += aux.empty() ? "" : aux + "@";

  std::cout << "Inform the location of your Microsoft Access database (ENTER to accept default \'" << (data_dir + "/ado/ADODataSource.mdb") << "\'): ";
  std::getline(std::cin, aux);
  if (!aux.empty())
    connInfo += aux;
  else
    connInfo += data_dir + "/ado/ADODataSource.mdb";

  #ifdef _M_IX86
    connInfo += "?&PROVIDER=Microsoft.Jet.OLEDB.4.0";
  #else
    connInfo += "?&PROVIDER=Microsoft.ACE.OLEDB.12.0";
  #endif

  std::unique_ptr<te::da::DataSource> outDs = te::da::DataSource::create("ADO", connInfo);
}

void Copy(std::string dataSetName, std::unique_ptr<te::da::DataSource> inDs, te::da::DataSource* outDs)
{
  std::auto_ptr<te::da::DataSetType> inDst = inDs->getDataSetType(dataSetName);

  std::auto_ptr<te::da::DataSet> inDset = inDs->getDataSet(dataSetName);

  std::map<std::string, std::string> options;

  inDset->moveFirst();

  te::da::Create(outDs, inDst.get(), inDset.get(), options);
}

void CopyFromShapeFile(te::da::DataSource* ds)
{
  // let's take the input dataset from a shape file
  std::string connInfo("file://");
  std::string data_dir = TERRALIB_DATA_DIR;

  std::string aux("");
  std::cout << "Inform the location of your data source (ENTER to accept default \'" << (data_dir + "/shape/poligono_unico.shp") << "\'): ";
  std::getline(std::cin, aux);
  if (!aux.empty())
    connInfo += aux;
  else
    connInfo += data_dir + "/shape/poligono_unico.shp";

  std::unique_ptr<te::da::DataSource> inDs = te::da::DataSourceFactory::make("OGR", connInfo);
  inDs->open();

  if(!aux.empty())
    Copy(aux, std::move(inDs), ds);
  else
    Copy("poligono_unico", std::move(inDs), ds);
}
