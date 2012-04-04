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
  \file TextSymbolizer.h
  
  \brief A TextSymbolizer is used to render text labels according to various graphical parameters.  
 */

#ifndef __TERRALIB_SE_INTERNAL_TEXTSYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_TEXTSYMBOLIZER_H

// TerraLib
#include "Symbolizer.h"

namespace te
{
// Forward declarations
  namespace fe { class PropertyName; }

  namespace se
  {
// Forward declarations
    class Fill;
    class Font;
    class Halo;
    class LabelPlacement;
    class ParameterValue;

    /*!
      \class TextSymbolizer
      
      \brief A TextSymbolizer is used to render text labels according to various graphical parameters.

      In this case, if a line, polygon, or raster geometry
      is used with this Symbolizer, then the semantic is to
      use the centroid of the geometry, or any similar
      representative point.

      \sa Symbolizer, ParameterValue, Font, LabelPlacement, Halo, Fill, te::fe::PropertyName

      \note If a Label element is not provided in a TextSymbolizer, then no text shall be rendered.
     */
    class TESEEXPORT TextSymbolizer : public Symbolizer
    {
      TE_DEFINE_VISITABLE

      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new TextSymbolizer. */
        TextSymbolizer();

        /*! \brief Destructor. */
        ~TextSymbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setGeometry(te::fe::PropertyName* g);
        void setLabel(ParameterValue* l);
        void setFont(Font* f);
        void setLabelPlacement(LabelPlacement* l);
        void setHalo(Halo* h);
        void setFill(Fill* f);

        //@}

      private:

        te::fe::PropertyName* m_geometry;  //!< The Geometry will be interpreted as a point or a line as needed by the LabelPlacement. (Optional)
        ParameterValue* m_label;           //!< The label specifies the textual content to be rendered. (Optional)
        Font* m_font;                      //!< The font element specifies the text font to use. (Optional)
        LabelPlacement* m_labelPlacement;  //!< The label placement specifies where and how a text label should be rendered relative to a geometry. (Optional)
        Halo* m_halo;                      //!< A Halo is a type of Fill that is applied to the backgrounds of font glyphs. (Optional)
        Fill* m_fill;                      //!< It specifies how the text area will be filled. (Optional)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_TEXTSYMBOLIZER_H

