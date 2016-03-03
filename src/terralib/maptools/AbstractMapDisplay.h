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
  \file terralib/maptools/AbstractMapDisplay.h

  \brief It defines the concept of a map display responsible for controlling how a set of layers are displayed.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H

// TerraLib
#include "AbstractLayer.h"
#include "Enums.h"

// STL
#include <list>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace map
  {
    /*!
      \class AbstractMapDisplay

      \brief It defines the concept of a map display responsible for controlling how a set of layers are displayed.

      \ingroup map

      \sa AbstractLayer, MapDisplay
    */
    class TEMAPEXPORT AbstractMapDisplay : public boost::noncopyable
    {
      public:

        /*! \brief It initializes a new MapDisplay. */
        AbstractMapDisplay():m_cancel(false) { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractMapDisplay() { }

        /** @name Map Display Virtual Methods
         *  Methods to configure the MapDisplay.
         */
        //@{

        /*!
          \brief It sets the layer list to be showed in the Map Display.

          \param layers The layer list.
        */
        virtual void setLayerList(const std::list<te::map::AbstractLayerPtr>& layers) = 0;

        /*!
          \brief It returns the MapDisplay current horizontal align.

          \return The MapDisplay current horizontal align.
        */
        virtual te::map::AlignType getHAlign() const = 0;

        /*!
          \brief It returns the MapDisplay current vertical align.

          \return The MapDisplay current vertical align.
        */
        virtual te::map::AlignType getVAlign() const = 0;

        /*!
          \brief It will set the align rendering of objects into the map display. Just successive drawings will be affected by this modification.

          \param h The new horizontal align.
          \param v The new vertical align.

          \note It will not automatically redraw the objects, you must explicit call the setExtent method.
        */
        virtual void setAlign(te::map::AlignType h, te::map::AlignType v) = 0;

        /*!
          \brief It returns the world extent showned by the MapDisplay.

          \return The world extent showned by the MapDisplay or NULL if none is set.

          \note The extent coordinates are in the Map Display SRS.
        */
        virtual const te::gm::Envelope& getExtent() const = 0;

        /*!
          \brief It sets the world visible area and refreshes the contents in the map display.

          If the given area is not proportional to the device width and height,
          the MapDisplay will change it in order to preserve the aspect ratio.

          \param e         The world visible area.
          \param doRefresh If true the display will refresh its contents.

          \pre The world coordinates must be in the map display SRS.
        */
        virtual void setExtent(te::gm::Envelope& e, bool doRefresh = true) = 0;

        /*!
          \brief It return the Spatial Reference System used by the Map Display.

          \return The Spatial Reference System used by the Map Display.
        */
        virtual int getSRID() const = 0;

        /*!
          \brief It sets a new Spatial Reference System to be used by the Map Display.

          It will also convert the current envelope coordinates to the new SRS. This may
          cause changes to the world visible area. In this case, the extent will be updated and
          new internal transformation function will be calculated.

          \param srid      The new Spatial Reference System to be used by the Map Display.
          \param doRefresh If true the display will refresh its contents.
        */
        virtual void setSRID(const int& srid, bool doRefresh = true) = 0;

        /*! \brief It updates the contents in the map display. */
        virtual void refresh(bool redraw = false) = 0;

        /*!
          \brief It returns the MapDisplay current width in pixels.

          \return The MapDisplay current width in pixels.
        */
        virtual unsigned int getWidth() const = 0;

        /*!
          \brief It returns the MapDisplay current height in pixels.

          \return The MapDisplay current height in pixels.
        */
        virtual unsigned int getHeight() const = 0;

        /*!
          \brief It returns the MapDisplay current width in millimeters.

          \return The MapDisplay current width in millimeters.
        */
        virtual double getWidthMM() const = 0;

        /*!
          \brief It returns the MapDisplay current height in millimeters.

          \return The MapDisplay current height in millimeters.
        */
        virtual double getHeightMM() const = 0;

        protected:

          bool m_cancel;


        //@}
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H
