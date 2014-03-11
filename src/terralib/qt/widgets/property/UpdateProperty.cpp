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

// Qt
#include <QtGui/QtGui>
#include <QtGui/QComboBox>
#include <QtGui/QMessageBox>

// TerraLib
#include "../../../dataaccess.h"
#include "../../../datatype.h"
#include "../../../datatype/ByteArray.h"
#include "../../../datatype/Enums.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../raster/Grid.h"
#include "../../../raster/RasterProperty.h"

#include "UpdateProperty.h"

te::qt::widgets::UpdateProperty::UpdateProperty(te::da::DataSource* ds, QWidget* parent)
  : QDialog(parent),
    m_ds(ds),
    m_oldProperty(0),
    m_updatedProperty(0)
{
  if (m_ds == 0)
    QMessageBox::critical(this, tr("Missing a Valid Data Source"), tr("Provide a valid data source!"));

  setupUi(this);

  layout()->setSizeConstraint(QLayout::SetFixedSize);

  // Get the dataset names of the data source
  std::vector<std::string> datasetNames = m_ds->getDataSetNames();

  // Fill alphabetically the dataSetCombobox with the dataset names of the data source
  QStringList dataSetList;

  size_t numDataSets = datasetNames.size();
  for (size_t i = 0; i < numDataSets; ++i)
    dataSetList << (datasetNames[i]).c_str();

  dataSetList.sort();
  dataSetComboBox->addItems(dataSetList);

  // Fill the dataTypeComboBox with the data types available
  // according the data source capabilities
  const te::da::DataSourceCapabilities& capabilities = m_ds->getCapabilities();
  const te::da::DataTypeCapabilities& dataTypeCapabilities = capabilities.getDataTypeCapabilities();

  if(dataTypeCapabilities.supportsArray())
  {
    dataTypeComboBox->addItem("ARRAY");
    m_propertyTypeMap[te::dt::ARRAY_TYPE] = "ARRAY";
  }

  if(dataTypeCapabilities.supportsBit())
  {
    dataTypeComboBox->addItem("BIT");
    m_propertyTypeMap[te::dt::BIT_TYPE] = "BIT";
  }

  if(dataTypeCapabilities.supportsBoolean())
  {
    dataTypeComboBox->addItem("BOOLEAN");
    m_propertyTypeMap[te::dt::BOOLEAN_TYPE] = "BOOLEAN";
  }

  if(dataTypeCapabilities.supportsByteArray())
  {
    dataTypeComboBox->addItem("BYTE_ARRAY");
    m_propertyTypeMap[te::dt::BYTE_ARRAY_TYPE] = "BYTE_ARRAY";
  }

  if(dataTypeCapabilities.supportsChar())
  {
    dataTypeComboBox->addItem("CHAR");
    m_propertyTypeMap[te::dt::CHAR_TYPE] = "CHAR";
  }

  if(dataTypeCapabilities.supportsComposite())
  {
    dataTypeComboBox->addItem("COMPOSITE");
    m_propertyTypeMap[te::dt::COMPOSITE_TYPE] = "COMPOSITE";
  }

  if(dataTypeCapabilities.supportsDataset())
  {
    dataTypeComboBox->addItem("DATASET");
    m_propertyTypeMap[te::dt::DATASET_TYPE] = "DATASET";
  }

  if(dataTypeCapabilities.supportsDateTime())
  {
    dataTypeComboBox->addItem("DATETIME");
    m_propertyTypeMap[te::dt::DATETIME_TYPE] = "DATETIME";
  }

  if(dataTypeCapabilities.supportsDouble())
  {
    dataTypeComboBox->addItem("DOUBLE");
    m_propertyTypeMap[te::dt::DOUBLE_TYPE] = "DOUBLE";
  }

  if(dataTypeCapabilities.supportsFloat())
  {
    dataTypeComboBox->addItem("FLOAT");
    m_propertyTypeMap[te::dt::FLOAT_TYPE] = "FLOAT";
  }

  if(dataTypeCapabilities.supportsGeometry())
  {
    dataTypeComboBox->addItem("GEOMETRY");
    m_propertyTypeMap[te::dt::GEOMETRY_TYPE] = "GEOMETRY";
  }

  if(dataTypeCapabilities.supportsInt16())
  {
    dataTypeComboBox->addItem("INT16");
    m_propertyTypeMap[te::dt::INT16_TYPE] = "INT16";
  }

  if(dataTypeCapabilities.supportsInt32())
  {
    dataTypeComboBox->addItem("INT32");
    m_propertyTypeMap[te::dt::INT32_TYPE] = "INT32";
  }

  if(dataTypeCapabilities.supportsInt64())
  {
    dataTypeComboBox->addItem("INT64");
    m_propertyTypeMap[te::dt::INT64_TYPE] = "INT64";
  }

  if(dataTypeCapabilities.supportsNumeric())
  {
    dataTypeComboBox->addItem("NUMERIC");
    m_propertyTypeMap[te::dt::NUMERIC_TYPE] = "NUMERIC";
  }

  if(dataTypeCapabilities.supportsRaster())
  {
    dataTypeComboBox->addItem("RASTER");
    m_propertyTypeMap[te::dt::RASTER_TYPE] = "RASTER";
  }

  if(dataTypeCapabilities.supportsString())
  {
    dataTypeComboBox->addItem("STRING");
    m_propertyTypeMap[te::dt::STRING_TYPE] = "STRING";
  }

  if(dataTypeCapabilities.supportsUChar())
  {
    dataTypeComboBox->addItem("UCHAR");
    m_propertyTypeMap[te::dt::UCHAR_TYPE] = "UCHAR";
  }

  if(dataTypeCapabilities.supportsUInt16())
  {
    dataTypeComboBox->addItem("UINT16");
    m_propertyTypeMap[te::dt::UINT32_TYPE] = "UINT16";
  }

  if(dataTypeCapabilities.supportsUInt32())
  {
    dataTypeComboBox->addItem("UINT32");
    m_propertyTypeMap[te::dt::UINT32_TYPE] = "UINT32";
  }

  if(dataTypeCapabilities.supportsUInt64())
  {
    dataTypeComboBox->addItem("UINT64");
    m_propertyTypeMap[te::dt::UINT64_TYPE] = "UINT64";
  }

  // Connect the signals/slots
  connect(dataSetComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataSetComboBoxChanged(const QString&)));
  connect(propertiesComboBox, SIGNAL(activated(const QString&)), this, SLOT(propertiesComboBoxActivated(const QString&)));
  connect(dataTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataTypeComboBoxChanged(const QString&)));
  connect(requiredCheckBox, SIGNAL(clicked(bool)), this, SLOT(requiredCheckBoxClicked(bool)));
  connect(arrayElementDataTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(arrayElementDataTypeComboBoxChanged(const QString&)));
  connect(okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClicked()));
  connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClicked()));
  connect(helpPushButton, SIGNAL(clicked()), this, SLOT(helpPushButtonClicked()));

  dataSetComboBoxChanged(dataSetComboBox->currentText());
}

