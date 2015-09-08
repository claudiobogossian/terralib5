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
  \file OutsideArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideArea.h"
#include "PropertiesDock.h"
#include "ObjectInspectorDock.h"
#include "EditTemplateDock.h"
#include "../../../layout/qt/outside/PropertiesOutside.h"
#include "../../../layout/qt/outside/ObjectInspectorOutside.h"
#include "../../../layout/core/enum/Enums.h"
#include "../../../layout/qt/core/BuildGraphicsItem.h"
#include "../../../layout/qt/core/BuildGraphicsOutside.h"
#include "terralib/layout/qt/outside/ToolbarController.h"

// STL
#include <string>

// Qt
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QStatusBar>

te::layout::OutsideArea::OutsideArea( te::layout::View* view, QStatusBar* status) :
  m_dockProperties(0),
  m_dockInspector(0),
  m_dockEditTemplate(0),
  m_view(view),
  m_toolbar(0),
  m_statusBar(status),
  m_optionNew("mnu_main_new"),
  m_optionUpdate("mnu_main_update"),
  m_optionImportJSON("mnu_main_import_json"),
  m_optionExportJSON("mnu_main_export_json"),
  m_optionPageConfig("mnu_main_page_config"),
  m_optionPrint("mnu_main_print"),
  m_optionExit("mnu_main_exit"),
  m_optionDockInspector("mnu_dock_inspector"),
  m_optionDockProperties("mnu_dock_properties"),
  m_optionDockToolbar("mnu_dock_toolbar"),
  m_optionDockEditTemplate("mnu_dock_edit_template"),
  m_layoutMenu(0)
{
  init();
}

te::layout::OutsideArea::~OutsideArea()
{
    if(m_dockProperties)
    {

      m_dockProperties->close();
      m_dockProperties->setParent(0);
      delete m_dockProperties;
      m_dockProperties = 0;
    }

    if(m_dockInspector)
    {

      m_dockInspector->close();
      m_dockInspector->setParent(0);
      delete m_dockInspector;
      m_dockInspector = 0;
    }

    if(m_toolbar)
    {

      m_toolbar->close();
      m_toolbar->setParent(0);
      delete m_toolbar;
      m_toolbar = 0;
    }

    if(m_dockEditTemplate)
    {

      m_dockEditTemplate->close();
      m_dockEditTemplate->setParent(0);
      delete m_dockEditTemplate;
      m_dockEditTemplate = 0;
    }
}

void te::layout::OutsideArea::init()
{
  if(m_view)
  {
    connect(m_view, SIGNAL(reloadProperties()), this, SLOT(onSelectionChanged()));
    connect(m_view->scene(), SIGNAL(addItemFinalized()), this, SLOT(onAddItemFinalized()));
    connect(m_view, SIGNAL(hideView()), this, SLOT(onHideView()));
    connect(m_view, SIGNAL(closeView()), this, SLOT(onCloseView()));
    connect(m_view, SIGNAL(showView()), this, SLOT(onShowView()));
    connect(this, SIGNAL(changeMenuMode(te::layout::EnumType*)), m_view, SLOT(onMainMenuChangeMode(te::layout::EnumType*)));
    connect(m_view, SIGNAL(changeContext()), this, SLOT(onRefreshStatusBar()));
  }
  
  createPropertiesDock();

  createInspectorDock();
  
  createToolbar();

  createEditTemplateDock();

  if(m_toolbar)
  {
    connect(m_toolbar, SIGNAL(changeMode(te::layout::EnumType*)), m_view, SLOT(onToolbarChangeMode(te::layout::EnumType*)));
    connect(m_toolbar, SIGNAL(zoomChangedInComboBox(int)), m_view, SLOT(setZoom(int)));

    te::layout::ToolbarController* controller = dynamic_cast<te::layout::ToolbarController*>(m_toolbar->getController());
    connect(m_view, SIGNAL(zoomChanged(int)), controller, SLOT(onZoomChanged(int)));
  }

  if(m_dockInspector)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)), this, SLOT(onDeleteFinalized(std::vector<std::string>)));

    connect(m_dockInspector->getObjectInspectorOutside(), SIGNAL(selectionChanged(QList<QGraphicsItem*>)), this, SLOT(onSelectionChanged(QList<QGraphicsItem*>)));
  }

  if(m_dockProperties)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)), 
      m_dockProperties->getPropertiesOutside(), SLOT(onClear(std::vector<std::string>)));
  }
}

void te::layout::OutsideArea::createPropertiesDock()
{
  m_dockProperties = new PropertiesDock;  
  m_dockProperties->setFeatures(QDockWidget::DockWidgetMovable |  
    QDockWidget::DockWidgetFloatable);
}

void te::layout::OutsideArea::createInspectorDock()
{
  m_dockInspector = new ObjectInspectorDock;
  m_dockInspector->setFeatures(QDockWidget::DockWidgetMovable |  
    QDockWidget::DockWidgetFloatable);
}

