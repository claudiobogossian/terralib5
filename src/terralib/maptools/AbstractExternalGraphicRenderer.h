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
  \file terralib/maptools/AbstractExternalGraphicRenderer.h

  \brief An abstract class for conversion of Symbology Enconding ExternalGraphic elements to an image pattern.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTEXTERNALGRAPHICRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTEXTERNALGRAPHICRENDERER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "Config.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class ExternalGraphic;
  }

  namespace map
  {
    /*!
      \class AbstractExternalGraphicRenderer

      \brief An abstract class for conversion of Symbology Enconding ExternalGraphic elements to an image pattern.
    */
    class TEMAPEXPORT AbstractExternalGraphicRenderer
    {
      public:

        /*! \brief Constructor. */
        AbstractExternalGraphicRenderer() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractExternalGraphicRenderer() { }

        /*!
          \brief It generates the image pattern from the given Symbology Enconding External Graphic element.

          \param eg     The Symbology Enconding External Graphic element that will be used to build the image pattern.
          \param height The height of image pattern that will be generated.
          \param width  An output parameter that will be filled with the width of generated image pattern.

          \return The RGBA image that represents the external graphic.

          \note If the given Symbology Enconding External Graphic element cannot be processed by the renderer the method should return a NULL pointer.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual te::color::RGBAColor** render(const te::se::ExternalGraphic* eg, std::size_t height, std::size_t& width) = 0;
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTEXTERNALGRAPHICRENDERER_H
