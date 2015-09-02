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
  \file BuildGraphicsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildGraphicsOutside.h"
#include "../../core/enum/Enums.h"
#include "pattern/factory/outside/NewOutsideFactory.h"
#include "pattern/factory/outside/OutsideFactoryParamsCreate.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"

// Qt
#include <QWidget>

// STL
#include <sstream>
#include <string>  

te::layout::BuildGraphicsOutside::BuildGraphicsOutside() 
{
 
}

te::layout::BuildGraphicsOutside::~BuildGraphicsOutside()
{
 
}

QWidget* te::layout::BuildGraphicsOutside::createOuside( te::layout::EnumType* type )
{
  QWidget* widget = 0;
    
  OutsideFactoryParamsCreate params;

  std::string name = type->getName();

  AbstractOutsideView* abstractItem = te::layout::NewOutsideFactory::make(name, params);
  widget = dynamic_cast<QWidget*>(abstractItem);
  
  return widget;
}