void te::layout::OutsideArea::createToolbar()
{

  te::layout::BuildGraphicsOutside buildOutside;

  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();
  if(!objectType)
  {
    return;
  }

  QWidget* widget = buildOutside.createOuside(objectType->getToolbar());
  if(!widget)
  {
    return;
  }

  m_toolbar = dynamic_cast<te::layout::ToolbarOutside*>(widget);


  /*
   * TODO: Done on plugin
   * win->addToolBar(m_toolbar);*/
}

void te::layout::OutsideArea::createEditTemplateDock()
{
  m_dockEditTemplate = new EditTemplateDock;
  m_dockEditTemplate->setFeatures(QDockWidget::NoDockWidgetFeatures);
  m_dockEditTemplate->setVisible(false);
}

void te::layout::OutsideArea::createMainMenu()
{
  if(!m_layoutMenu)
  {
    m_layoutMenu= new QMenu("Print Model");
  }

   connect(m_layoutMenu, SIGNAL(triggered(QAction*)), this, SLOT(onMainMenuTriggered(QAction*)));

   QAction* actionNew = createAction("New", m_optionNew, "layout-new");
   m_layoutMenu->addAction(actionNew);

   QAction* actionSave = createAction("Update Template", m_optionUpdate, "layout-save");
   m_layoutMenu->addAction(actionSave);

   m_layoutMenu->addSeparator();

   QMenu* mnuImport = m_layoutMenu->addMenu("Import Template");
   QMenu* mnuExport = m_layoutMenu->addMenu("Export Template");

   QAction* actionImportJSON = createAction("Import JSON Template", m_optionImportJSON, "layout-import");
   mnuImport->addAction(actionImportJSON);

   QAction* actionExportJSON = createAction("Export JSON Template", m_optionExportJSON, "layout-export");
   mnuExport->addAction(actionExportJSON);

   m_layoutMenu->addSeparator();

   QAction* actionDockInspector = createAction("Dock Inspector", m_optionDockInspector, "");
   actionDockInspector->setCheckable(true);
   actionDockInspector->setChecked(true);
   m_layoutMenu->addAction(actionDockInspector);

   QAction* actionDockProperties = createAction("Dock Properties", m_optionDockProperties, "");
   actionDockProperties->setCheckable(true);
   actionDockProperties->setChecked(true);
   m_layoutMenu->addAction(actionDockProperties);

   m_layoutMenu->addSeparator();

   QAction* actionPageConfig = createAction("Page Config...", m_optionPageConfig, "layout-page-setup");
   m_layoutMenu->addAction(actionPageConfig);

   QAction* actionPrint = createAction("Print...", m_optionPrint, "layout-printer");
   m_layoutMenu->addAction(actionPrint);

   m_layoutMenu->addSeparator();

}

void te::layout::OutsideArea::onMainMenuTriggered( QAction* action )
{
  te::layout::EnumModeType* type = te::layout::Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_optionNew.c_str()) == 0)
  {
    m_view->newTemplate();
  }
  else if(action->objectName().compare(m_optionUpdate.c_str()) == 0)
  {
    //changeAction wiil be TypeSaveCurrentTemplate
  }
  else if(action->objectName().compare(m_optionImportJSON.c_str()) == 0)
  {
    te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
    m_view->importTemplate(enumTemplate->getJsonType());
  }
  else if(action->objectName().compare(m_optionExportJSON.c_str()) == 0)
  {
    te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
    m_view->exportProperties(enumTemplate->getJsonType());
  }
  else if(action->objectName().compare(m_optionPageConfig.c_str()) == 0)
  {    
    m_view->showPageSetup();
  }
  else if(action->objectName().compare(m_optionPrint.c_str()) == 0)
  {
    m_view->print();
  }
  else if(action->objectName().compare(m_optionExit.c_str()) == 0)
  {
    m_view->close();
    emit exit();
  }
  else if(action->objectName().compare(m_optionDockInspector.c_str()) == 0)
  {
    if(m_dockInspector->isVisible())
    {
      m_dockInspector->setVisible(false);
    }
    else
    {
      m_dockInspector->setVisible(true);
    }
  }
  else if(action->objectName().compare(m_optionDockProperties.c_str()) == 0)
  {
    if(m_dockProperties->isVisible())
    {
      m_dockProperties->setVisible(false);
    }
    else
    {
      m_dockProperties->setVisible(true);
    }
  }
  else if(action->objectName().compare(m_optionDockEditTemplate.c_str()) == 0)
  {
    if(m_dockEditTemplate->isVisible())
    {
      m_dockEditTemplate->setVisible(false);
    }
    else
    {
      m_dockEditTemplate->setVisible(true);
    }
  }
}

QAction* te::layout::OutsideArea::createAction( std::string text, std::string objName, std::string icon, std::string tooltip )
{
  QAction *actionMenu = new QAction(text.c_str(), m_layoutMenu);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}

