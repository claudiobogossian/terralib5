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
  \file terralib/cellspace/CellSet.cpp
   
  \brief implementation of a Cell Set.
*/

// TerraLib
#include "Cell.h"
#include "CellSet.h"

te::cellspace::CellSet::CellSet(double resX, double resY)
  : m_resX(resX),
    m_resY(resY)
{
}

te::cellspace::CellSet::~CellSet()
{

}

double te::cellspace::CellSet::getResX()
{
  return m_resX;
}

double te::cellspace::CellSet::getResY()
{
  return m_resY;
}

std::vector<te::cellspace::Cell> te::cellspace::CellSet::getCells()
{
  return m_cells;
}

void te::cellspace::CellSet::setResX(double resX)
{
  m_resX = resX;
}

void te::cellspace::CellSet::setResY(double resY)
{
  m_resY = resY;
}

void te::cellspace::CellSet::setCells(std::vector<te::cellspace::Cell> cells)
{
  m_cells = cells;
}

void te::cellspace::CellSet::addCell(te::cellspace::Cell cell)
{
  m_cells.push_back(cell);
}