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
#include "../../core/pattern/singleton/Context.h"
#include "../../core/AbstractScene.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/OutsideModelObservable.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/EnumMode.h"

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
#include <QPushButton>

te::layout::ToolbarOutside::ToolbarOutside( OutsideController* controller, Observable* o ) :
	QToolBar(0),
	OutsideObserver(controller, o),
  m_optionMapDefault("map_default"),
  m_optionMapGrid("map_grid"),
  m_optionLegendDefault("legend_default"),
  m_optionRectangle("geometry_rectangle"),
  m_optionViewPan("view_pan"),
  m_optionViewZoomIn("view_zoom_in"),
  m_optionViewZoomOut("view_zoom_out"),
  m_optionScale("scale_object"),
  m_optionMapPan("map_pan"),
  m_optionMapZoomIn("map_zoom_in"),
  m_optionMapZoomOut("map_zoom_out"),
  m_optionGroup("items_group"),
  m_optionUngroup("items_ungroup"),
  m_optionLineIntersectionMouse("items_intersection_mouse"),
  m_optionSceneZoom("scene_zoom"),
  m_optionTextDefault("text_default"),
  m_optionImage("text_image"),
  m_optionMapSystematicScale("map_systematic_scale"),
  m_optionPoint("geometry_point"),
  m_optionEllipse("geometry_ellipse"),
  m_optionArrow("geometry_arrow"),
  m_optionMultiLineText("text_multiLineText"),
  m_optionStringGrid("text_stringGrid"),
  m_optionTitle("text_title"),
  m_optionMapCreateTextGrid("map_text_grid"),
  m_optionMapCreateMapText("map_text_map"),
  m_optionAlignLeft("align_left"),
  m_optionAlignRight("align_right"),
  m_optionAlignTop("align_top"),
  m_optionAlignBottom("align_bottom"),
  m_optionAlignCenterHorizontal("align_center_horizontal"),
  m_optionAlignCenterVertical("align_center_vertical"),
  m_optionRemoveObject("remove_item"),
  m_btnMap(0)
{
	te::gm::Envelope box = m_model->getBox();	

	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setMinimumSize(200, 10);

  createToolbar();
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
  createArrowCursorButton();
  createRecomposeToolButton();
  this->addSeparator();

  /*createLineIntersectionToolButton();
  this->addSeparator();*/

  createViewAreaToolButton();
  createMapToolsToolButton();
  this->addSeparator();

  createMapToolButton();  
  createGeometryToolButton();
  createTextToolButton();
  this->addSeparator();

  createItemTools();
  this->addSeparator();

  createRemoveObjectToolButton();
  this->addSeparator();

  createBringToFrontToolButton();
  createSendToBackToolButton();
  this->addSeparator();

  createAlignLeftToolButton();
  createAlignTopToolButton();
  createAlignBottomToolButton();
  createAlignRightToolButton();
  this->addSeparator();

  createAlignCenterHorizontalToolButton();
  createAlignCenterVerticalToolButton();
  this->addSeparator();

  createSceneZoomCombobox();
  this->addSeparator();
}

void te::layout::ToolbarOutside::createMapToolButton()
{
  QMenu* menu = new QMenu();

  QAction* actionDefaultMenu = createAction("Default Map Object", m_optionMapDefault, "layout-default-map");
  menu->addAction(actionDefaultMenu);
  
  QAction* actionMapGrid = createAction("Map Grid", m_optionMapGrid, "layout-map-grid");
  menu->addAction(actionMapGrid);

  QAction* actionLegend = createAction("Default Legend", m_optionLegendDefault, "layout-default-legend");
  menu->addAction(actionLegend);

  QAction* actionScale = createAction("Scale Object", m_optionScale, "layout-scale");
  menu->addAction(actionScale);
  
  QToolButton *btnMap = createToolButton("Map", "Map Objects", "layout-default-map");
  btnMap->setMenu(menu);
  btnMap->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onMapTriggered(QAction*)));
  this->addWidget(btnMap);
}

void te::layout::ToolbarOutside::createMapToolsToolButton()
{
  QMenu* menu = new QMenu();

  QAction* actionPan = createAction("Pan Tool", m_optionMapPan, "layout-map-pan");
  menu->addAction(actionPan);

  QAction* actionZoomIn = createAction("Zoom In", m_optionMapZoomIn, "layout-map-zoom-in");
  menu->addAction(actionZoomIn);

  QAction* actionZoomOut = createAction("Zoom Out", m_optionMapZoomOut, "layout-map-zoom-out");
  menu->addAction(actionZoomOut);

  QAction* actionSystematic = createAction("Systematic Scale", m_optionMapSystematicScale, "layout-systematic-map");
  menu->addAction(actionSystematic);

  QAction* actionTextGrid = createAction("Text Grid as Object", m_optionMapCreateTextGrid, "layout-createtext-as-obj");
  menu->addAction(actionTextGrid);

  QAction* actionTextMap = createAction("Text Map as Object", m_optionMapCreateMapText, "layout-createmap-text-as-objs");
  menu->addAction(actionTextMap);
  
  QToolButton *btnMapTools = createToolButton("Map Tools", "Map Tools", "layout-map-pan");
  btnMapTools->setMenu(menu);
  btnMapTools->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onMapToolsTriggered(QAction*)));
  this->addWidget(btnMapTools);
}

