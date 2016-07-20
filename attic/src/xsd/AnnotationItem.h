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
  \file AnnotationItem.h

  \brief A base class for XSD annotation elements.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ANNOTATIONITEM_H
#define __TERRALIB_XSD_INTERNAL_ANNOTATIONITEM_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class AnnotationItem

      \brief A base class for XSD annotation elements.

      \sa Annotation, AppInfo, Documentation

      \note Parent element: Annotation.
    */
    class TEXSDEXPORT AnnotationItem
    {
      public:

        /*! \brief Constructor. */
        AnnotationItem() { }

        /*! \brief Destructor. */
        virtual ~AnnotationItem() { }

        /*!
          \brief It creates a clone of the annotation item.

          \return A clone of the annotation item. The client of this method will take the ownership of the clone.
        */
        virtual AnnotationItem* clone() const = 0;
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ANNOTATIONITEM_H

