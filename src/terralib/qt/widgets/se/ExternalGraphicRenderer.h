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
  \file terralib/qt/widgets/se/ExternalGraphicRenderer.h

  \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding External Graphic elements to an image pattern.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_EXTERNALGRAPHICRENDERER_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_EXTERNALGRAPHICRENDERER_H

// TerraLib
#include "../../../maptools/AbstractExternalGraphicRenderer.h"
#include "../Config.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ExternalGraphicRenderer

        \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding External Graphic elements to an image pattern.

        \sa AbstractExternalGraphicRenderer
      */
      class TEQTWIDGETSEXPORT ExternalGraphicRenderer : public te::map::AbstractExternalGraphicRenderer
      {
        public:

          /*! \brief Constructor. */
          ExternalGraphicRenderer();

          /*! \brief Destructor. */
          ~ExternalGraphicRenderer();

          te::color::RGBAColor** render(const te::se::ExternalGraphic* eg, std::size_t height, std::size_t& width);
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_EXTERNALGRAPHICRENDERER_H
