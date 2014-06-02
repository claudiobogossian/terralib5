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
  \file terralib/qt/widgets/query/DataSetWidget.cpp

  \brief This file has the DataSetWidget class.
*/

// TerraLib
#include "../../../dataaccess/query/DataSetName.h"
#include "DataSetWidget.h"
#include "ui_DataSetWidgetForm.h"

// Qt
#include <QIcon>
#include <QMessageBox>
#include <QToolButton>


te::qt::widgets::DataSetWidget::DataSetWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataSetWidgetForm)
{
  m_ui->setupUi(this);

  // set icons
  m_ui->m_addDataSetPushButton->setIcon(QIcon::fromTheme("list-add"));

  //connects
  connect(m_ui->m_dataSetComboBox, SIGNAL(activated(const QString&)), this, SLOT(onDataSetComboBoxActivated(const QString&)));
  connect(m_ui->m_addDataSetPushButton, SIGNAL(clicked()), this, SLOT(onAddDataSetPushButtonClicked()));
}

te::qt::widgets::DataSetWidget::~DataSetWidget()
{
}

Ui::DataSetWidgetForm* te::qt::widgets::DataSetWidget::getForm() const
{
  return m_ui.get();
}

te::da::From* te::qt::widgets::DataSetWidget::getFrom()
{
  te::da::From* from = new te::da::From;

  std::vector<std::pair<std::string, std::string> > vec;

  getDataSetNames(vec);

  for(size_t t = 0; t < vec.size(); ++t)
  {
    te::da::FromItem* fi = new te::da::DataSetName(vec[t].first, vec[t].second);

    from->push_back(fi);
  }

  return from;
}

void te::qt::widgets::DataSetWidget::setFrom(const te::da::From* from)
{
  std::vector<std::pair<std::string, std::string> > list;

  for(std::size_t i = 0; i < from->size(); ++i)
  {
    const te::da::FromItem& item = from->at(i);

    const te::da::DataSetName* dsName = dynamic_cast<const te::da::DataSetName*>(&item);

    std::pair<std::string, std::string> p;
    p.first = dsName->getName();
    p.second = dsName->getAlias();
    list.push_back(p);
  }

  setDataSetNames(list);
}

void te::qt::widgets::DataSetWidget::setDataSetNames(const std::vector<std::string>& names)
{
  QStringList list;

  for(size_t t = 0; t < names.size(); ++t)
  {
    list.push_back(names[t].c_str());
  }

  m_ui->m_dataSetComboBox->clear();
  m_ui->m_dataSetComboBox->addItems(list);

  if(names.empty() == false)
    onDataSetComboBoxActivated(m_ui->m_dataSetComboBox->currentText());
}

void te::qt::widgets::DataSetWidget::setDataSetNames(std::vector<std::pair<std::string, std::string> >& list)
{
  for(std::size_t i = 0; i < list.size(); ++i)
  {
    int newrow = m_ui->m_dataSetTableWidget->rowCount();

    //check if already exist an alias with this name
    for(int j = 0; j < newrow; ++j)
    {
      QTableWidgetItem* itemName = m_ui->m_dataSetTableWidget->item(j, 2);

      if(itemName->text().toStdString() == list[j].second)
      {
        QMessageBox::warning(this, tr("Query Editor"), tr("Data Set Alias already defined!"));
        return;
      }
    }

    //new entry
    m_ui->m_dataSetTableWidget->insertRow(newrow);

    //remove button
    QToolButton* removeBtn = new QToolButton(m_ui->m_dataSetTableWidget);
    removeBtn->setIcon(QIcon::fromTheme("list-remove"));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveDataSetPushButtonClicked()));
    m_ui->m_dataSetTableWidget->setCellWidget(newrow, 0, removeBtn);

    QTableWidgetItem* itemDataSet = new QTableWidgetItem(QString::fromStdString(list[i].first));
    m_ui->m_dataSetTableWidget->setItem(newrow, 1, itemDataSet);

    QTableWidgetItem* itemAlias = new QTableWidgetItem(QString::fromStdString(list[i].second));
    m_ui->m_dataSetTableWidget->setItem(newrow, 2, itemAlias);

    m_ui->m_dataSetTableWidget->resizeColumnToContents(0);

    emit itemChanged();
  }
}

