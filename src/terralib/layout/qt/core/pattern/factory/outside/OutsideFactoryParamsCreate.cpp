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
  \file terralib/layout/qt/core/pattern/factory/outside/OutsideFactoryParamsCreate.cpp

  \brief Parameters to create a new item object (MVC graphic Object). 
*/

// TerraLib
#include "OutsideFactoryParamsCreate.h"

te::layout::OutsideFactoryParamsCreate::OutsideFactoryParamsCreate(const OutsideFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::OutsideFactoryParamsCreate::OutsideFactoryParamsCreate()
{

}

te::layout::OutsideFactoryParamsCreate::~OutsideFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::OutsideFactoryParamsCreate::clone() const
{
  return new OutsideFactoryParamsCreate(*this);
}

void te::layout::OutsideFactoryParamsCreate::reset() throw(te::common::Exception)
{
  //do nothing
}



