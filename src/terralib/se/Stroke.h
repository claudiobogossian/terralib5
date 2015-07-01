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
  \file terralib/se/Stroke.h

  \brief A Stroke specifies the appearance of a linear geometry.
*/

#ifndef __TERRALIB_SE_INTERNAL_STROKE_H
#define __TERRALIB_SE_INTERNAL_STROKE_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

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

      \ingroup se

      \sa LineSymbolizer, Graphic, GraphicStroke, Mark
    */
    class TESEEXPORT Stroke : public boost::noncopyable
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
          \brief Sets the GraphicFill element to this Stroke.
                 GraphicFill defines that the pixels of the line will be drawn repeating an area-fill pattern.

          \note The Stroke object will take the ownership of the informed fill pointer.
        */
        void setGraphicFill(Graphic* fill);

        /*!
          \brief Gets the GraphicFill element associate to this Stroke.
          
          \return The GraphicFill element.
        */
        const Graphic* getGraphicFill() const;

        /*!
          \brief Sets the GraphicStroke element to this Stroke.
                 GraphicStroke defines that a repeated linear graphic is plotted linearly
                 and has its graphic bent around the curves of the line string.

          \note The Stroke object will take the ownership of the informed stroke pointer.
        */
        void setGraphicStroke(GraphicStroke* stroke);

        /*!
          \brief Gets the GraphicStroke element associate to this Stroke.
          
          \return The GraphicStroke element.
        */
        const GraphicStroke* getGraphicStroke() const;

        /*!
          \brief Add a SvgParameter to this Stroke.

          \note If there is already a SvgParamater with the same name it will be overrided.
          \note The Stroke object will take the ownership of the informed p pointer.
        */
        void add(SvgParameter* p);

        void setColor(const std::string& color);

        void setOpacity(const std::string& opacity);

        void setWidth(const std::string& width);

        void setLineJoin(const std::string& join);

        void setLineCap(const std::string& cap);

        void setDashArray(const std::string& dasharray);

        void setDashOffset(const std::string& offset);

        const SvgParameter* getColor() const;

        const SvgParameter* getOpacity() const;

        const SvgParameter* getWidth() const;

        const SvgParameter* getLineJoin() const;

        const SvgParameter* getLineCap() const;

        const SvgParameter* getDashArray() const;

        const SvgParameter* setDashOffset() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Stroke* clone() const;

      private:

        void setParameter(const std::string& name, const std::string& value);

        const SvgParameter* getParameter(const std::string& name) const;

      private:

        Graphic* m_fill;                                   //!< The GraphicFill both indicates that a stipple-fill repeated graphic will be used and specifies the fill graphic. (Optional)
        GraphicStroke* m_stroke;                           //!< The GraphicStroke both indicates that a repeated-linear-graphic stroke type will be used. (Optional)
        std::map<std::string, SvgParameter*> m_svgParams;  //!< If neither a GraphicFill nor GraphicStroke is given, then the line Symbolizer will render a solid color. (Optional)

        static const std::string sm_stroke;                //!< SVG/CSS "stroke" parameter.
        static const std::string sm_opacity;               //!< SVG/CSS "stroke-opacity parameter.
        static const std::string sm_width;                 //!< SVG/CSS "stroke-width" parameter.
        static const std::string sm_linejoin;              //!< SVG/CSS "stroke-linejoin" parameter.
        static const std::string sm_linecap;               //!< SVG/CSS "stroke-linecap" parameter.
        static const std::string sm_dasharray;             //!< SVG/CSS "stroke-dasharray" parameter.
        static const std::string sm_dashoffset;            //!< SVG/CSS "stroke-dashoffset" parameter.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_STROKE_H

