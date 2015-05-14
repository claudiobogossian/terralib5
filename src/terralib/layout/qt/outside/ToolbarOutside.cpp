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
#include "../../core/enum/Enums.h"

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
#include <QUndoStack>

te::layout::ToolbarOutside::ToolbarOutside( OutsideController* controller, Observable* o ) :
  QToolBar(0),
  OutsideObserver(controller, o),
  m_comboZoom(0),
  m_actionMapDefault("map_default"),
  m_actionLegendDefault("legend_default"),
  m_actionScale("scale_object"),
  m_actionMapZoomIn("map_zoom_in"),
  m_actionMapZoomOut("map_zoom_out"),
  m_actionMapPan("map_pan"),
  m_actionMapSystematicScale("map_systematic_scale"),
  m_actionMapCreateTextGrid("map_text_grid"),
  m_actionMapCreateMapText("map_text_map"),
  m_actionMapCreateLegendChildAsObject("legend_child"),
  m_actionGridMap("map_grid_map"),
  m_actionGridPlanar("map_grid_planar"),
  m_actionGridGeodesic("map_grid_geodesic"),
  m_actionNorth("map_north"),
  m_actionMapLocation("map_location"),
  m_actionRectangle("geometry_rectangle"),
  m_actionPoint("geometry_point"),
  m_actionEllipse("geometry_ellipse"),
  m_actionArrow("geometry_arrow"),
  m_actionLine("geometry_line"),
  m_actionPolygon("geometry_polygon"),
  m_actionViewPan("view_pan"),
  m_actionViewZoomIn("view_zoom_in"),
  m_actionViewZoomOut("view_zoom_out"),
  m_actionGroup("items_group"),
  m_actionUngroup("items_ungroup"),
  m_actionLineIntersectionMouse("items_intersection_mouse"),
  m_actionSceneZoom("scene_zoom"),
  m_actionRemoveObject("remove_item"),
  m_actionTextDefault("text_default"),
  m_actionImage("text_image"),
  m_actionStringGrid("text_stringGrid"),
  m_actionTitle("text_title"),
  m_actionBalloon("text_balloon"),
  m_actionBarCode("text_barCode"),
  m_actionAlignLeft("align_left"),
  m_actionAlignRight("align_right"),
  m_actionAlignTop("align_top"),
  m_actionAlignBottom("align_bottom"),
  m_actionAlignCenterHorizontal("align_center_horizontal"),
  m_actionAlignCenterVertical("align_center_vertical"),
  m_actionUndo("undo"),
  m_actionRedo("redo"),
  m_actionDrawMap("draw_map"),
  m_actionObjectToImage("object_to_image"),
  m_actionExit("exit"),
  m_actionExportToPDF("export_to_pdf"),
  m_mapToolButton(0),
  m_mapToolsToolButton(0),
  m_geometryToolButton(0),
  m_viewAreaToolButton(0),
  m_arrowCursorButton(0),
  m_itemTools(0),
  m_lineIntersectionToolButton(0),
  m_bringToFrontToolButton(0),
  m_sendToBackToolButton(0),
  m_recomposeToolButton(0),
  m_textToolButton(0),
  m_alignLeftToolButton(0),
  m_alignRightToolButton(0),
  m_alignTopToolButton(0),
  m_alignBottomToolButton(0),
  m_alignCenterHorizontalToolButton(0),
  m_alignCenterVerticalToolButton(0),
  m_removeObjectToolButton(0),
  m_undoToolButton(0),
  m_drawMapToolButton(0),
  m_objectToImageButton(0),
  m_exitButton(0),
  m_exportToPDFButton(0),
  m_actionComboZoom(0),
  m_actionMapToolButton(0),
  m_actionMapToolsToolButton(0),
  m_actionGeometryToolButton(0),
  m_actionViewAreaToolButton(0),
  m_actionArrowCursorButton(0),
  m_actionItemTools(0),
  m_actionLineIntersectionToolButton(0),
  m_actionBringToFrontToolButton(0),
  m_actionSendToBackToolButton(0),
  m_actionRecomposeToolButton(0),
  m_actionTextToolButton(0),
  m_actionAlignLeftToolButton(0),
  m_actionAlignRightToolButton(0),
  m_actionAlignTopToolButton(0),
  m_actionAlignBottomToolButton(0),
  m_actionAlignCenterHorizontalToolButton(0),
  m_actionAlignCenterVerticalToolButton(0),
  m_actionRemoveObjectToolButton(0),
  m_actionUndoToolButton(0),
  m_actionDrawMapToolButton(0),
  m_actionObjectToImageButton(0),
  m_actionExitButton(0),
  m_actionExportToPDFButton(0)
{
	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setMinimumSize(200, 10);
}

te::layout::ToolbarOutside::~ToolbarOutside()
{

}

