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
  \file terralib/qt/widgets/charts/ChartLayerWidget.cpp

  \brief A widget used to build a grouping.
*/

// TerraLib
#include "ChartLayerWidget.h"
#include "ui_ChartLayerWidgetForm.h"

te::qt::widgets::ChartLayerWidget::ChartLayerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ChartLayerWidgetForm)
{
  m_ui->setupUi(this);

//connects
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked(int)));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked(int)));

  initialize();
}

te::qt::widgets::ChartLayerWidget::~ChartLayerWidget()
{
}

void te::qt::widgets::ChartLayerWidget::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  listAttributes();
}

void te::qt::widgets::ChartLayerWidget::initialize()
{

}

void te::qt::widgets::ChartLayerWidget::updateUi()
{
  m_ui->m_tableWidget->setRowCount(0);
}

void te::qt::widgets::ChartLayerWidget::onAddToolButtonClicked()
{
}

void te::qt::widgets::ChartLayerWidget::onRemoveToolButtonClicked()
{
}

void te::qt::widgets::ChartLayerWidget::listAttributes()
{
  m_ui->m_attrComboBox->clear();

  std::auto_ptr<te::map::LayerSchema> dsType(m_layer->getSchema());

  for(size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    te::dt::Property* p = dsType->getProperty(t);

    int dataType = p->getType();

    switch(dataType)
    {
      case te::dt::INT16_TYPE:
      case te::dt::INT32_TYPE:
      case te::dt::INT64_TYPE:
      case te::dt::FLOAT_TYPE:
      case te::dt::DOUBLE_TYPE:
      case te::dt::NUMERIC_TYPE:
        m_ui->m_attrComboBox->addItem(p->getName().c_str(), p->getType());

      default:
        continue;
    }
  }
}
