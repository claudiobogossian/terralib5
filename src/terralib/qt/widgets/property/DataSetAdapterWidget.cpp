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
  \file terralib/qt/widgets/property/DataSetAdapterWidget.cpp

  \brief A class used to define the DataSetAdapterWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "DataSetAdapterWidget.h"
#include "PropertyConverterDialog.h"
#include "ui_DataSetAdapterWidgetForm.h"

// Qt
#include <QMessageBox>


te::qt::widgets::DataSetAdapterWidget::DataSetAdapterWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataSetAdapterWidgetForm),
    m_sourceDataSetType(0),
    m_dataSetConverter(0)
{
  m_ui->setupUi(this);

  buidTypeMap();

// button icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_upToolButton->setIcon(QIcon::fromTheme("go-up"));
  m_ui->m_downToolButton->setIcon(QIcon::fromTheme("go-down"));

  //connects
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_editToolButton, SIGNAL(clicked()), this, SLOT(onEditToolButtonClicked()));
  connect(m_ui->m_upToolButton, SIGNAL(clicked()), this, SLOT(onUpToolButtonClicked()));
  connect(m_ui->m_downToolButton, SIGNAL(clicked()), this, SLOT(onDownToolButtonClicked()));
  connect(m_ui->m_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));
}

te::qt::widgets::DataSetAdapterWidget::~DataSetAdapterWidget()
{

}

void te::qt::widgets::DataSetAdapterWidget::setAdapterParameters(te::da::DataSetType* sourceDataSetType, te::da::DataSetTypeConverter* converter, const te::da::DataSourceInfoPtr& targetDatasource)
{
  //update interface button state
  m_ui->m_addToolButton->setEnabled(true);

  m_targetDataSource = te::da::DataSourceManager::getInstance().find(targetDatasource->getId());

  m_sourceDataSetType = sourceDataSetType;

  m_dataSetConverter = converter;

  fillDataSetTable();
}

te::da::DataSetTypeConverter* te::qt::widgets::DataSetAdapterWidget::getConverter()
{
  return m_dataSetConverter;
}

void te::qt::widgets::DataSetAdapterWidget::onAddToolButtonClicked()
{
  te::qt::widgets::PropertyConverterDialog w(this);

  w.set(m_targetDataSource->getId());
  w.set(m_sourceDataSetType);

  if(w.exec() == QDialog::Accepted)
  {
    w.adapt(m_dataSetConverter);

    fillDataSetTable();
  }
}

void te::qt::widgets::DataSetAdapterWidget::onRemoveToolButtonClicked()
{
  if(m_ui->m_tableWidget->currentRow() == -1)
    return;

  std::string propName = m_ui->m_tableWidget->item(m_ui->m_tableWidget->currentRow(), 1)->text().toStdString();

  m_dataSetConverter->remove(propName);

  m_ui->m_removeToolButton->setEnabled(false);

  fillDataSetTable();
}

void te::qt::widgets::DataSetAdapterWidget::onEditToolButtonClicked()
{
}

void te::qt::widgets::DataSetAdapterWidget::onUpToolButtonClicked()
{
}

void te::qt::widgets::DataSetAdapterWidget::onDownToolButtonClicked()
{
}

void te::qt::widgets::DataSetAdapterWidget::onCellClicked(int row, int col)
{
  std::string propName = m_ui->m_tableWidget->item(row, 1)->text().toStdString();

  std::vector<std::string> nap;

  m_dataSetConverter->getNonConvertedProperties(nap);

  for(size_t t = 0; t < nap.size(); ++t)
  {
    if(nap[t] == propName)
    {
      m_ui->m_removeToolButton->setEnabled(false);
      return;
    }
  }

  m_ui->m_removeToolButton->setEnabled(true);
}

void te::qt::widgets::DataSetAdapterWidget::buidTypeMap()
{
   m_typeMap.clear();

   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::ARRAY_TYPE, tr("Array").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::BIT_TYPE, tr("Bit").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::BOOLEAN_TYPE, tr("Boolean").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::BYTE_ARRAY_TYPE, tr("Byte Array").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::CHAR_TYPE, tr("Char").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::COMPOSITE_TYPE, tr("Composite").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::DATASET_TYPE, tr("Data Set").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::DATETIME_TYPE, tr("Date and Time").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::DOUBLE_TYPE, tr("Double").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::FLOAT_TYPE, tr("Float").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::GEOMETRY_TYPE, tr("Geometry").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT16_TYPE, tr("Int 16").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT32_TYPE, tr("Int 32").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT64_TYPE, tr("Int 64").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::NUMERIC_TYPE, tr("Numeric").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::RASTER_TYPE, tr("Raster").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::STRING_TYPE, tr("String").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::UCHAR_TYPE, tr("U Char").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT16_TYPE, tr("U Int 16").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT32_TYPE, tr("U Int 32").toStdString()));
   m_typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT64_TYPE, tr("U Int 64").toStdString()));
}

void te::qt::widgets::DataSetAdapterWidget::fillDataSetTable()
{
  assert(m_dataSetConverter);

  te::da::DataSetType* dsType = m_dataSetConverter->getResult();

  m_ui->m_tableWidget->setRowCount(0);

 //fill table with dataset properties
  for(size_t t = 0; t < dsType->size(); ++t)
  {
    std::string propName = dsType->getProperty(t)->getName();

    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    QTableWidgetItem* itemCheck = new QTableWidgetItem();
    itemCheck->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    itemCheck->setIcon(QIcon::fromTheme("check"));
    m_ui->m_tableWidget->setItem(newrow, 0, itemCheck);

    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(propName));
    itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_ui->m_tableWidget->setItem(newrow, 1, itemName);

    QTableWidgetItem* itemType = new QTableWidgetItem(m_typeMap[dsType->getProperty(t)->getType()].c_str());
    itemType->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_ui->m_tableWidget->setItem(newrow, 2, itemType);
  }

  //fill table with dataset properties that can not be adapted
  std::vector<std::string> nonAdaptedProperties;
  m_dataSetConverter->getNonConvertedProperties(nonAdaptedProperties);

  for(std::size_t i = 0; i < nonAdaptedProperties.size(); ++i)
  {
    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    QTableWidgetItem* itemCheck = new QTableWidgetItem();
    itemCheck->setFlags(Qt::ItemIsEnabled);
    itemCheck->setIcon(QIcon::fromTheme("delete"));
    m_ui->m_tableWidget->setItem(newrow, 0, itemCheck);

    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(nonAdaptedProperties[i]));
    itemName->setFlags(Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(newrow, 1, itemName);
  }
}
