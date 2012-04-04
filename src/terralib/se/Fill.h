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
  \file Fill.h
  
  \brief A Fill specifies the pattern for filling an area geometry.
 */

#ifndef __TERRALIB_SE_INTERNAL_FILL_H
#define __TERRALIB_SE_INTERNAL_FILL_H

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
// Forward declaration
    class Graphic;
    class SvgParameter;

    /*!
      \class Fill
      
      \brief A Fill specifies the pattern for filling an area geometry.

      Here, the SvgParameter names are "fill" instead of "stroke"
      and "fill-opacity" instead of "stroke-opacity".

      \sa PolygonSymbolizer, Graphic, FillBasicColor, Mark, TextSymbolizer, Halo
     */
    class TESEEXPORT Fill
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Fill. */
        Fill();

        /*! \brief Destructor. */
        ~Fill();

        //@}

        /*There are two types of fills, solid-color and repeated GraphicFill. (Optional)*/

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setGraphicFill(Graphic* g);

        void add(SvgParameter* p);

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other Fill.
         */
        Fill(const Fill& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other Fill.

          \return A reference for this.
         */
        Fill& operator=(const Fill& rhs);

        //@}

      private:

        Graphic* m_graphicFill;               //!< Repeated graphic fill style. If omitted, then no fill will be rendered. (Optional)
        std::vector<SvgParameter*> m_params;  //!< Solid color style. (Optional)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FILL_H

