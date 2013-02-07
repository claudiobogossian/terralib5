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
  \file terralib/maptools/AbstractLayer.h

  \brief This is the base class for Layers.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H

// TerraLib
#include "../common/TreeItem.h"
#include "../geometry/Envelope.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <string>

namespace te
{
// Forward declaration
  namespace gm { class Envelope; }

  namespace map
  {
// Forward declaration
    class Canvas;

    /*!
      \class AbstractLayer

      \brief This is the base class for Layers.

      \sa TreeItem, DataSetLayer, QueryLayer, RasterLayer, FolderLayer
    */
    class TEMAPEXPORT AbstractLayer : public te::common::TreeItem
    {
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractLayer(AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractLayer(const std::string& id, AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        AbstractLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Virtual destructor. */
        virtual ~AbstractLayer();

        /*!
          \brief It returns the layer id.

          \return The layer id.
        */
        const std::string& getId() const;

        /*!
          \brief It sets the layer id.

          \param id The layer id.
        */
        void setId(const std::string& id);

        /*!
          \brief It returns the layer title.

          \return The layer title.
        */
        const std::string& getTitle() const;

        /*!
          \brief It sets the layer title.

          \param title The layer title.
        */
        void setTitle(const std::string& title);

        /*!
          \brief It returns the layer visibility.

          This information can be used for instance to know if the layer must be drawn.

          \return The layer visibility.
        */
        Visibility getVisibility() const;

        /*!
          \brief It sets the visibility status of the layer.

          This information can be used for instance to know if the layer must be drawn.

          \param v The layer visibility.
        */
        void setVisibility(Visibility v);

        /*!
          \brief It returns the icon used when showing the layer.

          \return The icon used when showing the layer.
        */
        const std::string& getIcon() const;

        /*!
          \brief It sets an optional icon to be used when showing the layer.

          \param icon The location of an icon file (PNG, JPEG, GIF or ICO).
        */
        void setIcon(const std::string& icon);

        /*!
          \brief It returns the layer type.

          \return The layer type.
        */
        virtual const std::string& getType() const = 0;

        /*!
          \brief It returns true if the layer can be used for instance to draw, otherwise, it returns false.

          This method can be used to check if the data referenced by the layer is available (accessible), or not.

          \return True, if the layer is valid, otherwise, it returns false.
        */
        virtual bool isValid() const = 0;

        /*!
          \brief It returns the Layer extent (or minimum bounding box).

          \return The Layer extent (or minimum bounding box) with coordinates in the same SRS as the layer.
        */
        const te::gm::Envelope& getExtent() const;

        /*!
          \brief It sets the Layer extent (or minimum bounding box).

          \param mbr The Layer extent (or minimum bounding box).

          \pre The mbr coordinates must be in the same SRS as the layer.
        */
        void setExtent(const te::gm::Envelope& mbr);

        /*!
          \brief It returns the Spatial Reference System ID associated to the Layer.

          \return The Spatial Reference System ID associated to the Layer.
        */
        int getSRID() const;

        /*!
          \brief It sets the Spatial Reference System ID associated to the Layer.

          \param srid The Spatial Reference System ID to be associated to the Layer.
        */
        void setSRID(int srid);

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box can be used to constraint the layer objects to be drawn.

          The bbox coordinates must be in the same Spatial Reference System given by srid.

          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
        */
        virtual void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid) = 0;

      protected:

        /*!
          \brief It sets the visibility of the descendants layers of this layer.

          \param v The visibility of the descendants layers.
        */
        virtual void setDescendantsVisibility(Visibility v);

        /*! \brief It adjusts the visibility of the ascendants layers of this layer. */
        virtual void adjustAscendantsVisibility();

      private:

        std::string m_id;             //!< Layer id.
        std::string m_title;          //!< A brief description of this Layer that can be used by applications to show a text identifying this Layer.
        std::string m_icon;           //!< The icon used to show the layer when needed.
        te::gm::Envelope m_mbr;       //!< The layer bounding box.
        int m_srid;                   //!< The identifier of the layer spatial reference system.
        Visibility m_visibility;      //!< A flag that indicates the visibility status of the layer.
    };

    typedef boost::intrusive_ptr<AbstractLayer> AbstractLayerPtr;

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H

