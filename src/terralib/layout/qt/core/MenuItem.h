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
  \file MenuItem.h
   
  \brief Class responsible for creating the menu, right mouse button, and dynamically add menu options.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MENU_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MENU_ITEM_H

// Qt
#include <QObject>

// TerraLib
#include "../../core/property/Properties.h"
#include "../../core/enum/AbstractType.h"
#include "../../core/Config.h"

// STL
#include <string>
#include <vector>

class QAction;
class QMenu;
class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class GridSettingsOutside;
    class EnumType;
    class TextGridSettingsOutside;

    /*!
	  \brief Class responsible for creating the menu, right mouse button, and dynamically add menu options.
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT MenuItem : public QObject
    {
	    Q_OBJECT //for slots/signals

    public:

	    MenuItem( QObject * parent = 0 );

	    virtual ~MenuItem();
         
      virtual void createMenu(QList<QGraphicsItem*> items);

      virtual void menuExec(int x = 0, int y = 0);

      protected slots:

        virtual void onMenuTriggered(QAction* action);

        virtual void onUpdateGridSettingsProperty();

        virtual void onUpdateTextGridSettingsProperty();

    protected:
            
      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

      virtual Properties* intersection(QList<QGraphicsItem*> graphicsItems, bool& window);

      virtual Properties* sameProperties(QList<QGraphicsItem*> graphicsItems, bool& window);

      virtual void contains(std::vector<Properties*>::iterator itend, 
        std::vector<Properties*>::iterator it, std::string name, bool& result);

      virtual std::vector<Properties*> getAllProperties(QList<QGraphicsItem*> graphicsItems, bool& window);
      
      virtual void changePropertyValue(Property property);

      virtual Property findProperty(EnumType* dataType);

      virtual Property findProperty(std::string name);

      virtual void checkedBool( Property property, bool checked );

      virtual void showGridSettingsDlg( Property property );

      virtual void showImageDlg( Property property );

      virtual void showFontDlg( Property property );

      virtual void showColorDlg( Property property );

      virtual void showTextGridSettingsDlg( Property property );

    protected:

      QMenu*                m_menu;
      QList<QGraphicsItem*> m_graphicsItems;
      Properties*           m_properties;
      GridSettingsOutside* m_gridSettings;
      TextGridSettingsOutside* m_textGridSettings;
    };
  }
}

#endif