te::qt::widgets::UpdateProperty::~UpdateProperty()
{
}

void te::qt::widgets::UpdateProperty::dataSetComboBoxChanged(const QString& datasetName)
{
  propertiesComboBox->clear();

  std::vector<std::string> pNames = m_ds->getPropertyNames(datasetName.toStdString());
  for (size_t i = 0; i < pNames.size(); ++i)
    propertiesComboBox->addItem(pNames[i].c_str());

  propertiesComboBoxActivated(pNames[0].c_str());
}

void te::qt::widgets::UpdateProperty::propertiesComboBoxActivated(const QString& propertyName)
{
  // Get the old property
  m_oldProperty = (m_ds->getProperty(dataSetComboBox->currentText().toStdString(), propertyName.toStdString())).release();
 
  // Set in the dataTypeComboBox the initial old property data type
  int oldPropertyType = m_oldProperty->getType();

  int pos = dataTypeComboBox->findText(m_propertyTypeMap[oldPropertyType]);
  dataTypeComboBox->setCurrentIndex(pos);

  // Set in the propertyNameLineEdit the name of the old property name
  propertyNameLineEdit->clear();
  propertyNameLineEdit->setText(m_oldProperty->getName().c_str());

  // Initialize the autoNumberCheckBox status
  autoNumberCheckBox->setEnabled(false);
  autoNumberCheckBox->setChecked(false);

  // Initialize the requiredCheckBox status
  requiredCheckBox->setEnabled(true);
  requiredCheckBox->setChecked(false);

  // Initialize the defaultValueLineEdit status
  defaultValueLineEdit->clear();
  defaultValueLineEdit->setEnabled(true);

  // Set the dataTypeStackedWidget as visible
  dataTypeStackedWidget->setVisible(true);
 
  // Get the type of the property selected  
  QString dataType = dataTypeComboBox->currentText();

  // Set the particular data of the old property
  if(dataType == "ARRAY")
    setArrayPropertyData();
  else if(dataType == "COMPOSITE")
    setCompositePropertyData();
  else if(dataType == "DATETIME")
    setDateTimePropertyData();
  else if(dataType == "GEOMETRY")
    setGeometryPropertyData();
  else if(dataType == "NUMERIC")
    setNumericPropertyData();
  else if(dataType == "RASTER")
    setRasterPropertyData();
  else if(dataType == "STRING")
    setStringPropertyData();
  else
  {
    te::dt::SimpleProperty* sp = static_cast<te::dt::SimpleProperty*>(m_oldProperty);
    std::string* defaultValue = sp->getDefaultValue();

    if(dataType == "INT32" || dataType == "INT64")
    {
      if(sp->isAutoNumber())
      {
        autoNumberCheckBox->setChecked(true);
        requiredCheckBox->setChecked(true);
        requiredCheckBox->setEnabled(false);
        defaultValueLineEdit->setText(defaultValue->c_str());
        defaultValueLineEdit->setEnabled(false);
      }
    }

    if(sp->isRequired())
      requiredCheckBox->setChecked(true);

    if(defaultValue)
      defaultValueLineEdit->setText(defaultValue->c_str());

    dataTypeStackedWidget->setVisible(false);
  }
}

