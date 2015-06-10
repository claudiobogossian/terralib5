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
  \file terralib/se/LineSymbolizer.h

  \brief A LineSymbolizer is used to style a stroke along a linear geometry type, such as a string of line segments.
*/

#ifndef __TERRALIB_SE_INTERNAL_LINESYMBOLIZER_H
#define __TERRALIB_SE_INTERNAL_LINESYMBOLIZER_H

// TerraLib
#include "Symbolizer.h"

namespace te
{
  namespace fe { class PropertyName; }

  namespace se
  {
// Forward declarations
    class ParameterValue;
    class Stroke;

    /*!
      \class LineSymbolizer
      
      \brief A LineSymbolizer is used to style a stroke along a linear geometry type, such as a string of line segments.

      \ingroup se

      \sa Symbolizer, Stroke, ParameterValue, te::fe::PropertyName, ImageOutline
    */
    class TESEEXPORT LineSymbolizer : public Symbolizer
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new LineSymbolizer. */
        LineSymbolizer();

        /*!
          \brief Copy constructor.

          \param rhs The other line symbolizer.
        */
        LineSymbolizer(const LineSymbolizer& rhs);

        /*! \brief Destructor. */
        ~LineSymbolizer();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief

          The Geometry element of a LineSymbolizer defines the linear
          geometry to be used for styling. The Geometry element is
          optional and if it is absent then the all geometry properties
          of the feature type that is used in the containing FeatureType
          are used. Most frequently, though, feature types will have only
          a single geometry property. See OGC te::fe::PropertyName
          class for more information on attribute names.

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
          \brief 

          A Stroke specifies the appearance of a linear geometry.
          The Stroke element is optional inside of LineSymbolizer
          (and other Symbolizers), and its absence means that no
          stroke is to be rendered.
        */
        void setStroke(Stroke* stroke);

        const Stroke* getStroke() const;

        /*!
          \brief

          PerpendicularOffset allows to draw lines in parallel
          to the original geometry. For complex line strings
          these parallel lines have to be constructed so that
          the distance between original geometry and drawn line
          stays equal. This construction can result in drawn lines
          that are actually smaller or longer than the original
          geometry. The distance is in uoms and is positive
          to the left-hand side of the line string. Negative numbers
          mean right. The default offset is 0.
        */
        void setPerpendicularOffset(ParameterValue* perpendicularOffset);

        const ParameterValue* getPerpendicularOffset() const;

        const std::string& getType() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Symbolizer* clone() const;

      private:

        te::fe::PropertyName* m_geometry;       //!< A Geometry gives reference to a (the) geometry property of a feature to be used for rendering. (Optional)
        Stroke* m_stroke;                       //!< A Stroke specifies the appearance of a linear geometry. (Optional)
        ParameterValue* m_perpendicularOffset;  //!< A PerpendicularOffset gives the perpendicular distance away from a line to draw a label. (Optional)

        static const std::string sm_type;       //!< A static data member used in the implementation of getType method.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_LINESYMBOLIZER_H
