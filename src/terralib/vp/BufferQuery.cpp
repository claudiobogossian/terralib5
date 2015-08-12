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
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Buffer.h"
#include "../dataaccess/query/ST_Difference.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../sam/rtree.h"

#include "BufferQuery.h"
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

te::vp::BufferQuery::BufferQuery()
{}

te::vp::BufferQuery::~BufferQuery()
{}

bool te::vp::BufferQuery::run() throw(te::common::Exception)
{
  te::da::Fields* fields = new te::da::Fields;

  if(m_copyInputColumns)
  {
    std::vector<te::dt::Property*> props = m_converter->getResult()->getProperties();
    for(std::size_t i=0; i < props.size(); ++i)
    {
      if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
      {
        te::da::Field* f_props = new te::da::Field(te::da::PropertyName(props[i]->getName()));
        fields->push_back(f_props);
      }
    }
  }

  te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_converter->getResult());

  te::da::Expression* e_buffer = 0;
  te::da::Expression* e_aux = 0;
  te::da::Field* f_buffer = 0;
  

  for(int i=1; i <= m_levels; ++i)
  {
    std::stringstream ss;
    ss << i;
    std::string index = ss.str();

    //buffer
    if(m_bufferPolygonRule == te::vp::INSIDE_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), m_distance*i);
      te::da::Expression* e_buffer2 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -m_distance*i);
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else if(m_bufferPolygonRule == te::vp::ONLY_OUTSIDE)
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), m_distance*i);
      te::da::Expression* e_buffer2 = new te::da::PropertyName(geom->getName());
      e_buffer = new te::da::ST_Difference(e_buffer1, e_buffer2);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }
    else
    {
      te::da::Expression* e_buffer1 = new te::da::ST_Buffer(te::da::PropertyName(geom->getName()), -m_distance*i);
      te::da::Expression* e_buffer2 = new te::da::PropertyName(geom->getName());
      e_buffer = new te::da::ST_Difference(e_buffer2, e_buffer1);
      f_buffer = new te::da::Field(*e_buffer, "geom"+index);
    }

    if(!e_aux)
    {
      fields->push_back(f_buffer);
      e_aux = e_buffer;
    }
    else
    {
      te::da::Expression* e_diff = new te::da::ST_Difference(e_buffer, e_aux);
      f_buffer = new te::da::Field(*e_diff, "geom"+index);
      fields->push_back(f_buffer);
      e_aux = e_buffer;
    }
  }

  te::da::FromItem* fromItem = new te::da::DataSetName(m_converter->getResult()->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  te::da::Where* w_oid = 0;

  if(m_oidSet)
  {
    w_oid = new te::da::Where(m_oidSet->getExpression());
  }

  te::da::Select select(fields, from, w_oid);
  std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(select);

  std::auto_ptr<te::da::DataSetType> outDSType(GetDataSetType());
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDSType.get()));

  if(m_bufferBoundariesRule == te::vp::DISSOLVE)
  {
    std::vector<std::vector<te::gm::Geometry*> > vecDissolvedGeom = dissolveQuery(dsQuery.get(), m_levels);
    prepareDataSet(outDSType.get(), vecDissolvedGeom, outDSet.get(), m_distance);
  }
  else
  {
    prepareDataSet(outDSType.get(), dsQuery.get(), outDSet.get(), m_distance);
  }

// Converter to use SRID from layer.
  te::gm::GeometryProperty* geomPropertyLayer = te::da::GetFirstGeomProperty(m_converter->getResult());
  int layerSRID = geomPropertyLayer->getSRID();
  
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDSType.get(), m_outDsrc->getCapabilities(), m_outDsrc->getEncoding());
  te::da::AssociateDataSetTypeConverterSRID(converter, layerSRID);
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(outDSet.get(), converter));

  te::vp::Save(m_outDsrc.get(), dsAdapter.get(), dsTypeResult);
  return true;

}

