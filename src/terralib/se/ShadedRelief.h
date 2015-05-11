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
  \file terralib/se/ShadedRelief.h

  \brief ShadedRelief specifies the application of relief shading (or "hill shading") to a DEM raster to give it somewhat of a three-dimensional effect and to make elevation changes more visible.
*/

#ifndef __TERRALIB_SE_INTERNAL_SHADEDRELIEF_H
#define __TERRALIB_SE_INTERNAL_SHADEDRELIEF_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
    /*!
      \class ShadedRelief
      
      \brief ShadedRelief specifies the application of relief shading (or "hill shading") to a DEM raster to give it somewhat of a three-dimensional effect and to make elevation changes more visible.

      Exact parameters of the shading are system-dependent (for now).
      If the BrightnessOnly flag is "0" or "false" (false, default),
      the shading is applied to the layer being rendered as the current
      RasterSymbolizer. If BrightnessOnly is "1" or "true" (true),
      the shading is applied to the brightness of the colors
      in the rendering canvas generated so far by other layers,
      with the effect of relief-shading these other layers. The
      default for BrightnessOnly is "0" (false). The ReliefFactor
      gives the amount of exaggeration to use for the height of
      the "hills". A value of around 55 (times) gives reasonable results
      for Earth-based DEMs. The default value is system-dependent.
        
      \sa RasterSymbolizer
    */
    class TESEEXPORT ShadedRelief : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ShadedRelief. */
        ShadedRelief();

        /*! \brief Destructor. */
        ~ShadedRelief();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{
        
        /*! \Set brigtness parameter. */
        void setBrightnessOnly(bool b);

        /*! \Get brigtness parameter. */
        bool isBrightnessOnly() const;

        /*! \Set relief factor parameter. */
        void setReliefFactor(const double& rf);

        /*! \Get relief factor parameter. */
        double getReliefFactor() const;
        
        //@}

        /*! \brief It creates a new copy of this object. */
        ShadedRelief* clone() const;

      private:

        bool m_brightnessOnly;  //!< It may be 0 (false) or 1 (true). (Optional)
        double m_reliefFactor;  //!< It gives the amount of exaggeration to use for the height of the "hills". (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SHADEDRELIEF_H
