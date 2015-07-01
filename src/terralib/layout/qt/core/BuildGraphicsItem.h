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
#include "../../core/Config.h"

// Qt
#include <QObject>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class ItemModelObservable;

	/*!
  \brief Class responsible for creating or building graphics objects. All objects are children of QGraphicsItem and ItemObserver.
    An object of a type is created from a coordinated. Also an object of a type can be built from the properties saved in a template.
    Only place where the model and the controller is instantiated and the MVC component is created.
	  
	  \ingroup layout

	  \sa te::layout::AbstractBuildGraphicsItem
	*/
    class TELAYOUTEXPORT BuildGraphicsItem : public QObject, public AbstractBuildGraphicsItem
    {
      Q_OBJECT //for slots/signals

      public:

		/*!
          \brief Constructor
        */
        BuildGraphicsItem();

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
        QGraphicsItem* rebuildItem(te::layout::Properties* props, bool draw = true);
		
		   /*!
         \brief Method to create a graphic object from the properties.
		  
		     \param mode type of the object will created
		     \param coordinate represent the x,y coordinate
		     \param draw graphics object has or not to be drawing
		  
		     \return z value
        */
        QGraphicsItem* createItem(te::layout::EnumType* mode, const te::gm::Coord2D& coordinate, bool draw = true);

        /*!
          \brief Method to create a graphic object from the type.
		  
		      \param mode type of the object will created
		  
		      \return item value
        */
        QGraphicsItem* createItem(te::layout::EnumType* type, bool draw = true);

        /*!
          \brief Checks whether the coordinated intersects an item and adds a child. 
          Checks ItemModelObservable::isEnableChildren().

          \param x axis x coordinate
          \param y axis y coordinate
        */
        virtual bool addChild(QGraphicsItem* child, int x, int y);

      signals:

        void addChildFinalized(QGraphicsItem* parent, QGraphicsItem* child);

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
        virtual void afterBuild(QGraphicsItem* item, bool draw = true);

		    /*!
          \brief Create graphic object of type PaperItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createPaper();

		    /*!
          \brief Create graphic object of type MapItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createMap();
            
        /*!
          \brief Create graphic object of type GridMapItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createGridMap();

		    /*!
          \brief Create graphic object of type TextItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createText();
    
		    /*!
          \brief Create graphic object of type RectangleItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createRectangle();

		    /*!
          \brief Create graphic object of type LegendItem
		  		  
		      \return new object 
        */
	      QGraphicsItem* createLegend();
		
		    /*!
          \brief Create graphic object of type ScaleItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createScale();
		
		    /*!
          \brief Create graphic object of type ItemGroup
		  		  
		      \return new object 
        */
        QGraphicsItem* createItemGroup();

        /*!
          \brief Create graphic object of type MovingItemGroup
		  		  
		      \return new object 
        */
        QGraphicsItem* createMovingItemGroup();

		    /*!
          \brief Create graphic object of type ImageItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createImage();  
		
		    /*!
          \brief Create graphic object of type ArrowItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createArrow();
		
		    /*!
          \brief Create graphic object of type EllipseItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createEllipse();
		
		    /*!
          \brief Create graphic object of type PointItem
		  		  
		      \return new object 
        */        
        QGraphicsItem* createPoint();

		    /*!
          \brief Create graphic object of type TextGridItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createTextGrid();
		
		    /*!
          \brief Create graphic object of type TitleItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createTitle();
		
		    /*!
          \brief Create graphic object of type LegendChildItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createLegendChild();

        /*!
          \brief Create graphic object of type LineItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createLine();

       /*!
          \brief Create graphic object of type PolygonItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createPolygon();

        /*!
          \brief Create graphic object of type SVGItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createSVG();
       /*!
          \brief Create graphic object of type Balloon
		  		  
		      \return new object 
        */

        QGraphicsItem* createBalloon();
        
        /*!
          \brief Create graphic object of type BarCode
		  		  
		      \return new object 
        */
        QGraphicsItem* createBarCode();

        /*!
          \brief Create graphic object of type GridPlanarItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createGridPlanar();

        /*!
          \brief Create graphic object of type GridGeodesicItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createGridGeodesic();

        /*!
          \brief Create graphic object of type NorthItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createNorth();

        /*!
          \brief Create graphic object of type MapLocationItem
		  		  
		      \return new object 
        */
        QGraphicsItem* createMapLocation();
    };
  }
}

#endif


