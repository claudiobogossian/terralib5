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
  \file BuildGraphicsItem.h
   
  \brief Class responsible for creating or building graphics objects. All objects are children of QGraphicsItem and ItemObserver.
    An object of a type is created from a coordinated. Also an object of a type can be built from the properties saved in a template.
    Only place where the model and the controller is instantiated and the MVC component is created.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_GRAPHICS_ITEM_H

//TerraLib
#include "../../core/AbstractBuildGraphicsItem.h"
#include "../../core/property/Properties.h"
#include "../../core/Config.h"
#include "pattern/factory/item/ItemFactoryParamsCreate.h"

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class Scene;
  /*!
  \brief Class responsible for creating or building graphics objects. All objects are children of QGraphicsItem and ItemObserver.
    An object of a type is created from a coordinated. Also an object of a type can be built from the properties saved in a template.
    Only place where the model and the controller is instantiated and the MVC component is created.
    
    \ingroup layout

    \sa te::layout::AbstractBuildGraphicsItem
  */
    class TELAYOUTEXPORT BuildGraphicsItem : public AbstractBuildGraphicsItem
    {
      public:
    /*!
          \brief Constructor
        */
        BuildGraphicsItem(Scene* scene);

    /*!
          \brief Destructor
        */
        virtual ~BuildGraphicsItem();
        
    /*!
          \brief Method to build a graphics object from the properties.
      
      \param props properties
      \param draw graphics object has or not to be drawing
      
      \return z value
        */
        QGraphicsItem* buildItem(te::layout::Properties props);
    
       /*!
         \brief Method to create a graphic object from the properties.
      
         \param itemType type of the object that will be created
         \param coordinate represent the x,y coordinate
         \param draw graphics object has or not to be drawing
      
         \return z value
        */
        QGraphicsItem* createItem(te::layout::EnumType* itemType, const te::gm::Coord2D& coordinate, double width = 0, double height = 0);

        /*!
          \brief Method to create a graphic object from the type.
      
          \param mode type of the object will created
      
          \return item value
        */
        QGraphicsItem* createItem(te::layout::EnumType* itemType);
                
      protected:
    
        /*!
          \brief Creates the name of the new graphic object. Adds the number that corresponds to how many objects of this type have already been created.

          \param name of the class type of the graphic object
          \param type of the object

          \return name
        */
        std::string nameItem(te::layout::EnumType* type);
                
        /*!
          \brief After component construction, this method checks and adds a child, position and zValue. If necessary, also redraws.

          \param item built item
          \param draw if true the component will be redraw, false otherwise
        */
        virtual void afterBuild(QGraphicsItem* item);

      protected:

        virtual ItemFactoryParamsCreate createParams(te::layout::EnumType* type);

        Scene*      m_scene;
    };
  }
}

#endif


