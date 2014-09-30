/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/utils/CompositionModeMenuWidget.cpp

  \brief This file defines a class for a CompositionModeMenuWidget.
*/

// TerraLib
#include "../../../../maptools/Enums.h"
#include "CompositionModeDialog.h"
#include "CompositionModeMenuWidget.h"

// Qt
#include <QActionGroup>

te::qt::widgets::CompositionModeMenuWidget::CompositionModeMenuWidget(QWidget* parent, Qt::WindowFlags f):QWidget(parent, f)
{
  //create menu
  m_menu = new QMenu(tr("Composition Mode"), this);

  //create actions
  m_sourceOverAction = new QAction(tr("Default"), this);
  m_sourceOverAction->setCheckable(true);
  m_plusAction = new QAction(tr("Plus"), this);
  m_plusAction->setCheckable(true);
  m_compositionModeAction = new QAction(tr("Advanced..."), this);
  m_compositionModeAction->setCheckable(true);

  //add actions to menu
  m_menu->addAction(m_sourceOverAction);
  m_menu->addAction(m_plusAction);
  m_menu->addAction(m_compositionModeAction);

  //create action group
  QActionGroup* actGroup = new QActionGroup(this);
  actGroup->addAction(m_sourceOverAction);
  actGroup->addAction(m_plusAction);
  actGroup->addAction(m_compositionModeAction);

  //connections
  connect(m_sourceOverAction, SIGNAL(triggered()), this, SLOT(onSourceOverClicked()));
  connect(m_plusAction, SIGNAL(triggered()), this, SLOT(onPlusClicked()));
  connect(m_compositionModeAction, SIGNAL(triggered()), this, SLOT(onCompositionModeClicked()));
}

te::qt::widgets::CompositionModeMenuWidget::~CompositionModeMenuWidget()
{
}

void te::qt::widgets::CompositionModeMenuWidget::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  if(m_layer->getCompositionMode() == te::map::SourceOver)
  {
    m_sourceOverAction->setChecked(true);
  }
  else if(m_layer->getCompositionMode() == te::map::Plus)
  {
    m_plusAction->setChecked(true);
  }
  else
  {
    m_compositionModeAction->setChecked(true);
  }
}

QMenu* te::qt::widgets::CompositionModeMenuWidget::getMenu()
{
  return m_menu;
}

void te::qt::widgets::CompositionModeMenuWidget::onSourceOverClicked()
{
  assert(m_layer.get());

  m_layer->setCompositionMode(te::map::SourceOver);
}

void te::qt::widgets::CompositionModeMenuWidget::onPlusClicked()
{
  assert(m_layer.get());

  m_layer->setCompositionMode(te::map::Plus);
}

void te::qt::widgets::CompositionModeMenuWidget::onCompositionModeClicked()
{
  assert(m_layer.get());

  te::qt::widgets::CompositionModeDialog dlg(this);

  dlg.setLayer(m_layer);

  dlg.exec();
}
