/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file AbstractMapDisplay.h

  \brief It defines the concept of a map display responsible for controlling how a set of layers are displayed.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H

// TerraLib
#include "Config.h"
#include "Canvas.h"

namespace te
{
// Forward declarations
  namespace gm { class Envelope; }

  namespace map
  {
// Forward declarations
    class AbstractLayer;

    /*!
      \class AbstractMapDisplay

      \brief It defines the concept of a map display responsible for controlling how a set of layers are displayed.

      \sa AbstractLayer, MapDisplay
     */
    class TEMAPEXPORT AbstractMapDisplay
    {
      public:

         /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new MapDisplay. */
        AbstractMapDisplay(){};

         /*! \brief Virtual destructor. */
        virtual ~AbstractMapDisplay(){};

        /** @name Map Display Virtual Methods
         *  Methods to configure the MapDisplay.
         */
        //@{

         /*!
          \brief It sets the root layer.

          \param layer The root layer.

          \note The display will NOT take the ownership of the given layer.
                You must choose between using setLayerTree or setLayerList. Never use both.
          */
        virtual void setLayerTree(te::map::AbstractLayer* layer) = 0;

        /*!
          \brief It sets the layer list to be showed in the Map Display.

          \param order The layer list.

          \note The display will NOT take the ownership of the given layer.
                You must choose between using setLayerTree or setLayerList. Never use both.
          */
        virtual void setLayerList(std::list<te::map::AbstractLayer*>& order) = 0;

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
        virtual const te::gm::Envelope* getExtent() const = 0;

        /*!
          \brief It sets the world visible area. If the given area is not proportional to the device width and height,
                 the MapDisplay will change it in order to preserve the aspect ratio.
                 Just successive calls to the draw method will be affected.

          \param e The world visible area. It coordinates must be in the Map Display SRS.

          \todo Pensar se ter um metodo chamado getBestFit seria mais adequado para que o setExtent fosse mais burrinho, isto e, so fizesse o que manda e nao pensasse qual a melhro proporcao.
         */
        virtual void setExtent(const te::gm::Envelope& e) = 0;

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

          \param srid The new Spatial Reference System to be used by the Map Display.
         */
        virtual void setSRID(const int& srid) = 0;
 
        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The map display that would be copied.
         */
        AbstractMapDisplay(const AbstractMapDisplay& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The map display whose contents would be assigned to this map display .

          \return A reference to this map display.
         */
        AbstractMapDisplay& operator=(const AbstractMapDisplay& rhs);

        //@}
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTMAPDISPLAY_H

