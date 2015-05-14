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
  \file terralib/dataaccess/query/Like.h

  \brief It is intended to encode a character string comparison operator with pattern matching.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LIKE_H
#define __TERRALIB_DATAACCESS_INTERNAL_LIKE_H

// TerraLib
#include "Function.h"

namespace te
{
  namespace da
  {
    /*!
      \class Like
      
      \brief It is intended to encode a character string comparison operator with pattern matching.

      \sa Function
    */
    class TEDATAACCESSEXPORT Like : public Function
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param str The operator will take the expression ownership.
        */
        Like(Expression* str,
             const std::string& pattern,
             const std::string& wildCard = "%",
             const std::string& singleChar = "_",
             const std::string& escapeChar = "\\");

        /*! \brief Constructor. */
        Like(const Expression& str,
             const std::string& pattern,
             const std::string& wildCard = "%",
             const std::string& singleChar = "_",
             const std::string& escapeChar = "\\");

        /*! \brief Destructor. */
        ~Like();

        /*! \brief Copy constructor. */
        Like(const Like& rhs);

        /*! Assignment operator.  */
        Like& operator=(const Like& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the string expression to be compared with the like operator.

          \return The string expression to be compared with the like operator.
        */
        Expression* getString() const;

        /*!
          \brief It sets the string expression to be compared with the like operator.

          \param str The string expression to be compared with the like operator.
        */
        void setString(Expression* str);

        /*!
          \brief It returns the pattern used in the comparison.

          \return The pattern used in the comparison.
        */
        const std::string& getPattern();

        /*!
          \brief It sets the pattern to be used in the comparison.

          \param p The pattern to be used in the comparison.
        */
        void setPattern(const std::string& p);

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

      private:

        std::string m_pattern;      //!< The literal string pattern.
        std::string m_wildCard;     //!< The wild card character matches zero or more characters.
        std::string m_singleChar;   //!< The single char character matches exactly one character.
        std::string m_escapeChar;   //!< The escape char character is used to escape the meaning of the wild card, single char and escape char itself.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LIKE_H

