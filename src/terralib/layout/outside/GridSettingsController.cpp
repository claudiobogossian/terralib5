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
#include "../core/pattern/factory/OutsideParamsCreate.h"
#include "../core/pattern/mvc/OutsideModelObservable.h"
#include "../core/pattern/factory/AbstractOutsideFactory.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/pattern/mvc/Observable.h"
#include "GridSettingsModel.h"
#include "../core/enum/Enums.h"

te::layout::GridSettingsController::GridSettingsController( Observable* o ) :
	OutsideController(o)
{
  EnumType* type = Enums::getInstance().getEnumObjectType()->getGridSettings();
  o->setType(type);

	AbstractOutsideFactory* factory = Context::getInstance().getOutsideFactory(); 
	OutsideParamsCreate params(this, m_model);
  if(factory)
	  m_view = (Observer*)factory->make(m_model->getType(), params);
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
  m_gridGeodesic.clear();
  m_gridPlanar.clear();
}

void te::layout::GridSettingsController::addUpdateProperty( std::string name, Variant variant, LayoutGridType gridType )
{
  GridSettingsModel* outsideModel = 0;

  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    outsideModel = dynamic_cast<GridSettingsModel*>(model);    
  }
 
  if(!outsideModel)
    return;

  if(m_update.isNull())
  {
    Property prop = outsideModel->getOutsideProperty();

    m_update.setEditable(prop.isEditable());
    m_update.setId(prop.getId());

    m_update.setName(prop.getName());
    m_update.setValue(prop.getValue());
  }

  Property property = outsideModel->containsOutsideSubProperty(name, gridType);

  if(!property.isNull())
  {
    if(property.getOptionChoices().empty())
    {
      if(property.getValue() != variant)
      {
        property.setValue(variant);
        outsideModel->updateOutsideSubProperty(property, gridType);
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
        outsideModel->updateOutsideSubProperty(property, gridType);
      }
      else
      {
        return;
      }
    }

    if(gridType == TypeGeodesic)
    {
      addUpdateGeodesicProperty(outsideModel, property, gridType);
    }
    if(gridType == TypePlanar)
    {
      addUpdatePlanarProperty(outsideModel, property, gridType);
    }
  } 
}

void te::layout::GridSettingsController::addUpdateGeodesicProperty( GridSettingsModel* outsideModel, Property subProperty, LayoutGridType gridType )
{
  if(m_gridGeodesic.isNull())
  {
    Property prop = outsideModel->containsGrid(gridType);

    m_gridGeodesic.setEditable(prop.isEditable());
    m_gridGeodesic.setId(prop.getId());

    m_gridGeodesic.setName(prop.getName());
    m_gridGeodesic.setValue(prop.getValue());
  }

  if(!subProperty.isNull())
  {
    if(m_gridGeodesic.containsSubProperty(subProperty))
    {
      m_gridGeodesic.removeSubProperty(subProperty);
    }
    m_gridGeodesic.addSubProperty(subProperty);

    if(m_update.containsSubProperty(m_gridGeodesic))
    {
      m_update.removeSubProperty(m_gridGeodesic);
    }
    m_update.addSubProperty(m_gridGeodesic);
  }
}

void te::layout::GridSettingsController::addUpdatePlanarProperty( GridSettingsModel* outsideModel, Property subProperty, LayoutGridType gridType )
{
  if(m_gridPlanar.isNull())
  {
    Property prop = outsideModel->containsGrid(gridType);

    m_gridPlanar.setEditable(prop.isEditable());
    m_gridPlanar.setId(prop.getId());

    m_gridPlanar.setName(prop.getName());
    m_gridPlanar.setValue(prop.getValue());
  }

  if(!subProperty.isNull())
  {
    if(m_gridPlanar.containsSubProperty(subProperty))
    {
      m_gridPlanar.removeSubProperty(subProperty);
    }
    m_gridPlanar.addSubProperty(subProperty);

    if(m_update.containsSubProperty(m_gridPlanar))
    {
      m_update.removeSubProperty(m_gridPlanar);
    }
    m_update.addSubProperty(m_gridPlanar);
  }
}

te::layout::Property te::layout::GridSettingsController::getProperty( std::string name, LayoutGridType gridType )
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

  prop = outsideModel->containsOutsideSubProperty(name, gridType);

  return prop;
}