void te::qt::widgets::DataSetWidget::getDataSetNames(std::vector<std::pair<std::string, std::string> >& list)
{
  int row = m_ui->m_dataSetTableWidget->rowCount();

  //get properties for each data set
  for(int i = 0; i < row; ++i)
  {
    //alias name
    QTableWidgetItem* itemAlias = m_ui->m_dataSetTableWidget->item(i, 2);
    std::string alias = itemAlias->text().toStdString();

    //data set name
    QTableWidgetItem* itemDataSet = m_ui->m_dataSetTableWidget->item(i, 1);
    std::string dataSetName = itemDataSet->text().toStdString();

    list.push_back(std::pair<std::string, std::string> (dataSetName, alias));
  }
}

void te::qt::widgets::DataSetWidget::onDataSetComboBoxActivated(const QString& value)
{
  if(value.isEmpty() == false)
  {
    std::string dataSetName = value.toStdString();
    std::string aliasName = value.toStdString();

    int pos = dataSetName.find(".");
    if(pos != std::string::npos)
    {
      aliasName = dataSetName.substr(pos + 1, dataSetName.size() - 1);
    }

    m_ui->m_dataSetAliasLineEdit->setText(aliasName.c_str());
  }
}

void te::qt::widgets::DataSetWidget::onAddDataSetPushButtonClicked()
{
  if(m_ui->m_dataSetComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Data Set not selected."));
    return;
  }

  if(m_ui->m_dataSetAliasLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Data Set Alias not defined."));
    return;
  }

  std::string dataSetName = m_ui->m_dataSetComboBox->currentText().toStdString();
  std::string aliasName = m_ui->m_dataSetAliasLineEdit->text().toStdString();

  int newrow = m_ui->m_dataSetTableWidget->rowCount();

  //check if already exist an alias with this name
  for(int i = 0; i < newrow; ++i)
  {
    QTableWidgetItem* itemName = m_ui->m_dataSetTableWidget->item(i, 2);

    if(itemName->text().toStdString() == aliasName)
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Data Set Alias already defined."));
      return;
    }
  }

  //new entry
  m_ui->m_dataSetTableWidget->insertRow(newrow);

  //remove button
  QToolButton* removeBtn = new QToolButton(m_ui->m_dataSetTableWidget);
  removeBtn->setIcon(QIcon::fromTheme("list-remove"));
  connect(removeBtn, SIGNAL(clicked()), this, SLOT(onRemoveDataSetPushButtonClicked()));
  m_ui->m_dataSetTableWidget->setCellWidget(newrow, 0, removeBtn);

  QTableWidgetItem* itemDataSet = new QTableWidgetItem(QString::fromStdString(dataSetName));
  m_ui->m_dataSetTableWidget->setItem(newrow, 1, itemDataSet);

  QTableWidgetItem* itemAlias = new QTableWidgetItem(QString::fromStdString(aliasName));
  m_ui->m_dataSetTableWidget->setItem(newrow, 2, itemAlias);

  m_ui->m_dataSetTableWidget->resizeColumnToContents(0);

  emit itemChanged();
}

void te::qt::widgets::DataSetWidget::onRemoveDataSetPushButtonClicked()
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if(button) 
  { 
    int row = -1;

    for(int i = 0; i < m_ui->m_dataSetTableWidget->rowCount(); ++i)
    {
      QWidget* w = m_ui->m_dataSetTableWidget->cellWidget(i, 0);
      QToolButton* btn = dynamic_cast<QToolButton*>(w);
      if(button == w)
      {
        row = i;
        break;
      }
    }

    if(row >= 0)
      m_ui->m_dataSetTableWidget->removeRow(row);

    m_ui->m_dataSetTableWidget->resizeColumnToContents(0);
  }
  emit itemChanged();
}
