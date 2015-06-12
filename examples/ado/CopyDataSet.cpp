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
//#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>

// STL
#include <iostream>

void CreateDataSource(std::string name)
{
  std::map<std::string, std::string> connInfo;
#ifdef _M_IX86
  connInfo["PROVIDER"] = "Microsoft.Jet.OLEDB.4.0";
#else
  connInfo["PROVIDER"] = "Microsoft.ACE.OLEDB.12.0";
#endif
  connInfo["DB_NAME"] = name;
  connInfo["CREATE_OGC_METADATA_TABLES"] = "TRUE";

  std::auto_ptr<te::da::DataSource> outDs = te::da::DataSource::create("ADO", connInfo);
}

void Copy(std::string dataSetName, std::auto_ptr<te::da::DataSource> inDs, std::auto_ptr<te::da::DataSource> outDs)
{

  std::auto_ptr<te::da::DataSetType> inDst = inDs->getDataSetType(dataSetName);

  std::auto_ptr<te::da::DataSet> inDset = inDs->getDataSet(dataSetName);

  std::map<std::string, std::string> options;

  inDset->moveFirst();

  te::da::Create(outDs.get(), inDst.get(), inDset.get(), options);

}

void CopyFromShapeFile()
{
  std::map<std::string, std::string> ogrInfo;
  ogrInfo["SOURCE"] = "D://DADOS//shapes//Pontos//Focos20120710a20120717_Brasil_25.shp";

  std::auto_ptr<te::da::DataSource> inDs = te::da::DataSourceFactory::make("OGR");
  inDs->setConnectionInfo(ogrInfo);
  inDs->open();
  
  std::map<std::string, std::string> connInfo;
#ifdef _M_IX86 
  connInfo["PROVIDER"] = "Microsoft.Jet.OLEDB.4.0";
#else
  connInfo["PROVIDER"] = "Microsoft.ACE.OLEDB.12.0";
#endif
  connInfo["HOST"] = "localhost";
  connInfo["USER_NAME"] = "";
  connInfo["PASSWORD"] = "";
  connInfo["DB_NAME"] = "C://Users//Felipe Siqueira//Desktop//testeado//Banco//ADODataSource.accdb";
   
  // Create a data source using the data source factory
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("ADO"));

  // Open the data source using the connection information given above
  ds->setConnectionInfo(connInfo);
  ds->open();

  Copy("Focos20120710a20120717_Brasil_25", inDs, ds);
}
