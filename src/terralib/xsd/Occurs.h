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
  \file Occurs.h

  \brief A base class for XSD classes that have occurs attributes.
*/

#ifndef __TERRALIB_XSD_INTERNAL_OCCURS_H
#define __TERRALIB_XSD_INTERNAL_OCCURS_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace xsd
  {
    /*!
      \class Occurs

      \brief A base class for XSD classes that have occurs attributes.

      \sa All, Any, Choice, Group, LocalElement, Sequence
    */
    class TEXSDEXPORT Occurs
    {
      public:

        /*!
          \brief It returns the minOccurs values.

          \return The minOccurs value.
        */
        const unsigned int getMinOccurs() const;

        /*!
          \brief It returns the maxOccurs values.

          \return The maxOccurs value.
        */
        const unsigned int getMaxOccurs() const;

        /*!
          \brief It sets the minOccurs value.

          \param minOccurs The minOccurs value. The value can be any number >= 0.
        */
        void setMinOccurs(unsigned int minOccurs);

        /*!
          \brief It sets the maxOccurs value.

          \param maxOccurs The maxOccurs value. The value can be any number >= 0.

          \note If you want to set no limit on the maximum number, use the value "unbounded" defined as te::xsd::Occurs::unbounded.
        */
        void setMaxOccurs(unsigned int maxOccurs);

      protected:

        /*!
          \brief Constructor.
          
          \param minOccurs The minimum number of times the element can occur.
          \param maxOccurs The maximum number of times the element can occur.
        */
        Occurs(unsigned int minOccurs = 1, unsigned int maxOccurs = 1);

        /*!
          \brief Copy constructor.

          \param rhs Right-hand-side object.
        */
        Occurs(const Occurs& rhs);

        /*! \brief Destructor. */
        ~Occurs();

        /*!
          \brief Assignment operator.

          \param rhs Right-hand-side object.

          \return A reference to this object.
        */
        Occurs& operator=(const Occurs& rhs);

      public: 
        
        static const unsigned int unbounded; //!< Identifer for "unbounded" max values.
      
      protected:
        
        unsigned int m_minOccurs;  //!< It specifies the minimum number of times the any element can occur in the parent element. The value can be any number >= 0. Default value is 1. (Optional)
        unsigned int m_maxOccurs;  //!< It specifies the maximum number of times the any element can occur in the parent element. The value can be any number >= 0, or if you want to set no limit on the maximum number, use the value "unbounded". Default value is 1. (Optional)
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_OCCURS_H
