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
  m_optionMapDefault("map_default"),
  m_optionLegendDefault("legend_default"),
  m_optionScale("scale_object"),
  m_optionMapZoomIn("map_zoom_in"),
  m_optionMapZoomOut("map_zoom_out"),
  m_optionMapPan("map_pan"),
  m_optionMapSystematicScale("map_systematic_scale"),
  m_optionMapCreateTextGrid("map_text_grid"),
  m_optionMapCreateMapText("map_text_map"),
  m_optionMapCreateLegendChildAsObject("legend_child"),
  m_optionGridMap("map_grid_map"),
  m_optionGridPlanar("map_grid_planar"),
  m_optionGridGeodesic("map_grid_geodesic"),
  m_optionNorth("map_north"),
  m_optionMapLocation("map_location"),
  m_optionRectangle("geometry_rectangle"),
  m_optionPoint("geometry_point"),
  m_optionEllipse("geometry_ellipse"),
  m_optionArrow("geometry_arrow"),
  m_optionLine("geometry_line"),
  m_optionPolygon("geometry_polygon"),
  m_optionViewPan("view_pan"),
  m_optionViewZoomIn("view_zoom_in"),
  m_optionViewZoomOut("view_zoom_out"),
  m_optionGroup("items_group"),
  m_optionUngroup("items_ungroup"),
  m_optionLineIntersectionMouse("items_intersection_mouse"),
  m_optionSceneZoom("scene_zoom"),
  m_optionRemoveObject("remove_item"),
  m_optionTextDefault("text_default"),
  m_optionImage("text_image"),
  m_optionStringGrid("text_stringGrid"),
  m_optionTitle("text_title"),
  m_optionBalloon("text_balloon"),
  m_optionBarCode("text_barCode"),
  m_optionAlignLeft("align_left"),
  m_optionAlignRight("align_right"),
  m_optionAlignTop("align_top"),
  m_optionAlignBottom("align_bottom"),
  m_optionAlignCenterHorizontal("align_center_horizontal"),
  m_optionAlignCenterVertical("align_center_vertical"),
  m_optionUndo("undo"),
  m_optionRedo("redo"),
  m_optionDrawMap("draw_map"),
  m_optionObjectToImage("object_to_image"),
  m_optionExit("exit"),
  m_optionExportToPDF("export_to_pdf"),
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
  m_exportToPDFButton(0)
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

  QAction* actionDefaultMenu = createAction("Default Map Object", m_optionMapDefault, "layout-default-map", "", menu);
  menu->addAction(actionDefaultMenu);
  
  QAction* actionGridMap = createAction("Grid Map", m_optionGridMap, "layout-grid", "", menu);
  menu->addAction(actionGridMap);

  QAction* actionGridPlanar = createAction("Grid Planar", m_optionGridPlanar, "layout-grid", "", menu);
  menu->addAction(actionGridPlanar);

  QAction* actionGridGeodesic = createAction("Grid Geodesic", m_optionGridGeodesic, "layout-grid", "", menu);
  menu->addAction(actionGridGeodesic);

  QAction* actionLegend = createAction("Default Legend", m_optionLegendDefault, "layout-default-legend", "", menu);
  menu->addAction(actionLegend);

  QAction* actionScale = createAction("Scale Object", m_optionScale, "layout-scale", "", menu);
  menu->addAction(actionScale);

  QAction* actionNorth = createAction("North Object", m_optionNorth, "layout-north", "", menu);
  menu->addAction(actionNorth);

  QAction* actionMapLocation = createAction("Map Location Object", m_optionMapLocation, "layout-map-location", "", menu);
  menu->addAction(actionMapLocation);
  
  btnMap->setMenu(menu);
  btnMap->setPopupMode(QToolButton::MenuButtonPopup);
  btnMap->setDefaultAction(actionDefaultMenu);

  connect(btnMap, SIGNAL(triggered(QAction*)), this, SLOT(onMapTriggered(QAction*)));
  this->addWidget(btnMap);

  m_mapToolButton = btnMap;

  return btnMap;
}

