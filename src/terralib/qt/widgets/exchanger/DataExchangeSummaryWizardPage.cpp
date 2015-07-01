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
  \file terralib/plugins/exchanger/DataExchangeSummaryWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "ui_DataExchangeSummaryWizardPageForm.h"
#include "DataExchangeStatus.h"
#include "DataExchangeSummaryWizardPage.h"

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QIcon>
#include <QMessageBox>

te::qt::widgets::DataExchangeSummaryWizardPage::DataExchangeSummaryWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::DataExchangeSummaryWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  m_status = true;

  m_ui->m_reportTableWidget->resizeColumnsToContents();
}

te::qt::widgets::DataExchangeSummaryWizardPage::~DataExchangeSummaryWizardPage()
{
}

bool te::qt::widgets::DataExchangeSummaryWizardPage::isComplete() const
{
  return m_status;
}

void te::qt::widgets::DataExchangeSummaryWizardPage::set(const std::list<DataExchangeStatus>& result)
{
  m_ui->m_reportTableWidget->clearContents();

  m_ui->m_reportTableWidget->setRowCount(static_cast<int>(result.size()));

  int row = 0;

  for(std::list<DataExchangeStatus>::const_iterator it = result.begin(); it != result.end(); ++it)
  {
    QTableWidgetItem* item = new QTableWidgetItem(it->m_successful ? QIcon::fromTheme("check") : QIcon::fromTheme("delete"), "");
    m_ui->m_reportTableWidget->setItem(row, 0, item);

    item = new QTableWidgetItem(QString::fromStdString(it->m_dataset->getName()));
    m_ui->m_reportTableWidget->setItem(row, 1, item);

    if(it->m_successful)
    {
      std::string t;

      double sec = it->m_time.count();

      if(sec < 1.0)
      {
        boost::chrono::milliseconds dmili(boost::chrono::duration_cast<boost::chrono::milliseconds>(it->m_time));

        t = boost::lexical_cast<std::string>(dmili.count());
        t += " miliseconds";
      }
      else if(sec < 60.0)
      {
        boost::chrono::seconds dseconds(boost::chrono::duration_cast<boost::chrono::seconds>(it->m_time));
        t = boost::lexical_cast<std::string>(dseconds.count());
        t += " seconds";
      }
      else if(sec < 3600.0)
      {
        boost::chrono::minutes dminutes(boost::chrono::duration_cast<boost::chrono::minutes>(it->m_time));
        t = boost::lexical_cast<std::string>(dminutes.count());
        t += " minutes";
      }
      else
      {
        t = boost::lexical_cast<std::string>(it->m_time.count());
        t += " seconds";
      }

      item = new QTableWidgetItem(QString::fromStdString(t));
      m_ui->m_reportTableWidget->setItem(row, 2, item);
    }
    else
      m_status = false;

    item = new QTableWidgetItem(it->m_successful ? tr("Successfully transferred") : QString::fromStdString(it->m_exceptionMsg));
    m_ui->m_reportTableWidget->setItem(row, 3, item);

    ++row;
  }

  m_ui->m_reportTableWidget->resizeColumnsToContents();
}

