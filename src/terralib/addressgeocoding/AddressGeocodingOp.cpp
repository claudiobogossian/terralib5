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
 \file AddressGeocodingOp.cpp
 */

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry/GeometryProperty.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LineString.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/Utils.h"

#include "AddressGeocodingOp.h"

// BOOST
#include <boost/lexical_cast.hpp>

te::addressgeocoding::AddressGeocodingOp::AddressGeocodingOp():
  m_outDsetName("")
{
}

void te::addressgeocoding::AddressGeocodingOp::setInput(te::da::DataSourcePtr inDsrc,
                                                        std::string inDsetName,
                                                        int inSRID,
                                                        te::da::DataSourcePtr inAddressDsrc,
                                                        std::string inAddressDsetName)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inSRID = inSRID;
  m_inAddressDsrc = inAddressDsrc;
  m_inAddressDsetName = inAddressDsetName;
}

void te::addressgeocoding::AddressGeocodingOp::setParams( std::vector<std::string> associatedProps,
                                                          std::string streetNumber)
{
  m_associatedProps = associatedProps;
  m_streetNumber = streetNumber;
}

void te::addressgeocoding::AddressGeocodingOp::setNumAttributes(std::string initialLeft,
                                                                std::string finalLeft,
                                                                std::string initialRight,
                                                                std::string finalRight)
{
  m_initialLeft = initialLeft;
  m_finalLeft = finalLeft;
  m_initialRight = initialRight;
  m_finalRight = finalRight;
}

void te::addressgeocoding::AddressGeocodingOp::setOutput(te::da::DataSourcePtr outDsrc, std::string outDsetName)
{
  m_outDsrc = outDsrc;
  m_outDsetName = outDsetName;
}

bool te::addressgeocoding::AddressGeocodingOp::paramsAreValid()
{
  if (!m_inDsrc.get() || !m_inAddressDsrc.get())
    return false;
  if (m_inDsetName.empty() || m_inAddressDsetName.empty())
    return false;
  if (m_associatedProps.empty())
    return false;
  if (!m_outDsrc.get())
    return false;
  if (m_outDsetName.empty())
    return false;

  return true;
}

