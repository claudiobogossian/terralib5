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
  \file RasterTransform.h

  \brief A Raster Transform is a class that defines functions to transform a styled raster.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORM_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORM_H

// TerraLib
#include "Config.h"
#include "../color/RGBAColor.h"
#include "../color/ColorBar.h"

// STL
#include <map>

namespace te
{
// Forward declarations
  namespace se 
  { 
    class RasterSymbolizer;
  }
  
  namespace rst 
  { 
    class Raster;
    class RasterProperty;
  }

  namespace map
  {
// Forward declarations

    /*!
      \class RasterTransform

      \brief A Raster Transform is a class that defines functions to transform a styled raster.

      \sa 
    */
    class TEMAPEXPORT RasterTransform
    {
      public:

        typedef void (RasterTransform::*TransformFunction)(double, double, double, double);
        typedef te::color::RGBAColor (RasterTransform::*RGBAFunction)(double, double);

        typedef std::pair<double, double> RasterThreshold;

        typedef std::map<RasterThreshold, te::color::RGBAColor> CategorizedMap;
        typedef std::map<RasterThreshold, te::color::ColorBar> InterpolatedMap;

        //! The three channels of a display
        enum RGBChannels 
        { 
          RED_CHANNEL=0,
          GREEN_CHANNEL=1,
          BLUE_CHANNEL=2,
          ALPHA_CHANNEL=3
        };

        // The raster transform functions type
        enum RasterTransfFunctions
        {
          NO_TRANSF=0,
          MONO2THREE_TRANSF=1,
          EXTRACT2RGB_TRANSF=2,
          RED2THREE_TRANSF=3,
          GREEN2THREE_TRANSF=4,
          BLUE2THREE_TRANSF=5,
          CATEGORIZE_TRANSF=6,
          INTERPOLATE_TRANSF=7,
          BAND2BAND_TRANSF=8,
          EXTRACT2RGBA_TRANSF,
        };

      public:

        /*!
          \brief Constructor

          \param input   
          \param output  

          \note 
        */
        RasterTransform(te::rst::Raster* input, te::rst::Raster* output);

        /*! \brief Destructor. */
        ~RasterTransform();

        /*! \brief Gets the input raster. */
        te::rst::Raster* getInputRaster() { return m_rasterIn; }

        /*! \brief Gets the output raster. */
        te::rst::Raster* getOutputRaster() { return m_rasterOut; }

        /*! \brief Sets the transparency. */
        void setTransparency(double value) { m_transp = value; }

        /*! \brief Gets the transparency. */
        double getTransparency() { return m_transp; }

        /*! \brief Sets the gain. */
        void setGain(double value) { m_gain = value; }

        /*! \brief Gets the gain. */
        double getGain() { return m_gain; }

        /*! \brief Sets the offset. */
        void setOffset(double value) { m_offset = value; }

        /*! \brief Gets the offset. */
        double getOffset() { return m_offset; }

        /*! \brief Sets the constrast value for red band. */
        void setContrastR(double value) { m_rContrast = value; }

        /*! \brief Gets the constrast value for red band. */
        double getContrastR() { return m_rContrast; }

        /*! \brief Sets the constrast value for green band. */
        void setContrastG(double value) { m_gContrast = value; }

        /*! \brief Gets the constrast value for green band. */
        double getContrastG() { return m_gContrast; }

        /*! \brief Sets the constrast value for blue band. */
        void setContrastB(double value) { m_bContrast = value; }

        /*! \brief Gets the constrast value for blue band. */
        double getContrastB() { return m_bContrast; }

        /*! \brief Sets the constrast value for gray band. */
        void setContrastM(double value) { m_mContrast = value; }

        /*! \brief Gets the constrast value for gray band. */
        double getContrastM() { return m_mContrast; }

        /*! \brief Sets the rgb map values. */
        void setRGBMap(std::map<RGBChannels, short>& rgbMap);

        /*! Sets the mapping from a particular input band to a particular output channel */
        void setBChannelMapping(short bIn, RGBChannels bOut) { m_rgbMap[bOut] = bIn; }

        /*! Clears current mapping from bands to channel */
        void clearRGBMap() { m_rgbMap.clear(); }

        /*! Returns the mapping from a particular input band to a particular output channel */
        std::map<RGBChannels, short>& getRGBMap() { return m_rgbMap; }

        /*! Sets the mono band to be transformed */
        void setSrcBand(short n) { m_monoBand = n; }

        /*! Gets the mono band to be transformed */
        short getSrcBand() { return m_monoBand; }

        /*! Sets the destination of the mono band */
        void setDestBand(short n) { m_monoBandOut = n; }

        /*! Gets the destination of the mono band */
        short getDestBand() { return m_monoBandOut; }

        /*! Sets the categorize map information */
        void setCategorizedMap(CategorizedMap map) { m_categorizeMap = map; }

        /*! Gets the categorize map information */
        CategorizedMap& getCategorizedMap() { return m_categorizeMap; }

        /*! Sets the interpolate map information */
        void setInterpolatedMap(InterpolatedMap map) { m_interpolateMap = map; }

