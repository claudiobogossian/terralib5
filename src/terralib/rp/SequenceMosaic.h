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
  \file terralib/rp/SequenceMosaic.h
  \brief Create mosaics from a sequence of overlapped rasters using an automatic tie-points detection method. 
 */

#ifndef __TERRALIB_RP_INTERNAL_SEQUENCEMOSAIC_H
#define __TERRALIB_RP_INTERNAL_SEQUENCEMOSAIC_H

#include "Algorithm.h"
#include "FeedersRaster.h"
#include "Blender.h"
#include "TiePointsLocator.h"
#include "../raster/Interpolator.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../geometry/GTParameters.h"

#include <vector>
#include <string>

namespace te
{
  namespace rp
  {
    /*!
      \class SequenceMosaic
      \brief Create mosaics from a sequence of overlapped rasters using an automatic tie-points detection method. 
      \note When the tie-points detection fails, a new mosaic sequence is generated.
      \note Each mosaic sequence takes the first raster (of each sequence) as reference for resolution, SRS and equalization parameters.
      \ingroup rp_mos
     */
    class TERPEXPORT SequenceMosaic : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Mosaic input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            FeederConstRaster* m_feederRasterPtr; //!< Input rasters feeder.
            
            std::vector< std::vector< unsigned int > > m_inputRastersBands; //!< Bands to process for each input raster.
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used if tie-points are supplied (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            double m_noDataValue; //!< The pixel value used where no raster data is avaliable (defaul:0).
            
            bool m_forceInputNoDataValue; //!< If true, m_noDataValue will be used as the no-data value for input rasters (defalt:false).
            
            te::rp::Blender::BlendMethod m_blendMethod; //!< The pixel blending method (default: NoBlendMethod).
            
            bool m_autoEqualize; //!< Auto equalization will be performed using the overlaped image areas (default:true).
            
            bool m_useRasterCache; //!< Enable(true) or disable the use of raster caching (default:true).
            
            bool m_enableMultiThread; //!< Enable/Disable the use of multi-threads (default:true).
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            unsigned int m_tiePointsLocationBandIndex; //!< The band used to locate tie-points, this is the index inside each vector of m_inputRastersBands (defaul:0).
            
            std::string m_outDataSetsNamePrefix; //!< The raster output data sets names prefix.
            
            std::string m_outDataSetsNameSufix; //!< The raster output data sets names sufix.
            
            double m_minRequiredTiePointsCoveredAreaPercent; //!< The mininumum required tie-points covered area percent of each raster area - valid range [0,100] (default:25).
            
            te::rp::TiePointsLocator::InputParameters m_locatorParams; //!< The parameters used by the tie-points locator when processing each rasters pair (leave untouched to use the default).
            
            InputParameters();
            
            InputParameters( const InputParameters& );            
            
            ~InputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  InputParameters& operator=( const InputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };
        
        /*!
          \class MosaicSequenceInfo
          \brief Mosaic sequence info.
        */        
        class TERPEXPORT MosaicSequenceInfo
        {
          public:            
            
            std::string m_dataSetName; //!< The generated data set name for this mosaic sequence.
            
            std::vector< unsigned int > m_rasterFeederIndexes; //!< The indexes (inside the input rasters feeder) of the rasters written to this mosaic sequence.
            
            std::vector< std::vector< te::gm::GTParameters::TiePoint > > m_tiePoints; //!< The generated tie-pionts (te::gm::GTParameters::TiePoint::first are mosaic coods, te::gm::GTParameters::TiePoint::second are the used input raster line/column indexes).
            
            void clear(); //!< Clear the internal allocated resources.
            
            MosaicSequenceInfo();
            
            ~MosaicSequenceInfo();
        };          
        
        /*!
          \class OutputParameters
          \brief Mosaic output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::vector< MosaicSequenceInfo > m_sequencesInfo; //! The mosaic sequences info.
            
            te::da::DataSource* m_outputDSPtr; //!< The output data source where the mosaic rasters will be created.
            
            OutputParameters();
            
            OutputParameters( const OutputParameters& );
            
            ~OutputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  OutputParameters& operator=( const OutputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };        

        SequenceMosaic();
        
        ~SequenceMosaic();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        SequenceMosaic::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
          
        /*!
          \brief Raster band statistics calcule.
          
          \param band Input raster band.
          
          \param forceNoDataValue Force the noDataValue to be used as the band no-data value.
          
          \param noDataValue The no-data value to use.
          
          \param mean Pixels mean.
          
          \param variance Pixels variance.
        */
        static void calcBandStatistics( const te::rst::Band& band,
          const bool& forceNoDataValue,
          const double& noDataValue,
          double& mean, 
          double& variance );
          
        /*!
          \brief Create a raster data set from the given raster.
          
          \param dataSetName The data set name.
          
          \param sourceRaster The source raster.
          
          \param dataSourcePtr The output data source pointer.
          
          \return true if OK, false on errors.
        */
        bool createRasterDataSet( const std::string& dataSetName,
          const te::rst::Raster& sourceRaster, te::da::DataSource* dataSourcePtr ) const;
          
        /*!
          \brief Create copy of the given raster into a disk file.
          
          \param fileName The disk file name.
          
          \param sourceRaster The source raster.
          
          \return true if OK, false on errors.
        */
        bool createDiskRasterCopy( const std::string& fileName,
          const te::rst::Raster& sourceRaster ) const;
          
    };

  } // end namespace rp
}   // end namespace te

#endif