std::vector<std::vector<te::gm::Geometry*> > te::vp::BufferQuery::dissolveQuery(te::da::DataSet* dsQuery,
                                                                                const int& levels)
{
  std::map<int, std::vector<te::gm::Geometry*> > mapGeom;
  std::vector<std::vector<te::gm::Geometry*> > vecGeom;

  for(int i = 0; i < levels; ++i)
  {
    te::sam::rtree::Index<te::gm::Geometry*, 4> rtree;
      
    dsQuery->moveBeforeFirst();
    while(dsQuery->moveNext())
    {
      te::gm::Geometry* geom = dsQuery->getGeometry(i).release();
          
      std::vector<te::gm::Geometry*> vec;
          
      rtree.search(*(geom->getMBR()), vec);
          
      if(!vec.empty())
      {
        for(std::size_t t = 0; t < vec.size(); ++t)
        {
          if(geom->intersects(vec[t]))
          {
            geom = geom->Union(vec[t]);
            rtree.remove(*(vec[t]->getMBR()), vec[t]);
          }
        }
      }
      rtree.insert(*(geom->getMBR()), geom);
    }
      
    std::vector<te::gm::Geometry*> geomVec;
    std::auto_ptr<te::gm::Envelope> e = dsQuery->getExtent(i);
    rtree.search(*(e.get()), geomVec);
    vecGeom.push_back(geomVec);

    rtree.clear();
  }

  std::size_t vecSize = vecGeom.size();

  if(levels > 1)
  {
    for(std::size_t i = vecSize - 1; i > 0; --i)
    {
      std::vector<te::gm::Geometry*> currentVec = vecGeom[i];
      std::size_t c_vecSize = currentVec.size();

      for(std::size_t j = 0; j < i; ++j)
      {
        std::vector<te::gm::Geometry*> innerVec = vecGeom[j];
        std::size_t i_vecSize = innerVec.size();

        for(std::size_t k = 0; k < c_vecSize; ++k)
        {
          for(std::size_t l = 0; l < i_vecSize; ++l)
          {
            te::gm::Geometry* k_geom = currentVec[k];
            te::gm::Geometry* l_geom = innerVec[l];

            if(k_geom->intersects(l_geom))
            {
              te::gm::Geometry* tGeom = k_geom->difference(l_geom);
              if(tGeom->isValid())
              {
                delete currentVec[k];
                currentVec[k] = tGeom;
                vecGeom[i] = currentVec;
              }
            }
          }
        }
      }
    }
  }

  return vecGeom;
}

void te::vp::BufferQuery::prepareDataSet(te::da::DataSetType* dataSetType, 
                                        te::da::DataSet* dataSetQuery,
                                        te::mem::DataSet* outputDataSet,
                                        const double& distance)
{
  std::size_t numProps = dataSetQuery->getNumProperties();
  std::size_t firstGeomPos = te::da::GetFirstSpatialPropertyPos(dataSetQuery);
  int numItems = (int)numProps - (int)firstGeomPos;
  int pk = 0;
  dataSetQuery->moveBeforeFirst();
  
  unsigned int type;

  while(dataSetQuery->moveNext())
  {
    int numCurrentItem = 0;

    for(int i = 0; i < numItems; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);

      for(std::size_t j = 0; j < numProps; ++j)
      {
        type = dataSetQuery->getPropertyDataType(j);

        switch (type){
          case te::dt::INT32_TYPE:
            dataSetItem->setInt32(j+3, dataSetQuery->getInt32(j));
            break;
          case te::dt::INT64_TYPE:
            dataSetItem->setInt64(j+3, dataSetQuery->getInt64(j));
            break;
          case te::dt::DOUBLE_TYPE:
            dataSetItem->setDouble(j+3, dataSetQuery->getDouble(j));
            break;
          case te::dt::STRING_TYPE:
            dataSetItem->setString(j+3, dataSetQuery->getString(j));
            break;
          case te::dt::GEOMETRY_TYPE:
            {
              dataSetItem->setInt32(0, pk); //pk
              dataSetItem->setInt32(1, i+1); //level
              dataSetItem->setDouble(2, distance*(i+1)); //distance
            
              std::auto_ptr<te::gm::Geometry> geom = dataSetQuery->getGeometry(j+numCurrentItem);
              if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
              {
                dataSetItem->setGeometry(j+3, geom.release());
              }
              else
              {
                std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
                mPolygon->add(geom.release());
                dataSetItem->setGeometry(j+3, mPolygon.release());
              }

              outputDataSet->add(dataSetItem);

              ++numCurrentItem;
              ++pk;
              j = numProps;
            }
            break;
          default:
#ifdef TERRALIB_LOGGER_ENABLED
            te::common::Logger::logDebug("vp", "Buffer - Type not found.");
#endif //TERRALIB_LOGGER_ENABLED
            break;
        }
      }
    }
  }
}

void te::vp::BufferQuery::prepareDataSet(te::da::DataSetType* dataSetType, 
                                        std::vector<std::vector<te::gm::Geometry*> > vecDissolvedGeom,
                                        te::mem::DataSet* outputDataSet,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                const double& distance)
{
  int pk = 0;
  
  for(std::size_t i = 0; i < vecDissolvedGeom.size(); ++i)
  {
    std::vector<te::gm::Geometry*> vecGeom = vecDissolvedGeom[i];
    std::size_t sizeVecGeom = vecGeom.size();

    for(std::size_t j=0; j < sizeVecGeom; ++j)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outputDataSet);
      dataSetItem->setInt32(0, pk); //pk
      dataSetItem->setInt32(1, (int)i+1); //level
      dataSetItem->setDouble(2, distance*((int)i+1)); //distance

      std::auto_ptr<te::gm::Geometry> geom(vecGeom[j]);

      if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
      {
        dataSetItem->setGeometry(3, geom.release());
      }
      else
      {
        std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, geom->getSRID()));
        mPolygon->add(geom.release());
        dataSetItem->setGeometry(3, mPolygon.release());
      }
    
      outputDataSet->add(dataSetItem);

      ++pk;
    }
  }
}