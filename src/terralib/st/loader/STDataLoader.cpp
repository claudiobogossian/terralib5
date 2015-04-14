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
  \file STDataLoader.cpp

  \brief  This file contains a class that contains a set of static methods
          to load spatiotemporal data from data sources.
*/

//TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../dataaccess/datasource/DataSourceManager.h"

//ST
#include "STDataLoader.h"
#include "STDataLoaderImpl.h"
#include "STDataLoaderImplFactory.h"
#include "../core/observation/ObservationDataSet.h"
#include "../core/observation/ObservationDataSetInfo.h"
#include "../core/trajectory/TrajectoryDataSet.h"
#include "../core/trajectory/TrajectoryDataSetInfo.h"
#include "../core/timeseries/TimeSeriesDataSet.h"
#include "../core/timeseries/TimeSeriesDataSetInfo.h"
#include "../core/coverage/CoverageSeries.h"
#include "../core/coverage/Coverage.h"
#include "../core/coverage/PointCoverage.h"
#include "../core/coverage/RasterCoverage.h"
#include "../core/coverage/RasterCoverageDataSetInfo.h"
#include "../core/coverage/PointCoverageDataSetInfo.h"
#include "../Exception.h"
#include "../Globals.h"

bool te::st::STDataLoader::sm_STDataLoaderInitialized(false);

te::st::STDataLoader::STDataLoader()
{
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info, 
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, travType);
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Envelope& e,
                                 te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, e, r, travType);
}
        
std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info, const te::gm::Geometry& geom, 
                                 te::gm::SpatialRelation r, te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, geom, r, travType);
}
        
std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, dt, r, travType);
}

std::auto_ptr<te::st::ObservationDataSet>
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info,
                                 const te::dt::DateTime& dt, 
                                 te::dt::TemporalRelation tr,
                                 const te::gm::Envelope& e, 
                                 te::gm::SpatialRelation sr,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, dt, tr, e, sr, travType); 
}

std::auto_ptr<te::st::ObservationDataSet> 
te::st::STDataLoader::getDataSet(const te::st::ObservationDataSetInfo& info, 
                                 const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getDataSet(info, geom, sr, dt, tr, travType);
}

void te::st::STDataLoader::getInfo( const te::st::TrajectoryDataSetInfo& input, 
                             std::vector<te::st::TrajectoryDataSetInfo>& output)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(input.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getInfo(input, output); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const te::st::TrajectoryDataSetInfo& info, te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, travType); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const TrajectoryDataSetInfo& info,  
                             const te::gm::Geometry& geom, te::gm::SpatialRelation r,
                             te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, travType); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const TrajectoryDataSetInfo& info, const te::gm::Envelope& e, te::gm::SpatialRelation r,               
                                te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, e, r, travType); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const TrajectoryDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, dt, r, travType); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const TrajectoryDataSetInfo& info,
                                 const te::dt::DateTime& dt, 
                                 te::dt::TemporalRelation tr,
                                 const te::gm::Envelope& e, 
                                 te::gm::SpatialRelation sr,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, dt, tr, e, sr, travType); 
}

std::auto_ptr<te::st::TrajectoryDataSet> 
te::st::STDataLoader::getDataSet(const TrajectoryDataSetInfo& info, 
                                 const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation tr, te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, geom, sr, dt, tr, travType);
}


void te::st::STDataLoader::getInfo( const te::st::TimeSeriesDataSetInfo& input, 
                              std::vector<te::st::TimeSeriesDataSetInfo>& output)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(input.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getInfo(input, output);
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoader::getDataSet(const te::st::TimeSeriesDataSetInfo& info, te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, travType);
}

std::auto_ptr<te::st::TimeSeriesDataSet> 
te::st::STDataLoader::getDataSet(const te::st::TimeSeriesDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r,
                                 te::common::TraverseType travType)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getDataSet(info, dt, r, travType);
}

std::auto_ptr<te::st::CoverageSeries> 
te::st::STDataLoader::getCoverageSeries( const PointCoverageSeriesDataSetInfo& info,
                                               AbstractPointCoverageInterp* interp)
{
  te::st::PointCoverageSeriesDataSetInfo::const_iterator it = info.begin();
  std::auto_ptr<te::st::CoverageSeries> result(new CoverageSeries());
  while(it!=info.end())
  {
    std::auto_ptr<PointCoverage> cv(getCoverage(*it, interp));
    result->add(static_cast<te::dt::DateTime*>(cv->getTime()->clone()), cv.release());
    ++it;
  }

  return result;
}

std::auto_ptr<te::st::CoverageSeries> 
te::st::STDataLoader::getCoverageSeries(const RasterCoverageSeriesDataSetInfo& info)
{
  te::st::RasterCoverageSeriesDataSetInfo::const_iterator it = info.begin();
  std::auto_ptr<te::st::CoverageSeries> result(new CoverageSeries());
  while(it!=info.end())
  {
    std::auto_ptr<RasterCoverage> cv(getCoverage(*it));
    result->add(static_cast<te::dt::DateTime*>(cv->getTime()->clone()), cv.release());
    ++it;
  }
  return result;
}

