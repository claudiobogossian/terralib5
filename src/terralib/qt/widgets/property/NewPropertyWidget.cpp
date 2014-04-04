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
  \file terralib/qt/widgets/property/NewPropertyWidget.cpp

  \brief A class used to define the NewPropertyWidget class.
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/NumericProperty.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../datatype/StringProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "NewPropertyWidget.h"
#include "SimplePropertyWidget.h"
#include "SimplePropertyWidgetFactory.h"
#include "ui_NewPropertyWidgetForm.h"

// Qt
#include <QMessageBox>


te::qt::widgets::NewPropertyWidget::NewPropertyWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::NewPropertyWidgetForm),
    m_spWidget(0),
    m_simpleProperty(0)
{
  m_ui->setupUi(this);

  //adjust interface
  m_layout = new QGridLayout(m_ui->m_scrollArea);
  m_layout->setAlignment(Qt::AlignTop);

  //connects
  connect(m_ui->m_dataTypeComboBox, SIGNAL(activated(int)), this, SLOT(onDataTypeComboBoxActivated(int)));
}

te::qt::widgets::NewPropertyWidget::~NewPropertyWidget()
{
  delete m_spWidget;
  delete m_simpleProperty;
}

void te::qt::widgets::NewPropertyWidget::setDataSourceId(std::string id)
{
  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(id);

  //clear combobox
  m_ui->m_dataTypeComboBox->clear();
  m_ui->m_dataTypeComboBox->addItem("");

  if(ds.get())
  {
    //fill combobox type with supported types
    const te::da::DataSourceCapabilities& capabilities = ds->getCapabilities();
    const te::da::DataTypeCapabilities& dataTypeCapabilities = capabilities.getDataTypeCapabilities();

    //if(dataTypeCapabilities.supportsArray())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Array"), te::dt::ARRAY_TYPE);

    //if(dataTypeCapabilities.supportsBit())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Bit"), te::dt::BIT_TYPE);

    if(dataTypeCapabilities.supportsBoolean())
      m_ui->m_dataTypeComboBox->addItem(tr("Boolean"), te::dt::BOOLEAN_TYPE);

    //if(dataTypeCapabilities.supportsByteArray())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Byte Array"), te::dt::BYTE_ARRAY_TYPE);

    if(dataTypeCapabilities.supportsChar())
      m_ui->m_dataTypeComboBox->addItem(tr("Char"), te::dt::CHAR_TYPE);

    //if(dataTypeCapabilities.supportsComposite())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Composite"), te::dt::COMPOSITE_TYPE);

    //if(dataTypeCapabilities.supportsDataset())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Data Set"), te::dt::DATASET_TYPE);

    if(dataTypeCapabilities.supportsDateTime())
      m_ui->m_dataTypeComboBox->addItem(tr("Date and Time"), te::dt::DATETIME_TYPE);

    if(dataTypeCapabilities.supportsDouble())
      m_ui->m_dataTypeComboBox->addItem(tr("Double"), te::dt::DOUBLE_TYPE);

    if(dataTypeCapabilities.supportsFloat())
      m_ui->m_dataTypeComboBox->addItem(tr("Float"), te::dt::FLOAT_TYPE);

    if(dataTypeCapabilities.supportsGeometry())
      m_ui->m_dataTypeComboBox->addItem(tr("Geometry"), te::dt::GEOMETRY_TYPE);

    if(dataTypeCapabilities.supportsInt16())
      m_ui->m_dataTypeComboBox->addItem(tr("Int 16"), te::dt::INT16_TYPE);

    if(dataTypeCapabilities.supportsInt32())
      m_ui->m_dataTypeComboBox->addItem(tr("Int 32"), te::dt::INT32_TYPE);

    if(dataTypeCapabilities.supportsInt64())
      m_ui->m_dataTypeComboBox->addItem(tr("Int 64"), te::dt::INT64_TYPE);

    if(dataTypeCapabilities.supportsNumeric())
      m_ui->m_dataTypeComboBox->addItem(tr("Numeric"), te::dt::NUMERIC_TYPE);

    //if(dataTypeCapabilities.supportsRaster())
    //  m_ui->m_dataTypeComboBox->addItem(tr("Raster"), te::dt::RASTER_TYPE);

    if(dataTypeCapabilities.supportsString())
      m_ui->m_dataTypeComboBox->addItem(tr("String"), te::dt::STRING_TYPE);

    if(dataTypeCapabilities.supportsUChar())
      m_ui->m_dataTypeComboBox->addItem(tr("U Char"), te::dt::UCHAR_TYPE);

    if(dataTypeCapabilities.supportsUInt16())
      m_ui->m_dataTypeComboBox->addItem(tr("U Int 16"), te::dt::UINT16_TYPE);

    if(dataTypeCapabilities.supportsUInt32())
      m_ui->m_dataTypeComboBox->addItem(tr("U Int 32"), te::dt::UINT32_TYPE);

    if(dataTypeCapabilities.supportsUInt64())
      m_ui->m_dataTypeComboBox->addItem(tr("U Int 64"), te::dt::UINT64_TYPE);
  }
}

