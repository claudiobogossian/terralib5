/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PropertyBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesItemPropertyBrowser.h"
#include "../outside/GridSettingsOutside.h"
#include "../../outside/GridSettingsModel.h"
#include "../../outside/GridSettingsController.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/enum/Enums.h"

// STL
#include <utility>

// Qt
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>

te::layout::PropertiesItemPropertyBrowser::PropertiesItemPropertyBrowser(QObject* parent) :
  PropertyBrowser(parent),
  m_gridSettings(0)
{

}

te::layout::PropertiesItemPropertyBrowser::~PropertiesItemPropertyBrowser()
{
  PropertyBrowser::clearAll();

  m_dlgProps.clear();

  closeAllWindows();
}

void te::layout::PropertiesItemPropertyBrowser::clearAll()
{
  PropertyBrowser::clearAll();

  m_dlgProps.clear();
}

bool te::layout::PropertiesItemPropertyBrowser::addProperty( Property property )
{
  if(PropertyBrowser::addProperty(property))
    return true;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  QtProperty* qproperty = 0;

  if(property.getType() == dataType->getDataTypeGridSettings()
    || property.getType() == dataType->getDataTypeImage())
  {
    qproperty = m_strDlgManager->addProperty(tr(property.getName().c_str()));
    m_strDlgManager->setValue(qproperty, property.getValue().toString().c_str());
    
    /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/
    m_dlgProps.insert(std::pair<std::string, Property>(property.getName(),property));
  }
  
  if(qproperty)
  {
    addPropertyItem(qproperty, QLatin1String(property.getName().c_str()));
    return true;
  }

  return false;
}

void te::layout::PropertiesItemPropertyBrowser::onSetDlg(QWidget *parent, QtProperty * prop)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string name = prop->propertyName().toStdString();

  Property propt = findDlgProperty(name);
  if(propt.getType() == dataType->getDataTypeNone())
    return;

  if(propt.getType() == dataType->getDataTypeGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
  }
  if(propt.getType() == dataType->getDataTypeImage())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowImageDlg()));
  }
}

void te::layout::PropertiesItemPropertyBrowser::onShowGridSettingsDlg()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property prop = findDlgProperty(dataType->getDataTypeGridSettings());

  if(!m_gridSettings)
  {
    GridSettingsModel* model = new GridSettingsModel;
    model->setOutsideProperty(prop);
    GridSettingsController* controller = new GridSettingsController(model);
    
    Observer* obs = const_cast<Observer*>(controller->getView());
    OutsideObserver* observer = dynamic_cast<OutsideObserver*>(obs);
    m_gridSettings = dynamic_cast<GridSettingsOutside*>(observer);
    connect(m_gridSettings, SIGNAL(updateProperty()), this, SLOT(onUpdateGridSettingsProperty()));
  }
  
  if(m_gridSettings)
  {
    if(prop.getType() == dataType->getDataTypeGridSettings())
    {
      Observable* obs = dynamic_cast<Observable*>(m_gridSettings->getModel());
      if(obs)
      {
        GridSettingsModel* modelObs = dynamic_cast<GridSettingsModel*>(obs);
        if(modelObs)
        {
          modelObs->setOutsideProperty(prop);
        }

        ItemController* itCtrl = dynamic_cast<ItemController*>(m_gridSettings->getController());
        if(itCtrl)
        {
          GridSettingsController* gridController = dynamic_cast<GridSettingsController*>(itCtrl);
          if(gridController)
          {
            gridController->clearUpdate();
          }
        }
      }

      m_gridSettings->load();
      m_gridSettings->show();
    }
  }
}

void te::layout::PropertiesItemPropertyBrowser::onShowImageDlg()
{
  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QString path = QFileDialog::getOpenFileName(m_propertyEditor, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(m_propertyEditor, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }
  else
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop = findDlgProperty(dataType->getDataTypeImage());
    prop.setValue(path.toStdString(), dataType->getDataTypeImage());
    
    QVariant v(path);
    changeValueQtPropertyDlg(prop.getName(), v);
    
    emit updateOutside(prop);
  }
}

