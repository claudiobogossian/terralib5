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
  \file terralib/cellspace/Cell.cpp
   
  \brief Implementation of a Cell.
*/

// Terralib
#include "Cell.h"

// STL
#include <string>

te::cellspace::Cell::Cell(int column, int row)
  : m_column(column),
    m_row(row)
{

}

te::cellspace::Cell::~Cell()
{

}

int te::cellspace::Cell::getColumn()
{
  return m_column;
}

int te::cellspace::Cell::getRow()
{
  return m_row;
}

void te::cellspace::Cell::setColumn(int column)
{
  m_column = column;
}

void te::cellspace::Cell::setRow(int row)
{
  m_row = row;
}