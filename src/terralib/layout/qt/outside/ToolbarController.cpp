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
  \file ToolbarController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "../core/Scene.h"
#include "../../core/pattern/singleton/Context.h"
#include "../core/View.h"
#include "ToolbarController.h"
#include "ToolbarOutside.h"

// Qt
#include <QToolButton>
#include <QAction>
#include <QComboBox>

te::layout::ToolbarController::ToolbarController(AbstractOutsideModel* o) :
  AbstractOutsideController(o)
{
  
}

te::layout::ToolbarController::~ToolbarController()
{

}

void te::layout::ToolbarController::onMapTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }
  
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = getScene()->getView();

  if (action->objectName().compare(toolbar->getActionMapDefault().c_str()) == 0)
  {
    viewport->createItem(itemType->getMapItem());
  }
  else if (action->objectName().compare(toolbar->getActionGridMap().c_str()) == 0)
  {
    viewport->createItem(itemType->getGridMapItem());
  }
  else if (action->objectName().compare(toolbar->getActionGridPlanar().c_str()) == 0)
  {
    viewport->createItem(itemType->getGridPlanarItem());
  }
  else if (action->objectName().compare(toolbar->getActionGridGeodesic().c_str()) == 0)
  {
    viewport->createItem(itemType->getGridGeodesicItem());
  }
  else if (action->objectName().compare(toolbar->getActionLegendDefault().c_str()) == 0)
  {
    viewport->createItem(itemType->getLegendItem());
  }
  else if (action->objectName().compare(toolbar->getActionScale().c_str()) == 0)
  {
    viewport->createItem(itemType->getScaleItem());
  }
  else if (action->objectName().compare(toolbar->getActionNorth().c_str()) == 0)
  {
    viewport->createItem(itemType->getNorthItem());
  }
  else if (action->objectName().compare(toolbar->getActionMapLocation().c_str()) == 0)
  {
    viewport->createItem(itemType->getMapLocationItem());
  }
}

void te::layout::ToolbarController::onMapToolsTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if (action->objectName().compare(toolbar->getActionMapPan().c_str()) == 0)
  {
    toolbar->changeAction(type->getModeMapPan());
  }
  else if (action->objectName().compare(toolbar->getActionMapZoomIn().c_str()) == 0)
  {
    toolbar->changeAction(type->getModeMapZoomIn());
  }
  else if (action->objectName().compare(toolbar->getActionMapZoomOut().c_str()) == 0)
  {
    toolbar->changeAction(type->getModeMapZoomOut());
  }
  else if (action->objectName().compare(toolbar->getActionMapSystematicScale().c_str()) == 0)
  {
    toolbar->changeAction(type->getModeSystematicScale());
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateTextGrid().c_str()) == 0)
  {
    //do nothing
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateMapText().c_str()) == 0)
  {
    //do nothing
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateLegendChildAsObject().c_str()) == 0)
  {
    //do nothing
  }
}

void te::layout::ToolbarController::onGeometryTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = getScene()->getView();

  if (action->objectName().compare(toolbar->getActionRectangle().c_str()) == 0)
  {
    viewport->createItem(itemType->getRectangleItem());
  }
  else if (action->objectName().compare(toolbar->getActionArrow().c_str()) == 0)
  {
    viewport->createItem(itemType->getArrowItem());
  }
  else if (action->objectName().compare(toolbar->getActionEllipse().c_str()) == 0)
  {
    viewport->createItem(itemType->getEllipseItem());
  }
  else if (action->objectName().compare(toolbar->getActionPoint().c_str()) == 0)
  {
    viewport->createItem(itemType->getPointItem());
  }
  else if (action->objectName().compare(toolbar->getActionLine().c_str()) == 0)
  {
    viewport->createLineItem();
  }
  else if (action->objectName().compare(toolbar->getActionPolygon().c_str()) == 0)
  {
    viewport->createPolygonItem();
  }
  else if (action->objectName().compare(toolbar->getActionSVG().c_str()) == 0)
  {
    viewport->createItem(itemType->getSVGItem());
  }
}

void te::layout::ToolbarController::onViewAreaTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  View* viewport = getScene()->getView();

  if (action->objectName().compare(toolbar->getActionViewPan().c_str()) == 0)
  {
    viewport->pan();
  }
  else if (action->objectName().compare(toolbar->getActionViewZoomIn().c_str()) == 0)
  {
    viewport->zoomArea();
  }
  else if (action->objectName().compare(toolbar->getActionViewZoomOut().c_str()) == 0)
  {
    viewport->zoomOut();
  }
}

void te::layout::ToolbarController::onArrowCursorClicked(bool checked)
{
  View* viewport = getScene()->getView();
  viewport->arrowCursor();
}

