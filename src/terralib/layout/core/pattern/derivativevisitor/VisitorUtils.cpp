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
  \file VisitorUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VisitorUtils.h"
#include "../../enum/AbstractType.h"
#include "../mvc/ItemObserver.h"
#include "../../../item/MapModel.h"
#include "../../enum/Enums.h"
#include "AbstractVisitor.h"

// STL
#include <stddef.h>  // defines NULL

// Boost
#include <boost/foreach.hpp>

te::layout::VisitorUtils::VisitorUtils()
{

}

te::layout::VisitorUtils::~VisitorUtils()
{

}

bool te::layout::VisitorUtils::changeMapVisitable( QList<QGraphicsItem*> graphicsItems, Visitable* visitable )
{
  bool result = true;
  /*te::layout::MapModel* mpModel = dynamic_cast<te::layout::MapModel*>(visitable);

  if(!mpModel)
    return false;

  foreach( QGraphicsItem *it, graphicsItems) 
  {
    if(!it)
      continue;

    te::layout::ItemObserver* lIt = dynamic_cast<te::layout::ItemObserver*>(it);
    if(!lIt)
      continue;

    te::layout::ItemModelObservable* model = dynamic_cast<te::layout::ItemModelObservable*>(lIt->getModel());
    if(!model)
      continue;

    AbstractVisitor* visit = dynamic_cast<AbstractVisitor*>(model);
    if(!visit)
    {
      result = false;
      break;
    }

    mpModel->acceptVisitor(visit);
  }
  */

  return result;
}
