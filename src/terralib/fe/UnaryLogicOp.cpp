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
  \file UnaryLogicOp.cpp
  
  \brief A logical operator that can be used to combine one conditional expressions.
 */

// TerraLib
#include "AbstractOp.h"
#include "UnaryLogicOp.h"

// STL
#include <cassert>

te::fe::UnaryLogicOp::UnaryLogicOp(const char* opName)
  : LogicOp(opName),
    m_op(0)
{
}

te::fe::UnaryLogicOp::UnaryLogicOp(const char* opName, AbstractOp* o)
  : LogicOp(opName),
    m_op(o)
{
}

te::fe::UnaryLogicOp::~UnaryLogicOp()
{
  delete m_op;
}

void te::fe::UnaryLogicOp::setOp(AbstractOp* o)
{
  delete m_op;
  m_op = o;
}

te::fe::AbstractOp* te::fe::UnaryLogicOp::getOp() const
{
  return m_op;
}

