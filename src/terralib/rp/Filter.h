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
  \brief Implements a series of well-known filtering algorithms for images, linear and non-linear.
 */

#ifndef __TERRALIB_RP_INTERNAL_FILTER_H
#define __TERRALIB_RP_INTERNAL_FILTER_H

// TerraLib
#include "Algorithm.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace da
  {
    class DataSource;
  };

  namespace rst
  {
    class Raster;
    class Band;
  };

  namespace rp
  {
    /*!
      \class Filter
      \brief Implements a series of well-known filtering algorithms for images, linear and non-linear.
      \ingroup RPAlgorithms
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
            
            /*!
              \enum Allowed filter types.
            */
            enum FilterType
            {
              InvalidFilterT = 0,      //!< Invalid filter.
              SobelFilterT = 1,        //!< Sobel filter is an edge detector.
              MeanFilterT = 2,         //!< The resultant pixel will be the mean of pixels in the convolution window.
              ModeFilterT = 3,         //!< The resultant pixel will be the mode of pixels in the convolution window.
              MedianFilterT = 4,       //!< The resultant pixel will be the median of pixels in the convolution window.
              DilationFilterT = 5,     //!< The resultant pixel will be the highest pixel value in the convolution window.
              ErosionFilterT = 6,      //!< The resultant pixel will be the lowest pixel value in the convolution window.
              UserDefinedWindowT = 7,  //!< The user will define the weights of a convolution window.
            };

            FilterType m_type;                                  //!< The filter type to be applied.
            unsigned int m_windowH;                             //!< The height of the convolution window. (commonly 3, with W=3 to make a 3x3 window, and so on)
            unsigned int m_windowW;                             //!< The width of the convolution window.
            te::rst::Raster* m_inRasterPtr;                     //!< Input raster.
            std::vector<unsigned int> m_inRasterBands;          //!< Bands to be processed from the input raster.
            bool m_enableProgress;                              //!< Enable/Disable the progress interface (default:false).
            boost::numeric::ublas::matrix<double> m_window;     //!< User defined convolution window. (The size must be equal to m_windowH x m_windowW)

            InputParameters();

            ~InputParameters();

            //overload
            void reset() throw(te::rp::Exception);

            //overload
            const  InputParameters& operator=(const InputParameters& params);

            //overload
            AbstractParameters* clone() const;
        };

        /*!
          \class OutputParameters
          \brief Filter output parameters
          \details The result will be written to the raster instance pointed 
          by m_outRasterPtr (in this case the output bands must also be 
          passed by m_outRasterBands); or written to a new raster instance 
          created inside the given data source pointed by m_outDataSourcePtr 
          (in this case the data set name must be supplied - m_outDataSetName ).
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            te::rst::Raster* m_outRasterPtr;                              //!< A pointer to a valid initiated raster instance where the result must be written, leave NULL to create a new instance(in this case the other output parameters must be used).
            std::auto_ptr<te::rst::Raster> m_createdOutRasterPtr;         //!< A pointer to the created output raster instance, or an empty pointer empty if the result must be written to the raster pointed m_outRasterPtr.
            std::vector<unsigned int> m_outRasterBands;                   //!< Bands to be processed from the output raster.
            std::string m_createdOutRasterDSType;                         //!< Output raster data source type (as described in te::raster::RasterFactory ), leave empty if the result must be written to the raster pointed m_outRasterPtr.
            std::map<std::string, std::string> m_createdOutRasterInfo;    //!< The necessary information to create the raster (as described in te::raster::RasterFactory), leave empty if the result must be written to the raster pointed m_outRasterPtr.
            bool m_normalizeOutput;                                       //!< A flag to indicate that output raster will be normalized, by default [0, 255].

            OutputParameters();

            OutputParameters(const OutputParameters&);

            ~OutputParameters();

            //overload
            void reset() throw(te::rp::Exception);

            //overload
            const  OutputParameters& operator=(const OutputParameters& params);

            //overload
            AbstractParameters* clone() const;
        };

        Filter();

        ~Filter();

        //overload
        bool execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception);

        //overload
        void reset() throw(te::rp::Exception);

        //overload
        bool initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception);

        bool isInitialized() const;

      protected:

        Filter::InputParameters m_inputParameters;         //!< Filter input execution parameters.
        Filter::OutputParameters* m_outputParametersPtr;   //!< Filter input execution parameters.
        bool m_isInitialized;                              //!< Tells if this instance is initialized.

        /*!
          \brief Execute the Sobel filter following the internal parameters.
          \return true if OK, false on errors.
         */
        bool execSobelFilter();

        /*!
          \brief Execute the mean filter following the internal parameters.
          \return true if OK, false on errors.
        */
        bool execMeanFilter();

        /*!
          \brief Execute the mode filter following the internal parameters.
          \return true if OK, false on errors.
        */
        bool execModeFilter();

        /*!
          \brief Execute the median filter following the internal parameters.
          \return true if OK, false on errors.
        */
        bool execMedianFilter();

        /*!
          \brief Execute the dilation filter following the internal parameters.
          \return true if OK, false on errors.
        */
        bool execDilationFilter();

        /*!
          \brief Execute the erosion filter following the internal parameters.
          \return true if OK, false on errors.
        */
        bool execErosionFilter();

        /*!
          \brief Execute the convolution using some user defined window.
          \return true if OK, false on errors.
        */
        bool execUserDefinedFilter();
    };
  } // end namespace rp
}   // end namespace te

#endif // __TERRALIB_RP_INTERNAL_FILTER_H
