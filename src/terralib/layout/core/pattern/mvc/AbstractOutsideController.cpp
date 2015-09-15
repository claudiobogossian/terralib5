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
  \file AbstractOutsideController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractOutsideController.h"
#include "AbstractOutsideView.h"
#include "AbstractOutsideModel.h"
#include "../../enum/EnumType.h"
#include "../factory/AbstractOutsideFactory.h"
#include "../factory/OutsideParamsCreate.h"
#include "../singleton/Context.h"

te::layout::AbstractOutsideController::AbstractOutsideController(AbstractOutsideModel* o) :
  m_model(o),
  m_view(0)
{
  if (m_model != 0)
  {
    m_model->attach(this);
  }
}

te::layout::AbstractOutsideController::~AbstractOutsideController()
{
  if (m_model != 0)
  {
    m_model->detach(this);
    delete m_model;
    m_model = 0;
  }
}

te::layout::AbstractOutsideModel* te::layout::AbstractOutsideController::getModel() const
{
  return m_model;
}

te::layout::AbstractOutsideView* te::layout::AbstractOutsideController::getView() const
{
  return m_view;
}

void te::layout::AbstractOutsideController::setView(AbstractOutsideView* view)
{
  m_view = view;
}

void te::layout::AbstractOutsideController::update(const Subject* subject)
{
  
}

void te::layout::AbstractOutsideController::setPosition(const double& x, const double& y)
{
  if(m_model)
  {
    AbstractOutsideModel* model = dynamic_cast<AbstractOutsideModel*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}