        /*! Gets the categorize map information */
        InterpolatedMap& getInterpolatedMap() { return m_interpolateMap; }

        /*! 
          \brief Set parameters of linear transformation
        
          \param vmin smallest input value
          \param vmax largest input value
          \param rmin smallest value of the output range
          \param rmax largest value of the output range

        */
        void setLinearTransfParameters(double vmin, double vmax, double rmin, double rmax);

        /*! Returns the identifier of the transformation function currently set */
        RasterTransfFunctions getTransfFunction();

        /*! Sets the associated transformation function from an identifier */
        void setTransfFunction(RasterTransfFunctions func);

        /*! Sets the transformation method to be used */
        void setTransfFunction(RasterTransform::TransformFunction transfFuncPtr) { m_transfFuncPtr = transfFuncPtr; }

        /*! Sets the transformation method to be used */
        void setRGBAFunction(RasterTransform::RGBAFunction transfFuncPtr) { m_RGBAFuncPtr = transfFuncPtr; }

        /*! Applies the selected transformation method */
        void apply(double icol, double ilin, double ocol, double olin) {(this->*m_transfFuncPtr)(icol,ilin,ocol,olin); }

        te::color::RGBAColor apply(double icol, double ilin){return (this->*m_RGBAFuncPtr)(icol,ilin); }

      protected:

        /*! This transformation repeats the value of the first band in input three bands of the output */
        void setMono2ThreeBand(double icol, double ilin, double ocol, double olin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        te::color::RGBAColor getMono2ThreeBand(double icol, double ilin);

        /*! This transformation is used to define a particular mapping from input bands to RGB channels */
        void setExtractRGB(double icol, double ilin, double ocol, double olin);

        /*! This transformation is used to define a particular mapping from input bands to RGB channels */
        te::color::RGBAColor getExtractRGB(double icol, double ilin);

        /*! This transformation is used to define a particular mapping from input bands to RGBA channels */
        void setExtractRGBA(double icol, double ilin, double ocol, double olin);

        /*! This transformation is used to define a particular mapping from input bands to RGBA channels */
        te::color::RGBAColor getExtractRGBA(double icol, double ilin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        void setRed2ThreeBand(double icol, double ilin, double ocol, double olin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        te::color::RGBAColor getRed2ThreeBand(double icol, double ilin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        void setGreen2ThreeBand(double icol, double ilin, double ocol, double olin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        te::color::RGBAColor getGreen2ThreeBand(double icol, double ilin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        void setBlue2ThreeBand(double icol, double ilin, double ocol, double olin);

        /*! This transformation repeats the value of the first band in input three bands of the output */
        te::color::RGBAColor getBlue2ThreeBand(double icol, double ilin);

        /*! This transformation get the value of the selected band in input  and set the categorized value in output bands of the output */
        void setCategorize(double icol, double ilin, double ocol, double olin);

        /*! This transformation get the value of the selected band in input  and get the categorized value */
        te::color::RGBAColor getCategorize(double icol, double ilin);

        /*! This transformation get the value of the selected band in input  and set the interpolated value in output bands of the output */
        void setInterpolate(double icol, double ilin, double ocol, double olin);

        /*! This transformation get the value of the selected band in input  and get the interpolated value */
        te::color::RGBAColor getInterpolate(double icol, double ilin);

        /*! This transformation repeats the value of the n band in input to b band in output */
        void setBand2Band(double icol, double ilin, double ocol, double olin);


        /*! Function used to adjust the value in raster range */
        void fixValue(double& value);

        /*! 
          \brief Function used to check if value is or not a valid value 
        
          \return True if the value is EQUAL to NoValue and false in other case.
        */
        bool checkNoValue(double& value, int band);

        /*! Function used to get the interpolated color given a pixel value */
        te::color::RGBAColor getInterpolatedColor(double value);

        /*! Function used to get the categorized color given a pixel value */
        te::color::RGBAColor getCategorizedColor(double value);

      private:

        te::rst::Raster* m_rasterIn;              //!< Pointer to a input raster.
        te::rst::Raster* m_rasterOut;             //!< Pointer to a output raster.

        double m_transp;                          //!< Transparency factor
        double m_gain;                            //!< Gain factor.
        double m_offset;                          //!< Offset factor.
        double m_rstMinValue;                     //!< Min value from input raster.
        double m_rstMaxValue;                     //!< Max value from input raster.
        double m_rContrast;                       //!< Contrast value used in red band.
        double m_gContrast;                       //!< Contrast value used in green band.
        double m_bContrast;                       //!< Contrast value used in blue band.
        double m_mContrast;                       //!< Contrast value used in gray band.

        std::map<RGBChannels, short> m_rgbMap;    //!< Map used to set the band order.
        short m_monoBand;                         //!< Value for the gray band.
        short m_monoBandOut;                      //!< Value for output gray band channel.

        TransformFunction m_transfFuncPtr;        //!< Function used in transformation operation.
        RGBAFunction m_RGBAFuncPtr;               //!< Function used in transformation operation.

        CategorizedMap m_categorizeMap;           //!< Attribute to define the categorized transformation.
        InterpolatedMap m_interpolateMap;         //!< Attribute to define the interpolated transformation.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORM_H
