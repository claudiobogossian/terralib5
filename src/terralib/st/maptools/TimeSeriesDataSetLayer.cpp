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
  \file terralib/st/maptools/TimeSeriesDataSetLayer.cpp

  \brief A layer with a reference to an TimeSeriesDataSet. 
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
#include "../core/timeseries/TimeSeriesDataSetInfo.h"
#include "../core/timeseries/TimeSeriesDataSet.h"
#include "../loader/STDataLoader.h"
#include "TimeSeriesDataSetLayer.h"

// Boost
#include <boost/format.hpp>

const std::string te::st::TimeSeriesDataSetLayer::sm_type("TimeSeriesDataSetLayer");

te::st::TimeSeriesDataSetLayer::TimeSeriesDataSetLayer(AbstractLayer* parent, TimeSeriesDataSetInfo* info)
  : AbstractSTDataSetLayer(parent),
    m_info(info)
{
}

te::st::TimeSeriesDataSetLayer::TimeSeriesDataSetLayer(const std::string& id, AbstractLayer* parent, TimeSeriesDataSetInfo* info)
  : AbstractSTDataSetLayer(id, parent),
    m_info(info)
{
}

te::st::TimeSeriesDataSetLayer::TimeSeriesDataSetLayer(const std::string& id, 
                            const std::string& title, AbstractLayer* parent, 
                             TimeSeriesDataSetInfo* info)
  : AbstractSTDataSetLayer(id, title, parent),
    m_info(info)
{
}

te::st::TimeSeriesDataSetLayer::~TimeSeriesDataSetLayer()
{
}

std::auto_ptr<te::map::LayerSchema> te::st::TimeSeriesDataSetLayer::getSchema() const
{
  assert(!m_info->getObservationDataSetInfo().getDataSetName().empty());

  te::da::DataSourcePtr ds = te::da::GetDataSource(m_info->getDataSourceInfo().getId(), true);

  return ds->getDataSetType(m_info->getObservationDataSetInfo().getDataSetName());
}

te::dt::DateTimePeriod* te::st::TimeSeriesDataSetLayer::getTemporalExtent() const
{
  return te::st::STDataLoader::getDataSet(*m_info.get())->getTemporalExtent();
}

std::auto_ptr<te::da::DataSet> te::st::TimeSeriesDataSetLayer::getData(te::common::TraverseType travType,
                                                                       const te::common::AccessPolicy accessPolicy) const
{
  std::auto_ptr<te::st::TimeSeriesDataSet> tds = te::st::TimeSeriesDataSetLayer::getTimeSeriesDataset(travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::st::TimeSeriesDataSet> te::st::TimeSeriesDataSetLayer::getTimeSeriesDataset(te::common::TraverseType travType) const
{
  return te::st::STDataLoader::getDataSet(*m_info.get(), travType);
}

std::auto_ptr<te::da::DataSet> te::st::TimeSeriesDataSetLayer::getData( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                        te::common::TraverseType travType, 
                                                                        te::common::AccessPolicy rwRole) const
{
  std::auto_ptr<te::st::TimeSeriesDataSet> tds = te::st::TimeSeriesDataSetLayer::getTimeSeriesDataset(dt, r, travType);
  std::auto_ptr<te::da::DataSet> result = tds->release();
  return result;
}

std::auto_ptr<te::st::TimeSeriesDataSet> te::st::TimeSeriesDataSetLayer::getTimeSeriesDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                                                                               te::common::TraverseType travType) const
{
 return te::st::STDataLoader::getDataSet(*m_info.get(), dt, r, travType);
}

bool te::st::TimeSeriesDataSetLayer::isValid() const
{
  if(m_info->getDataSourceInfo().getId().empty())
    return false;

  te::da::DataSourcePtr ds;
  try
  {
    ds = te::da::GetDataSource(m_info->getDataSourceInfo().getId(), true);
  }
  catch(...)
  {
    return false;
  }

  if(ds.get() == 0 || !ds->isValid() || !ds->isOpened())
    return false;

  return true;
}

void te::st::TimeSeriesDataSetLayer::draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid)
{
  if(m_rendererType.empty())
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer type is empty!")) % getTitle()).str());

  // Try get the defined renderer
  std::auto_ptr<te::map::AbstractRenderer> renderer(te::map::RendererFactory::make(m_rendererType));
  if(renderer.get() == 0)
    throw te::map::Exception((boost::format(TE_TR("Could not draw the data set layer %1%. The renderer %2% could not be created!")) % getTitle() % m_rendererType).str());

  renderer->draw(this, canvas, bbox, srid);
}

const std::string& te::st::TimeSeriesDataSetLayer::getType() const
{
  return sm_type;
}

const std::string& te::st::TimeSeriesDataSetLayer::getRendererType() const
{
  return m_rendererType;
}

void te::st::TimeSeriesDataSetLayer::setRendererType(const std::string& t)
{
  m_rendererType = t;
}

bool te::st::TimeSeriesDataSetLayer::hasChilds()
{
  if (m_layers.empty())
    return false;
  else
    return true;
}