void te::layout::ToolbarOutside::createGeometryToolButton()
{
  QMenu* menu = new QMenu();

  QAction* actionRectagle = createAction("Rectangle Object", m_optionRectangle, "layout-square");
  menu->addAction(actionRectagle);

  QAction* actionArrow = createAction("Arrow Object", m_optionArrow, "layout-arrow");
  menu->addAction(actionArrow);

  QAction* actionEllipse = createAction("Ellipse Object", m_optionEllipse, "layout-ellipse");
  menu->addAction(actionEllipse);

  QAction* actionPoint = createAction("Point Object", m_optionPoint, "layout-point");
  menu->addAction(actionPoint);

  QToolButton *btnGeometry = createToolButton("Geometry Tools", "Geometries Objects", "layout-square");
  btnGeometry->setMenu(menu);
  btnGeometry->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onGeometryTriggered(QAction*)));
  this->addWidget(btnGeometry);
}

void te::layout::ToolbarOutside::createViewAreaToolButton()
{
  QMenu* menu = new QMenu();

  QAction* actionPan = createAction("Pan Tool", m_optionViewPan, "layout-paper-pan");
  menu->addAction(actionPan);
  
  QAction* actionZoomIn = createAction("Zoom In", m_optionViewZoomIn, "layout-paper-zoom-in");
  menu->addAction(actionZoomIn);
  
  QAction* actionZoomOut = createAction("Zoom Out", m_optionRectangle, "layout-paper-zoom-out");
  menu->addAction(actionZoomOut);

  QToolButton *btnViewArea = createToolButton("Paper Tools", "Paper Tools", "layout-paper-pan");
  btnViewArea->setMenu(menu);
  btnViewArea->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onViewAreaTriggered(QAction*)));
  this->addWidget(btnViewArea);
}

void te::layout::ToolbarOutside::createArrowCursorButton()
{
  QToolButton *btnArrowCursor = createToolButton("Arrow Cursor", "Arrow Cursor", "layout-default-cursor");
  btnArrowCursor->setCheckable(false);
  connect(btnArrowCursor, SIGNAL(clicked(bool)), this, SLOT(onArrowCursorClicked(bool)));

  this->addWidget(btnArrowCursor);
}

void te::layout::ToolbarOutside::createItemTools()
{
  QMenu* menu = new QMenu();

  QAction* actionGroup = createAction("Group", m_optionGroup, "layout-group");
  menu->addAction(actionGroup);

  QAction* actionUngroup = createAction("Ungroup", m_optionUngroup, "layout-ungroup");
  menu->addAction(actionUngroup);
  
  QToolButton *btnTools = createToolButton("Item Tools", "Item Tools", "layout-group");
  btnTools->setMenu(menu);
  btnTools->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onItemToolsTriggered(QAction*)));
  this->addWidget(btnTools);
}

void te::layout::ToolbarOutside::createLineIntersectionToolButton()
{
  QToolButton *btnLineMouse = createToolButton("Draw Line Intersection Mouse", "Draw Line Intersection Mouse", "layout-draw-line-mouse-intersection");
  connect(btnLineMouse, SIGNAL(toggled(bool)), this, SLOT(onLineIntersectionMouse(bool)));

  this->addWidget(btnLineMouse);
}

void te::layout::ToolbarOutside::createSceneZoomCombobox()
{
  m_comboSceneZoom = new QComboBox();
  m_comboSceneZoom->setObjectName(m_optionSceneZoom.c_str());

  m_comboSceneZoom->addItem("42%", 0.42);
  m_comboSceneZoom->addItem("50%", 0.5);
  m_comboSceneZoom->addItem("70%", 0.7);
  m_comboSceneZoom->addItem("100%", 1.);
  m_comboSceneZoom->addItem("150%", 1.5);
  m_comboSceneZoom->addItem("200%", 2.);
  m_comboSceneZoom->addItem("300%", 3.);
  m_comboSceneZoom->addItem("400%", 4.); 

  connect(m_comboSceneZoom, SIGNAL(currentIndexChanged(int)), this, SLOT(onSceneZoomCurrentIndexChanged(int)));
  m_comboSceneZoom->setCurrentIndex(1);
  Context::getInstance().setDefaultZoomFactor(m_comboSceneZoom->itemData(1).toDouble());
  
  this->addWidget(m_comboSceneZoom);
}

