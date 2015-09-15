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
  \file ObjectInspectorOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ObjectInspectorOutside.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../item/MovingItemGroup.h"

//Qt
#include <QGraphicsWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QTreeWidget>

te::layout::ObjectInspectorOutside::ObjectInspectorOutside(AbstractOutsideController* controller)
  : QWidget(0)
  , AbstractOutsideView(controller)
  , m_treeWidget(0)
  , m_isChangingSelection(false)
{
  m_treeWidget = new QTreeWidget(this);

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  te::gm::Envelope box = abstractModel->getBox();
  setBaseSize(box.getWidth(), box.getHeight());
  setVisible(false);
  setWindowTitle("Object Inspector");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_treeWidget);
  
  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  QVBoxLayout* layoutAll = new QVBoxLayout(this);
  layoutAll->setMargin(0);

  layoutAll->addWidget(groupBox);

  setLayout(layoutAll);

  QStringList headerLabels;
  headerLabels.append(tr("Name"));
  headerLabels.append(tr("Type"));

  m_treeWidget->setColumnCount(2);
  m_treeWidget->setHeaderLabels(headerLabels);
  m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(m_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
}

te::layout::ObjectInspectorOutside::~ObjectInspectorOutside()
{

}

void te::layout::ObjectInspectorOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ObjectInspectorOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ObjectInspectorOutside::itemsInspector(QList<QGraphicsItem*> graphicsItems)
{
  m_treeWidget->clear();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  
  foreach(QGraphicsItem* parentItem, graphicsItems) 
  {
    if(parentItem->parentItem() != 0)
    {
      continue;
    }

    MovingItemGroup* moving = dynamic_cast<MovingItemGroup*>(parentItem);
    if (moving)
    {
      continue;
    }

    AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(parentItem);
    if(absParentItem == 0)
    {
      continue;
    }
    if(absParentItem->getController() == 0)
    {
      continue;
    }

    if(absParentItem->getController()->getProperties().getTypeObj() == enumObj->getPaperItem())
    {
      continue;
    }

    const Property& pParentName = absParentItem->getController()->getProperty("name");
    const std::string& parentName = pParentName.getValue().toString();
    std::string parentTypeName = absParentItem->getController()->getProperties().getTypeObj()->getName();

    QStringList parentList;
    parentList.append(parentName.c_str());
    parentList.append(parentTypeName.c_str());

    QTreeWidgetItem* parentTreeItem = new QTreeWidgetItem(parentList);

    foreach(QGraphicsItem *childItem, parentItem->childItems()) 
    {
      MovingItemGroup* moving = dynamic_cast<MovingItemGroup*>(childItem);
      if (moving)
      {
        continue;
      }

      AbstractItemView* absChildItem = dynamic_cast<AbstractItemView*>(childItem);
      if(absChildItem == 0)
      {
        continue;
      }
      if(absChildItem->getController() == 0)
      {
        continue;
      }

      if(absChildItem->getController()->getProperties().getTypeObj() == enumObj->getPaperItem())
      {
        continue;
      }

      const Property& pChildName = absChildItem->getController()->getProperty("name");
      const std::string& childName = pChildName.getValue().toString();
      std::string childTypeName = absChildItem->getController()->getProperties().getTypeObj()->getName();

      QStringList childList;
      childList.append(childName.c_str());
      childList.append(childTypeName.c_str());

      QTreeWidgetItem* childTreeItem = new QTreeWidgetItem(childList);
      parentTreeItem->addChild(childTreeItem);
    }
    m_treeWidget->addTopLevelItem(parentTreeItem);
  }

  m_treeWidget->sortItems(0, Qt::AscendingOrder);
}

void te::layout::ObjectInspectorOutside::onRemoveProperties( std::vector<std::string> names )
{
  for(size_t i = 0; i < names.size(); ++i)
  {
    QString qName(names[i].c_str());
    QList<QTreeWidgetItem*> listItems = m_treeWidget->findItems(qName, Qt::MatchExactly, 0);
    for(int  j = 0; j < listItems.size(); ++j)
    {
      m_treeWidget->removeItemWidget(listItems[j], 0);
    }
  }
}

void te::layout::ObjectInspectorOutside::selectItems( QList<QGraphicsItem*> graphicsItems )
{
  m_isChangingSelection = true;

  m_treeWidget->clearSelection();
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    AbstractItemView* parentItem = dynamic_cast<AbstractItemView*>(item);
    if(parentItem == 0)
    {
      continue;
    }

    const Property& pName = parentItem->getController()->getProperty("name");
    std::string name = pName.getValue().toString();

    QString qName(name.c_str()); 

    QList<QTreeWidgetItem*> treeItems = m_treeWidget->findItems(qName, Qt::MatchExactly, 0);
    foreach(QTreeWidgetItem* treeItem, treeItems) 
    {
      treeItem->setSelected(true);
    }
  }

  m_isChangingSelection = false;
}

void te::layout::ObjectInspectorOutside::itemSelectionChanged()
{
  if(m_isChangingSelection == true)
  {
    return;
  }

  QList<QTreeWidgetItem*> selectedTreeItems = m_treeWidget->selectedItems();

  QList<QGraphicsItem*> selectedGraphicsItem;

  foreach(QTreeWidgetItem* treeItem, selectedTreeItems) 
  {
    QString qTreeItemName = treeItem->text(0);
    std::string treeItemName = qTreeItemName.toStdString();

    foreach(QGraphicsItem* graphicsItem, m_graphicsItems) 
    {
      AbstractItemView* absItemView = dynamic_cast<AbstractItemView*>(graphicsItem);
      if(absItemView == 0)
      {
        continue;
      }

      const Property& pName = absItemView->getController()->getProperty("name");
      std::string name = pName.getValue().toString();

      if(name == treeItemName)
      {
        selectedGraphicsItem.append(graphicsItem);
        break;
      }
    }
  }

  emit selectionChanged(selectedGraphicsItem);
}