void te::layout::ToolbarOutside::updateObserver( ContextItem context )
{
	setVisible(context.isShow());
	if(context.isShow() == true)
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
  createDrawMapToolButton();
  this->addSeparator();

  createRecomposeToolButton();
  this->addSeparator();

  createUndoToolButton();
  this->addSeparator();
  
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

  createObjectToImageButton();
  this->addSeparator();

  createExportToPDFButton();
  this->addSeparator();

  createSceneZoomCombobox();
  this->addSeparator();

  createExitButton();
  this->addSeparator();
}

QToolButton* te::layout::ToolbarOutside::createMapToolButton()
{
  QToolButton *btnMap = createToolButton("Map", "Map Objects", "");

  QMenu* menu = new QMenu(btnMap);

  QAction* actionDefaultMenu = createAction("Default Map Object", m_actionMapDefault, "layout-default-map", "", menu);
  menu->addAction(actionDefaultMenu);
  
  QAction* actionGridPlanar = createAction("Grid Planar", m_actionGridPlanar, "layout-grid", "", menu);
  menu->addAction(actionGridPlanar);

  QAction* actionGridGeodesic = createAction("Grid Geodesic", m_actionGridGeodesic, "layout-grid", "", menu);
  menu->addAction(actionGridGeodesic);

  QAction* actionLegend = createAction("Default Legend", m_actionLegendDefault, "layout-default-legend", "", menu);
  menu->addAction(actionLegend);

  QAction* actionScale = createAction("Scale Object", m_actionScale, "layout-scale", "", menu);
  menu->addAction(actionScale);

  QAction* actionNorth = createAction("North Object", m_actionNorth, "layout-north", "", menu);
  menu->addAction(actionNorth);

  QAction* actionMapLocation = createAction("Map Location Object", m_actionMapLocation, "layout-map-location", "", menu);
  menu->addAction(actionMapLocation);
  
  btnMap->setMenu(menu);
  btnMap->setPopupMode(QToolButton::MenuButtonPopup);
  btnMap->setDefaultAction(actionDefaultMenu);

  connect(btnMap, SIGNAL(triggered(QAction*)), this, SLOT(onMapTriggered(QAction*)));
  m_actionMapToolButton = this->addWidget(btnMap);

  m_mapToolButton = btnMap;

  return btnMap;
}