bool te::addressgeocoding::AddressGeocodingOp::run()
{
// Input layer encoding
  te::common::CharEncoding dSourceEncoding = m_inDsrc->getEncoding();
// Input layer DataSetType
  std::auto_ptr<te::da::DataSetType> inDataSetType = m_inDsrc->getDataSetType(m_inDsetName);

// Input file DataSet
  std::auto_ptr<te::da::DataSet> dsAddress = m_inAddressDsrc->getDataSet(m_inAddressDsetName);
  dsAddress->moveBeforeFirst();

// Output
std::auto_ptr<te::da::DataSetType> outDsType = getOutputDataSetType();
std::auto_ptr<te::mem::DataSet> dataSetMemory(new te::mem::DataSet(outDsType.get()));

  while(dsAddress->moveNext())
  {
    std::string query = "SELECT * FROM ";
    query += m_inDsetName + " WHERE tsvector @@ plainto_tsquery('english', ";

    for(std::size_t i = 0; i < m_associatedProps.size(); ++i)
    {
      //te::common::CharEncoding addressEncoding = dsAddress->getPropertyCharEncoding(i);
      te::common::CharEncoding addressEncoding = te::common::CP1252;
      std::string value;

      if(dSourceEncoding == addressEncoding)
        value = dsAddress->getAsString(m_associatedProps[i]);
      else
        value = te::common::CharEncodingConv::convert(dsAddress->getAsString(m_associatedProps[i]), addressEncoding, dSourceEncoding);

      if(i == 0)
        query+= "'"+value;
      else
        query+= " | "+value;
    }
    query+= "')";

    //Add condition in Where clause, filter result by street number.
    if( m_streetNumber != "")
    {
      if(m_initialLeft != "" && m_finalLeft != "" && m_initialRight != "" && m_finalRight != "")
      {
        query+= " AND (" + dsAddress->getAsString(m_streetNumber) + " > " + m_initialLeft + " OR " + dsAddress->getAsString(m_streetNumber) + " > " + m_initialRight + ")"
                " AND (" +  dsAddress->getAsString(m_streetNumber) + " < " + m_finalLeft + " OR " + dsAddress->getAsString(m_streetNumber) + " < " + m_finalRight + ")";
      }
      else if((m_initialLeft != "" && m_finalLeft != "" && m_initialRight == "" && m_finalRight == "")||
              (m_initialLeft != "" && m_finalLeft != "" && m_initialRight != "" && m_finalRight == "")||
              (m_initialLeft != "" && m_finalLeft != "" && m_initialRight == "" && m_finalRight != ""))
      {
        query+= " AND (" + dsAddress->getAsString(m_streetNumber) + " > " + m_initialLeft + " AND " + dsAddress->getAsString(m_streetNumber) + " < " + m_finalLeft + ")";
      }
      else if((m_initialLeft == "" && m_finalLeft == "" && m_initialRight != "" && m_finalRight != "")||
              (m_initialLeft == "" && m_finalLeft != "" && m_initialRight != "" && m_finalRight != "")||
              (m_initialLeft != "" && m_finalLeft == "" && m_initialRight != "" && m_finalRight != ""))
      {
        query+= " AND (" + dsAddress->getAsString(m_streetNumber) + " > " + m_initialRight + " AND " + dsAddress->getAsString(m_streetNumber) + " < " + m_finalRight + ")";
      }
    }
    else
    {
      te::dt::Property* gid = inDataSetType->getProperty(0);
      query += " ORDER BY " + gid->getName();
    }

// Submit the search query.
    std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(query);
    dsQuery->moveFirst();



// Building Address Geocoding.
    te::mem::DataSetItem* itemMemory = new te::mem::DataSetItem(dataSetMemory.get());

    if(dsQuery->size() > 0)
    {
      std::vector<te::dt::Property*> vecProps = inDataSetType->getProperties();
      double initVal = boost::lexical_cast<double>(dsAddress->getDouble(m_streetNumber));
      double finalVal = boost::lexical_cast<double>(dsAddress->getDouble(m_streetNumber));
      double aux_val;
      int geomPos;

      for(std::size_t i = 0; i < vecProps.size(); ++i)
      {
        if(vecProps[i]->getType() != te::dt::GEOMETRY_TYPE && vecProps[i]->getName() != "tsvector")
        {
          if(dsQuery->isNull(i) == false)
            itemMemory->setValue(vecProps[i]->getName(), dsQuery->getValue(i).release());

          if(vecProps[i]->getName() == m_initialLeft || vecProps[i]->getName() == m_initialRight)
          {
            int type = vecProps[i]->getType();

            switch (type)
            {
              case te::dt::INT16_TYPE:
              {
                aux_val = dsQuery->getInt16(i);
                if(aux_val <= initVal)
                  initVal = aux_val;
              }
              break;
              case te::dt::INT32_TYPE:
              {
                aux_val = dsQuery->getInt32(i);
                if(aux_val <= initVal)
                  initVal = aux_val;
              }
              break;
              case te::dt::INT64_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getInt64(i));
                if(aux_val <= initVal)
                  initVal = aux_val;
              }
              break;
              case te::dt::DOUBLE_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getDouble(i));
                if(aux_val <= initVal)
                  initVal = aux_val;
              }
              break;
              case te::dt::NUMERIC_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getNumeric(i));
                if(aux_val <= initVal)
                  initVal = aux_val;
              }
              break;
              default:
                break;
            }
          }
          if(vecProps[i]->getName() == m_finalLeft || vecProps[i]->getName() == m_finalRight)
          {
            int type = vecProps[i]->getType();

            switch (type)
            {
              case te::dt::INT16_TYPE:
              {
                aux_val = dsQuery->getInt16(i);
                if(aux_val >= finalVal)
                  finalVal = aux_val;
              }
              break;
              case te::dt::INT32_TYPE:
              {
                aux_val = dsQuery->getInt32(i);
                if(aux_val >= finalVal)
                  finalVal = aux_val;
              }
              break;
              case te::dt::INT64_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getInt64(i));
                if(aux_val >= finalVal)
                  finalVal = aux_val;
              }
              break;
              case te::dt::DOUBLE_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getDouble(i));
                if(aux_val >= finalVal)
                  finalVal = aux_val;
              }
              break;
              case te::dt::NUMERIC_TYPE:
              {
                aux_val = boost::lexical_cast<double>(dsQuery->getNumeric(i).c_str());
                if(aux_val >= finalVal)
                  finalVal = aux_val;
              }
              break;
              default:
                break;
            }
          }
        }
        else if (vecProps[i]->getType() == te::dt::GEOMETRY_TYPE)
        {
          geomPos = i;
        }
      }
      
      std::auto_ptr<te::gm::Geometry> geom = dsQuery->getGeometry(geomPos);

      te::gm::LineString* lineString = getLineString(geom.get());

      if(!lineString)
        continue;

      int numTarget = boost::lexical_cast<int>(dsAddress->getDouble(m_streetNumber));

      te::gm::Coord2D* target = te::gm::locateAlong(lineString, initVal, finalVal, numTarget);
      te::gm::Point* point = new te::gm::Point(target->getX(), target->getY(), m_inSRID);
      itemMemory->setValue(geomPos, point);

      dataSetMemory->add(itemMemory);
    }
    else
    {
      //Address not found.
    }

  }

  // save the result
  return save(dataSetMemory,outDsType);
}

