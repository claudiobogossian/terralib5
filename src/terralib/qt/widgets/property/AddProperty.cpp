/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include "AddProperty.h"
#include "../../../dataaccess.h"
#include "../../../datatype.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../raster/Grid.h"
#include "../../../raster/BandProperty.h"
#include "../../../raster/RasterProperty.h"

// Qt
#include <QtGui/QtGui>
#include <QtGui/QComboBox>
#include <QtGui/QMessageBox>

te::qt::widgets::AddProperty::AddProperty(te::da::DataSource* ds, QWidget* parent)
  : QDialog(parent), m_ds(ds), m_transactor(0), m_catalogLoader(0), m_property(0), m_defaultValue(0),
    m_propertyParent(0)
{
  if(m_ds == 0)
    QMessageBox::critical(this, tr("Missing a Valid Data Source"), tr("Provide a valid data source!"));

  setupUi(this);

  layout()->setSizeConstraint(QLayout::SetFixedSize);

  // Get a data source catalog loader to access the datasource catalog
  m_transactor = m_ds->getTransactor();
  m_catalogLoader = m_transactor->getCatalogLoader();
    
  // Load the catalog to find out the information in the data source (only the schemas)
  m_catalogLoader->loadCatalog();

  // Get the dataset names of the data source
  boost::ptr_vector<std::string> datasets;
  m_catalogLoader->getDataSets(datasets);

  // Fill alphabetically the dataSetCombobox with the dataset names of the data source
  QStringList dataSetList;

  size_t numDataSets = datasets.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (datasets[i]).c_str();

  dataSetList.sort();
  dataSetComboBox->addItems(dataSetList);

  // Fill the dataTypeComboBox with the data types available
  // according the data source capabilities
  std::map<std::string, std::string> capabilities; 
  m_ds->getCapabilities(capabilities);
  
  std::map<std::string, std::string>::iterator it;

  it = capabilities.find("ARRAY_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("ARRAY");

  it = capabilities.find("BIT_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("BIT");

  it = capabilities.find("BOOLEAN_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("BOOLEAN");

  it = capabilities.find("BYTE_ARRAY_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("BYTE_ARRAY");

  it = capabilities.find("CHAR_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("CHAR");

  it = capabilities.find("COMPOSITE_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("COMPOSITE");

  it = capabilities.find("DATETIME_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("DATETIME");

  it = capabilities.find("DOUBLE_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("DOUBLE");

  it = capabilities.find("FLOAT_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("FLOAT");

  it = capabilities.find("GEOMETRY_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("GEOMETRY");

  it = capabilities.find("INT16_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("INT16");

  it = capabilities.find("INT32_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("INT32");

  it = capabilities.find("INT64_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("INT64");

  it = capabilities.find("NUMERIC_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("NUMERIC");

  it = capabilities.find("RASTER_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("RASTER");

  it = capabilities.find("STRING_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("STRING");

  it = capabilities.find("UCHAR_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("UCHAR");

  it = capabilities.find("UINT16_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("UINT16");

  it = capabilities.find("UINT32_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("UINT32");

  it = capabilities.find("UINT64_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("UINT64");

  it = capabilities.find("UNKNOWN_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("UNKNOWN");

  it = capabilities.find("VOID_DT");
  if(it != capabilities.end() && it->second == "TRUE")
    dataTypeComboBox->addItem("VOID");

  // Connect the signals/slots
  connect(dataSetComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataSetComboBoxChanged(const QString&)));
  connect(autoNumberCheckBox, SIGNAL(clicked(bool)), this, SLOT(autoNumberCheckBoxClicked(bool)));
  connect(dataTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataTypeComboBoxChanged(const QString&)));
  connect(requiredCheckBox, SIGNAL(clicked(bool)), this, SLOT(requiredCheckBoxClicked(bool)));
  connect(arrayElementDataTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(arrayElementDataTypeComboBoxChanged(const QString&)));
  connect(stringTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(stringTypeComboBoxChanged(const QString&)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  dataSetComboBoxChanged(dataSetComboBox->currentText());
  dataTypeComboBoxChanged(dataTypeComboBox->currentText());
}

te::qt::widgets::AddProperty::~AddProperty()
{
  // Release the transactor
  if (m_transactor)
   delete m_transactor;
}

void te::qt::widgets::AddProperty::dataSetComboBoxChanged(const QString& dataSetName)
{
  propertiesComboBox->clear();

  // Get the DataSetType associated to the dataset selected; 
  // it will be used as the parent of the property to be added.
  if(m_propertyParent)
    delete m_propertyParent;

  if(dataSetComboBox->currentText().isEmpty() == false)
    m_propertyParent = m_catalogLoader->getDataSetType(dataSetName.toStdString(), false);

  // Fill the propertiesComboBox with the names of the properties of the dataset selected
  size_t numProperties = m_propertyParent->size();

  for (size_t i = 0; i < numProperties; ++i)
    propertiesComboBox->addItem(m_propertyParent->getProperty(i)->getName().c_str());
}

void te::qt::widgets::AddProperty::dataTypeComboBoxChanged(const QString& dataType)
{
  autoNumberCheckBox->setEnabled(false);
  autoNumberCheckBox->setChecked(false);

  requiredCheckBox->setEnabled(true);
  requiredCheckBox->setChecked(false);

  defaultValueLineEdit->clear();
  defaultValueLineEdit->setEnabled(false);

  dataTypeStackedWidget->setVisible(true);

  if(dataType == "ARRAY")
    setArrayPropertyParams();
  else if(dataType == "COMPOSITE")
  {
    requiredCheckBox->setEnabled(false);
    dataTypeStackedWidget->setCurrentWidget(compositePage);
  }
  else if(dataType == "DATETIME")
    setDateTimePropertyParams();
  else if(dataType == "GEOMETRY")
    setGeometryPropertyParams();
  else if(dataType == "NUMERIC")
    dataTypeStackedWidget->setCurrentWidget(numericPage);
  else if(dataType == "RASTER")
  {
    requiredCheckBox->setEnabled(false);
    dataTypeStackedWidget->setCurrentWidget(rasterPage);
  }
  else if(dataType == "STRING")
    setStringPropertyParams(); 
  else
  {
    dataTypeStackedWidget->setVisible(false);

    if(dataType == "INT32" || dataType == "INT64")
      autoNumberCheckBox->setEnabled(true);
  }
}

void te::qt::widgets::AddProperty::autoNumberCheckBoxClicked(bool checked)
{
  requiredCheckBox->setEnabled(true);
  defaultValueLineEdit->setEnabled(true);
  requiredCheckBox->setChecked(false);
  defaultValueLineEdit->clear();

  if(checked)
  {
    requiredCheckBox->setEnabled(false);
    defaultValueLineEdit->setEnabled(false);
    requiredCheckBox->setChecked(true);
  }
}

void te::qt::widgets::AddProperty::requiredCheckBoxClicked(bool checked)
{
  defaultValueLineEdit->clear();
  if(checked == true)
    defaultValueLineEdit->setEnabled(true);
  else
    defaultValueLineEdit->setEnabled(false);
}

void te::qt::widgets::AddProperty::arrayElementDataTypeComboBoxChanged(const QString& arrayElementDataType)
{
  if(arrayElementDataType == "NUMERIC")
  {
    arrayElementStackedWidget->setVisible(true);
    arrayElementStackedWidget->setCurrentWidget(arrayElementNumericPage);
  }
  else if(arrayElementDataType == "STRING")
  {
    arrayElementStackedWidget->setVisible(true);
    arrayElementStackedWidget->setCurrentWidget(arrayElementStringPage);
  }
  else
    arrayElementStackedWidget->setVisible(false);
}


void te::qt::widgets::AddProperty::stringTypeComboBoxChanged(const QString& stringType)
{
  stringSizeLineEdit->setEnabled(true);

  if(stringType == "STRING")
  {
    stringSizeLineEdit->clear();
    stringSizeLineEdit->setEnabled(false);
  }
}

void te::qt::widgets::AddProperty::okPushButtonClicked()
{
  // Get the property name
  if(propertyNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Missing the Property Name"),
      tr("Set the name of the new property!"));
    return;
  }

  m_propertyName = propertyNameLineEdit->text().toStdString();

  // Get the default value
  if(defaultValueLineEdit->text().isEmpty())
    m_defaultValue = 0;
  else
    m_defaultValue = new std::string(defaultValueLineEdit->text().toStdString());

  // Build the property
  QString dataType = dataTypeComboBox->currentText();

  m_property = buildProperty(dataType);

  if(m_property == 0)
    return;

  if(m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }

  accept();
}

void te::qt::widgets::AddProperty::cancelPushButtonClicked()
{
  // Release the transactor
  if (m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }

  reject();
}

void te::qt::widgets::AddProperty::helpPushButtonClicked()
{
}

void te::qt::widgets::AddProperty::setArrayPropertyParams()
{
  dataTypeStackedWidget->setCurrentWidget(arrayPage);

  // Fill the arrayElementDataTypeComboBox with the 
  // array element types currently supported
  if(arrayElementDataTypeComboBox->count() == 0)
  {
    for(int i = 0; i < dataTypeComboBox->count(); ++i)
    {
      QString dataType = dataTypeComboBox->itemText(i);

      if(dataType == "ARRAY" || dataType == "COMPOSITE" ||
         dataType == "GEOMETRY" || dataType == "RASTER")
        continue;

      arrayElementDataTypeComboBox->addItem(dataType);
    }
  }

  // Fill the arrayElementStringTypeComboBox with the string types
  if (arrayElementStringTypeComboBox->count() == 0)
  {
    arrayElementStringTypeComboBox->addItem("FIXED_STRING");
    arrayElementStringTypeComboBox->addItem("VAR_STRING");
    arrayElementStringTypeComboBox->addItem("STRING");
  }
}

void te::qt::widgets::AddProperty::setDateTimePropertyParams()
{
  dataTypeStackedWidget->setCurrentWidget(dateTimePage);

  // Fill the dateTimeTypeComboBox with the datetime types
  if (dateTimeTypeComboBox->count() == 0)
  {
    dateTimeTypeComboBox->addItem("DATE");
    dateTimeTypeComboBox->addItem("DATE_PERIOD");
    dateTimeTypeComboBox->addItem("DATE_DURATION");
    dateTimeTypeComboBox->addItem("TIME_DURATION");
    dateTimeTypeComboBox->addItem("TIME_INSTANT");
    dateTimeTypeComboBox->addItem("TIME_PERIOD");
    dateTimeTypeComboBox->addItem("TIME_INSTANT_TZ");
    dateTimeTypeComboBox->addItem("TIME_PERIOD_TZ");
  }

  QMessageBox::warning(this, tr("Add Operation Not Implemented"), 
        tr("The add operation for the datetime property was not implemented yet!"));
}

void te::qt::widgets::AddProperty::setGeometryPropertyParams()
{
  dataTypeStackedWidget->setCurrentWidget(geometryPage);

  // Fill the geometryTypeComboBox with the geometry types
  if (geometryTypeComboBox->count() == 0)
  {
    geometryTypeComboBox->addItem("GEOMETRY");
    geometryTypeComboBox->addItem("GEOMETRYZ");
    geometryTypeComboBox->addItem("GEOMETRYM");
    geometryTypeComboBox->addItem("GEOMETRYZM");
    geometryTypeComboBox->addItem("POINT");
    geometryTypeComboBox->addItem("POINTZ");
    geometryTypeComboBox->addItem("POINTM");
    geometryTypeComboBox->addItem("POINTZM");
    geometryTypeComboBox->addItem("LINESTRING");
    geometryTypeComboBox->addItem("LINESTRINGZ");
    geometryTypeComboBox->addItem("LINESTRINGM");
    geometryTypeComboBox->addItem("LINESTRINGZM");
    geometryTypeComboBox->addItem("POLYGON");
    geometryTypeComboBox->addItem("POLYGONZ");
    geometryTypeComboBox->addItem("POLYGONM");
    geometryTypeComboBox->addItem("POLYGONZM");
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTION");
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONZ");
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONM");
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONZM");
    geometryTypeComboBox->addItem("MULTIPOINT");
    geometryTypeComboBox->addItem("MULTIPOINTZ");
    geometryTypeComboBox->addItem("MULTIPOINTM");
    geometryTypeComboBox->addItem("MULTIPOINTZM");
    geometryTypeComboBox->addItem("MULTILINESTRING");
    geometryTypeComboBox->addItem("MULTILINESTRINGZ");
    geometryTypeComboBox->addItem("MULTILINESTRINGM");
    geometryTypeComboBox->addItem("MULTILINESTRINGZM");
    geometryTypeComboBox->addItem("MULTIPOLYGON");
    geometryTypeComboBox->addItem("MULTIPOLYGONZ");
    geometryTypeComboBox->addItem("MULTIPOLYGONM");
    geometryTypeComboBox->addItem("MULTIPOLYGONZM");
    geometryTypeComboBox->addItem("UNKNOWNGEOMETRY");
  }
}

void te::qt::widgets::AddProperty::setStringPropertyParams()
{
  dataTypeStackedWidget->setCurrentWidget(stringPage);

  // Fill the stringTypeComboBox with the string types
  if (stringTypeComboBox->count() == 0)
  {
    stringTypeComboBox->addItem("FIXED_STRING");
    stringTypeComboBox->addItem("VAR_STRING");
    stringTypeComboBox->addItem("STRING");
  }
}

te::dt::Property* te::qt::widgets::AddProperty::buildProperty(const QString& dataType)
{
  if(dataType == "ARRAY")
    return buildArrayProperty();
  else if(dataType == "BIT")
    return buildBitProperty(); 
  else if(dataType == "BOOLEAN")
    return buildBooleanProperty(); 
  else if(dataType == "BYTE_ARRAY")
    return buildByteArrayProperty();
  else if(dataType == "CHAR")
    return buildCharProperty();
  else if(dataType == "COMPOSITE")
   return buildCompositeProperty();
  else if(dataType == "DATETIME")
    return buildDateTimeProperty();
  else if(dataType == "DOUBLE")
    return buildDoubleProperty(); 
  else if(dataType == "FLOAT")
    return buildFloatProperty();
  else if(dataType == "GEOMETRY")
    return buildGeometryProperty();
  else if(dataType == "INT16")
    return buildInt16Property(); 
  else if (dataType == "INT32")
    return buildInt32Property(); 
  else if(dataType == "INT64")
    return buildInt64Property();
  else if(dataType == "NUMERIC")
    return buildNumericProperty();
  else if(dataType == "RASTER")
    return buildRasterProperty();
  else if(dataType == "STRING")
    return buildStringProperty();
  else if(dataType == "UCHAR")
    return buildUcharProperty(); 
  else if(dataType == "UINT16")
    return buildUint16Property(); 
  else if(dataType == "UINT32")
    return buildUint32Property(); 
  else if(dataType == "UINT64")
    return buildUint64Property();
  else if(dataType == "UNKNOWN")
    return buildUnknownProperty(); 
  else if(dataType == "VOID")
    return buildVoidProperty();

  return 0;
}

te::dt::Property* te::qt::widgets::AddProperty::buildArrayProperty()
{
  // Get the dimension
  unsigned int dimension = 1;
  if(arrayDimensionLineEdit->text().isEmpty() == false)
  {
    bool ok;
    dimension = arrayDimensionLineEdit->text().toUInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
      tr("The array dimension must be an unsigned int value!"));
      return 0;
    }
  }

  // Build the array element property
  te::dt::SimpleProperty* arrayElementProperty;

  QString arrayElementDataType = arrayElementDataTypeComboBox->currentText();

  std::string tempPropertyName = m_propertyName;
  m_propertyName = std::string();

  std::string* tempDefaultValue = m_defaultValue;
  m_defaultValue = 0;

  arrayElementProperty = static_cast<te::dt::SimpleProperty*>(buildProperty(arrayElementDataType));
  arrayElementProperty->setAutoNumber(false);
  arrayElementProperty->setRequired(false);

  m_propertyName = tempPropertyName;
  m_defaultValue = tempDefaultValue;

  if(dimension != 1)
  {
    te::dt::SimpleProperty* simpleP = arrayElementProperty;

    arrayElementProperty = 0;

    for(unsigned int i = 0; i < dimension - 2; ++i)
      arrayElementProperty = new te::dt::ArrayProperty(std::string(), arrayElementProperty);

    arrayElementProperty = new te::dt::ArrayProperty(std::string(), simpleP);
  }

   return new te::dt::ArrayProperty(m_propertyName, arrayElementProperty,
                                   requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildBitProperty()
{ 
  if(m_defaultValue != 0)
  {
    for(size_t i = 0; i < m_defaultValue->size(); ++i)
    {
      if(m_defaultValue->operator[](i) == '0' || m_defaultValue->operator[](i) == '1')
      {
        QMessageBox::warning(this, tr("Default Value Error"), 
          tr("The default value contents for the BIT property can only contains  0's or 1's!"));
        return 0;
      }
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::BIT_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildBooleanProperty()
{
  if(m_defaultValue != 0)
  {
    QString qs = defaultValueLineEdit->text().toUpper();

    if(qs != "TRUE" && qs != "FALSE") 
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value for the BOOLEAN property can only be: true, TRUE, false, or FALSE"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::BOOLEAN_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildByteArrayProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::BYTE_ARRAY_TYPE,
                       requiredCheckBox->isChecked(), m_defaultValue);
}


te::dt::Property* te::qt::widgets::AddProperty::buildCharProperty()
{
  if(m_defaultValue != 0)
  {
    bool ok;
    int val = defaultValueLineEdit->text().toInt(&ok);

    if(ok == false || val <-128 || val > 127)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be a char value >= -128 and <= 127!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::CHAR_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildCompositeProperty()
{
  std::string cname = std::string();

  if(compositeNameLineEdit->text().isEmpty() == false)
    cname = compositeNameLineEdit->text().toStdString();
    
  return new te::dt::CompositeProperty(cname, m_propertyName);
}

te::dt::Property* te::qt::widgets::AddProperty::buildDateTimeProperty()
{
  // Get the DateTime type
  te::dt::DateTimeType dateTimeType = 
    static_cast<te::dt::DateTimeType>(dateTimeTypeComboBox->currentIndex());

  // Build the DateTime property
  te::dt::Property* dateTimeProperty = new te::dt::DateTimeProperty(m_propertyName,
            dateTimeType, te::dt::SECOND, requiredCheckBox->isChecked(), m_defaultValue);
        
  return dateTimeProperty;
 }

te::dt::Property* te::qt::widgets::AddProperty::buildDoubleProperty()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toDouble(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be a double number!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::DOUBLE_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildFloatProperty()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toFloat(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be a float number!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::FLOAT_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildGeometryProperty()
{
  // Get the geometry subtype
  std::string gTypeStr = geometryTypeComboBox->currentText().toStdString();
  te::gm::GeomType gType = te::gm::Geometry::getGeomTypeId(gTypeStr);

  // Get the srid
  int srid = 0;
  if (geometrySRIDLineEdit->text().isEmpty() == false)
  {
    bool ok;
    srid = geometrySRIDLineEdit->text().toInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid SRID"), 
        tr("The SRID for the GEOMETRY property must be an integer number!"));
      return 0;
    }
  }

  return new te::gm::GeometryProperty(m_propertyName, srid, gType,
                  requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildInt16Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toShort(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be a short value!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::INT16_TYPE,
                requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildInt32Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be an integer value!"));
      return 0;
    }
  }

  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(m_propertyName, te::dt::INT32_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);

  p->setAutoNumber(autoNumberCheckBox->isChecked());

  return p;
}

te::dt::Property* te::qt::widgets::AddProperty::buildInt64Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toLongLong(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be a long long value!"));
      return 0;
    }
  }

  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(m_propertyName, te::dt::INT64_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);

  p->setAutoNumber(autoNumberCheckBox->isChecked());

  return p;
}

te::dt::Property* te::qt::widgets::AddProperty::buildNumericProperty()
{
  bool ok;

  // Check the default value
  if(dataTypeComboBox->currentText() == "NUMERIC")
  {
    if(m_defaultValue != 0)
    {
      defaultValueLineEdit->text().toDouble(&ok);
      if(ok == false)
      {
        QMessageBox::warning(this, tr("Default Value Error"), 
          tr("The default value must be a numeric value!"));
        return 0;
      }
    }
  }

  // Get the precision value
  unsigned int precision = 0;

  if(dataTypeComboBox->currentText() == "NUMERIC")
  {
    if(precisionLineEdit->text().isEmpty() == false)
      precision = precisionLineEdit->text().toUInt(&ok);
  }
  else
  {
    if(arrayElementPrecisionLineEdit->text().isEmpty() == false)
      precision = arrayElementPrecisionLineEdit->text().toUInt(&ok);
  }

  if(ok == false)
  {
    QMessageBox::warning(this, tr("Invalid Value"), 
    tr("The precision must be an unsigned int value!"));
    return 0;
  }

  // Get the scale value
  unsigned int scale = 0;

  if(dataTypeComboBox->currentText() == "NUMERIC")
  {
    if(scaleLineEdit->text().isEmpty() == false)
      scale = scaleLineEdit->text().toUInt(&ok);
  }
  else
  {
    if(arrayElementScaleLineEdit->text().isEmpty() == false)
      scale = arrayElementScaleLineEdit->text().toUInt(&ok);
  }

  if(ok == false)
  {
    QMessageBox::warning(this, tr("Invalid Value"), 
      tr("The scale must be an unsigned int value!"));
    return 0;
  }

  return new te::dt::NumericProperty(m_propertyName,
        precision, scale, requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildRasterProperty()
{
  bool ok;

  // Get the srid
  int srid = 0;
  if (rasterSRIDLineEdit->text().isEmpty() == false)
  {
    srid = rasterSRIDLineEdit->text().toInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The SRID must be an integer value!"));
      return 0;
    }
  }

  // Get the number of columns
  unsigned int nCols = 0;
  if (nColsLineEdit->text().isEmpty() == false)
  {
    nCols = nColsLineEdit->text().toUInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The number of columns must be an unsigned integer value!"));
      return 0;
    }
  }

  // Get the number of lines
  unsigned int nLines = 0;
  if (nLinesLineEdit->text().isEmpty() == false)
  {
    nLines = nLinesLineEdit->text().toUInt();
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The number of lines must be an unsigned integer value!"));
      return 0;
    }
  }

  // Get the X resolution
  float resX = 0.;
  if (resxLineEdit->text().isEmpty() == false)
  {
    resX = resxLineEdit->text().toFloat(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The X resolution must be a float value!"));
      return 0;
    }
  }

  // Get the Y resolution
  float resY = 0.;
  if (resyLineEdit->text().isEmpty() == false)
  {
    resY = resyLineEdit->text().toFloat();
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The Y resolution must be a float value!"));
      return 0;
    }
  }

  // Get the envelope
  double llx = 0.0;
  double lly = 0.0;
  double urx = 0.0;
  double ury = 0.0;

  te::gm::Envelope* envelope;

  if(rasterLLXLineEdit->text().isEmpty() == true &&
      rasterLLYLineEdit->text().isEmpty() == true &&
      rasterURXLineEdit->text().isEmpty() == true &&
      rasterURYLineEdit->text().isEmpty() == true)
  {
    envelope = 0;
  }
  else
  {
    if(rasterLLXLineEdit->text().isEmpty() == false)
    {
      llx = rasterLLXLineEdit->text().toDouble(&ok);
      if(ok == false)
      {
        QMessageBox::warning(this, tr("Invalid Value"), 
          tr("The LLX coordinate must be a double value!"));
        return 0;
      }
    }

    if(rasterLLYLineEdit->text().isEmpty() == false)
    {
      lly = rasterLLYLineEdit->text().toDouble(&ok);
      if(ok == false)
      {
        QMessageBox::warning(this, tr("Invalid Value"), 
          tr("The LLY coordinate must be a double value!"));
        return 0;
      }
    }

    if(rasterURXLineEdit->text().isEmpty() == false)
    {
      urx = rasterURXLineEdit->text().toDouble(&ok);
      if(ok == false)
      {
        QMessageBox::warning(this, tr("Invalid Value"), 
          tr("The URX coordinate must be a double value!"));
        return 0;
      }
    }

    if(rasterURYLineEdit->text().isEmpty() == false)
    {
      ury = rasterURYLineEdit->text().toDouble(&ok);
      if(ok == false)
      {
        QMessageBox::warning(this, tr("Invalid Value"), 
          tr("The URY coordinate must be a double value!"));
        return 0;
      }
    }

    envelope = new te::gm::Envelope(llx, lly, urx, ury);
  }

  // Get the number of bands
  unsigned int nBands = 0;

  if (nBandsLineEdit->text().isEmpty() == false)
  {
    nBands = nBandsLineEdit->text().toUInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Invalid Value"), 
        tr("The number of bands must be an unsigned integer value!"));
      return 0;
    }
  }

  // Build the raster property
  te::rst::Grid* grid = new te::rst::Grid(nCols,nLines);
  grid->setGeoreference(te::gm::Coord2D(llx, ury), srid, resX, resY);

  std::map<std::string, std::string> rinfo;

  // how to retrieve the band data type?
  std::vector<te::rst::BandProperty*> bprops;
  for (unsigned b = 0; b < nBands; b++)
    bprops.push_back(new te::rst::BandProperty(b, te::dt::CHAR_TYPE));

  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo, requiredCheckBox->isChecked());

  return rp;
}

te::dt::Property* te::qt::widgets::AddProperty::buildStringProperty()
{
  // Get the string type and the string size
  te::dt::StringType stringType;
  size_t stringSize = 0;
  bool ok;

  if(dataTypeComboBox->currentText() == "STRING")
  {
    stringType = static_cast<te::dt::StringType>(stringTypeComboBox->currentIndex());

    if(stringSizeLineEdit->text().isEmpty() == false)
      stringSize = stringSizeLineEdit->text().toUInt(&ok);
  }
  else
  {
    stringType = static_cast<te::dt::StringType>(arrayElementStringTypeComboBox->currentIndex());

    if(arrayElementStringSizeLineEdit->text().isEmpty() == false)
      stringSize = arrayElementStringSizeLineEdit->text().toUInt(&ok);
  }

  if(ok == false)
  {
    QMessageBox::warning(this, tr("Invalid Value"), 
      tr("The string size must be an unsigned int value!"));
    return 0;
  }

  te::dt::Property* stringProperty = new te::dt::StringProperty(m_propertyName,
            stringType, stringSize, requiredCheckBox->isChecked(), m_defaultValue);
      
  return stringProperty;
}

te::dt::Property* te::qt::widgets::AddProperty::buildUcharProperty()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    unsigned int val = defaultValueLineEdit->text().toUShort(&ok);
    if(ok == false || val > 255)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be an unsigned char value!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::UCHAR_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildUint16Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toUShort(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be an unsigned short value!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::UINT32_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildUint32Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toUInt(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be an unsigned integer value!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::UINT32_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildUint64Property()
{
  if(m_defaultValue != 0)
  {
    bool ok;

    defaultValueLineEdit->text().toULongLong(&ok);
    if(ok == false)
    {
      QMessageBox::warning(this, tr("Default Value Error"), 
        tr("The default value must be an unsigned long long value!"));
      return 0;
    }
  }

  return new te::dt::SimpleProperty(m_propertyName, te::dt::UINT64_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::AddProperty::buildUnknownProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::UNKNOWN_TYPE,
              false, 0);
}

te::dt::Property* te::qt::widgets::AddProperty::buildVoidProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::VOID_TYPE,
              false, 0);
}

void te::qt::widgets::AddProperty::closeEvent(QCloseEvent* /*e*/)
{
  // Release the transactor
  if (m_transactor)
  {
    delete m_transactor;
    m_transactor = 0;
  }
}
