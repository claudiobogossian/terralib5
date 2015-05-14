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
  \file terralib/se/GraphicStroke.h

  \brief A GraphicStroke defines a repeated-linear graphic pattern to be used for stroking a line.
*/

#ifndef __TERRALIB_SE_INTERNAL_GRAPHICSTROKE_H
#define __TERRALIB_SE_INTERNAL_GRAPHICSTROKE_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
// Forward declarations
    class Graphic;
    class ParameterValue;

    /*!
      \class GraphicStroke
      
      \brief A GraphicStroke defines a repeated-linear graphic pattern to be used for stroking a line.

      \sa Stroke, Graphic, ParameterValue
    */
    class TESEEXPORT GraphicStroke : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new GraphicStroke. */
        GraphicStroke();

        /*! \brief Destructor. */
        ~GraphicStroke();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It specifies the linear graphic.

          Proper stroking with a linear
          graphic requires two "hot-spot" points within the space of the graphic to indicate where
          the rendering line starts and stops. In the case of raster images with no special mark-up,
          this line will be assumed to be middle pixel row of the image, starting from the first pixel
          column and ending at the last pixel column.

          \param g The linear graphic.

          \note The GraphicStroke object will take the ownership of the informed Graphic pointer.
        */
        void setGraphic(Graphic* g);

        const Graphic* getGraphic() const;

        void removeGraphic();

        /*!
          \brief It specifies how far away the first graphic will be drawn relative to the start of the rendering line.

          \param initialGap The initial gap.

          \note The GraphicStroke object will take the ownership of the informed ParameterValue pointer.
        */
        void setInitialGap(ParameterValue* initialGap);

        const ParameterValue* getInitialGap() const;

        /*!
          \brief It sets the distance between two graphics.

          \param gap The initial gap.

          \note The GraphicStroke object will take the ownership of the informed ParameterValue pointer.
        */
        void setGap(ParameterValue* gap);

        const ParameterValue* getGap() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        GraphicStroke* clone() const;

      private:

        Graphic* m_graphic;            //!< It specifies the linear graphic. (Mandatory)
        ParameterValue* m_initialGap;  //!< It defines the initial empty space, before the first Graphic or Label should be rendered. (Optional)
        ParameterValue* m_gap;         //!< It defines the empty space between two Graphics or Labels. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_GRAPHICSTROKE_H