te::dt::SimpleProperty* te::qt::widgets::NewPropertyWidget::getProperty()
{
  if(m_simpleProperty)
    return (te::dt::SimpleProperty*)m_simpleProperty->clone();

  return 0;
}

bool te::qt::widgets::NewPropertyWidget::buildProperty()
{
  //check parameters
  if(m_ui->m_dataTypeComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Type not selected."));
    return false;
  }

  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Name not defined."));
    return false;
  }
  std::string name = m_ui->m_nameLineEdit->text().toStdString();

  std::string defaultValue = "";
  if(m_ui->m_defaultValueLineEdit->text().isEmpty() == false)
  {
    defaultValue = m_ui->m_defaultValueLineEdit->text().toStdString();
  }

  bool isRequired = false;
  if(m_ui->m_requiredCheckBox->isEnabled())
  {
    isRequired = m_ui->m_requiredCheckBox->isChecked();
  }

  bool isAutoNumber = false;
  if(m_ui->m_autoNumberCheckBox->isEnabled())
  {
    isAutoNumber = m_ui->m_autoNumberCheckBox->isChecked();
  }

  //build property
  delete m_simpleProperty;

  int index = m_ui->m_dataTypeComboBox->currentIndex();
  int dataType = m_ui->m_dataTypeComboBox->itemData(index).toInt();

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
      m_simpleProperty = new te::dt::SimpleProperty(name, dataType, isRequired, new std::string(defaultValue));
      m_simpleProperty->setAutoNumber(isAutoNumber);
      break;
    }

    case te::dt::STRING_TYPE:
    {
      m_simpleProperty = new te::dt::StringProperty(name, te::dt::STRING, isRequired, new std::string(defaultValue));
      break;
    }

    case te::dt::NUMERIC_TYPE:
    {
      m_simpleProperty = new te::dt::NumericProperty(name, 0, 0, isRequired, new std::string(defaultValue));
      m_simpleProperty->setAutoNumber(isAutoNumber);
      break;
    }

    case te::dt::DATETIME_TYPE:
    {
      m_simpleProperty = new te::dt::DateTimeProperty(name, te::dt::DATE, te::dt::UNKNOWN, isRequired, new std::string(defaultValue));
      break;
    }
        
    case te::dt::GEOMETRY_TYPE:
    {
      m_simpleProperty = new te::gm::GeometryProperty(name, isRequired, new std::string(defaultValue));
      break;
    }

    default:
    {
      m_simpleProperty = 0;
      return false;
    }
  }
  
   //the custom interface fills the simple property with specific parameters
  if(m_spWidget)
    m_spWidget->fillProperty(m_simpleProperty);

  return true;
}

void te::qt::widgets::NewPropertyWidget::onDataTypeComboBoxActivated(int index)
{
  //create the specific widget
  int dataType = m_ui->m_dataTypeComboBox->itemData(index).toInt();

  delete m_spWidget;

  if(te::qt::widgets::SimplePropertyWidgetFactory::find(dataType))
  {
    m_spWidget = te::qt::widgets::SimplePropertyWidgetFactory::make(dataType, m_ui->m_scrollArea);
    m_layout->addWidget(m_spWidget);
    m_spWidget->show();
  }
  else
  {
    m_spWidget = 0;
  }

  //set interface for this new widget
  m_ui->m_nameLineEdit->clear();

  m_ui->m_defaultValueLineEdit->clear();
  m_ui->m_defaultValueLineEdit->setEnabled(false);

  m_ui->m_autoNumberCheckBox->setChecked(false);
  m_ui->m_autoNumberCheckBox->setEnabled(false);

  m_ui->m_requiredCheckBox->setChecked(false);
  m_ui->m_requiredCheckBox->setEnabled(false);

  if(m_spWidget)
  {
    m_ui->m_defaultValueLineEdit->setEnabled(m_spWidget->hasAttributeDefaultValue());
    m_ui->m_autoNumberCheckBox->setEnabled(m_spWidget->hasAttributeAutoNumber());
    m_ui->m_requiredCheckBox->setEnabled(m_spWidget->hasAttributeRequired());
  }
}