QToolButton* te::layout::ToolbarOutside::createMapToolsToolButton()
{
  QToolButton *btnMapTools = createToolButton("Map Tools", "Map Tools", "");

  QMenu* menu = new QMenu(btnMapTools);

  QAction* actionPan = createAction("Pan Tool", m_actionMapPan, "layout-map-pan", "", menu);
  menu->addAction(actionPan);

  QAction* actionZoomIn = createAction("Zoom In", m_actionMapZoomIn, "layout-map-zoom-in", "", menu);
  menu->addAction(actionZoomIn);

  QAction* actionZoomOut = createAction("Zoom Out", m_actionMapZoomOut, "layout-map-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  QAction* actionSystematic = createAction("Systematic Scale", m_actionMapSystematicScale, "layout-systematic-map", "", menu);
  menu->addAction(actionSystematic);

  QAction* actionTextGrid = createAction("Text Grid as Object", m_actionMapCreateTextGrid, "layout-createtext-as-obj", "", menu);
  menu->addAction(actionTextGrid);

  QAction* actionTextMap = createAction("Text Map as Object", m_actionMapCreateMapText, "layout-createmap-text-as-objs", "", menu);
  menu->addAction(actionTextMap);

  QAction* actionLegend = createAction("Legend as Object", m_actionMapCreateLegendChildAsObject, "layout-legend-child", "", menu);
  menu->addAction(actionLegend);

  btnMapTools->setMenu(menu);
  btnMapTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnMapTools->setDefaultAction(actionPan);

  connect(btnMapTools, SIGNAL(triggered(QAction*)), this, SLOT(onMapToolsTriggered(QAction*)));
  m_actionMapToolsToolButton = this->addWidget(btnMapTools);

  m_mapToolsToolButton = btnMapTools;

  return btnMapTools;
}

QToolButton* te::layout::ToolbarOutside::createGeometryToolButton()
{
  QToolButton *btnGeometry = createToolButton("Geometry Tools", "Geometries Objects", "");

  QMenu* menu = new QMenu(btnGeometry);

  QAction* actionRectagle = createAction("Rectangle Object", m_actionRectangle, "layout-square", "", menu);
  menu->addAction(actionRectagle);

  QAction* actionArrow = createAction("Arrow Object", m_actionArrow, "layout-arrow", "", menu);
  menu->addAction(actionArrow);

  QAction* actionEllipse = createAction("Ellipse Object", m_actionEllipse, "layout-ellipse", "", menu);
  menu->addAction(actionEllipse);

  QAction* actionPoint = createAction("Point Object", m_actionPoint, "layout-point", "", menu);
  menu->addAction(actionPoint);

  QAction* actionLine = createAction("Line Object", m_actionLine, "layout-drawline", "", menu); 
  menu->addAction(actionLine);

  QAction* actionPolygon = createAction("Polygon Object", m_actionPolygon, "layout-polygon", "", menu); 
  menu->addAction(actionPolygon);

  btnGeometry->setMenu(menu);
  btnGeometry->setPopupMode(QToolButton::MenuButtonPopup);
  btnGeometry->setDefaultAction(actionRectagle);

  connect(btnGeometry, SIGNAL(triggered(QAction*)), this, SLOT(onGeometryTriggered(QAction*)));
  m_actionGeometryToolButton = this->addWidget(btnGeometry);

  m_geometryToolButton = btnGeometry;

  return btnGeometry;
}

QToolButton* te::layout::ToolbarOutside::createViewAreaToolButton()
{
  QToolButton *btnViewArea = createToolButton("Paper Tools", "Paper Tools", "");

  QMenu* menu = new QMenu(btnViewArea);

  QAction* actionPan = createAction("Pan Tool", m_actionViewPan, "layout-paper-pan", "", menu);
  menu->addAction(actionPan);
  
  QAction* actionZoomIn = createAction("Zoom In", m_actionViewZoomIn, "layout-paper-zoom-in", "", menu);
  menu->addAction(actionZoomIn);
  
  QAction* actionZoomOut = createAction("Zoom Out", m_actionViewZoomOut, "layout-paper-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  btnViewArea->setMenu(menu);
  btnViewArea->setPopupMode(QToolButton::MenuButtonPopup);
  btnViewArea->setDefaultAction(actionPan);

  connect(btnViewArea, SIGNAL(triggered(QAction*)), this, SLOT(onViewAreaTriggered(QAction*)));
  m_actionViewAreaToolButton = this->addWidget(btnViewArea);

  m_viewAreaToolButton = btnViewArea;

  return btnViewArea;
}

QToolButton* te::layout::ToolbarOutside::createArrowCursorButton()
{
  QToolButton *btnArrowCursor = createToolButton("Arrow Cursor", "Arrow Cursor", "layout-default-cursor");
  btnArrowCursor->setCheckable(false);
  connect(btnArrowCursor, SIGNAL(clicked(bool)), this, SLOT(onArrowCursorClicked(bool)));

  m_actionArrowCursorButton = this->addWidget(btnArrowCursor);

  m_arrowCursorButton = btnArrowCursor;

  return btnArrowCursor;
}

QToolButton* te::layout::ToolbarOutside::createItemTools()
{
  QToolButton *btnTools = createToolButton("Item Tools", "Item Tools", "");

  QMenu* menu = new QMenu(btnTools);

  QAction* actionGroup = createAction("Group", m_actionGroup, "layout-group", "", menu);
  menu->addAction(actionGroup);

  QAction* actionUngroup = createAction("Ungroup", m_actionUngroup, "layout-ungroup", "", menu);
  menu->addAction(actionUngroup);

  btnTools->setMenu(menu);
  btnTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnTools->setDefaultAction(actionGroup);

  connect(btnTools, SIGNAL(triggered(QAction*)), this, SLOT(onItemToolsTriggered(QAction*)));
  m_actionItemTools = this->addWidget(btnTools);

  m_itemTools = btnTools;

  return btnTools;
}

QToolButton* te::layout::ToolbarOutside::createLineIntersectionToolButton()
{
  QToolButton *btnLineMouse = createToolButton("Draw Line Intersection Mouse", "Draw Line Intersection Mouse", "layout-draw-line-mouse-intersection");
  connect(btnLineMouse, SIGNAL(toggled(bool)), this, SLOT(onLineIntersectionMouse(bool)));

  m_actionLineIntersectionToolButton = this->addWidget(btnLineMouse);

  m_lineIntersectionToolButton = btnLineMouse;

  return btnLineMouse;
}

QComboBox* te::layout::ToolbarOutside::createSceneZoomCombobox()
{
  m_comboZoom = new QComboBox(this);
  m_comboZoom->setObjectName(m_actionSceneZoom.c_str());

  m_comboZoom->addItem("42%", 0.42);
  m_comboZoom->addItem("50%", 0.5);
  m_comboZoom->addItem("70%", 0.7);
  m_comboZoom->addItem("100%", 1.);
  m_comboZoom->addItem("150%", 1.5);
  m_comboZoom->addItem("200%", 2.);
  m_comboZoom->addItem("300%", 3.); 

  connect(m_comboZoom, SIGNAL(currentIndexChanged(int)), this, SLOT(onSceneZoomCurrentIndexChanged(int)));
  m_comboZoom->setCurrentIndex(1);
  Context::getInstance().setDefaultZoomFactor(m_comboZoom->itemData(1).toDouble());
  
  m_actionComboZoom = this->addWidget(m_comboZoom);
  
  return m_comboZoom;
}

QToolButton* te::layout::ToolbarOutside::createBringToFrontToolButton()
{
  QToolButton *btn = createToolButton("Bring to front", "Bring to front", "layout-in-front");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onBringToFrontClicked(bool)));

  m_actionBringToFrontToolButton = this->addWidget(btn);

  m_bringToFrontToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createSendToBackToolButton()
{
  QToolButton *btn = createToolButton("Send to back", "Send to back", "layout-to-back");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onSendToBackClicked(bool)));

  m_actionSendToBackToolButton = this->addWidget(btn);

  m_sendToBackToolButton = btn;

  return btn;
}


