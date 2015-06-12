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
  \file TextGridSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridSettingsController.h"
#include "../core/pattern/mvc/OutsideModelObservable.h"
#include "../core/pattern/mvc/Observable.h"
#include "TextGridSettingsModel.h"

te::layout::TextGridSettingsController::TextGridSettingsController( Observable* o ) :
	OutsideController(o)
{
 
}

te::layout::TextGridSettingsController::~TextGridSettingsController()
{

}

te::layout::Property te::layout::TextGridSettingsController::updateProperty()
{
  return m_update;
}

void te::layout::TextGridSettingsController::addUpdateProperty( std::string name, Variant variant )
{
  TextGridSettingsModel* outsideModel = 0;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<TextGridSettingsModel*>(model);    
  }

  if(!outsideModel)
    return;

  if(m_update.isNull())
  {
    Property prop = outsideModel->getOutsideProperty();

    m_update.setEditable(prop.isEditable());

    m_update.setName(prop.getName());
    m_update.setValue(prop.getValue());
  }

  Property property = outsideModel->containsOutsideSubProperty(name);

  if(!property.isNull())
  {
    if(property.getOptionChoices().empty())
    {
      if(property.getValue() != variant)
      {
        property.setValue(variant);
        outsideModel->updateOutsideSubProperty(property);
      }
      else
      {
        return;
      }
    }
    else
    {
      if(property.getOptionByCurrentChoice() != variant)
      {
        property.setOptionChoice(variant);
        outsideModel->updateOutsideSubProperty(property);
      }
      else
      {
        return;
      }
    }

    addUpdateTextGridProperty(property);
  } 
}

void te::layout::TextGridSettingsController::addUpdateTextGridProperty( Property subProperty )
{
  TextGridSettingsModel* outsideModel = 0;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<TextGridSettingsModel*>(model);    
  }

  if(!outsideModel)
    return;

  if(m_textGridSettings.isNull())
  {
    Property prop = outsideModel->getOutsideProperty();
    m_textGridSettings.setEditable(prop.isEditable());

    m_textGridSettings.setName(prop.getName());
    m_textGridSettings.setValue(prop.getValue());
  }

  if(!subProperty.isNull())
  {
    if(m_textGridSettings.containsSubProperty(subProperty))
    {
      m_textGridSettings.removeSubProperty(subProperty);
    }
    m_textGridSettings.addSubProperty(subProperty);

    if(m_update.containsSubProperty(subProperty))
    {
      m_update.removeSubProperty(subProperty);
    }
    m_update.addSubProperty(subProperty);
  }
}

void te::layout::TextGridSettingsController::clearUpdate()
{
  m_update.clear();
  m_textGridSettings.clear();
}

te::layout::Property te::layout::TextGridSettingsController::getProperty( std::string name )
{
  TextGridSettingsModel* outsideModel = 0;
  Property prop;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<TextGridSettingsModel*>(model);    
  }

  if(!outsideModel)
    return prop;

  prop = outsideModel->containsOutsideSubProperty(name);

  return prop;
}

