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
  \file terralib/dataaccess/datasource/DataSourceTransactor.cpp

  \brief A DataSourceTransactor can be viewed as a gateway for reading/writing things into the data source.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/GeometryProperty.h"
#include "../../datatype/Property.h"
#include "../dataset/DataSetType.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceCatalog.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"

// STL
#include <memory>

te::da::DataSet* te::da::DataSourceTransactor::getDataSet(const std::string& name,
                                                          const te::gm::Envelope* e,
                                                          te::gm::SpatialRelation r,
                                                          te::common::TraverseType travType, 
                                                          te::common::AccessPolicy rwRole)
{
  if(e == 0)
    throw Exception(TR_DATAACCESS("You must provide a MBR for using this method!"));

// let's find the default geometric property
  te::dt::Property* p = 0;

// look in the catalog first
  te::da::DataSourceCatalog* catalog = getDataSource()->getCatalog();

  if(catalog != 0)
  {
    te::da::DataSetType* dt = catalog->getDataSetType(name);

    if(dt != 0)
    {
      p = dt->getDefaultGeomProperty();
    
      if(p == 0)
        throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
      else
        return getDataSet(name, p, e, r, travType, rwRole);
    }
  }
  
// still looking for the property?
  std::auto_ptr<DataSourceCatalogLoader> cloader(getCatalogLoader());

  std::auto_ptr<te::da::DataSetType> dt(cloader->getDataSetType(name, false));

  p = dt->getDefaultGeomProperty();

  if(p == 0)
    throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
  else
  {
// let's cache the data set type if there is a catalog!
    if(catalog)
      catalog->add(dt.release());

    return getDataSet(name, p, e, r, travType, rwRole);
  }
}

te::da::DataSet* te::da::DataSourceTransactor::getDataSet(const std::string& name,
                                                          const te::gm::Geometry* g,
                                                          te::gm::SpatialRelation r,
                                                          te::common::TraverseType travType, 
                                                          te::common::AccessPolicy rwRole)
{
  if(g == 0)
    throw Exception(TR_DATAACCESS("You must provide a valid geometry for using this method!"));

// let's find the default geometric property
  te::dt::Property* p = 0;

// look in the catalog first
  te::da::DataSourceCatalog* catalog = getDataSource()->getCatalog();

  if(catalog != 0)
  {
    te::da::DataSetType* dt = catalog->getDataSetType(name);

    if(dt != 0)
    {
      p = dt->getDefaultGeomProperty();
    
      if(p == 0)
        throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
      else
        return getDataSet(name, p, g, r, travType, rwRole);
    }
  }
  
// still looking for the property?
  std::auto_ptr<DataSourceCatalogLoader> cloader(getCatalogLoader());

  std::auto_ptr<te::da::DataSetType> dt(cloader->getDataSetType(name, false));

  p = dt->getDefaultGeomProperty();

  if(p == 0)
    throw Exception(TR_DATAACCESS("The dataset doesn't have a default geometric column or you missed it during catalog management!"));
  else
  {
// let's cache the data set type if there is a catalog!
    if(catalog)
      catalog->add(dt.release());

    return getDataSet(name, p, g, r, travType, rwRole);
  }
}