void te::qt::widgets::UpdateProperty::dataTypeComboBoxChanged(const QString& dataType)
{
  dataTypeStackedWidget->setVisible(false);

  precisionLineEdit->setEnabled(true);
  scaleLineEdit->setEnabled(true);
  stringTypeComboBox->setEnabled(false);
  stringSizeLineEdit->setEnabled(false);

  if(dataType == "NUMERIC")
  {
    dataTypeStackedWidget->setVisible(true);
    dataTypeStackedWidget->setCurrentWidget(numericPage);

    if(m_oldProperty->getType() == te::dt::NUMERIC_TYPE)
    {
      precisionLineEdit->setEnabled(false);
      scaleLineEdit->setEnabled(false);
    }
  }
  else if(dataType == "STRING")
  {
    dataTypeStackedWidget->setVisible(true);
    dataTypeStackedWidget->setCurrentWidget(stringPage);

    if(m_oldProperty->getType() == te::dt::STRING_TYPE)
    {
      stringTypeComboBox->setEnabled(false);
      stringSizeLineEdit->setEnabled(false);
    }
  }
}

void te::qt::widgets::UpdateProperty::requiredCheckBoxClicked(bool checked)
{
  defaultValueLineEdit->clear();
  if(checked == true)
    defaultValueLineEdit->setEnabled(true);
  else
    defaultValueLineEdit->setEnabled(false);
}

void te::qt::widgets::UpdateProperty::arrayElementDataTypeComboBoxChanged(const QString& arrayElementDataType)
{
  arrayElementStackedWidget->setVisible(false);

  arrayElementPrecisionLineEdit->setEnabled(true);
  arrayElementScaleLineEdit->setEnabled(true);
  arrayElementStringTypeComboBox->setEnabled(false);
  arrayElementStringSizeLineEdit->setEnabled(false);

  // Get the type of the old array element
  te::dt::ArrayProperty* oldArrayProperty = static_cast<te::dt::ArrayProperty*>(m_oldProperty);

  te::dt::Property* oldArrayElementProperty = oldArrayProperty->getElementType();
  while(oldArrayElementProperty->getType() == te::dt::ARRAY_TYPE)
    oldArrayElementProperty = static_cast<te::dt::ArrayProperty*>(oldArrayElementProperty)->getElementType();

  int oldArrayElementDataType = oldArrayElementProperty->getType();

  if(arrayElementDataType == "NUMERIC")
  {
    arrayElementStackedWidget->setVisible(true);
    arrayElementStackedWidget->setCurrentWidget(arrayElementNumericPage);

    if(oldArrayElementDataType == te::dt::NUMERIC_TYPE)
    {
      arrayElementPrecisionLineEdit->setEnabled(false);
      arrayElementScaleLineEdit->setEnabled(false);
    }
  }
  else if(arrayElementDataType == "STRING")
  {
    arrayElementStackedWidget->setVisible(true);
    arrayElementStackedWidget->setCurrentWidget(arrayElementStringPage);

    if(oldArrayElementDataType == te::dt::STRING_TYPE)
    {
      arrayElementStringTypeComboBox->setEnabled(false);
      arrayElementStringSizeLineEdit->setEnabled(false);
    }
  }
}

