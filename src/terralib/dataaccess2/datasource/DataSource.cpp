/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/DataSource.cpp

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

// TerraLib
#include "../dataset/ObjectIdSet.h"
#include "../query/DataSetName.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/From.h"
#include "../query/FromItem.h"
#include "../query/Select.h"
#include "../query/Where.h"
#include "DataSource.h"


te::da::DataSource::DataSource()
{
}

te::da::DataSource::~DataSource()
{
}

te::da::Connection* te::da::DataSource::getConnection()
{
  return 0;
}

void te::da::DataSource::closeConnection(te::da::Connection* /*conn*/)
{
}

std::auto_ptr<te::da::DataSet> te::da::DataSource::getDataSet(const std::string& name,
                                                const te::da::ObjectIdSet* oids,
                                                te::common::TraverseType travType)
{
  assert(!name.empty());
  assert(oids);
  assert(oids->size() > 0);

  // ObjectIds restriction
  Expression* exp = oids->getExpression();
  assert(exp);

  // Where clause
  Where* filter = new Where(exp);
  
  // All fields (?)
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));
  
  // From the data set
  FromItem* fromItem = new DataSetName(name);
  From* from = new From;
  from->push_back(fromItem);

  // The final Select
  std::auto_ptr<Select> select(new Select(all, from, filter));

  std::auto_ptr<te::da::DataSet> result = query(select.get(), travType);

  return result;
}
