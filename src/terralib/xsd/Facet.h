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
  \file Facet.h

  \brief This class can be used to model a facet (a constraint in the range of values).
*/

#ifndef __TERRALIB_XSD_INTERNAL_FACET_H
#define __TERRALIB_XSD_INTERNAL_FACET_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL 
#include <string>

namespace te
{
  namespace xsd
  {
    /*!
      \class Facet

      \brief This class can be used to model a facet (a constraint in the range of values).
    */
    class TEXSDEXPORT Facet
    {
      public:

        /*!
          \brief Constructor.
          
          \param fType Facet type.
          \param value Facet value.
        */
        Facet(FacetType fType, const std::string& value);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.

          \todo Implement!
        */
        Facet(const Facet& rhs);

        /*! \brief Destructor. */
        ~Facet();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
          
          \todo Implement!
        */
        Facet& operator=(const Facet& rhs);

        /*!
          \brief It returns the facet type code.

          \return The facet type code.
        */
        FacetType getType() const;

        /*!
          \brief It returns the facet type name.

          \return The facet type name.
        */
        const std::string& getName() const;

        /*!
          \brief It returns the facet value.

          \return The facet value.
        */
        const std::string& getValue();

      private:

        FacetType m_type;                   //!< The facet type.
        std::string m_value;                //!< The facet associated value.
        static std::string m_typeNames[12]; //!< The list of names for available facet types.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_FACET_H