te::layout::PropertiesDock* te::layout::OutsideArea::getPropertiesDock()
{
  return m_dockProperties;
}

te::layout::ObjectInspectorDock* te::layout::OutsideArea::getObjectInspectorDock()
{
  return m_dockInspector;
}

te::layout::ToolbarOutside* te::layout::OutsideArea::getToolbar()
{
  return m_toolbar;
}

te::layout::EditTemplateDock* te::layout::OutsideArea::getEditTemplate()
{
  return m_dockEditTemplate;
}

void te::layout::OutsideArea::openAllDocks()
{
    if(m_dockProperties)
    {
      m_dockProperties->setVisible(true);
    }
    if(m_dockInspector)
    {
      m_dockInspector->setVisible(true);
    }
    if(m_toolbar)
    {
      m_toolbar->setVisible(true);
    }
    if(m_dockEditTemplate)
    {
      bool visible = m_dockEditTemplate->isVisible();
      m_dockEditTemplate->setVisible(visible);
    }
}

void te::layout::OutsideArea::closeAllDocks()
{
   if(m_dockProperties)
   {
       m_dockProperties->close();
   }
   if(m_dockInspector)
   {
       m_dockInspector->close();
   }
   if(m_toolbar)
   {
       m_toolbar->close();
   }
   if(m_dockEditTemplate)
   {
       m_dockEditTemplate->close();
   }
}

void te::layout::OutsideArea::openMainMenu()
{
  if(!m_layoutMenu)
    return;
  
  bool exist_menu = false;
  QList<QAction*> acts = m_layoutMenu->actions();
 
  foreach(QAction* act, acts)
  {
    act->setVisible(true);
  }

  if(!acts.empty())
  {
    //m_parentMenu: The first QAction of the QMenu is to open the layout
    QAction* firstOption = acts.first();
    firstOption->setVisible(false);
  }
}

void te::layout::OutsideArea::closeMainMenu()
{
   if(!m_layoutMenu)
    return;

  QList<QAction*> acts = m_layoutMenu->actions();
  foreach(QAction* act, acts)
  {
    act->setVisible(false);
  }

  if(!acts.empty())
  {
    //m_parentMenu: The first QAction of the QMenu is to open the layout
    QAction* firstOption = acts.first();
    firstOption->setVisible(true);
  }

  m_view->closeOutsideWindows();
}

void te::layout::OutsideArea::onSelectionChanged()
{
  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();
  QList<QGraphicsItem*> allItems = m_view->scene()->items();

  //Refresh Property window   
  if(m_dockProperties)
    m_dockProperties->getPropertiesOutside()->itemsSelected(graphicsItems, allItems);

  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->selectItems(graphicsItems);
}

void te::layout::OutsideArea::onAddItemFinalized(QGraphicsItem* item)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}

void te::layout::OutsideArea::onSelectionChanged(QList<QGraphicsItem*> selectedItems)
{
  m_view->scene()->clearSelection();
  foreach(QGraphicsItem* item, selectedItems) 
  {
    item->setSelected(true);
  }

  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  if(m_dockProperties)
    m_dockProperties->getPropertiesOutside()->itemsSelected(selectedItems, allItems);
}

void te::layout::OutsideArea::onShowView()
{
  openAllDocks();
  openMainMenu();
}

void te::layout::OutsideArea::onHideView()
{
  closeAllDocks();
  closeMainMenu();
  m_view->closeOutsideWindows();
}

void te::layout::OutsideArea::onCloseView()
{
  closeAllDocks();
  closeMainMenu();
  m_view->closeOutsideWindows();
  emit exit();
}

void te::layout::OutsideArea::onRefreshStatusBar()
{
  if(!m_statusBar)
  {
    return;
  }

  te::layout::EnumType* mode = m_view->getCurrentMode();

  std::string msg;

  if(mode == te::layout::Enums::getInstance().getEnumModeType()->getModeNone())
  {
    msg = "Map Layout - TerraLib 5";
    m_statusBar->showMessage(msg.c_str());
    return;
  }
  
  msg = "Map Layout - TerraLib 5 | Context: ";
  
  std::string s_mode = mode->getLabel();
  if(s_mode.compare("") == 0)
  {
    s_mode = mode->getName();
  }
  msg += s_mode;
  m_statusBar->showMessage(msg.c_str());
}

void te::layout::OutsideArea::onAddChildFinalized( QGraphicsItem* parent, QGraphicsItem* child )
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}

void te::layout::OutsideArea::onDeleteFinalized(std::vector<std::string>)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}


QMenu* te::layout::OutsideArea::getMenu(QMenu* parentMenu)
{
  if(!m_layoutMenu)
  {
    if(parentMenu)
    {
      m_layoutMenu = parentMenu;
    }
    createMainMenu();
  }
  return m_layoutMenu;
}

