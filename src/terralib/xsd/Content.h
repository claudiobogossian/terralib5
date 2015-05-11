/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Content.h

  \brief A base class for elements that are not definitions nor declarations.
         This class is just for creating a hierarchy for All, Choice, Group and Sequence classes.
*/

#ifndef __TERRALIB_XSD_INTERNAL_CONTENT_H
#define __TERRALIB_XSD_INTERNAL_CONTENT_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Content

      \brief A base class for elements that are not definitions nor declarations.
             This class is just for creating a hierarchy for All, Choice, Group and Sequence classes.
    */
    class TEXSDEXPORT Content
    {
      public:

        Content() {}

        virtual ~Content() {}

        virtual Content* clone() const = 0;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_CONTENT_H


