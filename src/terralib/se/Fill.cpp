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
  \file Fill.cpp
  
  \brief A Fill specifies the pattern for filling an area geometry.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "Fill.h"
#include "Graphic.h"
#include "SvgParameter.h"

te::se::Fill::Fill()
  : m_graphicFill(0)
{
}

te::se::Fill::~Fill()
{
  delete m_graphicFill;
  te::common::FreeContents(m_params);
}

void te::se::Fill::setGraphicFill(Graphic* g)
{
  delete m_graphicFill;
  m_graphicFill = g;
}

void te::se::Fill::add(SvgParameter* p)
{
  m_params.push_back(p);
}

