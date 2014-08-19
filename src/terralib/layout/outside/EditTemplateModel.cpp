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
  \file EditTemplateModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EditTemplateModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"

te::layout::EditTemplateModel::EditTemplateModel() 
{
  //m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::EditTemplateModel::~EditTemplateModel()
{

}

te::layout::Properties* te::layout::EditTemplateModel::getProperties() const
{
  m_properties->clear();

  Property pro_name;
  pro_name.setName("PageSetup");

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::EditTemplateModel::updateProperties( te::layout::Properties* properties )
{

}