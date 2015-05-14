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
  \file terralib/se/Fill.h

  \brief A Fill specifies the pattern for filling an area geometry.
*/

#ifndef __TERRALIB_SE_INTERNAL_FILL_H
#define __TERRALIB_SE_INTERNAL_FILL_H

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
// Forward declaration
    class Graphic;
    class SvgParameter;

    /*!
      \class Fill
      
      \brief A Fill specifies the pattern for filling an area geometry.

      Here, the SvgParameter names are "fill" instead of "stroke"
      and "fill-opacity" instead of "stroke-opacity".

      \ingroup se

      \sa PolygonSymbolizer, Graphic, FillBasicColor, Mark, TextSymbolizer, Halo
    */
    class TESEEXPORT Fill : public boost::noncopyable
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

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief Sets the GraphicFill element to this Fill.
                 GraphicFill defines that the pixels of the area will be drawn repeating an area-fill pattern.

          \note The Fill object will take the ownership of the informed fill pointer.
        */
        void setGraphicFill(Graphic* g);

        /*!
          \brief Gets the GraphicFill element associate to this Fill.
          
          \return The GraphicFill element.
        */
        const Graphic* getGraphicFill() const;

        /*!
          \brief Add a SvgParameter to this Fill.

          \note If there is already a SvgParamater with the same name it will be overrided.
          \note The Fill object will take the ownership of the informed p pointer.
        */
        void add(SvgParameter* p);

        void setColor(const std::string& color);

        void setOpacity(const std::string& opacity);

        const SvgParameter* getColor() const;

        const SvgParameter* getOpacity() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Fill* clone() const;

      private:

        void setParameter(const std::string& name, const std::string& value);

        const SvgParameter* getParameter(const std::string& name) const;

      private:

        Graphic* m_graphicFill;                            //!< Repeated graphic fill style. If omitted, then no fill will be rendered. (Optional)
        std::map<std::string, SvgParameter*> m_svgParams;  //!< Solid color style. (Optional)

        static const std::string sm_fill;                  //!< SVG/CSS "fill" parameter.
        static const std::string sm_opacity;               //!< SVG/CSS "fill-opacity parameter.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FILL_H
