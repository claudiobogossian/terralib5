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
  \file terralib/qt/widgets/query/QueryBuilderWizard.cpp

  \brief A Qt dialog that allows users to create query builder based on TerraLib query framework.
*/

// TerraLib
#include "QueryBuilderWizard.h"

#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/query/SQLDialect.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
//#include "../../../dataaccess/datasource/DataSourceTransactor.h"
//#include "../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../utils/DoubleListWidget.h"
#include "ui_DoubleListWidgetForm.h"
#include "ui_QueryBuilderWizardForm.h"


// Qt
#include <QMessageBox>


te::qt::widgets::QueryBuilderWizard::QueryBuilderWizard(QWidget* parent)
  : QWizard(parent),
    m_ui(new Ui::QueryBuilderWizardForm)
{
  m_ui->setupUi(this);

  //fill interface
  getDataSourceIds();
  getOperators();
  getConnectors();
  getOrders();

  // set icons
  m_ui->m_addDataSetPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeDataSetPushButton->setIcon(QIcon::fromTheme("list-remove"));

  m_ui->m_addWhereClausePushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeWhereClausePushButton->setIcon(QIcon::fromTheme("list-remove"));

  m_ui->m_addOrderPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeOrderPushButton->setIcon(QIcon::fromTheme("list-remove"));

  // properties
  m_propertyList.reset(new DoubleListWidget(m_ui->m_propertyWizardPage));
  m_propertyList->getForm()->m_leftItemsLabel->setText(tr("Available Properties"));
  m_propertyList->getForm()->m_rightItemsLabel->setText(tr("Used Properties"));
  m_ui->m_propertyGridLayout->addWidget(m_propertyList.get());

  // group by
  m_groupByList.reset(new DoubleListWidget(m_ui->m_groupByWizardPage));
  m_groupByList->getForm()->m_leftItemsLabel->setText(tr("Available Properties"));
  m_groupByList->getForm()->m_rightItemsLabel->setText(tr("Used Properties"));
  m_ui->m_groupByGridLayout->addWidget(m_groupByList.get());

  //connects
  connect(m_ui->m_dataSourceComboBox, SIGNAL(activated(const QString&)), this, SLOT(onDataSourceComboBoxActivated(const QString&)));
  connect(m_ui->m_dataSetComboBox, SIGNAL(activated(const QString&)), this, SLOT(onDataSetComboBoxActivated(const QString&)));
  connect(m_ui->m_addDataSetPushButton, SIGNAL(clicked()), this, SLOT(onAddDataSetPushButtonClicked()));
  connect(m_ui->m_removeDataSetPushButton, SIGNAL(clicked()), this, SLOT(onRemoveDataSetPushButtonClicked()));
  connect(m_ui->m_addWhereClausePushButton, SIGNAL(clicked()), this, SLOT(onAddWhereClausePushButtonClicked()));
  connect(m_ui->m_removeWhereClausePushButton, SIGNAL(clicked()), this, SLOT(onRemoveWhereClausePushButtonClicked()));
  connect(m_ui->m_addOrderPushButton, SIGNAL(clicked()), this, SLOT(onAddOrderPushButton()));
  connect(m_ui->m_removeOrderPushButton, SIGNAL(clicked()), this, SLOT(onRemoveOrderPushButton()));
}

te::qt::widgets::QueryBuilderWizard::~QueryBuilderWizard()
{
  m_bimapOperator.clear();
  m_bimapConnector.clear();
  m_bimapOrder.clear();
}

bool te::qt::widgets::QueryBuilderWizard::validateCurrentPage()
{
  if(currentPage() == m_ui->m_dataSourceWizardPage)
  {
    return checkDataSourcePage();
  }
  else if(currentPage() == m_ui->m_dataSetWizardPage)
  {
    return checkDataSetPage();
  }
  else if(currentPage() == m_ui->m_propertyWizardPage)
  {
    return checkPropertiesPage();
  }
  else if(currentPage() == m_ui->m_whereClauseWizardPage)
  {
    return checkWhereClausePage();
  }
  else if(currentPage() == m_ui->m_groupByWizardPage)
  {
    return checkGroupByPage();
  }
  else if(currentPage() == m_ui->m_orderByWizardPage)
  {
    return checkOrderByPage();
  }

  return true;
}

