/*
 * File: ST.i
 *
 * Spatial Temporal module
 */

%include std_pair.i 
 
#define TESTEXPORT 

AUTO_PTR_TYPEMAPS(te::dt::DateTimePeriod)
AUTO_PTR_TYPEMAPS(te::st::Coverage)
AUTO_PTR_TYPEMAPS(te::st::TimeSeries)
AUTO_PTR_TYPEMAPS(te::st::CoverageSeries)
AUTO_PTR_TYPEMAPS(te::st::Observation)
AUTO_PTR_TYPEMAPS(te::st::Trajectory)
AUTO_PTR_TYPEMAPS(te::st::ObservationDataSet)
AUTO_PTR_TYPEMAPS(te::st::TrajectoryDataSet)
AUTO_PTR_TYPEMAPS(te::st::PointCoverage)
AUTO_PTR_TYPEMAPS(te::st::TimeSeriesDataSet)
AUTO_PTR_TYPEMAPS(te::st::RasterCoverage)

// Fail to export this operator
%ignore te::st::CoverageSeriesIterator::operator *;
 
%{
#include "terralib/st/core/coverage/CoverageDataSetInfo.h"
#include "terralib/st/core/coverage/CoverageSeries.h"
#include "terralib/st/core/coverage/CoverageSeriesDataSetInfo.h"
#include "terralib/st/core/coverage/CoverageSeriesObservation.h"
#include "terralib/st/core/coverage/Coverage.h"
#include "terralib/st/core/coverage/CoverageSeriesIterator.h"
#include "terralib/st/core/coverage/PointCoverageDataSetInfo.h"
#include "terralib/st/core/coverage/PointCoverageObservation.h"
#include "terralib/st/core/coverage/PointCoverageIterator.h"
#include "terralib/st/core/coverage/PointCoverage.h"
#include "terralib/st/core/coverage/RasterCoverage.h"
#include "terralib/st/core/coverage/RasterCoverageDataSetInfo.h"
#include "terralib/st/core/event/Event.h"
#include "terralib/st/core/event/Object.h"
#include "terralib/st/core/interpolator/AbstractCoverageSeriesInterp.h"
#include "terralib/st/core/interpolator/AbstractPointCoverageInterp.h"
#include "terralib/st/core/interpolator/AbstractTimeSeriesInterp.h"
#include "terralib/st/core/interpolator/AbstractTrajectoryInterp.h"
#include "terralib/st/core/interpolator/IDWInterpolator.h"
#include "terralib/st/core/interpolator/NearestCoverageAtTimeInterp.h"
#include "terralib/st/core/interpolator/NearestGeometryAtTimeInterp.h"
#include "terralib/st/core/interpolator/NearestValueAtTimeInterp.h"
#include "terralib/st/core/observation/Observation.h"
#include "terralib/st/core/observation/ObservationDataSet.h"
#include "terralib/st/core/observation/ObservationDataSetInfo.h"
#include "terralib/st/core/observation/ObservationDataSetType.h"
#include "terralib/st/core/timeseries/TimeSeries.h"
#include "terralib/st/core/timeseries/TimeSeriesDataSet.h"
#include "terralib/st/core/timeseries/TimeSeriesDataSetInfo.h"
#include "terralib/st/core/timeseries/TimeSeriesIterator.h"
#include "terralib/st/core/timeseries/TimeSeriesObservation.h"
#include "terralib/st/core/trajectory/Trajectory.h"
#include "terralib/st/core/trajectory/TrajectoryDataSet.h"
#include "terralib/st/core/trajectory/TrajectoryDataSetInfo.h"
#include "terralib/st/core/trajectory/TrajectoryIterator.h"
#include "terralib/st/core/trajectory/TrajectoryObservation.h"
#include "terralib/st/loader/STDataLoader.h"

/*
#include "terralib/st/loader/STDataLoaderFromDS.h"
#include "terralib/st/loader/STDataLoaderFromDSFactory.h"
#include "terralib/st/loader/STDataLoaderFromMemDS.h"
#include "terralib/st/loader/STDataLoaderFromMemDSFactory.h"
#include "terralib/st/loader/STDataLoaderImpl.h"
#include "terralib/st/loader/STDataLoaderImplFactory.h"

#include "terralib/st/maptools/AbstractSTDataSetLayer.h"
#include "terralib/st/maptools/ObservationDataSetLayer.h"
#include "terralib/st/maptools/TimeSeriesDataSetLayer.h"
#include "terralib/st/maptools/TrajectoryDataSetLayer.h"
*/
%}

