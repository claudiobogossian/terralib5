/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/Enums.h
 
  \brief Enumerations for the Raster module.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_ENUMS_H
#define __TERRALIB_RASTER_INTERNAL_ENUMS_H

namespace te
{
  namespace rst
  {
    /*!
      \enum SummaryTypes

      \brief Types for the BandSummary.
    */
    enum SummaryTypes
    {
      SUMMARY_MIN = 1,            //!< Calculate the min value.
      SUMMARY_MAX = 2,            //!< Calculate the max value.
      SUMMARY_STD = 4,            //!< Calculate the standard deviation value.
      SUMMARY_MEAN = 8,           //!< Calculate the mean value.
      SUMMARY_R_HISTOGRAM = 16,   //!< Calculate the histogram for the real part.
      SUMMARY_I_HISTOGRAM = 32,   //!< Calculate the histogram for the imaginary part.
      SUMMARY_ALL = 63            //!< Calculate all the summary types.
    };

    /*!
      \enum ColorInterp

      \brief Color model component use.
    */
    enum ColorInterp
    {
      UndefCInt,                  //!< No color interpretation is associated with the band.
      GrayIdxCInt,                //!< Index into a lookup table.
      PaletteIdxCInt,             //!< Palette indexes color interpretation.
      RedCInt,                    //!< Red channel color interpretation.
      GreenCInt,                  //!< Green channel color interpretation.
      BlueCInt,                   //!< Blue channel color interpretation.
      AlphaCInt,                  //!< Alpha channel color interpretation.
      HueCInt,                    //!< Hue channel color interpretation.
      SatCInt,                    //!< Saturation color interpretation.
      LigCInt,                    //!< Lightness color interpretation.
      CyanCInt,                   //!< Cyan color interpretation.
      MagentaCInt,                //!< Magenta color interpretation.
      YellowCInt,                 //!< Yellow color interpretation.
      KeyCInt,                    //!< Key (black) color interpretation.
      YCInt,                      //!< YCbCr Y Band color interpretation.
      CbCInt,                     //!< YCbCr Cb Band color interpretation.
      CrCInt                      //!< YCbCr Cr Band color interpretation.
    };

    /*!
      \enum PaletteInterpretation

      \brief Palette interpratation types.
    */
    enum PaletteInterpretation
    {
      UndefPalInt,                //!< Undefined palette interpretation.
      GrayPalInt,                 //!< Gray indexed palette interpretation.
      RGBPalInt,                  //!< RGB indexed palette interpretation. 
      CMYKPalInt,                 //!< CMYK indexed palette interpretation.
      HSLPalInt                   //!< HSL indexed palette interpretation. 
    };
    
    /*!
      \brief Allowed interpolation methods.
    */
    enum InterpolationMethod
    {
      UndefInterpMethod = 0,  //!< Undefined interpolation method.
      NearestNeighbor = 1,    //!< Near neighborhood interpolation method.
      Bilinear = 2,           //!< Bilinear interpolation method.
      Bicubic = 3             //!< Bicubic interpolation method.
    };    

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_ENUMS_H