void te::qt::widgets::QueryBuilderWizard::getDataSourceIds()
{
  te::da::DataSourceManager::iterator it = te::da::DataSourceManager::getInstance().begin();

  QStringList list;

  while(it != te::da::DataSourceManager::getInstance().end())
  {
    list.push_back(it->first.c_str());

    ++it;
  }

  m_ui->m_dataSourceComboBox->clear();
  m_ui->m_dataSourceComboBox->addItems(list);
}

void te::qt::widgets::QueryBuilderWizard::getDataSets()
{
  std::string dsId = m_ui->m_dataSourceComboBox->currentText().toStdString();

  std::vector<std::string> datasetNames;

  te::da::GetDataSetNames(datasetNames, dsId);

  QStringList list;

  for(size_t t = 0; t < datasetNames.size(); ++t)
  {
    list.push_back(datasetNames[t].c_str());
  }

  m_ui->m_dataSetComboBox->clear();
  m_ui->m_dataSetComboBox->addItems(list);

  if(datasetNames.empty() == false)
    onDataSetComboBoxActivated(m_ui->m_dataSetComboBox->currentText());
}

void te::qt::widgets::QueryBuilderWizard::getProperties()
{
  //get dataset list
  std::string dsId = m_ui->m_dataSourceComboBox->currentText().toStdString();
  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(dsId);
  std::vector<std::string> datasetNames = ds->getDataSetNames();

  int row = m_ui->m_dataSetTableWidget->rowCount();

  std::vector<std::string> inputProperties;
  QStringList list;

  //get properties for each data set
  for(int i = 0; i < row; ++i)
  {
    //alias name
    QTableWidgetItem* itemAlias = m_ui->m_dataSetTableWidget->item(i, 1);
    std::string alias = itemAlias->text().toStdString();

    //data set name
    QTableWidgetItem* itemDataSet = m_ui->m_dataSetTableWidget->item(i, 0);
    std::string dataSetName = itemDataSet->text().toStdString();

    //get datasettype
    std::auto_ptr<te::da::DataSetType> dsType;
    for(unsigned int i = 0; i < datasetNames.size(); ++i)
    {
      if(datasetNames[i] == dataSetName)
        dsType = ds->getDataSetType(datasetNames[i]);
    }

    if(dsType.get())
    {
      for(size_t t = 0; t < dsType->size(); ++t)
      {
        std::string propName = dsType->getProperty(t)->getName();
        std::string fullName = alias + "." + propName;

        inputProperties.push_back(fullName);
        list.push_back(fullName.c_str());
      }
    }

  }

  m_propertyList->setInputValues(inputProperties);
  m_groupByList->setInputValues(inputProperties);

  m_ui->m_restrictValueComboBox->clear();
  m_ui->m_restrictValueComboBox->addItems(list);

  m_ui->m_valuePropertyComboBox->clear();
  m_ui->m_valuePropertyComboBox->addItems(list);

  m_ui->m_orderPropertyComboBox->clear();
  m_ui->m_orderPropertyComboBox->addItems(list);
}

void te::qt::widgets::QueryBuilderWizard::getOperators()
{
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Plus, tr("+").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Minus, tr("-").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Multi, tr("*").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Div, tr("/").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Equal, tr("=").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Dif, tr("<>").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Great, tr(">").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_GreatEqual, tr(">=").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_Less, tr("<").toStdString()));
  m_bimapOperator.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOperator_LessEqual, tr("<=").toStdString()));

  m_ui->m_OperatorComboBox->clear();

  boost::bimap<int, std::string>::iterator it = m_bimapOperator.begin();

  while(it != m_bimapOperator.end())
  {
    m_ui->m_OperatorComboBox->addItem(it->right.c_str());
    ++it;
  }
}