void te::layout::PropertiesItemPropertyBrowser::onUpdateGridSettingsProperty()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_gridSettings->getController());
  if(controller)
  {
    Property prop = controller->updateProperty();

    if(prop.isNull())
      return;

    emit updateOutside(prop);
  }
}

void te::layout::PropertiesItemPropertyBrowser::blockOpenWindows( bool block )
{
  if(m_gridSettings)
  {
    if(block)
    {
      m_gridSettings->blockComponents();
    }
    else
    {
      m_gridSettings->load();
      m_gridSettings->unblockComponents();
    }
  }
}

void te::layout::PropertiesItemPropertyBrowser::closeAllWindows()
{
  if(m_gridSettings)
  {
    if(!m_gridSettings->isHidden())
    {
      m_gridSettings->close();
    }
  }
}

te::layout::Property te::layout::PropertiesItemPropertyBrowser::findDlgProperty( std::string name )
{
  Property prop;

  std::string propName;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    propName = it->first;
    if(name.compare(propName) == 0)
    {
      prop = it->second;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::PropertiesItemPropertyBrowser::findDlgProperty( te::layout::EnumType* dataType )
{
  Property prop;

  Property pro;
  std::map<std::string, Property>::iterator it;

  for (it = m_dlgProps.begin(); it != m_dlgProps.end(); ++it) {
    pro = it->second;
    if(pro.getType() == dataType)
    {
      prop = pro;
      break;
    }
  }

  return prop;
}

te::layout::Property te::layout::PropertiesItemPropertyBrowser::getProperty( std::string name )
{
  Property prop = PropertyBrowser::getProperty(name);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  if(prop.getType() != dataType->getDataTypeNone())
    return prop;

  QVariant variant = findPropertyValue(name);

  if(prop.getType() == dataType->getDataTypeGridSettings())
  {
    prop.setValue(variant.toString().toStdString(), prop.getType());
  }
  else if(prop.getType() == dataType->getDataTypeImage())
  {
    prop.setValue(variant.toString().toStdString(), prop.getType());
  }

  return prop;
}

te::layout::EnumType* te::layout::PropertiesItemPropertyBrowser::getLayoutType( QVariant::Type type, std::string name /*= ""*/ )
{
  Property prop;
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  EnumType* dataType = PropertyBrowser::getLayoutType(type, name);

  if(dataType != dtType->getDataTypeNone())
    return dataType;

  switch(type)
  {
  case QVariant::String:
    {
      dataType = dtType->getDataTypeString();

      //Custom types: Dialog Window Type
      if(name.compare("") != 0)
      {
        prop = findDlgProperty(name);
        if(!prop.getValue().isNull())
        {
          if(prop.getType() == dtType->getDataTypeGridSettings())
          {
            dataType = dtType->getDataTypeGridSettings();
          }
          if(prop.getType() == dtType->getDataTypeImage())
          {
            dataType = dtType->getDataTypeImage();
          }
        }
      }
    }
    break;
  default:
    prop.setValue(0, dtType->getDataTypeNone());
  }

  return dataType;
}

int te::layout::PropertiesItemPropertyBrowser::getVariantType( te::layout::EnumType* dataType )
{
  int type = PropertyBrowser::getVariantType(dataType);

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  if(type != QVariant::Invalid)
    return type;

  if(dataType == dtType->getDataTypeGridSettings())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeImage())
  {
      type = QVariant::String;
  }
  else
  {
    type = QVariant::Invalid;
  }

  return type;
}

void te::layout::PropertiesItemPropertyBrowser::changeValueQtPropertyDlg( std::string name, QVariant variant )
{
  QList<QtProperty*> list = m_propertyEditor->properties();
  foreach( QtProperty* prop, list) 
  {
    if(prop->propertyName().toStdString().compare(name))
    {
      m_strDlgManager->setValue(prop, variant.toString());
    }
  }
}
