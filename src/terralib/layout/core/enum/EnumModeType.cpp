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
  \file EnumGridStyleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumModeType.h"

te::layout::EnumModeType::EnumModeType() :
  m_modeNone(0),
  m_modeSelectByBox(0),
  m_modeSelect(0),
  m_modeArrowCursor(0),
  m_modePan(0),
  m_modeZoomIn(0),
  m_modeZoomOut(0),
  m_modeMapPan(0),
  m_modeMapZoomIn(0),
  m_modeMapZoomOut(0),
  m_modeCreateMap(0),
  m_modeCreateLegend(0),
  m_modeCreateRectangle(0),
  m_modeCreateText(0),
  m_modeCreateItemGroup(0),
  m_modeUnitsMetricsChange(0),
  m_modeExportPropsJSON(0),
  m_modeImportJSONProps(0),
  m_modeNewTemplate(0),
  m_modeCreateScale(0),
  m_modeGroup(0),
  m_modeUngroup(0),
  m_modeActiveLinesIntersectionMouse(0),
  m_modeOffLinesIntersectionMouse(0),
  m_modePrinter(0),
  m_modeExit(0),
  m_modeSceneZoom(0),
  m_modeBringToFront(0),
  m_modeSendToBack(0),
  m_modeRecompose(0),
  m_modeCreateImage(0),
  m_modePageConfig(0),
  m_modeSystematicScale(0),
  m_modeSVGView(0),
  m_modeCreatePoint(0),
  m_modeCreateArrow(0),
  m_modeCreateEllipse(0),
  m_modeCreateTitle(0),
  m_modeCreateTextGrid(0),
  m_modeWidgetView(0),
  m_modeWidgetInspector(0),
  m_modeWidgetProperties(0),
  m_modeWidgetToolbar(0),
  m_modeMapCreateTextGrid(0),
  m_modeMapCreateTextMap(0),
  m_modeAlignCenterHorizontal(0),
  m_modeAlignCenterVertical(0),
  m_modeAlignLeft(0),
  m_modeAlignRight(0),
  m_modeAlignTop(0),
  m_modeAlignBottom(0),
  m_modeRemoveObject(0),
  m_modeUndo(0),
  m_modeRedo(0),
  m_modeDrawSelectionMap(0),
  m_modeCreateLegendChild(0),
  m_modeLegendChildAsObject(0),
  m_modelObjectToImage(0),
  m_inspectorCurrentItemChanged(0),
  m_modelExportToPDF(0),
  m_modeCreateLine(0),
  m_modeCreatePolygon(0),
  m_modeCreateBalloon(0),
  m_modeCreateBarCode(0),
  m_modeCreateGridMap(0),
  m_coordWait(0),
  m_modeCreateGridPlanar(0),
  m_modeCreateGridGeodesic(0),
  m_modeCreateNorth(0),
  m_modeCreateMapLocation(0),
	m_modeCreateStar(0),
  m_modeCreateSVG(0)
{
  init();
}

