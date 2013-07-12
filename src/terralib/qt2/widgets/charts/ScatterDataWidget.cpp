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
#include "ui_ScatterDataWidgetForm.h"
#include "../../../qt2/widgets/charts/Utils.h"
#include "../../../dataaccess2.h"
#include "../../../datatype/Property.h"
#include "../../../raster.h"
#include "ScatterDataWidget.h"

te::qt::widgets::ScatterDataWidget::ScatterDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ScatterDataWidgetForm),
    m_dataSet (dataSet),
    m_dataType(dataType)
{
    m_ui->setupUi(this);

  QString item;

  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet, te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
    {
      size_t size =  dataSet->getRaster(rpos)->getNumberOfBands();
      for (size_t i = 0; i < size; i++)
      {
        item = QString::number(i);
        m_ui->m_propertyXComboBox->addItem(QString::fromStdString("Band: ") + item);
        m_ui->m_propertyYComboBox->addItem(QString::fromStdString("Band: ") + item);
      }
    }
  else
  {
    for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
    {
      item = QString::fromStdString(dataSet->getPropertyName(i));
      m_ui->m_propertyXComboBox->addItem(item);
      m_ui->m_propertyYComboBox->addItem(item);
    }
  }
}

Ui::ScatterDataWidgetForm* te::qt::widgets::ScatterDataWidget::getForm()
{
  return m_ui.get();
}

te::qt::widgets::Scatter* te::qt::widgets::ScatterDataWidget::getScatter()
{
  //Acquiring the dataset Properties types and creating a new scatter
    std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet.get(), te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
    return te::qt::widgets::createScatter(m_dataSet.get(), m_dataType.get(), m_ui->m_propertyXComboBox->currentIndex(), m_ui->m_propertyYComboBox->currentIndex());
  else
    return te::qt::widgets::createScatter(m_dataSet.get(), m_dataType.get(), te::da::GetPropertyPos(m_dataSet.get(), m_ui->m_propertyXComboBox->currentText().toStdString()), te::da::GetPropertyPos(m_dataSet.get(), m_ui->m_propertyYComboBox->currentText().toStdString()));
}

te::qt::widgets::ScatterDataWidget::~ScatterDataWidget()
{
}
