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
  \file QueryExamples.h

  \brief Several examples declarations.
 */

#ifndef __TERRALIB_EXAMPLES_QT_QUERY_QUERYEXAMPLES_H
#define __TERRALIB_EXAMPLES_QT_QUERY_QUERYEXAMPLES_H

// TerraLib
#include <terralib/dataaccess/dataset/DataSet.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/query/Select.h>

// STL
#include <string>

/* Load a shape data source and keep it in a data source manager */
te::da::DataSourcePtr LoadShapeDataSource(const std::string& fileName, const std::string& dsId);

/* Load a databse data source and keep it in a data source manager */
te::da::DataSourcePtr LoadPGISDataSource(const std::string& dsId);

/* Create a dataset given a select query and a data source */
te::da::DataSet* GetDataSet(te::da::Select& s, te::da::DataSourcePtr& ds);

#endif  // __TERRALIB_EXAMPLES_QT_QUERY_QUERYEXAMPLES_H