te::layout::EnumModeType::~EnumModeType()
{
  if(m_modeSelect)
  {
    delete m_modeSelect;
    m_modeSelect = 0;
  }
  if(m_modeArrowCursor)
  {
    delete m_modeArrowCursor;
    m_modeArrowCursor = 0;
  }
  if(m_modePan)
  {
    delete m_modePan;
    m_modePan = 0;
  }
  if(m_modeZoomIn)
  {
    delete m_modeZoomIn;
    m_modeZoomIn = 0;
  }
  if(m_modeZoomOut)
  {
    delete m_modeZoomOut;
    m_modeZoomOut = 0;
  }
  if(m_modeMapPan)
  {
    delete m_modeMapPan;
    m_modeMapPan = 0;
  }
  if(m_modeMapZoomIn)
  {
    delete m_modeMapZoomIn;
    m_modeMapZoomIn = 0;
  }
  if(m_modeMapZoomOut)
  {
    delete m_modeMapZoomOut;
    m_modeMapZoomOut = 0;
  }
  if(m_modeCreateMap)
  {
    delete m_modeCreateMap;
    m_modeCreateMap = 0;
  }
  if(m_modeCreateLegend)
  {
    delete m_modeCreateLegend;
    m_modeCreateLegend = 0;
  }
  if(m_modeCreateRectangle)
  {
    delete m_modeCreateRectangle;
    m_modeCreateRectangle = 0;
  }
  if(m_modeCreateText)
  {
    delete m_modeCreateText;
    m_modeCreateText = 0;
  }
  if(m_modeCreateItemGroup)
  {
    delete m_modeCreateItemGroup;
    m_modeCreateItemGroup = 0;
  }
  if(m_modeUnitsMetricsChange)
  {
    delete m_modeUnitsMetricsChange;
    m_modeUnitsMetricsChange = 0;
  }
  if(m_modeExportPropsJSON)
  {
    delete m_modeExportPropsJSON;
    m_modeExportPropsJSON = 0;
  }
  if(m_modeImportJSONProps)
  {
    delete m_modeImportJSONProps;
    m_modeImportJSONProps = 0;
  }
  if(m_modeNewTemplate)
  {
    delete m_modeNewTemplate;
    m_modeNewTemplate = 0;
  }
  if(m_modeCreateScale)
  {
    delete m_modeCreateScale;
    m_modeCreateScale = 0;
  }
  if(m_modeGroup)
  {
    delete m_modeGroup;
    m_modeGroup = 0;
  }
  if(m_modeUngroup)
  {
    delete m_modeUngroup;
    m_modeUngroup = 0;
  }
  if(m_modeActiveLinesIntersectionMouse)
  {
    delete m_modeActiveLinesIntersectionMouse;
    m_modeActiveLinesIntersectionMouse = 0;
  }
  if(m_modeOffLinesIntersectionMouse)
  {
    delete m_modeOffLinesIntersectionMouse;
    m_modeOffLinesIntersectionMouse = 0;
  }
  if(m_modePrinter)
  {
    delete m_modePrinter;
    m_modePrinter = 0;
  }
  if(m_modeExit)
  {
    delete m_modeExit;
    m_modeExit = 0;
  }
  if(m_modeSceneZoom)
  {
    delete m_modeSceneZoom;
    m_modeSceneZoom = 0;
  }
  if(m_modeBringToFront)
  {
    delete m_modeBringToFront;
    m_modeBringToFront = 0;
  }
  if(m_modeSendToBack)
  {
    delete m_modeSendToBack;
    m_modeSendToBack = 0;
  }
  if(m_modeRecompose)
  {
    delete m_modeRecompose;
    m_modeRecompose = 0;
  }
  if(m_modeCreateImage)
  {
    delete m_modeCreateImage;
    m_modeCreateImage = 0;
  }
  if(m_modePageConfig)
  {
    delete m_modePageConfig;
    m_modePageConfig = 0;
  }
  if(m_modeSystematicScale)
  {
    delete m_modeSystematicScale;
    m_modeSystematicScale = 0;
  }
  if(m_modeSVGView)
  {
    delete m_modeSVGView;
    m_modeSVGView = 0;
  }
  if(m_modeCreatePoint)
  {
    delete m_modeCreatePoint;
    m_modeCreatePoint = 0;
  }
  if(m_modeCreateArrow)
  {
    delete m_modeCreateArrow;
    m_modeCreateArrow = 0;
  }
  if(m_modeCreateEllipse)
  {
    delete m_modeCreateEllipse;
    m_modeCreateEllipse = 0;
  }
  if(m_modeCreateTitle)
  {
    delete m_modeCreateTitle;
    m_modeCreateTitle = 0;
  }
  if(m_modeCreateTextGrid)
  {
    delete m_modeCreateTextGrid;
    m_modeCreateTextGrid = 0;
  }
  if(m_modeWidgetView)
  {
    delete m_modeWidgetView;
    m_modeWidgetView = 0;
  }
  if(m_modeWidgetInspector)
  {
    delete m_modeWidgetInspector;
    m_modeWidgetInspector = 0;
  }
  if(m_modeWidgetProperties)
  {
    delete m_modeWidgetProperties;
    m_modeWidgetProperties = 0;
  }
  if(m_modeWidgetToolbar)
  {
    delete m_modeWidgetToolbar;
    m_modeWidgetToolbar = 0;
  }
  if(m_modeMapCreateTextGrid)
  {
    delete m_modeMapCreateTextGrid;
    m_modeMapCreateTextGrid = 0;
  }
  if(m_modeMapCreateTextMap)
  {
    delete m_modeMapCreateTextMap;
    m_modeMapCreateTextMap = 0;
  }
  if(m_modeAlignCenterHorizontal)
  {
    delete m_modeAlignCenterHorizontal;
    m_modeAlignCenterHorizontal = 0;
  }
  if(m_modeAlignCenterVertical)
  {
    delete m_modeAlignCenterVertical;
    m_modeAlignCenterVertical = 0;
  }
  if(m_modeAlignLeft)
  {
    delete m_modeAlignLeft;
    m_modeAlignLeft = 0;
  }
  if(m_modeAlignRight)
  {
    delete m_modeAlignRight;
    m_modeAlignRight = 0;
  }
  if(m_modeAlignTop)
  {
    delete m_modeAlignTop;
    m_modeAlignTop = 0;
  }
  if(m_modeAlignBottom)
  {
    delete m_modeAlignBottom;
    m_modeAlignBottom = 0;
  }
  if(m_modeRemoveObject)
  {
    delete m_modeRemoveObject;
    m_modeRemoveObject = 0;
  }
  if(m_modeNone)
  {
    delete m_modeRemoveObject;
    m_modeRemoveObject = 0;
  }
  if(m_modeUndo)
  {
    delete m_modeUndo;
    m_modeUndo = 0;
  }
  if(m_modeRedo)
  {
    delete m_modeRedo;
    m_modeRedo = 0;
  }
  if(m_modeDrawSelectionMap)
  {
    delete m_modeDrawSelectionMap;
    m_modeDrawSelectionMap = 0;
  }
  if(m_modeCreateLegendChild)
  {
    delete m_modeCreateLegendChild;
    m_modeCreateLegendChild = 0;
  }
  if(m_modeLegendChildAsObject)
  {
    delete m_modeLegendChildAsObject;
    m_modeLegendChildAsObject = 0;
  }
  if(m_modelObjectToImage)
  {
    delete m_modelObjectToImage;
    m_modelObjectToImage = 0;
  }
  if(m_inspectorCurrentItemChanged)
  {
    delete m_inspectorCurrentItemChanged;
    m_inspectorCurrentItemChanged = 0;
  }
  if(m_modelExportToPDF)
  {
    delete m_modelExportToPDF;
    m_modelExportToPDF = 0;
  }

  if(m_modeCreateLine)
  {
    delete m_modeCreateLine;
    m_modeCreateLine = 0;
  }
    
  if(m_modeCreatePolygon)
  {
    delete m_modeCreatePolygon;
    m_modeCreatePolygon = 0;
  }

  if(m_modeCreateBalloon)
  {
    delete m_modeCreateBalloon;
    m_modeCreateBalloon = 0;
  }

  if(m_modeCreateBarCode)
  {
    delete m_modeCreateBarCode;
    m_modeCreateBarCode = 0;
  }

  if(m_modeCreateGridMap)
  {
    delete m_modeCreateGridMap;
    m_modeCreateGridMap = 0;
  }
    
  if(m_coordWait)
  {
    delete m_coordWait;
    m_coordWait = 0;
  }

  if(m_modeCreateGridPlanar)
  {
    delete m_modeCreateGridPlanar;
    m_modeCreateGridPlanar = 0;
  }

  if(m_modeCreateGridGeodesic)
  {
    delete m_modeCreateGridGeodesic;
    m_modeCreateGridGeodesic = 0;
  }

	if(m_modeCreateStar)
	{
		delete m_modeCreateStar;
		m_modeCreateStar= 0;
	}

  if(m_modeCreateSVG)
  {
    delete m_modeCreateSVG;
    m_modeCreateSVG= 0;
  }
}

