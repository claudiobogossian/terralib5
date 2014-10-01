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
  \file terralib/qt/widgets/property/PropertyConverterDialog.cpp

  \brief A class used to define the PropertyConverterDialog class.
*/

// TerraLib
#include "../../../dataaccess/dataset/AttributeConverterManager.h"
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceCatalog.h"
#include "NewPropertyWidget.h"
#include "PropertyConverterDialog.h"
#include "ui_NewPropertyWidgetForm.h"
#include "ui_PropertyConverterDialogForm.h"

// Qt
#include <QMessageBox>
#include <QGridLayout>


te::qt::widgets::PropertyConverterDialog::PropertyConverterDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PropertyConverterDialogForm),
    m_propWidget(0),
    m_dsType(0)
{
  m_ui->setupUi(this);

  buidTypeMap();

  //build form
  m_propWidget = new te::qt::widgets::NewPropertyWidget(m_ui->m_widget);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  layout->addWidget(m_propWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  //get converters types
  std::vector<std::string> converters;
  te::da::AttributeConverterManager::getInstance().getConverters(converters);

  for(std::size_t t = 0; t < converters.size(); ++t)
  {
    m_ui->m_comboBox->addItem(converters[t].c_str());
  }

  m_ui->m_helpPushButton->setPageReference("widgets/property/property_converter.html");
}

te::qt::widgets::PropertyConverterDialog::~PropertyConverterDialog()
{
  m_typeMap.clear();
}

void te::qt::widgets::PropertyConverterDialog::set(te::da::DataSetType* dsType)
{
  assert(dsType);

  m_dsType = dsType;

  //fill table with dataset properties
  for(size_t t = 0; t < m_dsType->size(); ++t)
  {
    std::string propName = m_dsType->getProperty(t)->getName();

    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);

    QTableWidgetItem* itemCheck = new QTableWidgetItem();
    itemCheck->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    itemCheck->setCheckState(Qt::Unchecked);
    m_ui->m_tableWidget->setItem(newrow, 0, itemCheck);

    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(propName));
    itemName->setFlags(Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(newrow, 1, itemName);

    QTableWidgetItem* itemType = new QTableWidgetItem(m_typeMap[m_dsType->getProperty(t)->getType()].c_str());
    itemType->setFlags(Qt::ItemIsEnabled);
    m_ui->m_tableWidget->setItem(newrow, 2, itemType);
  }

  m_ui->m_tableWidget->resizeColumnsToContents();
}

void te::qt::widgets::PropertyConverterDialog::set(std::string dataSourceId)
{
  m_propWidget->setDataSourceId(dataSourceId);
}

void te::qt::widgets::PropertyConverterDialog::adapt(te::da::DataSetTypeConverter* converter)
{
  te::dt::SimpleProperty* p = m_propWidget->getProperty();
  std::vector<std::string> names;
  std::string converterName = m_ui->m_comboBox->currentText().toStdString();

  //get selected attributes
  int row = m_ui->m_tableWidget->rowCount();

  for(int i = 0; i < row; ++i)
  {
    QTableWidgetItem* item = m_ui->m_tableWidget->item(i, 0);

    if(item && item->checkState() == Qt::Checked)
    {
      names.push_back(m_ui->m_tableWidget->item(i, 1)->text().toStdString());
    }
  }

  //adapt
  converter->add(names, (te::dt::Property*)p, converterName);
}

void te::qt::widgets::PropertyConverterDialog::onOkPushButtonClicked()
{
  if(m_propWidget->buildProperty() == false)
  {
    return;
  }

  accept();
}

void te::qt::widgets::PropertyConverterDialog::buidTypeMap()
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
