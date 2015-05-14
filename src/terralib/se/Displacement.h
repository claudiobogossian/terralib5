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
  \file terralib/se/Displacement.h

  \brief A Displacement gives X and Y offset displacements to use for rendering a text label, graphic or other Symbolizer near a point.
*/

#ifndef __TERRALIB_SE_INTERNAL_DISPLACEMENT_H
#define __TERRALIB_SE_INTERNAL_DISPLACEMENT_H

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
      \class Displacement
      
      \brief A Displacement gives X and Y offset displacements to use for rendering a text label, graphic or other Symbolizer near a point.

      This element may be used to avoid over-plotting of
      multiple graphic symbols used as part of the same point
      symbol. The displacements are in units of measure above
      and to the right of the point. The default displacement
      is X = 0, Y = 0.
      <br>
      If Displacement is used in conjunction with Size and/or Rotation
      then the graphic symbol shall be scaled and/or rotated before it is displaced.

      \sa Graphic, ParameterValue, PointPlacement
    */
    class TESEEXPORT Displacement : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Displacement. */
        Displacement();

        /*! \brief Destructor. */
        ~Displacement();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setDisplacementX(ParameterValue* x);

        const ParameterValue* getDisplacementX() const;

        void setDisplacementY(ParameterValue* y);

        const ParameterValue* getDisplacementY() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Displacement* clone() const;

      private:

        ParameterValue* m_x;    //!< It gives the X offset displacement.
        ParameterValue* m_y;    //!< It gives the Y offset displacement.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_DISPLACEMENT_H