void te::layout::EnumModeType::init()
{
  m_modeNone = createEnum("None", this, "None");

  m_modeSelectByBox = createEnum("SelectByBox", this, "Select By Box");
  m_modeSelectByBox->setType(te::layout::EnumTool);

  m_modeSelect = createEnum("Select", this, "Select");
  m_modeSelect->setType(te::layout::EnumAction);

  m_modeArrowCursor = createEnum("ArrowCursor", this, "Arrow Cursor");
  m_modeArrowCursor->setType(te::layout::EnumAction);
  
  m_modePan = createEnum("Pan", this, "Pan");
  m_modePan->setType(te::layout::EnumTool);

  m_modeZoomIn = createEnum("ZoomIn", this, "Zoom In");
  m_modeZoomIn->setType(te::layout::EnumTool);

  m_modeZoomOut = createEnum("ZoomOut", this, "Zoom Out");
  m_modeZoomOut->setType(te::layout::EnumTool);

  m_modeMapPan = createEnum("MapPan", this, "Map Pan");
  m_modeMapPan->setType(te::layout::EnumTool);

  m_modeMapZoomIn = createEnum("MapZoomIn", this, "Map Zoom In");
  m_modeMapZoomIn->setType(te::layout::EnumTool);

  m_modeMapZoomOut = createEnum("MapZoomOut", this, "Map Zoom Out");
  m_modeMapZoomOut->setType(te::layout::EnumTool);

  m_modeCreateMap = createEnum("CreateMap", this, "Create Map Component");
  m_modeCreateMap->setType(te::layout::EnumCreate);
  
  m_modeCreateLegend = createEnum("CreateLegend", this, "Create Legend Component");
  m_modeCreateLegend->setType(te::layout::EnumCreate);

  m_modeCreateRectangle = createEnum("CreateRectangle", this, "Create Rectangle Component");
  m_modeCreateRectangle->setType(te::layout::EnumCreate);

  m_modeCreateText = createEnum("CreateText", this, "Create Text Component");
  m_modeCreateText->setType(te::layout::EnumCreate);

  m_modeCreateItemGroup = createEnum("CreateItemGroup", this, "Create Item Group Component");
  m_modeCreateItemGroup->setType(te::layout::EnumCreate);

  m_modeUnitsMetricsChange = createEnum("UnitsMetricsChange", this, "Unit Metric Change");
  m_modeUnitsMetricsChange->setType(te::layout::EnumAction);

  m_modeExportPropsJSON = createEnum("ExportPropsJSON", this, "Export Properties To JSON");
  m_modeExportPropsJSON->setType(te::layout::EnumDialog);

  m_modeImportJSONProps = createEnum("ImportJSONProps", this, "Import JSON To Properties");
  m_modeImportJSONProps->setType(te::layout::EnumDialog);

  m_modeNewTemplate = createEnum("NewTemplate", this, "New Template");
  m_modeNewTemplate->setType(te::layout::EnumAction);

  m_modeCreateScale = createEnum("CreateScale", this, "Create Scale Component");
  m_modeCreateScale->setType(te::layout::EnumCreate);

  m_modeGroup = createEnum("Group", this, "Group");
  m_modeGroup->setType(te::layout::EnumAction);

  m_modeUngroup = createEnum("Ungroup", this, "Ungroup");
  m_modeUngroup->setType(te::layout::EnumAction);

  m_modeActiveLinesIntersectionMouse = createEnum("ActiveLinesIntersectionMouse", this, "Active Lines That Intersection Mouse");
  m_modeActiveLinesIntersectionMouse->setType(te::layout::EnumTool);

  m_modeOffLinesIntersectionMouse = createEnum("OffLinesIntersectionMouse", this, "Off Lines That Intersection Mouse");
  m_modeOffLinesIntersectionMouse->setType(te::layout::EnumTool);

  m_modePrinter = createEnum("Printer", this, "Printer");
  m_modePrinter->setType(te::layout::EnumDialog);

  m_modeExit = createEnum("Exit", this, "Exit");
  m_modeExit->setType(te::layout::EnumAction);

  m_modeSceneZoom = createEnum("SceneZoom", this, "Scene Zoom");
  m_modeSceneZoom->setType(te::layout::EnumAction);

  m_modeBringToFront = createEnum("BringToFront", this, "Bring To Front");
  m_modeBringToFront->setType(te::layout::EnumAction);

  m_modeSendToBack = createEnum("SendToBack", this, "Send To Back");
  m_modeSendToBack->setType(te::layout::EnumAction);

  m_modeRecompose = createEnum("Recompose", this, "Recompose");
  m_modeRecompose->setType(te::layout::EnumAction);

  m_modeCreateImage = createEnum("CreateImage", this, "Create Image Component");
  m_modeCreateImage->setType(te::layout::EnumCreate);

  m_modePageConfig = createEnum("PageConfig", this, "Open Page Config");
  m_modePageConfig->setType(te::layout::EnumDialog);

  m_modeSystematicScale = createEnum("SystematicScale", this, "Systematic Scale");
  m_modeSystematicScale->setType(te::layout::EnumDialog);

  m_modeSVGView = createEnum("SVGView", this, "SVG View");
  m_modeSVGView->setType(te::layout::EnumDialog);

  m_modeCreatePoint = createEnum("CreatePoint", this, "Create Point Component");
  m_modeCreatePoint->setType(te::layout::EnumCreate);

  m_modeCreateArrow = createEnum("CreateArrow", this, "Create Arrow Component");
  m_modeCreateArrow->setType(te::layout::EnumCreate);

  m_modeCreateEllipse = createEnum("CreateEllipse", this, "Create Ellipse Component");
  m_modeCreateEllipse->setType(te::layout::EnumCreate);

  m_modeCreateTitle = createEnum("CreateTitle", this, "Create Title Component");
  m_modeCreateTitle->setType(te::layout::EnumCreate);

  m_modeCreateTextGrid = createEnum("CreateTextGrid", this, "Create Text Grid");
  m_modeCreateTextGrid->setType(te::layout::EnumCreate);
  
  m_modeWidgetView = createEnum("WidgetView", this, "Widget View");
  m_modeWidgetView->setType(te::layout::EnumDialog);

  m_modeWidgetInspector = createEnum("WidgetInspector", this, "Widget Inspector");
  m_modeWidgetInspector->setType(te::layout::EnumDialog);

  m_modeWidgetProperties = createEnum("WidgetProperties", this, "Widget Properties");
  m_modeWidgetProperties->setType(te::layout::EnumDialog);

  m_modeWidgetToolbar = createEnum("WidgetToolbar", this, "Widget Toolbar");
  m_enums.push_back(m_modeWidgetToolbar);

  m_modeMapCreateTextGrid = createEnum("CreateTextGrid", this, "Create TextGrid Component");
  m_modeMapCreateTextGrid->setType(te::layout::EnumCreate);

  m_modeMapCreateTextMap = createEnum("CreateTextMap", this, "Create Text of a Map");
  m_modeMapCreateTextMap->setType(te::layout::EnumCreate);

  m_modeAlignCenterHorizontal = createEnum("AlignCenterHorizontal", this, "Align center horizontal");
  m_modeAlignCenterHorizontal->setType(te::layout::EnumAction);

  m_modeAlignCenterVertical = createEnum("AlignCenterVertical", this, "Align center vertical");
  m_modeAlignCenterVertical->setType(te::layout::EnumAction);

  m_modeAlignLeft = createEnum("AlignLeft", this, "Align Left");
  m_modeAlignLeft->setType(te::layout::EnumAction);

  m_modeAlignRight = createEnum("AlignRight", this, "Align Right");
  m_modeAlignRight->setType(te::layout::EnumAction);

  m_modeAlignTop = createEnum("AlignTop", this, "Align Top");
  m_modeAlignTop->setType(te::layout::EnumAction);

  m_modeAlignBottom = createEnum("AlignBottom", this, "Align Bottom");
  m_modeAlignBottom->setType(te::layout::EnumAction);

  m_modeRemoveObject = createEnum("RemoveObject", this, "Remove Component");
  m_modeRemoveObject->setType(te::layout::EnumAction);

  m_modeUndo = createEnum("Undo", this, "Undo");
  m_modeUndo->setType(te::layout::EnumAction);

  m_modeRedo = createEnum("Redo", this, "Redo");
  m_modeRedo->setType(te::layout::EnumAction);

  m_modeDrawSelectionMap = createEnum("DrawSelectionMap", this, "Draw Selection Map");
  m_modeDrawSelectionMap->setType(te::layout::EnumAction);

  m_modeCreateLegendChild = createEnum("CreateLegendChild", this, "Create Legend Child Component");
  m_modeCreateLegendChild->setType(te::layout::EnumCreate);

  m_modeLegendChildAsObject = createEnum("LegendChildAsObject", this, "Create Legend Child As Text Component");
  m_modeLegendChildAsObject->setType(te::layout::EnumTool);

  m_modelObjectToImage = createEnum("ObjectToImage", this, "Selected Component To Image");
  m_modelObjectToImage->setType(te::layout::EnumDialog);

  m_inspectorCurrentItemChanged = createEnum("InspectorItemChanged", this, "Inspector Item Changed");
  m_inspectorCurrentItemChanged->setType(te::layout::EnumAction);

  m_modelExportToPDF = createEnum("ExporttoPDF", this, "Export To PDF");
  m_modelExportToPDF->setType(te::layout::EnumDialog);

  m_modeCreateLine = createEnum("Create Line", this, "Create Line"); 
  m_modeCreateLine->setType(te::layout::EnumCreate);

  m_modeCreatePolygon = createEnum("Create Polygon", this, "Create Polygon"); 
  m_modeCreatePolygon->setType(te::layout::EnumCreate);

  m_modeCreateBalloon = createEnum("Create Balloon", this, "Create Balloon"); 
  m_modeCreateBalloon->setType(te::layout::EnumCreate);

  m_modeCreateBarCode = createEnum("Create BarCode", this, "Create BarCode");
  m_modeCreateBarCode->setType(te::layout::EnumCreate);

  m_modeCreateGridMap = createEnum("Create GridMap", this, "Create GridMap");
  m_modeCreateGridMap->setType(te::layout::EnumCreate);

  m_coordWait = createEnum("Wait View", this, "Wait View"); 
  m_coordWait->setType(te::layout::EnumAction);

  m_modeCreateGridPlanar = createEnum("Create Grid Planar", this, "Create Grid Planar");
  m_modeCreateGridPlanar->setType(te::layout::EnumCreate);

  m_modeCreateGridGeodesic = createEnum("Create Grid Geodesic", this, "Create Grid Geodesic");
  m_modeCreateGridGeodesic->setType(te::layout::EnumCreate);

  m_modeCreateNorth = createEnum("Create North", this, "Create North");
  m_modeCreateNorth->setType(te::layout::EnumCreate);

  m_modeCreateMapLocation = createEnum("Create Map Location", this, "Create Map Location");
  m_modeCreateMapLocation->setType(te::layout::EnumCreate);

	m_modeCreateStar = createEnum("Create Star", this, "Create Star");
	m_modeCreateStar->setType(te::layout::EnumCreate);

  m_modeCreateSVG = createEnum("Create SVG", this, "Create SVG");
  m_modeCreateSVG->setType(te::layout::EnumCreate);
}

