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
  \file MapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../maptools/Canvas.h"

te::layout::MapModel::MapModel()
{
  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 255);
  m_box = te::gm::Envelope(0., 0., 100., 70.);
}

te::layout::MapModel::~MapModel()
{

}

void te::layout::MapModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::MapModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  updateVisitors();
}

void te::layout::MapModel::setLayers( std::list<te::map::AbstractLayerPtr> layers )
{
  m_layers = layers;
  
  updateVisitors();
}

std::list<te::map::AbstractLayerPtr> te::layout::MapModel::getLayers()
{
  return m_layers;
}

