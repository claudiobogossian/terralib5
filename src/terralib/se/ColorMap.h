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
  \file terralib/se/ColorMap.h

  \brief A ColorMap defines either the colors of a pallette-type raster source or the mapping of numeric pixel values to colors.
*/

#ifndef __TERRALIB_SE_INTERNAL_COLORMAP_H
#define __TERRALIB_SE_INTERNAL_COLORMAP_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  { 
// Forward declarations
    class Categorize;
    class Interpolate;
    class Recode;

    /*!
      \class ColorMap
      
      \brief A ColorMap defines either the colors of a pallette-type raster source or the mapping of numeric pixel values to colors.

      The ColorMap element defines the mapping of palette-type
      raster colors or fixed-numeric pixel values to colors
      using an Interpolate or Categorize SE function. The LookUpValue
      is in this case set to Rasterdata.
      For example, a DEM raster giving elevations in meters above
      sea level can be translated to a colored image with a ColorMap
      using a Categorize function.

      \ingroup se

      \sa RasterSymbolizer, Categorize, Interpolate
    */
    class TESEEXPORT ColorMap : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ColorMap. */
        ColorMap();

        /*!
          \brief Copy constructor.

          \param rhs The other color map.
        */
        ColorMap(const ColorMap& rhs);

        /*! \brief Destructor. */
        ~ColorMap();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setCategorize(Categorize* c);

        Categorize* getCategorize() const;

        void setInterpolate(Interpolate* i);

        Interpolate* getInterpolate() const;

        void setRecode(Recode* i);

        Recode* getRecode() const;

        //@}

        ColorMap* clone() const;

      private:

        Categorize*  m_categorize;  //!< Categorize function. (Mandatory if interpolate_ is not defined or empty otherwise)
        Interpolate* m_interpolate; //!< Interpolate function. (Mandatory if categorize_ is not defined or empty otherwise)
        Recode*      m_recode; //!< Recode function.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_COLORMAP_H
