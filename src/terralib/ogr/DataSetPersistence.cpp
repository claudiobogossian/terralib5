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
#include <memory>

te::ogr::DataSetPersistence::DataSetPersistence(DataSourceTransactor* t):
  m_t(t)
{}

te::ogr::DataSetPersistence::~DataSetPersistence()
{
}

void te::ogr::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw te::common::Exception(TR_OGR("OGR driver: not implemented yet."));
}

void te::ogr::DataSetPersistence::add(const std::string& datasetName,
                                      te::da::DataSet* d,
                                      const std::map<std::string, std::string>& /*options*/,
                                      std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

  OGRLayer* layer = m_t->getOGRDataSource()->GetLayerByName(datasetName.c_str());

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("DataSet not found.")));

  m_t->begin();

  std::size_t nproperties = d->getNumProperties();

  std::size_t nProcessedRows = 0;

  do
  {
    OGRFeature* feat = OGRFeature::CreateFeature(layer->GetLayerDefn());

    std::size_t currfield = 0;

    for(std::size_t i = 0; i != nproperties; ++i)
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
          feat->SetField(currfield, d->getAsString(i).c_str());
          ++currfield;
        break;

        case te::dt::DOUBLE_TYPE:
          feat->SetField(currfield, d->getDouble(i));
          ++currfield;
         break;

        case te::dt::NUMERIC_TYPE:
          feat->SetField(currfield, atof(d->getNumeric(i).c_str()));
          ++currfield;
        break;

        case te::dt::BYTE_ARRAY_TYPE:
          {
            std::auto_ptr<te::dt::ByteArray> ba(d->getByteArray(i));
            feat->SetField(currfield, ba->bytesUsed(), reinterpret_cast<unsigned char*>(ba->getData()));
            ++currfield;
          }
        break;

        case te::dt::DATETIME_TYPE:
          {
            std::auto_ptr<te::dt::DateTime> dtm(d->getDateTime(i));

            te::dt::Date* dtime = dynamic_cast<te::dt::Date*>(dtm.get());

            if(dtime)
            {
              feat->SetField(currfield,
                             static_cast<int>(dtime->getYear()),
                             static_cast<int>(dtime->getMonth()),
                             static_cast<int>(dtime->getDay()));
              ++currfield;
              break;
            }

            te::dt::TimeDuration* tduration = dynamic_cast<te::dt::TimeDuration*>(dtm.get());

            if(tduration)
            {
              feat->SetField(currfield, 0, 0, 0,
                             static_cast<int>(tduration->getHours()),
                             static_cast<int>(tduration->getMinutes()),
                             static_cast<int>(tduration->getSeconds()));
              ++currfield;
              break;
            }

            te::dt::TimeInstant* tinst = dynamic_cast<te::dt::TimeInstant*>(dtm.get());

            if(tinst)
            {
              feat->SetField(currfield,
                             static_cast<int>(dtime->getYear()),
                             static_cast<int>(dtime->getMonth()),
                             static_cast<int>(dtime->getDay()),
                             static_cast<int>(tduration->getHours()),
                             static_cast<int>(tduration->getMinutes()),
                             static_cast<int>(tduration->getSeconds()));
              ++currfield;
              break;
            }

            throw(te::common::Exception(TR_OGR("Unsupported date and time type by OGR.")));
          }
        break;

        case te::dt::GEOMETRY_TYPE:
          {
            std::auto_ptr<te::gm::Geometry> geom(d->getGeometry(i));
            OGRGeometry* OGRgeom = Convert2OGR(geom.get());
            feat->SetGeometryDirectly(OGRgeom);
          }
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

  }while(d->moveNext() && (nProcessedRows != limit));

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
