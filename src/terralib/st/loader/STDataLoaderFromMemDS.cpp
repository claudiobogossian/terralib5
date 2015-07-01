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
  \file STDataLoaderFromMemDS.cpp

  \brief  This file contains a class responsible for loading spatiotemporal data 
          from a ST in-memory DataSource which contains an internal chache of 
          in-memory DataSets indexed by space and time. 
*/

//TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/DateTimePeriod.h"
#include "../../geometry/Utils.h"

//STMEM
#include "../../stmemory/DataSource.h"
#include "../../stmemory/DataSet.h"

//ST
#include "../Exception.h"
#include "../Utils.h"
#include "../Globals.h"
#include "STDataLoaderFromMemDS.h"
#include "../core/observation/ObservationDataSetInfo.h"
#include "../core/trajectory/TrajectoryDataSetInfo.h"
#include "../core/timeseries/TimeSeriesDataSetInfo.h"
#include "../core/observation/ObservationDataSet.h"
#include "../core/trajectory/TrajectoryDataSet.h"
#include "../core/timeseries/TimeSeriesDataSet.h"

//STL
#include <vector>

te::st::STDataLoaderFromMemDS::STDataLoaderFromMemDS()
{
  //use the DataSourceManager to get the DataSource 
  m_ds = te::da::DataSourceManager::getInstance().find(te::st::Globals::sm_STMemoryDataSourceId);
  
  if(m_ds.get()==0)
    throw Exception("The STDataLoader is not inialized! Please, use the method STDataLoader::initialize"); 

  if(!m_ds->isOpened())
    m_ds->open();
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const ObservationDataSetInfo& info, 
                                 te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getDataSetName(), travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info); 
}


std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Envelope& e,
                          te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  if(!info.hasGeomProp())
    return std::auto_ptr<te::st::ObservationDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  //get the geometry property name
  std::string geomPropName = info.getGeomPropName();

  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getDataSetName(), geomPropName, &e, r, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);  
}
        
std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Geometry& geom, 
                          te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  if(!info.hasGeomProp())
    return std::auto_ptr<te::st::ObservationDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  //get the geometry property name
  std::string geomPropName = info.getGeomPropName();

  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getDataSetName(), geomPropName, &geom, r, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info); 
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const te::st::ObservationDataSetInfo& info, 
                          const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(info.getDataSetName(), &dt, r, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info); 
}

