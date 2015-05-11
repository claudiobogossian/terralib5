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
  \file terralib/geometry/Envelope.h

  \brief An Envelope defines a 2D rectangular region.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_ENVELOPE_H
#define __TERRALIB_GEOMETRY_INTERNAL_ENVELOPE_H

// TerraLib
#include "Config.h"

// STL
#include <cassert>
#include <limits>
#include <vector>

namespace te
{
  namespace gm
  {
// Forward declarations
    struct Coord2D;

    /*!
      \class Envelope

      \brief An Envelope defines a 2D rectangular region.

      \ingroup geometry
     */
    class TEGEOMEXPORT Envelope
    {
      public:

        /** @name Basic Envelope Methods
         *  Basic Envelope methods.
         */
        //@{

        /*! \brief It constructs an envelope with invalid coordinates. */
        Envelope();

        /*!
          \brief Constructor.

          \param llx Lower left corner x-coordinate.
          \param lly Lower left corner y-coordinate.
          \param urx Upper right corner x-coordinate.
          \param ury Upper right corner y-coordinate.
        */
        Envelope(const double& llx, const double& lly,
                 const double& urx, const double& ury);

        /*!
          \brief Constructor.

          \param vectd Vector of doubles representing the envelope lower left and upper right corners.
        */
        Envelope(const std::vector<double>& vectd);

        /*!
          \brief Copy constructor.

          \param rhs The source object to copy from.
        */
        Envelope(const Envelope& rhs);

        /*!
          \brief It initializes (sets) the envelope bounds.

          \param llx Lower left corner x-coordinate.
          \param lly Lower left corner y-coordinate.
          \param urx Upper right corner x-coordinate.
          \param ury Upper right corner y-coordinate.
        */
        void init(const double& llx, const double& lly,
                  const double& urx, const double& ury);

        /*!
          \brief It returns a constant reference to the x coordinate of the lower left corner.

          \return A constant reference to the x coordinate of the lower left corner.
        */
        const double& getLowerLeftX() const;

        /*!
          \brief It returns a constant reference to the x coordinate of the lower left corner.

          \return A constant reference to the x coordinate of the lower left corner.
        */
        double& getLowerLeftX();

        /*!
          \brief It returns a constant refernce to the y coordinate of the lower left corner.

          \return A constant refernce to the y coordinate of the lower left corner.
        */
        const double& getLowerLeftY() const;

        /*!
          \brief It returns a constant refernce to the y coordinate of the lower left corner.

          \return A constant refernce to the y coordinate of the lower left corner.
        */
        double& getLowerLeftY();

        /*!
          \brief It returns a constant refernce to the x coordinate of the upper right corner.

          \return A constant refernce to the x coordinate of the upper right corner.
        */
        const double& getUpperRightX() const;

        /*!
          \brief It returns a constant refernce to the y coordinate of the upper right corner.

          \return A constant refernce to the y coordinate of the upper right corner.
        */
        double& getUpperRightX();

        /*!
          \brief It returns a constant refernce to the x coordinate of the upper right corner.

          \return A constant refernce to the x coordinate of the upper right corner.
        */
        const double& getUpperRightY() const;

        /*!
          \brief It returns a constant refernce to the y coordinate of the upper right corner.

          \return A constant refernce to the y coordinate of the upper right corner.
        */
        double& getUpperRightY();

        /*!
          \brief It returns the lower left coordinate of the envelope.

          \return The lower left coordinate of the envelope.
        */
        Coord2D getLowerLeft() const;
 
        /*!
          \brief It returns the upper right coordinate of the envelope.

          \return The upper right coordinate of the envelope.
        */
        Coord2D getUpperRight() const;

        /*!
          \brief It returns the rectangle's center coordinate.

          \return The rectangle's center coordinate.
        */
        Coord2D getCenter() const;

        /*! \brief It will invalidated the envelope. */
        void makeInvalid();

        /*!
          \brief It tells if the rectangle is valid or not.

          \return False if the rectangle coordinates are not valid and true otherwise.
        */
        bool isValid() const;

        /*!
          \brief It returns the envelope width.

          \return The envelope width.
        */
        double getWidth() const;

        /*!
          \brief It returns the envelope height.

          \return The envelope height.
        */
        double getHeight() const;

        /*!
          \brief It returns the area of this envelope as measured in the spatial reference system of it.

          \return The area of this envelope.
        */
        double getArea() const;

        //@}

        /** @name Envelope Operators
         *  Overloaded operators for an envelope.
         */
        //@{

        /*!
          \brief Assignment operator.

          \param rhs The source object to copy from.

          \return A reference to this instance.
        */
        Envelope& operator=(const Envelope& rhs);

