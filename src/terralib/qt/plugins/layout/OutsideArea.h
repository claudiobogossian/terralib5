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

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H 
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H

// Qt
#include <QObject>

// TerraLib
#include "../../../layout/qt/core/View.h"

class QWidget;
class QMenu;
class QAction;

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout
      {
        class PropertiesDock;
        class ObjectInspectorDock;
        class ToolbarDock;
        class EditTemplateDock;
        class EnumType;

        class OutsideArea : public QObject
        {
	        Q_OBJECT //for slots/signals

        public:

	        OutsideArea(te::layout::View* view, QWidget* dockParent = 0, QMenu* mnuLayout = 0);
	        virtual ~OutsideArea();

          PropertiesDock* getPropertiesDock();
          ObjectInspectorDock* getObjectInspectorDock();
          ToolbarDock* getToolbarDock();

          virtual void openAllDocks();
          virtual void closeAllDocks();

          virtual void openMainMenu();
          virtual void closeMainMenu();

        public slots:

          virtual void onMainMenuTriggered(QAction* action);
          virtual void onSelectionChanged();
          virtual void onAddItemFinalized();
          virtual void onShowView();
          virtual void onHideView();
          virtual void onCloseView();

        signals:

          void changeMenuContext(bool change);

        protected:

          virtual void init();
          virtual void createPropertiesDock();
          virtual void createInspectorDock();
          virtual void createToolbarDock();
          virtual void createMainMenu();
          virtual void createEditTemplateDock();

          virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

          virtual void changeAction(te::layout::EnumType* mode);

        protected:

          QWidget* m_dockParent;
          PropertiesDock* m_dockProperties;
          ObjectInspectorDock* m_dockInspector;
          ToolbarDock* m_dockToolbar;
          EditTemplateDock* m_dockEditTemplate;
          QMenu* m_mainMenu;
          QMenu* m_parentMenu;
          te::layout::View* m_view;

          /* Menu options */
      
          std::string m_optionNew;
          std::string m_optionUpdate;
          std::string m_optionImport;
          std::string m_optionExport;
          std::string m_optionPageConfig;
          std::string m_optionPrint;
          std::string m_optionExit;
          std::string m_optionDockInspector;
          std::string m_optionDockProperties;
          std::string m_optionDockToolbar;
          std::string m_optionDockEditTemplate;
        };
      }
    }
  }
}


#endif