std::auto_ptr<te::st::ObservationDataSet>
te::st::STDataLoaderFromMemDS::getDataSet(const te::st::ObservationDataSetInfo& info,
                                          const te::dt::DateTime& dt, 
                                          te::dt::TemporalRelation tr,
                                          const te::gm::Envelope& e, 
                                          te::gm::SpatialRelation sr,
                                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getDataSetName(), &e, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info); 
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const te::st::ObservationDataSetInfo& info, 
                          const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                          const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(info.getDataSetName(), &geom, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

void 
te::st::STDataLoaderFromMemDS::getInfo( const TrajectoryDataSetInfo& /*input*/, std::vector<TrajectoryDataSetInfo>& /*output*/)
{
  return;
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info, te::common::TraverseType travType) 
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get the data set
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info,  
                             const te::gm::Geometry& geom, te::gm::SpatialRelation r,
                             te::common::TraverseType travType)
{
  if(!info.getObservationDataSetInfo().hasGeomProp())
    return std::auto_ptr<te::st::TrajectoryDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get data set type to get the geometry property name
  std::string geomPropName = info.getObservationDataSetInfo().getGeomPropName();
  
  //get the data set, applying the spatial filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                      geomPropName, &geom, r, travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TrajectoryDataSet>
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info, 
                                const te::gm::Envelope& e, te::gm::SpatialRelation r,               
                                te::common::TraverseType travType)
{
  if(!info.getObservationDataSetInfo().hasGeomProp())
    return std::auto_ptr<te::st::TrajectoryDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get data set type to get the geometry property name
  std::string geomPropName = info.getObservationDataSetInfo().getGeomPropName();
  
  //get the data set, applying the spatial filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                      geomPropName, &e, r, travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info, 
                          const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &dt, r, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info,
                                          const te::dt::DateTime& dt, 
                                          te::dt::TemporalRelation tr,
                                          const te::gm::Envelope& e, 
                                          te::gm::SpatialRelation sr,
                                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &e, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TrajectoryDataSet>  
te::st::STDataLoaderFromMemDS::getDataSet(const TrajectoryDataSetInfo& info, 
                          const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                          const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &geom, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

void te::st::STDataLoaderFromMemDS::getInfo( const TimeSeriesDataSetInfo& /*input*/, 
                      std::vector<TimeSeriesDataSetInfo>& /*output*/)
{
  return;
}
        
std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info, 
                      te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get the data set
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info,  
                             const te::gm::Geometry& geom, te::gm::SpatialRelation r,
                             te::common::TraverseType travType)
{
  if(!info.getObservationDataSetInfo().hasGeomProp())
    return std::auto_ptr<te::st::TimeSeriesDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get data set type to get the geometry property name
  std::string geomPropName = info.getObservationDataSetInfo().getGeomPropName();
  
  //get the data set, applying the spatial filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                      geomPropName, &geom, r, travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TimeSeriesDataSet>
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info, 
                                const te::gm::Envelope& e, te::gm::SpatialRelation r,               
                                te::common::TraverseType travType)
{
  if(!info.getObservationDataSetInfo().hasGeomProp())
    return std::auto_ptr<te::st::TimeSeriesDataSet>();
  
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  //get data set type to get the geometry property name
  std::string geomPropName = info.getObservationDataSetInfo().getGeomPropName();
  
  //get the data set, applying the spatial filter
  std::auto_ptr<te::da::DataSet> dset(m_ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                      geomPropName, &e, r, travType));
  
  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info, 
                          const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &dt, r, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info,
                                          const te::dt::DateTime& dt, 
                                          te::dt::TemporalRelation tr,
                                          const te::gm::Envelope& e, 
                                          te::gm::SpatialRelation sr,
                                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &e, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::st::TimeSeriesDataSet>  
te::st::STDataLoaderFromMemDS::getDataSet(const TimeSeriesDataSetInfo& info, 
                          const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                          const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                          te::common::TraverseType travType)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  std::auto_ptr<te::da::DataSet> dset(inMemDataSource->getDataSet(
    info.getObservationDataSetInfo().getDataSetName(), &geom, sr, &dt, tr, travType));

  return buildDataSet(static_cast<te::stmem::DataSet*>(dset.release()), info);
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromMemDS::getTemporalExtent(const ObservationDataSetInfo& info)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  return inMemDataSource->getTemporalExtent(info.getDataSetName());
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromMemDS::getTemporalExtent(const TrajectoryDataSetInfo& info)
{
   //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  return inMemDataSource->getTemporalExtent(info.getObservationDataSetInfo().getDataSetName());
}
        
std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromMemDS::getTemporalExtent(const TimeSeriesDataSetInfo& info)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  te::stmem::DataSource* inMemDataSource = static_cast<te::stmem::DataSource*>(m_ds.get());

  return inMemDataSource->getTemporalExtent(info.getObservationDataSetInfo().getDataSetName());
}

te::gm::Envelope 
te::st::STDataLoaderFromMemDS::getSpatialExtent(const ObservationDataSetInfo& info)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getDataSetName())) 
    loadDataSet(info.getDataSourceInfo(), info.getDataSetName(), info.getBeginTimePropName(), 
                info.getEndTimePropName(), info.getGeomPropName()); 

  std::auto_ptr<te::gm::Envelope> res(m_ds->getExtent(info.getDataSetName(), info.getGeomPropName()));
  return *res.get();
}

te::gm::Envelope 
te::st::STDataLoaderFromMemDS::getSpatialExtent(const TrajectoryDataSetInfo& info)
{
  //load the DataSet from its origin DataSource and put it into the In-Mem DataSource
  if(!m_ds->dataSetExists(info.getObservationDataSetInfo().getDataSetName())) 
    loadDataSet(info.getObservationDataSetInfo().getDataSourceInfo(), info.getObservationDataSetInfo().getDataSetName(), 
                info.getObservationDataSetInfo().getBeginTimePropName(), 
                info.getObservationDataSetInfo().getEndTimePropName(), 
                info.getObservationDataSetInfo().getGeomPropName()); 

  std::auto_ptr<te::gm::Envelope> res(m_ds->getExtent(info.getObservationDataSetInfo().getDataSetName(), 
                                     info.getObservationDataSetInfo().getGeomPropName()));
  return *res.get();
}

