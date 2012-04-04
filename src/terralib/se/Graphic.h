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
  \file Graphic.h
  
  \brief A Graphic is a graphic symbol with an inherent shape, color(s), and possibly size.
 */

#ifndef __TERRALIB_SE_INTERNAL_GRAPHIC_H
#define __TERRALIB_SE_INTERNAL_GRAPHIC_H

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
    class AnchorPoint;
    class Displacement;
    class ExternalGraphic;
    class Mark;
    class ParameterValue;

    /*!
      \class Graphic
      
      \brief A Graphic is a graphic symbol with an inherent shape, color(s), and possibly size.

      A graphic can be defined very informally as "a little picture" and
      can be of either a raster or vector-graphic source type.
      The default if neither an ExternalGraphic nor
      a Mark is specified is to use the default mark
      of a "square" with a 50%-gray fill and a black
      outline, with a size of 6 pixels, unless an
      explicit Size is specified.

      \sa Rule, PointSymbolizer, Stroke, GraphicStroke, Mark, ExternalGraphic, Displacement, ParameterValue, AnchorPoint
     */
    class TESEEXPORT Graphic
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Graphic. */
        Graphic();

        /*! \brief Destructor. */
        ~Graphic();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief 

          Graphics can either be referenced from an external URL
          in a common format (such as GIF or SVG). Multiple
          external URLs and marks may be referenced with the
          semantic that they all provide the equivalent graphic
          in different formats. The "hot spot" to use for positioning
          the rendering at a point must either be inherent in the
          external format or is defined to be the "central point" of
          the graphic, where the exact definition "central point" is
          system-dependent.
         */
        void add(ExternalGraphic* g);

        /*!
          \brief
         */
        void add(Mark* m);

        /*!
          \brief

          The Opacity element gives the opacity to use
          for rendering the graphic. It has the same
          semantics as the "stroke-opacity" and "fill-opacity"
          SvgParameter. The default value is 1.0.
         */
        void setOpacity(ParameterValue* value);

        /*!
          \brief

          The Size element gives the absolute size of
          the graphic in uoms encoded as a floating-point
          number. The default size for an object is
          context-dependent. Negative values are not allowed.

          The default size of an image format (such as GIF)
          is the inherent size of the image. The
          default size of a format without an
          inherent size (such as SVG which are
          not specially marked) is defined to be 16 pixels in
          height and the corresponding aspect in width.
          If a size is specified, the height of the graphic
          will be scaled to that size and the corresponding
          aspect will be used for the width. An expected common
          use case will be for image graphics to be on the order
          of 200 pixels in linear size and to be scaled to lower
          sizes. On systems that can resample these graphic
          images "smoothly", the results will be visually pleasing.
         */
        void setSize(ParameterValue* value);

        /*!
          \brief

          The Rotation element gives the rotation of
          a graphic in the clockwise direction about its
          center point in decimal degrees, encoded as a
          floating-point number. Negative values mean
          counter-clockwise rotation. The default value
          is 0.0 (no rotation). Note that there is no
          connection between source geometry types and
          rotations; the point used for plotting has no
          inherent direction. Also, the point within the
          graphic about which it is rotated is format
          dependent. If a format does not include an
          inherent rotation point, then the point of rotation
          should be the centroid.
         */
        void setRotation(ParameterValue* value);

        void setAnchorPoint(AnchorPoint* value);

        void setDisplacement(Displacement* value);

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other Graphic.
         */
        Graphic(const Graphic& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other Graphic.

          \return A reference for this.
         */
        Graphic& operator=(const Graphic& rhs);

        //@}

      private:

        std::vector<ExternalGraphic*> m_externalGraphics;  //!< An ExternalGraphic gives a reference to a raster or vector graphical object, either online or inline, in an externally-defined graphic format. (Optional)
        std::vector<Mark*> m_marks;     //!< A Mark specifies a geometric shape and applies coloring to it. (Optional)
        ParameterValue* m_opacity;      //!< The Opacity element gives the opacity to use for rendering the graphic. (Optional)
        ParameterValue* m_size;         //!< The Size element gives the absolute size of the graphic in uoms encoded as a floating-point number. (Optional)
        ParameterValue* m_rotation;     //!< The Rotation element gives the rotation of a graphic in the clockwise direction about its center point in decimal degrees, encoded as a floating-point number. Negative values mean counter-clockwise rotation. (Optional)
        AnchorPoint* m_anchorPoint;     //!< The AnchorPoint element of a PointSymbolizer gives the location inside of a Graphic to use for anchoring the graphic to the main-geometry point. (Optional)
        Displacement* m_displacement;   //!< The Displacement gives the X and Y displacements from the "hot-spot" point. (Optional)
    };
 

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_GRAPHIC_H