void te::qt::widgets::UpdateProperty::okPushButtonClicked()
{
  // Get the property name
  if(propertyNameLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, tr("Missing the Property Name"),
      tr("Set the name of the property to be added!"));
    return;
  }

  m_propertyName = propertyNameLineEdit->text().toStdString();

  // Get the default value
  if(defaultValueLineEdit->text().isEmpty())
    m_defaultValue = 0;
  else
    m_defaultValue = new std::string(defaultValueLineEdit->text().toStdString());

  // Build the property to be updated
  QString dataType = dataTypeComboBox->currentText();

  m_updatedProperty = buildUpdatedProperty(dataType);

  if(m_updatedProperty == 0)
    return;

  accept();
}

void te::qt::widgets::UpdateProperty::cancelPushButtonClicked()
{
  reject();
}

void te::qt::widgets::UpdateProperty::helpPushButtonClicked()
{
}

void te::qt::widgets::UpdateProperty::setArrayPropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(arrayPage);

  te::dt::ArrayProperty* oldArrayProperty = static_cast<te::dt::ArrayProperty*>(m_oldProperty);

  if(oldArrayProperty->isRequired() == true)
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = oldArrayProperty->getDefaultValue();
  if(defaultValue != 0)
  {
    size_t firstIndex = defaultValue->find("'");
    size_t lastIndex = defaultValue->rfind("'");

    size_t len = lastIndex - firstIndex - 1;

    std::string defaultVal = defaultValue->substr(firstIndex + 1, len);
    defaultValueLineEdit->setText(defaultVal.c_str());
  }

  arrayDimensionLineEdit->setEnabled(false);

  // Set the array dimension
  int dimension = 1;

  te::dt::Property* arrayElementProperty = oldArrayProperty->getElementType();
  while(arrayElementProperty->getType() == te::dt::ARRAY_TYPE)
  {
    ++dimension;
    arrayElementProperty = static_cast<te::dt::ArrayProperty*>(arrayElementProperty)->getElementType();
  }

  arrayDimensionLineEdit->setText(QString().number(dimension));

  // Set the array element data type
  if(arrayElementDataTypeComboBox->count() == 0)
  {
    for(int i = 0; i < dataTypeComboBox->count(); ++i)
    {
      QString typeItem = dataTypeComboBox->itemText(i);
      if(typeItem != "ARRAY")
        arrayElementDataTypeComboBox->addItem(typeItem);
    }
  }

  QString arrayElementDataType = m_propertyTypeMap[arrayElementProperty->getType()];
  int index = arrayElementDataTypeComboBox->findText(arrayElementDataType);
  arrayElementDataTypeComboBox->setCurrentIndex(index);

  // Set the array element parameters for the numeric and string types.
  if(arrayElementDataTypeComboBox->currentText() == "NUMERIC")
  {
    te::dt::NumericProperty* numericProperty = static_cast<te::dt::NumericProperty*>(arrayElementProperty);

    // Set the precision
    arrayElementPrecisionLineEdit->clear();
    arrayElementPrecisionLineEdit->setText(QString().setNum(numericProperty->getPrecision()));

    // Set the scale
    arrayElementScaleLineEdit->clear();
    arrayElementScaleLineEdit->setText(QString().setNum(numericProperty->getScale()));
  }
  else if(arrayElementDataTypeComboBox->currentText() == "STRING")
  {
    te::dt::StringProperty* stringProperty = static_cast<te::dt::StringProperty*>(arrayElementProperty);

    if(arrayElementStringTypeComboBox->count() == 0)
    {
      arrayElementStringTypeComboBox->addItem("FIXED_STRING");
      arrayElementStringTypeComboBox->addItem("VAR_STRING");
      arrayElementStringTypeComboBox->addItem("STRING");
    }

    te::dt::StringType stringType = stringProperty->getSubType();
    arrayElementStringTypeComboBox->setCurrentIndex(stringType);

    arrayElementStringSizeLineEdit->clear();
    arrayElementStringSizeLineEdit->setText(QString().setNum(stringProperty->size()));
  }
}

