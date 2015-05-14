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
  \file ADOExamples.h

  \brief Examples that show how to access/manipulate an ADO data source.
 */

#ifndef __TERRALIB_EXAMPLES_ADO_INTERNAL_ADOEXAMPLES_H
#define __TERRALIB_EXAMPLES_ADO_INTERNAL_ADOEXAMPLES_H

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/datatype_fw.h>
#include "../Config.h"

/*! \brief It loads the data source drivers. */
void LoadModules();

/*!
  \brief This example will print to the standard output all the information about the datasets stored in a data source.

  \param ds The data source used to retrieve information about their datasets.
 */
void PrintCatalog(te::da::DataSource* ds);

/*!
  \brief It prints datasets in a given data source.

  \param ds The data source you want to print its data.
 */
void PrintDataSets(te::da::DataSource* ds);

/*!
  \brief It prints the data of a given dataset.

  \param dataset A dataset.
 */
void PrintDataSetValues(const std::string& datasetName, te::da::DataSet* dataset);

void Copy(std::string dataSetName, std::auto_ptr<te::da::DataSource> inDs, std::auto_ptr<te::da::DataSource> outDs);

void CopyFromShapeFile();

#endif  // __TERRALIB_EXAMPLES_ADO_INTERNAL_ADOEXAMPLES_H

