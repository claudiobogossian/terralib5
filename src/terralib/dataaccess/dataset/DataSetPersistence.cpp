/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/DataSetPersistence.cpp

  \brief An abstract class responsible for persisting a dataset in a data source.
*/

// TerraLib
#include "DataSet.h"
#include "DataSetItem.h"
#include "DataSetPersistence.h"
#include "DataSetType.h"
#include "DataSetTypePersistence.h"

void te::da::DataSetPersistence::create(DataSet* d, std::size_t limit)
{
  std::map<std::string, std::string> options;

  create(d, options, limit);
}

void te::da::DataSetPersistence::create(DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  te::da::DataSetType* dt = d->getType();

  if(!dt->isFullLoaded())
    d->loadTypeInfo();

// get again the dataset type because loadTypeInfo can invalidate dt!
  create(d->getType(), d, options, limit);
}

void te::da::DataSetPersistence::create(DataSetType* dt, DataSet* d, std::size_t limit)
{
  std::map<std::string, std::string> options;

  create(dt, d, options, limit);
}

void te::da::DataSetPersistence::remove(DataSet* d, std::size_t limit)
{
  remove(d->getType(), d, limit);
}

void te::da::DataSetPersistence::remove(DataSetItem* item)
{
  remove(item->getType(), item);
}

