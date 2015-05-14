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
  \file terralib/qt/widgets/se/GraphicSelectorWidget.cpp

  \brief A widget used to select a graphic element.
*/

// TerraLib
#include "AbstractGraphicWidget.h"
#include "AbstractGraphicWidgetFactory.h"
#include "GraphicSelectorWidget.h"
#include "ui_GraphicSelectorWidgetForm.h"

// Qt
#include <QStackedWidget>

te::qt::widgets::GraphicSelectorWidget::GraphicSelectorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::GraphicSelectorWidgetForm)
{
  m_ui->setupUi(this);

  // Graphic Widgets
  m_graphicWidgets = new QStackedWidget(this);

  // Gets registered graphic widgets
  std::vector<std::string> keys;
  te::qt::widgets::AbstractGraphicWidgetFactory::RegisteredWidgets(keys);
  std::vector<std::string>::reverse_iterator it;
  for(it = keys.rbegin(); it != keys.rend(); ++it)
  {
    te::qt::widgets::AbstractGraphicWidget* gw = te::qt::widgets::AbstractGraphicWidgetFactory::make(*it);
    m_ui->m_graphicTypeComboBox->addItem(gw->getGraphicType());
    connect(gw, SIGNAL(graphicChanged()), SLOT(onGraphicChanged()));
    m_graphicWidgets->addWidget(gw);
  }

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_graphicWidgetFrame);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_graphicWidgets);

  // Signals & slots
  connect(m_ui->m_graphicTypeComboBox, SIGNAL(activated(int)), SLOT(onGraphicTypeComboBoxCurrentIndexChanged(int)));
}

te::qt::widgets::GraphicSelectorWidget::~GraphicSelectorWidget()
{
}

void te::qt::widgets::GraphicSelectorWidget::setGraphic(const te::se::Graphic* graphic)
{
  int i;
  for(i = 0; i < m_graphicWidgets->count(); ++i)
  {
    te::qt::widgets::AbstractGraphicWidget* gw = static_cast<te::qt::widgets::AbstractGraphicWidget*>(m_graphicWidgets->widget(i));
    if(gw->setGraphic(graphic))
      break;
  }

  if(i == m_graphicWidgets->count())
    return; // TODO: Exception! The given graphic cannot be dealt by any registered graphic widgets.

  // Updating Ui
  m_ui->m_graphicTypeComboBox->setCurrentIndex(i);
  m_graphicWidgets->setCurrentIndex(i);
}

te::se::Graphic* te::qt::widgets::GraphicSelectorWidget::getGraphic() const
{
  te::qt::widgets::AbstractGraphicWidget* g = static_cast<te::qt::widgets::AbstractGraphicWidget*>(m_graphicWidgets->currentWidget());
  return g->getGraphic();
}

QIcon te::qt::widgets::GraphicSelectorWidget::getGraphicIcon(const QSize& size)
{
  te::qt::widgets::AbstractGraphicWidget* g = static_cast<te::qt::widgets::AbstractGraphicWidget*>(m_graphicWidgets->currentWidget());
  return g->getGraphicIcon(size);
}

void te::qt::widgets::GraphicSelectorWidget::onGraphicTypeComboBoxCurrentIndexChanged(int index)
{
  m_graphicWidgets->setCurrentIndex(index);
  emit graphicChanged();
}

void te::qt::widgets::GraphicSelectorWidget::onGraphicChanged()
{
  // Propagates the signal
  emit graphicChanged();
}