        /*!
          \brief Equal operator.

          \param rhs The source object to be compared.

          \return True if the rectangles have the same exact coordinates.
        */
        bool operator==(const Envelope& rhs) const;

        //@}

        /** @name Spatial Operations
         *  Methods for testing spatial relations between Envelope objects and to perform some operations over envelope type.
         */
        //@{

        /*!
          \brief It returns true if the envelopes are "spatially equal".

          \param rhs The another envelope to be compared.

          \return True if the enevlopes are "spatially equal".
        */
        bool equals(const Envelope& rhs) const;

        /*!
          \brief It returns true if this envelope is "spatially disjoint" from rhs envelope.
 
          \param rhs The other envelope to be compared.

          \return True if this envelope is "spatially disjoint" from the other envelope.
        */
        bool disjoint(const Envelope& rhs) const;

        /*!
          \brief It returns true if the envelopes "spatially intersects".

          \param rhs The other envelope to be compared.

          \return True if the envelopes "spatially intersects".
        */
        bool intersects(const Envelope& rhs) const;

        /*!
          \brief It returns true if the envelopes "spatially touches".

          \param rhs The other envelope to be compared.

          \return True if the envelopes "spatially touches".
        */
        bool touches(const Envelope& rhs) const;

        /*!
          \brief It returns true if this envelope is "spatially within" the rhs envelope.

          \param rhs The other envelope to be compared.

          \return True if this envelope is "spatially within" the rhs envelope.
        */
        bool within(const Envelope& rhs) const;

        /*!
          \brief It returns true if this envelope "spatially contains" the rhs envelope.

          \param rhs The other envelope to be compared.

          \return True if this envelope "spatially contains" the rhs envelope.
        */
        bool contains(const Envelope& rhs) const;
     
        /*!
          \brief It returns the shortest distance between any two points in the two envelopes.

          \param rhs The other envelope.

          \return The shortest distance between any two points in the two envelopes.

          \note It calculates the shortest distance in the spatial reference system of the envelopes.

          \note If the two envelopes intersects it will return "0.0".
        */
        double distance(const Envelope& rhs) const;

        /*!
          \brief It returns an envelope that represents the point set intersection with another envelope.

          \param rhs The other envelope whose intersection with this envelope will be calculated.

          \return An envelope representing the intersection with this envelope. It can be a degenerated envelope if they only "spatially touch" each other".

          \pre The rhs envelope must intersects this envelope.

          \note The caller of this method will take the ownership of the returned envelope. 

          \warning Calling this method with two envelopes that doesn't intersect may return an invalid envelope.
        */
        Envelope intersection(const Envelope& rhs) const;

        /*!
          \brief It updates the envelop with coordinates of another envelope.

          \param rhs The other envelope whose coordinates will be used to update this one.
        */
        void Union(const Envelope& rhs);

        /*!
          \brief It will transform the coordinates of the Envelope from the old SRS to the new one.

          After calling this method the Envelope will be associated to the new SRID.

          \param oldsrid The old Spatial Reference System.
          \param newsrid The new Spatial Reference System used to transform the coordinates of the Envelope.

          \exception Exception It may throw an exception if it can not do the transformation.
        */
        void transform(int oldsrid, int newsrid);

        //@}

      public:

        double m_llx;  //!< Lower left corner x-coordinate.
        double m_lly;  //!< Lower left corner y-coordinate.
        double m_urx;  //!< Upper right corner x-coordinate.
        double m_ury;  //!< Upper right corner y-coordinate.
    };

    inline Envelope::Envelope()
    {
      makeInvalid();
    }

    inline Envelope::Envelope(const double& llx, const double& lly,
                              const double& urx, const double& ury)
    {
      init(llx, lly, urx, ury);
    }

    inline te::gm::Envelope::Envelope(const std::vector<double>& vectd)
    {
       double d[4] = { 0.0, 0.0, 0.0, 0.0 };

       int i = 0;

       for(std::vector<double>::const_iterator it = vectd.begin(); it < vectd.end(); ++it)
       {
           d[i] = (*it);
           i += 1;
       }

       init(d[0], d[1], d[2], d[3]);
    }

    inline Envelope::Envelope(const Envelope& rhs)
    {
      init(rhs.m_llx, rhs.m_lly, rhs.m_urx, rhs.m_ury);
    }

    inline void Envelope::init(const double& llx, const double& lly,
                               const double& urx, const double& ury)
    {
      m_llx = llx;
      m_lly = lly;
      m_urx = urx;
      m_ury = ury;
    }

    inline const double& Envelope::getLowerLeftX() const
    {
      return m_llx;
    }

