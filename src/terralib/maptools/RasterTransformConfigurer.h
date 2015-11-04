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
  \file RasterTransformConfigurer.h

  \brief A Raster Transform configurer generates a Raster Transform given a RasterSymbolzier.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORMCONFIGURER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORMCONFIGURER_H

// TerraLib
#include "Config.h"
#include "RasterTransform.h"

// STL
#include <map>

namespace te
{
// Forward declarations
  namespace se 
  { 
    class ContrastEnhancement;
    class RasterSymbolizer;
    class SelectedChannel;
    class Interpolate;
    class Categorize;
    class Recode;
  }
  
  namespace rst 
  { 
    class Raster;
    class RasterProperty;
  }

  namespace map
  {
    /*!
      \class RasterTransformConfigurer

      \brief A Raster Transform configurer generates a Raster Transform given a RasterSymbolzier.

      \sa RasterTransform
    */
    class TEMAPEXPORT RasterTransformConfigurer
    {
      public:

        /*!
          \brief Constructor

          \param rs  Valid raster symbolizer pointer
          \param rt  Pointer to a raster transform object that will be configurated

          \note 
        */
        RasterTransformConfigurer(te::se::RasterSymbolizer* rs, te::map::RasterTransform* rt);

        /*! \brief Destructor. */
        ~RasterTransformConfigurer();

        /*! \brief Configure Transformation. */
        void configure();

      protected:

        /*! Function used to get the commun properties (gain, offset, opacity) */
        void getGeneralProperties();

        /*! Function used to get channel selection order */
        void getChannelSelection();

        /*! Function used to get gray channel properties */
        void getGrayChannelProperties(te::se::SelectedChannel* se);

        /*! Function used to get red channel properties */
        void getRedChannelProperties(te::se::SelectedChannel* se);

        /*! Function used to get green channel properties */
        void getGreenChannelProperties(te::se::SelectedChannel* se);

        /*! Function used to get blue channel properties */
        void getBlueChannelProperties(te::se::SelectedChannel* se);

        /*! Function used to get gamma value from a Contrast Enhancement */
        double getGammaProperty(te::se::ContrastEnhancement* ce);

        /*! Function used to get the color map information */
        void getColorMapInformation();

        /*! Function used to get the color map Interpolated information */
        void getInterpolatedMap(te::se::Interpolate* interpolate);

        /*! Function used to get the color map Categorized information */
        void getCategorizedMap(te::se::Categorize* caterogize);

        /*! Function used to get the color map Recoded information */
        void getRecodedMap(te::se::Recode* recode);


      private:

        te::se::RasterSymbolizer* m_rstSymbolizer;    //!< SE element that describes the raster style.
        te::map::RasterTransform* m_rstTransform;      //!< Raster transformation object.

    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RASTERTRANSFORMCONFIGURER_H
