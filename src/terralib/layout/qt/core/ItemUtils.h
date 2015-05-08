/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ItemUtils.h
   
  \brief Utility class for manipulating items in the scene and vectorization of text and legend.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H

// TerraLib
#include "../../core/Config.h"
#include "../../../geometry/Point.h"
#include "../../core/property/Properties.h"

// STL
#include <vector>
#include <string>
#include <map>

// Qt
#include <QList>

class QGraphicsItem;
class QGraphicsScene;
class QFont;

namespace te
{
  namespace layout
  {
    class MapItem;
    class EnumType;
    class MapModel;
    class GridMapItem;

    /*!
      \brief Utility class for manipulating items in the scene and vectorization of text and legend.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT ItemUtils
    {
      public:

        /*!
          \brief Constructor
        */ 
        ItemUtils( QGraphicsScene* scene );

        /*!
          \brief Destructor
        */
        virtual ~ItemUtils();

        /*!
          \brief MVC components list of type te::layout::MapItem
		  
		      \param selected true if only selected te::layout::MapItem, false otherwise.
          \return MVC components list of type te::layout::MapItem
        */
        virtual std::vector<MapItem*> getMapItemList(bool selected = false);

        /*!
          \brief Search for te::layout::MapItem by name
		  
		      \param name of te::layout::MapItem
          \return MapItem found
        */
        virtual MapItem* getMapItem(std::string name);

        /*!
          \brief List of names te::layout::MapItem
		  
		      \param selected true if only selected te::layout::MapItem, false otherwise.
          \return list of names
        */
        virtual std::vector<std::string> mapNameList(bool selected = false);

        /*!
          \brief Returns the number of items added to the scene with the type
		  
		      \param type type to search
          \return number of items
        */
        virtual int countType(te::layout::EnumType* type);

        /*!
          \brief Search max id for a type.
		  
		      \param type type to search
          \return max id
        */
        virtual int maxTypeId(te::layout::EnumType* type);

        /*!
          \brief Indicates whether there is a tool active for object te::layout::MapItem.
		  
		      \return true if tool active, false otherwise 
        */
        virtual bool isCurrentMapTools();

        /*!
          \brief Changes the active tool of te::layout::MapItem.
		  
		      \param type of tool
        */
        virtual void setCurrentToolInSelectedMapItems(EnumType* mode);

        /*!
          \brief Vectorizes grid text of selected MapItem. (generates te::layout::TextItem objects)
        */
        virtual void createTextGridAsObject();

        /*!
          \brief Vectorizes inside text of selected MapItem. (generates te::layout::TextItem objects)
        */
        virtual void createTextMapAsObject();

        /*!
          \brief Vectorizes selected LegendItem. (generates te::layout::TextItem objects)
        */
        virtual void createLegendChildAsObject();

        /*!
          \brief Get properties of all GridMapItem, children of a MapItem.
		  
		      \param 
          \return properties of all GridMapItem, children of a MapItem.
        */
        virtual std::vector<Properties*> getGridMapProperties();

        /*!
          \brief Get all GridMapItem, children of a MapItem.
		  
		      \param 
          \return all GridMapItem, children of a MapItem.
        */
        virtual std::vector<GridMapItem*> getMapChildren();

        /*!
          \brief Checks whether the coordinated intersects an item and returns.

          \param x axis x coordinate
		      \param y axis y coordinate

          \return 
        */
        virtual QGraphicsItem* intersectionSelectionItem(int x, int y);
          
      protected:

        /*!
          \brief Returns the list of items added to a scene.

          \param selected true if only selected items, false otherwise.
          \return list of items
        */
        virtual QList<QGraphicsItem*> getItems(bool selected = false);

        /*!
          \brief Vectorizes grid text of selected MapItem. (generates te::layout::TextItem objects)
		  
		      \param map coordinate and text
          \param ft font
        */
        virtual void createTextItemFromObject(std::map<te::gm::Point*, std::string> map, QFont* ft = 0);

        /*!
          \brief Vectorizes grid text of selected MapItem. (generates te::layout::TextItem objects)
		  
          \param map coordinate and text
		      \param visitable MapItem model associated
        */
        virtual void createLegendChildItemFromLegend(std::map<te::gm::Point*, std::string> map, MapModel* visitable);

      protected:

        QGraphicsScene* m_scene; //!< A pointer to scene
    };
  }
}

#endif 
