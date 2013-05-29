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
  \file terralib/dataaccess/datasource/AbstractDataSource.cpp

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../common/Translator.h"
#include "../../Exception.h"
#include "AbstractDataSource.h"

// STL
#include <cassert>
#include <memory>

std::string te::da::core::AbstractDataSource::getConnectionStr() const
{
  return std::string("");
}

void te::da::core::AbstractDataSource::setConnectionStr(const std::string& connInfo)
{
  std::map<std::string, std::string> connInfoMap;

  te::common::ExtractKVP(connInfo, connInfoMap, "&", "=", false);

  setConnectionInfo(connInfoMap);
}

void te::da::core::AbstractDataSource::open(const std::map<std::string, std::string>& connInfo)
{
  setConnectionInfo(connInfo);
  open();
}

void te::da::core::AbstractDataSource::open(const std::string& connInfo)
{
  setConnectionStr(connInfo);
  open();
}

te::da::core::DataSet* te::da::core::AbstractDataSource::getDataSet(const std::string& name,
                                                                    const te::gm::Envelope* e,
                                                                    te::gm::SpatialRelation r,
                                                                    te::common::TraverseType travType)
{
//  if(e == 0)
//    throw Exception(TR_DATAACCESS("You must provide a MBR for using this method!"));
//
//// let's find the default geometric property
//  te::dt::Property* p = 0;
//
//// look in the catalog first
//  te::da::DataSourceCatalog* catalog = getDataSource()->getCatalog();
//
//  if(catalog != 0)
//  {
//    const te::da::DataSetTypePtr& dt = catalog->getDataSetType(name);
//
//    if(dt.get() != 0)
//    {
//      p = GetFirstSpatialProperty(dt.get());
//
//      if(p == 0)
//        throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
//      else
//        return getDataSet(name, p, e, r, travType, rwRole);
//    }
//  }
//  
//// still looking for the property?
//  std::auto_ptr<DataSourceCatalogLoader> cloader(getCatalogLoader());
//
//  DataSetTypePtr dt(cloader->getDataSetType(name, false));
//
//  p = GetFirstSpatialProperty(dt.get());
//
//  if(p == 0)
//    throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
//  else
//  {
//// let's cache the data set type if there is a catalog!
//    if(catalog)
//      catalog->add(dt);
//
//    return getDataSet(name, p, e, r, travType, rwRole);
//  }
  return 0;
}

te::da::core::DataSet* te::da::core::AbstractDataSource::getDataSet(const std::string& name,
                                                                    const te::gm::Geometry* g,
                                                                    te::gm::SpatialRelation r,
                                                                    te::common::TraverseType travType)
{
//  if(g == 0)
//    throw Exception(TR_DATAACCESS("You must provide a valid geometry for using this method!"));
//
//// let's find the default geometric property
//  te::dt::Property* p = 0;
//
//// look in the catalog first
//  te::da::DataSourceCatalog* catalog = getDataSource()->getCatalog();
//
//  if(catalog != 0)
//  {
//    const te::da::DataSetTypePtr& dt = catalog->getDataSetType(name);
//
//    if(dt.get() != 0)
//    {
//      p = GetFirstSpatialProperty(dt.get());
//    
//      if(p == 0)
//        throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
//      else
//        return getDataSet(name, p, g, r, travType, rwRole);
//    }
//  }
//  
//// still looking for the property?
//  std::auto_ptr<DataSourceCatalogLoader> cloader(getCatalogLoader());
//
//  DataSetTypePtr dt(cloader->getDataSetType(name, false));
//
//  p = GetFirstSpatialProperty(dt.get());
//
//  if(p == 0)
//    throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
//  else
//  {
//// let's cache the data set type if there is a catalog!
//    if(catalog)
//      catalog->add(dt);
//
//    return getDataSet(name, p, g, r, travType, rwRole);
//  }
  return 0;
}

te::da::core::DataSet* te::da::core::AbstractDataSource::getDataSet(const std::string& name,
                                                                    const ObjectIdSet* oids,
                                                                    te::common::TraverseType travType)
{
  //assert(!name.empty());
  //assert(oids);
  //assert(oids->size() > 0);

  //// ObjectIds restriction
  //Expression* exp = oids->getExpression();
  //assert(exp);

  //// Where clause
  //Where* filter = new Where(exp);
  //
  //// All fields (?)
  //te::da::Fields* all = new te::da::Fields;
  //all->push_back(new te::da::Field("*"));
  //
  //// From the data set
  //FromItem* fromItem = new DataSetName(name);
  //From* from = new From;
  //from->push_back(fromItem);

  //// The final Select
  //std::auto_ptr<Select> select(new Select(all, from, filter));

  //DataSet* result = query(select.get(), travType, rwRole);

  //return result;
  return 0;
}

