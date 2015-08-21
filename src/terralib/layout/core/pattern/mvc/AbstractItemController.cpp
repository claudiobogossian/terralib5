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

// TerraLib
#include "AbstractItemController.h"

#include "AbstractItemModel.h"
#include "AbstractItemView.h"
#include "../singleton/Context.h"
#include "../factory/ItemParamsCreate.h"
#include "../factory/AbstractItemFactory.h"

te::layout::AbstractItemController::AbstractItemController(AbstractItemModel* model)
  : NewObserver()
  , m_model(model)
  , m_view(0)
{
  AbstractItemFactory* factory = Context::getInstance().getItemFactory(); 
  ItemParamsCreate params(0, 0);
  params.m_newController = this;
  params.m_newModel = m_model;

  m_view = factory->makeNew(m_model->getProperties().getTypeObj(), params);

  if(m_model != 0)
  {
    m_model->attach(this);
  }
}

te::layout::AbstractItemController::~AbstractItemController()
{
  if(m_model != 0)
  {
    m_model->detach(this);
    delete m_model;
  }
}

te::layout::AbstractItemView* te::layout::AbstractItemController::getView() const
{
  return m_view;
}

te::layout::AbstractItemModel* te::layout::AbstractItemController::getModel() const
{
  return m_model;
}

const te::layout::Property& te::layout::AbstractItemController::getProperty(const std::string& propertyName) const
{
  return m_model->getProperty(propertyName);
}

void te::layout::AbstractItemController::update(const te::layout::Subject* subject)
{
  const Property& property = m_model->getProperty("rotation");
  if(property.getValue().toDouble() != m_view->getItemRotation()){
    m_view->setItemRotation(property.getValue().toDouble());
  }
}

bool te::layout::AbstractItemController::contains(const te::gm::Coord2D &coord) const
{
  return m_model->contains(coord);
}