void te::qt::widgets::QueryBuilderWizard::getConnectors()
{
  m_bimapConnector.insert(boost::bimap<int, std::string>::value_type(QueryBuilderConnector_NONE, tr("---").toStdString()));
  m_bimapConnector.insert(boost::bimap<int, std::string>::value_type(QueryBuilderConnector_AND, tr("AND").toStdString()));
  m_bimapConnector.insert(boost::bimap<int, std::string>::value_type(QueryBuilderConnector_OR, tr("OR").toStdString()));
  m_bimapConnector.insert(boost::bimap<int, std::string>::value_type(QueryBuilderConnector_NOT, tr("NOT").toStdString()));
  m_bimapConnector.insert(boost::bimap<int, std::string>::value_type(QueryBuilderConnector_LIKE, tr("LIKE").toStdString()));

  m_ui->m_connectorComboBox->clear();

  boost::bimap<int, std::string>::left_const_iterator it;
  it = m_bimapConnector.left.find(QueryBuilderConnector_NONE);
  m_ui->m_connectorComboBox->addItem(it->second.c_str());
  it = m_bimapConnector.left.find(QueryBuilderConnector_AND);
  m_ui->m_connectorComboBox->addItem(it->second.c_str());
  it = m_bimapConnector.left.find(QueryBuilderConnector_OR);
  m_ui->m_connectorComboBox->addItem(it->second.c_str());
  it = m_bimapConnector.left.find(QueryBuilderConnector_NOT);
  m_ui->m_connectorComboBox->addItem(it->second.c_str());
  it = m_bimapConnector.left.find(QueryBuilderConnector_LIKE);
  m_ui->m_connectorComboBox->addItem(it->second.c_str());
}

void te::qt::widgets::QueryBuilderWizard::getOrders()
{
  m_bimapOrder.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOrder_ASC, tr("ASC").toStdString()));
  m_bimapOrder.insert(boost::bimap<int, std::string>::value_type(QueryBuilderOrder_DESC, tr("DESC").toStdString()));

  m_ui->m_orderComboBox->clear();

  boost::bimap<int, std::string>::iterator it = m_bimapOrder.begin();

  while(it != m_bimapOrder.end())
  {
    m_ui->m_orderComboBox->addItem(it->right.c_str());
    ++it;
  }
}

bool te::qt::widgets::QueryBuilderWizard::checkDataSourcePage()
{
  if(m_ui->m_dataSourceComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Data Source not selected."));
    return false;
  }
  else
  {
    getDataSets();
  }

  return true;
}

bool te::qt::widgets::QueryBuilderWizard::checkDataSetPage()
{
  int numRows = m_ui->m_dataSetTableWidget->rowCount();

  if(numRows == 0)
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("No Data Set selected."));
    return false;
  }
  else
  {
    getProperties();
  }

  return true;
}

bool te::qt::widgets::QueryBuilderWizard::checkPropertiesPage()
{
  if(m_propertyList->getOutputValues().empty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("No Property selected."));
    return false;
  }

  return true;
}

bool te::qt::widgets::QueryBuilderWizard::checkWhereClausePage()
{
  int numRows = m_ui->m_whereClauseTableWidget->rowCount();

  if(numRows == 0)
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("No Where Clause defined."));
    return false;
  }

  return true;
}

bool te::qt::widgets::QueryBuilderWizard::checkGroupByPage()
{
  return true;
}

bool te::qt::widgets::QueryBuilderWizard::checkOrderByPage()
{
  return buildQuery();
}

bool te::qt::widgets::QueryBuilderWizard::buildQuery()
{
  return true;
}

void te::qt::widgets::QueryBuilderWizard::onDataSourceComboBoxActivated(const QString& value)
{
  std::string dataSourceName = value.toStdString();

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(dataSourceName);
}

void te::qt::widgets::QueryBuilderWizard::onDataSetComboBoxActivated(const QString& value)
{
  if(value.isEmpty() == false)
    m_ui->m_dataSetAliasLineEdit->setText(value);
}

void te::qt::widgets::QueryBuilderWizard::onAddDataSetPushButtonClicked()
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
    QTableWidgetItem* itemName = m_ui->m_dataSetTableWidget->item(i, 1);

    if(itemName->text().toStdString() == aliasName)
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Data Set Alias already defined."));
      return;
    }
  }

  //new entry
  m_ui->m_dataSetTableWidget->insertRow(newrow);

  QTableWidgetItem* itemDataSet = new QTableWidgetItem(QString::fromStdString(dataSetName));
  m_ui->m_dataSetTableWidget->setItem(newrow, 0, itemDataSet);

  QTableWidgetItem* itemAlias = new QTableWidgetItem(QString::fromStdString(aliasName));
  m_ui->m_dataSetTableWidget->setItem(newrow, 1, itemAlias);

  m_ui->m_dataSetTableWidget->resizeColumnToContents(0);
}