void te::qt::widgets::UpdateProperty::setCompositePropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(compositePage);

  compositeNameLineEdit->clear();
  te::dt::CompositeProperty* cp = static_cast<te::dt::CompositeProperty*>(m_oldProperty);
  compositeNameLineEdit->setText(cp->getCompositeName().c_str());
}

void te::qt::widgets::UpdateProperty::setDateTimePropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(dateTimePage);

  te::dt::DateTimeProperty* dtp = static_cast<te::dt::DateTimeProperty*>(m_oldProperty);

  if(dtp->isRequired() == true)
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = dtp->getDefaultValue();
  if(defaultValue != 0)
    defaultValueLineEdit->setText(defaultValue->c_str());

  // Fill the dateTimeTypeComboBox with the datetime types
  if (dateTimeTypeComboBox->count() == 0)
  {
    dateTimeTypeComboBox->addItem("TIME_STAMP");
    dateTimeTypeComboBox->addItem("TIME_STAMP_TZ");
    dateTimeTypeComboBox->addItem("DATE");
    dateTimeTypeComboBox->addItem("DATETIME");
    dateTimeTypeComboBox->addItem("TIME");
    dateTimeTypeComboBox->addItem("TIME_TZ");
  }

  te::dt::DateTimeType dtType = dtp->getSubType();
  dateTimeTypeComboBox->setCurrentIndex(dtType);
}