QToolButton* te::layout::ToolbarOutside::createRecomposeToolButton()
{
  QToolButton *btn = createToolButton("Recompose", "Recompose", "layout-recompose");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRecomposeClicked(bool)));

  m_actionRecomposeToolButton = this->addWidget(btn);

  m_recomposeToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createTextToolButton()
{
  QToolButton *btn = createToolButton("Text Tools", "Text Tools", "");

  QMenu* menu = new QMenu(btn);

  QAction* actionTxtDefault = createAction("Default Text Object", m_actionTextDefault, "layout-default-text", "", menu);
  menu->addAction(actionTxtDefault);
  
  QAction* actionTitle = createAction("Title Object", m_actionTitle, "layout-title", "", menu);
  menu->addAction(actionTitle);

  QAction* actionStringGrid = createAction("Text Grid Object", m_actionStringGrid, "layout-grid", "", menu);
  menu->addAction(actionStringGrid);

  QAction* actionImage = createAction("Image Object", m_actionImage, "layout-image", "", menu);
  menu->addAction(actionImage);

  QAction* actionBalloon = createAction("Balloon Object", m_actionBalloon, "layout-ballon", "", menu); 
  menu->addAction(actionBalloon);

  QAction* actionBarCode = createAction("BarCode Object", m_actionBarCode, "layout-barcode", "", menu);
  menu->addAction(actionBarCode);

  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::MenuButtonPopup);
  btn->setDefaultAction(actionTxtDefault);

  connect(btn, SIGNAL(triggered(QAction*)), this, SLOT(onTextToolsTriggered(QAction*)));
  m_actionTextToolButton = this->addWidget(btn);

  m_textToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignLeftToolButton()
{
  QToolButton *btn = createToolButton("Align Left", "Align Left", "layout-alignleft");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignLeftClicked(bool)));

  m_actionAlignLeftToolButton = this->addWidget(btn);

  m_alignLeftToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignRightToolButton()
{
  QToolButton *btn = createToolButton("Align Right", "Align Right", "layout-alignright");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignRightClicked(bool)));

  m_actionAlignRightToolButton = this->addWidget(btn);

  m_alignRightToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignTopToolButton()
{
  QToolButton *btn = createToolButton("Align Top", "Align Top", "layout-aligntop");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignTopClicked(bool)));

  m_actionAlignTopToolButton = this->addWidget(btn);

  m_alignTopToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignBottomToolButton()
{
  QToolButton *btn = createToolButton("Align Bottom", "Align Bottom", "layout-alignbottom");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignBottomClicked(bool)));

  m_actionAlignBottomToolButton = this->addWidget(btn);

  m_alignBottomToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterHorizontalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Horizontal", "Align Center Horizontal", "layout-alignhrzcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterHorizontalClicked(bool)));

  m_actionAlignCenterHorizontalToolButton = this->addWidget(btn);

  m_alignCenterHorizontalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterVerticalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Vertical", "Align Center Vertical", "layout-alignvrtcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterVerticalClicked(bool)));

  m_actionAlignCenterVerticalToolButton = this->addWidget(btn);

  m_alignCenterVerticalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createRemoveObjectToolButton()
{
  QToolButton *btn = createToolButton("Remove Object", "Remove Object", "layout-empty-trash");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRemoveObjectClicked(bool)));

  m_actionRemoveObjectToolButton = this->addWidget(btn);

  m_removeObjectToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createUndoToolButton()
{
  QToolButton *btn = createToolButton("Undo/Redo", "Undo/Redo", "");

  QMenu* menu = new QMenu(btn);
  
  Scene* lScene = dynamic_cast<Scene*>(Context::getInstance().getScene());

  QUndoStack* undoStack = lScene->getUndoStack();

  if(!undoStack)
    return 0;

  QAction* actionUndo = undoStack->createUndoAction(menu, tr("&Undo"));
  actionUndo->setShortcuts(QKeySequence::Undo);
  actionUndo->setObjectName(m_actionUndo.c_str());
  actionUndo->setIcon(QIcon::fromTheme("layout-undo"));
  actionUndo->setToolTip("Undo Action");
  menu->addAction(actionUndo);

  QAction* actionRedo  = undoStack->createRedoAction(menu, tr("&Redo"));
  actionRedo->setShortcuts(QKeySequence::Redo);
  actionRedo->setObjectName(m_actionRedo.c_str());
  actionRedo->setIcon(QIcon::fromTheme("layout-redo"));
  actionRedo->setToolTip("Redo Action");
  menu->addAction(actionRedo);
    
  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::MenuButtonPopup);
  btn->setDefaultAction(actionUndo);

  m_actionUndoToolButton = this->addWidget(btn);
  
  m_undoToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createDrawMapToolButton()
{
  QToolButton *btn = createToolButton("Redraw Selection Map", "Redraw Selection Map", "layout-draw-map");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onDrawMapClicked(bool)));

  m_actionDrawMapToolButton = this->addWidget(btn);

  m_drawMapToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createObjectToImageButton()
{
  QToolButton *btn = createToolButton("Object To Image", "Export all selected objects to image", "layout-object-to-image");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onObjectToImageClicked(bool)));

  m_actionObjectToImageButton = this->addWidget(btn);

  m_objectToImageButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExitButton()
{
  QToolButton *btn = createToolButton("Exit", "Exit", "layout-close");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onExitClicked(bool)));

  m_actionExitButton = this->addWidget(btn);

  m_exitButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExportToPDFButton()
{
  QToolButton *btn = createToolButton("Export To PDF", "Export to PDF", "layout-pdf");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onExportToPDFClicked(bool)));

  m_actionExportToPDFButton = this->addWidget(btn);

  m_exportToPDFButton = btn;

  return btn;
}

