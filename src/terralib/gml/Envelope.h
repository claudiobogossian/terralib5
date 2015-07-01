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
  \file terralib/gml/Envelope.h

  \brief Envelope defines an extent using a pair of positions defining opposite corners in arbitrary dimensions.
 */

#ifndef __TERRALIB_GML_INTERNAL_ENVELOPE_H
#define __TERRALIB_GML_INTERNAL_ENVELOPE_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace gm { class Envelope; }

  namespace gml
  {
    /*!
      \class Envelope

      \brief Envelope defines an extent using a pair of positions defining opposite corners in arbitrary dimensions.

      The first direct position is the "lower corner" (a coordinate position
      consisting of all the minimal ordinates for each dimension for all points
      within the envelope), the second one the "upper corner" (a coordinate position
      consisting of all the maximal ordinates for each dimension for all points
      within the envelope).
      <br>
      The use of the properties "coordinates" and "pos" has been deprecated.
      The explicitly named properties "lowerCorner" and "upperCorner" shall be used instead.
     */
    class TEGMLEXPORT Envelope
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new empty Envelope. */
        Envelope();

        /*!
          \brief It initializes the GML envelope with the given coordinates and srs.

          \param e    The envelope.
          \param srid The envelope spatial reference system.
         */
        Envelope(te::gm::Envelope* e, int srid);

        /*! \brief Destructor. */
        ~Envelope();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        te::gm::Envelope* getCoordinates() const;

        void setCoordinates(const double& llx, const double& lly, const double& urx, const double& ury);

        int getSRID() const;

        void setSRID(int srid);

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
         */
        Envelope(const Envelope& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this object.
         */
        Envelope& operator=(const Envelope& rhs);

        //@}

      private:

        te::gm::Envelope* m_envelope; //!< The envelope coordinates.
        int m_srid;                   //!< The SRS associated to the envelope coordinates.
    };

  } // end namespace gml
}   // end namespace te

#endif  // __TERRALIB_GML_INTERNAL_ENVELOPE_H
