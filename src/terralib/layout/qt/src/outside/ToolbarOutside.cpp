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
  \file ToolbarOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarOutside.h"
#include "Context.h"
#include "AbstractScene.h"
#include "Scene.h"
#include "OutsideModelObservable.h"
#include "ItemObserver.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "EnumMode.h"
#include "LayoutConfig.h" /*Layout Images*/

//STL
#include <string>

// Qt
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>
#include <QAction>

te::layout::ToolbarOutside::ToolbarOutside( OutsideController* controller, Observable* o ) :
	QDockWidget("", 0, 0),
	OutsideObserver(controller, o),
  m_optionMapDefault("map_default"),
  m_optionMapGrid("map_grid"),
  m_optionLegendDefault("legend_default"),
  m_optionRectangle("geometry_rectangle"),
  m_optionViewPan("view_pan"),
  m_optionViewZoomIn("view_zoom_in"),
  m_optionViewZoomOut("view_zoom_out"),
  m_toolbar(0),
  m_btnMap(0)
{
	te::gm::Envelope box = m_model->getBox();	

	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  setMinimumSize(200, 10);

  createToolbar();
  setWidget(m_toolbar);
}

te::layout::ToolbarOutside::~ToolbarOutside()
{

}

void te::layout::ToolbarOutside::updateObserver( ContextItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::ToolbarOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::ToolbarOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ToolbarOutside::createToolbar()
{
  m_toolbar = new QToolBar;

  createViewAreaToolButton();
  m_toolbar->addSeparator();

  createMapToolButton();
  m_toolbar->addSeparator();
  
  createGeometryToolButton();
  m_toolbar->addSeparator();

  /*QToolButton* btnSalveProps = new QToolButton;
  btnSalveProps->setText("Save Props");
  btnSalveProps->setGeometry(0,0,10,10);
  btnSalveProps->setCheckable(true);
  connect(btnSalveProps, SIGNAL(toggled(bool)), this, SLOT(onClickSalvePropsTool(bool)));*/

  /*m_toolbar->addWidget(btnSalveProps);
  m_toolbar->addSeparator();*/
}

void te::layout::ToolbarOutside::createMapToolButton()
{
  QMenu* menu = new QMenu();
  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onMapTriggered(QAction*)));

  QAction* actionDefaultMenu = createAction("Default Map Object", m_optionMapDefault, "layout-default-map");
  menu->addAction(actionDefaultMenu);
  
  QAction* actionMapGrid = createAction("Map Grid", m_optionMapGrid, "layout-map-grid");
  menu->addAction(actionMapGrid);

  QAction* actionLegend = createAction("Default Legend", m_optionLegendDefault, "layout-default-legend");
  menu->addAction(actionMapGrid);
  
  QToolButton *btnMap = createToolButton("Map", "Default Map Object", "layout-default-map");
  btnMap->setMenu(menu);
  btnMap->setPopupMode(QToolButton::InstantPopup);

  m_toolbar->addWidget(btnMap);
}

void te::layout::ToolbarOutside::createGeometryToolButton()
{
  QMenu* menu = new QMenu();
  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onGeometryTriggered(QAction*)));

  QAction* actionRectagle = createAction("Rectangle Object", m_optionRectangle, "layout-square");
  menu->addAction(actionRectagle);

  QToolButton *btnGeometry = createToolButton("Geometry Tools", "Geometries Objects", "layout-square");
  btnGeometry->setMenu(menu);
  btnGeometry->setPopupMode(QToolButton::InstantPopup);

  m_toolbar->addWidget(btnGeometry);
}

void te::layout::ToolbarOutside::createViewAreaToolButton()
{
  QMenu* menu = new QMenu();
  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onViewAreaTriggered(QAction*)));

  QAction* actionPan = createAction("Pan Tool", m_optionViewPan, "layout-paper-pan");
  menu->addAction(actionPan);
  
  QAction* actionZoomIn = createAction("Zoom In", m_optionViewZoomIn, "layout-paper-zoom-in");
  menu->addAction(actionZoomIn);
  
  QAction* actionZoomOut = createAction("Zoom Out", m_optionRectangle, "layout-paper-zoom-out");
  menu->addAction(actionZoomOut);

  QToolButton *btnViewArea = createToolButton("View Area Tools", "View Area Objects", "layout-paper-pan");
  btnViewArea->setMenu(menu);
  btnViewArea->setPopupMode(QToolButton::InstantPopup);

  m_toolbar->addWidget(btnViewArea);
}

void te::layout::ToolbarOutside::onMapTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionMapDefault.c_str()) == 0)
  {
    changeAction(TypeCreateMap);
  }
  if(action->objectName().compare(m_optionMapGrid.c_str()) == 0)
  {
    changeAction(TypeCreateMapGrid);
  }
  if(action->objectName().compare(m_optionLegendDefault.c_str()) == 0)
  {
    changeAction(TypeCreateLegend);
  }
}

void te::layout::ToolbarOutside::onGeometryTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionRectangle.c_str()) == 0)
  {
    changeAction(TypeCreateRectangle);
  }
}

void te::layout::ToolbarOutside::onViewAreaTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionViewPan.c_str()) == 0)
  {
    changeAction(TypePan);
  }
  if(action->objectName().compare(m_optionViewZoomIn.c_str()) == 0)
  {
    changeAction(TypeZoomIn);
  }
  if(action->objectName().compare(m_optionViewZoomOut.c_str()) == 0)
  {
    changeAction(TypeZoomOut);
  }
}

void te::layout::ToolbarOutside::changeAction( LayoutMode mode )
{
  bool result = false;
  LayoutMode layoutMode = Context::getInstance()->getMode();

  if(mode != layoutMode)
  {
    Context::getInstance()->setMode(mode);
  }

  emit changeContext(result);
}

QToolButton* te::layout::ToolbarOutside::createToolButton( std::string text, std::string tooltip, std::string icon )
{
  QToolButton *btn = new QToolButton;
  btn->setText(text.c_str());
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());
  
  std::string icon_path = LAYOUT_IMAGES_PNG"/" + icon;
  btn->setIcon(QIcon::fromTheme(icon_path.c_str()));

  return btn;
}

QAction* te::layout::ToolbarOutside::createAction( std::string text, std::string objName, std::string icon, std::string tooltip )
{
  QAction *actionMenu = new QAction(text.c_str(), this);
  actionMenu->setObjectName(objName.c_str());

  std::string icon_path = LAYOUT_IMAGES_PNG"/" + icon;
  actionMenu->setIcon(QIcon::fromTheme(icon_path.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}
