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
  \file terralib/layout/qt/core/pattern/factory/outside/ColorDialogOutsideFactory.cpp

  \brief This is the concrete factory for arrow item.
*/

// TerraLib
#include "EditTemplateOutsideFactory.h"
#include "../../../../../outside/EditTemplateModel.h"
#include "../../../../../outside/EditTemplateController.h"
#include "../../../../outside/EditTemplateOutside.h"

te::layout::AbstractOutsideView* te::layout::EditTemplateOutsideFactory::build(OutsideFactoryParamsCreate params)
{
  EditTemplateModel* model = new EditTemplateModel();

  EditTemplateController* controller = new EditTemplateController(model);
  EditTemplateOutside* view = new EditTemplateOutside(controller);
  controller->setView(view);

  return view;
}

te::layout::EditTemplateOutsideFactory::EditTemplateOutsideFactory() :
  OutsideFactory(Enums::getInstance().getEnumObjectType()->getEditTemplate()->getName())
{

}

te::layout::EditTemplateOutsideFactory::~EditTemplateOutsideFactory()
{

}


