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
  \file terralib/qt/widgets/se/PointSymbolizerWidget.cpp

  \brief A widget used to build a point symbolizer element.
*/

// TerraLib
#include "../../../se/PointSymbolizer.h"
#include "GraphicSelectorWidget.h"
#include "PointSymbolizerWidget.h"

// Qt
#include <QGridLayout>

// STL
#include <cassert>

te::qt::widgets::PointSymbolizerWidget::PointSymbolizerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_symb(new te::se::PointSymbolizer)
{
  // Graphic dialog
  m_graphicSelector = new GraphicSelectorWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_graphicSelector);

  // Setups initial point symbolizer
  m_symb->setGraphic(m_graphicSelector->getGraphic());

  // Signals & slots
  connect(m_graphicSelector, SIGNAL(graphicChanged()), SLOT(onGraphicChanged()));
}

te::qt::widgets::PointSymbolizerWidget::~PointSymbolizerWidget()
{
  delete m_symb;
}

void te::qt::widgets::PointSymbolizerWidget::setSymbolizer(const te::se::PointSymbolizer* symb)
{
  assert(symb);

  delete m_symb;

  m_symb = static_cast<te::se::PointSymbolizer*>(symb->clone());

  updateUi();
}

te::se::Symbolizer* te::qt::widgets::PointSymbolizerWidget::getSymbolizer() const
{
  return m_symb->clone();
}

void te::qt::widgets::PointSymbolizerWidget::updateUi()
{
  const te::se::Graphic* g = m_symb->getGraphic();
  assert(g);

  m_graphicSelector->setGraphic(g);
}

void te::qt::widgets::PointSymbolizerWidget::onGraphicChanged()
{
  m_symb->setGraphic(m_graphicSelector->getGraphic());
  emit symbolizerChanged();
}