void te::layout::ToolbarOutside::onMapTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_actionMapDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateMap());
  }
  else if(action->objectName().compare(m_actionGridMap.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridMap());
  }
  else if(action->objectName().compare(m_actionGridPlanar.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridPlanar());
  }
  else if(action->objectName().compare(m_actionGridGeodesic.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridGeodesic());
  }
  else if(action->objectName().compare(m_actionLegendDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateLegend());
  }
  else if(action->objectName().compare(m_actionScale.c_str()) == 0)
  {
    changeAction(type->getModeCreateScale());
  }
  else if(action->objectName().compare(m_actionNorth.c_str()) == 0)
  {
    changeAction(type->getModeCreateNorth());
  }
  else if(action->objectName().compare(m_actionMapLocation.c_str()) == 0)
  {
    changeAction(type->getModeCreateMapLocation());
  }
}

void te::layout::ToolbarOutside::onMapToolsTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_actionMapPan.c_str()) == 0)
  {
    changeAction(type->getModeMapPan());
  }
  else if(action->objectName().compare(m_actionMapZoomIn.c_str()) == 0)
  {
    changeAction(type->getModeMapZoomIn());
  }
  else if(action->objectName().compare(m_actionMapZoomOut.c_str()) == 0)
  {
    changeAction(type->getModeMapZoomOut());
  }
  else if(action->objectName().compare(m_actionMapSystematicScale.c_str()) == 0)
  {
    changeAction(type->getModeSystematicScale());
  }
  else if(action->objectName().compare(m_actionMapCreateTextGrid.c_str()) == 0)
  {
    changeAction(type->getModeMapCreateTextGrid());
  }
  else if(action->objectName().compare(m_actionMapCreateMapText.c_str()) == 0)
  {
    changeAction(type->getModeMapCreateTextMap());
  }
  else if(action->objectName().compare(m_actionMapCreateLegendChildAsObject.c_str()) == 0)
  {
    changeAction(type->getModeLegendChildAsObject());
  }
}

void te::layout::ToolbarOutside::onGeometryTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_actionRectangle.c_str()) == 0)
  {
    changeAction(type->getModeCreateRectangle());
  }
  else if(action->objectName().compare(m_actionArrow.c_str()) == 0)
  {
    changeAction(type->getModeCreateArrow());
  }
  else if(action->objectName().compare(m_actionEllipse.c_str()) == 0)
  {
    changeAction(type->getModeCreateEllipse());
  }
  else if(action->objectName().compare(m_actionPoint.c_str()) == 0)
  {
    changeAction(type->getModeCreatePoint());
  }
  else if (action->objectName().compare(m_actionLine.c_str()) == 0) 
  {
    changeAction(type->getModeCreateLine());
    Context::getInstance().setWait(type->getModeCoordWait());
  }
  else if (action->objectName().compare(m_actionPolygon.c_str()) == 0) 
  {
    changeAction(type->getModeCreatePolygon());
    Context::getInstance().setWait(type->getModeCoordWait());
  }
}

void te::layout::ToolbarOutside::onViewAreaTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_actionViewPan.c_str()) == 0)
  {
    changeAction(type->getModePan());
  }
  else if(action->objectName().compare(m_actionViewZoomIn.c_str()) == 0)
  {
    changeAction(type->getModeZoomIn());
  }
  else if(action->objectName().compare(m_actionViewZoomOut.c_str()) == 0)
  {
    changeAction(type->getModeZoomOut());
  }
}

void te::layout::ToolbarOutside::onArrowCursorClicked(bool checked)
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeArrowCursor());
}

void te::layout::ToolbarOutside::onItemToolsTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(action->objectName().compare(m_actionGroup.c_str()) == 0)
  {
    changeAction(type->getModeGroup());
  }
  else if(action->objectName().compare(m_actionUngroup.c_str()) == 0)
  {
    changeAction(type->getModeUngroup());
  }
}

