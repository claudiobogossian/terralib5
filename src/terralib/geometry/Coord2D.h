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
  \file terralib/geometry/Coord2D.h

  \brief An utility struct for representing 2D coordinates.
*/

#ifndef __TERRALIB_GEOMETRY_INTERNAL_COORD2D_H
#define __TERRALIB_GEOMETRY_INTERNAL_COORD2D_H

namespace te
{
  namespace gm
  {
    /*!
      \struct Coord2D

      \brief An utility struct for representing 2D coordinates.

      \ingroup geometry
    */
    struct Coord2D
    {
        /*! \brief Constructor. */
        Coord2D() { }

        /*!
          \brief Constructor.

          \param xx The x-coordinate.
          \param yy The y-coordinate.
        */
        Coord2D(const double& xx, const double& yy)
          : x(xx),
            y(yy)
        {
        }
        
        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side instance.
        */
        Coord2D(const Coord2D& rhs)
          : x(rhs.x),
            y(rhs.y)
        {
        }        

        /*!
          \brief It compares if two coordinates have the same exact values.

          \param rhs The right-hand-side coordinate.

          \return True if they have exact the same values.
        */
        bool operator==(const Coord2D& rhs) const
        {
          if(x != rhs.x)
            return false;

          if(y != rhs.y)
            return false;

          return true;
        }
      
        /*! \brief Lexicographic compare (x-y). */
        bool operator<(const Coord2D& rhs) const
        {
          if(x < rhs.x)
            return true;
          
          if(x > rhs.x)
            return false;
          
          if(y < rhs.y)
            return true;
          
          return false;
        }

        /*! \brief It returns the x-coordinate. */
        double getX() const
        {
          return x;
        }

        /*! \brief It returns the y-coordinate. */
        double getY() const
        {
          return y;
        }

        double x;  //!< x-coordinate.
        double y;  //!< y-coordinate.
    };

  } // end namespace gm
}   // end namespace te

#endif  // __TERRALIB_GEOMETRY_INTERNAL_COORD2D_H

