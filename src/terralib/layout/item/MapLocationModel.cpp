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
  \file MapLocationModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapLocationModel.h"
#include "../core/enum/Enums.h"
// STL
#include <vector>
#include <string>
#include <sstream> 

te::layout::MapLocationModel::MapLocationModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getMapLocationItem();
}

te::layout::MapLocationModel::~MapLocationModel()
{
  
}



