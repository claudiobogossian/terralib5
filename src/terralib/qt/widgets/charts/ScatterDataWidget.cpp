/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/scatterDataWidget.cpp

  \brief A widget used to adjust a scatter's input data.
*/

//Terralib
#include "ui_scatterDataWidgetForm.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "ScatterDataWidget.h"

te::qt::widgets::ScatterDataWidget::ScatterDataWidget(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ScatterDataWidgetForm),
    m_dataSet (dataSet)
{
    m_ui->setupUi(this);

  QString item;

  for (size_t i = 0; i < dataSet->getNumProperties(); i++)
  {
    item = item.fromStdString( dataSet->getPropertyName(i));
    m_ui->m_propertyXComboBox->addItem(item);
    m_ui->m_propertyYComboBox->addItem(item);
  }
}

te::qt::widgets::ScatterDataWidget::~ScatterDataWidget(){}