bool  te::addressgeocoding::AddressGeocodingOp::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType.get(), m_outDsrc->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(result.get(), converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  m_outDsrc->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  result->moveBeforeFirst();
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    m_outDsrc->addPrimaryKey(m_outDsetName,pk);
  }
  
  return true;
}

std::auto_ptr<te::da::DataSetType> te::addressgeocoding::AddressGeocodingOp::getOutputDataSetType()
{
  std::auto_ptr<te::da::DataSetType> inDsType = m_inDsrc->getDataSetType(m_inDsetName);
  std::auto_ptr<te::da::DataSetType> outDsType(new te::da::DataSetType(m_outDsetName));

  std::vector<te::dt::Property*> vecProps = inDsType->getProperties();

  for(std::size_t i = 0; i < vecProps.size(); ++i)
  {
    if(vecProps[i]->getType() != te::dt::GEOMETRY_TYPE && vecProps[i]->getName() != "tsvector")
    {
      outDsType->add(vecProps[i]->clone());
    }
    else if(vecProps[i]->getType() == te::dt::GEOMETRY_TYPE)
    {
      te::gm::GeometryProperty* p = dynamic_cast<te::gm::GeometryProperty*>(vecProps[i]);

      te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
      geometry->setGeometryType(te::gm::PointType);
      geometry->setSRID(p->getSRID());

      outDsType->add(geometry);
    }
  }

  return outDsType;
}

te::gm::LineString* te::addressgeocoding::AddressGeocodingOp::getLineString(te::gm::Geometry* geom)
{
  te::gm::LineString* line = 0;
  std::vector<te::gm::LineString*> vecLines;

  getLines(geom, vecLines);

  if(vecLines.size() > 0)
    line = vecLines[0];

  for(std::size_t i = 1; i < vecLines.size(); ++i)
    line->Union(vecLines[i]);

  return line;
}

void te::addressgeocoding::AddressGeocodingOp::getLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines)
{
  if(geom == 0)
    return;

  switch(geom->getGeomTypeId())
  {
    case te::gm::MultiLineStringType:
      getLines(dynamic_cast<te::gm::GeometryCollection*>(geom), lines);
    break;

    case te::gm::LineStringType:
      getLines(dynamic_cast<te::gm::LineString*>(geom), lines);
    break;

    default:
      return;
  }
}

void te::addressgeocoding::AddressGeocodingOp::getLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines)
{
  assert(gc);

  for(std::size_t i = 0; i < gc->getNumGeometries(); ++i)
    getLines(gc->getGeometryN(i), lines);
}

void te::addressgeocoding::AddressGeocodingOp::getLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines)
{
  assert(l);
  lines.push_back(l);
}