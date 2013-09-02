/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ogr/DataSetTypePersistence.cpp

  \brief DataSetTypePersistence implementation for OGR data provider.
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../datatype/Enums.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypePersistence.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/GeometryProperty.h"
#include "DataSource.h"
#include "DataSetTypePersistence.h"
#include "DataSourceTransactor.h"
#include "Globals.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <cassert>

te::ogr::DataSetTypePersistence::DataSetTypePersistence(DataSourceTransactor* t)
  : m_t(t)
{
  assert(m_t);
}

te::ogr::DataSetTypePersistence::~DataSetTypePersistence()
{
}

void te::ogr::DataSetTypePersistence::create(te::da::DataSetType* dt, const std::map<std::string, std::string>& /*options*/)
{
  OGRwkbGeometryType geomType = wkbUnknown;

  if(dt->hasGeom())
    geomType = Convert2OGR(te::da::GetFirstGeomProperty(dt)->getGeometryType());

  OGRDataSource* ogrds = m_t->getOGRDataSource();
  OGRLayer* newLayer = ogrds->CreateLayer(dt->getName().c_str(), 0, geomType);
  if(newLayer == 0)
    throw(te::common::Exception(TR_OGR("Error when attempting create the dataset type.")));

// add the feature-type to the catalog if needed
  //unsigned int id = m_t->getOGRDataSource()->GetLayerCount() - 1;
  //if(dt->getCatalog())
  //{
    //dt->getCatalog()->setId(dt, id);
  //}
  //else
  //{
    //dt->setId(id);
    //m_t->getDataSource()->getCatalog()->add(dt);
  //}

// add the properties
  for(size_t i = 0; i < dt->size(); ++i)
    add(dt, dt->getProperty(i));
}

void te::ogr::DataSetTypePersistence::clone(const std::string& /*datasetName*/, const std::string& /*newDatasetName*/, const std::map<std::string, std::string>& /*options*/)
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::DataSetTypePersistence::drop(te::da::DataSetType* dt)
{
  OGRErr error = m_t->getOGRDataSource()->DeleteLayer(dt->getId());
  
  if(error != OGRERR_NONE)
  {
    if(error == OGRERR_UNSUPPORTED_OPERATION)
      throw(te::common::Exception(TR_OGR("Deleting layers is not supported for this datasource.")));
    else
      throw(te::common::Exception(TR_OGR("Error when attempting remove the dataset type.")));
  }

  if(dt->getCatalog())
  {
    te::da::DataSourceCatalog* catalog = dt->getCatalog();
    catalog->remove(dt, true);
  }
  else
  {
    delete dt;
  }
}

void te::ogr::DataSetTypePersistence::drop(const std::string& /*datasetName*/)
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* dt, te::dt::Property* p)
{
  OGRLayer* layer = m_t->getOGRDataSource()->GetLayer(dt->getId());
  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  if(p->getType() != te::dt::GEOMETRY_TYPE)
  {
    OGRErr error = layer->CreateField(Convert2OGR(p));
    if(error != OGRERR_NONE)
      throw(te::common::Exception(TR_OGR("Error when attempting add the property type.")));
  }
//  throw(te::common::Exception(TR_OGR("OGR Driver not support adding geometry type.")));
}

te::da::DataSourceTransactor* te::ogr::DataSetTypePersistence::getTransactor() const
{
  return m_t;
}


void te::ogr::DataSetTypePersistence::rename(te::da::DataSetType* /*dt*/, const std::string& /*newName*/)
{ 
}

void te::ogr::DataSetTypePersistence::drop(te::dt::Property* /*p*/)
{

}

void te::ogr::DataSetTypePersistence::rename(te::dt::Property* /*p*/, const std::string& /*newName*/)
{  

}

void te::ogr::DataSetTypePersistence::update(te::dt::Property* /*oldP*/, te::dt::Property* /*newP*/)
{

}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::PrimaryKey* /*pk*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::PrimaryKey* /*pk*/)
{

}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::UniqueKey* /*uk*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::UniqueKey* /*uk*/)
{

}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::Index* /*index*/, const std::map<std::string, std::string>& /*options*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::Index* /*index*/)
{

}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::ForeignKey* /*fk*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::ForeignKey* /*fk*/)
{

}

void te::ogr::DataSetTypePersistence::add(te::da::DataSetType* /*dt*/, te::da::CheckConstraint* /*cc*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::CheckConstraint* /*cc*/)
{

}

void te::ogr::DataSetTypePersistence::create(te::da::Sequence* /*sequence*/)
{

}

void te::ogr::DataSetTypePersistence::drop(te::da::Sequence* /*sequence*/)
{
}

