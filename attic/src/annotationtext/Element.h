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
  \file terralib/annotationtext/Element.h

  \brief An Text Element consists of a string, location, leader line and may have attribute.
 */

#ifndef __TERRALIB_ANNOTATIONTEXT_INTERNAL_ELEMENT_H
#define __TERRALIB_ANNOTATIONTEXT_INTERNAL_ELEMENT_H

// TerraLib
#include "../common/STLUtils.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
// Forward declarations
  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace at
  {
// Forward declarations
    class Attributes;

    /*!
      \class Text

      \brief A Text may contain 1 or more Text Elements.
     */
    class TEANNOTATIONTEXTEXPORT Element
    {
      public:

        /*! \brief Text Element constructor. */
        Element();

        /*! \brief Text Element constructor.

          \param value The string value.
          \param location The Text entry point in world coordinates.
          \param leaderLine The Text leader line in world coordinates.

          \note The Element will become owner of location and leaderLine pointers.
         */
        Element(const std::string& value,
                te::gm::Geometry* location = 0,
                te::gm::Geometry* leaderLine = 0);

        /*! \brief Text Element destructor. */
        ~Element();

        /*!
          \brief It clones the Element.

          \return A copy of the given Element.

          \note The caller of this method will take the ownership of the returned Element.
         */
        Element* clone() const;

        /*!
          \brief it sets the text string.

          \param value The text string.
         */
        void setValue(const std::string& value);

        /*!
          \brief it returns the text string.

          \return The text string.
         */
        const std::string& getValue() const;

        /*!
          \brief it sets the text location.

          \param location The text location.

          \note The Element will become owner of location pointer.
         */
        void setLocation(te::gm::Geometry* location);

        /*!
          \brief it returns the text location.

          \return The text location.
         */
        const te::gm::Geometry* getLocation() const;

        /*!
          \brief it sets the text leader line.

          \param leaderLine The text leader line.

          \note The Element will become owner of leaderLine pointer.
         */
        void setLeaderLine(te::gm::Geometry* leaderLine);

        /*!
          \brief it returns the text leader line.

          \return The text leader line.
         */
        const te::gm::Geometry* getLeaderLine() const;

        /*!
          \brief it sets the text attributes.

          \param att The text attributes.

          \note The Element will become owner of attribute pointer.
         */
        void setAttributes(Attributes* attribute);

        /*!
          \brief it returns the text attributes.

          \retrun The text attributes.
         */
        const Attributes* getAttributes() const;

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other element.
         */
        Element(const Element& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other element.

          \return A reference for this.
         */
        Element& operator=(const Element& rhs);

      private:

        std::string       m_value;      //!< text value
        te::gm::Geometry* m_location;   //!< location
        te::gm::Geometry* m_leaderLine; //!< leader line
        Attributes*       m_attribute;  //!< attributes
    };

  } // end namespace at
}   // end namespace te

#endif  // __TERRALIB_ANNOTATIONTEXT_INTERNAL_ELEMENT_H

