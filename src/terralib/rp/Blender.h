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
  \file terralib/rp/Blender.h
  \brief Blended pixel value calculation for two overlaped rasters.
 */

#ifndef __TERRALIB_RP_INTERNAL_BLENDER_H
#define __TERRALIB_RP_INTERNAL_BLENDER_H

#include "Config.h"
#include "Macros.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"
#include "../raster/RasterSynchronizer.h"
#include "../raster/TileIndexer.h"
#include "../geometry/Polygon.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/GeometricTransformation.h"

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>
#include <complex>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class Blender
      \brief Blended pixel value calculation for two overlaped rasters.
      \details The overlap between the two images is modeled by the given geometric transformation direct mapping input raster 1 indexed coords (line, column) to input raster 2 indexed coords.
      \note Optimized for rasters where all bands have the same blocking scheme.
      \ingroup rp_aux
     */
    class TERPEXPORT Blender : public boost::noncopyable
    {
      public:
        
        /*! \enum BlendMethod Pixel Blend methods. */      
        enum BlendMethod 
        {
          InvalidBlendMethod, //!< Invalid blending method.
          NoBlendMethod, //!< No blending performed.
          EuclideanDistanceMethod, //!< Euclidean distance method.
          SumMethod //!< Pixels will be summed inside the raster overlapped area.
        };        
        
        /*! Default constructor. */
        Blender();
        
        /*! Default destructor. */
        ~Blender();
        
        /*!
          \brief Inititate the blender instance.
          \param raster1 Input raster 1.
          \param raster1Bands Input raster 1 band indexes to use.
          \param raster2 Input raster 2.
          \param raster2Bands Input raster 2 band indexes to use (this vector has the same size as raster1Bands).
          \param blendMethod The blend method to apply.
          \param interpMethod1 The interpolation method to use when reading raster 1 data.
          \param interpMethod2 The interpolation method to use when reading raster 2 data.
          \param noDataValue The value returned where there is no pixel data bo blend.
          \param forceRaster1NoDataValue Use noDataValue as the input raster 1 no-data value (The original rasters no-data values will be ignored)
          \param forceRaster2NoDataValue Use noDataValue as the input raster 1 no-data value (The original rasters no-data values will be ignored)  
          \param pixelOffsets1 The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales1 The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelOffsets2 The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param pixelScales2 The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
          \param r1ValidDataDelimiterPtr A pointer to a geometry (raster 1 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
          \param r2ValidDataDelimiterPtr A pointer to a geometry (raster 2 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
          \param geomTransformation A transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 pixels ( te::gm::GTParameters::TiePoint::second ) (Note: all coords are indexed by lines/columns).
          \param threadsNumber Enable/disable the use of threads when applicable (0:automatic , 1:disabled, any other integer dictates the number of threads).
          \param enableProgressInterface Enable/disable the use of a progress interface when applicable.
          \return true if ok, false on errors
          \note Aboute scale and offset parametrs:  outValue = ( ( inputValue + offsetA ) * scale ) + offsetB
        */
        bool initialize( 
          te::rst::Raster& raster1, 
          const std::vector< unsigned int >& raster1Bands, 
          const te::rst::Raster& raster2, 
          const std::vector< unsigned int >& raster2Bands,
          const BlendMethod& blendMethod, 
          const te::rst::Interpolator::Method& interpMethod1,
          const te::rst::Interpolator::Method& interpMethod2,
          const double& noDataValue,
          const bool forceRaster1NoDataValue,
          const bool forceRaster2NoDataValue,
          const std::vector< double >& pixelOffsets1,
          const std::vector< double >& pixelScales1,
          const std::vector< double >& pixelOffsets2,
          const std::vector< double >& pixelScales2,
          te::gm::MultiPolygon const * const r1ValidDataDelimiterPtr,
          te::gm::MultiPolygon const * const r2ValidDataDelimiterPtr,
          const te::gm::GeometricTransformation& geomTransformation,
          const unsigned int threadsNumber,
          const bool enableProgressInterface );
        
        
        /*!
          \brief Blend a pixel value using the current parameters.
          \param line Line (raster 1 reference).
          \param col Column (raster 1 reference).
          \param values A pointer to a pre-allocated vector where the blended values will be stored.
          \note The caller of this method must be aware that the returned blended value may be outside the original input rasters valid values range.
          \note Raster 1 values have precedence over raster 2 values (when applicable).
        */
        inline void getBlendedValues( const double& line, const double& col, 
          double* const values )
        {
          TERP_DEBUG_TRUE_OR_THROW( m_blendFuncPtr, "Invalid blend function pointer" );
          (this->*m_blendFuncPtr)( line, col, values );
        };

        /*!
          \brief Execute blending of the given input rasters and write the result into raster1.
          \note Raster 1 values have precedence over raster 2 values (when applicable).
        */        
        bool blendIntoRaster1();

      protected:
        
        /*!
          \brief Type definition for the a bleding function pointer.
          \param line Raster 1 Line.
          \param col Raster 1 Column.
          \param values A pointer to a pre-allocated vector where the blended values will be stored.
          \note The caller of this method must be aware that the returned blended value may be outside the original input rasters valid values range.
          \note Raster 1 values have precedence over raster 2 values (when applicable).
        */      
        typedef void (Blender::*BlendFunctPtr)( const double& line, 
          const double& col, double* const values );   
        
        /*!
          \brief Raster block info
        */  
        struct RasterBlockInfo
        {
          bool m_wasProcessed;
          unsigned int m_blkX;
          unsigned int m_blkY;
          unsigned int m_blkTotalPixelsNumber;
          unsigned int m_firstRasterRow2Process;
          unsigned int m_rasterRows2ProcessBound;
          unsigned int m_firstRasterCol2Process;
          unsigned int m_rasterCols2ProcessBound;
        };
        
        /*! 
          \brief The parameters passed to blendIntoRaster1Thread method.
        */     
        class BlendIntoRaster1ThreadParams
        {
          public :
            
            bool* m_returnValuePtr; //!< A pointer to the threadreturn value.
            bool* m_abortValuePtr; //!< A pointer to the abort execution value.
            te::rst::RasterSynchronizer* m_sync1Ptr; //!< Raster 1 syncronizer pointer.
            te::rst::RasterSynchronizer* m_sync2Ptr; //!< Raster 1 syncronizer pointer.
            std::vector< RasterBlockInfo >* m_raster1BlocksInfosPtr; //!< blocks to process.
            boost::mutex* m_mutexPtr; //!< mutex pointer.
            boost::mutex* m_blockProcessedSignalMutexPtr; //!< Mutex used to update the main process progress update.
            boost::condition_variable* m_blockProcessedSignalPtr; //!< Signal used to update the main process progress update.            
            unsigned int* m_runningThreadsCounterPtr; //!< a pointer to the running threads counter.
            std::vector< unsigned int > m_raster1Bands; //!< Input raster 1 band indexes to use.
            std::vector< unsigned int > m_raster2Bands; //!< Input raster 2 band indexes to use (this vector has the same size as raster1Bands).
            BlendMethod m_blendMethod; //!<  The blend method to apply.
            te::rst::Interpolator::Method m_interpMethod2; //!< The interpolation method to use when reading raster 2 data.
            double m_noDataValue; //!< The value returned where there is no pixel data bo blend.
            bool m_forceRaster1NoDataValue; //!< Use noDataValue as the input rasters no-data value (The original rasters no-data values will be ignored)
            bool m_forceRaster2NoDataValue; //!< Use noDataValue as the input rasters no-data value (The original rasters no-data values will be ignored)  
            std::vector< double > m_pixelOffsets1; //!< The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
            std::vector< double > m_pixelScales1; //!< The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
            std::vector< double > m_pixelOffsets2; //!< The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
            std::vector< double > m_pixelScales2; //!< The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
            std::auto_ptr< te::gm::MultiPolygon > m_r1ValidDataDelimiterPtr; //!< A pointer to a geometry (raster 1 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
            std::auto_ptr< te::gm::MultiPolygon > m_r2ValidDataDelimiterPtr; //!< A pointer to a geometry (raster 2 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
            std::auto_ptr< te::gm::GeometricTransformation > m_geomTransformationPtr; //!< A transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 pixels ( te::gm::GTParameters::TiePoint::second ) (Note: all coords are indexed by lines/columns).
            unsigned long int m_maxRasterCachedBlocks; //!< The maximum number of raster cache blocks.
            bool m_useProgress; //!< If enabled each thread will use its own progress interface, if false only a signal will be emitted on each processed block.
            
            BlendIntoRaster1ThreadParams();
            
            BlendIntoRaster1ThreadParams( const BlendIntoRaster1ThreadParams& rhs );
            
            ~BlendIntoRaster1ThreadParams();
            
            BlendIntoRaster1ThreadParams& operator=( const BlendIntoRaster1ThreadParams& rhs );
        };           

        bool m_enableProgressInterface; //!< Enable progress interface.
        bool m_forceRaster1NoDataValue; //!<  Use noDataValue as the input raster 1 no-data value (The original rasters no-data values will be ignored)
        bool m_forceRaster2NoDataValue; //!<  Use noDataValue as the input raster 2 no-data value (The original rasters no-data values will be ignored)  
        unsigned int m_threadsNumber; //!< The number of threads to use (0:automatic , 1:disabled, any other integer dictates the number of threads)..
        BlendMethod m_blendMethod; //!< The blend method to apply.
        BlendFunctPtr m_blendFuncPtr; //!< The current blend function.
        te::rst::Raster * m_raster1Ptr; //!< Input raster 1.
        te::rst::Raster const * m_raster2Ptr; //!< Input raster 2.
        std::auto_ptr< te::gm::MultiPolygon > m_r1ValidDataDelimiterPtr; //!< A pointer to a geometry (raster 1 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
        std::auto_ptr< te::gm::MultiPolygon > m_r2ValidDataDelimiterPtr; //!< A pointer to a geometry (raster 2 world/projected coords) delimiting the raster region with valid data, or null if all raster data area is valid.
        std::auto_ptr< te::gm::Geometry > m_intersectionPtr; //!< The Intersection geometry ( raster 1 indexed coods).
        std::vector< std::pair< te::gm::Coord2D, te::gm::Coord2D > > m_r1IntersectionSegmentsPoints; //!< A sub-set of the intersection polygon wich is part of raster 1 valid data polygon ( raster 1 indexed coods).
        std::size_t m_r1IntersectionSegmentsPointsSize; //!< Size of m_r1IntersectionSegmentsPoints;
        std::vector< std::pair< te::gm::Coord2D, te::gm::Coord2D > > m_r2IntersectionSegmentsPoints; //!< A sub-set of the intersection polygon wich is part of raster 2 valid data polygon ( raster 1 indexed coods).
        std::size_t m_r2IntersectionSegmentsPointsSize; //!< Size of m_r2IntersectionSegmentsPoints;
        te::gm::GeometricTransformation* m_geomTransformationPtr; //!< A transformation mapping raster 1 pixels ( te::gm::GTParameters::TiePoint::first ) to raster 2 ( te::gm::GTParameters::TiePoint::second ) (Note: all coords are indexed by lines/columns).
        te::rst::Interpolator::Method m_interpMethod1; //!< The interpolation method to use when reading raster 1 data.
        te::rst::Interpolator::Method m_interpMethod2; //!< The interpolation method to use when reading raster 2 data.
        double m_outputNoDataValue; //!< The output raster no-data value.
        std::auto_ptr< te::rst::Interpolator > m_interp1Ptr; //!< Raster 1 interpolator instance pointer.
        std::auto_ptr< te::rst::Interpolator > m_interp2Ptr; //!< Raster 2 interpolator instance pointer.        
        std::vector< unsigned int > m_raster1Bands; //!< Input raster 1 band indexes to use.
        std::vector< unsigned int > m_raster2Bands; //!< Input raster 2 band indexes to use.
        std::vector< double > m_pixelOffsets1; //!< The values offset to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelScales1; //!< The values scale to be applied to raster 1 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelOffsets2; //!< The values offset to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_pixelScales2; //!< The values scale to be applied to raster 2 pixel values before the blended value calcule (one element for each used raster channel/band).
        std::vector< double > m_raster1NoDataValues; //!< Raster 1 no-data values (on value per band).
        std::vector< double > m_raster2NoDataValues; //!< Raster 2 no-data values (on value per band).
        boost::ptr_vector< te::rst::TileIndexer > m_intersectionTileIndexers; //!< The Intersection geometry tile indexers( raster 1 indexed coods), one indexer for each intersection polygon.
        
        // variables used by the noBlendMethodImp method
        double m_noBlendMethodImp_Point1XProj1;
        double m_noBlendMethodImp_Point1YProj1;
        double m_noBlendMethodImp_Point1XProj2;
        double m_noBlendMethodImp_Point1YProj2;
        double m_noBlendMethodImp_Point2Line;
        double m_noBlendMethodImp_Point2Col;        
        std::complex< double > m_noBlendMethodImp_cValue;
        double m_noBlendMethodImp_Value;
        unsigned int m_noBlendMethodImp_BandIdx;
        
        // variables used by the euclideanDistanceMethodImp method
        te::gm::Point m_euclideanDistanceMethodImp_auxPoint;
        double m_euclideanDistanceMethodImp_Point2Line;
        double m_euclideanDistanceMethodImp_Point2Col;        
        std::complex< double > m_euclideanDistanceMethodImp_cValue1;
        std::complex< double > m_euclideanDistanceMethodImp_cValue2;
        unsigned int m_euclideanDistanceMethodImp_BandIdx;   
        double m_euclideanDistanceMethodImp_currDist;
        double m_euclideanDistanceMethodImp_dist1;
        double m_euclideanDistanceMethodImp_dist2;
        std::size_t m_euclideanDistanceMethodImp_vecIdx;
        double m_euclideanDistanceMethodImp_aux1;
        double m_euclideanDistanceMethodImp_aux2;
        unsigned int m_euclideanDistanceMethodImp_IntersectionTileIndexersIdx;
        bool m_euclideanDistanceMethodImp_PointInsideIntersection;
        
        // variables used by the sumMethodImp method
        te::gm::Point m_sumMethodImp_auxPoint;
        double m_sumMethodImp_Point2Line;
        double m_sumMethodImp_Point2Col;        
        std::complex< double > m_sumMethodImp_cValue1;
        std::complex< double > m_sumMethodImp_cValue2;
        unsigned int m_sumMethodImp_BandIdx; 
        unsigned int m_sumMethodImp_IntersectionTileIndexersIdx;
        bool m_sumMethodImp_PointInsideIntersection;
        
        /*! \brief Reset the instance to its initial default state. */
        void initState();
        
        /*! \brief Clear all internal allocated resources. */
        void clear();
        
        /*! \brief Set the value of m_blendFuncPtr following the given blend method. */
        void setBlendFunctionPonter( const BlendMethod blendMethod );
        
        /*!
          \brief Implementation for NoBlendMethod.
          \param line Raster 1 Line.
          \param col Raster 1 Column.
          \param values A pointer to a pre-allocated vector where the blended values will be stored.
        */
        void noBlendMethodImp( const double& line1, const double& col1,
          double* const values );        

        /*!
          \brief Implementation for EuclideanDistanceMethod.
          \param line Raster 1 Line.
          \param col Raster 1 Column.
          \param values A pointer to a pre-allocated vector where the blended values will be stored.
        */
        void euclideanDistanceMethodImp( const double& line1, const double& col1,
          double* const values );
        
        /*!
          \brief Implementation for SumMethod.
          \param line Raster 1 Line.
          \param col Raster 1 Column.
          \param values A pointer to a pre-allocated vector where the blended values will be stored.
        */
        void sumMethodImp( const double& line1, const double& col1,
          double* const values );        
        
        /*!
          \brief Thread entry for the method blendIntoRaster1.

        */        
        static void blendIntoRaster1Thread( BlendIntoRaster1ThreadParams* paramsPtr );
        
        /*!
          \brief Extract segments from the given geometry.
          \param geometryPtr Input geometry.
          \param segments Found segments (appended here).
          \return true if OK, false on errors.
        */        
        bool getSegments( te::gm::Geometry const * const geometryPtr, 
          std::vector< std::pair< te::gm::Coord2D, te::gm::Coord2D > >& segments ) const;
          
        
        /*!
          \brief Creater polygon tile indexers from the given geometry (only if it is a polygon or a collection of polygons).
          \param geometryPtr Input geometry.
          \param tileIndexers Created indexers (appended here).
          \return true if OK, false on errors.
          \note No indexers will be returned for geometries other than polygons or collections of polygons.
        */        
        bool getTileIndexers( te::gm::Geometry const * const geometryPtr, 
          boost::ptr_vector< te::rst::TileIndexer >& tileIndexers ) const;          
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ALGORITHM_H

