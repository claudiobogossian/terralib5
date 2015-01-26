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
#include "../datasource/DataSourceCapabilities.h"
#include "../utils/Utils.h"
#include "../Exception.h"
#include "And.h"
#include "AttributeRestrictionVisitor.h"
#include "DataSetName.h"
#include "Select.h"
#include "Field.h"
#include "Fields.h"
#include "From.h"
#include "FromItem.h"
#include "LiteralEnvelope.h"
#include "PropertyName.h"
#include "Or.h"
#include "QueryCapabilities.h"
#include "SpatialRestrictionVisitor.h"
#include "ST_EnvelopeIntersects.h"
#include "SpatialQueryProcessor.h"
#include "Where.h"

// STL
#include <cassert>
#include <set>
#include <vector>

te::da::SpatialQueryProcessor::SpatialQueryProcessor()
{
}

te::da::SpatialQueryProcessor::~SpatialQueryProcessor()
{
}

std::auto_ptr<te::da::DataSet> te::da::SpatialQueryProcessor::getDataSet(const DataSourcePtr& ds, const Select& q, te::common::TraverseType travType)
{
  // Gets the datasource query capabilities
  const QueryCapabilities& capabilities = ds->getCapabilities().getQueryCapabilities();

  // Gets a data source transactor
  std::auto_ptr<DataSourceTransactor> t = ds->getTransactor();

  return getDataSet(t.get(), capabilities, q, travType, false);
}

std::auto_ptr<te::da::DataSet> te::da::SpatialQueryProcessor::getDataSet(DataSourceTransactor* t, const QueryCapabilities& capabilities,
                                                                         const Select& q, te::common::TraverseType travType, bool connected)
{
  assert(t);

  // Try find the spatial restrictions
  SpatialRestrictionVisitor srv;
  q.accept(srv);

  if(!srv.hasSpatialRestrictions())
    return t->query(q, travType, connected);

  // Gets the spatial restrictions
  const std::vector<SpatialRestriction*>& restrictions = srv.getSpatialRestrictions();

  if(supportsSpatialTopologicOperatos(capabilities, restrictions))
    return t->query(q, travType, connected);

  // Generates the oids
  std::auto_ptr<ObjectIdSet> oids = getOIDSet(t, q);
  assert(oids.get());

  if(oids->size() == 0)
    throw(Exception(TE_TR("The query result is empty.")));

  // Gets the dataset name
  std::string datasetName = getDataSetName(q);
  assert(!datasetName.empty());

  return t->getDataSet(datasetName, oids.get(), travType, connected);
}

std::auto_ptr<te::da::ObjectIdSet> te::da::SpatialQueryProcessor::getOIDSet(const DataSourcePtr& ds, const Select& q)
{
  // Gets the datasource query capabilities
  const QueryCapabilities& capabilities = ds->getCapabilities().getQueryCapabilities();

  // Gets a data source transactor
  std::auto_ptr<DataSourceTransactor> t = ds->getTransactor();

  return getOIDSet(t.get(), capabilities, q);
}

std::auto_ptr<te::da::ObjectIdSet> te::da::SpatialQueryProcessor::getOIDSet(DataSourceTransactor* t, const QueryCapabilities& capabilities, const Select& q)
{
  assert(t);

  // Gets the dataset name
  std::string datasetName = getDataSetName(q);
  assert(!datasetName.empty());

  // Gets the dataset type
  std::auto_ptr<DataSetType> type = t->getDataSetType(datasetName);
  assert(type.get());

  // Try find the spatial restrictions
  SpatialRestrictionVisitor srv;
  q.accept(srv);

  if(!srv.hasSpatialRestrictions() && supportsSpatialTopologicOperatos(capabilities, srv.getSpatialRestrictions()))
  {
    // Gets the dataset
    std::auto_ptr<DataSet> dataset = t->query(q, te::common::FORWARDONLY);
    assert(dataset.get());

    // Generates the oids
    std::auto_ptr<ObjectIdSet> oids(GenerateOIDSet(dataset.get(), type.get()));

    return oids;
  }

  // Gets the query from
  const From* from = q.getFrom();
  assert(from);
  assert(from->size() == 1);
  assert(!from->is_null(0));

  // Gets the query fields
  const Fields* fields = q.getFields();
  assert(fields);

  // The base select
  Select baseSelect;
  baseSelect.setFields(fields->clone().release());
  baseSelect.setFrom(from->clone().release());

   // Attribute Restrictions
  te::da::Expression* attrRestrictions = getAttrRestrictions(q);

  // Gets the spatial restriction
  const std::vector<SpatialRestriction*>& restrictions = srv.getSpatialRestrictions();

  // The result
  ObjectIdSet* oids = 0;
  GetEmptyOIDSet(type.get(), oids);
  assert(oids);

  std::vector<te::gm::Geometry*> geomRestrictions;
  for(std::size_t i = 0; i < restrictions.size(); ++i)
    geomRestrictions.push_back(restrictions[i]->m_geometry);

  for(std::size_t i = 0; i < restrictions.size(); ++i)
    oids->Union(getOIDSet(t, baseSelect, attrRestrictions, restrictions[i], type.get(), geomRestrictions));

  return std::auto_ptr<te::da::ObjectIdSet>(oids);
}

