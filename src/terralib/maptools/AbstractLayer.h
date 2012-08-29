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
  \file AbstractLayer.h

  \brief This is the base class for Layers.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H

// TerraLib
#include "../common/TreeItem.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <memory>
#include <string>

namespace te
{
// Forward declarations
  namespace gm { class Envelope; }
  namespace se { class Style; }
  namespace xl { class SimpleLink; }

  namespace map
  {
// Forward declarations
    class Canvas;
    class Grouping;
    class LegendItem;

    /*!
      \class AbstractLayer

      \brief This is the base class for Layers.
    
      What is a Layer?<br>
      <ul>
      <li>It is a reference to a set of geographic data, but it doesn't contain any geographic data.</li>
      <li>It can be a reference to a set of features with geometric attributes or an image;</li>
      <li>It can have an associated style;</li>
      <li>A layer can be nested (it is a TreeItem), so it can have child layers;</li>
      <li>It may have a filter expression associated to it.</li>
      </ul>

      The main goal of this class is to support applications that handle information as layers.

      As a side effect, you can create distinct layers on the same geographic data that
      visualize different attributes or employ different drawing methods.
    
      \note A layer can be nested (see TreeItem for more details).

      \sa TreeItem, QueryLayer, FolderLayer, Layer
    */
    class TEMAPEXPORT AbstractLayer : public te::common::TreeItem<AbstractLayer>
    {
      public:

        /*!
          \brief It initializes a new Layer.          

          \param id     The layer id.
          \param title  The title is a brief description about the layer that can be used by an application to show a meaningful label to the user.
          \param parent The parent Layer (NULL if it has no parent).

          \note If the parent is informed, it will take the ownership of the new layer.
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

          \return A pointer to the icon used when showing the layer.
        */
        te::xl::SimpleLink* getIcon() const; 

        /*!
          \brief It sets an optional icon to be used when showing the layer.
          
          \param icon The location of an icon file (PNG, JPEG, GIF or ICO).

          \note The layer will take the icon ownership.
        */
        void setIcon(te::xl::SimpleLink* icon);        

        /*!
          \brief It returns the layer type.

          \return The layer type.
        */
        virtual const std::string& getType() const = 0;

        /*!
          \brief It returns true if the layer can be drawn, otherwise, it returns false.

          This method can be used to check if the data referenced by the layer is available (accessible), or not.

          \return True, if the layer can be drawn, otherwise, it returns false.
        */
        virtual bool isValid() const = 0;

        /*!
          \brief It returns true if the layer can be queried, otherwise, it returns false.

          Not all layers can be queried (ex: some WMS layers).

          \return True, if the layer can be queried, otherwise, it returns false.
        */
        virtual bool isQueryable() const = 0;

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box can be used to constraint the layer objects to be drawn.

          The bbox coordinates must be in the same Spatial Reference System given by srid.

          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
        */
        virtual void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid) = 0;

        /*!
          \brief It returns the style applied to the layer.

          \return The style applied to the layer.
        */
        virtual te::se::Style* getStyle() const = 0;

        /*!
          \brief It sets the style to be applied to the layer.

          \param style The layer style.

          \note The layer will take the ownership of the given style.
        */
        virtual void setStyle(te::se::Style* style) = 0;

        /*!
          \brief It returns the Layer extent (or minimum bounding box). Its coordinates are in the same SRS as the layer.

          \return The Layer extent (or minimum bounding box).
        */
        virtual const te::gm::Envelope* getExtent() const = 0;        

        /*!
          \brief It sets the Layer extent (or minimum bounding box). Its coordinates must be in the same SRS as the layer.

          \param mbr The Layer extent (or minimum bounding box).

          \note The layer will take the ownership of the given MBR.
        */
        virtual void setExtent(te::gm::Envelope* mbr) = 0;

        /*!
          \brief It returns the Spatial Reference System ID associated to the Layer.

          \return The Spatial Reference System ID associated to the Layer.
        */
        virtual int getSRID() const = 0;

        /*!
          \brief It sets the Spatial Reference System ID associated to the Layer.

          \param srid The Spatial Reference System ID to be associated to the Layer.
        */
        virtual void setSRID(int srid) = 0;

       /*!
          \brief It gets the grouping parameters used to generate the layer legend.

          \output It returns the grouping parameters of the legend layerr.
        */
        virtual te::map::Grouping* getGrouping() const;

        /*!
          \brief It sets the grouping parameters to be used to generate the layer legend.

          \param grouping The grouping to be used to generate the layer legend.

          \note This class will take the ownership of the pointer to the grouping argument.
        */
        virtual void setGrouping(Grouping* grouping);
        
        /*!
          \brief It checks if this layer has an associated legend.

          \output It returns true if the layer has an legend associated to this layer.
        */
        virtual bool hasLegend();

        /*!
          \brief It gets the legend associated to this layer.

          \output The legend associated to this layer.
        */
        virtual std::vector<te::map::LegendItem*>* getLegend();

        /*!
          \brief It removes the legend associated to this layer.
        */
        virtual void removeLegend();

        /*!
          \brief It inserts the given legend to this layer.

          \param legend The legend to be associated to this layer.
        */
        virtual void insertLegend(const std::vector<LegendItem*>& legend);

      protected:

        /*!
          \brief It sets the visibility of the descendants layers of this layer.
          
          \param v The visibility of the descendants layers.
        */
        void setDescendantsVisibility(Visibility v);

        /*! \brief It sets the visibility of the ascendants layers of this layer. */
        void setAscendantsVisibility();

      private:

        std::string m_id;                         //!< Layer id.
        std::string m_title;                      //!< A brief description of this Layer that can be used by applications to show a text identifying this Layer.        
        Visibility m_visibility;                  //!< A flag that indicates the visibility status of the layer.
        std::auto_ptr<te::xl::SimpleLink> m_icon; //!< The icon used to show the layer when needed.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYER_H

