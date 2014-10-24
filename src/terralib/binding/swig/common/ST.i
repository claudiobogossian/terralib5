/*
 * File: ST.i
 *
 * Spatial Temporal module
 */

%include std_pair.i 
 
%include lua/typemaps.i

#define TESTEXPORT 
 
AUTO_PTR_TYPEMAPS(te::dt::DateTimePeriod)
AUTO_PTR_TYPEMAPS(te::st::Coverage)
AUTO_PTR_TYPEMAPS(te::st::TimeSeries)
AUTO_PTR_TYPEMAPS(te::st::CoverageSeries)
AUTO_PTR_TYPEMAPS(te::st::Observation)
AUTO_PTR_TYPEMAPS(te::st::Trajectory)

// Fail to export this operator
%ignore te::st::CoverageSeriesIterator::operator *;
 
%{
#include "terralib\st\core\coverage\CoverageDataSetInfo.h"
#include "terralib\st\core\coverage\CoverageSeries.h"
#include "terralib\st\core\coverage\CoverageSeriesDataSetInfo.h"
#include "terralib\st\core\coverage\CoverageSeriesObservation.h"
#include "terralib\st\core\coverage\Coverage.h"
#include "terralib\st\core\coverage\CoverageSeriesIterator.h"
#include "terralib\st\core\coverage\PointCoverageDataSetInfo.h"
#include "terralib\st\core\coverage\PointCoverageObservation.h"
#include "terralib\st\core\coverage\PointCoverageIterator.h"
#include "terralib\st\core\coverage\PointCoverage.h"
#include "terralib\st\core\coverage\RasterCoverage.h"
#include "terralib\st\core\coverage\RasterCoverageDataSetInfo.h"
#include "terralib\st\core\event\Event.h"
#include "terralib\st\core\event\Object.h"
#include "terralib\st\core\interpolator\AbstractCoverageSeriesInterp.h"
#include "terralib\st\core\interpolator\AbstractPointCoverageInterp.h"
#include "terralib\st\core\interpolator\AbstractTimeSeriesInterp.h"
#include "terralib\st\core\interpolator\AbstractTrajectoryInterp.h"
#include "terralib\st\core\interpolator\IDWInterpolator.h"
#include "terralib\st\core\interpolator\NearestCoverageAtTimeInterp.h"
#include "terralib\st\core\interpolator\NearestGeometryAtTimeInterp.h"
#include "terralib\st\core\interpolator\NearestValueAtTimeInterp.h"
#include "terralib\st\core\observation\Observation.h"
#include "terralib\st\core\observation\ObservationDataSet.h"
#include "terralib\st\core\observation\ObservationDataSetInfo.h"
#include "terralib\st\core\observation\ObservationDataSetType.h"
#include "terralib\st\core\timeseries\TimeSeries.h"
#include "terralib\st\core\timeseries\TimeSeriesDataSet.h"
#include "terralib\st\core\timeseries\TimeSeriesDataSetInfo.h"
#include "terralib\st\core\timeseries\TimeSeriesDataSetType.h"
#include "terralib\st\core\timeseries\TimeSeriesIterator.h"
#include "terralib\st\core\timeseries\TimeSeriesObservation.h"
#include "terralib\st\core\trajectory\Trajectory.h"
#include "terralib\st\core\trajectory\TrajectoryDataSet.h"
#include "terralib\st\core\trajectory\TrajectoryDataSetInfo.h"
#include "terralib\st\core\trajectory\TrajectoryDataSetType.h"
#include "terralib\st\core\trajectory\TrajectoryIterator.h"
#include "terralib\st\core\trajectory\TrajectoryObservation.h"

/*
#include "terralib\st\loader\STDataLoader.h"
#include "terralib\st\loader\STDataLoaderFromDS.h"
#include "terralib\st\loader\STDataLoaderFromDSFactory.h"
#include "terralib\st\loader\STDataLoaderFromMemDS.h"
#include "terralib\st\loader\STDataLoaderFromMemDSFactory.h"
#include "terralib\st\loader\STDataLoaderImpl.h"
#include "terralib\st\loader\STDataLoaderImplFactory.h"

#include "terralib\st\maptools\AbstractSTDataSetLayer.h"
#include "terralib\st\maptools\ObservationDataSetLayer.h"
#include "terralib\st\maptools\TimeSeriesDataSetLayer.h"
#include "terralib\st\maptools\TrajectoryDataSetLayer.h"
*/
%}

%nspace te::st::CoverageType;

%include "terralib\st\Enums.h"
%include "terralib\st\core\observation\Observation.h"
%include "terralib\st\core\observation\ObservationDataSet.h"
%include "terralib\st\core\observation\ObservationDataSetInfo.h"
%include "terralib\st\core\observation\ObservationDataSetType.h"
%include "terralib\st\core\coverage\CoverageSeriesObservation.h"
%include "terralib\st\core\coverage\CoverageSeriesIterator.h"
%include "terralib\st\core\coverage\Coverage.h"
%include "terralib\st\core\coverage\CoverageDataSetInfo.h"
%include "terralib\st\core\coverage\CoverageSeries.h"
%include "terralib\st\core\coverage\CoverageSeriesDataSetInfo.h"
%include "terralib\st\core\coverage\PointCoverageObservation.h"
%include "terralib\st\core\coverage\PointCoverageIterator.h"
%include "terralib\st\core\coverage\PointCoverage.h"
%include "terralib\st\core\coverage\PointCoverageDataSetInfo.h"
%include "terralib\st\core\coverage\RasterCoverage.h"
%include "terralib\st\core\coverage\RasterCoverageDataSetInfo.h"
%include "terralib\st\core\event\Event.h"
%include "terralib\st\core\event\Object.h"
%include "terralib\st\core\interpolator\AbstractCoverageSeriesInterp.h"
%include "terralib\st\core\interpolator\AbstractPointCoverageInterp.h"
%include "terralib\st\core\interpolator\AbstractTimeSeriesInterp.h"
%include "terralib\st\core\interpolator\AbstractTrajectoryInterp.h"
%include "terralib\st\core\interpolator\IDWInterpolator.h"
%include "terralib\st\core\interpolator\NearestCoverageAtTimeInterp.h"
%include "terralib\st\core\interpolator\NearestGeometryAtTimeInterp.h"
%include "terralib\st\core\interpolator\NearestValueAtTimeInterp.h"
%include "terralib\st\core\timeseries\TimeSeriesObservation.h"
%include "terralib\st\core\timeseries\TimeSeriesIterator.h"
%include "terralib\st\core\timeseries\TimeSeries.h"
%include "terralib\st\core\timeseries\TimeSeriesDataSetType.h"
%include "terralib\st\core\timeseries\TimeSeriesDataSet.h"
%include "terralib\st\core\timeseries\TimeSeriesDataSetInfo.h"
%include "terralib\st\core\trajectory\TrajectoryObservation.h"
%include "terralib\st\core\trajectory\TrajectoryIterator.h"
%include "terralib\st\core\trajectory\Trajectory.h"
%include "terralib\st\core\trajectory\TrajectoryDataSetType.h"
%include "terralib\st\core\trajectory\TrajectoryDataSet.h"
%include "terralib\st\core\trajectory\TrajectoryDataSetInfo.h"