std::auto_ptr<te::da::ObjectIdSet> te::da::SpatialQueryProcessor::getOIDSet(DataSourceTransactor* t, const Select& q)
{
  assert(t);

  // Gets the dataset name
  std::string datasetName = getDataSetName(q);
  assert(!datasetName.empty());

  // Gets the dataset type
  std::auto_ptr<DataSetType> type = t->getDataSetType(datasetName);
  assert(type.get());

  // Finds the spatial restrictions
  SpatialRestrictionVisitor srv;
  q.accept(srv);

  assert(srv.hasSpatialRestrictions());

  // Gets the query from
  const From* from = q.getFrom();
  assert(from);
  assert(from->size() == 1);
  assert(!from->is_null(0));

  // Gets the query fields
  const Fields* fields = q.getFields();
  assert(fields);

  // The base select
  Select baseSelect;
  baseSelect.setFields(fields->clone().release());
  baseSelect.setFrom(from->clone().release());

  // Attribute Restrictions
  te::da::Expression* attrRestrictions = getAttrRestrictions(q);

  // Gets the spatial restriction
  const std::vector<SpatialRestriction*>& restrictions = srv.getSpatialRestrictions();

  // The result
  ObjectIdSet* oids = 0;
  GetEmptyOIDSet(type.get(), oids);
  assert(oids);

  std::vector<te::gm::Geometry*> geomRestrictions;
  for(std::size_t i = 0; i < restrictions.size(); ++i)
    geomRestrictions.push_back(restrictions[i]->m_geometry);

  for(std::size_t i = 0; i < restrictions.size(); ++i)
    oids->Union(getOIDSet(t, baseSelect, attrRestrictions, restrictions[i], type.get(), geomRestrictions));

  return std::auto_ptr<te::da::ObjectIdSet>(oids);
}

te::da::ObjectIdSet* te::da::SpatialQueryProcessor::getOIDSet(DataSourceTransactor* t, Select& baseSelect, te::da::Expression* attrRestrictions,
                                                              SpatialRestriction* restriction, const DataSetType* type,
                                                              const std::vector<te::gm::Geometry*>& geomRestrictions)
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
  ST_EnvelopeIntersects* eIntersects = new ST_EnvelopeIntersects(pname, lenv);

  // Adds the restriction to the base select
  if(attrRestrictions == 0)
    baseSelect.setWhere(new Where(eIntersects)); // only extent restriction
  else
    baseSelect.setWhere(new Where(new And(attrRestrictions, eIntersects))); // attribute restrictions AND extent restriction

  // Retrieves the dataset using the envelope restriction (the candidates)
  std::auto_ptr<DataSet> dataset(t->query(baseSelect, te::common::FORWARDONLY));
  assert(dataset.get());

  // The result
  ObjectIdSet* oids = 0;
  GetEmptyOIDSet(type, oids);
  assert(oids);

  // The property names that compose the oid set
  const std::vector<std::string>& pnames = oids->getPropertyNames();

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

    bool equalsToRestriction = false;
    for(std::size_t i = 0; i < geomRestrictions.size(); ++i)
    {
      if(currentGeom->equals(geomRestrictions[i]))
      {
        equalsToRestriction = true;
        break;
      }
    }

    if(equalsToRestriction)
      continue;

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

bool te::da::SpatialQueryProcessor::supportsSpatialTopologicOperatos(const QueryCapabilities& capabilities,
                                                                     const std::vector<SpatialRestriction*>& restrictions) const
{
  const std::set<std::string>& ops = capabilities.getSpatialTopologicOperators();

  for(std::size_t i = 0; i < restrictions.size(); ++i)
  {
    SpatialRestriction* sr = restrictions[i];

    const std::string& fname = sr->m_function->getName();

    if(ops.find(fname) == ops.end())
      return false;
  }

  return true;
}

std::string te::da::SpatialQueryProcessor::getDataSetName(const Select& q) const
{
  // TODO: verify and throw the exceptions!

  // Gets the query from
  const From* from = q.getFrom();
  assert(from);
  assert(from->size() == 1);
  assert(!from->is_null(0));

  // Gets the first from item to extract the dataset name
  const DataSetName* fromItem = dynamic_cast<const DataSetName*>(&from->at(0));

  return fromItem->getName();
}

te::da::Expression* te::da::SpatialQueryProcessor::getAttrRestrictions(const Select& q) const
{
  // Try find the attribute restrictions
  AttributeRestrictionVisitor arv;
  q.accept(arv);

  if(!arv.hasAttributeRestrictions())
    return 0;

  const std::vector<AttributeRestriction*> attrRestrictions = arv.getAttributeRestrictions();
  if(attrRestrictions.size() == 1)
    return attrRestrictions[0]->m_function->clone();

  assert(attrRestrictions.size() >= 2);

  // For while, using the AND operator
  And* andOp = new And(attrRestrictions[0]->m_function->clone(), attrRestrictions[1]->m_function->clone());
  Expression* result = andOp;
  for(std::size_t i = 2; i < attrRestrictions.size(); ++i)
  {
    And* nextAndOp = new And(result, attrRestrictions[i]->m_function->clone());
    result = nextAndOp;
  }

  return result;
}
