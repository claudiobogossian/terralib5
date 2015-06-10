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
  \file terralib/qt/widgets/se/GraphicFillWidget.cpp

  \brief A widget used to build a fill element associated with a graphic.
*/

// TerraLib
#include "../../../se/Fill.h"
#include "GraphicDialog.h"
#include "GraphicFillWidget.h"
#include "ui_GraphicFillWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::GraphicFillWidget::GraphicFillWidget(QWidget* parent, Qt::WindowFlags f)
  : AbstractFillWidget(parent, f),
    m_ui(new Ui::GraphicFillWidgetForm),
    m_graphicDialog(new GraphicDialog),
    m_fill(new te::se::Fill)
{
  m_ui->setupUi(this);

  // Setups initial fill
  m_fill->setGraphicFill(m_graphicDialog->getGraphic());

  updateUi();

  // Signals & slots
  connect(m_ui->m_graphicToolButton, SIGNAL(pressed()), SLOT(onGraphicToolButtonPressed()));
}

te::qt::widgets::GraphicFillWidget::~GraphicFillWidget()
{
  delete m_fill;
}

bool te::qt::widgets::GraphicFillWidget::setFill(const te::se::Fill* fill)
{
  assert(fill);

  // Verifying if this widget can deal with the given fill...
  const te::se::Graphic* g = fill->getGraphicFill();
  if(g == 0)
    return false;

  delete m_fill;

  m_fill = fill->clone();

  m_graphicDialog->setGraphic(g);

  updateUi();

  return true;
}

te::se::Fill* te::qt::widgets::GraphicFillWidget::getFill() const
{
  return m_fill->clone();
}

QString te::qt::widgets::GraphicFillWidget::getFillType() const
{
  return tr("Graphic Fill");
}

void te::qt::widgets::GraphicFillWidget::updateUi()
{
  // Updating graphic icon
  QSize size = m_ui->m_graphicToolButton->iconSize();
  m_ui->m_graphicToolButton->setIcon(m_graphicDialog->getGraphicIcon(size));
}

void te::qt::widgets::GraphicFillWidget::onGraphicToolButtonPressed()
{
  if(m_graphicDialog->exec() == QDialog::Rejected)
    return;

  // Updating fill
  m_fill->setGraphicFill(m_graphicDialog->getGraphic());
  emit fillChanged();
  
  updateUi();
}
