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
  \file  terralib/qt/widgets/externalTable/DataPropertiesWidget.h

  \brief A class used to configure the properties of a new textual file based layer
*/

//Terralib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/Property.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/NumericProperty.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../datatype/StringProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../memory/DataSet.h"
#include "../property/DataSetAdapterWidget.h"
#include "../srs/SRSManagerDialog.h"
#include "../table/DataSetTableView.h"
#include "../Utils.h"
#include "DataPropertiesWidget.h"
#include "ui_DataPropertiesWidgetForm.h"

// Qt
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//Utility functions used mainly to populate ui elements.
void buidTypeMap(std::map<int, std::string>& typeMap)
{
  typeMap.clear();

  typeMap.insert(std::map<int, std::string>::value_type(te::dt::ARRAY_TYPE, QObject::tr("Array").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::BYTE_ARRAY_TYPE, QObject::tr("Byte Array").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::DATETIME_TYPE, QObject::tr("Date and Time").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::DOUBLE_TYPE, QObject::tr("Double").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::GEOMETRY_TYPE, QObject::tr("Geometry").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT32_TYPE, QObject::tr("Int 32").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::INT64_TYPE, QObject::tr("Int 64").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::NUMERIC_TYPE, QObject::tr("Numeric").toStdString()));
  typeMap.insert(std::map<int, std::string>::value_type(te::dt::STRING_TYPE, QObject::tr("String").toStdString()));
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
      newProperty = new te::dt::StringProperty(name, te::dt::STRING, 0, isRequired, new std::string(defaultValue));
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
      newProperty = new te::dt::DateTimeProperty(name, te::dt::DATE, isRequired, new std::string(defaultValue));
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
      return nullptr;
    }
  }
  return newProperty;
}

void fillComboBox(std::map<int, std::string> typeMap, QComboBox* box)
{
  for(std::map<int, std::string>::iterator it = typeMap.begin(); it !=  typeMap.end(); ++it)
  {
    box->addItem(QString::fromStdString(it->second), QVariant(it->first));
  }
}

te::qt::widgets::DatapPropertiesWidget::DatapPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DataPropertiesWidgetForm)
{
  m_ui->setupUi(this);

  //preparing the Type Map
  buidTypeMap(m_typeMap);

  m_mapper = new QSignalMapper(this);

  //Adjusting the dataSetTableView that will be used to display the dataset's data
  m_tblView.reset(new DataSetTableView(m_ui->m_dataPreviewFrame));
  QGridLayout* dataPreviewLayout = new QGridLayout(m_ui->m_dataPreviewFrame);
  dataPreviewLayout->addWidget(m_tblView.get());
  dataPreviewLayout->setContentsMargins(0, 0, 0, 0);

  m_tblView->setAlternatingRowColors(true);
  m_tblView->verticalHeader()->setVisible(false);
  m_tblView->setSelectionMode(QAbstractItemView::NoSelection);

  m_ui->m_dataPropertiesTableWidget->setSelectionMode(QAbstractItemView::NoSelection);

  //add icon
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("tabular-import-hint").pixmap(112,48));
  m_ui->m_sridPushButton->setIcon(QIcon::fromTheme("srs"));

  //Connecting signals and slots
  connect(m_ui->m_inputDataToolButton, SIGNAL(clicked()), this, SLOT(onInputDataToolButtonTriggered()));
  connect(m_ui->m_sridPushButton, SIGNAL(clicked()), this, SLOT(onSridPushButtonCLicked()));
  connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(onPropertyTypeChanged(int)));
}

te::qt::widgets::DatapPropertiesWidget::~DatapPropertiesWidget()
{
  m_typeMap.clear();
}

Ui::DataPropertiesWidgetForm* te::qt::widgets::DatapPropertiesWidget::getForm() const
{
  return m_ui.get();
}

