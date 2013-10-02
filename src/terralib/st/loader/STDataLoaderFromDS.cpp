/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file STDataLoaderFromDS.cpp

  \brief  This file contains a class responsible for loading spatiotemporal data 
          from a DataSource using query capabilities. 
*/

//TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/DateTimePeriod.h"

//ST
#include "../../Exception.h"
#include "../../Utils.h"
#include "STDataLoaderFromDS.h"
#include "../observation/ObservationDataSetInfo.h"
#include "../trajectory/TrajectoryDataSetInfo.h"
#include "../timeseries/TimeSeriesDataSetInfo.h"
#include "../observation/ObservationDataSet.h"
#include "../trajectory/TrajectoryDataSet.h"
#include "../timeseries/TimeSeriesDataSet.h"

//STL
#include <vector>

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const ObservationDataSetInfo& info, 
                                 te::common::TraverseType travType)
{
  //use the DataSourceManager to get the DataSource 
 te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);
 
 std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getDataSetName(), travType));

 return buildDataSet(dset.release(), info); 
}


std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Envelope& e,
                          te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //get the geometry property name
  std::auto_ptr<te::da::DataSetType> dsettype(ds->getDataSetType(info.getDataSetName()));
  
  std::string geomPropName = dsettype->getProperty(info.getGeomPropIdx())->getName();

  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getDataSetName(), geomPropName, &e, r, travType));

  return buildDataSet(dset.release(), info);  
}
        
std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Geometry& geom, 
                          te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //get the geometry property name
  std::auto_ptr<te::da::DataSetType> dsettype(ds->getDataSetType(info.getDataSetName()));

  std::string geomPropName = dsettype->getProperty(info.getGeomPropIdx())->getName();
  
  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getDataSetName(), geomPropName, &geom, r, travType));

  return buildDataSet(dset.release(), info); 
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const te::st::ObservationDataSetInfo& /*info*/, 
                          const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*r*/,
                          te::common::TraverseType /*travType*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::st::ObservationDataSet>();
}
                                               
std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const te::st::ObservationDataSetInfo& /*info*/, 
                          const te::gm::Geometry& /*geom*/, te::gm::SpatialRelation /*sr*/,
                          const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*tr*/,
                          te::common::TraverseType /*travType*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::st::ObservationDataSet> ();
}

void 
te::st::STDataLoaderFromDS::getInfo( const TrajectoryDataSetInfo& /*input*/, std::vector<TrajectoryDataSetInfo>& /*output*/)
{
  return;
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const TrajectoryDataSetInfo& info, te::common::TraverseType travType) 
{
 //use the DataSourceManager to get the DataSource 
 te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);
 
 std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), travType));

 return buildDataSet(dset.release(), info); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const TrajectoryDataSetInfo& info,  
                             const te::gm::Geometry& geom, te::gm::SpatialRelation r,
                             te::common::TraverseType travType)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //get the geometry property name
  std::auto_ptr<te::da::DataSetType> dsettype(ds->getDataSetType(info.getObservationDataSetInfo().getDataSetName()));

  std::string geomPropName = dsettype->getProperty(info.getGeomPropIdx())->getName();
  
  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), geomPropName, &geom, r, travType));

  return buildDataSet(dset.release(), info); 
}