void te::qt::widgets::UpdateProperty::setGeometryPropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(geometryPage);

  te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(m_oldProperty);

  if(gp->isRequired() == true)
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = gp->getDefaultValue();
  if(defaultValue != 0)
    defaultValueLineEdit->setText(defaultValue->c_str());

  // Enable/Disable the widgets related to the geometry property
  geometryTypeComboBox->setEnabled(false);
  geometrySRIDLineEdit->setEnabled(false);
  defaultGeometryCheckBox->setEnabled(false);

  geometryEnvelopeGroupBox->setEnabled(false);

  // Fill the geometryTypeComboBox with the geometry types
  if (geometryTypeComboBox->count() == 0)
  {
    geometryTypeComboBox->addItem("GEOMETRY");
    m_geomTypeTextMap[te::gm::GeometryType] = "GEOMETRY";
    geometryTypeComboBox->addItem("GEOMETRYZ");
    m_geomTypeTextMap[te::gm::GeometryZType] = "GEOMETRYZ";
    geometryTypeComboBox->addItem("GEOMETRYM");
    m_geomTypeTextMap[te::gm::GeometryMType] = "GEOMETRYM";
    geometryTypeComboBox->addItem("GEOMETRYZM");
    m_geomTypeTextMap[te::gm::GeometryZMType] = "GEOMETRYZM";
    geometryTypeComboBox->addItem("POINT");
    m_geomTypeTextMap[te::gm::PointType] = "POINT";
    geometryTypeComboBox->addItem("POINTZ");
    m_geomTypeTextMap[te::gm::PointZType] = "POINTZ";
    geometryTypeComboBox->addItem("POINTM");
    m_geomTypeTextMap[te::gm::PointMType] = "POINTM";
    geometryTypeComboBox->addItem("POINTZM");
    m_geomTypeTextMap[te::gm::PointZMType] = "POINTZM";
    geometryTypeComboBox->addItem("LINESTRING");
    m_geomTypeTextMap[te::gm::LineStringType] = "LINESTRING";
    geometryTypeComboBox->addItem("LINESTRINGZ");
    m_geomTypeTextMap[te::gm::LineStringZType] = "LINESTRINGZ";
    geometryTypeComboBox->addItem("LINESTRINGM");
    m_geomTypeTextMap[te::gm::LineStringMType] = "LINESTRINGM";
    geometryTypeComboBox->addItem("LINESTRINGZM");
    m_geomTypeTextMap[te::gm::LineStringZMType] = "LINESTRINGZM";
    geometryTypeComboBox->addItem("POLYGON");
    m_geomTypeTextMap[te::gm::PolygonType] = "POLYGON";
    geometryTypeComboBox->addItem("POLYGONZ");
    m_geomTypeTextMap[te::gm::PolygonZType] = "POLYGONZ";
    geometryTypeComboBox->addItem("POLYGONM");
    m_geomTypeTextMap[te::gm::PolygonMType] = "POLYGONM";
    geometryTypeComboBox->addItem("POLYGONZM");
    m_geomTypeTextMap[te::gm::PolygonZMType] = "POLYGONZM";
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTION");
    m_geomTypeTextMap[te::gm::GeometryCollectionType] = "GEOMETRYCOLLECTION";
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONZ");
    m_geomTypeTextMap[te::gm::GeometryCollectionZType] = "GEOMETRYCOLLECTIONZ";
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONM");
    m_geomTypeTextMap[te::gm::GeometryCollectionMType] = "GEOMETRYCOLLECTIONM";
    geometryTypeComboBox->addItem("GEOMETRYCOLLECTIONZM");
    m_geomTypeTextMap[te::gm::GeometryCollectionZMType] = "GEOMETRYCOLLECTIONZM";
    geometryTypeComboBox->addItem("MULTIPOINT");
    m_geomTypeTextMap[te::gm::MultiPointType] = "MULTIPOINT";
    geometryTypeComboBox->addItem("MULTIPOINTZ");
    m_geomTypeTextMap[te::gm::MultiPointZType] = "MULTIPOINTZ";
    geometryTypeComboBox->addItem("MULTIPOINTM");
    m_geomTypeTextMap[te::gm::MultiPointMType] = "MULTIPOINTM";
    geometryTypeComboBox->addItem("MULTIPOINTZM");
    m_geomTypeTextMap[te::gm::MultiPointZMType] = "MULTIPOINTZM";
    geometryTypeComboBox->addItem("MULTILINESTRING");
    m_geomTypeTextMap[te::gm::MultiLineStringType] = "MULTILINESTRING";
    geometryTypeComboBox->addItem("MULTILINESTRINGZ");
    m_geomTypeTextMap[te::gm::MultiLineStringZType] = "MULTILINESTRINGZ";
    geometryTypeComboBox->addItem("MULTILINESTRINGM");
    m_geomTypeTextMap[te::gm::MultiLineStringMType] = "MULTILINESTRINGM";
    geometryTypeComboBox->addItem("MULTILINESTRINGZM");
    m_geomTypeTextMap[te::gm::MultiLineStringZMType] = "MULTILINESTRINGZM";
    geometryTypeComboBox->addItem("MULTIPOLYGON");
    m_geomTypeTextMap[te::gm::MultiPolygonType] = "MULTIPOLYGON";
    geometryTypeComboBox->addItem("MULTIPOLYGONZ");
    m_geomTypeTextMap[te::gm::MultiPolygonZType] = "MULTIPOLYGONZ";
    geometryTypeComboBox->addItem("MULTIPOLYGONM");
    m_geomTypeTextMap[te::gm::MultiPolygonMType] = "MULTIPOLYGONM";
    geometryTypeComboBox->addItem("MULTIPOLYGONZM");
    m_geomTypeTextMap[te::gm::MultiPolygonZMType] = "MULTIPOLYGONZM";

    geometryTypeComboBox->addItem("UNKNOWNGEOMETRY");
    m_geomTypeTextMap[te::gm::UnknownGeometryType] = "UNKNOWNGEOMETRY";
  }

  // Set the geometry type
  te::gm::GeomType geomType = gp->getGeometryType();
  int pos = geometryTypeComboBox->findText(m_geomTypeTextMap[geomType]);
  geometryTypeComboBox->setCurrentIndex(pos);

  // Set the srid
  geometrySRIDLineEdit->setText(QString().setNum(gp->getSRID()));

  // Set the defaultGeometryCheckBox, if this geometry property is the default
  std::auto_ptr<te::da::DataSetType> dt = m_ds->getDataSetType(dataSetComboBox->currentText().toStdString());
  te::gm::GeometryProperty* defaultGeomProperty = te::da::GetFirstGeomProperty(dt.get());

  if(gp == defaultGeomProperty)
    defaultGeometryCheckBox->setChecked(true);
  else
    defaultGeometryCheckBox->setChecked(false);

  // Set the envelope
  std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(dataSetComboBox->currentText().toStdString());

  std::size_t gpos = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::GEOMETRY_TYPE);

  std::auto_ptr<te::gm::Envelope> e = dataSet->getExtent(gpos);
  if(e.get())
  {
    geometryLLXLineEdit->setText(QString().setNum(e->getLowerLeftX(), 'g', 15));
    geometryLLYLineEdit->setText(QString().setNum(e->getLowerLeftY(), 'g', 15));
    geometryURXLineEdit->setText(QString().setNum(e->getUpperRightX(), 'g', 15));
    geometryURYLineEdit->setText(QString().setNum(e->getUpperRightY(), 'g', 15));
  }
}

