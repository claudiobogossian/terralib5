/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp_fw.h

  \brief This file contains forward declarations for the TerraLib Raster Processing module.
*/

#ifndef __TERRALIB_FW_INTERNAL_RP_FW_H
#define __TERRALIB_FW_INTERNAL_RP_FW_H
#include "rp/radar/RadarFunctions.h"

namespace te
{
  namespace rp
  {    
    class AbstractFeeder;
    class Algorithm;
    class AlgorithmInputParameters;
    class AlgorithmOutputParameters;
    class ArithmeticOperations;
    class Blender;
    class Classifier;
    class ClassifierDummyStrategy;
    class ClassifierEMStrategy;
    class ClassifierISOSegStrategy;
    class ClassifierKMeansStrategy;
    class ClassifierMAPStrategy;
    class ClassifierSAMStrategy;
    class ClassifierStrategy;
    class ClassifierStrategyFactory;
    class Contrast;
    class FeederConstRaster;
    class FeederConstRasterVector;
    class FeederConstRasterInfo;
    class FeederConstRasterDirectory;
    class Filter;
    class GeoMosaic;
    class IHSFusion;
    template< typename TemplateElementType > class Matrix;
    class MixtureModel;
    class MixtureModelLinearStrategy;
    class MixtureModelPCAStrategy;
    class MixtureModelStrategy;
    class MixtureModelStrategyFactory;
    class Module;
    class PCAFusion;
    class RasterAttributes;
    class RasterHandler;
    class Register;
    class Segmenter;
    class SegmenterDummyStrategy;
    class SegmenterIdsManager;
    class SegmenterRegionGrowingSegment;
    class SegmenterRegionGrowingSegmentsPool;
    class SegmenterRegionGrowingStrategy;
    class SegmenterSegmentsBlock;
    class SegmenterStrategy;
    class SegmenterStrategyFactory;
    class SegmenterStrategyParameters;
    class SequenceMosaic;
    class Skeleton;
    class StrategyParameters;
    struct Texture;
    class TiePointsLocator;
    class TiePointsLocatorInputParameters;
    class TiePointsLocatorMoravecStrategy;
    class TiePointsLocatorStrategy;
    class TiePointsLocatorSURFStrategy;
    class TiePointsMosaic;
    class WisperFusion;
    
    namespace radar
    {
      
      
    }
  } 
}

#endif  // __TERRALIB_FW_INTERNAL_RP_FW_H
