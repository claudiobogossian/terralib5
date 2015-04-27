/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/st/maptools/DataSetLayer.cpp

  \brief A layer with reference to an ObservationDataSet.
*/

// TerraLib

#include "../../dataaccess/query/DataSetName.h"
#include "../../dataaccess/query/Field.h"
#include "../../dataaccess/query/Fields.h"
#include "../../dataaccess/query/From.h"
#include "../../dataaccess/query/FromItem.h"
#include "../../dataaccess/query/Select.h"
#include "../../dataaccess/query/SpatialQueryProcessor.h"
#include "../../dataaccess/query/Where.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/Exception.h"
#include "../../maptools/RendererFactory.h"
#include "../core/observation/ObservationDataSet.h"
#include "../core/observation/ObservationDataSetInfo.h"
#include "../loader/STDataLoader.h"
#include "ObservationDataSetLayer.h"

// Boost
#include <boost/format.hpp>

const std::string te::st::ObservationDataSetLayer::sm_type("OBSERVATIONDATASETLAYER");

te::st::ObservationDataSetLayer::ObservationDataSetLayer(AbstractLayer* parent, ObservationDataSetInfo* info)
  : AbstractSTDataSetLayer(parent),
    m_info(info)
{
}

te::st::ObservationDataSetLayer::ObservationDataSetLayer(const std::string& id, AbstractLayer* parent, ObservationDataSetInfo* info)
  : AbstractSTDataSetLayer(id, parent),
    m_info(info)
{
}

te::st::ObservationDataSetLayer::ObservationDataSetLayer(const std::string& id, 
                            const std::string& title, AbstractLayer* parent, 
                             ObservationDataSetInfo* info)
  : AbstractSTDataSetLayer(id, title, parent),
    m_info(info)
{
}

te::st::ObservationDataSetLayer::~ObservationDataSetLayer()
{
}

std::auto_ptr<te::map::LayerSchema> te::st::ObservationDataSetLayer::getSchema() const
{
  assert(!m_info->getDataSetName().empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_info->getDataSourceInfo().getId(), true);

  return ds->getDataSetType(m_info->getDataSetName());
}

te::dt::DateTimePeriod* te::st::ObservationDataSetLayer::getTemporalExtent() const
{
  return 0;
  //return m_info->getTemporalExtent();
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData(te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(*e, r, travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(*g, r, travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData(te::da::Expression* restriction,
  te::common::TraverseType travType,
  const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::da::DataSet> result;
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData(const te::da::ObjectIdSet* oids,
  te::common::TraverseType travType,
  const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::da::DataSet> result;
  return result;
}

std::auto_ptr<te::st::ObservationDataSet> te::st::ObservationDataSetLayer::getObservationDataset(te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), travType);
}

std::auto_ptr<te::st::ObservationDataSet> te::st::ObservationDataSetLayer::getObservationDataset(const te::gm::Envelope& e,
                                                                                              te::gm::SpatialRelation r,
                                                                                              te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), e, r, travType);
}

std::auto_ptr<te::st::ObservationDataSet> te::st::ObservationDataSetLayer::getObservationDataset(const te::gm::Geometry& g,
                                                                                              te::gm::SpatialRelation r,
                                                                                              te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), g, r, travType);
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                        te::common::TraverseType travType, 
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(dt, r, travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
  const te::gm::Envelope& e, te::gm::SpatialRelation sr,
  te::common::TraverseType travType,
  te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(dt, tr, e, sr, travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                        const te::gm::Geometry& g, te::gm::SpatialRelation sr ,
                                                                        te::common::TraverseType travType,
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::ObservationDataSet> ods = te::st::ObservationDataSetLayer::getObservationDataset(dt, tr, g, sr, travType);
  std::auto_ptr<te::da::DataSet> result = ods->release();
  return result;
}

std::auto_ptr<te::st::ObservationDataSet> te::st::ObservationDataSetLayer::getObservationDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                                                 te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(), dt, r, travType);
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::ObservationDataSetLayer::getObservationDataset(const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                       const te::gm::Envelope& e, te::gm::SpatialRelation sr,
                                                       te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info, dt, tr, e, sr, travType);
}

std::auto_ptr<te::st::ObservationDataSet> te::st::ObservationDataSetLayer::getObservationDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                                                 const te::gm::Geometry& g, te::gm::SpatialRelation sr,
                                                                                                 te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(),g, sr,  dt, tr, travType);
}

bool te::st::ObservationDataSetLayer::isValid() const
{
  return true;
}

void te::st::ObservationDataSetLayer::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<te::map::AbstractRenderer> renderer(te::map::RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::st::ObservationDataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::st::ObservationDataSetLayer::getDataSourceId() const
{
  return m_info->getDataSourceInfo().getId();
}

const std::string& te::st::ObservationDataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::st::ObservationDataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

