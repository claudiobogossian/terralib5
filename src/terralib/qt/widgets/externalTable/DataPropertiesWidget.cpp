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
  \file  terralib/qt/widgets/externalTable/DataPropertiesWidget.h

  \brief A class used to configure the properties of a new textual file based layer
*/

//Terralib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/Property.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/NumericProperty.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../datatype/StringProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../memory/DataSet.h"
#include "../table/DataSetTableView.h"
#include "SRSManagerDialog.h"
#include "DataPropertiesWidget.h"
#include "DataSetAdapterWidget.h"
#include "ui_DataPropertiesWidgetForm.h"

// Qt
#include <QtCore/QFileInfo>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

//Utility functions used mianly to pupulate ui elements.
void buidTypeMap(std::map<int, std::string>& typeMap)
{
   typeMap.clear();

   typeMap.insert(std::map<int, std::string>::value_type(te::dt::ARRAY_TYPE, QObject::tr("Array").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::BIT_TYPE, QObject::tr("Bit").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::BOOLEAN_TYPE, QObject::tr("Boolean").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::BYTE_ARRAY_TYPE, QObject::tr("Byte Array").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::CHAR_TYPE, QObject::tr("Char").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::COMPOSITE_TYPE, QObject::tr("Composite").toStdString()));
   //typeMap.insert(std::map<int, std::string>::value_type(te::dt::DATASET_TYPE, QObject::tr("Data Set").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::DATETIME_TYPE, QObject::tr("Date and Time").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::DOUBLE_TYPE, QObject::tr("Double").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::FLOAT_TYPE, QObject::tr("Float").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::GEOMETRY_TYPE, QObject::tr("Geometry").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT16_TYPE, QObject::tr("Int 16").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT32_TYPE, QObject::tr("Int 32").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT64_TYPE, QObject::tr("Int 64").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::NUMERIC_TYPE, QObject::tr("Numeric").toStdString()));
   //typeMap.insert(std::map<int, std::string>::value_type(te::dt::RASTER_TYPE, QObject::tr("Raster").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::STRING_TYPE, QObject::tr("String").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::UCHAR_TYPE, QObject::tr("U Char").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT16_TYPE, QObject::tr("U Int 16").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT32_TYPE, QObject::tr("U Int 32").toStdString()));
   typeMap.insert(std::map<int, std::string>::value_type(te::dt::UINT64_TYPE, QObject::tr("U Int 64").toStdString()));
}

void fillComboBox(std::map<int, std::string> typeMap, QComboBox* box)
{
  for(std::map<int, std::string>::iterator it = typeMap.begin(); it !=  typeMap.end(); ++it)
  {
    std::string propName = it->second;
    box->addItem(QString::fromStdString(propName));
  }
}

void fillComboBox(const te::da::DataSetType* dsType, QComboBox* box)
{
  for(size_t t = 0; t < dsType->size(); ++t)
  {
    std::string propName = dsType->getProperty(t)->getName();
    box->addItem(QString::fromStdString(propName));
  }
}

te::dt::SimpleProperty* getConvertedproperty(std::string name, int dataType, std::string defaultValue = "", bool isRequired = false, bool isAutoNumber = true)
{
  te::dt::SimpleProperty* newProperty;

  switch(dataType)
  {
    case te::dt::BOOLEAN_TYPE:
    case te::dt::CHAR_TYPE:
    case te::dt::DOUBLE_TYPE:
    case te::dt::FLOAT_TYPE:
    case te::dt::INT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::INT64_TYPE:
    case te::dt::UCHAR_TYPE:
    case te::dt::UINT16_TYPE:
    case te::dt::UINT32_TYPE:
    case te::dt::UINT64_TYPE:
    {
      newProperty = new te::dt::SimpleProperty(name, dataType, isRequired, new std::string(defaultValue));
      newProperty->setAutoNumber(isAutoNumber);
      break;
    }

    case te::dt::STRING_TYPE:
    {
      newProperty = new te::dt::StringProperty(name, te::dt::STRING, isRequired, new std::string(defaultValue));
      break;
    }

    case te::dt::NUMERIC_TYPE:
    {
      newProperty = new te::dt::NumericProperty(name, 0, 0, isRequired, new std::string(defaultValue));
      newProperty->setAutoNumber(isAutoNumber);
      break;
    }

    case te::dt::DATETIME_TYPE:
    {
      newProperty = new te::dt::DateTimeProperty(name, te::dt::DATE, te::dt::UNKNOWN, isRequired, new std::string(defaultValue));
      break;
    }
        
    case te::dt::GEOMETRY_TYPE:
    {
      newProperty = new te::gm::GeometryProperty(name, isRequired, new std::string(defaultValue));
      break;
    }

    default:
    {
      newProperty = 0;
      return false;
    }
  }
  return newProperty;
}

te::qt::widgets::DatapPropertiesWidget::DatapPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataPropertiesWidgetForm)
{
  m_ui->setupUi(this);

  //preparing the Type Map
  buidTypeMap(m_typeMap);

  //Adjusting the dataSetTableView that will be used to display the dataset's data
  m_tblView.reset(new DataSetTableView(m_ui->m_dataPreviewFrame));
  QGridLayout* dataPreviewLayout = new QGridLayout(m_ui->m_dataPreviewFrame);
  dataPreviewLayout->addWidget(m_tblView.get());
  dataPreviewLayout->setContentsMargins(0, 0, 0, 0);

  m_tblView->setAlternatingRowColors(true);
  m_tblView->verticalHeader()->setVisible(false);
  m_tblView->setSelectionMode(QAbstractItemView::NoSelection);

  m_ui->m_datapropertiesTableWidget->setSelectionMode(QAbstractItemView::NoSelection);

  //Connecting signals and slots
  connect(m_ui->m_inputDataToolButton, SIGNAL(clicked()), this, SLOT(onInputDataToolButtonTriggered()));
  connect(m_ui->m_sridPushButton, SIGNAL(clicked()), this, SLOT(onSridPushButtonCLicked()));
}