void te::layout::ToolbarController::onItemToolsTriggered(QAction* action)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  View* viewport = getScene()->getView();

  if (action->objectName().compare(toolbar->getActionGroup().c_str()) == 0)
  {    
    viewport->createItemGroup();
  }
  else if (action->objectName().compare(toolbar->getActionUngroup().c_str()) == 0)
  {
    viewport->destroyItemGroup();
  }
}

void te::layout::ToolbarController::onComboZoomActivated()
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QString text = toolbar->getComboBoxZoom()->currentText();
  if (text.isEmpty() == true)
  {
    return;
  }

  QString textCopy = text;
  std::string cText = textCopy.toStdString();

  textCopy.replace(QString("%"), QString(""));

  std::string cText2 = textCopy.toStdString();

  bool converted = false;
  int newZoom = textCopy.toInt(&converted);
  if (converted == false)
  {
    toolbar->getComboBoxZoom()->setEditText("");
    return;
  }

  toolbar->notifyChangedZoom(newZoom);
}

void te::layout::ToolbarController::onZoomChanged(int zoom)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  if (zoom <= 0)
  {
    return;
  }

  QString value = QString::number(zoom) + "%";
  if (toolbar->getComboBoxZoom()->currentText() == value)
  {
    return;
  }

  toolbar->getComboBoxZoom()->setEditText(value);
}

void te::layout::ToolbarController::onBringToFrontClicked(bool checked)
{    
  getScene()->getAlignItems()->bringToFront();
}

void te::layout::ToolbarController::onSendToBackClicked(bool checked)
{
  getScene()->getAlignItems()->sendToBack();
}

void te::layout::ToolbarController::onRecomposeClicked(bool checked)
{
  Scene* sc = getScene();
  if (!sc)
  {
    return;
  }
  ContextObject context = sc->getContext();

  int zoom = context.getZoom();
  onZoomChanged(zoom);
  onComboZoomActivated();

  View* viewport = getScene()->getView();
  viewport->recompose();
}

void te::layout::ToolbarController::onTextToolsTriggered(QAction* action)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = getScene()->getView();

  if (action->objectName().compare(toolbar->getActionTextDefault().c_str()) == 0)
  {
    viewport->createItem(itemType->getTextItem());
  }
  else if (action->objectName().compare(toolbar->getActionStringGrid().c_str()) == 0)
  {
    viewport->createItem(itemType->getTextGridItem());
  }
  else if (action->objectName().compare(toolbar->getActionTitle().c_str()) == 0)
  {
    viewport->createItem(itemType->getTitleItem());
  }
  else if (action->objectName().compare(toolbar->getActionImage().c_str()) == 0)
  {
    viewport->createItem(itemType->getImageItem());
  }
  else if (action->objectName().compare(toolbar->getActionBalloon().c_str()) == 0)
  {
    viewport->createItem(itemType->getBalloonItem());
  }
  else if (action->objectName().compare(toolbar->getActionBarCode().c_str()) == 0)
  {
    viewport->createItem(itemType->getBarCodeItem());
  }
}

void te::layout::ToolbarController::onAlignLeftClicked(bool checked)
{
  getScene()->getAlignItems()->alignLeft();
}

void te::layout::ToolbarController::onAlignRightClicked(bool checked)
{
  getScene()->getAlignItems()->alignRight();
}

void te::layout::ToolbarController::onAlignTopClicked(bool checked)
{
  getScene()->getAlignItems()->alignTop();
}

void te::layout::ToolbarController::onAlignBottomClicked(bool checked)
{
  getScene()->getAlignItems()->alignBottom();
}

void te::layout::ToolbarController::onAlignCenterHorizontalClicked(bool checked)
{  
  getScene()->getAlignItems()->alignCenterHorizontal();
}

void te::layout::ToolbarController::onAlignCenterVerticalClicked(bool checked)
{
  getScene()->getAlignItems()->alignCenterVertical();
}

void te::layout::ToolbarController::onRemoveObjectClicked(bool checked)
{
  getScene()->removeSelectedItems();
}

void te::layout::ToolbarController::onDrawMapClicked(bool checked)
{
  getScene()->redrawSelectionMap();
}

void te::layout::ToolbarController::onObjectToImageClicked(bool checked)
{
  View* viewport = getScene()->getView();
  viewport->exportItemsToImage();
}

void te::layout::ToolbarController::onExitClicked(bool checked)
{
  View* viewport = getScene()->getView();
  viewport->close();
}

void te::layout::ToolbarController::onExportToPDFClicked(bool checked)
{
  View* viewport = getScene()->getView();
  viewport->exportToPDF();
}

te::layout::Scene* te::layout::ToolbarController::getScene()
{
  Scene* sc = 0;
  AbstractScene* abScene = Context::getInstance().getScene();
  if (!abScene)
  {
    return sc;
  }

  sc = dynamic_cast<Scene*>(abScene);
  return sc;
}



