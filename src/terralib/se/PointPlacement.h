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
  \file terralib/se/PointPlacement.h

  \brief A PointPlacement specifies how a text label should be rendered relative to a geometric point.
*/

#ifndef __TERRALIB_SE_INTERNAL_POINTPLACEMENT_H
#define __TERRALIB_SE_INTERNAL_POINTPLACEMENT_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  { 
// Forward declarations
    class AnchorPoint;
    class Displacement;
    class ParameterValue;

    /*!
      \class PointPlacement
      
      \brief A PointPlacement specifies how a text label should be rendered relative to a geometric point.

      For a PointPlacement, the anchor point of
      the label and a linear displacement from the
      point can be specified, to allow a graphic
      Symbolizer to be plotted directly at the point.
      This might be useful to label a city, for example.
      The AnchorPoint element of a PointPlacement gives
      the location inside of a label to use for anchoring
      the label to the main-geometry point. The Displacement
      element of a PointPlacement gives the X and Y
      displacements from the main-geometry point to
      render a text label.
      This will often be used to avoid over-plotting
      a graphic symbol marking a city or some such
      feature. The displacements are in units of pixels
      above and to the right of the point. A system may
      reflect this displacement about the X and/or Y axes
      to de-conflict labels. The default displacement is
      X = 0, Y = 0.
      The Rotation of a PointPlacement gives the clockwise
      rotation of the label in degrees from the normal
      direction for a font (left-to-right for
      Latin-derived human languages at least).

      \sa LabelPlacement, AnchorPoint, Displacement, ParameterValue
    */
    class TESEEXPORT PointPlacement : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PointPlacement. */
        PointPlacement();

        /*! \brief Destructor. */
        ~PointPlacement();

        //@}

         /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setAnchorPoint(AnchorPoint* a);

        const AnchorPoint* getAnchorPoint() const { return m_anchorPoint; }

        void setDisplacement(Displacement* d);

        const Displacement* getDisplacement() const { return m_displacement; }

        void setRotation(ParameterValue* r);

        const ParameterValue* getRotation() const { return m_rotation; }

        //@}

        /*! \brief It creates a new copy of this object. */
        PointPlacement* clone() const;

      private:

        AnchorPoint*    m_anchorPoint;    //!< It identifies the location inside of a text label to use an an 'anchor' for positioning it relative to a point geometry. (Optional)
        Displacement*   m_displacement;   //!< It gives X and Y offset displacements to use for rendering a text label, graphic or other Symbolizer near a point. (Optional)
        ParameterValue* m_rotation;       //!< The Rotation element gives the rotation of a graphic in the clockwise direction about its center point in decimal degrees, encoded as a floating-point number. Negative values mean counter-clockwise rotation. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_POINTPLACEMENT_H