std::auto_ptr<te::da::DataSetTypeConverter> te::qt::widgets::DatapPropertiesWidget::getConverter()
{
  //Searching for properties that the user selected to adapt
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_dsConverter->getResult());
  te::da::PrimaryKey* pkIn = m_dataType->getPrimaryKey();
  for (int i = 0; i < m_ui->m_dataPropertiesTableWidget->rowCount(); ++i)
  {
    if(dynamic_cast<QCheckBox*>(m_ui->m_dataPropertiesTableWidget->cellWidget(i, 0))->isChecked())
    {
      QComboBox* box = dynamic_cast<QComboBox*>(m_ui->m_dataPropertiesTableWidget->cellWidget(i, 1));
      int type = box->itemData(box->currentIndex()).toInt();

      m_dsConverter->add(m_dataType->getProperty(i)->getName(), getConvertedproperty(m_dataType->getProperty(i)->getName(), type));

      if(pkIn)
      {
        std::vector<te::dt::Property*> props = pkIn->getProperties();

        for(std::size_t t= 0; t < props.size(); ++t)
        {
          if(props[t]->getName() == m_dataType->getProperty(i)->getName())
          {
            te::dt::SimpleProperty* sp = getConvertedproperty(m_dataType->getProperty(i)->getName(), type);
            pk->add(sp);

            break;
          }
        }
      }
    }
  }

  //Configuring a new geometry if the user requested it.
  if(m_ui->m_geometryGroupBox->isCheckable() && m_ui->m_geometryGroupBox->isChecked())
  {
    te::gm::GeometryProperty* newGeom;

    std::vector<std::string> names;
    names.push_back(m_ui->m_xAxisComboBox->currentText().toStdString());
    names.push_back(m_ui->m_yAxisComboBox->currentText().toStdString());
    newGeom = new te::gm::GeometryProperty("OGR_POINT", true, new std::string());
    newGeom->setGeometryType(te::gm::PointType);

    m_dsConverter->add(names, newGeom, "XYToPointConverter");
  }

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_dsConverter->getResult());

  if(gp && !m_ui->m_sridLineEdit->text().isEmpty())
    gp->setSRID(boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString()));

  return m_dsConverter;
}

te::da::DataSetType* te::qt::widgets::DatapPropertiesWidget::getDataSetType()
{
  return m_dataType.get();
}

te::da::DataSource* te::qt::widgets::DatapPropertiesWidget::getDataSource()
{
  return m_dataSource.get();
}