QToolButton* te::layout::ToolbarOutside::createMapToolsToolButton()
{
  QToolButton *btnMapTools = createToolButton("Map Tools", "Map Tools", "");

  QMenu* menu = new QMenu(btnMapTools);

  QAction* actionPan = createAction("Pan Tool", m_optionMapPan, "layout-map-pan", "", menu);
  menu->addAction(actionPan);

  QAction* actionZoomIn = createAction("Zoom In", m_optionMapZoomIn, "layout-map-zoom-in", "", menu);
  menu->addAction(actionZoomIn);

  QAction* actionZoomOut = createAction("Zoom Out", m_optionMapZoomOut, "layout-map-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  QAction* actionSystematic = createAction("Systematic Scale", m_optionMapSystematicScale, "layout-systematic-map", "", menu);
  menu->addAction(actionSystematic);

  QAction* actionTextGrid = createAction("Text Grid as Object", m_optionMapCreateTextGrid, "layout-createtext-as-obj", "", menu);
  menu->addAction(actionTextGrid);

  QAction* actionTextMap = createAction("Text Map as Object", m_optionMapCreateMapText, "layout-createmap-text-as-objs", "", menu);
  menu->addAction(actionTextMap);

  QAction* actionLegend = createAction("Legend as Object", m_optionMapCreateLegendChildAsObject, "layout-legend-child", "", menu);
  menu->addAction(actionLegend);

  btnMapTools->setMenu(menu);
  btnMapTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnMapTools->setDefaultAction(actionPan);

  connect(btnMapTools, SIGNAL(triggered(QAction*)), this, SLOT(onMapToolsTriggered(QAction*)));
  this->addWidget(btnMapTools);

  m_mapToolsToolButton = btnMapTools;

  return btnMapTools;
}

QToolButton* te::layout::ToolbarOutside::createGeometryToolButton()
{
  QToolButton *btnGeometry = createToolButton("Geometry Tools", "Geometries Objects", "");

  QMenu* menu = new QMenu(btnGeometry);

  QAction* actionRectagle = createAction("Rectangle Object", m_optionRectangle, "layout-square", "", menu);
  menu->addAction(actionRectagle);

  QAction* actionArrow = createAction("Arrow Object", m_optionArrow, "layout-arrow", "", menu);
  menu->addAction(actionArrow);

  QAction* actionEllipse = createAction("Ellipse Object", m_optionEllipse, "layout-ellipse", "", menu);
  menu->addAction(actionEllipse);

  QAction* actionPoint = createAction("Point Object", m_optionPoint, "layout-point", "", menu);
  menu->addAction(actionPoint);

  QAction* actionLine = createAction("Line Object", m_optionLine, "layout-drawline", "", menu); 
  menu->addAction(actionLine);

  QAction* actionPolygon = createAction("Polygon Object", m_optionPolygon, "layout-polygon", "", menu); 
  menu->addAction(actionPolygon);

  btnGeometry->setMenu(menu);
  btnGeometry->setPopupMode(QToolButton::MenuButtonPopup);
  btnGeometry->setDefaultAction(actionRectagle);

  connect(btnGeometry, SIGNAL(triggered(QAction*)), this, SLOT(onGeometryTriggered(QAction*)));
  this->addWidget(btnGeometry);

  m_geometryToolButton = btnGeometry;

  return btnGeometry;
}

QToolButton* te::layout::ToolbarOutside::createViewAreaToolButton()
{
  QToolButton *btnViewArea = createToolButton("Paper Tools", "Paper Tools", "");

  QMenu* menu = new QMenu(btnViewArea);

  QAction* actionPan = createAction("Pan Tool", m_optionViewPan, "layout-paper-pan", "", menu);
  menu->addAction(actionPan);
  
  QAction* actionZoomIn = createAction("Zoom In", m_optionViewZoomIn, "layout-paper-zoom-in", "", menu);
  menu->addAction(actionZoomIn);
  
  QAction* actionZoomOut = createAction("Zoom Out", m_optionViewZoomOut, "layout-paper-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  btnViewArea->setMenu(menu);
  btnViewArea->setPopupMode(QToolButton::MenuButtonPopup);
  btnViewArea->setDefaultAction(actionPan);

  connect(btnViewArea, SIGNAL(triggered(QAction*)), this, SLOT(onViewAreaTriggered(QAction*)));
  this->addWidget(btnViewArea);

  m_viewAreaToolButton = btnViewArea;

  return btnViewArea;
}

QToolButton* te::layout::ToolbarOutside::createArrowCursorButton()
{
  QToolButton *btnArrowCursor = createToolButton("Arrow Cursor", "Arrow Cursor", "layout-default-cursor");
  btnArrowCursor->setCheckable(false);
  connect(btnArrowCursor, SIGNAL(clicked(bool)), this, SLOT(onArrowCursorClicked(bool)));

  this->addWidget(btnArrowCursor);

  m_arrowCursorButton = btnArrowCursor;

  return btnArrowCursor;
}

QToolButton* te::layout::ToolbarOutside::createItemTools()
{
  QToolButton *btnTools = createToolButton("Item Tools", "Item Tools", "");

  QMenu* menu = new QMenu(btnTools);

  QAction* actionGroup = createAction("Group", m_optionGroup, "layout-group", "", menu);
  menu->addAction(actionGroup);

  QAction* actionUngroup = createAction("Ungroup", m_optionUngroup, "layout-ungroup", "", menu);
  menu->addAction(actionUngroup);

  btnTools->setMenu(menu);
  btnTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnTools->setDefaultAction(actionGroup);

  connect(btnTools, SIGNAL(triggered(QAction*)), this, SLOT(onItemToolsTriggered(QAction*)));
  this->addWidget(btnTools);

  m_itemTools = btnTools;

  return btnTools;
}

QToolButton* te::layout::ToolbarOutside::createLineIntersectionToolButton()
{
  QToolButton *btnLineMouse = createToolButton("Draw Line Intersection Mouse", "Draw Line Intersection Mouse", "layout-draw-line-mouse-intersection");
  connect(btnLineMouse, SIGNAL(toggled(bool)), this, SLOT(onLineIntersectionMouse(bool)));

  this->addWidget(btnLineMouse);

  m_lineIntersectionToolButton = btnLineMouse;

  return btnLineMouse;
}

QComboBox* te::layout::ToolbarOutside::createSceneZoomCombobox()
{
  m_comboZoom = new QComboBox(this);
  m_comboZoom->setObjectName(m_optionSceneZoom.c_str());

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
  
  this->addWidget(m_comboZoom);
  
  return m_comboZoom;
}

QToolButton* te::layout::ToolbarOutside::createBringToFrontToolButton()
{
  QToolButton *btn = createToolButton("Bring to front", "Bring to front", "layout-in-front");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onBringToFrontClicked(bool)));

  this->addWidget(btn);

  m_bringToFrontToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createSendToBackToolButton()
{
  QToolButton *btn = createToolButton("Send to back", "Send to back", "layout-to-back");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onSendToBackClicked(bool)));

  this->addWidget(btn);

  m_sendToBackToolButton = btn;

  return btn;
}