%nspace te::st::SpatioTemporalRelation;
%nspace te::st::CoverageType;
%nspace te::st::Observation;
%nspace te::st::ObservationDataSet;
%nspace te::st::ObservationDataSetInfo;
%nspace te::st::ObservationDataSetType;
%nspace te::st::CoverageSeriesObservation;
%nspace te::st::CoverageSeriesIterator;
%nspace te::st::Coverage;
%nspace te::st::CoverageDataSetInfo;
%nspace te::st::CoverageSeries;
%nspace te::st::CoverageSeriesDataSetInfo;
%nspace te::st::PointCoverageObservation;
%nspace te::st::PointCoverageIterator;
%nspace te::st::PointCoverage;
%nspace te::st::PointCoverageDataSetInfo;
%nspace te::st::PointCoverageSeriesDataSetInfo;
%nspace te::st::RasterCoverage;
%nspace te::st::RasterCoverageDataSetInfo;
%nspace te::st::RasterCoverageSeriesDataSetInfo;
%nspace te::st::Event;
%nspace te::st::Object;
%nspace te::st::AbstractCoverageSeriesInterp;
%nspace te::st::AbstractPointCoverageInterp;
%nspace te::st::AbstractTimeSeriesInterp;
%nspace te::st::AbstractTrajectoryInterp;
%nspace te::st::IDWInterpolator;
%nspace te::st::NearestCoverageAtTimeInterp;
%nspace te::st::NearestGeometryAtTimeInterp;
%nspace te::st::NearestValueAtTimeInterp;
%nspace te::st::TimeSeriesObservation;
%nspace te::st::TimeSeriesIterator;
%nspace te::st::TimeSeries;
%nspace te::st::TimeSeriesDataSet;
%nspace te::st::TimeSeriesDataSetInfo;
%nspace te::st::TrajectoryObservation;
%nspace te::st::TrajectoryIterator;
%nspace te::st::Trajectory;
%nspace te::st::TrajectoryDataSet;
%nspace te::st::TrajectoryDataSetInfo;
%nspace te::st::STDataLoader;

%include "terralib/st/Enums.h"
%include "terralib/st/core/observation/Observation.h"
%include "terralib/st/core/observation/ObservationDataSet.h"
%include "terralib/st/core/observation/ObservationDataSetInfo.h"
%include "terralib/st/core/observation/ObservationDataSetType.h"
%include "terralib/st/core/coverage/CoverageSeriesObservation.h"
%include "terralib/st/core/coverage/CoverageSeriesIterator.h"
%include "terralib/st/core/coverage/Coverage.h"
%include "terralib/st/core/coverage/CoverageDataSetInfo.h"
%include "terralib/st/core/coverage/CoverageSeries.h"
%include "terralib/st/core/coverage/PointCoverageObservation.h"
%include "terralib/st/core/coverage/PointCoverageIterator.h"
%include "terralib/st/core/coverage/PointCoverage.h"
%include "terralib/st/core/coverage/PointCoverageDataSetInfo.h"
%include "terralib/st/core/coverage/RasterCoverage.h"
%include "terralib/st/core/coverage/RasterCoverageDataSetInfo.h"
%include "terralib/st/core/coverage/CoverageSeriesDataSetInfo.h"
%include "terralib/st/core/event/Event.h"
%include "terralib/st/core/event/Object.h"
%include "terralib/st/core/interpolator/AbstractCoverageSeriesInterp.h"
%include "terralib/st/core/interpolator/AbstractPointCoverageInterp.h"
%include "terralib/st/core/interpolator/AbstractTimeSeriesInterp.h"
%include "terralib/st/core/interpolator/AbstractTrajectoryInterp.h"
%include "terralib/st/core/interpolator/IDWInterpolator.h"
%include "terralib/st/core/interpolator/NearestCoverageAtTimeInterp.h"
%include "terralib/st/core/interpolator/NearestGeometryAtTimeInterp.h"
%include "terralib/st/core/interpolator/NearestValueAtTimeInterp.h"
%include "terralib/st/core/timeseries/TimeSeriesObservation.h"
%include "terralib/st/core/timeseries/TimeSeriesIterator.h"
%include "terralib/st/core/timeseries/TimeSeries.h"
%include "terralib/st/core/timeseries/TimeSeriesDataSet.h"
%include "terralib/st/core/timeseries/TimeSeriesDataSetInfo.h"
%include "terralib/st/core/trajectory/TrajectoryObservation.h"
%include "terralib/st/core/trajectory/TrajectoryIterator.h"
%include "terralib/st/core/trajectory/Trajectory.h"
%include "terralib/st/core/trajectory/TrajectoryDataSet.h"
%include "terralib/st/core/trajectory/TrajectoryDataSetInfo.h"
%include "terralib/st/loader/STDataLoader.h"