void te::qt::widgets::DatapPropertiesWidget::onInputDataToolButtonTriggered()
{
  try
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Textual File"), te::qt::widgets::GetFilePathFromSettings("tabular"), tr("Comma Separated Value (*.csv *.CSV);; dBASE (*.dbf *.DBF)"), 
      0, QFileDialog::ReadOnly);

    if(fileName.isEmpty())
      return;

    QFileInfo info(fileName);

    te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "tabular");

    //Getting the connection info
    std::string ogrInfo("connection_string=" + fileName.toStdString());
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = fileName.toStdString();

    boost::filesystem::path uri(fileName.toStdString());
    std::string file = uri.stem().string();

    //Creating a DataSource
    static boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();

    te::da::DataSourceInfoPtr dsInfo(new te::da::DataSourceInfo);
    dsInfo->setConnInfo(connInfo);
    dsInfo->setId(boost::uuids::to_string(u));
    dsInfo->setTitle(fileName.toStdString());
    dsInfo->setDescription("");
    dsInfo->setAccessDriver("OGR");
    dsInfo->setType("OGR");

    m_dataSource = te::da::DataSourceFactory::make(dsInfo->getAccessDriver());
    m_dataSource->setConnectionInfo(dsInfo->getConnInfo());

    if(te::da::DataSourceInfoManager::getInstance().add(dsInfo))
    {
      m_dataSource->setId(boost::uuids::to_string(u));
      te::da::DataSourceManager::getInstance().insert(m_dataSource);
    }
    else
    {
      dsInfo = te::da::DataSourceInfoManager::getInstance().getByConnInfo(dsInfo->getConnInfoAsString());
      m_dataSource->setId(dsInfo->getId());
    }

    m_dataSource->open();

    //Creating the DataSet and DataType
    std::auto_ptr<te::da::DataSet> dataset = m_dataSource->getDataSet(file);
    std::vector<std::string> datasetNames = m_dataSource->getDataSetNames();

    if(!datasetNames.empty())
      m_dataType = m_dataSource->getDataSetType(datasetNames[0]);
    else
    {
      QMessageBox::critical( this, tr("Error"),tr("The file could not be read!"));
      return;
    }

    m_ui->m_inputDataLineEdit->setText(fileName);

    //Creating the DataSetConverter 
    m_dsConverter.reset(new te::da::DataSetTypeConverter(m_dataType.get()));

    //Filling the data preview table
    std::vector<std::size_t> properties;
    for (std::size_t i = 0; i < dataset->getNumProperties(); ++i)
      properties.push_back(i);

    //The table will display 5 rows of the data for previewing purposes
    std::auto_ptr<te::mem::DataSet> memFeature((new te::mem::DataSet(*dataset.get(), properties, 5)));

    m_tblView->setDataSet(memFeature.release(), m_dataSource->getEncoding());
    m_tblView->resizeColumnsToContents();
    m_tblView->show();

    m_ui->m_dataPropertiesTableWidget->setRowCount(0);

    //Filling the properties table widget
    for(size_t t = 0; t < m_dataType->size(); ++t)
    {
      int newrow = m_ui->m_dataPropertiesTableWidget->rowCount();
      m_ui->m_dataPropertiesTableWidget->insertRow(newrow);

      //The Property name item
      std::string propName = m_dataType->getProperty(t)->getName();

      //A checkbox used to know if the user wants to import that row's property
      QCheckBox* impCheck = new QCheckBox();
      impCheck->setText(QString::fromStdString(propName));
      impCheck->setCheckState(Qt::Checked);

      if(m_dataType->getProperty(t)->getType() == te::dt::GEOMETRY_TYPE)
        connect(impCheck, SIGNAL(stateChanged (int )), this, SLOT(onGeomPropertyChecked(int)));

      m_ui->m_dataPropertiesTableWidget->setCellWidget(newrow, 0, impCheck);

      //The property type item
      QComboBox* typeCB = new QComboBox();
      fillComboBox(m_typeMap, typeCB);
      typeCB->setCurrentIndex(typeCB->findText(m_typeMap[m_dataType->getProperty(t)->getType()].c_str()));

      m_mapper->setMapping(typeCB, newrow);
      connect(typeCB, SIGNAL(currentIndexChanged (int)), m_mapper, SLOT(map()));

      m_ui->m_dataPropertiesTableWidget->setCellWidget(newrow, 1, typeCB);

      //check geom
      if(m_dataType->hasGeom())
      {
        te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_dataType.get());

        if(gp && gp->getName() == propName)
        {
          QString srid;
          srid.setNum(gp->getSRID());
          m_ui->m_sridLineEdit->setText(srid);

          typeCB->setEnabled(false);
          break;
        }
      }

      //check pk
      te::da::PrimaryKey* pk = m_dataType->getPrimaryKey();

      if(pk)
      {
        std::vector<te::dt::Property*> props = pk->getProperties();

        for(std::size_t t= 0; t < props.size(); ++t)
        {
          if(props[t]->getName() == propName)
          {
            typeCB->setEnabled(false);
            impCheck->setEnabled(false);
            break;
          }
        }
      }
    }

    m_ui->m_dataPropertiesTableWidget->resizeColumnsToContents();
    m_ui->m_dataPropertiesTableWidget->resizeRowsToContents();
