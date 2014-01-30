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
  \file terralib/rp/Filter.h
  \brief A series of well-known filtering algorithms for images, linear and non-linear.
 */

#ifndef __TERRALIB_RP_INTERNAL_FILTER_H
#define __TERRALIB_RP_INTERNAL_FILTER_H

#include "Algorithm.h"
#include "../raster/Raster.h"

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class Filter
      \brief A series of well-known filtering algorithms for images, linear and non-linear..
      \ingroup rp_enh
     */
    class TERPEXPORT Filter : public Algorithm
    {
      public:

        /*!
          \class InputParameters
          \brief Filter input parameters
         */
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:

            /*! \enum The edge filter type.*/
            enum FilterType
            {
              InvalidFilterT = 0,     //!< Invalid strategy.
              SobelFilterT = 1,       //!< Sobel filter type.
              RobertsFilterT = 2,     //!< Roberts filter type.
              MeanFilterT = 3,        //!< The resultant pixel will be the mean of pixels in the convolution window.
              ModeFilterT = 4,        //!< The resultant pixel will be the mode of pixels in the convolution window. When the window is multimodal, the first mode will be assumed.
              MedianFilterT = 5,      //!< The resultant pixel will be the median of pixels in the convolution window.
              DilationFilterT = 6,    //!< The resultant pixel will be the highest pixel value in the convolution window.
              ErosionFilterT = 7,     //!< The resultant pixel will be the lowest pixel value in the convolution window.
              UserDefinedWindowT = 8, //!< The user will define the weights of a convolution window.
            };

            FilterType m_filterType; //!< The edge filter type.

            te::rst::Raster const* m_inRasterPtr; //!< Input raster.

            std::vector< unsigned int > m_inRasterBands; //!< Bands to be used from the input raster 1.

            unsigned int m_iterationsNumber; //!< The number of iterations to perform (default:1).

            unsigned int m_windowH; //!< The height of the convolution window. (commonly 3, with W=3 to make a 3x3 window, and so on)

            unsigned int m_windowW; //!< The width of the convolution window.

            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).

            boost::numeric::ublas::matrix<double> m_window; //!< User defined convolution window. (The size must be equal to m_windowH x m_windowW)

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
          \class OutputParameters
          \brief Filter output parameters
         */
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:

            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).

            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the raster (as described in te::raster::RasterFactory).

            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< A pointer the ge generated output raster (label image).

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

        Filter();

        ~Filter();

        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );

        //overload
        void reset() throw( te::rp::Exception );

        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );

        //overload
        bool isInitialized() const;

      protected:

        /*!
          \brief Type definition for a filter method pointer.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
          \return true if ok, false on errors.
         */
        typedef bool (Filter::*FilterMethodPointerT)( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress );

        bool m_isInitialized; //!< Is this instance already initialized?

        Filter::InputParameters m_inputParameters; //!< Input parameters.

        /*!
          \brief Applay the Roberts filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool RobertsFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the Sobel filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool SobelFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the mean filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool MeanFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the mode filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool ModeFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the median filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool MedianFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the dilation filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool DilationFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the erosion filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool ErosionFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Applay the user defined filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.
          \param useProgress if true, the progress interface must be used.
         */
        bool UserDefinedFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx, const bool useProgress  );

        /*!
          \brief Returns true if i < j.
          \return Returns true if i < j.
         */
        static bool OrderFunction(double i, double j);
    };

  } // end namespace rp
}   // end namespace te

#endif

