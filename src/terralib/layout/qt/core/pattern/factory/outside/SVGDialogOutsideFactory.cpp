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
  \file terralib/layout/qt/core/pattern/factory/outside/SVGDialogOutsideFactory.cpp

  \brief This is the concrete factory for arrow item.
*/

// TerraLib
#include "SVGDialogOutsideFactory.h"
#include "../../../../../outside/SVGDialogModel.h"
#include "../../../../../outside/SVGDialogController.h"
#include "../../../../outside/SVGDialogOutside.h"

te::layout::AbstractOutsideView* te::layout::SVGDialogOutsideFactory::build(OutsideFactoryParamsCreate params)
{
  SVGDialogModel* model = new SVGDialogModel();

  SVGDialogController* controller = new SVGDialogController(model);
  SVGDialogOutside* view = new SVGDialogOutside(controller);
  controller->setView(view);

  return view;
}

te::layout::SVGDialogOutsideFactory::SVGDialogOutsideFactory() :
  OutsideFactory(Enums::getInstance().getEnumObjectType()->getSVGDialog()->getName())
{

}

te::layout::SVGDialogOutsideFactory::~SVGDialogOutsideFactory()
{

}


