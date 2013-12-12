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
  \file terralib/vp/AggregationDialog.cpp

  \brief A dialog for aggregation operation
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../core/Config.h"
#include "../core/Exception.h"
#include "../core/NumericStatisticalSummary.h"
#include "../core/StringStatisticalSummary.h"
#include "../core/SummaryFunctions.h"
#include "../core/Utils.h"
#include "StatisticsDialog.h"
#include "ui_StatisticsDialogForm.h"

// Qt
#include <QtGui/QListWidget>
#include <QtGui/QMessageBox>

// Boost
#include <boost/lexical_cast.hpp>

te::stat::StatisticsDialog::StatisticsDialog(QWidget* parent, Qt::WindowFlags f)
  :QDialog(parent, f),
  m_ui(new Ui::StatisticsDialogForm),
  m_dset(0)
{
  m_ui->setupUi(this);

  connect(m_ui->m_savePushButton, SIGNAL(clicked()), this, SLOT(onSavePushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::stat::StatisticsDialog::~StatisticsDialog()
{
}

void te::stat::StatisticsDialog::setStatistics(te::da::DataSet* dataSet, const std::string prop)
{
  m_dset = dataSet;
  m_prop = prop;
  int index = te::da::GetPropertyIndex(m_dset, m_prop);
  int propType = m_dset->getPropertyDataType(index);

  m_ui->m_datasourceTypeTitleLabel->setText("Statistics: " + QString(prop.c_str()));

  if(propType == te::dt::STRING_TYPE)
  {
    std::vector<std::string> values = te::stat::GetStringData(m_dset, m_prop);

    te::stat::StringStatisticalSummary ss;

    if(!values.empty())
    {
      te::stat::GetStringStatisticalSummary(values, ss);
      m_ui->m_statTableWidget->setRowCount(0);
      
      m_ui->m_statTableWidget->insertRow(0);
      QTableWidgetItem* itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()));
      QTableWidgetItem* itemValue = new QTableWidgetItem(QString(ss.m_minVal.c_str()));
      m_ui->m_statTableWidget->setItem(0, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(0, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(1);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()));
      itemValue = new QTableWidgetItem(QString(ss.m_maxVal.c_str()));
      m_ui->m_statTableWidget->setItem(1, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(1, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(2);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()));
      itemValue = new QTableWidgetItem(QString(QString(boost::lexical_cast<std::string>(m_dset->size()).c_str())));
      m_ui->m_statTableWidget->setItem(2, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(2, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(3);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()));
      itemValue = new QTableWidgetItem(QString(QString(boost::lexical_cast<std::string>(values.size()).c_str())));
      m_ui->m_statTableWidget->setItem(3, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(3, 1, itemValue);
    }
    else
      QMessageBox::information(this, "Warning", "The input vector of values is empty.");
  }
  else
  {
    std::vector<double> values = te::stat::GetNumericData(m_dset, m_prop);

    te::stat::NumericStatisticalSummary ss;
    
    if(!values.empty())
    {
      te::stat::GetNumericStatisticalSummary(values, ss);

      m_ui->m_statTableWidget->insertRow(0);
      QTableWidgetItem* itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()));
      QTableWidgetItem* itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_minVal).c_str()));
      m_ui->m_statTableWidget->setItem(0, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(0, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(1);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_maxVal).c_str()));
      m_ui->m_statTableWidget->setItem(1, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(1, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(2);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(m_dset->size()).c_str()));
      m_ui->m_statTableWidget->setItem(2, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(2, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(3);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(values.size()).c_str()));
      m_ui->m_statTableWidget->setItem(3, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(3, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(4);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEAN).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_mean).c_str()));
      m_ui->m_statTableWidget->setItem(4, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(4, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(5);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::SUM).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_sum).c_str()));
      m_ui->m_statTableWidget->setItem(5, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(5, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(6);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_stdDeviation).c_str()));
      m_ui->m_statTableWidget->setItem(6, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(6, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(7);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::VARIANCE).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_variance).c_str()));
      m_ui->m_statTableWidget->setItem(7, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(7, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(8);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::SKEWNESS).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_skewness).c_str()));
      m_ui->m_statTableWidget->setItem(8, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(8, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(9);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::KURTOSIS).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_kurtosis).c_str()));
      m_ui->m_statTableWidget->setItem(9, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(9, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(10);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_amplitude).c_str()));
      m_ui->m_statTableWidget->setItem(10, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(10, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(11);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEDIAN).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_median).c_str()));
      m_ui->m_statTableWidget->setItem(11, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(11, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(12);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF).c_str()));
      itemValue = new QTableWidgetItem(QString(boost::lexical_cast<std::string>(ss.m_varCoeff).c_str()));
      m_ui->m_statTableWidget->setItem(12, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(12, 1, itemValue);

      m_ui->m_statTableWidget->insertRow(13);
      itemParameter = new QTableWidgetItem(QString(te::stat::GetStatSummaryFullName(te::stat::MODE).c_str()));
      if(ss.m_mode.size() == 0)
        itemValue = new QTableWidgetItem(QString(""));
      else
      {
        std::string value;
        for(std::size_t i = 0; i < ss.m_mode.size(); ++i)
          value += ", "+ boost::lexical_cast<std::string>(ss.m_mode[i]);

        value.erase(0,2);
        itemValue = new QTableWidgetItem(QString(value.c_str()));
      }
      m_ui->m_statTableWidget->setItem(13, 0, itemParameter);
      m_ui->m_statTableWidget->setItem(13, 1, itemValue);

    }
    else
      QMessageBox::information(this, "Warning", "The input vector of values is empty.");
  }
  m_ui->m_statTableWidget->resizeColumnToContents(0);
}

void te::stat::StatisticsDialog::onSavePushButtonClicked()
{
  QMessageBox::information(this, "Save", "Under development - It should save the result...");
}

void te::stat::StatisticsDialog::onCancelPushButtonClicked()
{
  reject();
}