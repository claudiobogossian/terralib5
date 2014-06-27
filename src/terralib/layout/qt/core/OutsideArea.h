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
  \file OutsideArea.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_AREA_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_AREA_H

// Qt
#include <QObject>

// TerraLib
#include "EnumMode.h"

class QWidget;
class QMenu;
class QAction;

namespace te
{
  namespace layout
  {
    class PropertiesOutside;
    class ObjectInspectorOutside;
    class ToolbarOutside;

    class OutsideArea : public QObject
    {
	    Q_OBJECT //for slots/signals

    public:

	    OutsideArea(QWidget* dockParent = 0, QMenu* mnuLayout = 0);
	    virtual ~OutsideArea();

      PropertiesOutside* getPropertiesOutside();
      ObjectInspectorOutside* getObjectInspectorOutside();
      ToolbarOutside* getToolbarOutside();

      virtual void openAllDocks();
      virtual void closeAllDocks();

      virtual void openMainMenu();
      virtual void closeMainMenu();

    public slots:

      virtual void onMainMenuTriggered(QAction* action);

    signals:

      void changeMenuContext(bool change);

    protected:

      virtual void init();
      virtual void createPropertiesDock();
      virtual void createInspectorDock();
      virtual void createToolbarDock();
      virtual void createMainMenu();

      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

      virtual void changeAction(LayoutMode mode);

    protected:

      QWidget* m_dockParent;
      PropertiesOutside* m_dockProperties;
      ObjectInspectorOutside* m_dockInspector;
      ToolbarOutside* m_dockToolbar;
      QMenu* m_mainMenu;
      QMenu* m_parentMenu;

      /* Menu options */
      
      std::string m_optionNew;
      std::string m_optionUpdate;
      std::string m_optionImport;
      std::string m_optionExport;
      std::string m_optionPageConfig;
      std::string m_optionPrint;
      std::string m_optionExit;
    };
  }
}


#endif