#if (QT_VERSION >= 0x050000)
    m_ui->m_dataPropertiesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    m_ui->m_dataPropertiesTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    //Clearing the Comboxes if they aren't empty
    m_ui->m_xAxisComboBox->clear();
    m_ui->m_yAxisComboBox->clear();

    if(m_dataType->hasGeom())
    {
      m_ui->m_geometryGroupBox->setCheckable(false);
      m_ui->m_xAxisComboBox->setEnabled(false);
      m_ui->m_yAxisComboBox->setEnabled(false);
      m_ui->m_geometryGroupBox->setChecked(true);
    }
    else
    {
      m_ui->m_geometryGroupBox->setCheckable(true);
      m_ui->m_xAxisComboBox->setEnabled(true);
      m_ui->m_yAxisComboBox->setEnabled(true);
      m_ui->m_geometryGroupBox->setChecked(false);
    }

    //Filling the ComboBoxes that will be used to configure the resulting geometries
    for(size_t t = 0; t < m_dataType->size(); ++t)
    {
      std::string propName = m_dataType->getProperty(t)->getName();
      int type = m_dataType->getProperty(t)->getType();
      if((type >= te::dt::INT16_TYPE && type <= te::dt::UINT64_TYPE) || 
                type == te::dt::FLOAT_TYPE || type == te::dt::DOUBLE_TYPE)
      {
        m_ui->m_xAxisComboBox->addItem(QString::fromStdString(propName));
        m_ui->m_yAxisComboBox->addItem(QString::fromStdString(propName));
      }
    }
    emit itemChanged();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Tabular File"), e.what());
  }
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

void te::qt::widgets::DatapPropertiesWidget::onGeomPropertyChecked(int checked)
{
  if(checked == Qt::Unchecked)
  {
    m_ui->m_geometryGroupBox->setCheckable(true);
    m_ui->m_xAxisComboBox->setEnabled(true);
    m_ui->m_yAxisComboBox->setEnabled(true);
    m_ui->m_geometryGroupBox->setChecked(true);
  }
  else
  {
    m_ui->m_geometryGroupBox->setCheckable(false);
    m_ui->m_xAxisComboBox->setEnabled(false);
    m_ui->m_yAxisComboBox->setEnabled(false);
    m_ui->m_geometryGroupBox->setChecked(false);
  }
}

void te::qt::widgets::DatapPropertiesWidget::onPropertyTypeChanged(int row)
{
  //Acquiring the name of the cconfigured property and it's new type.
  QCheckBox* check = dynamic_cast<QCheckBox*>(m_ui->m_dataPropertiesTableWidget->cellWidget(row, 0));
  QComboBox* box = dynamic_cast<QComboBox*>(m_ui->m_dataPropertiesTableWidget->cellWidget(row, 1));
  std::string propName = check->text().toStdString();
  int type = box->itemData(box->currentIndex()).toInt();

  //Searching the property to see if it is already in the comboBoxes
  int xyAxis = m_ui->m_xAxisComboBox->findText(QString::fromStdString(propName));

  //Checking wheather the property needs to be added to or removed from the xAxisoOmboBox and yAxisoCOmboBox.
  //Their values will always be the same.
  if(xyAxis == -1)
  {
    if((type >= te::dt::INT16_TYPE && type <= te::dt::UINT64_TYPE) || type == te::dt::FLOAT_TYPE || type == te::dt::DOUBLE_TYPE)
    {
      m_ui->m_xAxisComboBox->addItem(QString::fromStdString(propName));
      m_ui->m_yAxisComboBox->addItem(QString::fromStdString(propName));
    }
  }
  else
  {
    if((type <= te::dt::INT16_TYPE || type >= te::dt::UINT64_TYPE) && type != te::dt::FLOAT_TYPE && type != te::dt::DOUBLE_TYPE)
    {
      m_ui->m_xAxisComboBox->removeItem(xyAxis);
      m_ui->m_yAxisComboBox->removeItem(xyAxis);
    }
  }
}