std::auto_ptr<te::st::RasterCoverage> 
te::st::STDataLoader::getCoverage(const RasterCoverageDataSetInfo& info)
{
  std::auto_ptr<te::da::DataSet> dset(te::da::GetDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                                         info.getObservationDataSetInfo().getDataSourceInfo().getId()));
  if(!dset.get())
    throw Exception("The DataSet was not loaded correctly!"); 

  std::auto_ptr<te::st::RasterCoverage> result(new RasterCoverage());
  while(dset->moveNext())
  {
    //por enquanto nao estou considerando tempo em duas propriedades, so pego a primeira
    std::auto_ptr<te::dt::DateTime> dt;
    if(info.getObservationDataSetInfo().getTime()!=0)
      dt.reset(dynamic_cast<te::dt::DateTime*>(info.getObservationDataSetInfo().getTime()->clone()));
    else
      dt = dset->getDateTime(info.getObservationDataSetInfo().getBeginTimePropName());
    
    std::auto_ptr<te::rst::Raster> raster(dset->getRaster(info.getRasterPropName()));
    result->set(raster.release(), dt.release());
  }
  return result;
}

std::auto_ptr<te::st::PointCoverage> 
te::st::STDataLoader::getCoverage(const PointCoverageDataSetInfo& info, 
                                  AbstractPointCoverageInterp* interp)
{
  std::auto_ptr<te::da::DataSet> dset(te::da::GetDataSet(info.getObservationDataSetInfo().getDataSetName(), 
                                                         info.getObservationDataSetInfo().getDataSourceInfo().getId()));
  if(!dset.get())
    throw Exception("The DataSet was not loaded correctly!"); 
  
  std::vector<std::string> vPropDS = info.getValuePropNames(); //The indexes of the observed properties in the DataSet
  std::vector<int> ptypes;
  std::vector<std::string> pnames;

  for(unsigned int i=0; i<vPropDS.size(); ++i)
  {
    std::size_t idx = te::da::GetPropertyPos(dset.get(), vPropDS[i]);
    ptypes.push_back(dset->getPropertyDataType(idx));
    pnames.push_back(vPropDS[i]);
  }
  
  //Get the property that has date and time associated to each observation
  //TO DO: Ainda temos que implementar quando for periodo em duas properties!!!!
  //TO DO: No caso acima, temos que gerar um periodo e passar para o PointCoverage como uma unica property!
  std::string tpDS = "";
  int tpCV = -1;
  const std::string& tPropName = info.getObservationDataSetInfo().getBeginTimePropName();
  if(!tPropName.empty())
  {
    tpDS = tPropName; //name of the date and time property in the DataSet
    std::size_t idx = te::da::GetPropertyPos(dset.get(), tPropName);
    ptypes.push_back(dset->getPropertyDataType(idx));
    pnames.push_back(tpDS);
    vPropDS.push_back(tpDS);
    tpCV = ptypes.size()-1; //index of the date and time property in the Coverage
  }

  std::auto_ptr<te::dt::DateTime> dt(dynamic_cast<te::dt::DateTime*>(info.getObservationDataSetInfo().getTime()->clone()));

  std::auto_ptr<te::st::PointCoverage> result(new PointCoverage(interp, 0, dt.release(), vPropDS.size(), ptypes, pnames, tpCV));
  
  while(dset->moveNext())
  {
    //get the point
    std::auto_ptr<te::gm::Geometry> geom(dset->getGeometry(info.getObservationDataSetInfo().getGeomPropName()));
    
    //get the observed values. If there is the date and time property, it will be loaded here!!!
    boost::ptr_vector<te::dt::AbstractData> values;
    for(unsigned int i=0; i<vPropDS.size(); ++i)
      values.push_back(dset->getValue(vPropDS[i]));

    result->add(*static_cast<te::gm::Point*>(geom.release()), values); //values.release() ?????
  }
  return result;
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoader::getTemporalExtent(const te::st::ObservationDataSetInfo& info)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getTemporalExtent(info);
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoader::getTemporalExtent(const te::st::TrajectoryDataSetInfo& info)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getTemporalExtent(info);
}

std::auto_ptr<te::dt::DateTimePeriod> 
te::st::STDataLoader::getTemporalExtent(const te::st::TimeSeriesDataSetInfo& info)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getTemporalExtent(info);
}

te::gm::Envelope 
te::st::STDataLoader::getSpatialExtent(const te::st::ObservationDataSetInfo& info)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getDataSourceInfo().getType()));
  return impl->getSpatialExtent(info);
}

te::gm::Envelope 
te::st::STDataLoader::getSpatialExtent(const te::st::TrajectoryDataSetInfo& info)
{
  std::auto_ptr<STDataLoaderImpl> impl(STDataLoaderImplFactory::make(info.getObservationDataSetInfo().getDataSourceInfo().getType()));
  return impl->getSpatialExtent(info);
}

void te::st::STDataLoader::initialize()
{
  if(te::st::STDataLoader::sm_STDataLoaderInitialized == true)
    return;
  te::da::DataSourceManager::getInstance().make(te::st::Globals::sm_STMemoryDataSourceId, "STMEMORY");
  te::st::STDataLoader::sm_STDataLoaderInitialized = true;
}

void te::st::STDataLoader::finalize()
{
  te::da::DataSourcePtr p = te::da::DataSourceManager::getInstance().find(te::st::Globals::sm_STMemoryDataSourceId);
  if(p.get()!=0)
    p->close();
}

te::st::STDataLoader::~STDataLoader()
{
}






