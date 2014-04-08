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
  \file ToolbarWindowLayoutController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarWindowLayoutController.h"
#include "LayoutOutsideParamsCreate.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutAbstractOutsideFactory.h"
#include "LayoutContext.h"

te::layout::ToolbarWindowLayoutController::ToolbarWindowLayoutController( LayoutObservable* o ) :
	LayoutOutsideController(o)
{
	LayoutAbstractOutsideFactory* factory = LayoutContext::getInstance()->getOutsideFactory(); 
	LayoutOutsideParamsCreate params(this, m_model);
  if(factory)
	  m_view = (LayoutObserver*)factory->make(TPToolbar, params);
}

te::layout::ToolbarWindowLayoutController::~ToolbarWindowLayoutController()
{

}

void te::layout::ToolbarWindowLayoutController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    LayoutOutsideModelObservable* model = dynamic_cast<LayoutOutsideModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}
