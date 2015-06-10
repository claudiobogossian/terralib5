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
  \file terralib/maptools/AbstractMarkRenderer.h

  \brief An abstract class for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKRENDERER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "Config.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class Mark;
  }

  namespace map
  {
    /*!
      \class AbstractMarkRenderer

      \brief An abstract class for conversion of Symbology Enconding Mark elements to an image pattern.
    */
    class TEMAPEXPORT AbstractMarkRenderer
    {
      public:

        /*! \brief Constructor. */
        AbstractMarkRenderer() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractMarkRenderer() { }

        /*!
          \brief It generates the image pattern from the given Symbology Enconding Mark element.

          \param mark The Symbology Enconding Mark element that will be used to build the image pattern.
          \param size The size of image pattern that will be generated.

          \return The RGBA image that represents the mark.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual te::color::RGBAColor** render(const te::se::Mark* mark, std::size_t size) = 0;

        /*!
          \brief Method that informs the set of supported marks by a specific concrete mark renderer.

          \param marks A pre-created vector of string that will be filled with the supported marks names.
        */
        virtual void getSupportedMarks(std::vector<std::string>& marks) const = 0;
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMARKRENDERER_H
