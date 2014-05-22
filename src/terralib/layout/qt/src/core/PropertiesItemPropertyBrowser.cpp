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
#include "ItemController.h"

te::layout::PropertiesItemPropertyBrowser::PropertiesItemPropertyBrowser(QObject* parent) :
  PropertyBrowser(parent),
  m_gridSettings(0)
{

}

te::layout::PropertiesItemPropertyBrowser::~PropertiesItemPropertyBrowser()
{
  PropertyBrowser::clearAll();

  closeAllWindows();
}

void te::layout::PropertiesItemPropertyBrowser::clearAll()
{
  PropertyBrowser::clearAll();
  
  m_propGridSettingsName = "";
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
    
    /*The sub properties should not appear in this case, 
    because will be previewed in the dialog window will be opened.*/
    m_propGridSettingsName = property.getName();
    m_dlgProperty = property;
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
  if(prop->propertyName().toStdString().compare(m_propGridSettingsName) == 0)
  {
    connect(parent, SIGNAL(showDlg()), this, SLOT(onShowGridSettingsDlg()));
  }
}

void te::layout::PropertiesItemPropertyBrowser::onShowGridSettingsDlg()
{
  if(!m_gridSettings)
  {
    GridSettingsModel* model = new GridSettingsModel;
    model->setOutsideProperty(m_dlgProperty);
    GridSettingsController* controller = new GridSettingsController(model);
    
    Observer* obs = const_cast<Observer*>(controller->getView());
    OutsideObserver* observer = dynamic_cast<OutsideObserver*>(obs);
    m_gridSettings = dynamic_cast<GridSettingsOutside*>(observer);
    connect(m_gridSettings, SIGNAL(updateProperty()), this, SLOT(onUpdateGridSettingsProperty()));
  }
  
  if(m_gridSettings)
  {
    if(m_propGridSettingsName.compare("") != 0)
    {
      Observable* obs = dynamic_cast<Observable*>(m_gridSettings->getModel());
      if(obs)
      {
        GridSettingsModel* modelObs = dynamic_cast<GridSettingsModel*>(obs);
        if(modelObs)
        {
          modelObs->setOutsideProperty(m_dlgProperty);
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

void te::layout::PropertiesItemPropertyBrowser::blockOpenGridWindows( bool block )
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
