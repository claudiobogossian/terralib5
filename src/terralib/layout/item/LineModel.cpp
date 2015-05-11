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
  \file LineModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../../geometry/Point.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/Utils.h"
#include "../core/pattern/mvc/ItemObserver.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"

te::layout::LineModel::LineModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getLineItem();

  m_borderColor = te::color::RGBAColor(255, 255, 255, 0);
  m_box = te::gm::Envelope(0., 0., 20., 20.);

  m_lineColor = te::color::RGBAColor(0, 0, 0, 255);

  m_border = false;
}

te::layout::LineModel::~LineModel()
{
  if(!m_coords.empty())
  {
    std::vector<te::gm::Point*>::iterator ito;

    for(ito = m_coords.begin() ; ito != m_coords.end() ; ++ito)
    { 
      if(*ito)
      {
        te::gm::Point* pt = *ito;
        if(pt)
        {
          delete pt;
          pt = 0;
        }
      }
    }

    m_coords.clear();
  }
}

void te::layout::LineModel::setCoords( std::vector<te::gm::Point*> coords )
{
  m_coords = coords;

  int sizeMCoords = m_coords.size();

  te::gm::LinearRing *lineOfPoints = new te::gm::LinearRing(sizeMCoords, te::gm::LineStringType);
  
  for(int i = 0; i < sizeMCoords ; ++i)
  {
    lineOfPoints->setPointN( i, te::gm::Point(m_coords[i]->getX(), m_coords[i]->getY()));    
  }

  const te::gm::Envelope *returnBox = lineOfPoints->getMBR();
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  x1 = returnBox->getLowerLeftX();
  y1 = returnBox->getLowerLeftY();
  x2 = returnBox->getUpperRightX();
  y2 = returnBox->getUpperRightY();

  m_box = te::gm::Envelope(x1, y1, x2, y2);

  if (lineOfPoints)
  {
    delete lineOfPoints;
    lineOfPoints = 0;
  }  

  double x = m_box.getLowerLeftX();
  double y = m_box.getLowerLeftY();

  te::gm::Coord2D coord (x, y);

  ContextItem context;
  context.setPos(coord);
  context.setChangePos(true);

  notifyAll(context);
}

std::vector<te::gm::Point*> te::layout::LineModel::getCoords()
{
  return m_coords;
}

te::color::RGBAColor te::layout::LineModel::getLineColor()
{
  return m_lineColor;
}

te::layout::Properties* te::layout::LineModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_linecolor(m_hashCode);
  pro_linecolor.setName("line_color");
  pro_linecolor.setLabel("line color");
  pro_linecolor.setValue(pro_linecolor, dataType->getDataTypeColor());
  pro_linecolor.setMenu(true);
  m_properties->addProperty(pro_linecolor);

  return m_properties;
}

void te::layout::LineModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_linecolor = vectorProps->contains("line_color");
  if(!pro_linecolor.isNull())
  {
    m_lineColor = pro_linecolor.getValue().toColor();
  }
}