void te::layout::ToolbarOutside::onLineIntersectionMouse( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  bool result = false;
  EnumType* mouseMode = Context::getInstance().getLineIntersectionMouseMode();

  if(checked)
  {
    if(type->getModeActiveLinesIntersectionMouse() != mouseMode)
    {
      Context::getInstance().setLineIntersectionMouseMode(type->getModeActiveLinesIntersectionMouse());
    }
  }
  else
  {
    if(type->getModeOffLinesIntersectionMouse() != mouseMode)
    {
      Context::getInstance().setLineIntersectionMouseMode(type->getModeOffLinesIntersectionMouse() );
    }
  }

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onSceneZoomCurrentIndexChanged( int index )
{
  QVariant variant = m_comboZoom->itemData(index);
  double zoomFactor = Context::getInstance().getZoomFactor();
  if(variant.toDouble() != zoomFactor)
  {
    EnumModeType* type = Enums::getInstance().getEnumModeType();
    Context::getInstance().setZoomFactor(variant.toDouble());
    Context::getInstance().setOldZoomFactor(zoomFactor);
    changeAction(type->getModeSceneZoom());
  }
}

void te::layout::ToolbarOutside::onBringToFrontClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeBringToFront());
}

void te::layout::ToolbarOutside::onSendToBackClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeSendToBack());
}

void te::layout::ToolbarOutside::onRecomposeClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  m_comboZoom->setCurrentIndex(1);
  changeAction(type->getModeRecompose());
}

void te::layout::ToolbarOutside::onTextToolsTriggered( QAction* action )
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button)
  {
    button->setDefaultAction(action);
  }

  EnumModeType* type = Enums::getInstance().getEnumModeType();
  if(action->objectName().compare(m_actionTextDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateText());
  }
  else if(action->objectName().compare(m_actionStringGrid.c_str()) == 0)
  {
    changeAction(type->getModeCreateTextGrid());
  }
  else if(action->objectName().compare(m_actionTitle.c_str()) == 0)
  {
    changeAction(type->getModeCreateTitle());
  }
  else if(action->objectName().compare(m_actionImage.c_str()) == 0)
  {
    changeAction(type->getModeCreateImage());
  }
  else if(action->objectName().compare(m_actionBalloon.c_str()) == 0)
  {
    changeAction(type->getModeCreateBalloon());
  }
  else if(action->objectName().compare(m_actionBarCode.c_str()) == 0)
  {
    changeAction(type->getModeCreateBarCode());
  }
}

void te::layout::ToolbarOutside::onAlignLeftClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignLeft());
}

void te::layout::ToolbarOutside::onAlignRightClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignRight());
}

void te::layout::ToolbarOutside::onAlignTopClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignTop());
}

void te::layout::ToolbarOutside::onAlignBottomClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignBottom());
}

void te::layout::ToolbarOutside::onAlignCenterHorizontalClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignCenterHorizontal());
}

void te::layout::ToolbarOutside::onAlignCenterVerticalClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeAlignCenterVertical());
}

void te::layout::ToolbarOutside::onRemoveObjectClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeRemoveObject());
}

void te::layout::ToolbarOutside::onDrawMapClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeDrawSelectionMap());
}

void te::layout::ToolbarOutside::onObjectToImageClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeObjectToImage());
}

void te::layout::ToolbarOutside::onExitClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeExit());
}

void te::layout::ToolbarOutside::onExportToPDFClicked( bool checked )
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeAction(type->getModeExportToPDF());
}

void te::layout::ToolbarOutside::changeAction( EnumType* mode )
{
  bool result = true;
  EnumType* layoutMode = Context::getInstance().getMode();

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
  QToolButton *btn = new QToolButton(this);
  btn->setText(text.c_str());
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());
  
  if(!icon.empty())
	btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QPushButton* te::layout::ToolbarOutside::createPushButton( std::string text, std::string tooltip, std::string icon )
{
  QPushButton *btn = new QPushButton(this);
  btn->setText(text.c_str());
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());

  btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QAction* te::layout::ToolbarOutside::createAction( std::string text, std::string objName, std::string icon, std::string tooltip, QWidget* parent )
{
  QWidget* parentToUse = parent;
  if(parentToUse == 0)
  {
	parentToUse = this;
  }

  QAction *actionMenu = new QAction(text.c_str(), parentToUse);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}

QComboBox* te::layout::ToolbarOutside::getComboBoxZoom()
{
  return m_comboZoom;
}

void te::layout::ToolbarOutside::onChangeZoom( double factor )
{
  int index = m_comboZoom->findData(factor);
  
  if ( index != -1 ) { // -1 for not found
    m_comboZoom->setCurrentIndex(index);
  }
}

QToolButton* te::layout::ToolbarOutside::getMapToolButton()
{
  return m_mapToolButton;
}

QToolButton* te::layout::ToolbarOutside::getMapToolsToolButton()
{
  return m_mapToolsToolButton;
}