    inline double& Envelope::getLowerLeftX()
    {
      return m_llx;
    }

    inline const double& Envelope::getLowerLeftY() const
    {
      return m_lly;
    }

    inline double& Envelope::getLowerLeftY()
    {
      return m_lly;
    }

    inline const double& Envelope::getUpperRightX() const
    {
      return m_urx;
    }

    inline double& Envelope::getUpperRightX()
    {
      return m_urx;
    }

    inline const double& Envelope::getUpperRightY() const
    {
      return m_ury;
    }

    inline double& Envelope::getUpperRightY()
    {
      return m_ury;
    }    

    inline void Envelope::makeInvalid()
    {
      init(std::numeric_limits<double>::max(),
           std::numeric_limits<double>::max(),
           -(std::numeric_limits<double>::max()),
           -(std::numeric_limits<double>::max()));
    }

    inline bool Envelope::isValid() const
    {
      return ((m_llx <= m_urx) && (m_lly <= m_ury)) ? true : false;
    }

    inline double Envelope::getWidth() const
    {
      return m_urx - m_llx;
    }

    inline double Envelope::getHeight() const
    {
      return m_ury - m_lly;
    }

    inline double Envelope::getArea() const
    {
      return (getWidth() * getHeight());
    }

    inline Envelope& Envelope::operator=(const Envelope& rhs)
    {
      if(this != &rhs)
        init(rhs.m_llx, rhs.m_lly, rhs.m_urx, rhs.m_ury);

      return *this;
    }

    inline bool Envelope::operator==(const Envelope& rhs) const
    {
      if((this->m_llx != rhs.m_llx) ||
         (this->m_lly != rhs.m_lly) ||
         (this->m_urx != rhs.m_urx) ||
         (this->m_ury != rhs.m_ury))
        return false;
      else 
        return true;
    }

    inline bool Envelope::equals(const Envelope& rhs) const
    {
      return *this == rhs;
    }

    inline bool Envelope::disjoint(const Envelope& rhs) const
    {
      if((m_urx < rhs.m_llx) ||
         (m_llx > rhs.m_urx) ||
         (m_ury < rhs.m_lly) ||
         (m_lly > rhs.m_ury))
        return true;
      else
        return false;
    }

    inline bool Envelope::intersects(const Envelope& rhs) const
    {
      if((m_urx < rhs.m_llx) ||
         (m_llx > rhs.m_urx) ||
         (m_ury < rhs.m_lly) ||
         (m_lly > rhs.m_ury))
        return false;
      else
        return true;
    }

    inline bool Envelope::touches(const Envelope& rhs) const
    {
      if((m_urx == rhs.m_llx) || (m_llx == rhs.m_urx))
      {
// below or above?
        if((m_ury < rhs.m_lly) || (m_lly > rhs.m_ury))
          return false;
        else
          return true;
      }

      if((m_ury == rhs.m_lly) || (m_lly == rhs.m_ury))
      {
// to the left or to the right?
        if((m_urx < rhs.m_llx) || (m_llx > rhs.m_urx))
          return false;
        else
          return true;
      }

      return false;
    }

    inline bool Envelope::within(const Envelope& rhs) const
    {
      return ((m_llx >= rhs.m_llx) &&
              (m_urx <= rhs.m_urx) &&
              (m_lly >= rhs.m_lly) &&
              (m_ury <= rhs.m_ury));
    }

    inline bool Envelope::contains(const Envelope& rhs) const
    {
      return ((rhs.m_llx >= m_llx) &&
              (rhs.m_urx <= m_urx) &&
              (rhs.m_lly >= m_lly) &&
              (rhs.m_ury <= m_ury));
    }

    inline Envelope Envelope::intersection(const Envelope& rhs) const
    {
      assert(intersects(rhs));

      double llx = m_llx > rhs.m_llx ? m_llx : rhs.m_llx;
      double lly = m_lly > rhs.m_lly ? m_lly : rhs.m_lly;
      double urx = m_urx < rhs.m_urx ? m_urx : rhs.m_urx;
      double ury = m_ury < rhs.m_ury ? m_ury : rhs.m_ury;

      return Envelope(llx, lly, urx, ury);
    }

    inline void Envelope::Union(const Envelope& rhs)
    {
      if(rhs.m_llx < m_llx)
        m_llx = rhs.m_llx;

      if(rhs.m_lly < m_lly)
        m_lly = rhs.m_lly;

      if(m_urx < rhs.m_urx)
        m_urx = rhs.m_urx;

      if(m_ury < rhs.m_ury)
        m_ury = rhs.m_ury;
    }

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_ENVELOPE_H

