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
  \file terralib/se/ContrastEnhancement.h

  \brief ContrastEnhancement defines the 'stretching' of contrast for a channel of a false-color image or for a whole grey/color image.
*/

#ifndef __TERRALIB_SE_INTERNAL_CONTRASTENHANCEMENT_H
#define __TERRALIB_SE_INTERNAL_CONTRASTENHANCEMENT_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  { 
    /*!
      \class ContrastEnhancement
      
      \brief ContrastEnhancement defines the 'stretching' of contrast for a channel of a false-color image or for a whole grey/color image.
        
      Contrast enhancement is used to make ground features
      in images more visible.
      In the case of a color image, the relative grayscale brightness
      of a pixel color is used. Normalize means to stretch the contrast
      so that the dimmest color is stretched to black and the brightest
      color is stretched to white, with all colors in between stretched
      out linearly. Histogram means to stretch the contrast based on
      a histogram of how many colors are at each brightness level on
      input, with the goal of producing equal number of pixels in the
      image at each brightness level on output. This has the effect of
      revealing many subtle ground features. A GammaValue tells how much
      to brighten (values greater than 1.0) or dim (values less than 1.0)
      an image. The default GammaValue is 1.0 (no change). If none of
      Normalize, Histogram, or GammaValue are selected in a ContrastEnhancement,
      then no enhancement is performed.

      \ingroup se

      \sa SelectedChannel, RasterSymbolizer
    */
    class TESEEXPORT ContrastEnhancement : public boost::noncopyable
    {
      public:

        /*!
          \enum ContrastEnhancementType

          \brief The type of contrast enhancement.
        */
        enum ContrastEnhancementType
        {
          ENHANCEMENT_NORMALIZE,  /*!< It means to stretch the contrast so that
                                       the dimmest color is stretched to black
                                       and the brightest color is stretched to
                                       white, with all colors in between stretched
                                       out linearly.                                   */
          ENHANCEMENT_HISTOGRAM,  /*!< It means to stretch the contrast based on a
                                       histogram of how many colors are at each
                                       brightness level on input, with the goal of
                                       producing equal number of pixels in the image
                                       at each brightness level on output. This has
                                       the effect of revealing many subtle ground
                                       features.                                       */
           ENHANCEMENT_NONE       /*!< No contrast enhancement must be applied.        */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ContrastEnhancement. */
        ContrastEnhancement();

        /*! \brief Destructor. */
        ~ContrastEnhancement();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setContrastEnhancementType(ContrastEnhancementType t);

        ContrastEnhancementType getContrastEnhancementType() const { return m_contrast; } 

        void setGammaValue(const double& v);

        double getGammaValue() const { return m_gammaValue; }

        //@}

        /*! \brief It creates a new copy of this object. */
        ContrastEnhancement* clone() const;

      private:

        ContrastEnhancementType m_contrast;  //!< Contrast enhancement type. (Optional)
        double m_gammaValue;                 //!< It tells how much to brighten (values greater than 1.0) or dim (values less than 1.0) an image. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_CONTRASTENHANCEMENT_H