void te::qt::widgets::QueryBuilderWizard::onRemoveDataSetPushButtonClicked()
{
  int row = m_ui->m_dataSetTableWidget->currentRow();

  if(row >= 0)
    m_ui->m_dataSetTableWidget->removeRow(row);

  m_ui->m_dataSetTableWidget->resizeColumnToContents(0);
}

void te::qt::widgets::QueryBuilderWizard::onAddWhereClausePushButtonClicked()
{
  if(m_ui->m_restrictValueComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Restrict value not defined."));
    return;
  }

  if(m_ui->m_valuePropertyRadioButton->isChecked() == false &&
     m_ui->m_valueValueRadioButton->isChecked() == false)
  {
     if(m_ui->m_valueValueComboBox->currentText().isEmpty())
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Value not defined."));
      return;
    }
  }

  if(m_ui->m_valueValueRadioButton->isChecked())
  {
    if(m_ui->m_valueValueComboBox->currentText().isEmpty())
    {
      QMessageBox::warning(this, tr("Query Builder"), tr("Value not defined."));
      return;
    }
  }

  int newrow = m_ui->m_whereClauseTableWidget->rowCount();

  std::string restrictValue = m_ui->m_restrictValueComboBox->currentText().toStdString();
  std::string operatorStr = m_ui->m_OperatorComboBox->currentText().toStdString();
  std::string connector = m_ui->m_connectorComboBox->currentText().toStdString();
  
  std::string valueStr = "";
  if(m_ui->m_valuePropertyRadioButton->isChecked())
    valueStr = m_ui->m_valuePropertyComboBox->currentText().toStdString();
  else if(m_ui->m_valueValueRadioButton->isChecked())
    valueStr = m_ui->m_valueValueComboBox->currentText().toStdString();

//new entry
  m_ui->m_whereClauseTableWidget->insertRow(newrow);

  QTableWidgetItem* itemProperty = new QTableWidgetItem(QString::fromStdString(restrictValue));
  m_ui->m_whereClauseTableWidget->setItem(newrow, 0, itemProperty);

  QTableWidgetItem* itemOperator = new QTableWidgetItem(QString::fromStdString(operatorStr));
  m_ui->m_whereClauseTableWidget->setItem(newrow, 1, itemOperator);

  QTableWidgetItem* itemValue = new QTableWidgetItem(QString::fromStdString(valueStr));
  m_ui->m_whereClauseTableWidget->setItem(newrow, 2, itemValue);

  QTableWidgetItem* itemConnector = new QTableWidgetItem(QString::fromStdString(connector));
  m_ui->m_whereClauseTableWidget->setItem(newrow, 3, itemConnector);

  m_ui->m_whereClauseTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::QueryBuilderWizard::onRemoveWhereClausePushButtonClicked()
{
  int row = m_ui->m_whereClauseTableWidget->currentRow();

  if(row >= 0)
    m_ui->m_whereClauseTableWidget->removeRow(row);

  m_ui->m_whereClauseTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::QueryBuilderWizard::onAddOrderPushButton()
{
  if(m_ui->m_orderPropertyComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Property value not defined."));
    return;
  }

  int newrow = m_ui->m_orderTableWidget->rowCount();

  std::string propertyValue = m_ui->m_orderPropertyComboBox->currentText().toStdString();
  std::string order = m_ui->m_orderComboBox->currentText().toStdString();

//new entry
  m_ui->m_orderTableWidget->insertRow(newrow);

  QTableWidgetItem* itemProperty = new QTableWidgetItem(QString::fromStdString(propertyValue));
  m_ui->m_orderTableWidget->setItem(newrow, 0, itemProperty);

  QTableWidgetItem* itemOrder = new QTableWidgetItem(QString::fromStdString(order));
  m_ui->m_orderTableWidget->setItem(newrow, 1, itemOrder);

  m_ui->m_orderTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::QueryBuilderWizard::onRemoveOrderPushButton()
{
  int row = m_ui->m_orderTableWidget->currentRow();

  if(row >= 0)
    m_ui->m_orderTableWidget->removeRow(row);

  m_ui->m_orderTableWidget->resizeColumnsToContents();
}
