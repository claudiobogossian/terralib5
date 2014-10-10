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
   
  \brief 

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
    class TELAYOUTEXPORT EnumObjectType : public AbstractEnum
    {
      public:

        EnumObjectType(); 

        virtual ~EnumObjectType();
        
        virtual EnumType* getRetangleItem() const;

        virtual EnumType* getDefaultTextItem() const;

        virtual EnumType* getMapItem() const;

        virtual EnumType* getMapGridItem() const;
        
        virtual EnumType* getPaperItem() const;
        
        virtual EnumType* getLegendItem() const;
        
        virtual EnumType* getPropertiesWindow() const;
        
        virtual EnumType* getDisplayWindow() const;
        
        virtual EnumType* getObjectInspectorWindow() const;
        
        virtual EnumType* getToolbar() const;
        
        virtual EnumType* getGridSettings() const;
        
        virtual EnumType* getGridPlanar() const;
        
        virtual EnumType* getGridGeodesic() const;
                
        virtual EnumType* getItemGroup() const;
        
        virtual EnumType* getScaleItem() const;
        
        virtual EnumType* getPageSetup() const;
        
        virtual EnumType* getImageItem() const;
        
        virtual EnumType* getSystematicScale() const;
        
        virtual EnumType* getPointItem() const;
        
        virtual EnumType* getArrowItem() const;
        
        virtual EnumType* getEllipseItem() const;
        
        virtual EnumType* getTitleItem() const;
        
        virtual EnumType* getTextGridItem() const;
        
        virtual EnumType* getMultiLineTextItem() const;
        
        virtual EnumType* getEditTemplate() const;
        
        virtual EnumType* getMenuItem() const;
        
        virtual EnumType* getTextGridSettings() const;

        virtual EnumType* getLegendChildItem() const;
        
        virtual EnumType* getObjectUnknown() const;

      protected:

        virtual void init();

      protected:
        
        EnumType* m_retangleItem;
        EnumType* m_defaultTextItem;
        EnumType* m_mapItem;
        EnumType* m_mapGridItem;
        EnumType* m_paperItem;
        EnumType* m_legendItem;
        EnumType* m_propertiesWindow;
        EnumType* m_displayWindow;
        EnumType* m_objectInspectorWindow;
        EnumType* m_toolbar;
        EnumType* m_gridSettings;
        EnumType* m_gridPlanar;
        EnumType* m_gridGeodesic;
        EnumType* m_itemGroup;
        EnumType* m_scaleItem;
        EnumType* m_pageSetup;
        EnumType* m_imageItem;
        EnumType* m_systematicScale;
        EnumType* m_pointItem;
        EnumType* m_arrowItem;
        EnumType* m_ellipseItem;
        EnumType* m_titleItem;
        EnumType* m_textGridItem;
        EnumType* m_multiLineTextItem;
        EnumType* m_editTemplate;
        EnumType* m_menuItem;
        EnumType* m_textGridSettings;
        EnumType* m_legendChildItem;
        EnumType* m_objectUnknown;
    };
  }
}

#endif
