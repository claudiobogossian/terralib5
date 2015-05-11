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
  \file IntersectionQuery.h

  \brief Intersection Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/query/And.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/Join.h"
#include "../dataaccess/query/JoinConditionOn.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Intersection.h"
#include "../dataaccess/query/ST_Intersects.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "IntersectionQuery.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

te::vp::IntersectionQuery::IntersectionQuery()
{}

te::vp::IntersectionQuery::~IntersectionQuery()
{}


bool te::vp::IntersectionQuery::run() throw(te::common::Exception)
{
  if(m_SRID == 0)
  {
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inFirstDsetType.get());
    m_SRID = geom->getSRID();
  }

  te::da::Fields* fields = new te::da::Fields;
  te::da::Select* select = new te::da::Select;

  std::vector<te::dt::Property*> firstProps = getTabularProps(m_inFirstDsetType.get());
  std::vector<te::dt::Property*> secondProps;
  if(m_copyInputColumns)
    secondProps = getTabularProps(m_inSecondDsetType.get());

  te::gm::GeometryProperty* firstGeom;
  te::gm::GeometryProperty* secondGeom;

  firstGeom = te::da::GetFirstGeomProperty(m_inFirstDsetType.get());
  secondGeom = te::da::GetFirstGeomProperty(m_inSecondDsetType.get());
      
  std::string firstTableName = te::vp::GetSimpleTableName(m_inFirstDsetType->getTitle());
  std::string secondTableName = te::vp::GetSimpleTableName(m_inSecondDsetType->getTitle());

  for(std::size_t i = 0; i < firstProps.size(); ++i)
  {
    te::da::Field* f_field = new te::da::Field(firstTableName + "." + firstProps[i]->getName() + " ", firstTableName + "_" + firstProps[i]->getName());
    fields->push_back(f_field);
  }

  for(std::size_t i = 0; i < secondProps.size(); ++i)
  {
    te::da::Field* f_field = new te::da::Field(secondTableName + "." + secondProps[i]->getName() + " ", secondTableName + "_" + secondProps[i]->getName());
    fields->push_back(f_field);
  }

  te::da::Expression* e_intersection = new te::da::ST_Intersection( new te::da::PropertyName(m_inFirstDsetType->getName() + "." + firstGeom->getName()),
                                                                    new te::da::PropertyName(m_inSecondDsetType->getName() + "." + secondGeom->getName()));
  te::da::Field* f_intersection = new te::da::Field(*e_intersection, "geom");
  fields->push_back(f_intersection);

  te::da::FromItem* firstFromItem = new te::da::DataSetName(m_inFirstDsetType->getName());
  te::da::FromItem* secondFromItem = new te::da::DataSetName(m_inSecondDsetType->getName());
  te::da::Expression* e_intersects = new te::da::ST_Intersects( new te::da::PropertyName(m_inFirstDsetType->getName() + "." + firstGeom->getName()),
                                                                new te::da::PropertyName(m_inSecondDsetType->getName() + "." + secondGeom->getName()));
  te::da::JoinConditionOn* on = new te::da::JoinConditionOn(e_intersects);
  te::da::Join* join = new te::da::Join(*firstFromItem, *secondFromItem, te::da::INNER_JOIN, *on);

  te::da::From* from = new te::da::From;
  from->push_back(join);

  select->setFields(fields);
  select->setFrom(from);