QToolButton* te::layout::ToolbarOutside::createRecomposeToolButton()
{
  QToolButton *btn = createToolButton("Recompose", "Recompose", "layout-recompose");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRecomposeClicked(bool)));

  this->addWidget(btn);

  m_recomposeToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createTextToolButton()
{
  QToolButton *btn = createToolButton("Text Tools", "Text Tools", "");

  QMenu* menu = new QMenu(btn);

  QAction* actionTxtDefault = createAction("Default Text Object", m_optionTextDefault, "layout-default-text", "", menu);
  menu->addAction(actionTxtDefault);
  
  QAction* actionTitle = createAction("Title Object", m_optionTitle, "layout-title", "", menu);
  menu->addAction(actionTitle);

  QAction* actionStringGrid = createAction("Text Grid Object", m_optionStringGrid, "layout-grid", "", menu);
  menu->addAction(actionStringGrid);

  QAction* actionImage = createAction("Image Object", m_optionImage, "layout-image", "", menu);
  menu->addAction(actionImage);

  QAction* actionBalloon = createAction("Balloon Object", m_optionBalloon, "layout-ballon", "", menu); 
  menu->addAction(actionBalloon);

  QAction* actionBarCode = createAction("BarCode Object", m_optionBarCode, "layout-barcode", "", menu);
  menu->addAction(actionBarCode);

  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::MenuButtonPopup);
  btn->setDefaultAction(actionTxtDefault);

  connect(btn, SIGNAL(triggered(QAction*)), this, SLOT(onTextToolsTriggered(QAction*)));
  this->addWidget(btn);

  m_textToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignLeftToolButton()
{
  QToolButton *btn = createToolButton("Align Left", "Align Left", "layout-alignleft");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignLeftClicked(bool)));

  this->addWidget(btn);

  m_alignLeftToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignRightToolButton()
{
  QToolButton *btn = createToolButton("Align Right", "Align Right", "layout-alignright");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignRightClicked(bool)));

  this->addWidget(btn);

  m_alignRightToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignTopToolButton()
{
  QToolButton *btn = createToolButton("Align Top", "Align Top", "layout-aligntop");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignTopClicked(bool)));

  this->addWidget(btn);

  m_alignTopToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignBottomToolButton()
{
  QToolButton *btn = createToolButton("Align Bottom", "Align Bottom", "layout-alignbottom");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignBottomClicked(bool)));

  this->addWidget(btn);

  m_alignBottomToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterHorizontalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Horizontal", "Align Center Horizontal", "layout-alignhrzcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterHorizontalClicked(bool)));

  this->addWidget(btn);

  m_alignCenterHorizontalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterVerticalToolButton()
{
  QToolButton *btn = createToolButton("Align Center Vertical", "Align Center Vertical", "layout-alignvrtcenter");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onAlignCenterVerticalClicked(bool)));

  this->addWidget(btn);

  m_alignCenterVerticalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createRemoveObjectToolButton()
{
  QToolButton *btn = createToolButton("Remove Object", "Remove Object", "layout-empty-trash");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onRemoveObjectClicked(bool)));

  this->addWidget(btn);

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
  actionUndo->setObjectName(m_optionUndo.c_str());
  actionUndo->setIcon(QIcon::fromTheme("layout-undo"));
  actionUndo->setToolTip("Undo Action");
  menu->addAction(actionUndo);

  QAction* actionRedo  = undoStack->createRedoAction(menu, tr("&Redo"));
  actionRedo->setShortcuts(QKeySequence::Redo);
  actionRedo->setObjectName(m_optionRedo.c_str());
  actionRedo->setIcon(QIcon::fromTheme("layout-redo"));
  actionRedo->setToolTip("Redo Action");
  menu->addAction(actionRedo);
    
  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::MenuButtonPopup);
  btn->setDefaultAction(actionUndo);

  this->addWidget(btn);

  m_undoToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createDrawMapToolButton()
{
  QToolButton *btn = createToolButton("Redraw Selection Map", "Redraw Selection Map", "layout-draw-map");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onDrawMapClicked(bool)));

  this->addWidget(btn);

  m_drawMapToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createObjectToImageButton()
{
  QToolButton *btn = createToolButton("Object To Image", "Export all selected objects to image", "layout-object-to-image");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onObjectToImageClicked(bool)));

  this->addWidget(btn);

  m_objectToImageButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExitButton()
{
  QToolButton *btn = createToolButton("Exit", "Exit", "layout-close");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onExitClicked(bool)));

  this->addWidget(btn);

  m_exitButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExportToPDFButton()
{
  QToolButton *btn = createToolButton("Export To PDF", "Export to PDF", "layout-pdf");
  btn->setCheckable(false);
  connect(btn, SIGNAL(clicked(bool)), this, SLOT(onExportToPDFClicked(bool)));

  this->addWidget(btn);

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

  if(action->objectName().compare(m_optionMapDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateMap());
  }
  else if(action->objectName().compare(m_optionGridMap.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridMap());
  }
  else if(action->objectName().compare(m_optionGridPlanar.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridPlanar());
  }
  else if(action->objectName().compare(m_optionGridGeodesic.c_str()) == 0)
  {
    changeAction(type->getModeCreateGridGeodesic());
  }
  else if(action->objectName().compare(m_optionLegendDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateLegend());
  }
  else if(action->objectName().compare(m_optionScale.c_str()) == 0)
  {
    changeAction(type->getModeCreateScale());
  }
  else if(action->objectName().compare(m_optionNorth.c_str()) == 0)
  {
    changeAction(type->getModeCreateNorth());
  }
  else if(action->objectName().compare(m_optionMapLocation.c_str()) == 0)
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

  if(action->objectName().compare(m_optionMapPan.c_str()) == 0)
  {
    changeAction(type->getModeMapPan());
  }
  else if(action->objectName().compare(m_optionMapZoomIn.c_str()) == 0)
  {
    changeAction(type->getModeMapZoomIn());
  }
  else if(action->objectName().compare(m_optionMapZoomOut.c_str()) == 0)
  {
    changeAction(type->getModeMapZoomOut());
  }
  else if(action->objectName().compare(m_optionMapSystematicScale.c_str()) == 0)
  {
    changeAction(type->getModeSystematicScale());
  }
  else if(action->objectName().compare(m_optionMapCreateTextGrid.c_str()) == 0)
  {
    changeAction(type->getModeMapCreateTextGrid());
  }
  else if(action->objectName().compare(m_optionMapCreateMapText.c_str()) == 0)
  {
    changeAction(type->getModeMapCreateTextMap());
  }
  else if(action->objectName().compare(m_optionMapCreateLegendChildAsObject.c_str()) == 0)
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

  if(action->objectName().compare(m_optionRectangle.c_str()) == 0)
  {
    changeAction(type->getModeCreateRectangle());
  }
  else if(action->objectName().compare(m_optionArrow.c_str()) == 0)
  {
    changeAction(type->getModeCreateArrow());
  }
  else if(action->objectName().compare(m_optionEllipse.c_str()) == 0)
  {
    changeAction(type->getModeCreateEllipse());
  }
  else if(action->objectName().compare(m_optionPoint.c_str()) == 0)
  {
    changeAction(type->getModeCreatePoint());
  }
  else if (action->objectName().compare(m_optionLine.c_str()) == 0) 
  {
    changeAction(type->getModeCreateLine());
    Context::getInstance().setWait(type->getModeCoordWait());
  }
  else if (action->objectName().compare(m_optionPolygon.c_str()) == 0) 
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

  if(action->objectName().compare(m_optionViewPan.c_str()) == 0)
  {
    changeAction(type->getModePan());
  }
  else if(action->objectName().compare(m_optionViewZoomIn.c_str()) == 0)
  {
    changeAction(type->getModeZoomIn());
  }
  else if(action->objectName().compare(m_optionViewZoomOut.c_str()) == 0)
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

  if(action->objectName().compare(m_optionGroup.c_str()) == 0)
  {
    changeAction(type->getModeGroup());
  }
  else if(action->objectName().compare(m_optionUngroup.c_str()) == 0)
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
  if(action->objectName().compare(m_optionTextDefault.c_str()) == 0)
  {
    changeAction(type->getModeCreateText());
  }
  else if(action->objectName().compare(m_optionStringGrid.c_str()) == 0)
  {
    changeAction(type->getModeCreateTextGrid());
  }
  else if(action->objectName().compare(m_optionTitle.c_str()) == 0)
  {
    changeAction(type->getModeCreateTitle());
  }
  else if(action->objectName().compare(m_optionImage.c_str()) == 0)
  {
    changeAction(type->getModeCreateImage());
  }
  else if(action->objectName().compare(m_optionBalloon.c_str()) == 0)
  {
    changeAction(type->getModeCreateBalloon());
  }
  else if(action->objectName().compare(m_optionBarCode.c_str()) == 0)
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
