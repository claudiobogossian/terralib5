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
  \file MapGridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapGridModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "GridModel.h"
#include "../../../maptools/Canvas.h"
#include "Property.h"
#include "Properties.h"
#include "AbstractType.h"

// STL
#include <vector>

te::layout::MapGridModel::MapGridModel() :
  MapModel(),
  m_grid(0),
  m_gridPlanar(true),
  m_gridGeodesic(false)
{
  m_backgroundColor = te::color::RGBAColor(0, 255, 255, 200);

  m_grid = new GridModel;
}

te::layout::MapGridModel::~MapGridModel()
{
  if(m_grid)
  {
    delete m_grid;
    m_grid = 0;
  }
}

void te::layout::MapGridModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  m_grid->setGridPlanar(m_gridPlanar);
  m_grid->draw(canvas, m_box);

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  te::gm::Envelope newBox(m_box.getLowerLeftX() + 10, m_box.getLowerLeftY(), 
    m_box.getLowerLeftX() + 10, m_box.getUpperRightY());
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);

  line = utils->createSimpleLine(newBox);
  utils->drawLineW(line);

  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);

  //utils->drawRectW(m_box);
    
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

te::layout::Properties* te::layout::MapGridModel::getProperties() const
{
  MapModel::getProperties();

  if(m_grid)
  {
    Properties* propGridModel = const_cast<Properties*>(m_grid->getProperties());
    std::vector<Property> props = propGridModel->getProperties();

    Property pro_grid;
    pro_grid.setName("grid");
    pro_grid.setId("unknown");
    std::string sValue = "Settings";
    pro_grid.setValue(sValue, DataTypeGridSettings);
    
    for(unsigned int i = 0 ; i < props.size() ; ++i)
    {
      Property prop = props[i];
      pro_grid.addSubProperty(prop);
    }

    m_properties->addProperty(pro_grid);
  }

  return m_properties;
}

void te::layout::MapGridModel::updateProperties( te::layout::Properties* properties )
{

}