void te::layout::ToolbarOutside::createBringToFrontToolButton()
{
  QToolButton *btn = createToolButton("Bring to front", "Bring to front", "layout-in-front");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onBringToFrontClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createSendToBackToolButton()
{
  QToolButton *btn = createToolButton("Send to back", "Send to back", "layout-to-back");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onSendToBackClicked(bool)));

  this->addWidget(btn);
}


void te::layout::ToolbarOutside::createRecomposeToolButton()
{
  QToolButton *btn = createToolButton("Recompose", "Recompose", "layout-recompose");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRecomposeClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createTextToolButton()
{
  QMenu* menu = new QMenu();

  QAction* actionTxtDefault = createAction("Default Text Object", m_optionTextDefault, "layout-default-text");
  menu->addAction(actionTxtDefault);

  QAction* actionMultiLineText = createAction("MultiLine Text Object", m_optionMultiLineText, "layout-multiline-text");
  menu->addAction(actionMultiLineText);

  QAction* actionTitle = createAction("Title Object", m_optionTitle, "layout-title");
  menu->addAction(actionTitle);

  QAction* actionStringGrid = createAction("Grid Object", m_optionStringGrid, "layout-grid");
  menu->addAction(actionStringGrid);

  QAction* actionImage = createAction("Image Object", m_optionImage, "layout-image");
  menu->addAction(actionImage);

  QToolButton *btn = createToolButton("Text Tools", "Text Tools", "layout-default-text");
  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::InstantPopup);

  connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(onTextToolsTriggered(QAction*)));
  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignLeftToolButton()
{
  QToolButton *btn = createToolButton("Align Left", "Align Left", "layout-alignleft");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignLeftClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignRightToolButton()
{
  QToolButton *btn = createToolButton("Align Right", "Align Right", "layout-alignright");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignRightClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignTopToolButton()
{
  QToolButton *btn = createToolButton("Align Top", "Align Top", "layout-aligntop");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignTopClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignBottomToolButton()
{
  QToolButton *btn = createToolButton("Align Bottom", "Align Bottom", "layout-alignbottom");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignBottomClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignCenterHorizontalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Horizontal", "Align Center Horizontal", "layout-alignhrzcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterHorizontalClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createAlignCenterVerticalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Vertical", "Align Center Vertical", "layout-alignvrtcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterVerticalClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::createRemoveObjectToolButton()
{
  QToolButton *btn = createToolButton("Remove Object", "Remove Object", "layout-empty-trash");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRemoveObjectClicked(bool)));

  this->addWidget(btn);
}

void te::layout::ToolbarOutside::onMapTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionMapDefault.c_str()) == 0)
  {
    changeAction(TypeCreateMap);
  }
  else if(action->objectName().compare(m_optionMapGrid.c_str()) == 0)
  {
    changeAction(TypeCreateMapGrid);
  }
  else if(action->objectName().compare(m_optionLegendDefault.c_str()) == 0)
  {
    changeAction(TypeCreateLegend);
  }
  else if(action->objectName().compare(m_optionScale.c_str()) == 0)
  {
    changeAction(TypeCreateScale);
  }
}

void te::layout::ToolbarOutside::onMapToolsTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionMapPan.c_str()) == 0)
  {
    changeAction(TypeMapPan);
  }
  else if(action->objectName().compare(m_optionMapZoomIn.c_str()) == 0)
  {
    changeAction(TypeMapZoomIn);
  }
  else if(action->objectName().compare(m_optionMapZoomOut.c_str()) == 0)
  {
    changeAction(TypeMapZoomOut);
  }
  else if(action->objectName().compare(m_optionMapSystematicScale.c_str()) == 0)
  {
    changeAction(TypeSystematicScale);
  }
  else if(action->objectName().compare(m_optionMapCreateTextGrid.c_str()) == 0)
  {
    changeAction(TypeMapCreateTextGrid);
  }
  else if(action->objectName().compare(m_optionMapCreateMapText.c_str()) == 0)
  {
    changeAction(TypeMapCreateTextMap);
  }
}

void te::layout::ToolbarOutside::onGeometryTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionRectangle.c_str()) == 0)
  {
    changeAction(TypeCreateRectangle);
  }
  else if(action->objectName().compare(m_optionArrow.c_str()) == 0)
  {
    changeAction(TypeCreateArrow);
  }
  else if(action->objectName().compare(m_optionEllipse.c_str()) == 0)
  {
    changeAction(TypeCreateEllipse);
  }
  else if(action->objectName().compare(m_optionPoint.c_str()) == 0)
  {
    changeAction(TypeCreatePoint);
  }
}

