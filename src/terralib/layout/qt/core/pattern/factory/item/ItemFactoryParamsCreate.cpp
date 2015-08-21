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
  \file terralib/layout/qt/core/pattern/factory/ItemFactoryParamsCreate.cpp

  \brief Parameters to create a new item object (MVC graphic Object). 
*/

// TerraLib
#include "ItemFactoryParamsCreate.h"

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(const ItemFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(std::string name, int zValue, int id, te::gm::Coord2D coord /*= te::gm::Coord2D()*/, Properties props /*= 0*/) :
  m_name(name),
  m_coord(coord),
  m_zValue(zValue),
  m_id(id)
{

}

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(std::string name, int zValue, te::gm::Coord2D coord /*= te::gm::Coord2D()*/) :
  m_name(name),
  m_coord(coord),
  m_zValue(zValue),
  m_id(0)
{

}

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(Properties props):
  m_name("unknown"),
  m_props(props),
  m_zValue(0),
  m_id(0)
{

}

te::layout::ItemFactoryParamsCreate::~ItemFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::ItemFactoryParamsCreate::clone() const
{
  return new ItemFactoryParamsCreate(*this);
}

void te::layout::ItemFactoryParamsCreate::reset() throw(te::common::Exception)
{
  m_name.clear();
  m_zValue = 0;
  m_id = 0;
  m_props.clear();
}

std::string te::layout::ItemFactoryParamsCreate::getName()
{
  return m_name;
}

te::layout::Properties te::layout::ItemFactoryParamsCreate::getProperties()
{
  return m_props;
}

te::gm::Coord2D te::layout::ItemFactoryParamsCreate::getCoord()
{
  return m_coord;
}

int te::layout::ItemFactoryParamsCreate::getZValue()
{
  return m_zValue;
}

int te::layout::ItemFactoryParamsCreate::getId()
{
  return m_id;
}