te::layout::EnumType* te::layout::EnumModeType::getModeSelectByBox() const
{
  return m_modeSelectByBox;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSelect() const
{
  return m_modeSelect;
}

te::layout::EnumType* te::layout::EnumModeType::getModeArrowCursor() const
{
  return m_modeArrowCursor;
}

te::layout::EnumType* te::layout::EnumModeType::getModePan() const
{
  return m_modePan;
}

te::layout::EnumType* te::layout::EnumModeType::getModeZoomIn() const
{
  return m_modeZoomIn;
}

te::layout::EnumType* te::layout::EnumModeType::getModeZoomOut() const
{
  return m_modeZoomOut;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapPan() const
{
  return m_modeMapPan;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapZoomIn() const
{
  return m_modeMapZoomIn;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapZoomOut() const
{
  return m_modeMapZoomOut;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateMap() const
{
  return m_modeCreateMap;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateLegend() const
{
  return m_modeCreateLegend;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateRectangle() const
{
  return m_modeCreateRectangle;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateText() const
{
  return m_modeCreateText;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateItemGroup() const
{
  return m_modeCreateItemGroup;
}

te::layout::EnumType* te::layout::EnumModeType::getModeUnitsMetricsChange() const
{
  return m_modeUnitsMetricsChange;
}

te::layout::EnumType* te::layout::EnumModeType::getModeExportPropsJSON() const
{
  return m_modeExportPropsJSON;
}

te::layout::EnumType* te::layout::EnumModeType::getModeImportJSONProps() const
{
  return m_modeImportJSONProps;
}

te::layout::EnumType* te::layout::EnumModeType::getModeNewTemplate() const
{
  return m_modeNewTemplate;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateScale() const
{
  return m_modeCreateScale;
}

te::layout::EnumType* te::layout::EnumModeType::getModeGroup() const
{
  return m_modeGroup;
}

te::layout::EnumType* te::layout::EnumModeType::getModeUngroup() const
{
  return m_modeUngroup;
}

te::layout::EnumType* te::layout::EnumModeType::getModeActiveLinesIntersectionMouse() const
{
  return m_modeActiveLinesIntersectionMouse;
}

te::layout::EnumType* te::layout::EnumModeType::getModeOffLinesIntersectionMouse() const
{
  return m_modeOffLinesIntersectionMouse;
}

te::layout::EnumType* te::layout::EnumModeType::getModePrinter() const
{
  return m_modePrinter;
}

te::layout::EnumType* te::layout::EnumModeType::getModeExit() const
{
  return m_modeExit;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSceneZoom() const
{
  return m_modeSceneZoom;
}

te::layout::EnumType* te::layout::EnumModeType::getModeBringToFront() const
{
  return m_modeBringToFront;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSendToBack() const
{
  return m_modeSendToBack;
}

te::layout::EnumType* te::layout::EnumModeType::getModeRecompose() const
{
  return m_modeRecompose;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateImage() const
{
  return m_modeCreateImage;
}

te::layout::EnumType* te::layout::EnumModeType::getModePageConfig() const
{
  return m_modePageConfig;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSystematicScale() const
{
  return m_modeSystematicScale;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreatePoint() const
{
  return m_modeCreatePoint;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateArrow() const
{
  return m_modeCreateArrow;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateEllipse() const
{
  return m_modeCreateEllipse;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateTitle() const
{
  return m_modeCreateTitle;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateTextGrid() const
{
  return m_modeCreateTextGrid;
}

te::layout::EnumType* te::layout::EnumModeType::getModeWidgetView() const
{
  return m_modeWidgetView;
}

te::layout::EnumType* te::layout::EnumModeType::getModeWidgetInspector() const
{
  return m_modeWidgetInspector;
}

te::layout::EnumType* te::layout::EnumModeType::getModeWidgetProperties() const
{
  return m_modeWidgetProperties;
}

te::layout::EnumType* te::layout::EnumModeType::getModeWidgetToolbar() const
{
  return m_modeWidgetToolbar;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapCreateTextGrid() const
{
  return m_modeMapCreateTextGrid;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapCreateTextMap() const
{
  return m_modeMapCreateTextMap;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignCenterHorizontal() const
{
  return m_modeAlignCenterHorizontal;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignCenterVertical() const
{
  return m_modeAlignCenterVertical;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignLeft() const
{
  return m_modeAlignLeft;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignRight() const
{
  return m_modeAlignRight;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignTop() const
{
  return m_modeAlignTop;
}

te::layout::EnumType* te::layout::EnumModeType::getModeAlignBottom() const
{
  return m_modeAlignBottom;
}

te::layout::EnumType* te::layout::EnumModeType::getModeRemoveObject() const
{
  return m_modeRemoveObject;
}

te::layout::EnumType* te::layout::EnumModeType::getModeNone() const
{
  return m_modeNone;
}

te::layout::EnumType* te::layout::EnumModeType::getModeUndo() const
{
  return m_modeUndo;
}

te::layout::EnumType* te::layout::EnumModeType::getModeRedo() const
{
  return m_modeRedo;
}

te::layout::EnumType* te::layout::EnumModeType::getModeDrawSelectionMap() const
{
  return m_modeDrawSelectionMap;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateLegendChild() const
{
  return m_modeCreateLegendChild;
}

te::layout::EnumType* te::layout::EnumModeType::getModeLegendChildAsObject() const
{
  return m_modeLegendChildAsObject;
}

te::layout::EnumType* te::layout::EnumModeType::getModeObjectToImage() const
{
  return m_modelObjectToImage;
}

te::layout::EnumType* te::layout::EnumModeType::getModeInspectorCurrentItemChanged() const
{
  return m_inspectorCurrentItemChanged;
}

te::layout::EnumType* te::layout::EnumModeType::getModeExportToPDF() const
{
  return m_modelExportToPDF;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateLine() const
{
  return m_modeCreateLine;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreatePolygon() const
{
  return m_modeCreatePolygon;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateBalloon() const
{
  return m_modeCreateBalloon;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCoordWait() const
{
  return m_coordWait;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateBarCode() const
{
  return m_modeCreateBarCode;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateGridMap() const
{
  return m_modeCreateGridMap;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateGridPlanar() const
{
  return m_modeCreateGridPlanar;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateGridGeodesic() const
{
  return m_modeCreateGridGeodesic;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateNorth() const
{
  return m_modeCreateNorth;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateMapLocation() const
{
  return m_modeCreateMapLocation;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateStar() const
{
	return m_modeCreateStar;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateSVG() const
{
  return m_modeCreateSVG;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSVGView() const
{
  return m_modeSVGView;
}

