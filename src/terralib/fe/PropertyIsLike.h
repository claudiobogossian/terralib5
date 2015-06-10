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
  \file PropertyIsLike.h
  
  \brief It is intended to encode a character string comparison operator with pattern matching.
 */

#ifndef __TERRALIB_FE_INTERNAL_PROPERTYISLIKE_H
#define __TERRALIB_FE_INTERNAL_PROPERTYISLIKE_H

// TerraLib
#include "ComparisonOp.h"

// STL
#include <string>

namespace te
{
  namespace fe
  {
// Forward declarations
    class Literal;
    class PropertyName;

    /*!
      \class PropertyIsLike
      
      \brief It is intended to encode a character string comparison operator with pattern matching.

      The pattern is defined by a combination of regular
      characters, the wildCard character, the singleChar
      character, and the escapeChar character. The wildCard
      character matches zero or more characters. The singleChar
      character matches exactly one character. 

      \ingroup fe

      \sa ComparisonOp, PropertyName, Literal
     */
    class TEFEEXPORT PropertyIsLike : public ComparisonOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new PropertyIsLike. */
        PropertyIsLike();

        /*! \brief Destructor. */
        ~PropertyIsLike();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It sets the property name.

          \param p The property name.

          \note It will take the ownership of the property name.
         */
        void setPropertyName(PropertyName* p);

        /*!
          \brief It returns the property name.

          \return The property name.
         */
        PropertyName* getPropertyName() const;

        /*!
          \brief It sets the literal value of the operator.

          \param l The literal value of the operator.

          \note It will take the ownership of the literal value.
         */
        void setLiteral(Literal* l);

        /*!
          \brief It returns the literal value.

          \return The literal value.
         */
        Literal* getLiteral() const;

        /*!
          \brief It sets the wild character.

          \param w The wild character.
         */
        void setWildCard(const std::string& w);

        /*!
          \brief It returns the wild character.

          \return The wild character.
         */
        const std::string& getWildCard() const;

        /*!
          \brief It sets the wild single character.

          \param s The wild single character.
         */
        void setSingleChar(const std::string& s);

        /*!
          \brief It returns the single wild character.

          \return The single wild character.
         */
        const std::string& getSingleChar() const;

        /*!
          \brief It sets the escape character.

          \param e The escape character.
         */
        void setEscapeChar(const std::string& e);

        /*!
          \brief It returns the escape character.

          \return The escape character.
         */
        const std::string& getEscapeChar() const;

        //@}

      private:

        PropertyName* m_property;  //!< Mandatory.
        Literal* m_literalValue;   //!< Mandatory.
        std::string m_wildCard;    //!< The wildCard_ character matches zero or more characters. (Mandatory)
        std::string m_singleChar;  //!< The singleChar_ character matches exactly one character. (Mandatory)
        std::string m_escapeChar;  //!< The escapeChar_ character is used to escape the meaning of the wildCard_, singleChar_ and escapeChar_ itself. (Mandatory.)
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_PROPERTYISLIKE_H
