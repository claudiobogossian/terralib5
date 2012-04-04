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
  \file DataSetPersistence.cpp

  \brief An abstract class responsible for persisting a dataset in a DataSource.
 */

#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSourceTransactor.h"
#include "Globals.h"
#include "Utils.h"

#include "../common/Exception.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"

#include "../datatype/ByteArray.h"
#include "../datatype/Date.h"
#include "../datatype/TimeDuration.h"
#include "../datatype/TimeInstant.h"

#include "../datatype/ArrayProperty.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"

#include "ogrsf_frmts.h"

#include <cassert>

te::ogr::DataSetPersistence::DataSetPersistence(DataSourceTransactor* t):
  m_t(t)
{}

te::ogr::DataSetPersistence::~DataSetPersistence()
{
}

void te::ogr::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t /*limit*/)
{
  te::ogr::DataSetTypePersistence* pers = new te::ogr::DataSetTypePersistence(m_t);
  pers->create(dt, options);
  add(dt,d);
  delete pers;
}

void te::ogr::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/)
{
  throw te::common::Exception(TR_OGR("OGR driver: not implemented yet."));  
}

void te::ogr::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, std::size_t /*limit*/)
{
  throw te::common::Exception(TR_OGR("OGR driver: not implemented yet."));
}

void te::ogr::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSetItem* /*item*/)
{
  throw te::common::Exception(TR_OGR("OGR driver does not support the concept of data set item."));
}

void te::ogr::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  OGRLayer* layer = m_t->getOGRDataSource()->GetLayer(dt->getId());
  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  te::dt::ByteArray* ba = 0;
  te::dt::DateTime* dtm = 0;
  OGRGeometry* OGRgeom = 0;
  size_t currfield;

  m_t->begin();
  d->moveBeforeFirst();
  while (d->moveNext())
  {
    OGRFeature* feat = OGRFeature::CreateFeature(layer->GetLayerDefn());
    currfield = 0;
    for (size_t i = 0; i < dt->size(); ++i)
    {
      if(d->isNull(i))
      {
        if (dt->getProperty(i)->getType() != te::dt::GEOMETRY_TYPE)
          ++currfield;
        continue;
      }

      te::dt::Property* p = dt->getProperty(i);
      switch(p->getType())
      {
        case te::dt::INT32_TYPE:
          feat->SetField(currfield, d->getInt32(i));
          ++currfield;
        break;

        case te::dt::STRING_TYPE:
          feat->SetField(currfield,d->getAsString(i).c_str());
          ++currfield;
        break;

        case te::dt::DOUBLE_TYPE:
          feat->SetField(currfield,d->getDouble(i));
          ++currfield;        
         break;

        case te::dt::NUMERIC_TYPE:
          feat->SetField(currfield,atof(d->getNumeric(i).c_str()));
          ++currfield;
        break;

        case te::dt::BYTE_ARRAY_TYPE:
          ba = d->getByteArray(i);
          feat->SetField(currfield,ba->bytesUsed(),(unsigned char*) ba->getData());
          ++currfield;
          delete ba;
        break;

        //case te::dt::ARRAY_TYPE:
        //{
        //  te::da::ArrayProperty* at = static_cast<te::da::ArrayProperty*>(p);
        //  int elementType = at->getElementType()->getType();
        //
        //  if(elementType == te::dt::INT32_TYPE)
        //    feat->SetField();
        //  else if(elementType == te::dt::STRING_TYPE)
        //    feat->SetField();
        //  else if(elementType == te::dt::DOUBLE_TYPE)
        //    feat->SetField();
        //  else
        //    throw(te::common::Exception(TR_OGR("Unsupported data type by OGR.")));
        //}

        case te::dt::DATETIME_TYPE:
        {
          const te::dt::DateTimeProperty* dp = static_cast<const te::dt::DateTimeProperty*>(p);
          dtm = d->getDateTime(i);
          te::dt::DateTimeType elementType = dp->getSubType();
          if(elementType == te::dt::DATE)
          {
            te::dt::Date* d = static_cast<te::dt::Date*>(dtm);
            feat->SetField(currfield,(int)d->getYear(),(int)d->getMonth(),(int)d->getDay());
          }
          else if(elementType == te::dt::TIME_DURATION)
          {
            te::dt::TimeDuration* td = static_cast<te::dt::TimeDuration*>(dtm);
            feat->SetField(currfield,0,0,0,(int)td->getHours(),(int)td->getMinutes(),(int)td->getSeconds());
          }
          else if(elementType == te::dt::TIME_INSTANT)
          {
            te::dt::Date d = static_cast<te::dt::TimeInstant*>(dtm)->getDate();
            te::dt::TimeDuration td = static_cast<te::dt::TimeInstant*>(dtm)->getTime();
            feat->SetField( currfield,(int)d.getYear(), (int)d.getMonth(), (int)d.getDay(),
                            (int)td.getHours(),(int)td.getMinutes(),(int)td.getSeconds());
          }
          ++currfield;
          delete dtm;
          break;
        }
      
        case te::dt::GEOMETRY_TYPE:
          OGRgeom = Convert2OGR(d->getGeometry(i));
          feat->SetGeometry(OGRgeom);
        break;

        default:
          throw(te::common::Exception(TR_OGR("Unsupported data type by OGR.")));
      }
    }
    if (layer->CreateFeature(feat) != OGRERR_NONE )
    {
      OGRFeature::DestroyFeature(feat);
      throw te::common::Exception(TR_OGR("Fail to insert dataset item."));
    }
    OGRFeature::DestroyFeature(feat);    
  }
  m_t->commit();
}