//protected
void te::st::STDataLoaderFromMemDS::loadDataSet(const te::da::DataSourceInfo& info, const std::string dsname, 
                                                const std::string& begTimePropName, const std::string& endTimePropName, 
                                                const std::string& gmPropName)
{
  std::auto_ptr<te::da::DataSet> dset(te::da::GetDataSet(dsname, info.getId()));
  if(!dset.get())
    throw Exception("The DataSet was not loaded correctly!"); 

  std::auto_ptr<te::da::DataSetType> dsettype(te::da::GetDataSetType(dsname, info.getId()));
  if(!dsettype.get())
    throw Exception("The DataSetType was not loaded correctly!"); 
  
  //Get indexes
  int begTimePropIdx = te::da::GetPropertyPos(dsettype.get(), begTimePropName);
  int endTimePropIdx = te::da::GetPropertyPos(dsettype.get(), endTimePropName);
  int gmPropIdx = te::da::GetPropertyPos(dsettype.get(), gmPropName);

  //Before creating a stmem DataSet, we need to put it in a right place to be copied.
  dset->moveNext();
  te::stmem::DataSet* inMemdset = new te::stmem::DataSet(dset.get(), begTimePropIdx, endTimePropIdx, gmPropIdx, 0);
  
  //Add them into the in-mem data source
  te::stmem::DataSource* memDS = static_cast<te::stmem::DataSource*>(m_ds.get());
  memDS->add(dsname, dsettype.release(), inMemdset);
}

std::auto_ptr<te::st::ObservationDataSet>  
te::st::STDataLoaderFromMemDS::buildDataSet(te::stmem::DataSet* ds, const ObservationDataSetInfo& info)
{
  //get type from info
  ObservationDataSetType obsType = te::st::GetType(info);

  //get temporal and spatial extents
  std::auto_ptr<te::dt::DateTimePeriod> period(ds->getTemporalExtent()); 
  std::auto_ptr<te::gm::Geometry> geom; 
  if(info.hasGeomProp())
  {
    std::size_t idx = te::da::GetPropertyPos(ds, info.getGeomPropName());
    std::auto_ptr<te::gm::Envelope> aux = ds->getExtent(idx);
    geom.reset(te::gm::GetGeomFromEnvelope(aux.get(), -1));
  }
  
  obsType.setSpatialExtent(geom.release());
  obsType.setTemporalExtent(period.release());
  return std::auto_ptr<ObservationDataSet> (new ObservationDataSet(ds, obsType)); 
}

std::auto_ptr<te::st::TrajectoryDataSet>  
te::st::STDataLoaderFromMemDS::buildDataSet(te::stmem::DataSet* ds, const TrajectoryDataSetInfo& tjInfo)
{
  //get type from info
  const ObservationDataSetInfo& info = tjInfo.getObservationDataSetInfo();
  ObservationDataSetType obsType = te::st::GetType(info);
  
  //get temporal and spatial extents
  std::auto_ptr<te::dt::DateTimePeriod> period(ds->getTemporalExtent()); 
  std::auto_ptr<te::gm::Geometry> geom; 
  if(info.hasGeomProp())
  {
    std::size_t idx = te::da::GetPropertyPos(ds, info.getGeomPropName());
    std::auto_ptr<te::gm::Envelope> aux = ds->getExtent(idx);
    geom.reset(te::gm::GetGeomFromEnvelope(aux.get(), -1));
  }
  
  obsType.setSpatialExtent(geom.release());
  obsType.setTemporalExtent(period.release());

  return std::auto_ptr<TrajectoryDataSet> (new TrajectoryDataSet(ds, obsType)); 
}

std::auto_ptr<te::st::TimeSeriesDataSet>  
te::st::STDataLoaderFromMemDS::buildDataSet(te::stmem::DataSet* ds, const TimeSeriesDataSetInfo& tsInfo)
{
  //get type from info
  const ObservationDataSetInfo& info = tsInfo.getObservationDataSetInfo();
  ObservationDataSetType obsType = te::st::GetType(info);
  
  //get temporal extents
  std::auto_ptr<te::dt::DateTimePeriod> period(ds->getTemporalExtent()); 
  obsType.setTemporalExtent(period.release());

  return std::auto_ptr<TimeSeriesDataSet> ( new TimeSeriesDataSet(ds, obsType, 
                                            tsInfo.getValuePropNames())); 
}

te::st::STDataLoaderFromMemDS::~STDataLoaderFromMemDS()
{
}



