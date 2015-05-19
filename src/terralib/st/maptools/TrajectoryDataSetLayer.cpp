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
  \file terralib/st/maptools/TrajectoryDataSetLayer.cpp

  \brief A layer with a reference to an TrajectoryDataSet. 
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
#include "../../datatype/DateTime.h"
#include "../../datatype/DateTimePeriod.h"
#include "../../maptools/Exception.h"
#include "../../maptools/RendererFactory.h"
#include "../core/observation/ObservationDataSet.h"
#include "../core/trajectory/TrajectoryDataSetInfo.h"
#include "../core/trajectory/TrajectoryDataSet.h"
#include "../loader/STDataLoader.h"
#include "TrajectoryDataSetLayer.h"

// Boost
#include <boost/format.hpp>

const std::string te::st::TrajectoryDataSetLayer::sm_type("TRAJECTORYDATASETLAYER");

te::st::TrajectoryDataSetLayer::TrajectoryDataSetLayer(AbstractLayer* parent)
  : AbstractSTDataSetLayer(parent)
{
}

te::st::TrajectoryDataSetLayer::TrajectoryDataSetLayer(AbstractLayer* parent, TrajectoryDataSetInfo* info)
  : AbstractSTDataSetLayer(parent),
    m_info(info)
{
}

te::st::TrajectoryDataSetLayer::TrajectoryDataSetLayer(const std::string& id, AbstractLayer* parent, TrajectoryDataSetInfo* info)
  : AbstractSTDataSetLayer(id, parent),
    m_info(info)
{
}

te::st::TrajectoryDataSetLayer::TrajectoryDataSetLayer(const std::string& id, 
                            const std::string& title, AbstractLayer* parent, 
                             TrajectoryDataSetInfo* info)
  : AbstractSTDataSetLayer(id, title, parent),
    m_info(info)
{
}

te::st::TrajectoryDataSetLayer::~TrajectoryDataSetLayer()
{
}

std::auto_ptr<te::map::LayerSchema> te::st::TrajectoryDataSetLayer::getSchema() const
{
  assert(!m_info->getObservationDataSetInfo().getDataSetName().empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_info->getObservationDataSetInfo().getDataSourceInfo().getId(), true);

  return ds->getDataSetType(m_info->getObservationDataSetInfo().getDataSetName());
}

te::dt::DateTimePeriod* te::st::TrajectoryDataSetLayer::getTemporalExtent() const
{
  return 0;
  //return te::st::STDataLoader::getDataSet(*m_info.get())->getTemporalExtent();
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData(te::common::TraverseType travType,
                                                                       const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Envelope* e,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(*e, r, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData(const std::string& propertyName,
                                                              const te::gm::Geometry* g,
                                                              te::gm::SpatialRelation r,
                                                              te::common::TraverseType travType,
                                                              const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(*g, r, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData(te::da::Expression* restriction,
                                        te::common::TraverseType travType,
                                        const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::da::DataSet> result;
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData(const te::da::ObjectIdSet* oids,
                                                                       te::common::TraverseType travType,
                                                                       const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::da::DataSet> result;
  return result;
}

std::auto_ptr<te::st::TrajectoryDataSet> te::st::TrajectoryDataSetLayer::getTrajectoryDataset(te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), travType);
}

std::auto_ptr<te::st::TrajectoryDataSet> te::st::TrajectoryDataSetLayer::getTrajectoryDataset(const te::gm::Envelope& e,
                                                                                              te::gm::SpatialRelation r,
                                                                                              te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), e, r, travType);
}

std::auto_ptr<te::st::TrajectoryDataSet> te::st::TrajectoryDataSetLayer::getTrajectoryDataset(const te::gm::Geometry& g,
                                                                                              te::gm::SpatialRelation r,
                                                                                              te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), g, r, travType);
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                        te::common::TraverseType travType, 
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(dt, r, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                        const te::gm::Envelope& e, te::gm::SpatialRelation sr,
                                                                        te::common::TraverseType travType,
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(dt, tr, e, sr, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                        const te::gm::Geometry& g, te::gm::SpatialRelation sr ,
                                                                        te::common::TraverseType travType,
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::TrajectoryDataSet> tds = te::st::TrajectoryDataSetLayer::getTrajectoryDataset(dt, tr, g, sr, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::st::TrajectoryDataSet> te::st::TrajectoryDataSetLayer::getTrajectoryDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                                               te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(), dt, r, travType);
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::TrajectoryDataSetLayer::getTrajectoryDataset(const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                     const te::gm::Envelope& e, te::gm::SpatialRelation sr,
                                                     te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(), dt, tr, e, sr, travType);
}

std::auto_ptr<te::st::TrajectoryDataSet> te::st::TrajectoryDataSetLayer::getTrajectoryDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                                               const te::gm::Geometry& g, te::gm::SpatialRelation sr,
                                                                                               te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(), g, sr,  dt, tr, travType);
}

bool te::st::TrajectoryDataSetLayer::isValid() const
{
  if(m_info->getObservationDataSetInfo().getDataSourceInfo().getId().empty())
    return false;

  te::da::DataSourcePtr ds;
  try
  {
    ds = te::da::GetDataSource(m_info->getObservationDataSetInfo().getDataSourceInfo().getId(), true);
  }
  catch(...)
  {
    return false;
  }

  if(ds.get() == 0 || !ds->isValid() || !ds->isOpened())
    return false;

  return true;
}

void te::st::TrajectoryDataSetLayer::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<te::map::AbstractRenderer> renderer(te::map::RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::st::TrajectoryDataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::st::TrajectoryDataSetLayer::getDataSourceId() const
{
  return m_info->getObservationDataSetInfo().getDataSourceInfo().getId();
}


const std::string& te::st::TrajectoryDataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::st::TrajectoryDataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

bool te::st::TrajectoryDataSetLayer::hasChilds()
{
  if (m_layers.empty())
    return false;
  else
    return true;
}