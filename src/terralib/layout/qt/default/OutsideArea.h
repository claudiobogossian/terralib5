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
  \file OutsideArea.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H 
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H

// TerraLib
#include "../../../layout/qt/core/View.h"
#include "../../../layout/qt/outside/ToolbarOutside.h"

// STL
#include <string>

// Qt
#include <QObject>

class QWidget;
class QMenu;
class QAction;
class QStatusBar;
class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class PropertiesDock;
    class ObjectInspectorDock;
    class EditTemplateDock;
    class EnumType;

    class OutsideArea : public QObject
    {
      Q_OBJECT //for slots/signals

    public:

      OutsideArea(te::layout::View* view, QStatusBar* status = 0);
      virtual ~OutsideArea();

      PropertiesDock* getPropertiesDock();
      ObjectInspectorDock* getObjectInspectorDock();
      te::layout::ToolbarOutside* getToolbar();
      te::layout::EditTemplateDock* getEditTemplate();
      QMenu* getMenu(QMenu* parentMenu);

      virtual void openAllDocks();
      virtual void closeAllDocks();

      virtual void openMainMenu();
      virtual void closeMainMenu();

    public slots:

      virtual void onMainMenuTriggered(QAction* action);
      virtual void onAddItemFinalized();
      virtual void onShowView();
      virtual void onHideView();
      virtual void onCloseView();
      virtual void onRefreshStatusBar();
      virtual void onAddChildFinalized(QGraphicsItem* parent, QGraphicsItem* child);
      virtual void onDeleteFinalized(std::vector<std::string>);
      virtual void onSelectionChanged(QList<QGraphicsItem*> selectedItems);
      virtual void onSelectionChanged();


    signals:

      void changeMenuMode(te::layout::EnumType* newMode);

      void exit();

    protected:

      virtual void init();
      virtual void createPropertiesDock();
      virtual void createInspectorDock();
      virtual void createToolbar();
      virtual void createMainMenu();
      virtual void createEditTemplateDock();

      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

    protected:

      PropertiesDock* m_dockProperties;
      ObjectInspectorDock* m_dockInspector;
      EditTemplateDock* m_dockEditTemplate;
      te::layout::View* m_view;
      te::layout::ToolbarOutside* m_toolbar;
      QStatusBar* m_statusBar;
      QMenu* m_layoutMenu;

      /* Menu options */

      std::string m_optionNew;
      std::string m_optionUpdate;
      std::string m_optionImportJSON;
      std::string m_optionExportJSON;
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


#endif
