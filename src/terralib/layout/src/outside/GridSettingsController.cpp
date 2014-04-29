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
  \file GridSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsController.h"
#include "OutsideParamsCreate.h"
#include "OutsideModelObservable.h"
#include "AbstractOutsideFactory.h"
#include "Context.h"
#include "Observable.h"
#include "GridSettingsModel.h"

te::layout::GridSettingsController::GridSettingsController( Observable* o ) :
	OutsideController(o)
{
	AbstractOutsideFactory* factory = Context::getInstance()->getOutsideFactory(); 
	OutsideParamsCreate params(this, m_model);
  if(factory)
	  m_view = (Observer*)factory->make(TPGridSettings, params);
}

te::layout::GridSettingsController::~GridSettingsController()
{

}

void te::layout::GridSettingsController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}

te::layout::Property te::layout::GridSettingsController::updateProperty()
{
  return m_update;
}

void te::layout::GridSettingsController::clearUpdate()
{
  m_update.clear();
}

void te::layout::GridSettingsController::addUpdateProperty( std::string name, Variant variant )
{
  GridSettingsModel* outsideModel = 0;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<GridSettingsModel*>(model);    
  }
 
  if(!outsideModel)
    return;

  Property property = outsideModel->containsOutsideSubProperty(name);

  if(m_update.isNull())
  {
    Property prop = outsideModel->getOutsideProperty();

    m_update.setEditable(prop.isEditable());
    m_update.setId(prop.getId());

    m_update.setName(prop.getName());
    m_update.setValue(prop.getValue());
  }
  
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

    if(m_update.containsSubProperty(property))
    {
      m_update.removeSubProperty(property);
    }
    m_update.addSubProperty(property);
  }
}

te::layout::Property te::layout::GridSettingsController::getProperty( std::string name )
{
  GridSettingsModel* outsideModel = 0;
  Property prop;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<GridSettingsModel*>(model);    
  }

  if(!outsideModel)
    return prop;

  prop = outsideModel->containsOutsideSubProperty(name);

  return prop;
}
