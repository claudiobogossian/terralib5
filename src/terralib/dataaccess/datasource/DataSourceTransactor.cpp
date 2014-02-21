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
  \file terralib/dataaccess/datasource/DataSourceTransactor.cpp

  \brief A DataSourceTransactor can be viewed as a gateway for reading/writing things into the data source.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/GeometryProperty.h"
#include "../../datatype/Property.h"
#include "../dataset/ObjectIdSet.h"
#include "../query/DataSetName.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/Select.h"
#include "../query/Where.h"
#include "../utils/Utils.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"

te::da::DataSourceTransactor::DataSourceTransactor()
{
}

te::da::DataSourceTransactor::~DataSourceTransactor()
{
}

std::auto_ptr<te::da::DataSet> te::da::DataSourceTransactor::getDataSet(const std::string& name,
                                                                        const te::da::ObjectIdSet* oids,
                                                                        te::common::TraverseType travType, 
                                                                        bool connected,
                                                                        const te::common::AccessPolicy accessPolicy)
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

  std::auto_ptr<te::da::DataSet> result = query(select.get(), travType, connected);

  return result;
}

void te::da::DataSourceTransactor::changePropertyDefinition(const std::string& datasetName, const std::string& propName, te::dt::Property* newProp)
{
}
