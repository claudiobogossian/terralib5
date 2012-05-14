/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/FeatureTypeStyle.cpp
  
  \brief The FeatureTypeStyle defines the styling that is to be applied to a dataset that can be viewed as a feature type.
*/

// TerraLib
#include "FeatureTypeStyle.h"

te::se::FeatureTypeStyle::FeatureTypeStyle()
  : m_featureTypeName(0)
{
}

te::se::FeatureTypeStyle::~FeatureTypeStyle()
{
  delete m_featureTypeName;
}

void te::se::FeatureTypeStyle::setFeatureTypeName(std::string* name)
{
  delete m_featureTypeName;
  m_featureTypeName = name;
}

const std::string* te::se::FeatureTypeStyle::getFeatureTypeName() const
{
  return m_featureTypeName;
}

