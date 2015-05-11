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
  \file terralib/se/ImageOutline.h

  \brief ImageOutline specifies how individual source rasters in a multi-raster set (such as a set of satellite-image scenes) should be outlined to make the individual-image locations visible.
*/

#ifndef __TERRALIB_SE_INTERNAL_IMAGEOUTLINE_H
#define __TERRALIB_SE_INTERNAL_IMAGEOUTLINE_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
// Forward declaration
    class Symbolizer;

    /*!
      \class ImageOutline
      
      \brief ImageOutline specifies how individual source rasters in a multi-raster set (such as a set of satellite-image scenes) should be outlined to make the individual-image locations visible.

      The ImageOutline element specifies that individual source
      rasters in a multi-raster set (such as a set of satellite-image
      scenes) should be outlined with either a LineSymbolizer or PolygonSymbolizer.
        
      \sa RasterSymbolizer, LineSymbolizer, PolygonSymbolizer
    */
    class TESEEXPORT ImageOutline : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ImageOutline. */
        ImageOutline();

        /*! \brief Destructor. */
        ~ImageOutline();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{
        
        void setSymbolizer(Symbolizer* s);

        Symbolizer* getSymbolizer() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        ImageOutline* clone() const;

      private:

        Symbolizer* m_symbol; //!< Mandatory.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_IMAGEOUTLINE_H