void te::ogr::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  OGRLayer* layer = m_t->getOGRDataSource()->GetLayer(dt->getId());
  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  te::dt::ByteArray* ba = 0;
  te::dt::DateTime* dtm = 0;
  OGRGeometry* OGRgeom = 0;
  size_t currfield = 0;  
  
  OGRFeature* feat = OGRFeature::CreateFeature(layer->GetLayerDefn());
  for (size_t i = 0; i < item->getType()->size(); ++i)
  {
    if (item->isNull(i))
    {
      if (dt->getType() != te::dt::GEOMETRY_TYPE)
        ++currfield;
      continue;
    }

    te::dt::Property* p = dt->getProperty(i);
    switch(p->getType())
    {
      case te::dt::INT32_TYPE:
        feat->SetField(currfield, item->getInt32(i));
        ++currfield;
      break;

      case te::dt::STRING_TYPE:
        feat->SetField(currfield,item->getAsString(i).c_str());
        ++currfield;
      break;

      case te::dt::DOUBLE_TYPE:
        feat->SetField(currfield,item->getDouble(i));
        ++currfield;        
        break;

      case te::dt::NUMERIC_TYPE:
        feat->SetField(currfield,atof(item->getNumeric(i).c_str()));
        ++currfield;
      break;

      case te::dt::BYTE_ARRAY_TYPE:
        ba = item->getByteArray(i);
        feat->SetField(currfield,ba->bytesUsed(),(unsigned char*) ba->getData());
        ++currfield;
        delete ba;
      break;

      //case te::dt::ARRAY_TYPE:
      //{
      //  te::da::ArrayProperty* at = static_cast<te::da::ArrayProperty*>(p);
      //  int elementType = at->getElementType()->getType();
      //
      //  if(elementType == te::dt::INT32_TYPE)
      //    feat->SetField();
      //  else if(elementType == te::dt::STRING_TYPE)
      //    feat->SetField();
      //  else if(elementType == te::dt::DOUBLE_TYPE)
      //    feat->SetField();
      //  else
      //    throw(te::common::Exception(TR_OGR("Unsupported data type by OGR.")));
      //}

      case te::dt::DATETIME_TYPE:
      {
        const te::dt::DateTimeProperty* dp = static_cast<const te::dt::DateTimeProperty*>(p);
        dtm = item->getDateTime(i);
        te::dt::DateTimeType elementType = dp->getSubType();
        
        if(elementType == te::dt::DATE)
        {
          te::dt::Date* d = static_cast<te::dt::Date*>(dtm);
          feat->SetField(currfield,(int)d->getYear(),(int)d->getMonth(),(int)d->getDay());
        }
        else if(elementType == te::dt::TIME_DURATION)
        {
          te::dt::TimeDuration* td = static_cast<te::dt::TimeDuration*>(dtm);
          feat->SetField(currfield,0,0,0,(int)td->getHours(),(int)td->getMinutes(),(int)td->getSeconds());
        }
        else if(elementType == te::dt::TIME_INSTANT)
        {
          te::dt::Date d = static_cast<te::dt::TimeInstant*>(dtm)->getDate();
          te::dt::TimeDuration td = static_cast<te::dt::TimeInstant*>(dtm)->getTime();
          feat->SetField( currfield,(int)d.getYear(), (int)d.getMonth(), (int)d.getDay(),
                            (int)td.getHours(),(int)td.getMinutes(),(int)td.getSeconds());          
        }        
        ++currfield;
        delete dtm;
        break;
      }

      case te::dt::GEOMETRY_TYPE:
        OGRgeom = Convert2OGR(item->getGeometry(i));
        feat->SetGeometry(OGRgeom);
      break;

      default:
        throw(te::common::Exception(TR_OGR("Unsupported data type by OGR.")));
    }
  }
  if (layer->CreateFeature(feat) != OGRERR_NONE )
  {
    OGRFeature::DestroyFeature(feat);
    throw te::common::Exception(TR_OGR("Fail to insert dataset item."));
  }
  OGRFeature::DestroyFeature(feat);
}

void te::ogr::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
            te::da::DataSet* /*f*/,
            const std::vector<te::dt::Property*>& /*properties*/,
            std::size_t /*limit*/)
{
  throw te::common::Exception(TR_OGR("OGR driver does not support this method."));
}

void te::ogr::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSetItem* /*item*/,
                    const std::vector<te::dt::Property*>& /*properties*/)
{
  throw te::common::Exception(TR_OGR("OGR driver does not support this method."));
}

void te::ogr::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                    te::da::DataSet* /*oldD*/,
                    te::da::DataSet* /*newD*/,
                    const std::vector<te::dt::Property*>& /*properties*/,
                    std::size_t /*limit*/)
{
    throw te::common::Exception(TR_OGR("OGR driver does not support this method."));
}

void te::ogr::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
            te::da::DataSetItem* /*oldItem*/,
            te::da::DataSetItem* /*newItem*/,
            const std::vector<te::dt::Property*>& /*properties*/)
{
  throw te::common::Exception(TR_OGR("OGR driver does not support this method."));
}

te::da::DataSourceTransactor* te::ogr::DataSetPersistence::getTransactor() const
{
  return m_t;
}
