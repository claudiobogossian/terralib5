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
  \file Text.h

  \brief An Text consists of a Envelope and a Text Element array.
 */

#ifndef __TERRALIB_ANNOTATIONTEXT_INTERNAL_TEXT_H
#define __TERRALIB_ANNOTATIONTEXT_INTERNAL_TEXT_H

// TerraLib
#include "Config.h"
#include "../geometry/Envelope.h"
#include "../common/STLUtils.h"

namespace te
{
// Forward declarations
  namespace gm
  {
    class Envelope;
  }

  namespace at
  {
    class Element;

    /*!
      \class Text

      \brief A Text may contain 1 or more Text Elements.
     */
    class TEANNOTATIONTEXTEXPORT Text
    {
      public:

        /*! \brief Text constructor. */
        Text();

        /*! \brief Text destructor. */
        ~Text();

        /*!
          \brief It clones the Text.

          \return A copy of the given Text.

          \note The caller of this method will take the ownership of the returned Text.
         */
        Text* clone();

        /*!
          \brief It sets the envelope.

          \param envelope The envelope.
         */
        void setEnvelope(const te::gm::Envelope& envelope);

        /*!
          \brief It returns the envelope.

          \return The envelope.
         */
        const te::gm::Envelope& getEnvelope();

        /*!
          \brief It sets the elements.

          \param envelope The envelope.

          \note The Text will become owner of elements.
         */
        void setElements(const std::vector<Element*> elements);

        /*!
          \brief It returns the elements.

          \return The elements.
         */
        const std::vector<Element*>& getElements() const;

        /*!
          \brief It adds the element.

          \param element The element to be added.

          \note The Text will become owner of the element.
         */
        void add(Element* element);

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other Text.
         */
        Text(const Text& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other Text.

          \return A reference for this.
         */
        Text& operator=(const Text& rhs);

      private:

        te::gm::Envelope      m_envelope; // envelope
        std::vector<Element*>  m_elements; // elements
    };

  } // end namespace at
}   // end namespace te

#endif  // __TERRALIB_ANNOTATIONTEXT_INTERNAL_TEXT_H

