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
  \file terralib/ogr/DataSetPersistence.cpp

  \brief This is the OGR implementation for the DataSetPersistence API..
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/ArrayProperty.h"
#include "../datatype/ByteArray.h"
#include "../datatype/Date.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"
#include "../datatype/TimeDuration.h"
#include "../datatype/TimeInstant.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSourceTransactor.h"
#include "Globals.h"
#include "Utils.h"

// OGR
#include "ogrsf_frmts.h"

// STL
#include <cassert>

te::ogr::DataSetPersistence::DataSetPersistence(DataSourceTransactor* t):
  m_t(t)
{}

te::ogr::DataSetPersistence::~DataSetPersistence()
{
}

void te::ogr::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t limit)
{
  te::ogr::DataSetTypePersistence* pers = new te::ogr::DataSetTypePersistence(m_t);
  pers->create(dt, options);
  add(dt->getName(), d, options, limit);
  delete pers;
}

void te::ogr::DataSetPersistence::remove(const std::string& /*datasetName*/)
{
  throw te::common::Exception(TR_OGR("OGR driver: not implemented yet."));
}

void te::ogr::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw te::common::Exception(TR_OGR("OGR driver: not implemented yet."));
}

void te::ogr::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
  OGRLayer* layer = m_t->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  te::dt::ByteArray* ba = 0;
  te::dt::DateTime* dtm = 0;
  OGRGeometry* OGRgeom = 0;
  std::size_t currfield;

  m_t->begin();

  while(d->moveNext())
  {
    OGRFeature* feat = OGRFeature::CreateFeature(layer->GetLayerDefn());
    currfield = 0;
    for(size_t i = 0; i < d->size(); ++i)
    {
      if(d->isNull(i))
      {
        if(d->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
          ++currfield;
        continue;
      }

      switch(d->getPropertyDataType(i))
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

        /*case te::dt::DATETIME_TYPE:
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
        }*/
      
        case te::dt::GEOMETRY_TYPE:
          OGRgeom = Convert2OGR(d->getGeometry(i));
          feat->SetGeometry(OGRgeom);
        break;

        default:
          throw(te::common::Exception(TR_OGR("Unsupported data type by OGR.")));
      }
    }

    if(layer->CreateFeature(feat) != OGRERR_NONE)
    {
      OGRFeature::DestroyFeature(feat);
      throw te::common::Exception(TR_OGR("Fail to insert dataset item."));
    }

    OGRFeature::DestroyFeature(feat);
  }

  m_t->commit();
}

void te::ogr::DataSetPersistence::update(const std::string& /*datasetName*/,
                                         te::da::DataSet* /*dataset*/,
                                         const std::vector<std::size_t>& /*properties*/,
                                         const te::da::ObjectIdSet* /*oids*/,
                                         const std::map<std::string, std::string>& /*options*/,
                                         std::size_t /*limit*/)
{
  throw te::common::Exception(TR_OGR("OGR driver does not support this method."));
}