te::qt::widgets::DatapPropertiesWidget::~DatapPropertiesWidget()
{
  m_typeMap.clear();
}

std::auto_ptr<te::da::DataSetAdapter> te::qt::widgets::DatapPropertiesWidget::getAdapter()
{
  //Searching for properties that the user selected to adapt
  for (int i = 0; i < m_ui->m_datapropertiesTableWidget->rowCount(); ++i)
  {
    if(m_typeMap[m_dataType->getProperty(i)->getType()].c_str() != dynamic_cast<QComboBox*>(m_ui->m_datapropertiesTableWidget->cellWidget(i, 1))->currentText().toStdString())
    {
      m_dsConverter->add(m_dataType->getProperty(i)->getName(), getConvertedproperty(m_dataType->getProperty(i)->getName(), m_dataType->getProperty(i)->getType()));
    }
  }

  //Configuring a new geometry if the user requested it.
  if(m_ui->m_geometryGroupBox->isChecked())
  {
    te::gm::GeometryProperty* newGeom;

    if(m_ui->m_pointRadioButton->isChecked())
    {
      std::vector<std::string> names;
      names.push_back(m_ui->m_xAxisComboBox->currentText().toStdString());
      names.push_back(m_ui->m_yAxisComboBox->currentText().toStdString());
      newGeom = new te::gm::GeometryProperty("Point", true, new std::string());
      newGeom->setSRID(boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString()));
      m_dsConverter->add(names, newGeom, te::da::XYToPointConverter);
    }
    else if(m_ui->m_wktRadioButton->isChecked())
    {
      //std::string name = m_ui->m_wktComboBox->currentText().toStdString();
      //newGeom = new te::gm::GeometryProperty(name, true, new std::string());
      //newGeom->setSRID(boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString()));
      //m_dsConverter->add(name, newGeom);
    }
  }

  std::auto_ptr<te::da::DataSetAdapter> adapter(te::da::CreateAdapter(m_dataSet.release(), m_dsConverter, true));
  return adapter;
}

void te::qt::widgets::DatapPropertiesWidget::onInputDataToolButtonTriggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Textual File"), "", tr("Comma Separated Value (*.csv *.CSV);; dBASE (*.dbf *.dbf)"), 
    0, QFileDialog::ReadOnly);

  if(fileName.isEmpty())
    return;

  m_ui->m_inputDataLineEdit->setText(fileName);

  //Getting the connection info
  std::string ogrInfo("connection_string=" + fileName.toStdString());
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName.toStdString();

  boost::filesystem::path uri(m_ui->m_inputDataLineEdit->text().toStdString());
  std::string file = uri.stem().string();

  //Creating a DataSource
  m_dataSource = te::da::DataSourceFactory::make("OGR");
  m_dataSource->setConnectionInfo(connInfo);
  m_dataSource->open();

  //Creating the DataSet and DataType
  m_dataSet = m_dataSource->getDataSet(file);
  std::vector<std::string> datasetNames = m_dataSource->getDataSetNames();
  m_dataType = m_dataSource->getDataSetType(datasetNames[0]);

  //Creating the DataSetConverter 
  m_dsConverter = new te::da::DataSetTypeConverter(m_dataType.get(), m_dataSource->getCapabilities());

  //Filling the data preview table
  std::vector<std::size_t> properties;
  for(std::size_t i = 0; i < m_dataSet->getNumProperties(); ++i)
    properties.push_back(i);

  //The table will display 5 rows of the data for previewing purposes
  std::auto_ptr<te::mem::DataSet> memFeature((new te::mem::DataSet(*m_dataSet.get(), properties, 5)));

  m_tblView->setDataSet(memFeature.release());
  m_tblView->resizeColumnsToContents();
  m_tblView->show();

  m_ui->m_datapropertiesTableWidget->setRowCount(0);

  //Filling the properties table widget
  for(size_t t = 0; t < m_dataType->size(); ++t)
  {
    int newrow = m_ui->m_datapropertiesTableWidget->rowCount();
    m_ui->m_datapropertiesTableWidget->insertRow(newrow);

    //The Property name item
    std::string propName = m_dataType->getProperty(t)->getName();

    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(propName));
    itemName->setFlags(Qt::ItemIsEnabled);

    m_ui->m_datapropertiesTableWidget->setItem(newrow, 0, itemName);

    //The property type item
    QComboBox* typeCB = new QComboBox();
    fillComboBox(m_typeMap, typeCB);
    typeCB->setCurrentIndex(typeCB->findText(m_typeMap[m_dataType->getProperty(t)->getType()].c_str()));

    m_ui->m_datapropertiesTableWidget->setCellWidget(newrow, 1, typeCB);
  }

  m_ui->m_datapropertiesTableWidget->resizeColumnsToContents();
  m_ui->m_datapropertiesTableWidget->resizeRowsToContents();
  m_ui->m_datapropertiesTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

  //Filling the ComboBoxes that will be used to configure the resulting geometries
  fillComboBox(m_dataType.get(), m_ui->m_xAxisComboBox);
  fillComboBox(m_dataType.get(), m_ui->m_yAxisComboBox);
  fillComboBox(m_dataType.get(), m_ui->m_wktComboBox);
}

void te::qt::widgets::DatapPropertiesWidget::onSridPushButtonCLicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if(srsDialog.exec() != QDialog::Rejected)
  {
    std::pair<int, std::string> srid = srsDialog.getSelectedSRS();
    m_ui->m_sridLineEdit->setText(QString::number(srid.first));
  }
}