//Where clause if the object ids were selected.
  te::da::Where* w_oid = 0;
  if(m_firstOidSet && m_secondOidSet)
  {
    te::da::And* exp_and = new te::da::And(m_firstOidSet->getExpressionByInClause(m_inFirstDsetType->getName()), m_secondOidSet->getExpressionByInClause(m_inSecondDsetType->getName()));
    w_oid = new te::da::Where(exp_and);
  }
  else if(m_firstOidSet)
    w_oid = new te::da::Where(m_firstOidSet->getExpressionByInClause(m_inFirstDsetType->getName()));
  else if(m_secondOidSet)
    w_oid = new te::da::Where(m_secondOidSet->getExpressionByInClause(m_inSecondDsetType->getName()));
  
  select->setWhere(w_oid);

  std::auto_ptr<te::da::DataSet> dsQuery = m_inFirstDsrc->query(select);
  dsQuery->moveBeforeFirst();

  if(dsQuery->size() == 0)
    throw te::common::Exception(TE_TR("The Layers do not intersect!"));


  std::auto_ptr<te::da::DataSetType> outDataSetType(new te::da::DataSetType(m_outDsetName));

  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(m_outDsetName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  outDataSetType->add(pkProperty);
  
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDsetName + "_pk", outDataSetType.get());
  pk->add(pkProperty);
  outDataSetType->setPrimaryKey(pk);

  for(size_t i = 0; i < firstProps.size(); ++i)
  {
    te::dt::Property* prop = firstProps[i]->clone();
    if(!m_inFirstDsetType->getTitle().empty())
      prop->setName(te::vp::GetSimpleTableName(m_inFirstDsetType->getTitle()) + "_" + prop->getName());
    outDataSetType->add(prop);
  }

  for(size_t i = 0; i < secondProps.size(); ++i)
  {
    te::dt::Property* prop = secondProps[i]->clone();
    prop->setName(te::vp::GetSimpleTableName(m_inSecondDsetType->getTitle()) + "_" + prop->getName());
    outDataSetType->add(prop);
  }

  te::gm::GeomType newType = te::vp::GeomOpResultType(te::da::GetFirstGeomProperty(m_inFirstDsetType.get())->getGeometryType(), 
                                                      te::da::GetFirstGeomProperty(m_inSecondDsetType.get())->getGeometryType());

  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
  newGeomProp->setGeometryType(newType);
  newGeomProp->setSRID(te::da::GetFirstGeomProperty(m_inFirstDsetType.get())->getSRID());
  
  outDataSetType->add(newGeomProp);

  std::auto_ptr<te::da::DataSet> outDset(this->updateGeomType(outDataSetType.get(), dsQuery.release()));

  outDset->moveBeforeFirst();

  te::vp::Save(m_outDsrc.get(), outDset.get(), outDataSetType.get());
  return true;

}

te::da::DataSet* te::vp::IntersectionQuery::updateGeomType(te::da::DataSetType* dsType, te::da::DataSet* ds)
{
  te::mem::DataSet* dsMem = new te::mem::DataSet(dsType);
  
  std::size_t type = 0;
  std::vector<te::dt::Property*> props = dsType->getProperties();

  std::size_t pk = 0;
  while(ds->moveNext())
  {
    std::string propName;
    te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem);
    
    dsItem->setInt32(0, pk);
  
    for(std::size_t i = 1; i < props.size(); ++i)
    {
      type = props[i]->getType();
      propName = props[i]->getName();

      if(type != te::dt::GEOMETRY_TYPE)
      {
        if (!ds->isNull(propName))
        {
          std::auto_ptr<te::dt::AbstractData> value = ds->getValue(propName);
          dsItem->setValue(i, value.release());
        }
      }
      else
      {
        std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(propName);
        std::auto_ptr<te::gm::GeometryProperty> geomProp((te::gm::GeometryProperty*)props[i]->clone());

        if(geomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::PolygonType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if(geomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::LineStringType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiLineStringType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
        else if(geomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if(geom->getGeomTypeId() == te::gm::MultiPointType)
          {
            dsItem->setGeometry(i, geom.release());
          }
          else if(geom->getGeomTypeId() == te::gm::PointType)
          {
            std::auto_ptr<te::gm::GeometryCollection> newGeom(new te::gm::GeometryCollection(0, te::gm::MultiPointType, geom->getSRID()));
            newGeom->add(geom.release());
            dsItem->setGeometry(i, newGeom.release());
          }
        }
      }
    }
    ++pk;
    dsMem->add(dsItem);
  }
  return dsMem;
}
