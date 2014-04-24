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
#include "GridSettingsOutside.h"
#include "GridSettingsModel.h"
#include "GridSettingsController.h"

// Qt
#include "../../../../../../third-party/qt/propertybrowser/qtvariantproperty.h"
#include "../../../../../../third-party/qt/propertybrowser/qteditorfactory.h"

te::layout::PropertiesItemPropertyBrowser::PropertiesItemPropertyBrowser(QObject* parent) :
  PropertyBrowser(parent),
  m_gridSettings(0)
{

}

te::layout::PropertiesItemPropertyBrowser::~PropertiesItemPropertyBrowser()
{
 if(m_gridSettings)
 {
   delete m_gridSettings;
   m_gridSettings = 0;
 }
}

bool te::layout::PropertiesItemPropertyBrowser::addProperty( Property property )
{
  if(PropertyBrowser::addProperty(property))
    return true;

  QtProperty* qproperty = 0;  
  QtVariantProperty* vproperty = 0;

  if(property.getType() == DataTypeGridSettings)
  {
    qproperty = m_strDlgManager->addProperty(tr(property.getName().c_str()));
    m_strDlgManager->setValue(qproperty, property.getValue().toString().c_str());
    
    foreach( Property prop, property.getSubProperty()) 
    {
      vproperty = m_variantPropertyEditorManager->addProperty(QVariant::String, tr(property.getName().c_str()));
      vproperty->setValue(property.getValue().toString().c_str());
      vproperty->setEnabled(false);
      qproperty->addSubProperty(vproperty);
    }
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
  connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
}

void te::layout::PropertiesItemPropertyBrowser::onShowGridSettingsDlg()
{
  if(!m_gridSettings)
  {
    GridSettingsModel* model = new GridSettingsModel;
    GridSettingsController* controller = new GridSettingsController(model);
    Observer* obs = const_cast<Observer*>(controller->getView());
    OutsideObserver* observer = dynamic_cast<OutsideObserver*>(obs);
    m_gridSettings = dynamic_cast<GridSettingsOutside*>(observer);
  }
  
  if(m_gridSettings)
    m_gridSettings->exec();
}