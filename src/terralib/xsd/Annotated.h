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
  \file Annotated.h

  \brief A base class for XSD classes that may allow annotation.
*/

#ifndef __TERRALIB_XSD_INTERNAL_ANNOTATED_H
#define __TERRALIB_XSD_INTERNAL_ANNOTATED_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace xsd
  {
// Forward declaration
    class Annotation;

    /*!
      \class Annotated

      \brief A base class for XSD classes that may allow annotation.

      \sa Annotation, Attribute
    */
    class TEXSDEXPORT Annotated
    {
      public:

        /*!
          \brief It returns the associated annotation.

          \return The associated annotation.
        */
        Annotation* getAnnotation() const;

        /*!
          \brief It sets the associated annotation.

          \param ann The annotation to be associated to the object. The annotated class will take the ownership of the given pointer.
        */
        void setAnnotation(Annotation* ann);

      protected:

        /*!
          \brief Constructor.
          
          \param ann The annotation to be associated to the object.

          \note The Annotation object will take the ownership of the informed pointer.
        */
        Annotated(Annotation* ann = 0);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Annotated(const Annotated& rhs);

        /*! \brief Destructor. */
        ~Annotated();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Annotated& operator=(const Annotated& rhs);

      private:

        Annotation* m_annotation; //!< Annotation for human reading or machine consumption. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_ANNOTATED_H

