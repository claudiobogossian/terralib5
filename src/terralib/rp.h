/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/rp.h

  \brief This file contains include headers for the TerraLib Raster Processing module.
*/

#ifndef __TERRALIB_INTERNAL_RP_H
#define __TERRALIB_INTERNAL_RP_H

// TerraLib
#include "rp/AbstractFeeder.h"
#include "rp/Algorithm.h"
#include "rp/AlgorithmInputParameters.h"
#include "rp/AlgorithmOutputParameters.h"
#include "rp/ArithmeticOperations.h"
#include "rp/Blender.h"
#include "rp/ClassifierDummyStrategy.h"
#include "rp/ClassifierEMStrategy.h"
#include "rp/Classifier.h"
#include "rp/ClassifierISOSegStrategy.h"
#include "rp/ClassifierKMeansStrategy.h"
#include "rp/ClassifierMAPStrategy.h"
#include "rp/ClassifierSAMStrategy.h"
#include "rp/ClassifierStrategyFactory.h"
#include "rp/ClassifierStrategy.h"
#include "rp/Config.h"
#include "rp/Contrast.h"
#include "rp/Exception.h"
#include "rp/FeedersRaster.h"
#include "rp/Filter.h"
#include "rp/Functions.h"
#include "rp/GeoMosaic.h"
#include "rp/IHSFusion.h"
#include "rp/Macros.h"
#include "rp/Matrix.h"
#include "rp/MixtureModel.h"
#include "rp/MixtureModelLinearStrategy.h"
#include "rp/MixtureModelPCAStrategy.h"
#include "rp/MixtureModelStrategyFactory.h"
#include "rp/MixtureModelStrategy.h"
#include "rp/Module.h"
#include "rp/PCAFusion.h"
#include "rp/RasterAttributes.h"
#include "rp/RasterHandler.h"
#include "rp/Register.h"
#include "rp/SegmenterDummyStrategy.h"
#include "rp/Segmenter.h"
#include "rp/SegmenterIdsManager.h"
#include "rp/SegmenterRegionGrowingSegment.h"
#include "rp/SegmenterRegionGrowingSegmentsPool.h"
#include "rp/SegmenterRegionGrowingBaatzStrategy.h"
#include "rp/SegmenterRegionGrowingMeanStrategy.h"
#include "rp/SegmenterSegmentsBlock.h"
#include "rp/SegmenterStrategyFactory.h"
#include "rp/SegmenterStrategy.h"
#include "rp/SegmenterStrategyParameters.h"
#include "rp/SequenceMosaic.h"
#include "rp/Skeleton.h"
#include "rp/SpectralResponseFunctions.h"
#include "rp/StrategyParameters.h"
#include "rp/Texture.h"
#include "rp/TiePointsLocator.h"
#include "rp/TiePointsLocatorInputParameters.h"
#include "rp/TiePointsLocatorMoravecStrategy.h"
#include "rp/TiePointsLocatorStrategy.h"
#include "rp/TiePointsLocatorSURFStrategy.h"
#include "rp/TiePointsMosaic.h"
#include "rp/WisperFusion.h"
#include "rp/radar/RadarFunctions.h"

/*!
  \defgroup rp Raster Processing

  \brief This module provides classes and functions with raster processing capabilities.

    Basically, the TerraLib Raster Processing module contains several processing operations applied to an image.
    The main classes/concepts in this module are listed here. The namespace associated to the Raster Processing module is te::rp.
    To know more about it, see the te::rp namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for Raster Processing module of TerraLib.
  */
  namespace rp
  {
    /*!
      \brief Namespace for Radar Raster Processing.
    */
    namespace radar
    {
    } // end namespace radar
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_RP_H
