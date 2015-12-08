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

//Python doesn't work with namespaces
#ifndef SWIGPYTHON

%nspace te::rst::SummaryTypes;
%nspace te::rst::ColorInterp;
%nspace te::rst::PaletteInterpretation;
%nspace te::rst::Band;
%nspace te::rst::BandIterator;
%nspace te::rst::BandIteratorWindow;
%nspace te::rst::BandProperty;
%nspace te::rst::BandSummary;
%nspace te::rst::Grid;
%nspace te::rst::Interpolator;
%nspace te::rst::PositionIterator;
%nspace te::rst::Raster;
%nspace te::rst::ProxyRaster;
%nspace te::rst::RasterFactory;
%nspace te::rst::RasterIterator;
%nspace te::rst::RasterProperty;
%nspace te::rst::RasterSummary;
%nspace te::rst::RasterSummaryManager;
%nspace te::rst::RasterSynchronizer;
%nspace te::rst::Reprojection;
%nspace te::rst::SynchronizedBand;
%nspace te::rst::SynchronizedBandBlocksManager;
%nspace te::rst::SynchronizedRaster;
%nspace te::rst::TileIndexer;
%nspace te::rst::Vectorizer;
%nspace te::rst::VectorizerPolygonStructure;

#endif //SWIGPYTHON

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
