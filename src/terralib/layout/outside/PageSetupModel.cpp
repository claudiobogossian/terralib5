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
  \file PageSetupModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PageSetupModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/Enums.h"

te::layout::PageSetupModel::PageSetupModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getPageSetup();
  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::PageSetupModel::~PageSetupModel()
{

}

te::layout::Properties* te::layout::PageSetupModel::getProperties() const
{
  m_properties->clear();

  Property pro_name(m_hashCode);
  pro_name.setName(m_name);

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::PageSetupModel::updateProperties( te::layout::Properties* properties )
{

}
