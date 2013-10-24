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
  \file terralib/dataaccess/query/SpatialQueryProcessor.cpp

  \brief A basic query processor for spatial restrictions.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/Utils.h"
#include "../../srs/Config.h"
#include "../dataset/DataSet.h"
#include "../dataset/FilteredDataSet.h"
#include "../datasource/DataSourceTransactor.h"
#include "../utils/Utils.h"
#include "../Exception.h"
#include "AttributeRestrictionVisitor.h"
#include "DataSetName.h"
#include "Select.h"
#include "Field.h"
#include "Fields.h"
#include "From.h"
#include "FromItem.h"
#include "LiteralEnvelope.h"
#include "PropertyName.h"
#include "SpatialRestrictionVisitor.h"
#include "ST_Intersects.h"
#include "SpatialQueryProcessor.h"
#include "Where.h"

// STL
#include <cassert>
#include <vector>

te::da::SpatialQueryProcessor::SpatialQueryProcessor()
{
}

te::da::SpatialQueryProcessor::~SpatialQueryProcessor()
{
}

std::auto_ptr<te::da::DataSet> te::da::SpatialQueryProcessor::getDataSet(const DataSourcePtr& ds, const Select& q, te::common::TraverseType travType)
{
  std::auto_ptr<DataSourceTransactor> t = ds->getTransactor();

  return getDataSet(t.get(), q, travType, false);
}

std::auto_ptr<te::da::DataSet> te::da::SpatialQueryProcessor::getDataSet(DataSourceTransactor* t, const Select& q,
                                                                         te::common::TraverseType travType, bool connected)
{
  assert(t);

  // Try find the spatial restrictions
  SpatialRestrictionVisitor srv;
  q.accept(srv);

  if(!srv.hasSpatialRestrictions())
    return t->query(q, travType, connected);

  const Fields* fields = q.getFields();
  assert(fields);

  const From* from = q.getFrom();
  assert(from);

  // The base select
  Select baseSelect;
  baseSelect.setFields(fields->clone().release());
  baseSelect.setFrom(from->clone().release());

  // Gets the spatial restriction
  const std::vector<SpatialRestriction*>& restrictions = srv.getSpatialRestrictions();

  for(std::size_t i = 0; i < restrictions.size(); ++i)
  {
    SpatialRestriction* sr = restrictions[i];

    te::gm::Geometry* geomRestriction = sr->m_geometry;
    assert(geomRestriction);
    
    // Gets the geometry mbr
    const te::gm::Envelope* genv = geomRestriction->getMBR();
    assert(genv);

    // Creates the ST_Intersects
    LiteralEnvelope* lenv = new LiteralEnvelope(new te::gm::Envelope(*genv), geomRestriction->getSRID());
    PropertyName* pname = new PropertyName(sr->m_pname);
    ST_Intersects* intersects = new ST_Intersects(pname, lenv);

    // Create the query restriction
    baseSelect.setWhere(new Where(intersects));

    // Retrieves the dataset using the envelope restriction
    std::auto_ptr<DataSet> dataset(t->query(baseSelect));
    assert(dataset.get());

    // The vector of positions that actually obey the restriction
    std::vector<std::size_t> positions;

    std::size_t currentPos = 0;
    while(dataset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> currentGeom(0);
      try
      {
        currentGeom = dataset->getGeometry(sr->m_pname);
        if(currentGeom.get() == 0)
          continue;
      }
      catch(std::exception& /*e*/)
      {
        continue;
      }

      assert(currentGeom.get());

      if(te::gm::SatisfySpatialRelation(currentGeom.get(), geomRestriction, sr->m_type))
        positions.push_back(currentPos);

      ++currentPos;
    }

    dataset->moveBeforeFirst();

    // Create the filtered data set with the found positions
    std::auto_ptr<DataSet> filtered(new FilteredDataSet(dataset.release(), positions, true));

    // TODO: For while considering the first spatial restriction!
    return filtered;
  }

  throw Exception("Under Development!");
}

std::auto_ptr<te::da::ObjectIdSet> te::da::SpatialQueryProcessor::getOIDSet(const DataSourcePtr& ds, const Select& q)
{
  std::auto_ptr<DataSourceTransactor> t = ds->getTransactor();

  return getOIDSet(t.get(), q);
}

