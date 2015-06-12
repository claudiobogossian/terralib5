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
  \file PtMarker.h
  
  \brief This file contains several utility routines for dealing with point markers.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_PTMARKER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_PTMARKER_H

// TerraLib
#include "Config.h"
#include "Enums.h"

namespace te
{
// Forward declaration
  namespace color { class RGBAColor; }

  namespace map
  {
    /*!
      \brief It creates a point marker.
 
      \param type  The marker type.
      \param width The marker width.
      \param color The marker color.

      \return The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
     */
    TEMAPEXPORT te::color::RGBAColor** CreateMarker(PtMarkerType type, int width, const te::color::RGBAColor& color);

    /*!
      \brief It creates a four rays marker.
 
      \param width The marker width.
      \param color The marker color.

      \return The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerFourRays(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a diamond marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerDiamond(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a square marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerSquare(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a circle marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerCircle(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a colorssed circle marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerCrossedCircle(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi ellipse left marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerSemiEllipseLeft(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi ellipse right marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerSemiEllipseRight(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi ellipse left marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerSemiEllipseUp(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi ellipse down marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerSemiEllipseDown(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi triangle left marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerTriangleLeft(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi triangle right marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerTriangleRight(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi triangle up marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerTriangleUp(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a semi triangle down marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerTriangleDown(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a cross marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerCross(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a x marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerX(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a dash marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerDash(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a dot marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerDot(int width, const te::color::RGBAColor& color);

    /*!
      \brief It create a pixel marker.

      \param width The marker width.
      \param color The marker color.

      \return The The marker: a square matrix of int's where each position corresponds to a RGBA color.

      \note The caller of this method will take the ownership of the returned pointer.
      */
    TEMAPEXPORT te::color::RGBAColor** CreateMarkerPixel(int width, const te::color::RGBAColor& color);

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_PTMARKER_H