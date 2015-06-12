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
  \file terralib/se/AnchorPoint.h

  \brief An AnchorPoint identifies the location inside of a text label to use an 'anchor' for positioning it relative to a point geometry.
*/

#ifndef __TERRALIB_SE_INTERNAL_ANCHORPOINT_H
#define __TERRALIB_SE_INTERNAL_ANCHORPOINT_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
// Forward declaration
    class ParameterValue;

    /*!
      \class AnchorPoint
      
      \brief An AnchorPoint identifies the location inside of a text label to use an 'anchor' for positioning it relative to a point geometry.

      The AnchorPoint element of a PointSymbolizer
      gives the location inside of a Graphic (or label)
      to use for anchoring the graphic to the
      main-geometry point. The coordinates are given as
      two floating-point numbers in the AnchorPointX and AnchorPointY
      elements each with values between 0.0 and 1.0 inclusive.
      The bounding box of the graphic/label to be rendered is considered
      to be in a coordinate space from 0.0 (lower-left corner) to
      1.0 (upper-right corner), and the anchor position is specified
      as a point in this space. The default point is X = 0.5, Y = 0.5,
      which is at the middle height and middle length of the
      graphic/label text. A system may choose different anchor
      points to de-conflict graphics/labels.

      \sa Graphic, ParameterValue, PointPlacement
    */
    class TESEEXPORT AnchorPoint : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new AnchorPoint. */
        AnchorPoint();

        /*! \brief Destructor. */
        ~AnchorPoint();

        //@}

         /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setAnchorPointX(ParameterValue* x);

        const ParameterValue* getAnchorPointX() const;

        void setAnchorPointY(ParameterValue* y);

        const ParameterValue* getAnchorPointY() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        AnchorPoint* clone() const;

      private:

        ParameterValue* m_x;  //!< Floating-point number of the the coordinate to anchoring (default = 0.5).
        ParameterValue* m_y;  //!< Floating-point number of the the coordinate to anchoring (default = 0.5).
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_ANCHORPOINT_H

