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
  \file FolderLayer.h

  \brief A FolderLayer can be used as a container for other kind of layers.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_FOLDERLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_FOLDERLAYER_H

// TerraLib
#include "AbstractLayer.h"

namespace te
{
  namespace map
  {
    /*!
      \class FolderLayer

      \brief A FolderLayer can be used as a container for other kind of layers.
    
      \sa TreeItem, AbstractLayer, QueryLayer, Layer
     */
    class TEMAPEXPORT FolderLayer : public AbstractLayer
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new Layer.          

          \param id     The layer id.
          \param title  The title is a brief description about the layer, that can be used by an application to show a meaninfull label to the user.
          \param parent The parent Layer (NULL if it has no parent).

          \note If parent is informed, it will take the ownership of the new layer.
         */
        FolderLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~FolderLayer();

        //@}
        

        /** @name Implementation of Pure Virtual Methods from AbstractLayer
         *  Every subclass must implent this interface.
         */
        //@{        

        /*!
          \brief It returns the layer type: "FOLDERLAYER".

          \return The layer type: "FOLDERLAYER".
         */
        const std::string& getType() const;

        /*!
          \brief It returns true if the layer can be drawn, otherwise, returns false.

          This method can be used to check if the data referenced by the layer is available (or acessible) or not.

          \return True if the layer can be drawn, otherwise, returns false.
         */
        bool isValid() const;

        /*!
          \brief It returns true if the layer can be queried, otherwise, returns false.

          Not all layers in a WMS server can bet be queried.

          \return True if the layer can be queried, otherwise, returns false.
         */
        bool isQueryable() const;

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box (bbox) is used to constraint the Layer objects to be drawn.
          The bbox coordinates must be in the Spatial Reference System given by srid.

          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
         */
        void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the style applied to the layer.

          \return The style applied to the layer.
         */
        te::se::Style* getStyle() const;

        /*!
          \brief It sets the style to be applied to the layer.

          \param style The layer style.

          \note The layer will take the ownership of the given style.
         */
        void setStyle(te::se::Style* style);

        /*!
          \brief It returns the Layer extent (or minimum bounding box). Its coordinates are in the same SRS as the layer.

          \return The Layer extent (or minimum bounding box).
         */
        const te::gm::Envelope* getExtent() const;        

        /*!
          \brief It sets the Layer extent (or minimum bounding box). Its coordinates must be in the same SRS as the layer.

          \param mbr The Layer extent (or minimum bounding box).
         */
        void setExtent(te::gm::Envelope* mbr);

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

        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        FolderLayer(const FolderLayer& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        FolderLayer& operator=(const FolderLayer& rhs);

        //@}

      public:

        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.

      private:

        int m_srid;               //!< The layer SRS.
        te::gm::Envelope* m_mbr;  //!< The Layer bounding box.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_FOLDERLAYER_H