QToolButton* te::layout::ToolbarOutside::getGeometryToolButton()
{
  return m_geometryToolButton;
}

QToolButton* te::layout::ToolbarOutside::getViewAreaToolButton()
{
  return m_viewAreaToolButton;
}

QToolButton* te::layout::ToolbarOutside::getArrowCursorButton()
{
  return m_arrowCursorButton;
}

QToolButton* te::layout::ToolbarOutside::getItemTools()
{
  return m_itemTools;
}

QToolButton* te::layout::ToolbarOutside::getLineIntersectionToolButton()
{
  return m_lineIntersectionToolButton;
}

QToolButton* te::layout::ToolbarOutside::getBringToFrontToolButton()
{
  return m_bringToFrontToolButton;
}

QToolButton* te::layout::ToolbarOutside::getSendToBackToolButton()
{
  return m_sendToBackToolButton;
}

QToolButton* te::layout::ToolbarOutside::getRecomposeToolButton()
{
  return m_recomposeToolButton;
}

QToolButton* te::layout::ToolbarOutside::getTextToolButton()
{
  return m_textToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignLeftToolButton()
{
  return m_alignLeftToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignRightToolButton()
{
  return m_alignRightToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignTopToolButton()
{
  return m_alignTopToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignBottomToolButton()
{
  return m_alignBottomToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignCenterHorizontalToolButton()
{
  return m_alignCenterHorizontalToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignCenterVerticalToolButton()
{
  return m_alignCenterVerticalToolButton;
}

QToolButton* te::layout::ToolbarOutside::getRemoveObjectToolButton()
{
  return m_removeObjectToolButton;
}

QToolButton* te::layout::ToolbarOutside::getUndoToolButton()
{
  return m_undoToolButton;
}

QToolButton* te::layout::ToolbarOutside::getDrawMapToolButton()
{
  return m_drawMapToolButton;
}

QToolButton* te::layout::ToolbarOutside::getObjectToImageButton()
{
  return m_objectToImageButton;
}

QToolButton* te::layout::ToolbarOutside::getExitButton()
{
  return m_exitButton;
}

std::string te::layout::ToolbarOutside::getActionMapDefault()
{
  return m_actionMapDefault;
}

std::string te::layout::ToolbarOutside::getActionLegendDefault()
{
  return m_actionLegendDefault;
}

std::string te::layout::ToolbarOutside::getActionScale()
{
  return m_actionScale;
}

std::string te::layout::ToolbarOutside::getActionMapZoomIn()
{
  return m_actionMapZoomIn;
}

std::string te::layout::ToolbarOutside::getActionMapZoomOut()
{
  return m_actionMapZoomOut;
}

std::string te::layout::ToolbarOutside::getActionMapPan()
{
  return m_actionMapPan;
}

std::string te::layout::ToolbarOutside::getActionMapSystematicScale()
{
  return m_actionMapSystematicScale;
}

std::string te::layout::ToolbarOutside::getActionMapCreateTextGrid()
{
  return m_actionMapCreateTextGrid;
}

std::string te::layout::ToolbarOutside::getActionMapCreateMapText()
{
  return m_actionMapCreateMapText;
}

std::string te::layout::ToolbarOutside::getActionMapCreateLegendChildAsObject()
{
  return m_actionMapCreateLegendChildAsObject;
}

std::string te::layout::ToolbarOutside::getActionGridMap()
{
  return m_actionGridMap;
}

std::string te::layout::ToolbarOutside::getActionGridPlanar()
{
  return m_actionGridPlanar;
}

std::string te::layout::ToolbarOutside::getActionGridGeodesic()
{
  return m_actionGridGeodesic;
}

std::string te::layout::ToolbarOutside::getActionNorth()
{
  return m_actionNorth;
}

std::string te::layout::ToolbarOutside::getActionMapLocation()
{
  return m_actionMapLocation;
}

std::string te::layout::ToolbarOutside::getActionRectangle()
{
  return m_actionRectangle;
}

std::string te::layout::ToolbarOutside::getActionPoint()
{
  return m_actionPoint;
}

std::string te::layout::ToolbarOutside::getActionEllipse()
{
  return m_actionEllipse;
}

std::string te::layout::ToolbarOutside::getActionArrow()
{
  return m_actionArrow;
}

std::string te::layout::ToolbarOutside::getActionLine()
{
  return m_actionLine;
}

std::string te::layout::ToolbarOutside::getActionPolygon()
{
  return m_actionPolygon;
}

std::string te::layout::ToolbarOutside::getActionViewPan()
{
  return m_actionViewPan;
}

std::string te::layout::ToolbarOutside::getActionViewZoomIn()
{
  return m_actionViewZoomIn;
}

std::string te::layout::ToolbarOutside::getActionViewZoomOut()
{
  return m_actionViewZoomOut;
}

std::string te::layout::ToolbarOutside::getActionGroup()
{
  return m_actionGroup;
}

std::string te::layout::ToolbarOutside::getActionUngroup()
{
  return m_actionUngroup;
}

std::string te::layout::ToolbarOutside::getActionLineIntersectionMouse()
{
  return m_actionLineIntersectionMouse;
}

std::string te::layout::ToolbarOutside::getActionSceneZoom()
{
  return m_actionSceneZoom;
}

std::string te::layout::ToolbarOutside::getActionRemoveObject()
{
  return m_actionRemoveObject;
}

std::string te::layout::ToolbarOutside::getActionTextDefault()
{
  return m_actionTextDefault;
}

std::string te::layout::ToolbarOutside::getActionImage()
{
  return m_actionImage;
}

std::string te::layout::ToolbarOutside::getActionStringGrid()
{
  return m_actionStringGrid;
}

std::string te::layout::ToolbarOutside::getActionTitle()
{
  return m_actionTitle;
}

std::string te::layout::ToolbarOutside::getActionBalloon()
{
  return m_actionBalloon;
}

std::string te::layout::ToolbarOutside::getActionBarCode()
{
  return m_actionBarCode;
}

std::string te::layout::ToolbarOutside::getActionAlignLeft()
{
  return m_actionAlignLeft;
}

std::string te::layout::ToolbarOutside::getActionAlignRight()
{
  return m_actionAlignRight;
}

std::string te::layout::ToolbarOutside::getActionAlignTop()
{
  return m_actionAlignTop;
}

std::string te::layout::ToolbarOutside::getActionAlignBottom()
{
  return m_actionAlignBottom;
}

std::string te::layout::ToolbarOutside::getActionAlignCenterHorizontal()
{
  return m_actionAlignCenterHorizontal;
}

std::string te::layout::ToolbarOutside::getActionAlignCenterVertical()
{
  return m_actionAlignCenterVertical;
}

std::string te::layout::ToolbarOutside::getActionUndo()
{
  return m_actionUndo;
}

std::string te::layout::ToolbarOutside::getActionRedo()
{
  return m_actionRedo;
}

std::string te::layout::ToolbarOutside::getActionDrawMap()
{
  return m_actionDrawMap;
}

std::string te::layout::ToolbarOutside::getActionObjectToImage()
{
  return m_actionObjectToImage;
}

std::string te::layout::ToolbarOutside::getActionExit()
{
  return m_actionExit;
}

std::string te::layout::ToolbarOutside::getActionExportToPDF()
{
  return m_actionExportToPDF;
}

QAction* te::layout::ToolbarOutside::getActionMapToolButton()
{
  return m_actionMapToolButton;
}

QAction* te::layout::ToolbarOutside::getActionMapToolsToolButton()
{
  return m_actionMapToolsToolButton;
}

QAction* te::layout::ToolbarOutside::getActionGeometryToolButton()
{
  return m_actionGeometryToolButton;
}

QAction* te::layout::ToolbarOutside::getActionViewAreaToolButton()
{
  return m_actionViewAreaToolButton;
}

QAction* te::layout::ToolbarOutside::getActionArrowCursorButton()
{
  return m_actionArrowCursorButton;
}

QAction* te::layout::ToolbarOutside::getActionItemTools()
{
  return m_actionItemTools;
}

QAction* te::layout::ToolbarOutside::getActionLineIntersectionToolButton()
{
  return m_actionLineIntersectionToolButton;
}

QAction* te::layout::ToolbarOutside::getActionBringToFrontToolButton()
{
  return m_actionBringToFrontToolButton;
}

QAction* te::layout::ToolbarOutside::getActionSendToBackToolButton()
{
  return m_actionSendToBackToolButton;
}

QAction* te::layout::ToolbarOutside::getActionRecomposeToolButton()
{
  return m_actionRecomposeToolButton;
}

QAction* te::layout::ToolbarOutside::getActionTextToolButton()
{
  return m_actionTextToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignLeftToolButton()
{
  return m_actionAlignLeftToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignRightToolButton()
{
  return m_actionAlignRightToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignTopToolButton()
{
  return m_actionAlignTopToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignBottomToolButton()
{
  return m_actionAlignBottomToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignCenterHorizontalToolButton()
{
  return m_actionAlignCenterHorizontalToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignCenterVerticalToolButton()
{
  return m_actionAlignCenterVerticalToolButton;
}

QAction* te::layout::ToolbarOutside::getActionRemoveObjectToolButton()
{
  return m_actionRemoveObjectToolButton;
}

QAction* te::layout::ToolbarOutside::getActionUndoToolButton()
{
  return m_actionUndoToolButton;
}

QAction* te::layout::ToolbarOutside::getActionDrawMapToolButton()
{
  return m_actionDrawMapToolButton;
}

QAction* te::layout::ToolbarOutside::getActionObjectToImageButton()
{
  return m_actionObjectToImageButton;
}

QAction* te::layout::ToolbarOutside::getActionExitButton()
{
  return m_actionExitButton;
}

QAction* te::layout::ToolbarOutside::getActionComboBoxZoom()
{
  return m_actionComboZoom;
}



