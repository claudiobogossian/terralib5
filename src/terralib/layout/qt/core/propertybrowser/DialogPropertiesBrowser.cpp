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
  \file DialogPropertiesBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DialogPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/Font.h"

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>

te::layout::DialogPropertiesBrowser::DialogPropertiesBrowser(QObject* parent) :
  AbstractPropertiesBrowser(parent),
  m_strDlgManager(0),
  m_dlgEditorFactory(0)
{
  createManager();
}

te::layout::DialogPropertiesBrowser::DialogPropertiesBrowser( QtDlgEditorFactory* factory, QtStringPropertyManager* manager, QObject *parent /*= 0*/ ):
  AbstractPropertiesBrowser(parent),
  m_strDlgManager(manager),
  m_dlgEditorFactory(factory)
{
  if(m_dlgEditorFactory)
  {
    connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
  }
}

te::layout::DialogPropertiesBrowser::~DialogPropertiesBrowser()
{
  closeAllWindows();

  m_dlgProps.clear();

  if(m_strDlgManager)
  {
    delete m_strDlgManager;
    m_strDlgManager = 0;
  }

  if(m_dlgEditorFactory)
  {
    delete m_dlgEditorFactory;
    m_dlgEditorFactory = 0;
  }
}

void te::layout::DialogPropertiesBrowser::createManager()
{
  m_strDlgManager = new QtStringPropertyManager(this);
  
  m_dlgEditorFactory = new QtDlgEditorFactory(this);

  connect(m_dlgEditorFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(onSetDlg(QWidget *, QtProperty *)));
}

void te::layout::DialogPropertiesBrowser::onSetDlg( QWidget *parent, QtProperty * prop )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return;
  }

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
  if(propt.getType() == dataType->getDataTypeTextGridSettings())
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowTextGridSettingsDlg()));
  }
}

QtStringPropertyManager* te::layout::DialogPropertiesBrowser::getStringPropertyManager()
{
  return m_strDlgManager;
}

QtDlgEditorFactory* te::layout::DialogPropertiesBrowser::getDlgEditorFactory()
{
  return m_dlgEditorFactory;
}

bool te::layout::DialogPropertiesBrowser::changeQtPropertyValue(QtProperty* pproperty, Property property)
{
  QColor qcolor;
  QFont qfont;
  te::color::RGBAColor color;
  Font font;

  m_changeProperty = true;

  if(!pproperty)
  {
    return false;
  }
    
  m_changeProperty = false;

  return true;
}

QtProperty* te::layout::DialogPropertiesBrowser::addProperty( Property property )
{
  QtProperty* qproperty = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  if(dataType)
  {
    return qproperty;
  }

  if(!m_strDlgManager)
  {
    return qproperty;
  }

  qproperty = m_strDlgManager->addProperty(tr(property.getName().c_str()));
  m_strDlgManager->setValue(qproperty, property.getValue().toString().c_str());
  addPropertyItem(qproperty, QLatin1String(property.getName().c_str()));
  /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/
  m_dlgProps.insert(std::pair<std::string, Property>(property.getName(),property));

  return qproperty;
}

bool te::layout::DialogPropertiesBrowser::checkDlgType( Property prop )
{
  bool result = false;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(prop.getType() == dataType->getDataTypeGridSettings()
    || prop.getType() == dataType->getDataTypeImage()
    || prop.getType() == dataType->getDataTypeTextGridSettings())
  {
    result = true;
  }

  return result;
}

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty( std::string name )
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

te::layout::Property te::layout::DialogPropertiesBrowser::findDlgProperty( EnumType* dataType )
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

void te::layout::DialogPropertiesBrowser::changeValueQtPropertyDlg( std::string name, QVariant variant )
{
  if(!m_strDlgManager)
  {
    return;
  }

  QSet<QtProperty*> list = m_strDlgManager->properties();
  foreach( QtProperty* prop, list) 
  {
    if(prop->propertyName().toStdString().compare(name))
    {
      m_strDlgManager->setValue(prop, variant.toString());
    }
  }
}

void te::layout::DialogPropertiesBrowser::onShowGridSettingsDlg( Property property )
{

}

void te::layout::DialogPropertiesBrowser::onShowImageDlg( Property property )
{
  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QDesktopWidget* wg = QApplication::desktop();
  QFileDialog dialog;

  //Put the dialog in the screen center
  QRect screen = wg->screenGeometry();
  dialog.move( screen.center() - dialog.rect().center() );

  QString path = dialog.getOpenFileName(wg, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(wg, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }
  else
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop = findDlgProperty(dataType->getDataTypeImage());
    prop.setValue(path.toStdString(), dataType->getDataTypeImage());

    QVariant v(path);
    changeValueQtPropertyDlg(prop.getName(), v);

    emit changeDlgProperty(prop);
  }
}

void te::layout::DialogPropertiesBrowser::onShowTextGridSettingsDlg( Property property )
{

}

te::layout::Property te::layout::DialogPropertiesBrowser::getProperty( std::string name )
{
  Property prop;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return prop;
  }
  
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

te::layout::EnumType* te::layout::DialogPropertiesBrowser::getLayoutType( QVariant::Type type, std::string name /*= ""*/ )
{
  Property prop;
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  EnumType* dataType = 0;

  if(!dtType)
  {
    return 0;
  }
  
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

int te::layout::DialogPropertiesBrowser::getVariantType( EnumType* dataType )
{
  int type = QVariant::Invalid;

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  if(!dtType)
  {
    return type;
  }
  
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

bool te::layout::DialogPropertiesBrowser::updateProperty( Property property )
{
  std::string name = property.getName();
  QtProperty* qprop = findProperty(name);
  QColor qcolor;
  te::color::RGBAColor color;
  Font font;
  QFont qfont;
  
  if(!qprop)
  {
    return false;
  }  

  return changeQtPropertyValue(qprop, property);
}

std::map<std::string, te::layout::Property> te::layout::DialogPropertiesBrowser::getDlgProps()
{
  return m_dlgProps;
}

void te::layout::DialogPropertiesBrowser::closeAllWindows()
{

}










