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
  \file terralib/dataaccess/query/LiteralEnvelope.h

  \brief A class that models a literal for Envelope values.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALENVELOPE_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALENVELOPE_H

// TerraLib
#include "Literal.h"

// STL
#include <memory>

namespace te
{
// Forward declaratons
  namespace gm { class Envelope; }

  namespace da
  {
    /*!
      \class LiteralEnvelope
      
      \brief A class that models a literal for Envelope values.

      \sa Expression

      \note This class doesn't derive from Literal! In future we will do something to adjust this point!
    */
    class TEDATAACCESSEXPORT LiteralEnvelope : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param e    The envelope value.
          \param srid The envelope SRS.

          \note The LiteralEnvelope will take the ownership of the given envelope.
        */
        LiteralEnvelope(te::gm::Envelope* e, int srid);

        /*!
          \brief Constructor.

          \param e    The envelope value.
          \param srid The envelope SRS.
        */
        LiteralEnvelope(const te::gm::Envelope& e, int srid);

        /*! \brief Copy constructor. */
        LiteralEnvelope(const LiteralEnvelope& rhs);

        /*! \brief Destructor. */
        ~LiteralEnvelope();

        /*! Assignment operator.  */
        LiteralEnvelope& operator=(const LiteralEnvelope& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;

        /*!
          \brief It returns the associated envelope value.

          \return The associated envelope value.
        */
        te::gm::Envelope* getValue() const;

        /*!
          \brief It sets the envelope value associated to the Literal.

          \param e The envelope value.

          \note The LiteralEnvelope will take the ownership of the given envelope.
        */
        void setValue(te::gm::Envelope* e);

        /*!
          \brief It returns the envelope SRS id.

          \return The envelope SRS id.
        */
        int getSRID() const { return m_srid; }

        /*!
          \brief It sets the envelope SRS id.

          \param srid The envelope SRS id.
        */
        void setSRID(int srid) { m_srid = srid; }

      private:

        std::auto_ptr<te::gm::Envelope> m_eval;   //!< The envelope value.
        int m_srid;                               //!< The envelope SRS ID.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALENVELOPE_H

