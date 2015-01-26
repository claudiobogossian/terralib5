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
  \file EnumObjectType.h
   
   \brief Class to represent a graphic object (MVC component) and widget object (MVC widget) type enumeration. 
      Ex. component: map, legend, scale, rectangle, text, etc.
      Ex. widget: object inspector, toolbar, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_OBJECTTYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_OBJECTTYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

// STL
#include <vector>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class to represent a graphic object (MVC component) and widget object (MVC widget) type enumeration. 
        Ex. component: map, legend, scale, rectangle, text, etc.
        Ex. widget: object inspector, toolbar, etc.
	  
	    \ingroup layout

	    \sa te::layout::AbstractEnum
	  */
    class TELAYOUTEXPORT EnumObjectType : public AbstractEnum
    {
      public:

        /*!
          \brief Constructor
        */
        EnumObjectType(); 

        /*!
          \brief Destructor
        */ 
        virtual ~EnumObjectType();
        
        /*!
          \brief Returns value that represents rectangle (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getRetangleItem() const;

        /*!
          \brief Returns value that represents map (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getMapItem() const;

        /*!
          \brief Returns value that represents map grid (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getMapGridItem() const;
        
        /*!
          \brief Returns value that represents paper (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getPaperItem() const;
        
        /*!
          \brief Returns value that represents legend (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getLegendItem() const;
        
        /*!
          \brief Returns value that represents properties widget (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getPropertiesWindow() const;
        
        /*!
          \brief Returns value that represents QGraphicsView widget (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getDisplayWindow() const;
        
        /*!
          \brief Returns value that represents object inspector widget (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getObjectInspectorWindow() const;
        
        /*!
          \brief Returns value that represents toolbar widget (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getToolbar() const;
        
        /*!
          \brief Returns value that represents GridSettings window (MVC dialog) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getGridSettings() const;
        
        /*!
          \brief Returns value that represents grid planar type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getGridPlanar() const;
        
        /*!
          \brief Returns value that represents grid geodesic type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getGridGeodesic() const;
                
        /*!
          \brief Returns value that represents item group (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getItemGroup() const;
        
        /*!
          \brief Returns value that represents scale (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getScaleItem() const;
        
        /*!
          \brief Returns value that represents page setup window (MVC dialog) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getPageSetup() const;
        
        /*!
          \brief Returns value that represents image (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getImageItem() const;
        
        /*!
          \brief Returns value that represents systematic scale window (MVC dialog) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getSystematicScale() const;
        
        /*!
          \brief Returns value that represents point (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getPointItem() const;
        
        /*!
          \brief Returns value that represents arrow (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getArrowItem() const;
        
        /*!
          \brief Returns value that represents ellipse (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getEllipseItem() const;
        
        /*!
          \brief Returns value that represents title (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getTitleItem() const;
        
        /*!
          \brief Returns value that represents text grid (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getTextGridItem() const;
          
        /*!
          \brief Returns value that represents edit template widget (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getEditTemplate() const;
                
        /*!
          \brief Returns value that represents text grid settings (MVC widget) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getTextGridSettings() const;

        /*!
          \brief Returns value that represents legend child (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getLegendChildItem() const;
        
        /*!
          \brief Returns value that represents text (MVC Component) type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getTextItem() const;

        /*!
          \brief Returns value that represents unknown type belonging to enumeration.
		  
		      \return enum value  	  
        */
        virtual EnumType* getObjectUnknown() const;

      protected:

        /*!
          \brief Reimplemented from AbstractEnum
        */
        virtual void init();

      protected:
        
        EnumType* m_retangleItem; //!< value that represents rectangle (MVC Component) type belonging to enumeration
        EnumType* m_mapItem; //!< value that represents map (MVC Component) type belonging to enumeration
        EnumType* m_mapGridItem; //!< value that represents map grid (MVC Component) type belonging to enumeration
        EnumType* m_paperItem; //!< value that represents paper (MVC Component) type belonging to enumeration
        EnumType* m_legendItem; //!< value that represents legend (MVC Component) type belonging to enumeration
        EnumType* m_propertiesWindow; //!< value that represents properties widget (MVC widget) type belonging to enumeration
        EnumType* m_displayWindow; //!< value that represents QGraphicsView widget (MVC widget) type belonging to enumeration
        EnumType* m_objectInspectorWindow; //!< value that represents object inspector widget (MVC widget) type belonging to enumeration
        EnumType* m_toolbar; //!< value that represents toolbar widget (MVC widget) type belonging to enumeration
        EnumType* m_gridSettings; //!< value that represents GridSettings window (MVC dialog) type belonging to enumeration
        EnumType* m_gridPlanar; //!< value that represents grid planar type belonging to enumeration
        EnumType* m_gridGeodesic; //!< value that represents grid geodesic type belonging to enumeration
        EnumType* m_itemGroup; //!< value that represents item group (MVC Component) type belonging to enumeration
        EnumType* m_scaleItem; //!< value that represents scale (MVC Component) type belonging to enumeration
        EnumType* m_pageSetup; //!< value that represents page setup window (MVC dialog) type belonging to enumeration
        EnumType* m_imageItem; //!< value that represents image (MVC Component) type belonging to enumeration
        EnumType* m_systematicScale; //!< value that represents systematic scale window (MVC dialog) type belonging to enumeration
        EnumType* m_pointItem; //!< value that represents point (MVC Component) type belonging to enumeration
        EnumType* m_arrowItem; //!< value that represents arrow (MVC Component) type belonging to enumeration
        EnumType* m_ellipseItem; //!< value that represents ellipse (MVC Component) type belonging to enumeration
        EnumType* m_titleItem; //!< value that represents title (MVC Component) type belonging to enumeration
        EnumType* m_textGridItem; //!< value that represents text grid (MVC Component) type belonging to enumeration
        EnumType* m_editTemplate; //!< value that represents edit template widget (MVC widget) type belonging to enumeration
        EnumType* m_textGridSettings; //!< value that represents text grid settings (MVC widget) type belonging to enumeration
        EnumType* m_legendChildItem; //!< value that represents legend child (MVC Component) type belonging to enumeration
        EnumType* m_textItem; //!< value that represents text (MVC Component) type belonging to enumeration
        EnumType* m_objectUnknown; //!< value that represents unknown type belonging to enumeration
    };
  }
}

#endif
