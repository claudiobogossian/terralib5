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
#include "../singleton/Context.h"
#include "../factory/ItemParamsCreate.h"
#include "../factory/AbstractItemFactory.h"

te::layout::AbstractItemController::AbstractItemController(AbstractItemModel* model)
  : m_model(model)
  , m_view(0)
{
  AbstractItemFactory* factory = Context::getInstance().getItemFactory(); 
  ItemParamsCreate params(0, 0);
  params.m_newController = this;
  params.m_newModel = m_model;

  m_view = factory->makeNew(m_model->getProperties().getTypeObj(), params);
}

te::layout::AbstractItemController::~AbstractItemController()
{
}

te::layout::AbstractItemModel* te::layout::AbstractItemController::getModel() const
{
  return m_model;
}

te::layout::AbstractItemView* te::layout::AbstractItemController::getView() const
{
  return m_view;
}
