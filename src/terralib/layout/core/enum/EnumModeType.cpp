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
  m_modeMapPan(0),
  m_modeMapZoomIn(0),
  m_modeMapZoomOut(0),
  m_modeCreateMap(0),
  m_modeCreateLegend(0),
  m_modeCreateRectangle(0),
  m_modeCreateText(0),
  m_modeCreateItemGroup(0),
  m_modeCreateScale(0),
  m_modePrinter(0),
  m_modeCreateImage(0),
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
  m_modeUndo(0),
  m_modeRedo(0),
  m_modeCreateLegendChild(0),
  m_modeLegendChildAsObject(0),
  m_inspectorCurrentItemChanged(0),
  m_modeCreateLineItem(0),
  m_modeCreatePolygonItem(0),
  m_modeCreateBalloon(0),
  m_modeCreateBarCode(0),
  m_modeCreateGridMap(0),
  m_modeCreateGridPlanar(0),
  m_modeCreateGridGeodesic(0),
  m_modeCreateNorth(0),
  m_modeCreateMapLocation(0),
  m_modeCreateSVG(0),
  m_modeTextEditorInteraction(0),
  m_modeArrowCursor(0)
{
  init();
}

te::layout::EnumModeType::~EnumModeType()
{
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
  if(m_modeCreateScale)
  {
    delete m_modeCreateScale;
    m_modeCreateScale = 0;
  }
  if(m_modePrinter)
  {
    delete m_modePrinter;
    m_modePrinter = 0;
  }
  if(m_modeCreateImage)
  {
    delete m_modeCreateImage;
    m_modeCreateImage = 0;
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
  if(m_inspectorCurrentItemChanged)
  {
    delete m_inspectorCurrentItemChanged;
    m_inspectorCurrentItemChanged = 0;
  }
  if(m_modeCreateLineItem)
  {
    delete m_modeCreateLineItem;
    m_modeCreateLineItem = 0;
  }
    
  if(m_modeCreatePolygonItem)
  {
    delete m_modeCreatePolygonItem;
    m_modeCreatePolygonItem = 0;
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

  if(m_modeCreateSVG)
  {
    delete m_modeCreateSVG;
    m_modeCreateSVG= 0;
  }

  if(m_modeTextEditorInteraction)
  {
    delete m_modeTextEditorInteraction;
    m_modeTextEditorInteraction = 0;
  }
  if (m_modeArrowCursor)
  {
    delete m_modeArrowCursor;
    m_modeArrowCursor = 0;
  }
}

void te::layout::EnumModeType::init()
{
  m_modeNone = createEnum("None", this, "None");

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

  m_modeCreateScale = createEnum("CreateScale", this, "Create Scale Component");
  m_modeCreateScale->setType(te::layout::EnumCreate);
  
  m_modePrinter = createEnum("Printer", this, "Printer");
  m_modePrinter->setType(te::layout::EnumDialog);

  m_modeCreateImage = createEnum("CreateImage", this, "Create Image Component");
  m_modeCreateImage->setType(te::layout::EnumCreate);

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

  m_modeUndo = createEnum("Undo", this, "Undo");
  m_modeUndo->setType(te::layout::EnumAction);

  m_modeRedo = createEnum("Redo", this, "Redo");
  m_modeRedo->setType(te::layout::EnumAction);

  m_modeCreateLegendChild = createEnum("CreateLegendChild", this, "Create Legend Child Component");
  m_modeCreateLegendChild->setType(te::layout::EnumCreate);

  m_modeLegendChildAsObject = createEnum("LegendChildAsObject", this, "Create Legend Child As Text Component");
  m_modeLegendChildAsObject->setType(te::layout::EnumTool);

  m_inspectorCurrentItemChanged = createEnum("InspectorItemChanged", this, "Inspector Item Changed");
  m_inspectorCurrentItemChanged->setType(te::layout::EnumAction);

  m_modeCreateLineItem = createEnum("Create Line Item", this, "Create Line Item"); 
  m_modeCreateLineItem->setType(te::layout::EnumCreate);

  m_modeCreatePolygonItem = createEnum("Create Polygon Item", this, "Create Polygon Item"); 
  m_modeCreatePolygonItem->setType(te::layout::EnumCreate);

  m_modeCreateBalloon = createEnum("Create Balloon", this, "Create Balloon"); 
  m_modeCreateBalloon->setType(te::layout::EnumCreate);

  m_modeCreateBarCode = createEnum("Create BarCode", this, "Create BarCode");
  m_modeCreateBarCode->setType(te::layout::EnumCreate);

  m_modeCreateGridMap = createEnum("Create GridMap", this, "Create GridMap");
  m_modeCreateGridMap->setType(te::layout::EnumCreate);

  m_modeCreateGridPlanar = createEnum("Create Grid Planar", this, "Create Grid Planar");
  m_modeCreateGridPlanar->setType(te::layout::EnumCreate);

  m_modeCreateGridGeodesic = createEnum("Create Grid Geodesic", this, "Create Grid Geodesic");
  m_modeCreateGridGeodesic->setType(te::layout::EnumCreate);

  m_modeCreateNorth = createEnum("Create North", this, "Create North");
  m_modeCreateNorth->setType(te::layout::EnumCreate);

  m_modeCreateMapLocation = createEnum("Create Map Location", this, "Create Map Location");
  m_modeCreateMapLocation->setType(te::layout::EnumCreate);

  m_modeCreateSVG = createEnum("Create SVG", this, "Create SVG");
  m_modeCreateSVG->setType(te::layout::EnumCreate);

  m_modeTextEditorInteraction = createEnum("Text Editor Interaction", this, "Text Editor Interaction");
  m_modeTextEditorInteraction->setType(te::layout::EnumAction);

  m_modeArrowCursor = createEnum("Arrow Cursor", this, "Arrow Cursor");
  m_modeArrowCursor->setType(te::layout::EnumAction);
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

te::layout::EnumType* te::layout::EnumModeType::getModeCreateScale() const
{
  return m_modeCreateScale;
}

te::layout::EnumType* te::layout::EnumModeType::getModePrinter() const
{
  return m_modePrinter;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateImage() const
{
  return m_modeCreateImage;
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

te::layout::EnumType* te::layout::EnumModeType::getModeCreateLegendChild() const
{
  return m_modeCreateLegendChild;
}

te::layout::EnumType* te::layout::EnumModeType::getModeLegendChildAsObject() const
{
  return m_modeLegendChildAsObject;
}

te::layout::EnumType* te::layout::EnumModeType::getModeInspectorCurrentItemChanged() const
{
  return m_inspectorCurrentItemChanged;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateLineItem() const
{
  return m_modeCreateLineItem;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreatePolygonItem() const
{
  return m_modeCreatePolygonItem;
}

te::layout::EnumType* te::layout::EnumModeType::getModeCreateBalloon() const
{
  return m_modeCreateBalloon;
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

te::layout::EnumType* te::layout::EnumModeType::getModeCreateSVG() const
{
  return m_modeCreateSVG;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSVGView() const
{
  return m_modeSVGView;
}

te::layout::EnumType* te::layout::EnumModeType::getModeTextEditorInteraction() const
{
  return m_modeTextEditorInteraction;
}

te::layout::EnumType* te::layout::EnumModeType::getModeArrowCursor() const
{
  return m_modeArrowCursor;
}

