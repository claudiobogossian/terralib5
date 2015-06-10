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
  \file terralib/qt/widgets/charts/scatterDataWidget.cpp

  \brief A widget used to adjust a scatter's input data.
*/

//Terralib

#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../raster.h"
#include "../../../statistics/core/Enums.h"
#include "../../../statistics/core/Utils.h"
#include "ScatterDataWidget.h"
#include "ui_ScatterDataWidgetForm.h"

void updateSummary(te::da::DataSet* dataSet, Ui::ScatterDataWidgetForm* ui)
{
  ui->m_summaryComboBox->clear();

  ui->m_summaryComboBox->addItem(QString::fromStdString("None"), QVariant(-1));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()), QVariant(te::stat::MIN_VALUE));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()), QVariant(te::stat::MAX_VALUE));
  //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()), QVariant(te::stat::COUNT));
  //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()), QVariant(te::stat::VALID_COUNT));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEAN).c_str()), QVariant(te::stat::MEAN));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SUM).c_str()), QVariant(te::stat::SUM));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION).c_str()), QVariant(te::stat::STANDARD_DEVIATION));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VARIANCE).c_str()), QVariant(te::stat::VARIANCE));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SKEWNESS).c_str()), QVariant(te::stat::SKEWNESS));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::KURTOSIS).c_str()), QVariant(te::stat::KURTOSIS));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE).c_str()), QVariant(te::stat::AMPLITUDE));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEDIAN).c_str()), QVariant(te::stat::MEDIAN));
  ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF).c_str()), QVariant(te::stat::VAR_COEFF));
}

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
    m_ui->m_readAllCheckBox->hide();
    for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
    {
      if((dataSet->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE) & (dataSet->getPropertyDataType(i) != te::dt::STRING_TYPE))
      {
        item = QString::fromStdString(dataSet->getPropertyName(i));
        m_ui->m_propertyXComboBox->addItem(item);
        m_ui->m_propertyYComboBox->addItem(item);
      }
    }
  }

  updateSummary(m_dataSet.get(), getForm());

  if(te::da::HasLinkedTable(dataType))
  {
    m_ui->m_summaryComboBox->show();
    m_ui->m_summaryLabel->show();
  }
  else
  {
    m_ui->m_summaryComboBox->hide();
    m_ui->m_summaryLabel->hide();
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
  int stat = m_ui->m_summaryComboBox->itemData(m_ui->m_summaryComboBox->currentIndex()).toInt();

  if(rpos != std::string::npos)
    return te::qt::widgets::createScatter(m_dataSet.get(), m_dataType.get(), m_ui->m_propertyXComboBox->currentIndex(), m_ui->m_propertyYComboBox->currentIndex(), stat,  m_ui->m_readAllCheckBox->isChecked());
  else
  {

    return te::qt::widgets::createScatter(m_dataSet.get(), m_dataType.get(),
                                         te::da::GetPropertyPos(m_dataSet.get(), m_ui->m_propertyXComboBox->currentText().toStdString()),
                                         te::da::GetPropertyPos(m_dataSet.get(), m_ui->m_propertyYComboBox->currentText().toStdString()), stat);
  }

}

te::qt::widgets::ScatterDataWidget::~ScatterDataWidget()
{
}