std::auto_ptr<te::da::ObjectIdSet> te::da::SpatialQueryProcessor::getOIDSet(DataSourceTransactor* t, const Select& q)
{
  assert(t);

  // Gets the query from
  const From* from = q.getFrom();
  assert(from);
  assert(from->size() == 1);
  assert(!from->is_null(0));

  // Gets the first from item to extract the dataset name
  const DataSetName* fromItem = dynamic_cast<const DataSetName*>(&from->at(0));
  assert(fromItem);

  const std::string& datasetName = fromItem->getName();
  assert(!datasetName.empty());

  // Gets the dataset type
  std::auto_ptr<DataSetType> type = t->getDataSetType(datasetName);
  assert(type.get());

  // Try find the spatial restrictions
  SpatialRestrictionVisitor srv;
  q.accept(srv);

  if(!srv.hasSpatialRestrictions())
  {
    // Gets the dataset
    std::auto_ptr<DataSet> dataset = t->query(q, te::common::FORWARDONLY);
    assert(dataset.get());

    // Generates the oids
    std::auto_ptr<ObjectIdSet> oids(GenerateOIDSet(dataset.get(), type.get()));

    return oids;
  }

  // Gets the query fields
  const Fields* fields = q.getFields();
  assert(fields);

  // The base select
  Select baseSelect;
  baseSelect.setFields(fields->clone().release());
  baseSelect.setFrom(from->clone().release());

  // Gets the spatial restriction
  const std::vector<SpatialRestriction*>& restrictions = srv.getSpatialRestrictions();

  // The result
  ObjectIdSet* oids = 0;
  GetEmptyOIDSet(type.get(), oids);
  assert(oids);

  for(std::size_t i = 0; i < restrictions.size(); ++i)
    oids->Union(getOIDSet(t, baseSelect, restrictions[i], type.get()));

  return std::auto_ptr<te::da::ObjectIdSet>(oids);
}

te::da::ObjectIdSet* te::da::SpatialQueryProcessor::getOIDSet(DataSourceTransactor* t, Select& baseSelect,
                                                              SpatialRestriction* restriction, const DataSetType* type)
{
  assert(t);
  assert(restriction);
  assert(type);

  // Gets the geometry restriction
  te::gm::Geometry* geomRestriction = restriction->m_geometry;
  assert(geomRestriction);

  // Gets the geometry mbr
  const te::gm::Envelope* genv = geomRestriction->getMBR();
  assert(genv);

  // Creates the ST_Intersects
  LiteralEnvelope* lenv = new LiteralEnvelope(new te::gm::Envelope(*genv), geomRestriction->getSRID());
  PropertyName* pname = new PropertyName(restriction->m_pname);
  ST_Intersects* intersects = new ST_Intersects(pname, lenv);

  // Adds the restriction to the base select
  baseSelect.setWhere(new Where(intersects));

  // Retrieves the dataset using the envelope restriction (the candidates)
  std::auto_ptr<DataSet> dataset(t->query(baseSelect, te::common::FORWARDONLY));
  assert(dataset.get());

  // The result
  ObjectIdSet* oids = 0;
  GetEmptyOIDSet(type, oids);
  assert(oids);

  // The property names that compose the oid set
  const std::vector<std::string>& pnames = oids->getPropertyNames();

  std::size_t size = dataset->size();

  while(dataset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> currentGeom(0);
    try
    {
      currentGeom = dataset->getGeometry(restriction->m_pname);
      if(currentGeom.get() == 0)
        continue;
    }
    catch(std::exception& /*e*/)
    {
      continue;
    }

    assert(currentGeom.get());

    if(!te::gm::SatisfySpatialRelation(currentGeom.get(), geomRestriction, restriction->m_type))
      continue;

    // Satisfy! Generates the oid to this current dataset element
    oids->add(GenerateOID(dataset.get(), pnames));
  }

  return oids;
}

te::gm::Envelope* te::da::SpatialQueryProcessor::computeEnvelope(const std::vector<SpatialRestriction*>& restrictions) const
{
  te::gm::Envelope* result = new te::gm::Envelope;

  for(std::size_t i = 0; i < restrictions.size(); ++i)
  {
    SpatialRestriction* sr = restrictions[i];

    te::gm::Geometry* geom = sr->m_geometry;
    assert(geom);

    result->Union(*geom->getMBR());
  }

  return result;
}
