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
  \file Stroke.h
  
  \brief A Stroke specifies the appearance of a linear geometry.
 */

#ifndef __TERRALIB_SE_INTERNAL_STROKE_H
#define __TERRALIB_SE_INTERNAL_STROKE_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

// STL
#include <vector>

namespace te
{
  namespace se
  {
// Forward declarations
    class Graphic;
    class GraphicStroke;
    class SvgParameter;

    /*!
      \class Stroke
      
      \brief A Stroke specifies the appearance of a linear geometry.

      It is defined in parallel with SVG strokes.
      There are three basic types of strokes:
      <ul>
      <li>solid-color</li>
      <li>GraphicFill (stipple)</li>
      <li>repeated linear GraphicStroke</li>
      </ul>
      If neither a GraphicFill nor GraphicStroke element
      is given, then the line Symbolizer will render a solid color.

      \sa LineSymbolizer, Graphic, GraphicStroke, Mark
     */
    class TESEEXPORT Stroke
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Stroke. */
        Stroke();

        /*! \brief Destructor. */
        ~Stroke();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief

          \note The Stroke object will take the ownership of the informed fill pointer.
         */
        void setGraphicFill(Graphic* fill);

        const Graphic* getGraphicFill() const;

        /*!
          \brief

          \note The Stroke object will take the ownership of the informed stroke pointer.
         */
        void setGraphicStroke(GraphicStroke* stroke);

        const GraphicStroke* getGraphicStroke() const;

        void add(SvgParameter* p);

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
         */
        Stroke(const Stroke& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
         */
        Stroke& operator=(const Stroke& rhs);

        //@}

      private:

        Graphic* m_fill;                         //!< The GraphicFill both indicates that a stipple-fill repeated graphic will be used and specifies the fill graphic. (Optional)
        GraphicStroke* m_stroke;                 //!< The GraphicStroke both indicates that a repeated-linear-graphic stroke type will be used. (Optional)          
        std::vector<SvgParameter*> m_svgParams;  //!< If neither a GraphicFill nor GraphicStroke is given, then the line Symbolizer will render a solid color.  (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_STROKE_H

