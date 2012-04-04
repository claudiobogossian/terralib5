/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file LayerExplorer.cpp

  \brief It can be used to explore the layers belonging to a project in a QTreeView.
 */

// TerraLib
#include "LayerExplorer.h"
#include <QtGui/QContextMenuEvent>

#include "../../../maptools/AbstractLayer.h"
#include "AbstractLayerItem.h"

te::qt::widgets::LayerExplorer::LayerExplorer(QWidget* parent)
  : QTreeView(parent),
    m_previousCurrentIndex(QModelIndex()),
    m_checkBoxWasClicked(false),
    m_prevPressedItemCheckState(Qt::Unchecked)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));
}

te::qt::widgets::LayerExplorer::~LayerExplorer()
{
}

void te::qt::widgets::LayerExplorer::pressedItem(const QModelIndex& index)
{
  if(m_mouseButton == Qt::RightButton)
    emit contextMenuPressed(index, m_mousePos);

  m_checkBoxWasClicked = false;
  m_prevPressedItemCheckState = static_cast<Qt::CheckState>(model()->data(index, Qt::CheckStateRole).toInt());
}

void te::qt::widgets::LayerExplorer::clickedItem(const QModelIndex& index)
{
  if (model()->data(index, Qt::CheckStateRole) != m_prevPressedItemCheckState)
  {
    m_checkBoxWasClicked = true;
    emit checkBoxWasClicked(index);
  }

  if (index == m_previousCurrentIndex)
  {
    if (m_checkBoxWasClicked == false)
      selectionModel()->select(index, QItemSelectionModel::Select);
  }
  else
  {
    if (m_checkBoxWasClicked == true)
    {
      selectionModel()->select(index, QItemSelectionModel::Deselect);
      selectionModel()->setCurrentIndex(m_previousCurrentIndex, QItemSelectionModel::Select);
    }
    else
    {
      selectionModel()->select(m_previousCurrentIndex, QItemSelectionModel::Deselect);
      selectionModel()->select(index, QItemSelectionModel::Select);
      m_previousCurrentIndex = index;
    }
  }
}

void te::qt::widgets::LayerExplorer::updateCurrentIndex(const QModelIndex& dragIndex, const QModelIndex& dropIndex)
{
  setCurrentIndex(dropIndex);
}

//void te::qt::widgets::LayerExplorer::contextMenuEvent(QContextMenuEvent *e)
//{
//  QPoint p = e->pos();
//}

void te::qt::widgets::LayerExplorer::mousePressEvent(QMouseEvent *e)
{
  m_mouseButton = e->button();
  m_mousePos = e->globalPos();
  QTreeView::mousePressEvent(e);
}