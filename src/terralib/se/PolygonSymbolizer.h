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
  \file terralib/se/PolygonSymbolizer.h

  \brief A PolygonSymbolizer is used to draw a polygon (or other area-type geometries), including filling its interior and stroking its border (outline).
*/

#ifndef __TERRALIB_SE_INTERNAL_POLYGONSYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_POLYGONSYMBOLIZER_H

// TerraLib
#include "Symbolizer.h"

namespace te
{
// Forward declarations
  namespace fe { class PropertyName; }

  namespace se
  {
// Forward declarations
    class Displacement;
    class Fill;
    class ParameterValue;
    class Stroke;

    /*!
      \class PolygonSymbolizer
      
      \brief A PolygonSymbolizer is used to draw a polygon (or other area-type geometries), including filling its interior and stroking its border (outline).

      The Fill and Stroke elements are contained in the PolygonSymbolizer in the
      conceptual order that they are used and plotted using the "painters model",
      where the Fill will be rendered first, and then the Stroke will be rendered
      on top of the fill.
      A missing Stroke element means that the geometry will not be stroked.

      \ingroup se

      \sa Symbolizer, Fill, Stroke, te::fe::PropertyName, Displacement, ParameterValue, ImageOutline
    */
    class TESEEXPORT PolygonSymbolizer : public Symbolizer
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PolygonSymbolizer. */
        PolygonSymbolizer();

        /*!
          \brief Copy constructor.

          \param rhs The other polygon symbolizer.
        */
        PolygonSymbolizer(const PolygonSymbolizer& rhs);

        /*! \brief Destructor. */
        ~PolygonSymbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief

          The Geometry element of a PolygonSymbolizer defines the linear
          geometry to be used for styling. The Geometry element is
          optional and if it is absent then the all geometry properties
          of the feature type that is used in the containing FeatureType
          are used. Most frequently, though, feature types will have only
          a single geometry property. See OGC te::fe::PropertyName
          class for more information on attribute names.
          If a polygon has "holes", then they are not filled, but the
          borders around the holes are stroked in the usual way. "Islands"
          within holes are filled and stroked, and so on. If a point geometry
          is referenced instead of a polygon, then a small, square, ortho-normal
          polygon should be constructed for rendering. If a line is referenced,
          then the line (string) is closed for filling (only) by connecting its
          end point to its start point, any line crossings are corrected in
          some way, and only the original line is stroked. If a raster geometry
          is used, then the raster-coverage area is used as the polygon. A missing
          Geometry element selects the "default" geometry for a feature type.

          \note Geometry types other than inherently linear types can
                also be used. If a point geometry is used, it should be
                interpreted as a line of "epsilon" (arbitrarily small) length
                with a horizontal orientation centered on the point, and should
                be rendered with two end caps. If a polygon is used (or
                other "area" type), then its closed outline is used as the line
                string (with no end caps). If a raster geometry is used, its
                coverage-area outline is used for the line, rendered with no end caps.
        */
        void setGeometry(te::fe::PropertyName* geometry);

        const te::fe::PropertyName* getGeometry() const;

        /*!
          \brief A Fill specifies the pattern for filling an area geometry.

          The allowed SvgParameters are: "fill" (color) and "fill-opacity".
        */
        void setFill(Fill* f);

        /*!
          \brief Gets the Fill associates with the PolygonSymbolizer.

          \return The Fill element.
        */
        const Fill* getFill() const;

        /*!
          \brief A Stroke specifies the appearance of a linear geometry.

          A missing Stroke element means that the geometry will not be stroked.
        */
        void setStroke(Stroke* stroke);

        /*!
          \brief Gets the Stroke associates with the PolygonSymbolizer.

          \return The Stroke element.
        */
        const Stroke* getStroke() const;

        /*!
          \brief The Displacement gives the X and Y displacements from the original geometry.
          
          This element may be used to
          avoid over-plotting of multiple PolygonSymbolizers for
          one geometry or supplying "shadows" of polygon gemeotries.
          The displacements are in units of pixels above and to the
          right of the point. The default displacement is X = 0, Y = 0.

          \param d
        */
        void setDisplacement(Displacement* d);

        const Displacement* getDisplacement() const;

        /*!
          \brief PerpendicularOffset works as defined for LineSymbolizer, allowing to draw polygons smaller or larger than their actual geometry.
          
          The distance is in uoms and is positive
          to the outside of the polygon. Negative numbers mean drawing
          the polygon smaller. The default offset is 0.
        */
        void setPerpendicularOffset(ParameterValue* perpendicularOffset);

        const ParameterValue* getPerpendicularOffset() const;

        const std::string& getType() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Symbolizer* clone() const;

      private:

        te::fe::PropertyName* m_geometry;      //!< A Geometry gives reference to a (the) geometry property of a feature to be used for rendering. (Optional)
        Fill* m_fill;                          //!< Specifies how the area of the geometry will be filled. (Optional)
        Stroke* m_stroke;                      //!< A "Stroke" specifies the appearance of a linear geometry. (Optional)
        Displacement* m_displacement;          //!< The Displacement gives the X and Y displacements from the original geometry. (Optional)
        ParameterValue* m_perpendicularOffset; //!< A "PerpendicularOffset" gives the perpendicular distance away from a line to draw a label. (Optional)

        static const std::string sm_type;      //!< A static data member used in the implementation of getType method.
    };

  } // namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_POLYGONSYMBOLIZER_H
