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
  \file AggregationQuery.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/query/Avg.h"
#include "../dataaccess/query/Count.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/GroupBy.h"
#include "../dataaccess/query/GroupByItem.h"
#include "../dataaccess/query/Max.h"
#include "../dataaccess/query/Min.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Sub.h"
#include "../dataaccess/query/Sum.h"
#include "../dataaccess/query/StdDev.h"
#include "../dataaccess/query/ST_Union.h"
#include "../dataaccess/query/Variance.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"

#include "AggregationQuery.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::vp::AggregationQuery::AggregationQuery()
{}

te::vp::AggregationQuery::~AggregationQuery()
{}

bool te::vp::AggregationQuery::run() throw(te::common::Exception)
{
  std::auto_ptr<te::da::DataSetType> outDSetType(new te::da::DataSetType(m_outDset));
  
// Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(m_outDset + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  outDSetType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDset + "_pk", outDSetType.get());
  pk->add(pkProperty);
  outDSetType->setPrimaryKey(pk);

  // include the grouping properties in the query
  te::da::Fields* fields = new te::da::Fields;
  for(std::size_t i=0; i<m_groupProps.size(); ++i)
  {
    te::da::Field* f_aggName = new te::da::Field(m_groupProps[i]->getName());
    fields->push_back(f_aggName);
    
    te::dt::Property* propclone = m_groupProps[i]->clone();
    outDSetType->add(propclone);
  }
  
  // number of objects in each group (mandatory)
  te::da::Expression* e_aggCount = new te::da::Count(new te::da::PropertyName(m_groupProps[0]->getName()));
  te::da::Field* f_aggCount = new te::da::Field(*e_aggCount, "NUM_OBJ");
  fields->push_back(f_aggCount);
  
  te::dt::SimpleProperty* countProp = new te::dt::SimpleProperty("NUM_OBJ", te::dt::INT32_TYPE);
  outDSetType->add(countProp);
  
  // build the query expression according to the summarization requested
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itStat = m_statSum.begin();
  
  while(itStat != m_statSum.end())
  {
    std::vector<te::stat::StatisticalSummary>::const_iterator itFunc = itStat->second.begin();
    while (itFunc != itStat->second.end())
    {
      te::da::PropertyName* p_name = new te::da::PropertyName(itStat->first->getName());
      int p_type = itStat->first->getType();
      te::dt::Property* newProp = 0;
      
      te::da::Expression *sexp, *e_max, *e_min;
      te::da::Field*  sfield;
      switch (*itFunc)
      {
        case MIN_VALUE:
          sexp = new te::da::Min(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_MIN_VALUE");
          if (p_type == te::dt::STRING_TYPE)
            newProp = new te::dt::StringProperty(p_name->getName() + "_MIN_VALUE");
          else
            newProp = new te::dt::SimpleProperty(p_name->getName() + "_MIN_VALUE", p_type);
          break;
        case MAX_VALUE:
          sexp = new te::da::Max(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_MAX_VALUE");
          if (p_type == te::dt::STRING_TYPE)
            newProp = new te::dt::StringProperty(p_name->getName() + "_MAX_VALUE");
          else
            newProp = new te::dt::SimpleProperty(p_name->getName() + "_MAX_VALUE", p_type);
          break;
        case MEAN:
          sexp = new te::da::Avg(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_MEAN");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_MEAN", te::dt::DOUBLE_TYPE);
          break;
        case SUM:
          sexp = new te::da::Sum(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_SUM");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_SUM", p_type);
          break;
        case COUNT:
          sexp = new te::da::Count(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_COUNT");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_COUNT", te::dt::INT32_TYPE);
          break;
        case STANDARD_DEVIATION:
          sexp = new te::da::StdDev(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_STANDARD_DEVIATION");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_STANDARD_DEVIATION", te::dt::DOUBLE_TYPE);
          break;
        case VARIANCE:
          sexp = new te::da::Variance(p_name);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_VARIANCE");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_VARIANCE", te::dt::DOUBLE_TYPE);
          break;
        case AMPLITUDE:
          e_min = new te::da::Min(p_name);
          e_max = new te::da::Max(p_name);
          sexp = new te::da::Sub(*e_max, *e_min);
          sfield = new te::da::Field(*sexp, p_name->getName() + "_AMPLITUDE");
          newProp = new te::dt::SimpleProperty(p_name->getName() + "_AMPLITUDE", p_type);
          break;
        default:
          ++itFunc;
          continue;
      }
      fields->push_back(sfield);
      outDSetType->add(newProp);
      ++itFunc;
    }
    ++itStat;
  }
  
  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inDsetType.get());
    
  te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(geom->getName()));
  te::da::Field* f_union = new te::da::Field(*e_union, "geom");
  fields->push_back(f_union);
  
  // define the resulting spatial property
  te::gm::GeometryProperty* p = static_cast<te::gm::GeometryProperty*>(m_inDsetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
  
  // creates the output geometry property
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  te::gm::GeomType geotype = getGeomResultType(p->getGeometryType());
  geometry->setGeometryType(geotype);
  geometry->setSRID(p->getSRID());
  outDSetType->add(geometry);
  
  te::da::FromItem* fromItem = new te::da::DataSetName(m_inDsetType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);
  
  te::da::Where* w_oid = 0;

  if(m_oidSet)
    w_oid = new te::da::Where(m_oidSet->getExpression());

  te::da::Select select(fields, from, w_oid);
  
  te::da::GroupBy* groupBy = new te::da::GroupBy();
  for(std::size_t i=0; i<m_groupProps.size(); ++i)
  {
    te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(m_groupProps[i]->getName());
    groupBy->push_back(e_groupBy);
  }
  select.setGroupBy(groupBy);
  
  std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(select);


  if (dsQuery->isEmpty())
    return false;
  
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDSetType.get()));

  int key = 0;
  dsQuery->moveBeforeFirst();
  while (dsQuery->moveNext())
  {
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDSet.get());
    outDSetItem->setInt32(0, key);

    for (size_t i=1; i<outDSetType->size(); ++i)
    {
      switch (outDSetType->getProperty(i)->getType())
      {
        case te::dt::STRING_TYPE:
          if (!dsQuery->isNull(i-1))
            outDSetItem->setString(i,dsQuery->getAsString(i-1));
          break;
        case te::dt::INT32_TYPE:
          if (!dsQuery->isNull(i-1))
            outDSetItem->setInt32(i, boost::lexical_cast<int>(dsQuery->getAsString(i-1)));
          break;
        case te::dt::INT64_TYPE:
          if (!dsQuery->isNull(i-1))
            outDSetItem->setInt64(i,dsQuery->getInt64(i-1));
          break;
        case te::dt::DOUBLE_TYPE:
          if (!dsQuery->isNull(i-1))
            outDSetItem->setDouble(i,dsQuery->getDouble(i-1));
          break;
        case te::dt::NUMERIC_TYPE:
          if (!dsQuery->isNull(i-1))
            outDSetItem->setNumeric(i,dsQuery->getNumeric(i-1));
          break;
        case te::dt::GEOMETRY_TYPE:
          if (!dsQuery->isNull(i-1))
          {
            std::auto_ptr<te::gm::Geometry> agg_geo(dsQuery->getGeometry(i-1));
            agg_geo->setSRID(p->getSRID());

            if (agg_geo->getGeomTypeId() != geotype)
            {
              te::gm::GeometryCollection* gc = new te::gm::GeometryCollection(1,geotype,agg_geo->getSRID());
              gc->setGeometryN(0, agg_geo.release());
              outDSetItem->setGeometry("geom", gc);
            }
            else
              outDSetItem->setGeometry("geom", agg_geo.release());
          }
          break;
        default:
          continue;
      }
    }
    outDSet->add(outDSetItem);
    ++key;
  }
  
  te::vp::Save(m_outDsrc.get(), outDSet.get(), outDSetType.get());
  return true;
}