std::auto_ptr<te::st::TrajectoryDataSet>
te::st::STDataLoaderFromDS::getDataSet(const TrajectoryDataSetInfo& info, 
                                const te::gm::Envelope& e, te::gm::SpatialRelation r,               
                                te::common::TraverseType travType)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //get the geometry property name
  std::auto_ptr<te::da::DataSetType> dsettype(ds->getDataSetType(info.getObservationDataSetInfo().getDataSetName()));
  
  std::string geomPropName = dsettype->getProperty(info.getGeomPropIdx())->getName();

  //get the data set applying he filter
  std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), geomPropName, &e, r, travType));

  return buildDataSet(dset.release(), info);  
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const TrajectoryDataSetInfo& /*info*/, 
                          const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*r*/,
                          te::common::TraverseType /*travType*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::st::TrajectoryDataSet>();
}

std::auto_ptr<te::st::TrajectoryDataSet>  
te::st::STDataLoaderFromDS::getDataSet(const TrajectoryDataSetInfo& /*info*/, 
                          const te::gm::Geometry& /*geom*/, te::gm::SpatialRelation /*sr*/,
                          const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*tr*/,
                          te::common::TraverseType /*travType*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::st::TrajectoryDataSet>();
}

void te::st::STDataLoaderFromDS::getInfo( const TimeSeriesDataSetInfo& /*input*/, 
                      std::vector<TimeSeriesDataSetInfo>& /*output*/)
{
  return;
}
        
std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const TimeSeriesDataSetInfo& info, 
                      te::common::TraverseType travType)
{
 //use the DataSourceManager to get the DataSource 
 te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);
 
 std::auto_ptr<te::da::DataSet> dset(ds->getDataSet(info.getObservationDataSetInfo().getDataSetName(), travType));

 return buildDataSet(dset.release(), info); 
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoaderFromDS::getDataSet(const TimeSeriesDataSetInfo& /*info*/, 
                          const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*r*/,
                          te::common::TraverseType /*travType*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::st::TimeSeriesDataSet>();
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromDS::getTemporalExtent(const ObservationDataSetInfo& /*info*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::dt::DateTimePeriod>();
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromDS::getTemporalExtent(const TrajectoryDataSetInfo& /*info*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::dt::DateTimePeriod>();
}
        
std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoaderFromDS::getTemporalExtent(const TimeSeriesDataSetInfo& /*info*/)
{
  //use the DataSourceManager to get the DataSource 
  //te::da::DataSource* ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  //montar uma Query e passar para o data source!

  //return buildDataSet(dset.release(), info);
  return std::auto_ptr<te::dt::DateTimePeriod>();
}

te::gm::Envelope 
te::st::STDataLoaderFromDS::getSpatialExtent(const ObservationDataSetInfo& info)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  std::auto_ptr<te::gm::Envelope> res(ds->getExtent(info.getDataSetName(), info.getGeomPropIdx()));
  return *res.get();
}

te::gm::Envelope 
te::st::STDataLoaderFromDS::getSpatialExtent(const TrajectoryDataSetInfo& info)
{
  //use the DataSourceManager to get the DataSource 
  te::da::DataSourcePtr ds = te::da::GetDataSource(info.getDataSourceInfo().getId(), false);

  std::auto_ptr<te::gm::Envelope> res(ds->getExtent(info.getObservationDataSetInfo().getDataSetName(), 
                                     info.getObservationDataSetInfo().getGeomPropIdx()));
  return *res.get();
}

//protected
std::auto_ptr<te::st::ObservationDataSet>  
te::st::STDataLoaderFromDS::buildDataSet(te::da::DataSet* ds, const ObservationDataSetInfo& info)
{
  return std::auto_ptr<ObservationDataSet> (new ObservationDataSet(ds, te::st::GetType(info), 0, te::gm::Envelope())); 
}

std::auto_ptr<te::st::TrajectoryDataSet>  
te::st::STDataLoaderFromDS::buildDataSet(te::da::DataSet* ds, const TrajectoryDataSetInfo& info)
{
  return std::auto_ptr<TrajectoryDataSet> (new TrajectoryDataSet(ds, te::st::GetType(info), 0, te::gm::Envelope())); 
}

std::auto_ptr<te::st::TimeSeriesDataSet>  
te::st::STDataLoaderFromDS::buildDataSet(te::da::DataSet* ds, const TimeSeriesDataSetInfo& info)
{
  
  return std::auto_ptr<TimeSeriesDataSet> (new TimeSeriesDataSet(ds, te::st::GetType(info), 0));
}