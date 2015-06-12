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
  \file terralib/rp/MixtureModelStrategy.cpp

  \brief Raster mixture model strategy base class.
*/

#include "MixtureModelStrategy.h"

te::rp::MixtureModelStrategy::MixtureModelStrategy()
{
}

te::rp::MixtureModelStrategy::~MixtureModelStrategy()
{
}

te::rp::MixtureModelStrategy::MixtureModelStrategy(const te::rp::MixtureModelStrategy&)
{
}

const te::rp::MixtureModelStrategy& te::rp::MixtureModelStrategy::operator=(const te::rp::MixtureModelStrategy&)
{
  return *this;
}