void te::qt::widgets::UpdateProperty::setNumericPropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(numericPage);

  te::dt::NumericProperty* np = static_cast<te::dt::NumericProperty*>(m_oldProperty);

  if(np->isRequired())
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = np->getDefaultValue();
  if(defaultValue != 0)
    defaultValueLineEdit->setText(defaultValue->c_str());

  precisionLineEdit->setEnabled(false);
  scaleLineEdit->setEnabled(false);

  // Set the precision
  precisionLineEdit->clear();
  precisionLineEdit->setText(QString().setNum(np->getPrecision()));

  // Set the scale
  scaleLineEdit->clear();
  scaleLineEdit->setText(QString().setNum(np->getScale()));
}

void te::qt::widgets::UpdateProperty::setRasterPropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(rasterPage);

  te::rst::RasterProperty* rp = static_cast<te::rst::RasterProperty*>(m_oldProperty);

  if(rp->isRequired())
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = rp->getDefaultValue();
  if(defaultValue != 0)
    defaultValueLineEdit->setText(defaultValue->c_str());

  //Set the SRID
  rasterSRIDLineEdit->setText(QString().setNum(rp->getGrid()->getSRID()));

  // Set the number of bands
  nBandsLineEdit->setText(QString().setNum(rp->getBandProperties().size()));

  // Set the number of columns
  nColsLineEdit->setText(QString().setNum(rp->getGrid()->getNumberOfColumns()));

  // Set the number of lines
  nLinesLineEdit->setText(QString().setNum(rp->getGrid()->getNumberOfRows()));

  // Set the X resolution
  resxLineEdit->setText(QString().setNum(rp->getGrid()->getResolutionX())); 

  // Set the Y resolution
  resyLineEdit->setText(QString().setNum(rp->getGrid()->getResolutionY())); 

  // Set the envelope
  const te::gm::Envelope* e = rp->getGrid()->getExtent();

  if(e != 0)
  {
    rasterLLXLineEdit->setText(QString().setNum(e->getLowerLeftX()));
    rasterLLYLineEdit->setText(QString().setNum(e->getLowerLeftY()));
    rasterURXLineEdit->setText(QString().setNum(e->getUpperRightX()));
    rasterURYLineEdit->setText(QString().setNum(e->getUpperRightY()));
  }
}

