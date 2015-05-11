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
  \file terralib/se/Halo.h

  \brief A Halo is a type of Fill that is applied to the backgrounds of font glyphs.
*/

#ifndef __TERRALIB_SE_INTERNAL_HALO_H
#define __TERRALIB_SE_INTERNAL_HALO_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  { 
// Forward declarations
    class Fill;
    class ParameterValue;

    /*!
      \class Halo
      
      \brief A Halo is a type of Fill that is applied to the backgrounds of font glyphs.

      A Halo fills an extended area outside the glyphs of a rendered
      text label to make the label easier to read over a background.
      The Radius element gives the absolute size of a halo
      radius in pixels encoded as a floating-point number. The
      radius is taken from the outside edge of a font glyph to
      extend the area of coverage of the glyph (and the inside
      edge of "holes" in the glyphs). The halo of a text label
      is considered to be a single shape. The default radius is
      one pixel. Negative values are not allowed. The default
      halo fill is solid white (Color "#FFFFFF"). The glyph’s
      fill is plotted on top of the halo. The default font fill
      is solid black (Color "#000000"). If no Halo is selected
      in the containing TextSymbolizer, then no halo will be rendered.

      \sa TextSymbolizer, Fill, ParameterValue
    */
    class TESEEXPORT Halo : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Halo. */
        Halo();

        /*! \brief Destructor. */
        ~Halo();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setRadius(ParameterValue* radius);

        const ParameterValue* getRadius() const;

        void setFill(Fill* fill);

        const Fill* getFill() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Halo* clone() const;

      private:

        ParameterValue* m_radius;    //!< The Radius element gives the absolute size of a halo radius in pixels encoded as a floating-point number. The radius is taken from the outside edge of a font glyph to extend the area of coverage of the glyph (and the inside edge of 'holes' in the glyphs). The halo of a text label is considered to be a single shape. (Optional)
        Fill* m_fill;                //!< Specifies how the hallo text area will be filled. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_HALO_H
