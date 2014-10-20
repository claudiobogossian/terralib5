/*
 * File: Raster.i
 *
 * Raster module
 */

#define TERASTEREXPORT 
  
%{

#include "terralib/raster/Band.h"
#include "terralib/raster/BandIterator.h"
#include "terralib/raster/BandIteratorWindow.h"
#include "terralib/raster/BandProperty.h"
#include "terralib/raster/BandSummary.h"
#include "terralib/raster/Enums.h"
#include "terralib/raster/Grid.h"
#include "terralib/raster/Interpolator.h"
#include "terralib/raster/PositionIterator.h"
#include "terralib/raster/ProxyRaster.h"
#include "terralib/raster/Raster.h"
#include "terralib/raster/RasterFactory.h"
#include "terralib/raster/RasterIterator.h"
#include "terralib/raster/RasterProperty.h"
#include "terralib/raster/RasterSummary.h"
#include "terralib/raster/RasterSummaryManager.h"
#include "terralib/raster/RasterSynchronizer.h"
#include "terralib/raster/Reprojection.h"
#include "terralib/raster/SynchronizedBand.h"
#include "terralib/raster/SynchronizedBandBlocksManager.h"
#include "terralib/raster/SynchronizedRaster.h"
#include "terralib/raster/TileIndexer.h"
#include "terralib/raster/Vectorizer.h"
#include "terralib/raster/VectorizerPolygonStructure.h"

%}

%ignore te::rst::Raster::callOperator;

%include "terralib/raster/Enums.h"
%include "terralib/raster/Band.h"
%include "terralib/raster/BandIterator.h"
%include "terralib/raster/BandIteratorWindow.h"
%include "terralib/raster/BandProperty.h"
%include "terralib/raster/BandSummary.h"
%include "terralib/raster/Grid.h"
%include "terralib/raster/Interpolator.h"
%include "terralib/raster/PositionIterator.h"
%include "terralib/raster/Raster.h"
%include "terralib/raster/ProxyRaster.h"
%include "terralib/raster/RasterFactory.h"
%include "terralib/raster/RasterIterator.h"
%include "terralib/raster/RasterProperty.h"
%include "terralib/raster/RasterSummary.h"
%include "terralib/raster/RasterSummaryManager.h"
%include "terralib/raster/RasterSynchronizer.h"
%include "terralib/raster/Reprojection.h"
%include "terralib/raster/SynchronizedBand.h"
%include "terralib/raster/SynchronizedBandBlocksManager.h"
%include "terralib/raster/SynchronizedRaster.h"
%include "terralib/raster/TileIndexer.h"
%include "terralib/raster/Vectorizer.h"
%include "terralib/raster/VectorizerPolygonStructure.h"