void te::qt::widgets::UpdateProperty::setStringPropertyData()
{
  dataTypeStackedWidget->setCurrentWidget(stringPage);

  te::dt::StringProperty* sp = static_cast<te::dt::StringProperty*>(m_oldProperty);

  if(sp->isRequired())
    requiredCheckBox->setChecked(true);

  std::string* defaultValue = sp->getDefaultValue();
  if(defaultValue)   
    defaultValueLineEdit->setText(defaultValue->c_str());

  stringTypeComboBox->setEnabled(false);
  stringSizeLineEdit->setEnabled(false);

  // Fill the stringTypeComboBox with the string types
  if (stringTypeComboBox->count() == 0)
  {
    stringTypeComboBox->addItem("FIXED_STRING");
    stringTypeComboBox->addItem("VAR_STRING");
    stringTypeComboBox->addItem("STRING");
  }

  // Set the StringProperty size
  stringSizeLineEdit->setText(QString().setNum(sp->size()));

  // Set the StringProperty subtype
  te::dt::StringType stringType = sp->getSubType();
  stringTypeComboBox->setCurrentIndex(stringType);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedProperty(const QString& dataType)
{
  if(dataType == "ARRAY")
    return buildUpdatedArrayProperty();
  else if(dataType == "BIT")
    return buildUpdatedBitProperty();
  else if(dataType == "BOOLEAN")
    return buildUpdatedBooleanProperty();
  else if(dataType == "CHAR")
    return buildUpdatedCharProperty();
  else if(dataType == "COMPOSITE")
    return buildUpdatedCompositeProperty();
  else if(dataType == "DATETIME")
    return buildUpdatedDateTimeProperty();
  else if(dataType == "DOUBLE")
    return buildUpdatedDoubleProperty();
  else if(dataType == "FLOAT")
    return buildUpdatedFloatProperty();
  else if(dataType == "GEOMETRY")
    return buildUpdatedGeometryProperty();
  else if(dataType == "INT16")
    return buildUpdatedInt16Property();
  else if(dataType == "INT32")
    return buildUpdatedInt32Property();
  else if(dataType == "INT64")
    return buildUpdatedInt64Property();
  else if(dataType == "NUMERIC")
    return buildUpdatedNumericProperty();
  else if(dataType == "RASTER")
    return buildUpdatedRasterProperty();
  else if(dataType == "STRING")
    return buildUpdatedStringProperty();
  else if(dataType == "UCHAR")
    return buildUpdatedUcharProperty();
  else if(dataType == "UINT16")
    return buildUpdatedUint16Property();
  else if(dataType == "UINT32")
    return buildUpdatedUint32Property();
  else if(dataType == "UINT64")
    return buildUpdatedUint64Property();
  else if(dataType == "UNKNOWN")
    return buildUpdatedUnknownProperty();
  else if(dataType == "VOID")
    return buildUpdatedVoidProperty();

  return 0;
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedArrayProperty()
{
  // Get the dimension
  unsigned int dimension = arrayDimensionLineEdit->text().toUInt();

  // Build the array element property
  te::dt::SimpleProperty* arrayElementProperty;

  QString arrayElementDataType = arrayElementDataTypeComboBox->currentText();

  std::string tempPropertyName = m_propertyName;
  m_propertyName = std::string();

  std::string* tempDefaultValue = m_defaultValue;
  m_defaultValue = 0;

  arrayElementProperty = static_cast<te::dt::SimpleProperty*>(buildUpdatedProperty(arrayElementDataType));
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


te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedBitProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedBooleanProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedByteArrayProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::BYTE_ARRAY_TYPE,
                       requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedCharProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedCompositeProperty()
{
  std::string cname = std::string();

  if(compositeNameLineEdit->text().isEmpty() == false)
    cname = compositeNameLineEdit->text().toStdString();
    
  return new te::dt::CompositeProperty(cname, m_propertyName);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedDateTimeProperty()
{
  // Get the DateTime type
  te::dt::DateTimeType dateTimeType = 
    static_cast<te::dt::DateTimeType>(dateTimeTypeComboBox->currentIndex());

  // Build the DateTime property
  return new te::dt::DateTimeProperty(m_propertyName,
            dateTimeType, te::dt::SECOND, requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedDoubleProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedFloatProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedGeometryProperty()
{
  // Get the geometry subtype
  std::string gTypeStr = geometryTypeComboBox->currentText().toStdString();
  te::gm::GeomType gType = te::gm::Geometry::getGeomTypeId(gTypeStr);

  // Get the srid
  int srid = geometrySRIDLineEdit->text().toInt();

  return new te::gm::GeometryProperty(m_propertyName,
            srid, gType, false, 0);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedInt16Property()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedInt32Property()
{
  if(m_defaultValue != 0  && autoNumberCheckBox->isChecked() == false)
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedInt64Property()
{
  if(m_defaultValue != 0  && autoNumberCheckBox->isChecked() == false)
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

  return new te::dt::SimpleProperty(m_propertyName, te::dt::INT64_TYPE,
              requiredCheckBox->isChecked(), m_defaultValue);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedNumericProperty()
{
  bool ok = false;

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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedRasterProperty()
{
  return 0;
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedStringProperty()
{
  // Get the string type and the string size
  te::dt::StringType stringType;
  size_t stringSize = 0;
  bool ok = false;

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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedUcharProperty()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedUint16Property()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedUint32Property()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedUint64Property()
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

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedUnknownProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::UNKNOWN_TYPE,
              false, 0);
}

te::dt::Property* te::qt::widgets::UpdateProperty::buildUpdatedVoidProperty()
{
  return new te::dt::SimpleProperty(m_propertyName, te::dt::VOID_TYPE,
              false, 0);
}

void te::qt::widgets::UpdateProperty::closeEvent(QCloseEvent* /*e*/)
{
}