void te::layout::ToolbarOutside::onViewAreaTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionViewPan.c_str()) == 0)
  {
    changeAction(TypePan);
  }
  else if(action->objectName().compare(m_optionViewZoomIn.c_str()) == 0)
  {
    changeAction(TypeZoomIn);
  }
  else if(action->objectName().compare(m_optionViewZoomOut.c_str()) == 0)
  {
    changeAction(TypeZoomOut);
  }
}

void te::layout::ToolbarOutside::onArrowCursorClicked(bool checked)
{
  changeAction(TypeArrowCursor);
}

void te::layout::ToolbarOutside::onItemToolsTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionGroup.c_str()) == 0)
  {
    changeAction(TypeGroup);
  }
  else if(action->objectName().compare(m_optionUngroup.c_str()) == 0)
  {
    changeAction(TypeUngroup);
  }
}

void te::layout::ToolbarOutside::onLineIntersectionMouse( bool checked )
{
  bool result = false;
  LayoutMode mouseMode = Context::getInstance().getLineIntersectionMouseMode();

  if(checked)
  {
    if(TypeActiveLinesIntersectionMouse != mouseMode)
    {
      Context::getInstance().setLineIntersectionMouseMode(TypeActiveLinesIntersectionMouse);
    }
  }
  else
  {
    if(TypeOffLinesIntersectionMouse != mouseMode)
    {
      Context::getInstance().setLineIntersectionMouseMode(TypeOffLinesIntersectionMouse);
    }
  }

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onSceneZoomCurrentIndexChanged( int index )
{
  QVariant variant = m_comboSceneZoom->itemData(index);
  double zoomFactor = Context::getInstance().getZoomFactor();
  if(variant.toDouble() != zoomFactor)
  {
    Context::getInstance().setZoomFactor(variant.toDouble());
    Context::getInstance().setOldZoomFactor(zoomFactor);
    changeAction(TypeSceneZoom);
  }
}

void te::layout::ToolbarOutside::onBringToFrontClicked( bool checked )
{
  changeAction(TypeBringToFront);
}

void te::layout::ToolbarOutside::onSendToBackClicked( bool checked )
{
  changeAction(TypeSendToBack);
}

void te::layout::ToolbarOutside::onRecomposeClicked( bool checked )
{
  m_comboSceneZoom->setCurrentIndex(1);
  changeAction(TypeRecompose);
}

void te::layout::ToolbarOutside::onTextToolsTriggered( QAction* action )
{
  if(action->objectName().compare(m_optionTextDefault.c_str()) == 0)
  {
    changeAction(TypeCreateText);
  }
  else if(action->objectName().compare(m_optionMultiLineText.c_str()) == 0)
  {
    changeAction(TypeCreateMultiLineText);
  }
  else if(action->objectName().compare(m_optionStringGrid.c_str()) == 0)
  {
    changeAction(TypeCreateTextGrid);
  }
  else if(action->objectName().compare(m_optionTitle.c_str()) == 0)
  {
    changeAction(TypeCreateTitle);
  }
  else if(action->objectName().compare(m_optionImage.c_str()) == 0)
  {
    changeAction(TypeCreateImage);
  }
}

void te::layout::ToolbarOutside::onAlignLeftClicked( bool checked )
{
  changeAction(TypeAlignLeft);
}

void te::layout::ToolbarOutside::onAlignRightClicked( bool checked )
{
  changeAction(TypeAlignRight);
}

void te::layout::ToolbarOutside::onAlignTopClicked( bool checked )
{
  changeAction(TypeAlignTop);
}

void te::layout::ToolbarOutside::onAlignBottomClicked( bool checked )
{
  changeAction(TypeAlignBottom);
}

void te::layout::ToolbarOutside::onAlignCenterHorizontalClicked( bool checked )
{
  changeAction(TypeAlignCenterHorizontal);
}

void te::layout::ToolbarOutside::onAlignCenterVerticalClicked( bool checked )
{
  changeAction(TypeAlignCenterVertical);
}

void te::layout::ToolbarOutside::onRemoveObjectClicked( bool checked )
{
  changeAction(TypeRemoveObject);
}

void te::layout::ToolbarOutside::changeAction( LayoutMode mode )
{
  bool result = true;
  LayoutMode layoutMode = Context::getInstance().getMode();

  if(mode != layoutMode)
  {
    Context::getInstance().setMode(mode);
  }
  else
  {
    result = false;
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
  
  btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QPushButton* te::layout::ToolbarOutside::createPushButton( std::string text, std::string tooltip, std::string icon )
{
  QPushButton *btn = new QPushButton;
  btn->setText(text.c_str());
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());

  btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QAction* te::layout::ToolbarOutside::createAction( std::string text, std::string objName, std::string icon, std::string tooltip )
{
  QAction *actionMenu = new QAction(text.c_str(), this);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}
